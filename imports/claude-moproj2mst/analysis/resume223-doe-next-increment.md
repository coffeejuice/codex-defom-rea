# CP223 DOE next-increment assessment

## Decision

No independent source-backed Type2-study Open/Save implementation increment is ready that avoids the missing B120 Add-Study witness and does not duplicate integrated DOE work. Keep the current Type2 Study lifecycle guards. The smallest useful next action is a serialized, fresh-process original capture of the B120 Add-Study action that binds the pre-action project tree, exact user action, created study/variable/geometry state, and subsequent Open/Save call sequence to the same process and project identity. Until that exists, retained B120 after-state files cannot serve as pre-action input or establish lifecycle ordering.

## Current queue and prior integrations

The active DOE dependency record, `analysis/wave3-doe-next-active-study-dependency.json`, says no active-study variable/output/optimization branch is currently reachable. Its listed candidates are already integrated as detached/direct helpers or depend on the guarded native Report/TaskInfo construction chain. It explicitly says to retain the native Report guard and wait for the original factory/QObject witness before integrating the constructor frame and selecting the first reachable branch.

CP215 already integrated the lazy-reader `StudyIo*` identity check at the Type2 notifier boundary (`source/apps/moproj2mst/src/doe/activation.cpp` and `tests/doe/test_keyword_notifier_entry.cpp`). It is a narrow identity guard; it does not supply a new study or notifier callback.

CP218 already stages the source-qualified operation-directory decisions in per-call Open state (`source/apps/moproj2mst/src/model/operation.cpp` and its operation-directory test). It deliberately does not perform filesystem writes or enter `FileEffects`; CP217/219 lack the same-occurrence original creator/failure-order witness needed to activate those effects.

CP222's validation identifies its own active successor as editor ownership migration (viewer geometry-type read), not a DOE task. No evidence indicates an unowned DOE production seam was integrated there.

## B120-specific limitation

The retained gap qualification (`analysis/resume102-retained-gap-qualification.md`) establishes that B120–B122 after-states include GUI-created study/variable/geometry state absent from their before trees. The historical B120 comparator remains a DIFF; it is not fresh-original acceptance. Thus no implementation can safely infer what the Type2 study Open/Save sequence should consume or produce from the after-state alone. The independent `wave3-doe-next-active-study-dependency` blocker is earlier still: native first Report construction remains guarded before TaskInfo initialization.

## Smallest evidence action and acceptance

Acquire one controlled original occurrence only after the coordinating owner schedules it: capture a fresh original process and disposable B120 copy before any GUI action; perform the exact Add-Study interaction; record the newly created study and child files/state; then bind the subsequent project Open and explicit Save calls, their same-project Study identity, and before/after paths to that occurrence. Preserve raw receipts and cleanup evidence. The decisive acceptance is a reviewer-verifiable same-occurrence binding that distinguishes Add-Study-created inputs from Open/Save effects and supplies source/call ordering for one concrete Type2 branch. If capture shows the existing native Report/QObject prerequisite is still unmet, stop at that dependency and do not add candidate lifecycle behavior.

## Scope and files

No production or test file is proposed for modification. A source edit before the evidence action would either duplicate CP215/CP218 or invent reachability/effects. This assessment only writes `analysis/resume223-doe-next-increment.md`; no build, test, GUI, original process, or mutable REA action was performed.
