#pragma once
#include "ImageView.h"
#include <atomic>
#include <cstring>
#include <new>
#include <stdexcept>
#include <utility>

namespace Core::ImageType
{
	namespace Detail
	{
		// ROI 데이터 주소와 별도로 보관하며, 마지막 소유자가 실제 할당 원본을 해제한다.
		struct ImageBufferControl
		{
			std::atomic<size_t> m_references{1};
			void* m_allocation{};
			size_t m_bytes{};
		};
	}

	// 공유 소유권은 동일 모듈 내부에서만 사용한다. DLL API에는 ImageView를 전달한다.
	// atomic 참조 카운트는 서로 다른 핸들의 수명만 보호한다. 픽셀의 동시 쓰기와
	// 동일 Image 객체의 동시 대입·소멸에는 별도의 동기화가 필요하다.
	template <ImageElement T, uint32_t Channels> requires (Channels >= 1 && Channels <= 4)
	class Image
	{
	public:
		using value_type = std::remove_const_t<T>;
		using view_type = ImageView<T, Channels>;
		using const_view_type = ImageView<const value_type, Channels>;
		using const_image_type = Image<const value_type, Channels>;
		using mutable_image_type = Image<value_type, Channels>;

		static constexpr size_t Alignment = 64;

	private:
		template <ImageElement U, uint32_t N> requires (N >= 1 && N <= 4)
		friend class Image;

		view_type m_view{};
		Detail::ImageBufferControl* m_control{};

		Image(view_type view, Detail::ImageBufferControl* control) noexcept
			: m_view(view), m_control(view.Empty() ? nullptr : control)
		{
			AddRef();
		}

		void AddRef() noexcept
		{
			if (m_control)
				m_control->m_references.fetch_add(1, std::memory_order_relaxed);
		}

	public:
		Image() noexcept = default;

		Image(uint32_t width, uint32_t height) requires (!std::is_const_v<T>)
		{
			constexpr size_t limit = (std::numeric_limits<size_t>::max)();
			constexpr size_t pixelBytes = sizeof(T) * Channels;

			if (width == 0 || height == 0)
				throw std::invalid_argument("image dimensions must be positive");

			if (width > (limit - (Alignment - 1)) / pixelBytes)
				throw std::length_error("image row size overflow");

			const size_t rowBytes = static_cast<size_t>(width) * pixelBytes;
			const size_t stride = (rowBytes + Alignment - 1) & ~(Alignment - 1);

			if (height > limit / stride)
				throw std::length_error("image buffer size overflow");

			const size_t bytes = stride * height;
			auto* data = static_cast<T*>(::operator new(bytes, std::align_val_t{Alignment}));

			try
			{
				m_control = new Detail::ImageBufferControl;
			}
			catch (...)
			{
				::operator delete(data, std::align_val_t{Alignment});
				throw;
			}

			m_control->m_allocation = data;
			m_control->m_bytes = bytes;
			std::memset(data, 0, bytes);

			if (!view_type::TryWrap(data, bytes, width, height, stride, m_view))
			{
				ReleaseBuffer();

				throw std::length_error("invalid image layout");
			}
		}

		// 빌려 쓰는 버퍼의 배치를 검증하려면 버퍼 크기가 필요하다. 참조 카운트는 할당하지 않는다.
		Image(T* buffer, size_t bufferBytes, uint32_t width, uint32_t height, size_t stride)
		{
			if (!Attach(buffer, bufferBytes, width, height, stride))
				throw std::invalid_argument("invalid borrowed image layout");
		}

		Image(const Image& other) noexcept
			: m_view(other.m_view)
			, m_control(other.m_control)
		{
			AddRef();
		}

		Image(Image&& other) noexcept
			: m_view(std::exchange(other.m_view, {}))
			, m_control(std::exchange(other.m_control, nullptr))
		{
		}

		template <ImageElement U> requires (std::is_const_v<T> && std::same_as<U, value_type>)
		Image(const Image<U, Channels>& other) noexcept
			: m_view(other.m_view)
			, m_control(other.m_control)
		{
			AddRef();
		}

		Image& operator=(const Image& other) noexcept
		{
			Image copy(other);
			Swap(copy);
			return *this;
		}

