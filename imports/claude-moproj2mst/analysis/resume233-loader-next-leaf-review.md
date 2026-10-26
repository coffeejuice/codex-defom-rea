# CP233 loader next-leaf review

## Verdict: PASS, with two implementation details to preserve

The revised contract resolves the prior guard ambiguity. It explicitly treats the command as a synchronous continuation of the already-admitted `decideAndLoad` DB path, omits a second geometry guard, and explains why: `simCtl()` performs the existing admission before any `nstart` mutation, while adding a later throwing check would change the failure prefix. The documented no-intervening-replacement/barrier-transition precondition is consistent with the source path; the review does not extend this to concurrent or reentrant callers. Keep this method scoped to that route and non-throwing by construction.

The exact assignment order remains correct: after `loadDbStep` and the existing model/control refusal, compute and store adjusted `nstart`, write the loaded step number, then write `tnow3`, `tnow2`, `nstartGiven`, and `startStepNo`, before class-specific work. The command body should remain only `step->u744 = stepNumber`; no null default or catch should be introduced. `<cstdint>` is already included in `engine.hpp`, and no C ABI change is involved.

The revised test proposal addresses the prior coverage gap by extending `oracle_b_124_db_step_load`, which follows `openActiveOperation` through the actual DB branch. Assert `engine::opModel(o.p, *o.op)` is non-null and check the model step number. At implementation, first confirm which first/last query the fixture selects from `idx`/`lastMatch`, then derive the expected number from that query or a fixture record; do not assume that a lower-level `readDbStep` assertion proves the later `decideAndLoad` write. Keep existing lower-level reader and rejection coverage. Resolving the registered CTest name from configured metadata at implementation time is an adequate explicit verification step.

One access detail: because `decideAndLoad` is a free function in `loader.cpp`, a private command is usable only if the class grants it access or it is reached through an existing private/internal bridge. Otherwise declare the narrowly scoped method public as an internal C++ API; do not broaden it into a general step setter. This is an implementation access choice, not a contract blocker.

Evidence remains source-based; no fresh original runtime claim is made. No source, test, build, shared state, original, GUI, or REA artifacts were changed, and no tests were run.
