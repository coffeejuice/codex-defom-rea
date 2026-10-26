# CP185 connector independent source review

**PASS — no source blocker found for the CP185 single-instruction cut.** Integration execution and source-stable freeze remain pending; this review does not claim that tests ran.

Reviewed `resume185-connector-next.md`, `resume185-connector-plan-review.md`, and `resume185-connector-implementation.md` against `source/apps/moproj2mst/src/doe/task_factory.cpp`, `include/m2m/doe_task_runtime.hpp`, `include/m2m/doe_task_factory.hpp`, and `tests/mdt/test_doe_task_factory.cpp`.

The retained qualification in `resume163-connector-qobject-static.json` identifies QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its bounded initializer listing has `48 83 c4 30` at RVA `0x13123a`, `pop rdi` at `0x13123e`, and `ret` at `0x13123f`; the caller listing has call RVA `0x131417` and continuation `0x13141c`. These are static unwind-region facts, not live stack observations or proof of the full logical function.

## Contract checks

- `task_factory.cpp:210` validates live RSP against phase: `S-0x38` before ADD, `S-8` at `BeforePrivateRdiRestore`. Saved RDI remains `[S-8]` and the return slot `[S]`. The historical saved-RBX MOV relation remains separately `S-0x38`, `[S+0x10] = [RSP+0x48]`; it is not incorrectly advanced with live RSP.
- The instruction loop at lines 570–650 has CP184 ordinal 6 and CP185 terminal ordinal 7. Case 6 assigns only live displacement `-8`, then advances phase to `BeforePrivateRdiRestore`; the next iteration validates the retained frame before refusing at `0x13123e`. The default budget is 7 and the ordinal guard rejects larger values. There is no POP, RET, caller-continuation, RDI-value restoration, or backlink publication in this cut.
- Entry checks at lines 184–213 preserve the CP183 result guard while permitting both successors. Saved task RBX provenance checks the store control block independently of pointer equality, pending Report ordinal and identity, module/caller/initializer/return RVAs, and saved stack relation. The loop independently validates allocation-bound private/RAX identity, task/RBX identity, root bindings/control blocks, version input, sparse state, historical flags expressions, and incomplete/unpublished state before mutation or terminal refusal.
- Case 6 and phase publication are nonthrowing scalar assignments. No private store, allocation, ownership binding, shared reference, map, completion flag, or historical `flags20Eax`/`private20` expression changes. The source explicitly documents that ADD affects arithmetic flags and that this frame has no architectural RFLAGS model; it correctly invents no concrete flags or stack contents.
- Native `createTaskBase` initializes before requested-key lookup. Same-key, different-key, and unknown-key reentry therefore all validate and refuse at the retained terminal. A lowered budget leaves CP185 terminal intact; explicit test rewinds restore the matching displacement and retain CP184 RBX or clear stage-dependent results before earlier phases. Resuming from CP184 executes ADD once.

## Test-source coverage

The three CP185 cases beginning at line 5290 cover the CP184-to-CP185 transition, complete represented frame/private/owner/reference/allocation/cache snapshot preservation after restoring only phase and live RSP for comparison, repeated same-key and other-key terminal refusals, lowered budget, explicit CP185-to-CP184 rewind/resume, 40 corruption cases at both predecessor and terminal, and all earlier flag-budget rewinds. The corruption matrix includes absent/foreign restored RBX, historical saved-RBX provenance, wrong live displacement, saved-RDI and return slots, private/RAX identity, version provenance, sparse flags, roots, allocation size and premature backlink availability. Existing CP183 and CP184 boundary cases remain explicit.

The shared snapshot helper includes the three new symbolic stack fields, historical flags and return/restored identities, saved-RBX values/control block, version provenance, sparse private fields and weak-owner bindings, retained counts, cache counts, and shared-null reference deltas. Testkit entry point and supported `CHECK`/`REQUIRE` assertions are present; no new C `assert` or `REQUIRE_EQ` is used. CMake registers this file through the existing MDT test glob and links `m2m_core` and `testkit`.

Reviewed SHA-256 identities:

| File | SHA-256 |
|---|---|
| `src/doe/task_factory.cpp` | `3d524382330df0108193b7bddc22fde33dff9128cc5cba44f5187bb31c3a760d` |
| `include/m2m/doe_task_factory.hpp` | `5563744c8bd0643d90adc6b0cab6e9651fc1105ad4b0249813b8cf6f5463f9d9` |
| `include/m2m/doe_task_runtime.hpp` | `421f2cbc932ec9c3668e968227fa385118dd7c6268b335f41732bb105a8b6048` |
| `tests/mdt/test_doe_task_factory.cpp` | `beb551f40198605ea2ad952d7f888aa8184a187b0e1de59cf9978f26f2e67fd7` |

Only this review report was written. No source edit, build, test execution, GUI interaction, REA database operation, deployment, or shared-state update was performed. Original comparison, focused execution, full regression, bound receipts, and source-stable freeze remain integration requirements.
