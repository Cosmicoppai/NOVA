#pragma once

#include <karm-base/clamp.h>
#include <karm-fmt/fmt.h>

#include "funcs.h"

namespace Karm::Math {

template <typename T>
union Vec2 {
    using Scalar = T;

    struct {
        T x, y;
    };

    struct {
        T width, height;
    };

    Array<T, 2> _els;

    constexpr Vec2()
        : _els{} {}

    constexpr Vec2(T x, T y)
        : _els{x, y} {}

    constexpr Vec2(T value)
        : _els{value, value} {}

    constexpr T min() const {
        return Karm::min(x, y);
    }

    constexpr T max() const {
        return Karm::max(x, y);
    }

    constexpr Vec2 min(Vec2 other) const {
        return {Karm::min(x, other.x), Karm::min(y, other.y)};
    }

    constexpr Vec2 max(Vec2 other) const {
        return {Karm::max(x, other.x), Karm::max(y, other.y)};
    }

    constexpr T dot(Vec2 other) const {
        return x * other.x + y * other.y;
    }

    constexpr T cross(Vec2 other) const {
        return x * other.y - y * other.x;
    }

    constexpr T len() const {
        return sqrt(dot(*this));
    }

    constexpr T lenSq() const {
        return dot(*this);
    }

    constexpr T dist(Vec2 other) const {
        return (*this - other).len();
    }

    constexpr Vec2 norm() const {
        return Vec2{x, y} / len();
    }

    constexpr T angle() const {
        return Math::atan2(y, x);
    }

    constexpr Vec2 rotate(T angle) const {
        T c = cos(angle);
        T s = sin(angle);
        return {x * c - y * s, x * s + y * c};
    }

    constexpr T angleWith(Vec2 other) const {
        auto r = norm().dot(other.norm());
        auto sign = (x * other.y < y * other.x) ? -1.0 : 1.0;
        return sign * acos(r);
    }

    constexpr T operator[](isize i) const {
        return _els[i];
    }

    constexpr Vec2 operator+(T const &other) const {
        return {x + other, y + other};
    }

    constexpr Vec2 operator-(T const &other) const {
        return {x - other, y - other};
    }

    constexpr Vec2 operator*(T const &other) const {
        return {x * other, y * other};
    }

    constexpr Vec2 operator/(T const &other) const {
        return {x / other, y / other};
    }

    template <typename U>
    constexpr auto operator+(Vec2<U> const &other) const
        -> Vec2<decltype(T{} + U{})> {
        return {x + other.x, y + other.y};
    }

    template <typename U>
    constexpr auto operator-(Vec2<U> const &other) const
        -> Vec2<decltype(T{} - U{})> {
        return {x - other.x, y - other.y};
    }

    template <typename U>
    constexpr auto operator*(Vec2<U> const &other) const
        -> Vec2<decltype(T{} * U{})> {
        return {x * other.x, y * other.y};
    }

    template <typename U>
    constexpr auto operator/(Vec2<U> const &other) const
        -> Vec2<decltype(T{} / U{})> {
        return {x / other.x, y / other.y};
    }

    constexpr Vec2 operator-() const {
        return {-x, -y};
    }

    template <typename U>
    constexpr Vec2<U> cast() const {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    constexpr Ordr cmp(Vec2 other) const {
        return ::cmp(x, other.x) | ::cmp(y, other.y);
    }

    bool hasNan() const {
        return isNan(x) or isNan(y);
    }
};

template <typename T>
Vec2<T> operator+(T const &lhs, Vec2<T> const &rhs) {
    return rhs + lhs;
}

template <typename T>
Vec2<T> operator-(T const &lhs, Vec2<T> const &rhs) {
    return rhs - lhs;
}

template <typename T>
Vec2<T> operator*(T const &lhs, Vec2<T> const &rhs) {
    return rhs * lhs;
}

template <typename T>
Vec2<T> operator/(T const &lhs, Vec2<T> const &rhs) {
    return rhs / lhs;
}

using Vec2i = Vec2<isize>;

using Vec2u = Vec2<usize>;

using Vec2f = Vec2<f64>;

template <typename T>
bool epsilonEq(Vec2<T> const &lhs, Vec2<T> const &rhs, T epsilon) {
    return epsilonEq(lhs.x, rhs.x, epsilon) and
           epsilonEq(lhs.y, rhs.y, epsilon);
}

template <typename T>
union Vec3 {
    using Scalar = T;

    struct {
        T x, y, z;
    };

    struct {
        Vec2<T> xy;
        T _z;
    };

    struct {
        T _x;
        Vec2<T> yz;
    };

    Array<T, 3> _els;

