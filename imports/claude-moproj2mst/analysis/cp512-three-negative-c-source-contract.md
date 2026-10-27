# CP512 selected three-item negative C placement

This selected static parser/model route starts with an empty chart and three
ordinary, nongroup, initially unlinked `SfOprForming2D` items A/B/C. A and B
have numerically zero parsed RectF components. Their signed zero dimension
bits remain meaningful. Each final CellPos is independently missing or has
`row==-1 || col==-1`. C has finite parsed RectF `(x,y,w,h)` with `x<0`,
`y<160`, and finite signed dimensions. Snapshot all inputs before placement.

Original immutable GUI SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The independent DUMPBIN 14.44.35228.0 audit verified 15 complete GUI
`.pdata` intervals, 8,277 contiguous bytes and 1,686 instruction rows
against mapped PE bytes, with zero mismatches. The clamp/forward body is
`1401574f0..14015786f`; rounding `14014db20..14014dbfe` hashes to
`df1306c970626a94befbbb0a5b003f5a44f6f7bb659892254c358f956a380cbc`,
and direction `14014e210..14014e2cf` to
`1cca9d01ac29b30d83b5c22bc5e86e70b0b3164185ee8a6fbf399a1332ce3c86`.
QtCore's separately checked `QRectF::moveTo` leaf writes x/y only, retaining
parsed dimension bits. This is source evidence, not a fresh original lifecycle
comparison.

Set `effectiveX = x<-60 ? +0 : x`; compute binary64 `q=effectiveX/205`.
After A/B occupy columns 0/1, `q<0` requests direction -1, inserts C at
column 0 and shifts A/B to columns 1/2. Graph/MST are C→A→B. When `q==0`
(including signed negative zero after underflow), native comparison takes the
nonnegative arm: C inserts at column 1 and shifts B to column 2; graph/MST
are A→C→B. Exact -60 remains negative; its predecessor clamps to +0.
Under ordinary round-to-nearest gradual underflow, -102 minimum subnormals
divide to -0 and -103 divide to a negative minimum subnormal. The source
implementation tests the computed quotient, not a decimal threshold.

Final item origins are `(205*column,+0)`; parsed width/height binary64 bits,
IDs and model-vector order remain unchanged. The row0 common y bound is
`y<160` after A/B establish a minimum row height185; exact160 is not
universally invalid when graphics enlarge the row.

Fully explicit occupied cells, nonfinite inputs, nonnegative C, changed A/B
geometry, links, preseeded charts, other groups/classes/counts and native
notifications remain outside this increment. Fresh Open/Save/Close parity
remains open.
