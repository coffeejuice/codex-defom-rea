# CP511 selected three-item positive C placement

The selected static parser/model route begins with an empty chart and three
ordinary, nongroup, initially unlinked `SfOprForming2D` items A/B/C in model
order. A and B have parsed zero RectFs. Each final CellPos is independently
missing or has either coordinate -1. C has a finite parsed RectF `(x,y,w,h)`
with `0<x<1332.5` and `y<160`. Eligibility is snapshotted before placement.

The original immutable GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Independent DUMPBIN 14.44.35228.0 checks covered 24 GUI spans, 11,752
contiguous bytes, 2,362 instructions and the ten-byte QtCore `moveTo` leaf,
with zero mapped-PE mismatches. Key original body hashes: rounding
`14014db20..14014dbfe` =
`df1306c970626a94befbbb0a5b003f5a44f6f7bb659892254c358f956a380cbc`,
installer `14015b610..14015b7d2` =
`31e7ce04f32ab20a292f48db5fb4b8865b65fa20a9da835de3462c493df9d0c0`,
recursive shift `14015b7e0..14015b92e` =
`1138810530ad3bced8d0afe16eff35a7fd01c4197f35209995c70194b831776b`,
graph scan `140158cc0..140158daf` =
`db3e23e020d655aea1616691c0b4459df91ab3ae42bb281d65a0d75935526c8e`.

After A/B install, grid columns 0/1 contain A/B. For `0<x<205`, C inserts
at column 1 and shifts B to 2, giving A/C/B. The rounding boundary 102.5
changes requested token and direction but not this final order. For
`205<=x<512.5`, C ends in free column 2, giving A/B/C; 307.5 changes the
requested token but not the final order. For `512.5<=x<1332.5`, C installs
directly in columns 3..6 and the gap after B breaks its graph/MST connection.
Every item ends row0; RectF origins become `(205*column,+0)` and parsed width
and height bits are preserved. IDs and model order remain A/B/C. Reciprocal
graph links follow adjacent occupied columns. MST starts with A and follows
the contiguous chain.

The populated row has a common minimum height 185, so `y<160` is admitted
independent of C dimensions. Exact160 is outside this common route but is not
universally invalid if graphics enlarge the row. Fully explicit collisions,
negative C, nonfinite/outside-envelope geometry, other counts/classes/groups,
initial links and preseeded charts remain separate work. Native notifications
and fresh Open/Save/Close parity remain unqualified.
