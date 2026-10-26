# CP112 — Geometry receiver binding from retained REA exports

## Increment over CP107

Stack mapping resolves `[rsp+0x1030]` inside `0x14090ee20` to the reader's saved third argument (`R8` at entry): the prologue saves `R8` at `[rsp+0x18]`, then reserves `0x1008` and pushes two registers, making that saved slot current `[rsp+0x1030]`.

At the known caller `0x14090e080`, the prologue saves its second argument (`RDX`) at `[rsp+0x10]`, then reserves `0x398`; this maps that same argument to `[rsp+0x3a8]`. The callsite `0x14090e475..0x14090e48d` loads `[rsp+0x3a8]` into `R8` and calls `0x14090ee20`. Thus the Geometry receiver is exactly the second argument (`param_2`) of `0x14090e080`; the indirect slots `+0x178` and `+0x1a0` are not on an arbitrary local or newly allocated reader object.

The retained REA export `evidence/gui_mo/s1/mdt/DOE/d0.c` supplies one wrapper edge: `FUN_1400d3200` calls `FUN_14090e080(uStack_28, param_1)` (around its line 252). Therefore the receiver is that wrapper's `param_1`. The same export's `FUN_1400d3200` body also dispatches several virtual methods on `param_1`, consistent with this object being the DOE model/manager passed through the wrapper. This binds identity through the call chain, but not concrete runtime class or lifetime.

## What remains unclosed

Neither `d0.c` nor the retained bounded caller/reader PE captures show the construction/vptr write for the specific `param_1` supplied to `FUN_1400d3200`, nor its destruction or a caller-specific vtable constant. The receiver's concrete `+0x1a0` target therefore remains dynamic. The native `0x1408175c0` helper proves list-record allocation/insertion into that receiver's `+0x28`, but the chain from manager destruction to destruction of those records remains unproven.

No later open/read of `NominalGeoFile`, `UpperGeoFile`, or `LowerGeoFile` is present in the retained Geometry body. The available export does not include a later consumer keyed to those stored names. A pure XML parser/unit test would not close either missing owner edge, so no isolated test is proposed in this lane.

## Exact next executable action

Starting from the actual caller of `FUN_1400d3200` (not merely its reader wrapper), recover the concrete `param_1` construction and vptr initialization from immutable original PE/retained REA export. Resolve that object's vtable slots `+0x178` and `+0x1a0`, then follow the same object's destructor/container teardown and any filename-field consumer. If the caller is virtual/dynamically selected and no static vtable value is recoverable, record that dynamic type as the remaining requirement; do not infer deletion from insertion.

## Evidence scope and qualification

- Original GUI binary SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- `analysis/resume50-doe-variable-body-pe-annotated.asm`: reader prologue at `0x14090ee20..0x14090ee4f`, virtual slot reads at `0x14090f0c5..0x14090f0f5`, where `[rsp+0x1030]` is the saved `R8` argument.
- `analysis/resume104-doe-geometry-path-caller-14090e080.asm`: caller prologue and `R8` load/call at `0x14090e475..0x14090e48d`.
- `evidence/gui_mo/s1/mdt/DOE/d0.c`: retained REA pseudocode call from `FUN_1400d3200` to `FUN_14090e080(uStack_28,param_1)`; decompiler output, not a concrete runtime type witness.
- No mutable REA session, GUI, execution, source edit, build or test was used.
