# CP516 selected seven-item positive G placement

The selected static parser/model route begins with an empty chart and seven
ordinary nongroup, initially unlinked `SfOprForming2D` items A–G. A–F have
numerically zero parsed RectFs. Each final CellPos is independently missing
or an OR sentinel (`row==-1 || col==-1`). G has finite parsed RectF
`(x,y,w,h)` with `0<x<1332.5`, `y<160` and finite signed dimensions.
Eligibility is snapshotted before mutation.

Independent DUMPBIN 14.44.35228.0 audit of the immutable original GUI SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
checked 17 full `.pdata` spans, 15,348 contiguous instruction bytes and
2,969 addressed instructions with zero gaps or mapped-PE mismatches.
Original installer `14015b610..14015b7d2` hashes to
`31e7ce04f32ab20a292f48db5fb4b8865b65fa20a9da835de3462c493df9d0c0`,
recursive shift `14015b7e0..14015b92e` to
`1138810530ad3bced8d0afe16eff35a7fd01c4197f35209995c70194b831776b`,
graph scan `140158cc0..140158daf` to
`db3e23e020d655aea1616691c0b4459df91ab3ae42bb281d65a0d75935526c8e`,
and MST `1400b6630..1400b745d` to
`9f2a04dcc677f82ec5e1b59815c7deef16574f77e422667466636da1cd17c01a`.
The separately checked QtCore `QRectF::moveTo` leaf writes x/y only, leaving
parsed dimension bits unchanged.

After A–F, grid order is A0/F1/E2/D3/C4/B5. G placement is:

| G x | Final adjacent grid / graph / MST |
| --- | --- |
| `0<x<205` | A,G,F,E,D,C,B |
| `205<=x<410` | A,F,G,E,D,C,B |
| `410<=x<615` | A,F,E,G,D,C,B |
| `615<=x<820` | A,F,E,D,G,C,B |
| `820<=x<1025` | A,F,E,D,C,G,B |
| `1025<=x<1332.5` | A,F,E,D,C,B,G |

Columns are consecutive 0..6 and all links reciprocal; MST includes all
seven. Every cell is row0, every RectF origin `(205*column,+0)`, and IDs,
model order and parsed dimension bits remain unchanged. The requested token
changes at half-up boundaries 102.5, 307.5, 512.5, 717.5, 922.5 and
1127.5. At x>=1025, G lands at free column6, so even direction +1 never
advances to column7. Exact1332.5 requests column7 and is outside the
selected admission gate; do not extrapolate huge finite conversion behavior.

Other counts/classes/groups, preseeded charts, initial links, changed A–F
geometry, nonsentinel explicit cells, nonfinite/outside-envelope G and native
notifications remain excluded. Exact y=160 is outside the common row0 cut,
not universally native-invalid. Fresh Open/Save/Close parity remains open.
