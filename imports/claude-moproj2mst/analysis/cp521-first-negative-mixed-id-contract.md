# CP521 first-negative mixed-ID row-height extension

The first ordinary Forming2D item's negative Operation_Id alone supplies
graphics height190 and row0 height at least215. Later positive or mixed IDs
cannot lower that row height: the original rebuild only stores a larger
graphics-height-plus25 value. Thus the existing CP519 sentinel pair and CP520
distinct direct-target routes may retain their later finite `y<190` bound
while requiring only the first ID negative.

For the direct route, retain CP520's exact guards: initially empty chart,
2–7 initially unlinked ordinary nongroup Forming2D items, every RectF
component finite, each x<1332.5, first y<67.5, later y<190, and each final
CellPos explicit valid row0 column0..6 or missing/OR sentinel. Sentinel
resolution clamps x<-60 and half-up rounds x/205; snapshot targets must be
distinct. Every final cell is `(0,c)` and RectF `(205*c,+0,w,h)` with parsed
dimension bits, IDs and model order preserved. Grid-adjacent links are
reciprocal; gaps break the chain and MST follows the lowest contiguous run.

For the sentinel pair, retain CP519's A-zero/B finite positive-x geometry,
missing/OR-sentinel cells, empty chart and initial-link guards. B has
`0<x<1332.5`, `y<190`, finite signed dimensions; A alone needs negative ID.
B column1 for x<307.5 connects A→B; columns2..6 leave a gap. The pair's
ID/model order and RectF dimension bits remain unchanged.

Independent DUMPBIN14.44.35228.0 verification of the immutable original GUI
SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
checked 16 complete `.pdata` spans, 14,765 contiguous bytes and 2,759
instructions, with zero PE-byte mismatches. Key bodies: row rebuild
`140157fe0..1401583ce` hash
`4800f06d904dfa13555f738873e8cf3bb6aa5f3bd976ab56a089f096d417d65c`,
graphic update `1401637b0..140163e41` hash
`7205cb45bbd02dbce3e75b9083a4e70e8657d1dfffa29372f198186c2303eb11`,
and graph admission `140159780..140159956` hash
`2c6783997a7c6af03f8ce1a0af5f78820e022571e2c0c109344952c9ed2405b1`.
The selected editor's cycle-prevention body contains no signed ID gate;
native notifications are outside this parser/model projection.

Nonnegative-first height extensions, collisions/duplicate targets, other
classes/groups/counts, preseeded charts, initial links, nonfinite/outside
geometry and fresh Open/Save/Close parity remain excluded.
# CP525 integration qualification

The later CP525 graph audit found that associated Operations may add
object-row connector effects. The integrated above-common-height negative
pair and direct routes now require every item unbound to a Project
Operation. This narrows the original CP521 candidate; its PE height proof
and historical evidence above are retained.

