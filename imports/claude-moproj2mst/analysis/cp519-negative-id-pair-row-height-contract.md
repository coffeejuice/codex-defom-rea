# CP519 selected negative-ID pair row-height extension

This static parser/model increment extends the existing two-item sentinel
pair route only when both ordinary nongroup initially unlinked Forming2D
items have negative operation IDs in an initially empty chart. A has parsed
zero RectF. B has finite parsed `(x,y,w,h)` with `0<x<1332.5`,
`160<=y<190`, finite signed dimensions. Each CellPos independently is
missing or has `row==-1 || col==-1`.

Independent DUMPBIN14.44.35228.0 inspection of the immutable original GUI
SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
verified eight bounded spans totaling 5,485 contiguous bytes, zero mapped-PE
mismatches. The negative-ID graphics constructor sets the expansion flag at
`14016258d`; update `140163979..1401639aa` sets content height160;
`14016d900` applies ±15 margins, yielding graphics height190. Row rebuild
`140157fe0` sets row height `max(185,190+25)=215`, and populated row lookup
`14014dfa0` accepts strict `y<215-25`, hence `y<190`. PE import metadata
independently resolves `rect`, `height`, `adjusted`, `setRect` and `setHeight`.
Parsed model RectF height does not determine graphics height.

The existing CP503 placement result remains: A cell `(0,0)`; B column1 for
`0<x<307.5`, otherwise native half-up derived column2..6. B final RectF is
`(205*column,+0,w,h)` with parsed dimensions preserved. Column1 gives
reciprocal A→B links and MST `[A,B]`; a column gap leaves null links and
MST `[A]`. IDs and model order remain unchanged. Exact y=190 is outside this
selected negative-ID profile. The prior graphics-independent `y<160` route
remains valid for other IDs.

The separately PE-qualified distinct direct-target all-negative-ID route
for two to seven items is queued after this smallest pair increment. Other
graphics, positive-ID first items, nonfinite/outside-envelope input,
preseeded charts, initial links, groups/classes and native notifications
remain excluded. Fresh Open/Save/Close parity remains unqualified.

# CP525 integration qualification

The later CP525 graph audit found that associated Operations may add
object-row connector effects. The integrated above-common-height pair route
now requires both items unbound to Project Operations. This narrows the
original CP519 candidate; the historical evidence above is retained.

