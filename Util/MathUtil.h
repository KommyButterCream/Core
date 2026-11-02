#pragma once

#include <type_traits>

// 각도 변환과 수학 상수.
//
// 매크로 대신 constexpr 함수를 쓴다. 타입 안전하고, 디버거에서 값을 볼 수 있고,
// 인자를 두 번 평가하지 않는다.

namespace Core
{
	namespace Util
	{
		// long double 리터럴로 두고 T 로 좁힌다.
		// float 로 인스턴스화해도 double 정밀도에서 반올림된 값이 들어간다.
		template <typename T>
		inline constexpr T kPi = static_cast<T>(3.14159265358979323846L);

		template <typename T>
		inline constexpr T kTwoPi = static_cast<T>(6.28318530717958647692L);

		template <typename T>
		inline constexpr T kHalfPi = static_cast<T>(1.57079632679489661923L);

		template <typename T>
		constexpr T DegToRad(T degrees) noexcept
		{
			static_assert(std::is_floating_point_v<T>,
				"DegToRad requires a floating point type");

			return degrees * (kPi<T> / static_cast<T>(180));
		}

		template <typename T>
		constexpr T RadToDeg(T radians) noexcept
		{
			static_assert(std::is_floating_point_v<T>,
				"RadToDeg requires a floating point type");

			return radians * (static_cast<T>(180) / kPi<T>);
		}
	}
}
