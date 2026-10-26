# CP113 — Geometry receiver ownership, path construction, and remaining payload gap

## Receiver binding correction

CP107 left the receiver dispatch as an unresolved dynamic object. CP91/93 retained manager evidence closes the specific dispatch used here, with an important distinction between the manager and the returned variable:

1. At `0x14090f0c5`, the object `[rsp+0x1030]` is queried through manager vtable slot `+0x178` for its variable count. The code subtracts one and calls slot `+0x1a0` at `0x14090f0ef` with that index.
2. CP91's retained `SfMoDoeManager` vtable at `0x1421671b8` maps `+0x178` to `0x1400d7f00` and `+0x1a0` to `0x14091b950`. The return from `+0x1a0` is stored in `[rsp+0xa8]` at `0x14090f0f5`.
3. Geometry calls (`0x140911f99`, `0x1409122fe`, `0x14091264c`) use `[rsp+0xa8]`, the selected Var record, not the manager `[rsp+0x1030]`. `0x1408175c0` appends the `0x38`-byte geometry row to that Var's `+0x28` list.

This corrects the earlier shorthand that the manager itself was being mutated. The manager chooses a Var; the Var owns its Geometry rows.

## Destruction chain

The retained CP91 manager destructor (`0x140919cb0`) walks the manager's variable list and calls its variable-delete wrapper (`0x14091db70`, flag 1). That wrapper calls the Var destructor (`0x1408169d0`) and raw delete. The Var destructor clears geometry using `0x140817d10` before releasing the geometry list wrapper.

`0x140817d10` iterates the Var `+0x28` list. For each row it calls `0x1408178810` with flag 1, then removes the row from the list. The bounded static body at `0x140818810..0x140818845` calls row destructor `0x140818850`, then `operator delete` (`0x1414dfc68`) when flag bit 0 is set. `0x140818850` destroys the row's list/string members at offsets `+0x30`, `+0x28`, `+0x10`, `+0x8`, and `+0x0`. Thus the current static chain closes ownership of allocated Geometry rows under Var destruction; it does not need a manager-owned side list.

## Filename/path flow

The path input to the Geometry reader is the actual DoeInputTable XML filename QString passed through the outer read call, as qualified by CP104's caller capture. For a direct Geometry child, `0x140911542` constructs `QFileInfo` from that QString, and `0x140911559` obtains `QFileInfo::path()` into the local at `[rsp+0x238]`. It then reads `UpperGeoFile`, `LowerGeoFile`, and `NominalGeoFile` attributes. The code combines the path local and the file attribute using helper `0x1400048f0` at `0x140911862`, `0x14091189f`, `0x1409119a6`, and `0x140911aea`; those QString results are passed as fields to `0x1408175c0` and copied into the geometry row. Correspondence `PointPair` children are separately parsed into QPointF pairs and sent to the same Var record through `0x140818000` and `0x140818080`.

## Payload effect still open

The Geometry reader's retained body (`0x140911532..0x140912a10`) shows QFileInfo construction/path extraction, path-string composition, metadata insertion, correspondence parsing, and local wrapper destruction. It does not show an open/read of the resulting GEO filename. The closest established later lifecycle use is destruction of the stored strings with their row; the available evidence does not bind a later consumer that opens the stored files. A Save/XML metadata consumer is distinct from loading or regenerating the GEO payload and is not substituted for one here.

## Bounded implementation action

Keep the actual-input reader's current refusal before QFileInfo construction until the candidate has an explicit source-qualified input-path value and Qt-equivalent path/join behavior. The now-closed manager/Var/row ownership chain permits a future isolated metadata-only record test, but it does not qualify public Geometry import or payload effects. If that projection is selected, keep it clearly separate from native import and accept the already-derived parent directory as an explicit input; it should test attribute/point-pair retention without opening or copying GEO files. The first production import increment still depends on identifying the later native file consumer and its mutation/ordering contract.

## Evidence limits

- Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Reader/path and Var selection: `analysis/resume50-doe-variable-body-pe-annotated.asm`, `0x14090f0c5..0x14090f0f5`, `0x140911532..0x140911ffe`, and `0x14091201c..0x140912a10`.
- Manager vtable and Var destruction: `analysis/resume91-doe-variable-owner.md`, `analysis/resume91-doe-variable-lifetime2-140919cb0.asm`, `analysis/resume91-doe-variable-lifetime2-14091db70.asm`, `analysis/resume91-doe-variable-lifetime2-140816900.asm`, and `analysis/resume91-doe-variable-lifetime2-140817d10.asm`.
- Geometry row destructor/delete: read-only bounded DUMPBIN on the hash-qualified PE at `0x140818810..0x140818845` and `0x140818850..0x1408188af`. No mutable REA, GUI, execution, source edits, build, or tests were used.
