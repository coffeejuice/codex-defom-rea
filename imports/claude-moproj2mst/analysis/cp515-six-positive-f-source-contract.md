# CP515 selected six-item positive F placement

The selected static parser/model route begins with an empty chart and six
ordinary nongroup, initially unlinked `SfOprForming2D` items A/B/C/D/E/F.
A–E have numerically zero parsed RectFs. Each final CellPos is independently
missing or an OR sentinel (`row==-1 || col==-1`). F has finite parsed RectF
`(x,y,w,h)` with `0<x<1332.5`, `y<160`, and finite signed dimensions.
Eligibility is snapshotted before placement.

Independent DUMPBIN 14.44.35228.0 audit of the immutable original GUI SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
verified 19 complete `.pdata` spans, 15,220 contiguous bytes and 2,965
addressed instructions with zero mapped-PE gaps/mismatches. Installer
`14015b610..14015b7d2` hashes to
`31e7ce04f32ab20a292f48db5fb4b8865b65fa20a9da835de3462c493df9d0c0`,
shift `14015b7e0..14015b92e` to
`1138810530ad3bced8d0afe16eff35a7fd01c4197f35209995c70194b831776b`,
relayout `1401583e0..140158cb2` to
`9a30e18faf8c61ae6da9bbd7741531eb73546a8e933ed597051c7493232eb29c`,
and graph `140158cc0..140158daf` to
`db3e23e020d655aea1616691c0b4459df91ab3ae42bb281d65a0d75935526c8e`.
The independently verified QtCore `QRectF::moveTo` leaf writes only x/y,
preserving parsed width/height binary64 bits.

After A–E, grid order is A0/E1/D2/C3/B4. F placement is:

| F x | Final grid / graph | MST |
| --- | --- | --- |
| `0<x<205` | A0/F1/E2/D3/C4/B5 | A,F,E,D,C,B |
| `205<=x<410` | A0/E1/F2/D3/C4/B5 | A,E,F,D,C,B |
| `410<=x<615` | A0/E1/D2/F3/C4/B5 | A,E,D,F,C,B |
| `615<=x<820` | A0/E1/D2/C3/F4/B5 | A,E,D,C,F,B |
| `820<=x<1127.5` | A0/E1/D2/C3/B4/F5 | A,E,D,C,B,F |
| `1127.5<=x<1332.5` | A0/E1/D2/C3/B4, gap, F6 | A,E,D,C,B |

Native half-up requested-column changes occur at 102.5, 307.5, 512.5,
717.5, 922.5 and 1127.5. The final insertion boundaries are 205, 410,
615, 820 and 1127.5. Every final cell is row0; origins are
`(205*column,+0)`. Reciprocal links follow adjacent occupied columns and
the empty column5 isolates F6. IDs, model order and parsed dimension bits
remain unchanged. Exact y=160 is outside the common populated-row envelope
but not universally native-invalid if graphics enlarge row height.

Other counts/classes/groups, preseeded charts, initial links, changed A–E
geometry, nonsentinel explicit cells, outside-envelope/nonfinite F, native
notifications and fresh Open/Save/Close parity remain excluded.
