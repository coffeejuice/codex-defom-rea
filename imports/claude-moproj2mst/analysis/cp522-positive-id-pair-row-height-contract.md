# CP522 positive-ID pair conservative row-height cut

An ordinary Forming2D item with nonnegative Operation_Id and no matching
Project Operation uses the original positive-ID graphic template. The
expanded template has at least seven visible rows whose measured heights sum
to 69. The base graphic remains at least 60 high; expansion and margins
establish graphics height at least 189. Native row rebuild adds 25, so an
existing row0 admits a later item's finite top `y<189`. This is a safe lower
bound, not an exact height for every project or environment.

The selected route is exactly two initially unlinked, nongroup Forming2D
items in an initially empty chart. The first has nonnegative ID, no matching
Project Operation, parsed zero RectF x/y and finite dimensions. The second
has missing or OR-sentinel CellPos, finite `0<x<1332.5`, finite `y<189`, and
finite signed dimensions. Its column is half-up rounding of x/205, including
the native x<-60 clamp where applicable; RectF x/y normalize to grid origin
and its dimension bits survive. Column1 connects A to B; a later column
leaves a graph gap and MST contains only A. The source branch does not widen
associated-Operation cases, y>=189, nonfinite dimensions or other classes.

Evidence: immutable original DEF_GUI_MO_64.exe SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
DUMPBIN14.44.35228.0 verified 17 complete `.pdata` spans, 14,351
contiguous bytes and 2,564 instructions with zero PE-byte mismatches. Key
bodies: expansion `140163e50..140164395` SHA-256
`9bc8af6c701801dad23723c64f73ad1b5f83fdcbeb4d3bce77d30e51da778d69`,
base `14016cab0..14016d8f7` SHA-256
`eace6af3e9c789a3b1e36ca93ad0d4a024649a8fdddad4a0d38c9c61ca42dec5`,
graphic template `140071d40..140072224` SHA-256
`e657684a275765e963d72fea07852c75a4266e970762ac6056959181855f5d87`,
and Forming2D init `140c043e0..140c048b1` SHA-256
`493f37dd7b0b4c67fcc196a4c21610b4cd822b9d59d3352c96a5df3c157c6506`.
The native graphical/template route can add further height. Fresh
Open/Save/Close parity and exact positive graphic height remain unobserved.
# CP525 integration qualification

The later CP525 graph audit found that an associated second Operation may
add object-row connector effects. The integrated positive-ID pair
above-common-height helper now requires both items unbound; CP525 separately
admits a first associated zero-slot/no-restart item through its distinct
direct-target route. This narrows the original CP522 candidate; the height
proof and historical evidence above are retained.

