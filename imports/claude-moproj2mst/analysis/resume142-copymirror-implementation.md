# CP142 CopyMirror writer-prefix implementation

## Change

`CopyMirrorClass::writeMst` now invokes its existing `propsToEngine(project, operation)` override for a matched `CopyMirrorProps` before calling `writeBlock`. This reproduces the retained native `0x140e24e80` order: properties virtual `+0x60` (`0x140e2d230`) first, then `0x140e24f60`. The placement is before the block writer increments the simulation counter and evaluates `onlySim`, so the property mutation still occurs when that block is filtered out.

The l5 property slot is called only when the engine slot table is installed. The packet-standalone P12 text-writer test intentionally links no engine slots and has no model; this case preserves that harness behavior. In the full converter, the engine slot table is installed before modeled writer execution. No registry or public Open/Save guard was changed. CopyMirror remains unregistered in the reconstructed public registry.

## Focused regression

New `tests/mdt/test_copymirror_property_writer_prefix.cpp` opens retained `42_vdie` as a read-only fixture, retags an operation only inside the test for direct private writer exercise, and sets nondefault values through the existing CopyMirror property `applyValue` slot. It calls `writeMst` with `onlySim=99`, then checks the simulation increment, empty output, and mutation of axis, conversion kind, and selected extrusion fields on a real non-kind-7 engine object. This isolates the pre-filter effect without claiming that particular MST bytes depend on those values.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/mdt/test_copymirror_property_writer_prefix.cpp` passed with no include/testkit issues. No compiler, CTest, build, original executable, GUI, or mutable REA operation was run; root owns build and test validation.

## Remaining boundary

The direct native writer call and candidate mutation body are source-qualified. This increment does not establish which output bytes are field-sensitive, raw parity, CopyMirror public Open reachability, or a reason to register the class. The downstream byte question remains separate.

## Write set and hashes

Only the assigned existing writer source, one new focused test, and this receipt were edited:

- `src/ops/copymirror.cpp`: `AA2308F131F94D78284B60CD80A05B780782A5F28CD0B341268B7A8BC9BC07A9`
- `tests/mdt/test_copymirror_property_writer_prefix.cpp`: `B246F94CEB349398F3FFC297A7487D24CF9A0BDF0080FB1A7DA01EA6E03C753E`
- `analysis/resume142-copymirror-implementation.md` (SHA-256 reported with the freeze receipt)




