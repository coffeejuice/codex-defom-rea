# CP76: Report document owner trace and first Save call

## Result

The retained original evidence establishes the Report document's lazy construction sequence and non-null main-window parent argument, followed by publication into the active SubProject slot. It does not establish the slot's ownership protocol or destruction order at project Close. The first Report-specific Save call is vt+0x80: it derives a sibling `.DS` path and calls `0x140645030(document, path, 1)`. The smallest source-safe prefix remains the already implemented pure `reportDsPath(Project.path)` value computation; no document mutation/Save prefix can be integrated without an owner witness.

## Original lazy owner path

`evidence/gui_mo/s1/mdt/L7/d3.c`, `FUN_1400b5ad0`, gives this exact order:

1. Read the slot at `SubProject+0x30` through `0x140c1aef0`.
2. If empty, read mode from `SubProject+0x18 == 2`; allocate `0x70` bytes.
3. Obtain the main-window pointer through `0x1402c8090(0x143d766b8)`.
4. Call `0x140643100(raw, mainWindow, mode, mode != 2, 0)`.
5. After the constructor returns, store the result to `SubProject+0x30` through `0x140cbc920`; read the slot again and return it. A populated slot follows the reuse path.

This is a lazy SubProject-associated slot with a main-window parent argument. The getter/setter names and call pattern do not prove that the slot is an owning `shared_ptr`, `QPointer`, or another specific wrapper. The evidence does not show when the slot is cleared, which reference is last, or how SubProject destruction interacts with QObject parent deletion.

The bounded constructor capture for `0x140643100` shows it invokes the Qt QObject base constructor with the supplied parent before installing the document vtable and initializing document members. It then initializes the listed members/flags, calls `0x1406433a0`, and registers seven connections to the main-window object through `0x1402cfbe0`. The static QtCore constructor body describes private state/backlinks, thread-data selection/ref increment, parent compatibility/parent-helper call and process counter order. But no observation binds the actual Report instance's main-window pointer, QThreadData choice, compatibility outcome, child publication/events, constructor failure/unwind, or connection teardown. Thus “parented to the main window” describes the constructor input; it is not by itself a complete lifetime contract.

## First Save callsite and owner dependency

The generic File > Save route reaches operation vt+0x68 through `0x140052ec0`. Report's override is `0x1412adf90`; retained `evidence/gui_mo/s1/mdt/L7/s0.c` shows it calls virtual vt+0x80 and then direct `0x1412af0c0` on the same Report operation receiver.

The first Report-specific Save callback, `0x1412aeb30` (vt+0x80), derives an absolute project directory and a cleaned `QFileInfo::baseName`, appends the native separator and `.DS`, resolves the active SubProject's Report document, then invokes `0x140645030(document, path, 1)`. Hence the path formula is independently represented by candidate `reportDsPath(Project.path)`, but the actual first Save effect is a mutation of the native Report document. The later viewer callback cannot be bypassed: `0x1412af0c0` follows vt+0x80 and before generic Report property serialization.

The next callback's bounded machine code obtains the current operation model and Project, obtains its Display, calls `0x140d0d6f0(display, model)`, counts and iterates the returned object's entries, then invokes further opaque helper/IAT targets. This is enough to establish a real viewer-model dependency and order, but not to bind the returned object to candidate `Project::display` or `displayRoiModel`, nor to assert per-entry file/mutation/failure semantics.

## Smallest safe prefix and missing witness

The smallest independently safe prefix is only the pure filename derivation from `Project.path`; it has already been implemented and tested. It does not construct, retrieve, mutate or save the document. There is no stateful Report Save prefix before the first native document mutation that can be added without assuming the `SubProject+0x30` ownership and parent lifetime.

The minimum additional owner witness is a same-instance native lifecycle trace that binds: slot read and empty/non-empty branch; constructor identity and main-window parent identity; QObject/private and QThreadData identities/effects; parent compatibility/child-list result; document initialization and seven connection registrations; slot publication; then normal SubProject/Project Close through slot clearing/destruction and main-window child teardown. Static source recovery should additionally identify the concrete wrapper/reference behavior behind `0x140c1aef0`/`0x140cbc920` and the exact destructor/clear callsites. Without that witness, preserve the public Report Open/Save guards and do not attach a candidate `shared_ptr` or widget owner to the slot.

## Evidence

- `evidence/gui_mo/s1/mdt/L7/d3.c` (`FUN_1400b5ad0`): SHA-256 `6f6f7205103782d7ae11c35bf2f33245433b60b6bf8b09dea29c2572bed19582`.
- `evidence/gui_mo/s1/mdt/L7/s0.c` (Report vt+0x68 caller and vt+0x80 path/document call): SHA-256 `66149afcfb363dbf81cd71d86549469d451b5ca2c169dc0cd7ea8a8ef5c2e935`.
- `analysis/resume63-report-viewer-bounded-capture.txt` (bounded constructor and viewer machine-code ranges): SHA-256 `0a0b5b87c1f034e2f0207f23864222098aeb2577af6d5f95def9d49f536b5559`.
- `analysis/resume71-report-owner-edge.md` (current source owner gap and Close limit): SHA-256 `51ab8bfb2917607fb386d19be7ead3e42a8b1e4b2eaaa0e295e8f5ff963111b8`.
- Original `evidence/original-binaries/DEF_GUI_MO_64.exe`: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Read-only retained-evidence/source audit only. No source, build, test, GUI, runtime capture, or mutable REA action was performed.
