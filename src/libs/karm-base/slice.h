#pragma once

#include "iter.h"
#include "ordr.h"

namespace Karm {

using Byte = u8;

inline constexpr Byte operator"" _byte(unsigned long long arg) noexcept {
    return static_cast<Byte>(arg);
}

template <typename T, typename U = typename T::Inner>
concept Sliceable =
    requires(T const &t) {
        typename T::Inner;
        { t.len() } -> Meta::Same<usize>;
        { t.buf() } -> Meta::Same<U const *>;
        { t[0uz] } -> Meta::Same<U const &>;
    };

template <typename T, typename U = typename T::Inner>
concept MutSliceable =
    Sliceable<T, U> and
    requires(T &t) {
        { t.len() } -> Meta::Same<usize>;
        { t.buf() } -> Meta::Same<U *>;
        { t[0uz] } -> Meta::Same<U &>;
    };

template <typename T>
struct Slice {
    using Inner = T;

    T const *_buf{};
    usize _len{};

    constexpr Slice() = default;

    constexpr Slice(T const *buf, usize len)
        : _buf(buf), _len(len) {}

    constexpr Slice(T const *begin, T const *end)
        : Slice(begin, end - begin) {}

    constexpr Slice(Sliceable<T> auto const &other)
        : Slice(other.buf(), other.len()) {}

    constexpr T const &operator[](usize i) const {
        return _buf[i];
    }

    constexpr T const *buf() const { return _buf; }

    constexpr T const *begin() const { return _buf; }

    constexpr T const *end() const { return _buf + _len; }

    constexpr usize len() const { return _len; }
};

template <typename T>
struct MutSlice {
    using Inner = T;

    T *_buf{};
    usize _len{};

    constexpr MutSlice() = default;

    constexpr MutSlice(T *buf, usize len)
        : _buf(buf), _len(len) {}

    constexpr MutSlice(T *begin, T *end)
        : MutSlice(begin, end - begin) {}

    constexpr MutSlice(MutSliceable<T> auto &other)
        : MutSlice(other.buf(), other.len()) {}

    constexpr T &operator[](usize i) { return _buf[i]; }

    constexpr T const &operator[](usize i) const { return _buf[i]; }

    constexpr T *buf() { return _buf; }

    constexpr T const *buf() const { return _buf; }

    constexpr T *begin() { return _buf; }

    constexpr T *end() { return _buf + _len; }

    constexpr T const *begin() const { return _buf; }

    constexpr T const *end() const { return _buf + _len; }

