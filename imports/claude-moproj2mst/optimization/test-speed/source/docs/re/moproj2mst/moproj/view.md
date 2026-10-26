# .moproj writer: ViewTransform (0x14009a640)

Packet moproj-view (MDT L2), 2026-10-22. Port: `apps/moproj2mst/src/moproj/write.cpp` (`viewCentre`),
test `moproj_view_centre_oracle_b` in `tests/moproj/test_moproj_write.cpp`. Decompiles:
`gui_mo/s1/moproj_vt/v0a.c .. v6.c`.

## What the GUI writes

`<ViewTransform><CenterX/><CenterY/>` is `mapToScene(viewport()->geometry().center())` of the
Operation Editor's QGraphicsView (SfDiagCellEditor), written with `QString::number(v, 'g', 6)`. No
zoom or centre is read back at open, so the value is the view state the GUI builds when it opens the
project.

## How that state is built

1. Scene rect, 0x14014f280: `(-25, -25, W + 25, H + 25)`, `W = max(7 * 205 + 25, view.width - 50)`,
   `H = max(lastRow.y + lastRow.h + 25 + 185, view.height - 50)` (row list 0x140157fe0).
2. FitAll, 0x140174120: `fitInView(itemsBoundingRect().adjusted(-40, -40, 20, 20), KeepAspectRatio)`
   (empty scene: `QRectF(0, 0, 205, 185)`). An item's extent is its base rect (0, 0, 180, h) adjusted by
   (-15, -15, 15, 15) at its cell, so its height is `itemRectHeight` of the layout (layout.md).
   Group items (SfOprCycle) add nothing (the Cycles2 saves match without them).
3. Qt 4.8: `fitInView` (margin 2), `recalculateContentSize` (scroll bar ranges with `q_round_bound`,
   centred indents otherwise), `centerOn(rect.center())` clamped by the ranges, `updateScroll`
   (`qint64(-leftIndent) + hbar`, `vbar - topIndent`) and `mapToScene(QPoint)`.

## Fresh-GUI profile (measured, coordinator 2026-10-22)

Oracle B on Sim2, 168 DPI: the main window is created hidden and maximised (client 3840x2036) before
any project opens. The view's frame is 3371x123 at screen (35, 1886) with a 1 px frame; viewport
121 px high; vertical scroll bar shown, no horizontal one. The bar measures 29 px but the centres need
PM_ScrollBarExtent = 30 (viewport 3339 wide, `width / 2 = 1669`); the measurement most likely lost
the bar's border pixel to the frame colour.

Checks that fix the profile (all 70 saves with the fresh profile, CenterX of the 1650 family):

* Every scale is `117 / (bh + 60)` with `bh` the items' bounding height: the fit is height-limited at
  a 121 px viewport (R46 bh 234, R47 523, CHAMF1 1480, Cogging Lab2 324, COG_P1 294).
* `CenterX * s - 1` is `ceil(s * (W - 25) / 2)` with `W = 3371 - 50` for every save.
* The vertical bar sits at its minimum `qRound(-25 * s)` (centerOn asks for less), and
  `CenterY * s = 61 + qRound(-25 * s)`: `int()` instead of `qRound` fails R46 and CHAMF1.

The other saves (R11, R19, the `_before` copies, B01..B20, 59..92) were made at other window sizes and
keep their input values in the writer test.

## Other window states

* A project whose primary database opens at open (124, 127..133, the DOE runs) has the view 175 px
  high (viewport 173); without it (125 no DB, 126 renamed DB, the corpus re-saves) 123. The same
  arithmetic with that height gives the centre (scale 169 / 354 for COG_P1_B1, vertical bar at
  qRound(-25 s) = -12, py = 87). `openProjectDb` (db_load.cpp) sets `Project::diagramViewHeight`;
  which widget the open database shows is not traced.
* The DOE runs' main sub-project (120, 121) has the same scale 169 / 354, but its CenterY
  (83.787, 40 px) is below the 173 px viewport's centre plus the bar's minimum: another view state,
  saved while the study is active after diagram edits; not reproduced.
* The `_before` copies of 120..126 and 122's save are COG_P1_B1 with the bar 26 px down (CenterY
  239.026): a scrolled view, not an open state.

## Limits

* `fitInView` first rescales the previous matrix to 1:1; the port starts from 1:1 exactly. The
  difference is ulp-level and does not reach six digits for any save.
* Item heights come from `views.expansion()`: GuiViews returns the expansion rows the open builds
  (0x140073900, L7), so the converter's own path computes the centre; the test runs through
  `guiViews()`.
