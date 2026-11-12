#pragma once
#include "ImageTypeBase.h"
#include "../ShapeType/Rect2.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>
#include <span>

namespace Core::ImageType
{
	// 픽셀마다 채널이 연속되는 배치이며 stride의 단위는 바이트다. 픽셀 메모리를 할당·해제하지 않는다.
	// T에 const를 지정할 수 있다. 뷰 객체 자체가 const여도 픽셀이 읽기 전용이 되는 것은 아니다.
	template <ImageElement T, uint32_t Channels> requires (Channels >= 1 && Channels <= 4)
		class ImageView
	{
	public:
		using value_type = std::remove_const_t<T>;
		using byte_type = std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>;

	private:
		T* m_data{};
		uint32_t m_width{};
		uint32_t m_height{};
		size_t m_stride{};

	public:
		ImageView() noexcept = default;

		// 다른 타입의 복사 생성자 호출을 금지한다.
		template <ImageElement U> requires (std::is_const_v<T>&& std::same_as<U, value_type>)
			ImageView(const ImageView<U, Channels>& source) noexcept
			: m_data(source.Data())
			, m_width(source.Width())
			, m_height(source.Height())
			, m_stride(source.StrideBytes())
		{
		}

		ImageView(const ImageView&) noexcept = default;
		ImageView(ImageView&&) noexcept = default;

		ImageView& operator=(const ImageView&) noexcept = default;
		ImageView& operator=(ImageView&&) noexcept = default;

		~ImageView() = default;

		// 외부 메모리를 이미지로 해석할 수 있는지 검사하고, 유효하면 그 메모리를 가리키는 ImageView 를 반환한다.
		static bool TryWrap(T* data, size_t bufferBytes, uint32_t width, uint32_t height, size_t stride, ImageView& result) noexcept
		{
			result = {};

			constexpr size_t pixelBytes = sizeof(T) * Channels;
			constexpr size_t limit = (std::numeric_limits<size_t>::max)();

			if (!data || width == 0 || height == 0 || width > limit / pixelBytes)
				return false;

			const size_t rowBytes = static_cast<size_t>(width) * pixelBytes;

			if (stride < rowBytes || stride % alignof(T) != 0 ||
				reinterpret_cast<uintptr_t>(data) % alignof(T) != 0)
				return false;

			if (height - 1 > (limit - rowBytes) / stride ||
				bufferBytes < static_cast<size_t>(height - 1) * stride + rowBytes)
				return false;

			result.m_data = data;
			result.m_width = width;
			result.m_height = height;
			result.m_stride = stride;

			return true;
		}

		static constexpr uint32_t Channel() noexcept { return Channels; }

		static constexpr PixelType GetPixelType() noexcept { return PixelTypeOf<T>; }

		bool IsInside(uint32_t x, uint32_t y) const noexcept { return x < m_width && y < m_height; }

		bool IsAligned(size_t alignment = 64) const noexcept
		{
			return !Empty() && alignment != 0 && (alignment & (alignment - 1)) == 0 &&
				reinterpret_cast<uintptr_t>(m_data) % alignment == 0 &&
				(m_height == 1 || m_stride % alignment == 0);
		}

		T* Data() const noexcept { return m_data; }

		uint32_t Width() const noexcept { return m_width; }

		uint32_t Height() const noexcept { return m_height; }

		size_t StrideBytes() const noexcept { return m_stride; }

		size_t RowBytes() const noexcept { return static_cast<size_t>(m_width) * Channels * sizeof(T); }

		bool Empty() const noexcept { return m_data == nullptr; }

		bool IsContinuous() const noexcept { return !Empty() && (m_height == 1 || m_stride == RowBytes()); }

		// Row/At은 빠른 접근용 함수다. 호출자는 유효 범위 내의 좌표를 전달해야 한다.
		T* Row(uint32_t y) const noexcept
		{
			assert(y < m_height);
			return reinterpret_cast<T*>(reinterpret_cast<byte_type*>(m_data) + static_cast<size_t>(y) * m_stride);
		}

		T& At(uint32_t x, uint32_t y, uint32_t channel = 0) const noexcept
		{
			assert(x < m_width && channel < Channels);
			return Row(y)[static_cast<size_t>(x) * Channels + channel];
		}

		std::span<T> RowSpan(uint32_t y) const noexcept
		{
			return { Row(y), static_cast<size_t>(m_width) * Channels };
		}

		std::span<T, Channels> Pixel(uint32_t x, uint32_t y) const noexcept
		{
			assert(x < m_width);
			return std::span<T, Channels>(Row(y) + static_cast<size_t>(x) * Channels, Channels);
		}

