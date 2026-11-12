#pragma once
#include <concepts>
#include <cstdint>
#include <type_traits>

namespace Core::ImageType
{
	// uint8_t, uint16_t, int16_t, float, double 타입만을 허용한다.
	template <class T>
	concept ImageScalar = std::same_as<T, uint8_t> || std::same_as<T, uint16_t> ||
		std::same_as<T, int16_t> || std::same_as<T, float> || std::same_as<T, double>;

	// T 의 const 제거 후 ImageScalar 타입에 적절한지 평가한다.
	template <class T>
	concept ImageElement = ImageScalar<std::remove_const_t<T>>;

	enum class PixelType : uint8_t
	{
		U8 = 0,
		U16 = 1,
		F32 = 2,
		S16 = 3,
		F64 = 4
	};

	// 원소 타입 T 에 대응하는 PixelType 값을 정한다.
	template <ImageElement T>
	inline constexpr PixelType PixelTypeOf = [] {

		// const 제거한 Scalar 가 각 타입과 일치하는지 조건 체크
		using Scalar = std::remove_const_t<T>;
		if constexpr (std::same_as<Scalar, uint8_t>) return PixelType::U8;
		else if constexpr (std::same_as<Scalar, uint16_t>) return PixelType::U16;
		else if constexpr (std::same_as<Scalar, int16_t>) return PixelType::S16;
		else if constexpr (std::same_as<Scalar, float>) return PixelType::F32;
		else return PixelType::F64;
	}();
}