    constexpr usize len() const { return _len; }
};

using Bytes = Slice<Byte>;

using MutBytes = MutSlice<Byte>;

template <Sliceable S, typename T = typename S::Inner>
Slice<T> sub(S &slice) {
    return {
        slice.buf(),
        slice.len(),
    };
}

template <Sliceable S, typename T = typename S::Inner>
Slice<T> sub(S &slice, usize start, usize end) {
    return {
        slice.buf() + start,
        clamp(end, start, slice.len()) - start,
    };
}

template <Sliceable S, typename T = typename S::Inner>
Slice<T> next(S &slice, usize start) {
    return sub(slice, start, slice.len());
}

template <MutSliceable S, typename T = typename S::Inner>
MutSlice<T> mutSub(S &slice, usize start, usize end) {
    return {
        slice.buf() + start,
        clamp(end, start, slice.len()) - start,
    };
}

template <MutSliceable S, typename T = typename S::Inner>
MutSlice<T> mutSub(S &slice) {
    return {
        slice.buf(),
        slice.len(),
    };
}

template <MutSliceable S, typename T = typename S::Inner>
MutSlice<T> mutNext(S &slice, usize start) {
    return mutSub(slice, start, slice.len());
}

template <Sliceable S>
Bytes bytes(S &slice) {
    return {
        reinterpret_cast<Byte const *>(slice.buf()),
        slice.len() * sizeof(typename S::Inner),
    };
}

template <MutSliceable S>
MutBytes mutBytes(S &slice) {
    return {
        reinterpret_cast<Byte *>(slice.buf()),
        slice.len() * sizeof(typename S::Inner),
    };
}

template <Sliceable S>
usize sizeOf(S &slice) {
    return slice.len() * sizeof(typename S::Inner);
}

template <Sliceable LHS, Sliceable RHS>
constexpr Ordr cmp(LHS const &lhs, RHS const &rhs) {
    return cmp(lhs.buf(), lhs.len(), rhs.buf(), rhs.len());
}

template <Sliceable S>
constexpr auto iter(S &slice) {
    return Iter([&slice, i = 0uz]() mutable -> typename S::Inner const * {
        if (i >= slice.len()) {
            return nullptr;
        }

        return &slice.buf()[i++];
    });
}

template <Sliceable S>
constexpr auto iterRev(S &slice) {
    return Iter([&slice, i = slice.len()]() mutable -> typename S::Inner const * {
        if (i == 0) {
            return nullptr;
        }

        return &slice.buf()[--i];
    });
}

template <MutSliceable S>
constexpr auto mutIter(S &slice) {
    return Iter([&slice, i = 0uz]() mutable -> typename S::Inner * {
        if (i >= slice.len()) {
            return nullptr;
        }

        return &slice.buf()[i++];
    });
}

template <MutSliceable S>
constexpr auto mutIterRev(S &slice) {
    return Iter([&slice, i = slice.len()]() mutable -> typename S::Inner * {
        if (i == 0) {
            return nullptr;
        }

        return &slice.buf()[--i];
    });
}

template <Sliceable S>
constexpr auto iterSplit(S &slice, typename S::Inner const &sep) {
    return Iter([&slice, sep, i = 0uz]() mutable -> Opt<Slice<typename S::Inner>> {
        if (i >= slice.len()) {
            return NONE;
        }

        usize start = i;
        while (i < slice.len() && slice.buf()[i] != sep) {
            i++;
        }

        usize end = i;
        if (i < slice.len()) {
            i++;
        }

        return Slice{
            slice.buf() + start,
            end - start,
        };
    });
}

template <Sliceable S>
constexpr auto iterLevelSplit(S &slice, typename S::Inner const &sep) {
    return Iter([&slice, sep, i = 0uz]() mutable -> Opt<Slice<typename S::Inner>> {
        if (i >= slice.len()) {
            return NONE;
        }

        while (i < slice.len() && slice.buf()[i] != sep) {
            i++;
        }

        usize end = i;
        if (i < slice.len()) {
            i++;
        }

        return Slice{
            slice.buf(),
            end,
        };
    });
}

template <Sliceable S>
constexpr auto iterSplitRev(S &slice, typename S::Inner const &sep) {
    return Iter([&slice, sep, i = slice.len()]() mutable -> Opt<Slice<typename S::Inner>> {
        if (i == 0) {
            return NONE;
        }

        usize end = i;
        while (i > 0 && slice.buf()[i - 1] != sep) {
            i--;
        }

        usize start = i;
        if (i > 0) {
            i--;
        }

        return Slice{
            slice.buf() + start,
            end - start,
        };
    });
}

template <Sliceable S>
constexpr auto iterLevelSplitRev(S &slice, typename S::Inner const &sep) {
    return Iter([&slice, sep, i = slice.len()]() mutable -> Opt<Slice<typename S::Inner>> {
        if (i == 0) {
            return NONE;
        }

        usize end = i;
        while (i > 0 && slice.buf()[i - 1] != sep) {
            i--;
        }

        if (i > 0) {
            i--;
        }

        return Slice{
            slice.buf(),
            end,
        };
    });
}

constexpr usize len(Sliceable auto &slice) {
    return slice.len();
}

constexpr bool isEmpty(Sliceable auto &slice) {
    return slice.len() == 0;
}

template <Sliceable S>
constexpr auto const *begin(S &slice) {
    return slice.buf();
}

template <Sliceable S>
constexpr auto const *end(S &slice) {
    return slice.buf() + slice.len();
}

template <MutSliceable S>
constexpr auto *begin(S &slice) {
    return slice.buf();
}

template <MutSliceable S>
constexpr auto *end(S &slice) {
    return slice.buf() + slice.len();
}

constexpr auto const &first(Sliceable auto &slice) {
    return slice.buf()[0];
}

constexpr auto &first(MutSliceable auto &slice) {
    return slice.buf()[0];
}

constexpr auto const &last(Sliceable auto &slice) {
    return slice.buf()[slice.len() - 1];
}

constexpr auto &last(MutSliceable auto &slice) {
    return slice.buf()[slice.len() - 1];
}

constexpr auto const &at(Sliceable auto &slice, usize i) {
    return slice.buf()[i];
}

constexpr auto &at(MutSliceable auto &slice, usize i) {
    return slice.buf()[i];
}

template <typename T>
constexpr usize move(MutSlice<T> src, MutSlice<T> dest) {
    usize l = min(len(src), len(dest));

    for (usize i = 0; i < l; i++) {
        at(dest, i) = std::move(at(src, i));
    }

    return l;
}

template <typename T>
constexpr usize copy(Slice<T> src, MutSlice<T> dest) {
    usize copied = 0;

    for (usize i = 0; i < min(len(src), len(dest)); i++) {
        at(dest, i) = at(src, i);
        copied++;
    }

    return copied;
}

template <typename T>
constexpr void reverse(MutSlice<T> slice) {
    for (usize i = 0; i < len(slice) / 2; i++) {
        std::swap(at(slice, i), at(slice, len(slice) - i - 1));
    }
}

template <typename T>
constexpr usize fill(MutSlice<T> slice, T value) {
    for (usize i = 0; i < len(slice); i++) {
        at(slice, i) = value;
    }

    return len(slice);
}

template <typename T>
constexpr usize zeroFill(MutSlice<T> slice) {
    return fill(slice, {});
}

ALWAYS_INLINE constexpr void sort(MutSliceable auto &slice, auto cmp) {
    if (len(slice) <= 1) {
        return;
    }

    auto pivot = at(slice, len(slice) / 2);
    auto left = 0uz;
    auto right = len(slice) - 1;

    while (left <= right) {
        while (cmp(at(slice, left), pivot).isLt()) {
            left++;
        }

        while (cmp(at(slice, right), pivot).isGt()) {
            right--;
        }

        if (left <= right) {
            std::swap(at(slice, left), at(slice, right));
            left++;
            right--;
        }
    }

    auto rightSlice = mutSub(slice, 0, right + 1);
    sort(rightSlice, cmp);

    auto leftSlice = mutSub(slice, left, len(slice));
    sort(leftSlice, cmp);
}

ALWAYS_INLINE Opt<usize> search(Sliceable auto const &slice, auto cmp) {
    if (len(slice) == 0) {
        return NONE;
    }

    if (cmp(slice[0]).isGt()) {
        return NONE;
    }

    if (cmp(slice[len(slice) - 1]).isLt()) {
        return NONE;
    }

    usize left = 0;
    usize right = len(slice) - 1;

    while (left <= right) {
        usize mid = (left + right) / 2;

        auto result = cmp(at(slice, mid));

        if (result.isEq()) {
            return mid;
        }

        if (result.isLt()) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return NONE;
}

} // namespace Karm
