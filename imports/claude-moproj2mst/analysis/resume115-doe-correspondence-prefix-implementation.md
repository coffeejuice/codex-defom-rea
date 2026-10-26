# CP115 Geometry correspondence receiver prefix

The isolated helper now appends explicit point pairs to the existing GeoFile row's upper or lower sequence. It preserves the reader's important target distinction: correspondence calls use row zero, while CP114's earlier flag writes use the list tail. Public Geometry still refuses before QFileInfo; no XML-to-correspondence or GEO payload admission was added.

## Contract and ownership

The evidence/plan was frozen before source edits in `analysis/resume115-doe-correspondence-owner-plan.md`, SHA-256 `D504781D38323FB885294134B5A545ABC089EC8C8A19441E295C4EAA7138D1D5`. The parallel native report `analysis/resume115-geometry-point-methods-static.md`, SHA-256 `B326881DBF50B46E65E39BEF884477CC057E9D581E782E35A17420030E76A39D`, binds Upper `0x140818000` and Lower `0x140818080` to the retained original GUI PE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its bounded raw receipt is `analysis/resume115-geometry-point-methods-bytes.txt`, SHA-256 `FFA65B76926B3743B0CB469D71E8E611FF7F54928B47B1B28DE6C8DCE1D3A174`.

Both native wrappers reject negative or count-or-greater signed indexes before pair construction or row selection. A valid call snapshots two 16-byte QPointF values as one 32-byte pair, selects the row by the supplied outer index and appends that pair to the row's Upper+0x28 or Lower+0x30 sequence. There is no deduplication, list clear, replacement, returned status branch or local rollback. Reader calls at `0x1409122fe` / `0x14091264c` explicitly pass index zero. The native COW/detach allocator, allocation failure policy and outer exception behavior remain unknown.

Root added `GeometryPoint`, `GeometryPointPair`, and two vectors on the existing `GeoFile` in its shared model lane. This worker did not edit that header. `HeaderVariableRecord` remains concretely owned by Study, with `Var::files` as its sole logical native row owner. The pair sequences are value owners nested in those rows; no additional borrowed row aliases or parallel owner list were introduced.

## Source behavior

The private header `src/doe/geometry_row_prefix.hpp` now declares:

```cpp
bool appendGeometryCorrespondencePair(
    HeaderVariableRecord&, int rowIndex, GeometryCorrespondenceSide,
    const GeometryPoint& first, const GeometryPoint& second);
```

`GeometryCorrespondenceSide` has Upper and Lower alternatives. The Boolean is a host diagnostic only: false for the native invalid-index no-op, true after normal append. The native receiver methods are void; this return is not native import or public Save success.

Implementation in `src/doe/variable_record.cpp` first checks the signed row bounds, then constructs a local pair snapshot, selects the row and side sequence, and appends once. Snapshotting before append preserves inputs that alias an existing sequence element when host vector growth moves its storage. Existing row keys are irrelevant to this indexed method and are neither checked nor filled. Flags, MorphMethod, filenames, paths, header fields, deferred links and sample state are untouched by this helper.

There is no XML traversal or numeric conversion here. Native reader token splitting, unchecked token indexes, QString::toDouble/provider state and QPointF construction precede this receiver seam and remain separately qualified. The existing host C-locale `qt::toDouble` is not substituted. Explicit point inputs, including signed zero or nonfinite doubles, are copied without numerical transformation. This is a normal value/ownership projection, not native Qt layout/refcount/COW or allocation-failure parity. Host append exceptions propagate; no catch, transaction or replay was added.

## Focused tests and checks

New `tests/doe/test_geometry_correspondence_prefix.cpp` has six Testkit cases:

1. Public header reading creates the real concrete Study owner; CP114 creates two rows and an earlier-key duplicate. Pair calls on row zero leave the tail empty while preserving the prior tail flags and MorphMethod.
2. Repeated pairs append in call order, upper/lower sequences remain independent, and aliased inputs are snapshotted before a subsequent append.
3. Valid nonzero index targets that row even with unknown legacy key fields; no dedup admission is borrowed from CP114.
4. Empty owner, negative indexes, integer extrema and index equal to row count are silent no-ops, preserving prior appended data and metadata.
5. Explicit signed zero, infinity and NaN inputs are copied without a parse/conversion claim.
6. Public Geometry containing PointPair still refuses at the existing entry guard after the header prefix, leaving the row list empty.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_geometry_correspondence_prefix.cpp` passed. Direct checks found final newlines and no trailing whitespace in all three worker source/test files; tracked `git diff --check` passed. No worker build, compiler invocation, CTest, GUI or mutable REA operation ran. Root owns runtime validation of `m2m_doe_geometry_correspondence_prefix` / `m2m.doe.geometry_correspondence_prefix`.

## Source release

The worker source was frozen and released before receipt completion:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/doe/variable_record.cpp | `14444C68752ED9B282999DEC079EF650DA5622620E2BA57D280E688C37890642` |
| src/doe/geometry_row_prefix.hpp | `975B97C64F8979A5BC70496171DE53C232F56A25A34A96A2DAEA1E198712B3A4` |
| tests/doe/test_geometry_correspondence_prefix.cpp | `B7AB7B43A0065B86A7CFD5B7C832847BC1192310AAF223FBD8CEDEB3A8FA244B` |

Root-owned shared `include/m2m/mst/doe_model.hpp` observed hash: `DB0BAD4EE205770CD645C1D417A9E0526877307FDC5458E2404C2C917A9F6370`. No shared header, CMake, parser call, ABI, writer or plan edits by this worker.

## Next dependency

The receiver value/append edge is implemented in isolation. Production binding still requires the actual named InputTable/QFileInfo normal-return owner and the reader's numeric conversion/provider state. The bounded next evidence action is to establish the PointPair token-index helper's short-list behavior and original locale conversion occurrence, without substituting host C-locale values. Actual GEO payload handling, public Save reconciliation and live refresh callback return remain independent guards. `admin/check_continuation.py` reports CONTINUE with 15 outstanding items; this source release is not objective completion.
