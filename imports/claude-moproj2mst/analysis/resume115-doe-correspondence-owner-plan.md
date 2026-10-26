# CP115 correspondence receiver ownership plan

The next isolated Geometry increment should append already-converted PointPair values to the existing GeoFile row owner. Public Geometry must retain the earlier QFileInfo refusal. The plan requires the bounded receiver evidence from the parallel graph audit before any source or test edit.

## Closed reader-side seam

The retained original reader `analysis/resume50-doe-variable-body-pe-annotated.asm` traverses direct Geometry children in encounter order. Exact `UpperCorrespondence` and `LowerCorrespondence` elements have their own direct child loops; exact `PointPair` elements supply text. Other tags do not enter these point branches. This traversal follows the CP114 row insertion, tail flags and variable MorphMethod effects.

At `0x14091219a..0x1409121cc` and `0x1409124e8..0x14091251a`, the reader splits text using literal `, ` with KeepEmptyParts and case-sensitive comparison. It accesses token indexes 0, 1, 2 and 3 through `0x140004b40`, calls QString::toDouble with a null validity pointer, and constructs two QPointF values in the order `(token0, token1)`, `(token2, token3)`. This is not a source-qualified basis for substituting the host C-locale `qt::toDouble`: original locale/provider state and malformed token access remain separate dependencies.

The upper call at `0x1409122fe` receives `EDX=0` from `0x1409122f4`; the lower call at `0x14091264c` receives `EDX=0` from `0x140912642`. These values must not be replaced with the previous row-prefix matched index or tail index. The concrete receiver is the same previously selected Var at `[rsp+0xa8]`.

## Existing owner and proposed source seam

`HeaderVariableRecord` is concretely owned by `Study::headerVariableOwners` and aliases `Study::vars`. Its inherited `Var::files` is the single logical native Var+0x28 owner. CP114 adds keyed rows there and intentionally stops before correspondence. A separate HeaderVariableRecord row collection would split that owner and is excluded.

The proposed receiver API, subject to graph evidence confirmation, is an internal explicit-input operation:

```cpp
bool appendGeometryCorrespondencePair(
    HeaderVariableRecord&, int rowIndex, GeometryCorrespondenceSide,
    const GeometryPoint& first, const GeometryPoint& second);
```

Only already-converted values enter this seam. The public parser does not call it. A later qualified reader binder would pass index zero and dispatch upper/lower calls in encounter order; it must separately establish native input filename/QFileInfo/conversion ownership first.

The parallel audit must establish whether each side owns paired records or separate point collections, exact field offsets, bounds behavior and append/failure order. If it confirms one paired record per call, the proposed parent-owned shared fields are `GeometryPoint {double x,y}`, `GeometryPointPair {GeometryPoint first,second}`, and GeoFile `upperCorrespondence` / `lowerCorrespondence` vectors of pairs. This proposal must be revised rather than forced if the native receiver uses another shape. No keys or field values will be fabricated for legacy rows.

## Reserved write set and tests

Proposed exclusive worker source set: existing `src/doe/variable_record.cpp`, existing private `src/doe/geometry_row_prefix.hpp`, and new `tests/doe/test_geometry_correspondence_prefix.cpp`. Root owns any `include/m2m/mst/doe_model.hpp` changes. No source/test edits have been made at plan creation.

Focused tests should construct an actual Study header owner, create at least two keyed rows with the existing row helper, and prove correspondence lands on the explicit selected row rather than the tail. They should cover ordered repeated pairs, upper/lower separation, invalid-index behavior if evidenced, unchanged flags/MorphMethod/key/path fields, and continued public Geometry refusal. Unknown legacy key admission must follow the pair receiver itself rather than borrowing CP114's dedup preflight without evidence. Host container ownership and normal value effects are the claimed cut; native allocator/null/EH and Qt numeric conversion effects require their own qualification.

## Receiver contract closure

The parallel audit is now frozen in `analysis/resume115-geometry-point-methods-static.md`, SHA-256 `B326881DBF50B46E65E39BEF884477CC057E9D581E782E35A17420030E76A39D`. It confirms the proposed paired storage: each side owns one sequence of 32-byte pairs, with two 16-byte QPointF inputs in order. Negative and count-or-greater signed row indexes return before pair construction or row selection. A valid row receives one append through the same void helper; no local rollback/status branch exists. Native COW, allocation failure and outer exception behavior remain unqualified.

The final private API keeps the proposed shape. Its Boolean is a host observation (`false` for native invalid-index no-op, `true` after normal append), not a recovered native return or public success. It must not require row key fields: the receiver selects by index without examining those keys. Root owns the agreed shared two-point/two-sequence value representation. This freezes the evidence/plan before worker implementation; a separate implementation receipt will report exact source hashes and static validation.
