# RGEOM successor after the installed directory query

The next small source-closed cut is the actual arrays-timer checkpoint after the low-version loader's early return, during the **same initial invocation**. It needs no saved-step dereference or generation scheme. Keep the model barrier installed, record that timer phase once, and stop before object assignment. No recovered later continuation consumer or step-generation witness was found.

Only this report was written. No source edits, build/test execution, GUI, original execution or mutable REA access occurred. The audit read current source/tests, retained normal instruction views and bounded original PE disassembly/bytes.

## Initial borrow remains synchronous

`GuiModelAccess::withStep` (`src/engine/gui_model_access.hpp:22`) checks the model barrier before acquiring `model.step.get()`, then invokes a void/bool callback synchronously. The low-record branch in `viewer.cpp:638..646` enters that borrow before installing its own barrier. Inside it, installation precedes `step_dir_sizes`, followed by post-version and `QueryCommitted` recording. This preserves the query's conditional native version mutation before any retry can observe a lower version and take a different path.

The outer strong model and Project-owned DbHandle remain live through the call; the reader is checked against that owner and the actual step, and the directory receipt/index are current and physically bounded. The closed query itself reads directory fields/writes four outputs and conditionally the actual step version. It dispatches no callback or event. The callback does not retain its borrowed pointer. `frame->step`, assigned separately before install, remains a historical identity and is never consumed.

These arguments retain CP97/98's serialized-call qualification. Neither the public unique_ptr step nor withStep synchronizes an unrelated external replacement. A before/after pointer match is not an interval or ABA proof. Do not broaden this initial borrow into later pointer authority.

## Next native effect is closed

The retained GUI loader `140828060` calls directory query `140825440` at `14082814e`, tests its bool, then calls predicate `14082bf10` at `140828167`. Predicate returns true for the already admitted dimension-3, u1b58=0 profile when the resulting signed stored version is below 6000. The loader then returns without A allocation/body read/cache publication. No additional loader state effect precedes that return.

The predicate body was independently rechecked: it reads u1b58 at `14082bf1e`, calls dimension predicate `14035b400`, calls stored-version getter at `14082bf3f`, compares EAX with 6000 at `14082bf44`, and takes signed JL at `14082bf49` to true. Dimension predicate directly compares actual step+0x1c with 3. No new dimension assumption is needed beyond this admitted branch.

Reader caller ordering is explicit:

| Original site | Effect |
| --- | --- |
| `14082d8c0` | Call the DB loader. |
| `14082d8c5` | Set sub argument R8=0. |
| `14082d8c8` | Load literal `Assign the variable to the object data`. |
| `14082d8cf` | Select actual arrays timer global `143dcdae0`. |
| `14082d8d6` | Call existing checkpoint `140832370`. |
| `14082d8db` onward | Begin object-assignment selection/work; leave guarded. |

CP71/73 qualify that global as existing `def::model::g_timer_read_arrays`, with the same timer implementation already used by the null-companion branch. That source checkpoint appends only when count<100: one clock value, one label and null sub, then count++. A nonnegative count>=100 is a normal no-op. Negative count risks pre-array indexing in native source and remains the existing reconstruction refusal, without reset or repair.

Current low-record code returns `HighVersionDbLoaderPending` immediately after `QueryCommitted`, and existing tests require the arrays timer unchanged. Thus the native checkpoint has not yet been represented on this branch. CP78 had identified this phase as a possible successor; the present source still stops before it.

## Minimal implementation boundary for root

The exclusive source/test pair would be `src/engine/viewer.cpp` and `tests/mdt/test_viewer_box.cpp`. No shared header/CMake, public ABI, directory reader/layout, general borrow policy or step ownership migration is required. This report does not authorize or perform those edits.

Keep the existing install/query callback intact. After it returns, admit the timer phase **only if this initial call actually installed and completed the query**, returned true, and its retained post-version establishes the recovered below-6000 loader return under the already checked profile. A local completed-query marker or explicit frame phase can express that fact; do not let an unsuccessful install, prepared frame, exception or merely historical QueryCommitted receipt act as a new invocation.

