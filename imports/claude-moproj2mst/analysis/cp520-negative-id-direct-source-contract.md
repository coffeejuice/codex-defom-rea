# CP520 selected all-negative distinct direct targets

This static parser/model route starts with an empty chart and two to seven
ordinary nongroup initially unlinked `SfOprForming2D` items, all with
negative operation IDs. Snapshot parsed RectFs, final CellPos values and
links before placement. Every RectF component is finite and every x is
below 1332.5. The first item requires y below67.5; each later item requires
y below190. The first negative-ID graphic expands to height190, so the
populated row height is at least215 and admits these later items. Exact190
is outside this common selected cut.

Each final CellPos is explicit `(0,c)` with `0<=c<=6`, or is missing/OR
sentinel (`row==-1 || col==-1`). A sentinel discards its other coordinate,
clamps x below -60 to zero and derives its column by native half-up x/205.
All snapshot target columns must be distinct within0..6. Every requested
token is empty, so the explicit-path uninitialized direction local cannot
affect this selected route.

Final cells are `(0,c)`, RectF origins `(205*c,+0)`, with parsed signed
width/height bits, IDs and model order preserved. Graph links are reciprocal
between adjacent occupied columns in grid order. Empty columns break the
chain; MST begins at the lowest occupied column and follows its contiguous
linked run.

This contract combines the independently verified CP519 negative-ID
graphics/row-height chain and CP510 four-to-seven/CP507 three-item direct
placement decisions. The original immutable GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
CP519's DUMPBIN14.44.35228.0 audit checked eight bounded spans and 5,485
contiguous bytes with zero PE mismatches; CP510 checked 21 spans, 16,177
bytes and 3,148 instructions with zero mismatches. The original QtCore
`QRectF::moveTo` leaf writes only x/y, preserving parsed dimensions.

Explicit collisions, duplicate target columns, positive-ID profiles,
nonfinite/outside-envelope geometry, initial links/preseeded charts,
other classes/groups/counts and native notifications remain excluded.
Fresh original Open/Save/Close parity remains unqualified.
# CP525 integration qualification

The later CP525 graph audit found that associated Operations may add
object-row connector effects. The integrated above-common-height negative
direct route now requires every item unbound to a Project Operation. This
narrows the original CP520 candidate; historical evidence above is retained.

