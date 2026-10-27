# CP513 selected four-item positive D placement

This static parser/model route starts with an empty chart and four ordinary,
nongroup, initially unlinked `SfOprForming2D` items A/B/C/D. A/B/C have
parsed zero RectFs. Every final CellPos is independently missing or has
`row==-1 || col==-1`. D has a finite parsed RectF `(x,y,w,h)` with
`0<x<1332.5`, `y<160` and finite signed dimensions. Eligibility is
snapshotted before any placement mutation.

Independent DUMPBIN 14.44.35228.0 verification of the immutable original GUI
SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
covered 20 spans, 12,322 contiguous bytes and 2,455 addressed instructions
with zero mapped-PE mismatches. Original installer
`14015b610..14015b7d2` hashes to
`31e7ce04f32ab20a292f48db5fb4b8865b65fa20a9da835de3462c493df9d0c0`,
recursive shift `14015b7e0..14015b92e` to
`1138810530ad3bced8d0afe16eff35a7fd01c4197f35209995c70194b831776b`,
and graph scan `140158cc0..140158daf` to
`db3e23e020d655aea1616691c0b4459df91ab3ae42bb281d65a0d75935526c8e`.
The separately verified QtCore `QRectF::moveTo` leaf writes only x/y, so
parsed dimension bits survive.

After A/B/C placement the grid is A0/C1/B2. Native rounding boundaries
102.5, 307.5 and 512.5 affect the requested token and direction; final
insertion boundaries are 205, 410 and 717.5:

| D x | Final grid | MST |
| --- | --- | --- |
| `0<x<205` | A0/D1/C2/B3 | A,D,C,B |
| `205<=x<410` | A0/C1/D2/B3 | A,C,D,B |
| `410<=x<717.5` | A0/C1/B2/D3 | A,C,B,D |
| `717.5<=x<1332.5` | A0/C1/B2/D4..6 | A,C,B |

All final cells are row0, RectF origins are `(205*column,+0)`, and parsed
width/height binary64 bits, IDs and model order remain unchanged. Reciprocal
links follow adjacent occupied grid columns; the gap before D in the last
interval breaks its link and MST membership. Exact160 is outside the common
populated-row envelope but not universally native-invalid when graphics
enlarge row height.

Other counts, linked/preseeded charts, other classes/groups, fully explicit
CellPos, nonzero A/B/C RectFs, nonfinite D, `x<=0`, `x>=1332.5`, and
`y>=160` remain outside this increment. Native notifications and fresh
Open/Save/Close parity remain unqualified.