		Image& operator=(Image&& other) noexcept
		{
			if (this != &other)
			{
				ReleaseBuffer();
				m_view = std::exchange(other.m_view, {});
				m_control = std::exchange(other.m_control, nullptr);
			}
			return *this;
		}

		~Image() noexcept { ReleaseBuffer(); }

		void Swap(Image& other) noexcept
		{
			std::swap(m_view, other.m_view);
			std::swap(m_control, other.m_control);
		}

		void ReleaseBuffer() noexcept
		{
			if (m_control && m_control->m_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				::operator delete(m_control->m_allocation, std::align_val_t{Alignment});
				delete m_control;
			}

			m_control = nullptr;
			m_view = {};
		}

		// 잘못된 배치나 할당 실패 시 기존 이미지 상태를 유지한다.
		bool Create(uint32_t width, uint32_t height) noexcept requires (!std::is_const_v<T>)
		{
			try
			{
				Image replacement(width, height);
				Swap(replacement);
				return true;
			}
			catch (const std::bad_alloc&) { return false; }
			catch (const std::length_error&) { return false; }
			catch (const std::invalid_argument&) { return false; }
		}

		bool Attach(T* buffer, size_t bufferBytes, uint32_t width, uint32_t height, size_t stride) noexcept
		{
			view_type view;

			if (!view_type::TryWrap(buffer, bufferBytes, width, height, stride, view))
				return false;

			// 자신이 소유한 버퍼를 다시 빌리면 기존 소유권 해제 시 포인터가 무효화될 수 있다.
			// 소유권을 유지하려면 ROI를 생성하거나 Image 핸들을 복사한다.
			if (m_control)
			{
				const auto base = reinterpret_cast<uintptr_t>(m_control->m_allocation);
				const auto address = reinterpret_cast<uintptr_t>(buffer);

				// 이 검사를 위해 제어 블록에 기록한 할당 크기를 사용한다.
				if (address >= base && address - base < m_control->m_bytes)
					return false;
			}

			Image replacement = Borrow(view);
			Swap(replacement);

			return true;
		}

		static Image Borrow(view_type view) noexcept
		{
			return Image(view, nullptr);
		}

		bool IsEmpty() const noexcept { return m_view.Empty(); }

		bool OwnsBuffer() const noexcept { return m_control != nullptr; }

		size_t UseCount() const noexcept
		{
			return m_control ? m_control->m_references.load(std::memory_order_relaxed) : 0;
		}

		uint32_t Width() const noexcept { return m_view.Width(); }

		uint32_t Height() const noexcept { return m_view.Height(); }

		size_t StrideBytes() const noexcept { return m_view.StrideBytes(); }

		size_t Stride() const noexcept { return StrideBytes(); }

		size_t RowBytes() const noexcept { return m_view.RowBytes(); }

		static constexpr uint32_t Channel() noexcept { return Channels; }

		static constexpr PixelType GetPixelType() noexcept { return PixelTypeOf<T>; }

		bool IsInside(uint32_t x, uint32_t y) const noexcept { return m_view.IsInside(x, y); }

		bool IsContinuous() const noexcept { return m_view.IsContinuous(); }

		bool IsAligned(size_t alignment = Alignment) const noexcept { return m_view.IsAligned(alignment); }

		T* Data() noexcept { return m_view.Data(); }

		const value_type* Data() const noexcept { return m_view.Data(); }

		T* Row(uint32_t y) noexcept { return m_view.Row(y); }

		const value_type* Row(uint32_t y) const noexcept { return m_view.Row(y); }

		T& At(uint32_t x, uint32_t y, uint32_t channel = 0) noexcept { return m_view.At(x, y, channel); }

		const value_type& At(uint32_t x, uint32_t y, uint32_t channel = 0) const noexcept
		{
			return m_view.At(x, y, channel);
		}

		std::span<T> RowSpan(uint32_t y) noexcept { return m_view.RowSpan(y); }

		std::span<const value_type> RowSpan(uint32_t y) const noexcept { return View().RowSpan(y); }

		std::span<T, Channels> Pixel(uint32_t x, uint32_t y) noexcept { return m_view.Pixel(x, y); }