    constexpr Vec3()
        : _els{} {}

    constexpr Vec3(T x, T y, T z)
        : _els{x, y, z} {}

    constexpr Vec3(T value)
        : _els{value, value, value} {}

    constexpr Vec2<T> xz() const {
        return {x, y};
    }

    constexpr T min() const {
        return Karm::min(x, y, z);
    }

    constexpr T max() const {
        return Karm::max(x, y, z);
    }

    constexpr Vec3 min(Vec3 const &other) {
        return {
            Karm::min(x, other.x),
            Karm::min(y, other.y),
            Karm::min(z, other.z),
        };
    }

    constexpr Vec3 max(Vec3<T> const &other) {
        return {
            Karm::max(x, other.x),
            Karm::max(y, other.y),
            Karm::max(z, other.z),
        };
    }

    constexpr T dot(Vec3 const &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr T lenSq() const {
        return dot(*this);
    }

    constexpr T len() const {
        return sqrt(lenSq());
    }

    constexpr T dist(Vec3 const &other) const {
        return (*this - other).len();
    }

    constexpr Vec3 norm() const {
        T l = len();
        return {x / l, y / l, z / l};
    }

    constexpr T operator[](isize i) {
        return _els[i];
    }

    constexpr Vec3 operator+(T const &other) const {
        return {x + other, y + other, z + other};
    }

    constexpr Vec3 operator-(T const &other) const {
        return {x - other, y - other, z - other};
    }

    constexpr Vec3 operator*(T const &other) const {
        return {x * other, y * other, z * other};
    }

    constexpr Vec3 operator/(T const &other) const {
        return {x / other, y / other, z / other};
    }

    template <typename U>
    constexpr auto operator+(Vec3<U> const &other) const
        -> Vec3<decltype(T{} + U{})> {
        return {x + other.x, y + other.y, z + other.z};
    }

    template <typename U>
    constexpr auto operator-(Vec3<U> const &other) const
        -> Vec3<decltype(T{} - U{})> {
        return {x - other.x, y - other.y, z - other.z};
    }

    template <typename U>
    constexpr auto operator*(Vec3<U> const &other) const
        -> Vec3<decltype(T{} * U{})> {
        return {x * other.x, y * other.y, z * other.z};
    }

    template <typename U>
    constexpr auto operator/(Vec3<U> const &other) const
        -> Vec3<decltype(T{} / U{})> {
        return {x / other.x, y / other.y, z / other.z};
    }

    constexpr Vec3 operator-() const {
        return {-x, -y, -z};
    }

    template <typename U>
    constexpr Vec3<U> cast() const {
        return {
            static_cast<U>(x),
            static_cast<U>(y),
            static_cast<U>(z),
        };
    }

    constexpr Ordr cmp(Vec3 other) const {
        return ::cmp(x, other.x) | ::cmp(y, other.y) | ::cmp(z, other.z);
    }

    bool hasNan() const {
        return isNan(x) or isNan(y) or isNan(z);
    }
};

template <typename T>
constexpr Vec3<T> operator+(T const &lhs, Vec3<T> const &rhs) {
    return {lhs + rhs.x, lhs + rhs.y, lhs + rhs.z};
}

template <typename T>
constexpr Vec3<T> operator-(T const &lhs, Vec3<T> const &rhs) {
    return {lhs - rhs.x, lhs - rhs.y, lhs - rhs.z};
}

template <typename T>
constexpr Vec3<T> operator*(T const &lhs, Vec3<T> const &rhs) {
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
}

template <typename T>
constexpr Vec3<T> operator/(T const &lhs, Vec3<T> const &rhs) {
    return {lhs / rhs.x, lhs / rhs.y, lhs / rhs.z};
}

using Vec3i = Vec3<isize>;

using Vec3u = Vec3<usize>;

using Vec3f = Vec3<f64>;

template <typename T>
bool epsilonEq(Vec3<T> const &lhs, Vec3<T> const &rhs, T epsilon) {
    return epsilonEq(lhs.x, rhs.x, epsilon) and
           epsilonEq(lhs.y, rhs.y, epsilon) and
           epsilonEq(lhs.z, rhs.z, epsilon);
}

template <typename T>
union Vec4 {
    using Scalar = T;

    struct {
        T x, y, z, w;
    };

    struct {
        Vec3<T> xyz;
        T _w;
    };

    struct {
        Vec2<T> xy;
        Vec2<T> zw;
    };

    Array<T, 4> _els;

    constexpr Vec4()
        : _els{} {}

    constexpr Vec4(T x, T y, T z, T w)
        : _els{x, y, z, w} {}

    constexpr Vec4(T value)
        : _els{value, value, value, value} {}