Then inspect the actual arrays timer's count. For negative count, preserve the query/version/barrier and stop before timer writes. For nonnegative count, call the existing checkpoint once with the exact label/null sub and record a consumed timer phase, including the >=100 no-op case. Retain the model barrier and return object-assignment pending. The checkpoint can occur after the borrow ends: it requires no step pointer, and the same enclosing invocation still owns its model/DB lease. Do not reacquire a blocked step borrow or read frame->step.

No retry entry should consume this stage: existing barrier gates must continue refusing before fresh edge sampling, coordinate COW, table selection, timers or query. Marking the phase is bookkeeping for reached native work, not a resume API. Do not clear the barrier, requery, restore version, set cache, copy A data into an object, replace coordinates/connectivity, or expose a known scene box.

Focused acceptance contract:

- Low directory versions 5999, 0, -1 and INT32_MIN preserve their current query/version/COW effects and append the arrays checkpoint once on the initial invocation; a second refresh is effect-free and remains scene-box unsupported.
- Initial arrays counts 0 and 99 append exactly one label/null sub and increment count; unrelated timer slots/fields remain current. Do not assert a fixed clock sample.
- Counts 100 and greater execute the source no-op once without writing timer storage; negative count preserves all arrays-timer bytes while retaining the already committed query/version/barrier.
- High-record pending, null-companion, invalid receipt/owner and cached branches keep their existing behavior. In particular, no new timer appears on unqualified DB admission or a later parked retry.
- The existing broad parked-model test continues refusing after cache/version/global/Viewer/DB changes, without replaying query, either timer, COW or object assignment.

The helper `checkGeometryDirectoryPending` at test line 1817 and low-version test at line 1889 currently compare arrays-timer bytes unchanged. Adjust only the newly admitted first low-version call expectation; preserve later retry and other-branch comparisons. The parked ownership test around line 2072 also needs that first-call expectation updated, with all its later refusal checks retained.

## Later consumer and generation gap

Source-wide reads still find no resume/retire/clear operation and no later consumer of frame->step, query outputs or stage. `GeometrySyncAccess` checks presence, installs once and refuses; GuiModel reset also refuses while parked. The native suspended frame's next work is synchronous object assignment, not evidence for an asynchronous callback or later invocation.

GuiModelGuard proves model lifetime, not that public `GuiModel::step` still names the same allocation. A counter advanced only by proposed setters would miss direct reset/replacement of that public unique_ptr. No generation-only helper is justified until an actual later consumer and all replacement/destruction boundaries are recovered. The timer-only cut above deliberately avoids that issue; further object assignment needs its own current step/object/array source contract.

## Frozen read identities

Current source SHA-256 values: viewer.cpp `e2287cac017f17035e7c6f172d6f2765300a7c56ff0207b489e7b07142cdbede`; gui_model_access.hpp `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26`; test_viewer_box.cpp `2be69b66f3e1e06585fbe608f650381dc3bb2b57269a974716da44bb6731dee9`.

Original EXE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Newly checked PE intervals, end exclusive: predicate `[14082bf10,14082bf60)` SHA `ff562e929ab7898d28d029027ae8954edd2f556984392d4857c16c6c522c582d`; query-return/predicate gate `[14082814e,140828178)` SHA `0a4e7f64c9451e1797491aa3d6c3c3a1cf0e783829073e6829ac235b212c2f23`; loader-call/timer checkpoint `[14082d8c0,14082d8db)` SHA `ebb56534075131ebee57ce5598f84ff9a7d322a6e9513e306b72853577639a78`.

Controlling retained instruction views: `wave3-editor-mds-gui-coordinate-lazy-file-read-140828060.asm` and `wave3-editor-mds-gui-coordinate-lazy-read-14082d7d0.asm`; CP71/73 pin complete loader/query/timer hashes. This recommendation advances one already recovered synchronous effect and preserves all unresolved geometry continuation/public guards.
