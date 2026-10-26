# CP203 R300 editor300 cache1 and raw-step closure audit

## Decision

There is no source-ready cache1 increment in the current converter snapshot. Keep the CP163 Stopping refusal immediately after the A-array publication. The selected-length read and zero-only cache1 store are statically understood, but the current public `GuiModel::step` member and mutable pointer accessors let consumers reach the partially committed step outside a model-owned guard. A Stopping flag checked by the converted entry points would not close those bypasses.

The smallest previously identified raw-borrow prefix (`opStep` return removal) is already present: `engine_internal.hpp` exposes `withOpStep`, and a scoped search finds no `opStep(` declaration, definition, or call. That helper is not an admission barrier: it retains the nullable `opModel` lookup, invokes a callback, and documents that callers must not retain/capture the borrow. It cannot authorize cache1. The remaining step migration is not one isolated owner: a read-only source scan finds `.step`/`->step` in 41 files under the application source. A clean move to private storage would require an exhaustive no-escape migration and reconciliation of external C++ users, not just another callback around the Stopping getter.

## Current raw-access boundary

In `include/m2m/engine.hpp`, `GuiModel::step` remains a public `std::unique_ptr<DB_STEP_DATA>`. `simCtl()`, `object()` and `objectAt()` also return mutable raw pointers. `objectCount()` reads the step, and `Operation::engineModel` exposes shared model ownership. In `gui_model.cpp`, geometry admission protects `simCtl`, `object`, `objectAt`, and `reset`, but `objectCount()` directly reads `step`; callers can also read, replace, or retain the public unique pointer without entering those methods.

`GuiModelAccess::withStep` is a synchronous convention, not an enforced borrow lifetime. It checks the existing geometry barrier and then passes `model.step.get()` to a callback; its type restricts the result to `void` or `bool`, but a callback can still capture the pointer. `withOpStep` has the same escape limitation and deliberately adds no geometry check. Neither protects an alias acquired before a Stopping pending state is installed. The existing geometry barrier is therefore not evidence of Stopping closure.

The current Stopping path in `simcontrol300.cpp` validates the project lease, reader and directory receipt, executes the size query, conditionally publishes a fresh A buffer, and returns `CoordinateSync`. It explicitly leaves selected-length/cache publication unopened. The existing cached-path test checks that cache index 11 remains zero on the covered refusal/retry paths. No source or test was changed or run for this audit.

## Retained original evidence and limits

The retained original is `DEF_GUI_MO_64` v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, verified against `imports/claude-moproj2mst/evidence/original-binaries/DEF_GUI_MO_64.exe` during this read-only pass. The retained native disassembly in `analysis/resume159-editor-cachemiss-140828060.asm` shows the selected actual-step length compare at `0x140828248` and zero-only `u26c4[id]=1` store at `0x140828265`, then loader return. The recorded native hash for `[0x140828235,0x140828275)` is `f00c9d46a8a960e005bc09ce4ed3abb11314dc29b9e02bbe7bb27c32d9f628a5`.

The retained reader disassembly `[0x14082d7d0,0x14082dc8b)` places the arrays timer checkpoint, main-object assignment loop and node-timer Stop only after normal loader return. Its recorded native hash is `a3bc02528ce5ce18e576c5f938d82fc8b081616504437a8ca980001ce74dd6e4`. These retained byte views establish static instruction/order facts. They do not establish a same-occurrence public Open-to-InitialFill execution or justify returning from the converter while a later cache-hit can observe cache1.

## Exact prerequisites to advance

For an internal cache1 cut, the missing source witness is a reviewed, complete access-closure proof for this candidate: all direct `GuiModel::step` reads/writes migrated with preserved admission/effect order; `step` no longer publicly permits replacement or borrow; mutable `simCtl`/object aliases and `Operation::engineModel` paths are either confined to guarded scopes or covered by a gate that spans outstanding borrows; and every supported Stopping, load/reload, reset, viewer/display, and Save entry checks the same model-owned parked state before reading or mutating the step. Callback wrappers alone are insufficient unless their capture/escape sites are audited. Until that closure is achieved, a model-owned barrier cannot prevent a previously obtained pointer from bypassing it.

Separately, original public acceptance still lacks one same-occurrence witness binding the live Project/DB reader, actual step and selected Stopping node, query/version change, A publication, selected zero length, cache1 store, loader normal return, arrays checkpoint, recipient assignments, and reader/outer return. Historical isolated routine disassemblies do not supply that public lifecycle witness. The current app-control gate also remains release-unverified; this audit did not acquire GUI control or run the original.

## Scope and binding

Current source hashes: `engine.hpp` `77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f`; `gui_model.cpp` `26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45`; `gui_model_access.hpp` `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26`; `engine_internal.hpp` `f4e6627b808eebf31dbcf8aae0280fe1221bc50abbaa6f77639156ea704bcb0d`; `simcontrol300.cpp` `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`.

This is converter-only static analysis. It changed only this report; no source, tests, fixture, build, executable, deployment, GUI/app-control session, or live/mutable REA state was touched.
