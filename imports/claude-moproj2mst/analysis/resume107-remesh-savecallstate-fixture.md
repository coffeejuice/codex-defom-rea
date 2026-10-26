# CP107: SaveCallState at the remesh failed-open boundary

## Boundary and native qualification

The retained original Save order (`analysis/wave3-boolean-save-io-1400980d0.asm`, tied to the verified `DEF_GUI_MO_64.exe` hash in `analysis/wave3-remesh-save-lifecycle-contract.json`) is current-operation Save, project writer, operation properties, main MST writer, then the enabled remesh writer. The project/MST status locals are combined, but a false result from either does not skip the remesh tail. The native code has no `SaveCallState` object; candidate fields below are instrumentation of preceding adapter decisions, not recovered original fields or original GUI status.

At enabled remesh `fopen("wt")` failure, the first `fprintf(NULL)` reaches the CRT invalid-parameter dispatcher. For the qualified initialized-null/default-handler path, this ends in `TerminateProcess(..., 0xC0000417)`. Under that conditional handler profile the original does not return an ordinary Save status and does not reach later writes, `fclose`, GUI status reporting, or normal Close. A custom handler that returns is a separate unbound runtime profile. See `analysis/resume103-remesh-terminal-close-order.md`; no original fault was executed.

## Candidate SaveCallState snapshot

For a public candidate conversion that reaches the same remesh boundary, successful Open and validation are prerequisites. Given at least one validated sub-project and no guarded last-operation modal, the candidate state at remesh dispatch is:

| Field | Expected value at the boundary | Qualification |
|---|---|---|
| `opened` | `true` | Candidate Save validation observes the opened project. |
| `checkedSubprojects` | Number of sub-project checks that returned normally | Do not hard-code a count without binding the selected fixture. |
| `validationComplete` | `true` | A pending warning/modal guard exits before project/MST/remesh. |
| `answer` | `DialogAnswer::No` | Candidate normal validation assignment; not the original GUI Save status nor evidence of a delivered modal answer. |
| `lastOperationModalPending` | `false` | The guarded must-be-last path sets it and throws before Save writers. |
| `pendingDefault` | `DialogAnswer::NoButton` | Default state on the reachable path. `Yes` is set only by the pending warning path, which does not reach remesh. |
| `projectWriteAttempted` / `projectWriteReturned` | `true` / `true` | Project false is a returned writer result, not an exception that exits before remesh. |
| `projectWriteSucceeded` | `true` or `false` | Preserve the actual candidate writer observation. A scripted post-close error may produce false after complete physical bytes; do not equate false with no output. |
| `mstWriteAttempted` / `mstWriteReturned` | `true` / `true` | The MST branch marks both before invoking remesh. |
| `mstWriteSucceeded` | `true` or `false` | Preserve actual candidate writer result. False can mean open failure or post-open stream failure with different residue; this state bit alone does not distinguish them. |

Thus the combined-failure cell is project attempted/returned/false and MST attempted/returned/false at remesh dispatch. The other reachable cells retain the same validation and attempt/return fields while varying either success bit independently. In all cells the candidate remesh guard precedes its adapter error mapping: it reports `M2M_E_UNSUPPORTED` before an earlier `M2M_E_WRITE` for MST or project. This is candidate policy, not an original returned status.

`SaveCallState` contains no remesh-open, invalid-parameter, or terminal-outcome fields. The separate candidate `RemeshSaveReport` records `opened=false`, `invalidParameterReached=true`, and an unsupported outcome for the guarded refusal. When an injected `RemeshTerminalCallService` is installed, `RemeshTerminalForwarder` snapshots the live `SaveCallState` at dispatch; that snapshot is taken before the remesh function sets its `invalidParameterReached`/outcome fields. A returning endpoint enters the service's fail-closed path. The ordinary public conversion has no such service and returns the safe `M2M_E_UNSUPPORTED` adapter result.

## Modal/default and test limits

A test of this boundary must use a project that completes validation without `lastOperationCheck` selecting the must-be-last warning. The candidate's pending-default `Yes` belongs to that unrecovered warning case, which currently throws before file writers. Do not assert it at the remesh boundary. The terminal default-handler claim remains conditional on the original process actually retaining the initialized default handler; the static evidence does not exclude later replacement, a debugger, or external modification.

Existing `tests/s/test_save_io.cpp::save_project_postclose_error_then_remesh_open_failure_keeps_prior_outputs` reaches the boundary with project false/MST true and asserts the candidate guard/result. CP99 covers a project/MST dual false cell with disabled-remesh deletion denial, not enabled failed-open. The isolated writer test proves its `RemeshSaveReport` fields, while `tests/mdt/test_core_remesh_terminal_bridge.cpp` proves snapshot copying using a synthetic state; neither asserts a real public dual-false snapshot at enabled failed-open. A public candidate acceptance can combine the existing project callback error and MST open denial, keep remesh enabled and locked only after Open, then assert this table at an injected terminal-service dispatch. Keep that result distinct from the conditional native process termination and from any original GUI status.

No source/test/build, GUI, original execution, or mutable REA action was performed for CP107.
