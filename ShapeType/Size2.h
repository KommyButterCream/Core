#pragma once
#include "ShapeTypeBase.h"
#include <algorithm>
#include <cmath>

namespace Core::ShapeType
{
    template <ShapeScalar T> struct Size2
    {
      public:
        using value_type = T;
        using real_type = ShapeReal<T>;

        // < Constructors >
        constexpr Size2() noexcept = default;
        constexpr Size2(T width, T height) noexcept;

        Size2(const Size2 &) = default;
        Size2(Size2 &&) noexcept = default;

        Size2 &operator=(const Size2 &) = default;
        Size2 &operator=(Size2 &&) noexcept = default;

        ~Size2() = default;

        // < Members >
        T width{T{0}};
        T height{T{0}};

        // < Basic Operations >
        constexpr void Set(T newWidth, T newHeight) noexcept;

        constexpr void Reset() noexcept;

        // < Queries >
        real_type Area() const noexcept;

        real_type AspectRatio() const noexcept;

        bool IsEmpty() const noexcept; // width <= 0 || height <= 0

        bool IsValid() const noexcept; // width >= 0 && height >= 0

        // < Utilities >
        void ClampMin(T minValue) noexcept;

        void ClampMax(T maxValue) noexcept;

        bool EqualsEpsilon(const Size2 &rhs, T eps = static_cast<T>(1e-6)) const noexcept;

        // < Static >
        static constexpr Size2 Zero() noexcept;

        // < Operators >
        Size2 &operator+=(const Size2 &rhs) noexcept;

        Size2 &operator-=(const Size2 &rhs) noexcept;

        Size2 &operator*=(T scalar) noexcept;

        Size2 &operator/=(T scalar) noexcept;

        Size2 operator+(const Size2 &rhs) const noexcept;

        Size2 operator-(const Size2 &rhs) const noexcept;

        Size2 operator*(T scalar) const noexcept;

        Size2 operator/(T scalar) const noexcept;

        bool operator==(const Size2 &rhs) const noexcept;

        bool operator!=(const Size2 &rhs) const noexcept;
    };

    template <ShapeScalar T> constexpr Size2<T>::Size2(T width, T height) noexcept : width(width), height(height)
    {
    }

    template <ShapeScalar T> constexpr void Size2<T>::Set(T newWidth, T newHeight) noexcept
    {
        width = newWidth;
        height = newHeight;
    }

    template <ShapeScalar T> constexpr void Size2<T>::Reset() noexcept
    {
        width = T{0};
        height = T{0};
    }

    template <ShapeScalar T> auto Size2<T>::Area() const noexcept -> real_type
    {
        return static_cast<real_type>(width) * static_cast<real_type>(height);
    }

    template <ShapeScalar T> auto Size2<T>::AspectRatio() const noexcept -> real_type
    {
        return (height != T{0}) ? (static_cast<real_type>(width) / static_cast<real_type>(height)) : T{0};
    }

    template <ShapeScalar T> bool Size2<T>::IsEmpty() const noexcept
    {
        return width <= T{0} || height <= T{0};
    }

    template <ShapeScalar T> bool Size2<T>::IsValid() const noexcept
    {
        return width >= T{0} && height >= T{0};
    }

    template <ShapeScalar T> void Size2<T>::ClampMin(T minValue) noexcept
    {
        width = (std::max)(width, minValue);
        height = (std::max)(height, minValue);
    }

    template <ShapeScalar T> void Size2<T>::ClampMax(T maxValue) noexcept
    {
        width = (std::min)(width, maxValue);
        height = (std::min)(height, maxValue);
    }

    template <ShapeScalar T> bool Size2<T>::EqualsEpsilon(const Size2<T> &rhs, T eps) const noexcept
    {
        return std::abs(static_cast<real_type>(width) - static_cast<real_type>(rhs.width)) <= eps && std::abs(static_cast<real_type>(height) - static_cast<real_type>(rhs.height)) <= eps;
    }

    template <ShapeScalar T> constexpr Size2<T> Size2<T>::Zero() noexcept
    {
        return Size2<T>(T{0}, T{0});
    }

    template <ShapeScalar T> Size2<T> &Size2<T>::operator+=(const Size2<T> &rhs) noexcept
    {
        width += rhs.width;
        height += rhs.height;

        return *this;
    }

    template <ShapeScalar T> Size2<T> &Size2<T>::operator-=(const Size2<T> &rhs) noexcept
    {
        width -= rhs.width;
        height -= rhs.height;

        return *this;
    }

    template <ShapeScalar T> Size2<T> &Size2<T>::operator*=(T scalar) noexcept
    {
        width *= scalar;
        height *= scalar;

        return *this;
    }

    template <ShapeScalar T> Size2<T> &Size2<T>::operator/=(T scalar) noexcept
    {
        if (scalar != T{0})
        {
            width /= scalar;
            height /= scalar;
        }

        return *this;
    }

    template <ShapeScalar T> Size2<T> Size2<T>::operator+(const Size2<T> &rhs) const noexcept
    {
        return Size2<T>(width + rhs.width, height + rhs.height);
    }

    template <ShapeScalar T> Size2<T> Size2<T>::operator-(const Size2<T> &rhs) const noexcept
    {
        return Size2<T>(width - rhs.width, height - rhs.height);
    }

    template <ShapeScalar T> Size2<T> Size2<T>::operator*(T scalar) const noexcept
    {
        return Size2<T>(width * scalar, height * scalar);
    }

    template <ShapeScalar T> Size2<T> Size2<T>::operator/(T scalar) const noexcept
    {
        return (scalar != T{0}) ? Size2<T>(width / scalar, height / scalar) : Size2<T>();
    }

    template <ShapeScalar T> bool Size2<T>::operator==(const Size2<T> &rhs) const noexcept
    {
        return width == rhs.width && height == rhs.height;
    }

    template <ShapeScalar T> bool Size2<T>::operator!=(const Size2<T> &rhs) const noexcept
    {
        return !(*this == rhs);
    }

    using Size2i = Size2<int32_t>;
    using Size2u = Size2<uint32_t>;
    using Size2f = Size2<float>;
    using Size2d = Size2<double>;
} // namespace Core::ShapeType
