#include <karm-ui/input.h>
#include <karm-ui/view.h>

#include "app.h"

namespace Spreadsheet {

struct Table : public Ui::View<Table> {
    State const *_state;
    Math::Vec2i _scroll;
    Ui::MouseListener _mouseListener;

    Table(State const &state)
        : _state(&state) {}

    void reconcile(Table &o) override {
        _state = o._state;
    }

    Sheet const &sheet() {
        return _state->activeSheet();
    }

    /* --- Geometry --- */

    Math::Recti colHeaderBound(usize col) {
        return {
            sheet().cols[col].x,
            0,
            sheet().cols[col].width,
            CELL_HEIGHT,
        };
    }

    Math::Recti rowHeaderBound(usize row) {
        return {
            0,
            sheet().rows[row].y,
            CELL_WIDTH,
            sheet().rows[row].height,
        };
    }

    Math::Recti cellBound(usize row, usize col) {
        return {
            sheet().cols[col].x + CELL_WIDTH,
            sheet().rows[row].y + CELL_HEIGHT,
            sheet().cols[col].width,
            sheet().rows[row].height,
        };
    }

    /* --- Events --- */

    void event(Events::Event &e) override {
        e.handle<Events::MouseEvent>([&](Events::MouseEvent const &m) {
            auto pos = m.pos - bound().topStart();
            if (bound().contains(m.pos)) {
                if (m.type == Events::MouseEvent::PRESS) {
                    auto cell = sheet().cellAt(pos - Math::Vec2i{CELL_WIDTH, CELL_HEIGHT});
                    if (cell) {
                        Model::dispatch<UpdateSelection>(*this, Range{*cell});
                    }
                } else if (m.type == Events::MouseEvent::MOVE && ((m.buttons & Events::Button::LEFT) == Events::Button::LEFT)) {
                    auto cell = sheet().cellAt(pos - Math::Vec2i{CELL_WIDTH, CELL_HEIGHT});
                    if (cell) {
                        auto sel = *_state->selection;
                        sel.end = *cell;

                        if (Op::ne(*_state->selection, sel)) {
                            Model::dispatch<UpdateSelection>(*this, sel);
                        }
                    }
                }

                return true;
            }

            return false;
        });
    }

    /* --- Painting --- */

    void paintCell(Gfx::Context &g, Cell const &, Math::Recti bound) {
        g.rect(bound.cast<f64>());
    }

    void paintColHeader(Gfx::Context &g, usize idx) {
        auto col = sheet().cols[idx];
        auto bound = colHeaderBound(idx);
        auto sep = Math::Edgei{
            col.x + col.width - 1,
            0,
            col.x + col.width - 1,
            _bound.height,
        };

        g.fillStyle(Ui::GRAY800);
        g.fill(bound);
        g.debugLine(sep, Gfx::WHITE.withOpacity(0.05));
    }

    void paintRowHeader(Gfx::Context &g, usize idx) {
        auto row = sheet().rows[idx];
        Math::Recti bound = {0, row.y, CELL_WIDTH, row.height};
        Math::Edgei sep = {
            0,
            row.y + row.height - 1,
            _bound.width,
            row.y + row.height - 1,
        };

        g.fillStyle(Ui::GRAY800);
        g.fill(bound);
        g.debugLine(sep, Gfx::WHITE.withOpacity(0.05));
    }

    void paintSelection(Gfx::Context &g, Range r) {
        auto start = cellBound(r.start.row, r.start.col);
        auto end = cellBound(r.end.row, r.end.col);
        auto all = start.mergeWith(end);

        g.fillStyle(Ui::ACCENT600.withOpacity(0.2));
        g.begin();
        g.rect(all.cast<f64>().grow(3), 4);
        g.rect(start.cast<f64>().grow(3), 4);
        g.fill(Gfx::FillRule::EVENODD);

        g.strokeStyle(Gfx::stroke(Ui::ACCENT500).withAlign(Gfx::OUTSIDE_ALIGN).withWidth(2));
        g.stroke(all.grow(3), 4);

        auto handle = all.bottomEnd() + 3;
        g.fillStyle(Ui::ACCENT500);
        g.fill(Math::Ellipsei{handle, 4});

        g.strokeStyle(Gfx::stroke(Ui::GRAY950).withAlign(Gfx::OUTSIDE_ALIGN).withWidth(2));
        g.stroke();
    }

    void paint(Gfx::Context &g, Math::Recti) override {
        g.save();
        g.clip(bound());
        g.origin(bound().xy);

        // Draw columns.
        isize headerX = CELL_WIDTH;
        usize index = 0;
        while (headerX < _bound.width &&
               index < sheet().cols.len()) {

            auto col = sheet().cols[index];
            Math::Recti colBound = {headerX, 0, col.width, CELL_HEIGHT};

            g.fillStyle(Ui::GRAY800);
            g.fill(colBound);

            g.debugLine(Math::Edgei{headerX + col.width - 1, 0, headerX + col.width - 1, _bound.height}, Gfx::WHITE.withOpacity(0.05));

            headerX += col.width;
            index++;
        }

        // Draw rows.
        isize headerY = CELL_HEIGHT;
        index = 0;
        while (headerY < _bound.height &&
               index < sheet().rows.len()) {

            auto row = sheet().rows[index];
            Math::Recti rowBound = {0, headerY, CELL_WIDTH, row.height};

            g.fillStyle(Ui::GRAY800);
            g.fill(rowBound);

            g.debugLine(Math::Edgei{0, headerY + row.height - 1, _bound.width, headerY + row.height - 1}, Gfx::WHITE.withOpacity(0.05));

            headerY += row.height;
            index++;
        }

        if (_state->selection)
            paintSelection(g, *_state->selection);

        g.restore();
    }

    Math::Vec2i size(Math::Vec2i, Layout::Hint) override {
        return {100, 100};
    }
};

Ui::Child table(State const &s) {
    return makeStrong<Table>(s);
}

} // namespace Spreadsheet
