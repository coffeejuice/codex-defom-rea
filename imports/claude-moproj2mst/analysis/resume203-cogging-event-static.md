# CP203 RCOG: event-pump, CWD, and operation witness audit

## Disposition

No narrow public source change is justified from the retained evidence. CP156 established a concrete static CWD-writing route reachable from the Save event pump, while CP171 refined the gap: neither queued File > Open delivery during that specific pump nor the post-pump Project/active-operation identity is known. CP201's candidate still represents the Save poll only through an explicitly supplied schedule adapter; this is not Qt event-queue behavior or a source-bound Open callback. The exact missing item is one same-run original witness that correlates queue delivery, CWD transition, Save owner/operation selection, and the Cogging relative existence result.

## Reconciled source behavior

- `source/apps/moproj2mst/src/engine/save_event_prefix.cpp` models one `processEvents(flags=0)` boundary at native site `0x140098129`. If a dispatcher is present, `SaveEntryEventPrefix::run` requires a supplied `SaveEntryDueObservation` and dispatches its IDs once. A missing observation is an error; an empty list is meaningful only when explicitly qualified. This is an adapter contract, not a native queue observation.
- Its tests in `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp` use `DeclaredAdapterSchedule` and a test callback. They establish ordered one-shot adapter behavior only; they do not prove any event is due, File > Open is dispatched, or a callback changes CWD or Save state.
- The native static route in `analysis/resume156-cogging-cwd-event-next.md` is: Save pump -> main-window File > Open slot `0x140025b60` -> dispatcher `0x14001ca30` -> `.moproj` open route `0x14008a9b0` -> `QDir::setCurrent(projectDir(P))`. This proves capability, not actual delivery during Save.
- `source/apps/moproj2mst/src/engine/cogging_save.cpp` currently constructs `projectDir(p)/MAN_COUPLE.DAT` and probes that explicit path. The retained original evidence describes a process-relative `QFile::exists("MAN_COUPLE.DAT")` after the event pump. Replacing the candidate path with either the original project's directory or a presumed newly opened project's directory would assume away the unresolved schedule and ownership effects. Keep public Save guarded at the existing evidence boundary.
- `analysis/resume201-root-integration.md` records the CP201 frozen source digest `2e90ced71ce9e116259a45104df0ceffb2bdd8b94d2a0167f10c0b39636df1de`. Its passive original Save-route helper is explicitly not admitted for an original run due to a watchdog termination receipt defect; no same-run witness is present in that record.

## Exact witness still required

A single controlled original Save invocation must retain the same process/run identity and input/project hashes while recording:

1. Immediately before and after the Save `processEvents(flags=0)` at `0x140098129`: process CWD, Project identity, active-operation identity, and event-pump entry/exit.
2. Every `QDir::setCurrent` inside that interval (caller and target), and whether the queued File > Open handler actually ran.
3. At the first post-pump active-operation resolution and at `0x140052ec0`: the Project and operation identities selected by the continuing Save.
4. At `0x140762f30`: the actual CWD and result of relative `QFile::exists("MAN_COUPLE.DAT")`.

For route discrimination, queue Open of a second disposable project with `MAN_COUPLE.DAT` present only in its directory. Record Open outcome separately from the still-running Save outcome. A candidate-supplied event schedule or a private injected existence bit cannot substitute for this correlated original trace.

## Evidence boundary and next source step

Until that witness exists, leave `SaveEntryEventPrefix` as an explicit schedule adapter and do not promote its callback into native Qt dispatch semantics. Do not wire the public Cogging lookup to `projectDir(p)` as a presumed behavioral correction, and do not infer post-pump operation selection from CWD alone. Once the witness is available, the narrow implementation decision is to encode only the observed event/CWD/owner transition at the existing Save-entry/Cogging seam, with separate assertions for lookup directory and selected operation; otherwise the exact missing same-run witness remains the disposition.

## Sources

- `analysis/resume156-cogging-cwd-event-next.md`
- `analysis/resume164-cogging-next.md`
- `analysis/resume171-rcog-next.md`
- `analysis/resume201-root-integration.md`
- `source/apps/moproj2mst/src/engine/save_event_prefix.cpp`
- `source/apps/moproj2mst/include/m2m/engine/save_event_prefix.hpp`
- `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp`
- `source/apps/moproj2mst/src/engine/cogging_save.cpp`

Read-only audit. No source, tests, build, GUI/app control, original process, or mutable REA state was changed or used.
