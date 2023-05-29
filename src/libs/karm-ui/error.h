#pragma once

#include <karm-fmt/case.h>
#include <karm-ui/box.h>
#include <karm-ui/layout.h>
#include <karm-ui/view.h>

namespace Karm::Ui {

static inline Child errorScope(Str what, auto callback) {
    Res<Child> child = callback();

    if (child) {
        return child.unwrap();
    }

    return box(
        {
            .padding = 12,
            .borderRadius = 6,
            .borderWidth = 1,
            .borderPaint = Gfx::RED700,
            .backgroundPaint = Gfx::RED900,
            .foregroundPaint = Gfx::RED100,
        },
        hflow(
            6,

            icon(Mdi::ALERT_DECAGRAM_OUTLINE, 26) | center(),
            vflow(
                4,
                text(TextStyle::titleMedium().withColor(Gfx::WHITE), "Can't display {}", what),
                text(Fmt::toSentenceCase(child.none().msg()).unwrap()))));
}

} // namespace Karm::Ui