		std::span<const value_type, Channels> Pixel(uint32_t x, uint32_t y) const noexcept
		{
			return View().Pixel(x, y);
		}

		view_type View() & noexcept { return m_view; }

		const_view_type View() const & noexcept { return m_view.AsConst(); }

		view_type View() && = delete;

		const_view_type View() const && = delete;

		Image ROI(uint32_t x, uint32_t y, uint32_t width, uint32_t height) noexcept
		{
			return Image(m_view.ROI(x, y, width, height), m_control);
		}

		const_image_type ROI(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const noexcept
		{
			return const_image_type(m_view.AsConst().ROI(x, y, width, height), m_control);
		}

		Image ROI(const Core::ShapeType::Rect2i& rect) noexcept
		{
			return Image(m_view.ROI(rect), m_control);
		}

		const_image_type ROI(const Core::ShapeType::Rect2i& rect) const noexcept
		{
			return const_image_type(m_view.AsConst().ROI(rect), m_control);
		}

		void Fill(value_type value) noexcept requires (!std::is_const_v<T>) { m_view.Fill(value); }

		mutable_image_type Clone() const { return Clone(m_view.AsConst()); }

		static mutable_image_type Clone(const_view_type source)
		{
			if (source.Empty())
				return {};

			mutable_image_type result(source.Width(), source.Height());
			for (uint32_t y = 0; y < source.Height(); ++y)
				std::memcpy(result.Row(y), source.Row(y), source.RowBytes());

			return result;
		}

		// 실제 픽셀만 복사하고 대상의 패딩은 유지한다. ROI가 겹치면 임시 복사본을
		// 사용한다. 할당 예외는 호출 모듈 내부에서 처리해야 한다.
		bool CopyTo(ImageView<value_type, Channels> destination) const
		{
			if (destination.Width() != Width() || destination.Height() != Height())
				return false;

			if (IsEmpty())
				return destination.Empty();

			if (destination.Data() == Data() && destination.StrideBytes() == StrideBytes())
				return true;

			const auto sourceAddress = reinterpret_cast<uintptr_t>(Data());
			const auto destinationAddress = reinterpret_cast<uintptr_t>(destination.Data());
			const size_t sourceBytes = static_cast<size_t>(Height() - 1) * StrideBytes() + RowBytes();
			const size_t destinationBytes = static_cast<size_t>(Height() - 1) * destination.StrideBytes() + RowBytes();
			const bool overlaps = sourceAddress <= destinationAddress
				? destinationAddress - sourceAddress < sourceBytes
				: sourceAddress - destinationAddress < destinationBytes;

			mutable_image_type snapshot;

			auto source = m_view.AsConst();
			if (overlaps)
			{
				snapshot = Clone();
				source = snapshot.View();
			}

			for (uint32_t y = 0; y < Height(); ++y)
				std::memcpy(destination.Row(y), source.Row(y), RowBytes());

			return true;
		}
	};

	using Image_8U_C1 = Image<uint8_t, 1>;
	using Image_8U_C2 = Image<uint8_t, 2>;
	using Image_8U_C3 = Image<uint8_t, 3>;
	using Image_8U_C4 = Image<uint8_t, 4>;
	using Image_16U_C1 = Image<uint16_t, 1>;
	using Image_16U_C2 = Image<uint16_t, 2>;
	using Image_16U_C3 = Image<uint16_t, 3>;
	using Image_16U_C4 = Image<uint16_t, 4>;
	using Image_16S_C1 = Image<int16_t, 1>;
	using Image_16S_C2 = Image<int16_t, 2>;
	using Image_16S_C3 = Image<int16_t, 3>;
	using Image_16S_C4 = Image<int16_t, 4>;
	using Image_32F_C1 = Image<float, 1>;
	using Image_32F_C2 = Image<float, 2>;
	using Image_32F_C3 = Image<float, 3>;
	using Image_32F_C4 = Image<float, 4>;
	using Image_64F_C1 = Image<double, 1>;
	using Image_64F_C2 = Image<double, 2>;
	using Image_64F_C3 = Image<double, 3>;
	using Image_64F_C4 = Image<double, 4>;
}
