# Editor300 wrapper QPointer and notifier gate (checkpoint 48)

## Finding

The retained original PE evidence supports a specific post-`QWidget` chain for the first 3D editor factory instance. It does **not** establish a completed editor constructor or the full notifier receiver population. The current implementation correctly keeps this chain behind a hard initialization gate: `EditorBaseConstruction` may report `qwidgetReturned`, but `applicationWrappersReturned` and `completeForPublicSave` are compile-time false, and `resume()` returns `ApplicationWrappersUnrecovered` after the widget shared tail. This report concerns wrapper QPointer/notifier ownership only; it does not depend on the gradient-brush branch.

## Original ownership and call order

Static evidence is tied to retained `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The relevant disassembly is in `wave3-editor-native-owner-140130740.asm`, `wave3-editor-native-parent-140130830.asm`, `wave3-editor-native-parent-140377950.asm`, and `wave3-editor-owned-open-model-link-140377b20.asm`; the contract crosswalk is `wave3-editor46-palette-tail-wrapper-contract.json`.

1. The factory constructor returns a QWidget before application wrappers run. The first wrapper initializer at `0x140377950` installs `SfWidget` wrapper vtables, initializes the model guard at wrapper `+0x28` to null through `0x14015d170`, sets its enabled byte at `+0x34`, applies the remaining wrapper defaults (`+0x30=0`, `+0x35=0`), and routes the null model through `0x140377b20`.
2. `0x140377b20` delegates to `0x140cbc920`, which compares the stored guard pointer and calls Qt `QMetaObject::changeGuard` only when the pointer changes. This is a native QObject-tracking QPointer effect. A raw `GuiModel*`, `shared_ptr`, or the converter's `GuiModelGuard` by itself does not reproduce this registration/update/teardown contract.
3. The operation wrapper initializer at `0x140130740` installs operation-wrapper vtables and initializes guard `+0x38` to null. It then calls `0x140130830(actual Operation)` before writing procedure type at `+0x40` via `0x140130d70`.
4. In that constructor, `0x140130830` assigns the actual operation into guard `+0x38`, resolves the guarded operation twice around `0x14005f770` to obtain the OPERATION model, and sends that model through `0x140377b20` into the widget wrapper's model guard at `+0x28`. It constructs a typed `SfStepData*` argument and calls notifier `0x1402cf3c0` with key `2signalChangeActiveStepData()`.
5. Retained code for notifier `0x1402cf3c0` shows an initialization/suppression gate at byte `+0x141`: only when it is zero does dispatch proceed. The routine first resets byte `+0x90` to zero, looks up the key in the notifier's map at `+0x88`, iterates the selected receiver list, removes expired QObject guards, resolves each live receiver and invokes its virtual dispatch. Thus the empty-map case is an observed lookup result, not a generally safe assumption; live receiver order and effects matter.

`wave3-original-editor-probe-004-receipt.json` binds this chain to an owned original process/module identity and reports observations 49–53 at wrapper-guard/notifier entry and return. Those observations saw initialized null/model/operation guard storage, notifier suppression byte zero, and `+0x90` remaining zero. They do **not** prove receiver registration completeness, callback effects, or successful native wrapper completion. The probe stopped at first editor-grid publication and cleaned the owned process; it did not reach full UI setup, Save, or Close.

## Current editor-state comparison

The current `editor_base` model traces base widget construction through its resumable shared constructor tail. Its report explicitly says wrappers and their signal connections follow QWidget, and `resume()` returns `ApplicationWrappersUnrecovered` after a completed tail. That is consistent with the source boundary. The existing `StagedEditor300` record and `GuiModelGuard` preserve adapter identity/lifetime constraints, but the native contract requires a Qt-style weak QObject guard that updates on QObject destruction and a real notifier owner/key/list dispatch.

The current converter does not claim those effects complete: `EditorBaseReport::applicationWrappersReturned` and `completeForPublicSave` are false; no wrapper phase, QPointer update receipt, or active-step notifier dispatch state is exposed as completed. Keep operation/model identity and the notifier state as separate gates. The outer operation wrapper's constructor order means its procedure type assignment is downstream of the operation guard setup and notifier call; do not publish a fully initialized wrapper before those complete.

## Smallest source evidence needed to advance the gate

A bounded next static pass should bind, without runtime mutation:

- exact wrapper field layout and QPointer helper call for model `+0x28` and operation `+0x38`, including the destructor/clear path that invokes guard change on QObject destruction;
- the owner and creation/registration path for the notifier map/list used by key `2signalChangeActiveStepData()` on the first actual editor operation;
- ordered receiver identities for that key at the first call, their expired-QPointer pruning behavior, and which receiver virtual methods run before constructor return;
- the resulting post-call `+0x90` and `+0x141` values and the explicit continuation into procedure type initialization and UI setup.

If complete static caller/registration closure does not establish ordered population, preserve the current gate. Do not infer the native notifier is empty from one probe's zero suppression byte, or treat entry/return observations as proof of successful downstream callback effects.

## Qualifications

This is a static source trace from retained PE disassembly plus the bounded original probe receipt and current-source comparison. It establishes the recovered wrapper field writes, guard helper dispatch, notifier key/gate behavior, and limited probe values. It does not establish arbitrary Qt event timing, all receiver population/order, notifier registration side effects, a full completed 3D editor constructor, wrapper destructor behavior in every path, Save parity, or settings/session invariance. No source, build, GUI/app-control, or mutable REA action was performed.