    constexpr T min() const {
        return min(x, y, z, w);
    }

    constexpr T max() const {
        return max(x, y, z, w);
    }

    constexpr Vec4 min(Vec4 const &other) {
        return {
            min(x, other.x),
            min(y, other.y),
            min(z, other.z),
            min(w, other.w),
        };
    }

    constexpr Vec4 max(Vec4 const &other) {
        return {
            max(x, other.x),
            max(y, other.y),
            max(z, other.z),
            max(w, other.w),
        };
    }

    constexpr T dot(Vec4 const &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    constexpr T len() const {
        return sqrt(dot(*this));
    }

    constexpr T dist(Vec4 const &other) const {
        return (*this - other).len();
    }

    constexpr Vec4 norm() const {
        T l = len();
        return {x / l, y / l, z / l, w / l};
    }

    constexpr T operator[](isize i) {
        return _els[i];
    }

    constexpr Vec4 operator+(T const &other) const {
        return {x + other, y + other, z + other, w + other};
    }

    constexpr Vec4 operator-(T const &other) const {
        return {x - other, y - other, z - other, w - other};
    }

    constexpr Vec4 operator*(T const &other) const {
        return {x * other, y * other, z * other, w * other};
    }

    constexpr Vec4 operator/(T const &other) const {
        return {x / other, y / other, z / other, w / other};
    }

    template <typename U>
    constexpr auto operator+(Vec4<U> const &other) const
        -> Vec4<decltype(T{} + U{})> {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    template <typename U>
    constexpr auto operator-(Vec4<U> const &other) const
        -> Vec4<decltype(T{} - U{})> {
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    }

    template <typename U>
    constexpr auto operator*(Vec4<U> const &other) const
        -> Vec4<decltype(T{} * U{})> {
        return {x * other.x, y * other.y, z * other.z, w * other.w};
    }

    template <typename U>
    constexpr auto operator/(Vec4<U> const &other) const
        -> Vec4<decltype(T{} / U{})> {
        return {x / other.x, y / other.y, z / other.z, w / other.w};
    }

    constexpr Vec4 operator-() const {
        return {-x, -y, -z, -w};
    }

    template <typename U>
    constexpr Vec4<U> cast() const {
        return {
            static_cast<U>(x),
            static_cast<U>(y),
            static_cast<U>(z),
            static_cast<U>(w),
        };
    }

    bool hasNan() const {
        return isNan(x) or isNan(y) or isNan(z) or isNan(w);
    }

    Ordr cmp(Vec4 other) const {
        return ::cmp(x, other.x) | ::cmp(y, other.y) | ::cmp(z, other.z) | ::cmp(w, other.w);
    }
};

template <typename T>
constexpr Vec4<T> operator+(T const &lhs, Vec4<T> const &rhs) {
    return {lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w};
}

template <typename T>
constexpr Vec4<T> operator-(T const &lhs, Vec4<T> const &rhs) {
    return {lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w};
}

template <typename T>
constexpr Vec4<T> operator*(T const &lhs, Vec4<T> const &rhs) {
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w};
}

template <typename T>
constexpr Vec4<T> operator/(T const &lhs, Vec4<T> const &rhs) {
    return {lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w};
}

using Vec4i = Vec4<isize>;

using Vec4u = Vec4<usize>;

using Vec4f = Vec4<f64>;

template <typename T>
bool epsilonEq(Vec4<T> const &lhs, Vec4<T> const &rhs, T epsilon) {
    return epsilonEq(lhs.x, rhs.x, epsilon) and
           epsilonEq(lhs.y, rhs.y, epsilon) and
           epsilonEq(lhs.z, rhs.z, epsilon) and
           epsilonEq(lhs.w, rhs.w, epsilon);
}

} // namespace Karm::Math

template <typename T>
struct Karm::Fmt::Formatter<Math::Vec2<T>> {
    Res<usize> format(Io::TextWriter &writer, Math::Vec2<T> vec) {
        return Fmt::format(writer, "Vec2({}, {})", vec.x, vec.y);
    }
};

template <typename T>
struct Karm::Fmt::Formatter<Math::Vec3<T>> {
    Res<usize> format(Io::TextWriter &writer, Math::Vec3<T> vec) {
        return Fmt::format(writer, "Vec3({}, {}, {})", vec.x, vec.y, vec.z);
    }
};

template <typename T>
struct Karm::Fmt::Formatter<Math::Vec4<T>> {
    Res<usize> format(Io::TextWriter &writer, Math::Vec4<T> vec) {
        return Fmt::format(writer, "Vec4({}, {}, {}, {})", vec.x, vec.y, vec.z);
    }
};
