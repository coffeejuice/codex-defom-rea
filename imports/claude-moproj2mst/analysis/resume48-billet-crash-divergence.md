# Case 004 crash divergence and minimal case 005 record

## Earliest checkpoint supported by retained evidence

There is **no instrumented shared checkpoint** in case 004 that proves where the original and candidate state first diverged. The original GUI trace provides these observations:

1. The first Open attempt populated the native chooser with a forward-slash path. At 11:22:35Z the chooser rejected it as an invalid filename; the actual project had not yet been opened. The observed default `OK` dismissed that validation dialog.
2. A corrected Windows backslash path was selected. At 11:22:56.628Z the main window was titled `DEFORM Multiple Operation`; subsequent capture records title `BILLET - DEFORM Multiple Operation`, establishing that the project reached a rendered-open state.
3. At 11:22:56.7123126Z the original faulted at QtCore4_64.dll+0x12a0e4 with `c0000005`; later exception reporting includes `c000041d` at the same offset. There are no fault registers, application stack, or phase markers in the retained evidence. It cannot show whether object construction, editor setup, an event callback, or some later Open tail was in flight.
4. A Save-stage screenshot/capture was attempted at 11:23:03.834Z, after the first fault. This does not mean Save ran. The original observation marks the action crashed and says the PID/window disappeared; Close was unavailable.

The static disassembly identifies the faulting leaf instruction as a dereference in Qt's `QObject::metaObject`-adjacent region, but the export proximity is not a callsite attribution. The retained crash-events list includes repeated WER rows for the same PID/start time, not independent trials. Thus case 004 establishes one crash after visible Open and before verified Save, with the exact internal crash boundary unknown.

Candidate evidence begins later, at 11:25:01.815Z, in another process/cwd. It records one CLI call to frozen `moproj2mst.exe <BILLET.moproj> --write-save`, exit 0, and declarative Open/Save/Close completion. `candidate.log` is empty. There are no per-stage candidate checkpoints, no comparable GUI state, and no internal state dump. Its first candidate observable is therefore process start/command, not a shared Open milestone. The raw filesystem comparison reports one byte-changed file (`BILLET.moproj`, first differing offset 3428) and metadata changes; this is an end-state difference, not evidence locating the earliest runtime divergence. Conclusion: the minimal defensible divergence statement is **original visible-open followed by crash; candidate adapter full-save call completed later; internal first divergence is unlocalized**.

## Smallest fresh disposable case 005 preflight

Use the same small training fixture and same single BILLET project, but prepare a new append-only harness case. Do not reuse the mutated 004 sandboxes. Copy from the immutable imported fixture into fresh original and candidate subtrees, verify all project and side-input SHA-256 values against the newly recorded fixture/input manifests, bind the exact original EXE hash and one frozen candidate build receipt/binary set, then run harness preflight. Keep the native path spelling correct from the first chooser entry. Confirm no already-running original process, no existing project in that process, current settings/locale/code page, original identity and module hashes, license availability, actual cwd, and exact target path before starting.

For this narrow attempt, the primary acceptance record is a phase ledger rather than a presumed pass:

| Phase | Required evidence | Pass condition |
|---|---|---|
| Initial state | PID/argv/cwd, EXE and loaded module identities, locale/code page, settings snapshot, initial window, no prior project | All observations bind to the owned fresh PID; unknown fields explicit |
| File Open | Exact backslash path, chooser selection, dialog text/default/action, screenshot, project-read/open status and visible project title | Open is positively observed; a rendered title alone is recorded as `visible_open`, not internal Open completion |
| First post-open stable point | Screenshot/window/process status and short timestamped observation after UI settles, before Save | Process remains alive and project remains selected; this distinguishes case 004's immediate post-open failure window |
| Save | Actual Save action, resulting messages/default dialog answers, project/cwd manifests | Save stage only if original PID remains alive and completion is observed; otherwise retain failed-prefix and partial writes |
| Close | Actual Close action, observed default only, final PID/window and project/cwd manifests | Close completion positively observed; do not infer from process disappearance after crash |

The minimum useful result can be an original-only prefix: if case 005 reproduces the fault before the first stable post-open point, record exact phase, timestamp, WER/application event and available fault context, then stop. Do not continue key attempts against a dead process. If the app remains alive after stable Open, proceed once to Save and Close using observed defaults. Only after the original receipt is frozen should the candidate be run in a separate disposable copy/cwd; its outcomes remain a separate column and never fill missing original stages. Compare raw project and cwd bytes first; list metadata independently and label outside-project write coverage/ordering gaps explicitly.

To improve localization without widening this one-project test, retain a crash dump/register/context record if the supported runtime capture already available can do so safely; otherwise document it unavailable. Do not add a debugger, mutate the GUI, or infer that the candidate's internal call sequence matches the GUI's. Do not make an editor-constructor probe or native CLI-open probe an acceptance substitute: either can answer a narrower diagnostic question but changes the path/timing under test.

## Capture controls and limits

- Case 004's existing user settings were not reset. Record current values; a new process gives project-state freshness only, not pristine settings.
- Case 004 used a long path under the harness tree and native chooser behavior was part of the failure history. Preserve the actual exact path. If path shortening is required, make it an explicit case variable and bind it in the receipt rather than silently changing directories.
- BILLET is a training case selected for a small HeatTransfer project; it is not held-out evaluation. A successful run validates only this input and this runtime state.
- Case 004's candidate receipt has exit 0, but `candidate.log` provides no execution trace and the receipt's stages are operator-declared. Do not claim internal cleanup from this alone.
- Project/cwd manifests do not capture all registry, recent-project or license-log effects and cannot prove write order.

This analysis reads retained evidence only. No app launch/control, source/build change, candidate execution or mutable REA operation was performed.
