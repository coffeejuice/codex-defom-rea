# CP190 R300 equipment independent plan review

PASS for the bounded equipment-loop lexical confinement in `analysis/resume190-r300-equipment-next.md`. No plan blocker found. This admits implementation and subsequent source review; it does not establish tests passing, original-runtime equivalence, or completion of R300.

## Frozen basis and exact ownership

Independently read the plan, current `save_current.cpp`, relevant `test_save_io.cpp` fixtures, `engine/gui_model.cpp`, retained Packet S sections 1–2, reconstructed equipment writer, and CP189 restart source review. Current owner hashes match the selected CP189 restart release:

| Owner file | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `1f7a02ece2e6f240cf5ade0339d460432e8804ba908438922a5591823b0d0298` |
| `source/apps/moproj2mst/tests/s/test_save_io.cpp` | `61405f012034b143d8d25d15ab7fb18c595bf868faa5388dcee103991f0185a9` |

These are exactly the two assigned implementation files. The CP189 restart callback at save_current.cpp:751 remains outside this change. No additional helper, writer, public API, model admission change or owner file is needed.

## Boundary and ordering

The proposed immediately invoked, nonstored void callback encloses each slot's `m->object(s + 1)` acquisition and all uses of its `eo`. It also confines the record pointer, equipment ID admission, manager item lookup, path expression and writer call. Callback-local `return` faithfully substitutes the current loop's `continue`, allowing later slots, materials, restart and NSTART to proceed. The cached `m`, ascending slot order, earlier `n`, ignored equipment status and overall object-save result `r` remain unchanged.

The object call must remain before `objEntry(op, s)` and its admission checks. `GuiModel::object` at engine/gui_model.cpp:210 first calls `requireGeometrySyncAvailable()`, then guards absent `step`, then performs the numbered lookup. The plan explicitly preserves this operation even for missing records or rejected equipment IDs. Direct list lookup, an early null-step return, or a replacement adapter would change that sequence or guard policy.

Keeping `*m->step` in the existing writer expression inside the callback preserves late step access. The allowed explicit local step acquisition must remain after manager lookup at the writer slot. A callback parameter evaluated before object acquisition would move this access earlier and is correctly excluded. Static source review must verify immediate invocation and absence of escaping aliases; stable-identity assertions alone cannot prove confinement. Cached model lifetime and destructive callback reentry remain unresolved, as the plan states.

## Manager fallback and fixture coverage

The existing `find(eq)` miss supplies empty `String()` to `saveEquipment`, rather than skipping. The plan correctly requires recording evidence for `equipmentKeyTarget(String())` while avoiding an actual native write to its cwd-derived destination. `captureKey` at save_current.cpp:77 passes a disposable temporary KEY filename to the serializer and only records the requested target, so this case can exercise the fallback without creating `.KEY` in the working directory. Native positive targets and filesystem probes remain under `M2M_TEST_SCRATCH`.

`TaskKeyRig` at test_save_io.cpp:1188 uses `Rig files{true}`. The Rig constructor selects configured corpus `49_MO2_Boolean_Oprations`, `MO2_Boolean_Oprations.moproj`, and Task00001. This agrees with the plan and CP189 correction. The proposed positive slot requires both record and numbered engine object, uses a positive equipment ID and explicit Equipment manager row, creates its parent, normalizes a path without `.KEY`, and leaves object numbers untouched. Existing direct equipment coverage demonstrates that the loaded object can be serialized; it is not fresh original evidence.

Filtering recorded writes and native attempts by the computed target avoids counting unrelated task writes. Exactly one selected equipment write, nonempty recorded bytes, native byte equality, overlay removal after successful open, empty deferred writes, preserved identities/ID/row and hook-time owner checks are appropriate. Ordering must use observed ObjProperties and a real subsequent material write when present, with a disposable restart as the reliable later boundary. The plan correctly forbids a vacuous material assertion.

Separate zero-ID and negative-ID checks with fresh/reset effects test both rejection branches while sync continues. Missing manager is a distinct positive-ID fallback case. Missing record/object remains a source-review requirement unless a direct sync mutation survives earlier Save stages; no detached-step fixture or unrelated stage edit is justified. Existing `native_equipment_denial_and_failed_flush_are_not_replayed` remains intact, preserving its direct writer denial/flush contract coverage.

## Retained evidence and execution limits

Packet S section 1 records caller `0x140057370`, engine object s+1 before record lookup, both-present positive-ID admission, manager path lookup, overwrite true, and equipment before materials/restart/NSTART. Its wrapper annotation and section 2 identify `0x140358190`, `0x140896840` and twin `0x1401c6d00`, object number at +8 and ignored wrapper result. The plan accurately distinguishes these retained annotations from current reconstructed writer behavior, including failed-open status 3. The historical `s1c.c`–`s9c.c` decompiles were not found by the bounded checkout file search; no new caller assembly, native trace or exact original-byte comparison was obtained.

Root still owns Testkit preflight, confirmation of registered focused targets, focused/full execution and freeze after implementation. This review ran no build or test and opened no GUI session or REA database. Only this report was written; source, tests, shared state, installed binaries and deployment were untouched.
