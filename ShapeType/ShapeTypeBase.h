#pragma once
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Core::ShapeType
{
    template <class T>
    concept ShapeScalar = std::same_as<T, int32_t> || std::same_as<T, uint32_t> ||
                          std::same_as<T, float> || std::same_as<T, double>;

    // Integer storage uses double for geometric calculations and results.
    template <ShapeScalar T> using ShapeReal = std::conditional_t<std::integral<T>, double, T>;
} // namespace Core::ShapeType
