# .moproj writer: diagram scene layout (RectF of a DiagramItem)

Packet moproj, 2026-10-22. Port: `apps/moproj2mst/src/moproj/write.cpp` (`sceneRect`), RectF read at
open in `src/model/subproject.cpp` (`readRectF`).

## What the saved RectF is

`<RectF>` is the model item's rectangle (vt+0x98, item +0x60 from the ISfDiagItem base). Open reads
it from the file (0x140097d00 through QRectF::setX / setY / setWidth / setHeight, then vt+0x90). The
cell editor (SfDiagCellEditor) then lays the chart out: 0x1401583e0 calls 0x14016d9f0 for every cell
item, which does `setPos` on the graphics item and `moveTo(x, y)` on the model rectangle, so width and
height stay as read (0 in every saved file). Group items (SfOprCycle) have no cell and keep the
rectangle read at open.

## Geometry

* Editor ctor 0x14014dc10: cell width 205 (+0x00), row height 185 (+0x04), origin (0, 0) (+0x08,
  reset by 0x14014e7b0), 7 columns (+0x28).
* Column x, 0x14014e2e0: `col * 205 + origin.x`.
* Row list, 0x140157fe0: `y = origin.y`; per row `iy = (int)y`, row.y = iy, row.h = 185, and for every
  cell with an item `row.h = max(row.h, itemRect.height + 25)`. When a cell of the row carries a group
  connector (cell +0x08, set by 0x1401574f0 for the cells from the group's begin to its end cell,
  row-major over 7 columns) `row.y += 20` and `iy += 20`. Next `y = iy + row.h`.
* Row y, 0x140159ab0: row.y from the list (past its end: last y + h + (row - n) * 185).

Rows below row 0 take the heights of the graphics items of the rows above (next section).

## Item height, 0x1401637b0 (ported: `itemRectHeight`)

* Base rect 0x14016cab0: font = QApplication::font() family at 8 pt; `H0 = max(2 * fontHeight,
  iconHeight (pixmap 30x30), ports) + 8 + 4`, ports = n * 8 + (n - 1) * 4 with n = 0 or 1 (vt+0x40,
  vt+0x48); `H0 = max(H0, 60)`, `H = ceil(H0 / 30) * 30`; width 180.
* Graphics flags: +0x60 expanded is 1 from creation (0x140162340); only the user changes it (toggle
  slot 0x14017a3c0, paste 0x140173170), so after open every item is laid out expanded, whatever the
  model's Expanded flag. Data +0x110 = (Operation_Id < 0).
* Expanded: data +0x110 set: 160. Empty expansion list: H + 200. With expansions (0x140163e50):
  start at H; Text and Space 15 each, Line 3; height = last bottom + 30. (Not expanded would be H + 5.)
* 0x14016d900 sets `rect.adjusted(-15, -15, 15, 15)`, so the graphics rect is 30 taller.
* Fresh-GUI profile: font height 13 (MS Shell Dlg 2 8 pt at 96 DPI, GDI-measured on Sim2 by the
  coordinator; the exe has no dpiAware entry, so 96 DPI is inferred) and icon height 30 (upper
  bound of QIcon::pixmap(30, 30)). H = 60 for any font height up to 24.
* Oracle B check: R47F / R50F rows 274 apart (Boolean: 8 Text/Space + 3 Line = 129;
  60 + 129 + 30 + 30 + 25) and CHAMF1 319 (Cogging, 174). The moproj test passes R47F and R50F with
  RectF from the port.

## Expanded attribute (ported: `savedExpanded`)

* Saved `Expanded` is the model item's byte at primary +0x80 (ISfDiagItem vt+0xa8, -0x58 from the
  secondary base at +0xd8). Open compares the attribute and drops the result (0x140091f00).
* The SfDiagItem ctor 0x14015ef80 never initialises +0x80 (heap memory from operator new).
* The cell editor's item-list virtual 0x1401712a0 (vt+0x1f8) copies each graphics item's +0x60
  (always 1 after open) into the model flag through vt+0xa0, but only for items that have a
  graphics item (vt+0x1d8). Operation items therefore save "yes" in every Oracle B case.
* Group items (SfOprCycle) have no cell graphics item, so they keep the uninitialised byte. The
  same input gives "yes" in 97_B_CNVACT and "no" in 97S_B_CNVACT_UP2. That can't be reproduced.
  **Known deviation** (Aleksandr 2026-10-22, option b): the port writes "yes" for group items. The
  moproj test accepts either value for SfOprCycle / SfOprGroup items only.
* The call from the save path to vt+0x1f8 is not traced (0x140099040 does not call it directly).
  Oracle B shows the sync happened: inputs with "no" on operation items save "yes" (97, R47F,
  R50F, 99, 102, 104).

## Expansion_Items (ported: `buildExpansion`, src/model/subproject.cpp)

Built once when the item is created at open (0x140073900, called by ParseDiagramItem), kept on
`FlowItem::expansion` and written by 0x14009b710.
* Template 0x140071d40: Text "Process", Line Solid, the class's process items (info +0x88, max 2;
  its setter 0x1405fdf70 has no caller, so there are none) padded with Space rows to 2, Line Solid,
  then Text "Objects" and Line Dash when the class's mime list (info +0x90, set by 0x1405fdfc0 in 25
  class inits) has an entry starting with "application/sftc.text.obj".
* Then, for an operation whose class accepts objects, one Text row per object slot: "Undefined"
  (code -1), "Read From DB" (-2), else the slot object's name (0x140061250 appends missing
  entries, as in the original). The lubricant row needs an expansion row that accepts
  "application/sftc.text.lub" and none does.
* Text rows are SfDiagItemExpansion with indent 2 (0x140160e70); the label is two spaces plus the
  text (0x140188730). tr() is identity (no translator installed).
* Oracle B: all 15 moproj test cases match with the ported rows, including 97 where the input's
  Convert2Dto3D had 5 rows and the GUI saved 8.
