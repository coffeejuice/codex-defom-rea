# CP104 — Geometry child after QFileInfo

## Result

The retained reader establishes a narrow ordering boundary, but not a Geometry owner or payload import. At `0x140911542`, the native code constructs `QFileInfo` from the actual filename argument held at `[rsp+0x1020]`; `0x140911559` calls `QFileInfo::path`, writing its QString result to the local at `[rsp+0x238]`. The very next operation (`0x140911560` onward) begins parsing the `GeoOperation` attribute. Its value is converted with `QString::toInt` at `0x1409115ea` and stored at `[rsp+0x210]`. `GeoObject` is likewise parsed and converted at `0x140911632..0x1409116b3`, stored at `[rsp+0x21c]`; `MorphMethod` parsing starts at `0x1409116f1`.

In this bounded span, the `path()` result has no visible consumer before attribute parsing: the destination local is not read in the displayed instructions. This does not prove it is unused later; it only shows that path construction is an earlier side-effect/ownership boundary than the geometry attributes. Therefore neither the returned path nor the following integer attributes establish that a geometry record was appended or that any file payload was opened.

## Source-qualified limit

The candidate `readInputTable(bytes, Study&)` receives bytes and a Study, not the actual input XML filename QString used by the native call chain. `activation.cpp` computes `Control/DoeInputTable.XML` as a string for `StudyIo`, but that does not preserve the native QString/QFileInfo construction, path return, or file-engine behavior. The current `variable_record.cpp` refusal immediately before this boundary remains correctly conservative. Moving it past QFileInfo/path would require a source-qualified filename/path representation and matching path behavior; moving it through the later Geometry fields would additionally require the geometry record/data owner and payload-import contract.

## Next dependency

Smallest useful recovery step: thread an explicitly qualified input-path value through the in-memory StudyIo/readInputTable call chain and establish its native-equivalent construction and QFileInfo::path result (including path normalization and lifetime). Until that is available, keep refusal before `0x140911542`. After that boundary, separately recover the concrete geometry owner/allocation and the first file consumer; the current evidence only demonstrates scalar attribute extraction, not an import increment.

## Evidence and limits

- `analysis/resume50-doe-variable-body-pe-annotated.asm`, lines 2470–2490 and 2518–2551: QFileInfo construction/path call and first GeoOperation/GeoObject extraction.
- `analysis/resume104-doe-geometry-entry-plan.md`: native caller argument provenance and current candidate filename limitation.
- `source/apps/moproj2mst/src/doe/activation.cpp`, `source/apps/moproj2mst/src/doe/variable_record.cpp`: current bytes-only reader path and refusal boundary.
- This is a static, bounded disassembly reading. No runtime import, geometry owner, payload access, or later use of the `path()` local is claimed.
