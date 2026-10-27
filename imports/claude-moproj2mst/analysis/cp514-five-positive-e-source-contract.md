# CP514 selected five-item positive E placement

The selected static parser/model route begins with an empty chart and five
ordinary nongroup, initially unlinked `SfOprForming2D` items A/B/C/D/E. A–D
have numerically zero parsed RectFs. Every final CellPos is independently
missing or an OR sentinel (`row==-1 || col==-1`). E has finite parsed RectF
`(x,y,w,h)` with `0<x<1332.5`, `y<160`, finite signed dimensions. Snapshot
eligibility before placement.

An independent DUMPBIN 14.44.35228.0 audit checked the immutable original
GUI SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:
16 complete `.pdata` spans, 12,068 contiguous bytes, 2,355 addressed
instructions, zero gaps or mapped-byte mismatches. Original installer
`14015b610..14015b7d2` hashes to
`31e7ce04f32ab20a292f48db5fb4b8865b65fa20a9da835de3462c493df9d0c0`,
recursive shift `14015b7e0..14015b92e` to
`1138810530ad3bced8d0afe16eff35a7fd01c4197f35209995c70194b831776b`,
and graph scan `140158cc0..140158daf` to
`db3e23e020d655aea1616691c0b4459df91ab3ae42bb281d65a0d75935526c8e`.
The separately checked QtCore `QRectF::moveTo` leaf writes only x/y, leaving
parsed dimension bits intact.

After A–D, grid order is A0/D1/C2/B3. Native half-up requested-column
rounding changes at 102.5, 307.5, 512.5, 717.5, 922.5 and 1127.5. The
selected final installation changes at 205, 410, 615 and 922.5:

| E x | Final grid / graph / MST |
| --- | --- |
| `0<x<205` | A0/E1/D2/C3/B4 |
| `205<=x<410` | A0/D1/E2/C3/B4 |
| `410<=x<615` | A0/D1/C2/E3/B4 |
| `615<=x<922.5` | A0/D1/C2/B3/E4 |
| `922.5<=x<1332.5` | A0/D1/C2/B3, gap, E5..6 isolated |

Links are reciprocal between adjacent occupied columns. MST starts at A and
traverses its contiguous linked run, excluding E in the last interval. Each
final cell is row0, each RectF origin `(205*column,+0)`. Parsed dimension
bits, IDs and model-vector order remain unchanged. `y<160` is the common
populated-row envelope; exact160 is outside the selected cut, not universally
native-invalid if the row height expands.

Other counts/classes/groups, preseeded charts, initial links, changed A–D
geometry, explicit nonsentinel CellPos, nonfinite or outside-envelope E and
native notifications remain excluded. Fresh original Open/Save/Close parity
remains unqualified.
