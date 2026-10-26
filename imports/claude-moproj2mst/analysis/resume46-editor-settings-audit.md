# Editor300 settings audit (checkpoint 46)

## Scope and evidence

This is a read-only reconciliation of the existing editor300 receipts for the settings/control-enable, step/stopping, meshing, and environment portions of InitialEditorSession. It does not alter source, build, launch a GUI, or mutate REA. The subtree's original-runtime probes 003 and 004 both reached native CLI startup Open through first editor-grid publication and then cleaned their owned process. They did not interact with controls or reach complete editor setup/Save. Both explicitly retain current user settings, and both qualify raw widget layout/dirty/event bits as not fully sourced. Probe 003/004 module identity includes original DEF_GUI_MO_64.exe SHA-256 `fd8ae1f8…64725acc` and QtGui4_64.dll SHA-256 `c360c971…44725887`.

The probes therefore establish endpoint/startup observations only. They do not establish that settings were defaults, that enabled controls were effectively enabled, or that applying any changed setting writes a particular model value. The converter remains behind its public editor300 guard; these reports describe isolated/recovered helpers, not full lifecycle parity.

## Reconciled evidence

- **Settings and enabled state:** `wave3-editor-setup-remesh-radio-contract.json` resolves checked state for only two radio groups (Neighbor selected; Local selected). Its explicit limitation is that requested enabled flags do not equal effective enabled state through the parent widget tree. Other controls retain `requestedChecked` with `checked=nullopt`; spin/combobox/line-edit captures are not grounds to invent defaults. The original probes add no interactive/default-settings observation.
- **Step size and stopping:** `wave2-save-diffs-300-step-stop-increment.json` records source-bound 3D helper behavior and ordered partial writes. Step-size Apply writes mode and step fields in a specific order; only `Invalid` stops the sequence, preserving earlier writes. Stopping Apply similarly has ordered TMAX/SMAX/VMIN/LMAX/EMAX/MDS/rotation effects; movement-warning combinations return UnsupportedSnapshot after a prefix. The result is bounded to the listed 3D, locale, object/table, finite-value and selection guards. The public Save path is still refused, so these helper contracts do not prove the user-visible dialog path.
- **Meshing:** `wave3-editor-remesh-owner-increment.json` maps the original process singleton to `ConversionSession::remeshState()`. Remembered page widgets are copied back into the same owner; disabled remembered remesh changes only `enabled=false` and retains other current global fields. This owner contract was source-frozen for root integration; its listed tests were not run by the worker. It is not evidence of effective GUI enabled state or full Open/Apply/Save behavior.
- **Environment:** `wave3-editor-environment-table-contract.json` proposes explicit selection plus constant input/label and definition-enable state for the first three curve controls, while preserving actual remembered curve owners. The increment is qualified static-only and leaves ACVCOF constant mode unchanged. `wave3-editor-environment-table-increment.json` is an isolated helper increment, not a GUI capture. No receipt establishes effective parent enablement, user-edited text, or the final model value after an interactive Apply.

## State still unbound

The evidence does not bind the following state to a complete reachable Open → edit → Apply/Save sequence:

1. Actual fresh-session/default persisted settings for each control, including any settings file or process-global values consumed before editor construction.
2. Effective enabled/disabled state after parent-tree propagation, callbacks, translator/resource setup, and final page refill. Only requested flags and two radio checked states are qualified.
3. Initial and changed numeric/text values for every step-size and stopping control, including locale/validator behavior outside the qualified finite KnownC 3D profile.
4. UI-to-model changes and partial-failure ordering through the public Save/lifecycle path, especially MDS existing-object callbacks and PDIE warning answers.
5. Effective remeshing control state and complete native owner effects under actual enabled/disabled user interactions; only the helper's same-owner mutation rule is specified.
6. Environment constant/curve control enablement and actual selected curve/value effects after final refill and Apply. The contract's proposed state is not an observed user interaction.
7. Constructor/refill state after the first-grid probe endpoint, including spacing/margins and later initialization that may change widget flags or values.

These are evidence gaps, not assertions that the implementation is wrong. Keep explicit unsupported profiles and qualified unknowns separate from established defaults.

## Smallest useful next setting case

The smallest discriminating case is one valid 3D, KnownC step-size scalar: record its filled text/mode, change only that scalar to a distinct finite value, invoke the actual Apply path once, and record the corresponding DTTYPE field plus all fields touched before/after it. This isolates a concrete widget-to-model write and ordered-prefix behavior without combining stopping, meshing, or environment branches. Preserve a fresh-process/settings receipt and compare the unedited baseline with the one-field edit. It cannot be run through GUI input under the current release gate; the current GUI queue has `release_unverified` and no available cases. The case definition is therefore a test recommendation, not a claim of runtime verification.

### Qualification

Read-only basis: converter `AGENTS.md` and `IMPLEMENTATION_PLAN.md`; original-editor-probe receipts 003/004; the setup-remesh radio contract; step/stopping increment; remesh-owner increment; environment-table contract/increment; and independent `gui_validation_status.py` output. No source, build, app-control, or REA mutation was performed for this audit.
