#pragma once

#include "anim.h"
#include "input.h"
#include "layout.h"
#include "view.h"

namespace Karm::Ui {

/* --- Drag Event ----------------------------------------------------------- */

struct DragEvent : public Events::BaseEvent<DragEvent> {
    enum _Type : u8 {
        START,
        DRAG,
        END
    };

    _Type type;
    Math::Vec2i delta;
};

/* --- Dismisable ----------------------------------------------------------- */

enum struct DismisDir {
    LEFT = 1 << 0,
    RIGHT = 1 << 1,
    TOP = 1 << 2,
    DOWN = 1 << 3,

    HORIZONTAL = LEFT | RIGHT,
    VERTICAL = TOP | DOWN,
};

FlagsEnum$(DismisDir);

using OnDismis = Func<void(Node &)>;

Child dismisable(OnDismis onDismis, DismisDir dir, f64 threshold, Ui::Child child);

inline auto dismisable(OnDismis onDismis, DismisDir dir, f64 threshold) {
    return [=, onDismis = std::move(onDismis)](Ui::Child child) mutable {
        return dismisable(std::move(onDismis), dir, threshold, child);
    };
}

/* --- Drag Region ---------------------------------------------------------- */

Child dragRegion(Child child);

inline auto dragRegion() {
    return [](Child child) {
        return dragRegion(child);
    };
}

/* --- Handle --------------------------------------------------------------- */

Child handle();

Child dragHandle();

Child buttonHandle(OnPress press);

} // namespace Karm::Ui