		ImageView ROI(const Core::ShapeType::Rect2i& rect) const noexcept
		{
			if (rect.left < 0 || rect.top < 0 || rect.Empty())
				return {};

			return ROI(
				static_cast<uint32_t>(rect.left),
				static_cast<uint32_t>(rect.top),
				static_cast<uint32_t>(rect.right - rect.left),
				static_cast<uint32_t>(rect.bottom - rect.top)
			);
		}

		// ROI 뷰는 같은 버퍼를 빌려 쓰며 소유자의 수명을 연장하지 않는다.
		// x 방향으로 이동한 ROI의 시작 주소는 64바이트 정렬이 아닐 수 있다.
		ImageView ROI(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const noexcept
		{
			if (Empty() || width == 0 || height == 0 || x >= m_width || y >= m_height ||
				width > m_width - x || height > m_height - y)
			{
				return {};
			}

			ImageView result = *this;

			result.m_data = Row(y) + static_cast<size_t>(x) * Channels;
			result.m_width = width;
			result.m_height = height;

			return result;
		}

		ImageView<const value_type, Channels> AsConst() const noexcept
		{
			ImageView<const value_type, Channels> result;

			if (!Empty())
			{
				ImageView<const value_type, Channels>::TryWrap(
					m_data,
					static_cast<size_t>(m_height - 1) * m_stride + RowBytes(),
					m_width,
					m_height,
					m_stride,
					result
				);
			}

			return result;
		}

		void Fill(value_type value) const noexcept requires (!std::is_const_v<T>)
		{
			for (uint32_t y = 0; y < m_height; ++y)
			{
				std::fill_n(Row(y), static_cast<size_t>(m_width) * Channels, value);
			}
		}
	};

	using ImageView_8U_C1 = ImageView<uint8_t, 1>;
	using ConstImageView_8U_C1 = ImageView<const uint8_t, 1>;
	using ImageView_8U_C2 = ImageView<uint8_t, 2>;
	using ConstImageView_8U_C2 = ImageView<const uint8_t, 2>;
	using ImageView_8U_C3 = ImageView<uint8_t, 3>;
	using ConstImageView_8U_C3 = ImageView<const uint8_t, 3>;
	using ImageView_8U_C4 = ImageView<uint8_t, 4>;
	using ConstImageView_8U_C4 = ImageView<const uint8_t, 4>;
	using ImageView_16U_C1 = ImageView<uint16_t, 1>;
	using ConstImageView_16U_C1 = ImageView<const uint16_t, 1>;
	using ImageView_16U_C2 = ImageView<uint16_t, 2>;
	using ConstImageView_16U_C2 = ImageView<const uint16_t, 2>;
	using ImageView_16U_C3 = ImageView<uint16_t, 3>;
	using ConstImageView_16U_C3 = ImageView<const uint16_t, 3>;
	using ImageView_16U_C4 = ImageView<uint16_t, 4>;
	using ConstImageView_16U_C4 = ImageView<const uint16_t, 4>;
	using ImageView_16S_C1 = ImageView<int16_t, 1>;
	using ConstImageView_16S_C1 = ImageView<const int16_t, 1>;
	using ImageView_16S_C2 = ImageView<int16_t, 2>;
	using ConstImageView_16S_C2 = ImageView<const int16_t, 2>;
	using ImageView_16S_C3 = ImageView<int16_t, 3>;
	using ConstImageView_16S_C3 = ImageView<const int16_t, 3>;
	using ImageView_16S_C4 = ImageView<int16_t, 4>;
	using ConstImageView_16S_C4 = ImageView<const int16_t, 4>;
	using ImageView_32F_C1 = ImageView<float, 1>;
	using ConstImageView_32F_C1 = ImageView<const float, 1>;
	using ImageView_32F_C2 = ImageView<float, 2>;
	using ConstImageView_32F_C2 = ImageView<const float, 2>;
	using ImageView_32F_C3 = ImageView<float, 3>;
	using ConstImageView_32F_C3 = ImageView<const float, 3>;
	using ImageView_32F_C4 = ImageView<float, 4>;
	using ConstImageView_32F_C4 = ImageView<const float, 4>;
	using ImageView_64F_C1 = ImageView<double, 1>;
	using ConstImageView_64F_C1 = ImageView<const double, 1>;
	using ImageView_64F_C2 = ImageView<double, 2>;
	using ConstImageView_64F_C2 = ImageView<const double, 2>;
	using ImageView_64F_C3 = ImageView<double, 3>;
	using ConstImageView_64F_C3 = ImageView<const double, 3>;
	using ImageView_64F_C4 = ImageView<double, 4>;
	using ConstImageView_64F_C4 = ImageView<const double, 4>;
}
