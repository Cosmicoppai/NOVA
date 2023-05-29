#pragma once

#include "flow.h"
#include "size.h"

namespace Karm::Layout {

struct Align {
    static constexpr u16 NONE = (0);

    static constexpr u16 START = (1 << 0);
    static constexpr u16 TOP = (1 << 1);
    static constexpr u16 END = (1 << 2);
    static constexpr u16 BOTTOM = (1 << 3);
    static constexpr u16 VSTRETCH = (1 << 4);
    static constexpr u16 HSTRETCH = (1 << 5);
    static constexpr u16 VCENTER = (1 << 6);
    static constexpr u16 HCENTER = (1 << 7);
    static constexpr u16 COVER = (1 << 8);
    static constexpr u16 FIT = (1 << 9);
    static constexpr u16 HCLAMP = (1 << 10);
    static constexpr u16 VCLAMP = (1 << 11);

    static constexpr u16 STRETCH = (HSTRETCH | VSTRETCH);
    static constexpr u16 VFILL = (VSTRETCH | TOP);
    static constexpr u16 HFILL = (HSTRETCH | START);
    static constexpr u16 CENTER = (HCENTER | VCENTER);
    static constexpr u16 FILL = (VFILL | HFILL);
    static constexpr u16 CLAMP = (HCLAMP | VCLAMP);

    u16 _value = 0;

    Align(u16 value = 0) : _value(value) {}

    template <typename T>
    Math::Rect<T> apply(Flow flow, Math::Rect<T> inner, Math::Rect<T> outer) {
        if (_value == NONE)
            return outer;

        if (_value & COVER)
            inner = inner.cover(outer);

        if (_value & FIT)
            inner = inner.fit(outer);

        if (_value & START)
            inner = flow.setX(inner, flow.getStart(outer));

        if (_value & TOP)
            inner = flow.setY(inner, flow.getTop(outer));

        if (_value & END)
            inner = flow.setX(inner, flow.getEnd(outer) - flow.getWidth(inner));

        if (_value & BOTTOM)
            inner = flow.setY(inner, flow.getBottom(outer) - flow.getHeight(inner));

        if (_value & HCLAMP)
            inner = flow.setWidth(inner, clamp(flow.getWidth(inner), 0, flow.getWidth(outer)));

        if (_value & VCLAMP)
            inner = flow.setHeight(inner, clamp(flow.getHeight(inner), 0, flow.getHeight(outer)));

        if (_value & HSTRETCH)
            inner = flow.setWidth(inner, flow.getWidth(outer));

        if (_value & VSTRETCH)
            inner = flow.setHeight(inner, flow.getHeight(outer));

        if (_value & HCENTER)
            inner = flow.setX(inner, flow.getHcenter(outer) - flow.getWidth(inner) / 2);

        if (_value & VCENTER)
            inner = flow.setY(inner, flow.getVcenter(outer) - flow.getHeight(inner) / 2);

        return inner;
    }

    template <typename T>
    Math::Vec2<T> size(Math::Vec2<T> inner, Math::Vec2<T> outer, Layout::Hint hint) {
        if (_value & COVER) {
            if (hint == Layout::Hint::MIN)
                inner = {};
            else
                inner = outer;
        }

        if (_value & HSTRETCH and hint == Layout::Hint::MAX)
            inner.x = outer.x;

        if (_value & VSTRETCH and hint == Layout::Hint::MAX)
            inner.y = outer.y;

        return inner;
    }
};

} // namespace Karm::Layout
