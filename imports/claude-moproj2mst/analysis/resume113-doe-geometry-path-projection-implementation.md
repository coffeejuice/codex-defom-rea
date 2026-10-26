# CP113 isolated Geometry path-value helper

Implemented the approved isolated conditional path-value projection in three new DOE files. It returns the supplied source filename, projected parent and upper/lower/nominal strings; it neither constructs QFileInfo nor mutates a Study/Variable/geometry row. Public Geometry still refuses before QFileInfo. Root verified CP112 freeze before authorizing these edits.

Frozen contract `resume113-doe-geometry-path-projection.md`, SHA-256 `53E280F7A4CAEB82D576AE3016F396402820853138784B0A03FF0493FB2243E1`, is controlling. It coordinates graph_audit's corrected manager-selected Var/owned-row destruction chain and retains exact original hashes/capture bounds. This implementation deliberately stops at the **path-value** layer; operation/object key, row append/duplicate behavior, flags/correspondence, payload consumer and native owner admission remain separate.

## Implemented API and domain

New internal `geometry_path_projection.hpp` declares `GeometryPathValues` and `projectGeometryPathValues(const InputTableSource&, const xml::Node&)`. The result owns String values for source filename, parent directory, upper/lower/nominal file strings. It is not an owner/registration/constructor-return receipt or a public-import capability.

The helper requires an exact Geometry element and a nonempty named absolute drive/UNC **filename** shorter than32768 UTF-16 code units. It replaces input backslashes with '/', rejects relative/search/resource/root-only names, NUL, trailing-directory slash, incomplete UNC share and device namespace profiles, and retains the narrow signed16 separator bound. These are reconstruction domain restrictions, not native QFileInfo input validation.

Parent selection uses the existing `qt::path` lexical value helper after normalization. New retained native getter evidence established that normal QFileInfo(QString) clears private+0xb4.bit0 and path() then directly selects QFileSystemEntry::path0x670ef4b0. The constructor resolver can copy back rewritten entries for search prefixes; the selected absolute drive/UNC routes skip that rewriting. Native normal construction, handler/ref/engine effects and failure return are **not** supplied by this value calculation. Dot components, repeated separators, case and parent drive-root slash are retained rather than cleaned/canonicalized.

For each exact UpperGeoFile, LowerGeoFile and NominalGeoFile attribute the helper returns `parent + "/" + rawAttribute`. It does not substitute an absolute right-hand side, convert its backslashes, collapse dots/slashes or turn missing/empty attributes into empty paths. Attribute role/order is upper, lower, nominal, independently named in the result. No numeric attributes, flags, point pairs or file payloads are interpreted. There is no filesystem query, copy, stream, notifier, cache, native reference or Study/Var argument.

## Focused tests and static validation

Six cases in new `tests/doe/test_geometry_path_projection.cpp` cover:

- Absolute drive and UNC paths, normalized input separators, drive-root parent and repeated/dot components retained.
- Literal joins with relative `../`, repeated slash, absolute drive and rooted/right-hand values; right-side backslashes remain raw.
- Empty/missing attributes produce the retained parent plus literal slash, including `C://` at a drive-root parent, with nonnull result values.
- Null/empty, relative, drive-relative, search/resource prefix, rooted non-UNC, trailing-directory, incomplete UNC, device namespace, embedded-NUL and wrong-tag profile refusals.
-32767-code-unit accepted filename and32768 refusal, bounding the native signed16 separator representation.
- A computed projection does not bypass either bytes-only or named-source public Geometry refusal, append Var.files or supply GeometryVariable metadata; the reached header/index-1 state remains.

Testkit/include preflight passed for the new test: supported checks/main and direct quoted includes resolve against actual target include paths. Direct trailing-whitespace/final-newline scans passed for all three new files. No worker compiler/build/CTest, GUI/native process or mutable REA was used. Root owns grouped executable validation and CMake discovery; no shared build file was changed.

Expected target `m2m_doe_geometry_path_projection`, CTest `m2m.doe.geometry_path_projection`, is registered by existing globs. The public parser, helper/Study headers and all existing source/tests remain untouched by this source lane.

## Source freeze receipt

All paths below are under `source/apps/moproj2mst`. Hashes were sent to root and independent review after static checks; files remain held through root validation/archive.

| New file | SHA-256 |
| --- | --- |
| src/doe/geometry_path_projection.hpp | 6B75D760FE9D0A3CECF5A180F8741782BDE15EC5395F3D0EF816BDC3B172C7FA |
| src/doe/geometry_path_projection.cpp | 002215B84AF1AB51F26DC5BB3E4A2827F7009EE454E0872E381ED6B316B5B4F6 |
| tests/doe/test_geometry_path_projection.cpp | 9F7EE32F6D7204CE787A0E29E361CDDFBB660B4336529698DFAFD9541B733039 |

Next integration requires the actual named filename's native QString/shared-null/QFileInfo construction/handler occurrence and a coherent owned Geometry row representation with IDs/key/flags/correspondence before consuming these values publicly. The existing partial GeoFile projection is not silently populated from strings alone. A later GEO payload reader and active Save table/timer callback-return ownership remain unclosed. No native constructor or import parity is claimed by these isolated values.
