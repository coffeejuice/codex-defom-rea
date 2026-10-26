# Editor300 public guard and next control branch (checkpoint 49)

## Public gate dependencies

The current public editor300 path remains deliberately guarded at several distinct stages:

1. `StagedEditor300::constructPageState` retains the exact Project/Operation/GuiModel/Remesh binding and records partial construction; unsupported or incomplete page state cannot be applied.
2. `EditorBaseConstruction::resume` must first finish the base QWidget constructor tail. Even after `qwidgetReturned`, it returns `ApplicationWrappersUnrecovered`; `EditorBaseReport::applicationWrappersReturned` and `completeForPublicSave` remain false.
3. The retained original wrapper contract places native model and operation QPointer setup plus `2signalChangeActiveStepData()` notification after QWidget return. Current probe evidence observes guard/notifier entry and return but does not close ordered receiver population/callback effects. No wrapper-ready state is published.
4. Native UI setup/page handoff follows these wrappers. Original editor probe 004 stopped at the first editor grid publication, before spacing/margins, full UI setup, Save, or Close. It is not an interactive control-state receipt.

Consequently the original receipts do not authorize removing the public guard or asserting end-to-end control enablement/Save parity. Static page and helper contracts can support isolated implementation and tests beneath that guard.

## Control branch disposition

The setup evidence resolves two remeshing radio checked states: Neighbor is selected in its pair; Local is selected in Global/Local. The current `editor_setup.cpp` already seeds those four checked bits. The same contract explicitly says requested enabled flags are not effective enabled state through the parent tree. No retained receipt currently closes effective remesh UI enablement after constructor callbacks/parent propagation; do not add or claim that behavior from the requested flags alone.

The stopping page's fresh native radio/spin/coordinate defaults are also already represented in `EditorSetupState` and the page handoff. `wave3-editor-stopping-partial-node-contract.json` extends that into a large, guarded callback/selection branch, but it is not a new evidence gap that should be recoded from scratch. It remains limited to the fresh native profile, configured no-active-translator/ASCII-label context, stated model/node-owner guards, and does not prove full Qt visibility or arbitrary default/callback effects.

## Smallest evidence-ready meshing branch

The smallest remaining control branch with a specific original state effect is the **disabled remembered remesh page applied to the same live process-global owner**:

- Original source evidence and `wave3-remesh-open-lifetime-contract.json` bind Open and page Apply to the same fixed process singleton (`0x143d91238`).
- `wave3-editor-remesh-owner-increment.json` maps that original process lifetime to `ConversionSession::remeshState()` and records the exact narrow behavior: when remembered `remeshEnabled` is false, Apply writes only `enabled=false`; it leaves all other current singleton fields as they stand. It requires the same external owner captured at Open. This is an adapter identity check, not an invented native error dialog/pointer check.
- The increment already has two focused cases: `initial_editor_requires_same_live_remesh_owner_before_any_apply_effect` and `initial_editor_remesh_widgets_write_same_owner_without_restoring_other_global_fields`. It is source-frozen, but the worker did not run builds/tests; the receipt explicitly assigns root integration and lifecycle/remesh/session tests.

Therefore the smallest next action is root integration and focused validation of this frozen disabled-remesh branch, not a new worker source edit. Validate the live `ConversionSession::remeshState()` accessor and Open hook, then test that false remembered state flips only `enabled` while independent later singleton changes survive, and that a missing/replaced owner refuses before any Apply effect. Keep the public editor300 guard and do not characterize this as GUI control-enable parity.

## Evidence ceiling and separate next gap

The disabled-remesh helper contract can be implemented/tested without claiming the native checkbox's effective enabled state. Probes 003/004 were passive startup observations with current user settings retained; neither edited a control or ran the event loop. They cannot establish the checkbox's initial/default enabled flag, parent-tree effective state, or an interactive toggled callback.

For a future **control-enable** change, first close the exact remesh checkbox-to-child dependency chain in retained original source: native widget identity; checked/enabled setters and parent relation; internal/automatic toggle connection and delivery order; final enabled state after the full refill sequence. Until that evidence exists, keep it as unknown. For stopping, the missing bigger dependency is full Qt callback/default visibility and completion of the public wrapper/setup chain; existing isolated MDS contracts do not close it.

## Sources reconciled

`analysis/resume47-editor300.json`; `analysis/wave3-editor46-palette-tail-wrapper-contract.json`; `analysis/wave3-original-editor-probe-004-receipt.json`; `analysis/wave3-editor-setup-remesh-radio-contract.json`; `analysis/wave3-editor-stopping-partial-node-contract.json`; `analysis/wave3-editor-remesh-owner-increment.json`; current `src/engine/editor_base.cpp`, `src/engine/editor_setup.cpp`, `src/engine/simcontrol300_owner.cpp`, and `src/engine/simcontrol300_lifecycle.cpp`. No source, build, GUI/app-control, or mutable REA changes were made.
