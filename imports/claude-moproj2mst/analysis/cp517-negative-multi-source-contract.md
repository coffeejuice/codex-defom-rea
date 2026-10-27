# CP517 selected four-to-seven negative-last placement

This static parser/model route begins with an empty chart, four to seven
ordinary nongroup initially unlinked `SfOprForming2D` items, and each final
CellPos independently missing or OR-sentinel (`row==-1 || col==-1`). All but
the last parsed RectF have numerically zero components, retaining signed
zero dimension bits. The last RectF is finite `(x,y,w,h)` with `x<0`,
`y<160` and finite signed dimensions. Snapshot all inputs before mutation.

The immutable original GUI SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Independent DUMPBIN14.44.35228.0 verification covered 19 complete `.pdata`
spans, 14,588 contiguous bytes and 2,841 addressed instructions with zero
mapped-PE gaps or mismatches. The verified bodies include clamp, rounding,
direction, row lookup/admission, installer, recursive shift, relayout, graph,
MST and the seven-column constructor. Separately checked QtCore
`QRectF::moveTo` writes x/y only, preserving parsed dimension bits.

Set `effectiveX = x<-60 ? +0 : x`, and binary64 `q=effectiveX/205`.
For `q<0`, the final item inserts at column0 and shifts all existing items
one column right. For `q==0`, including -0 after underflow, it inserts at
column1, leaving the first item at0 and shifting the occupied suffix right.
Exact -60 takes the negative branch; its representable predecessor clamps.
Under nearest rounding with gradual underflow, -102 minimum subnormals
divide to -0 and -103 to a negative value. The source tests the quotient.

| Count | `q<0` grid/MST | `q==0` grid/MST |
| --- | --- | --- |
| 4 | D,A,C,B | A,D,C,B |
| 5 | E,A,D,C,B | A,E,D,C,B |
| 6 | F,A,E,D,C,B | A,F,E,D,C,B |
| 7 | G,A,F,E,D,C,B | A,G,F,E,D,C,B |

Every cell is row0 with consecutive columns 0..N-1. Origins are
`(205*column,+0)`; links are reciprocal between adjacent occupied columns,
so MST includes the full grid sequence. Parsed dimensions, IDs and model
vector order remain unchanged. Seven is the maximum selected row0 count;
eight needs separate wrap analysis. Exact y=160 remains outside the common
row-height cut, without claiming universal native invalidity.

Explicit cells, nonfinite/outside-envelope inputs, changed prefix geometry,
preseeded charts, initial links, other groups/classes/counts and native
notifications remain outside this increment. Fresh Open/Save/Close parity
remains unqualified.
