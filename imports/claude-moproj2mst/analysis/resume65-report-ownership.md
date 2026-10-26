# CP65 — Report viewer owner, Save order, and Close boundary

## Updated result

The CP63 bounded DUMPBIN receipt supersedes the older owner-only notes: the immutable PE body of `0x1412af0c0` and constructor `0x140643100` is now retained. Report's native Save override `0x1412adf90` calls vt+0x80 first and then `0x1412af0c0` on the same Report operation. That establishes a concrete Save call path and substantially narrows the remaining question.

The body does **not** yet qualify a complete Report Save or a live viewer state mutation. `0x1412af0c0` reaches an unresolved `0x140d0d6f0` after resolving the current operation model and project. That helper is distinct from the already ported `displayRoiModel` (`0x140d0d670`). The later per-object loop contains further unresolved helper/IAT calls. No native Report/subproject destructor or normal-Close order was recovered. Keep public Report Open/Save refused.

## Source-qualified call and owner chain

1. Native generic File > Save reaches operation vt+0x68 through `0x140052ec0`. MDT L7 identifies Report vt+0x68 as `0x1412adf90`; the bounded retained caller excerpt at `evidence/gui_mo/s1/mdt/L7/s0.c` shows `FUN_1412adf90` calling virtual `+0x80`, then direct `0x1412af0c0`, with the same `param_1` receiver. The existing current-source Report guard preserves this point in `src/engine/save_current.cpp`.
2. In `resume63-report-viewer-bounded-capture.txt`, `0x1412af0c0` begins with `0x1405ef2b0(this)` then `0x14005f770(current)` to obtain the operation's `GuiModel`; null model branches to the common cleanup/return at `0x1412af698`. It then obtains the current project through `0x1412b0fc0(this)`; null project takes the same return. It performs local path/string work before resolving the viewer model.
3. At `0x1412af3b5`, it calls `0x140c7ae00(model)`; at `0x1412af416`, it calls `0x140d0d6f0(display, model)`. It then asks the returned object for an object count via `0x140c7c1a0` and loops over indices, obtaining entries with `0x140c7ae80`.
4. On each loop iteration, the native body reads the entry name at `object+0x828`, forms a name/path, calls `0x1408f37d0`, and passes the result through `0x1408f4760`; cleanup helpers follow. Because the payload/path and helper semantics are unresolved, do not claim what asset is read, what fields change, or what failure status is returned. The capture shows no identified public filesystem write call, but opaque helpers prevent a whole-function “no file effect” claim.
5. Current source has `engine::displayRoiModel(model)` for the separate native path `0x140c7ae00 -> 0x140d0d670`; `displayRoiModel` selects/creates the named `ROIs` model and sets `f25c=2`. The Report Save body calls `0x140d0d6f0`, not `0x140d0d670`. Their shared first argument being the model's Display does not establish semantic or object identity equivalence. The exact Save viewer model owner is therefore the return from `0x140d0d6f0`; the existing source has no binding to it.

## Document constructor and teardown order

The other Report owner is the document slot at native `SubProject+0x30`. The retained `FUN_1400b5ad0` owner excerpt shows getter first, and on empty slot: query subproject mode, allocate `0x70`, obtain the main-window object, call `0x140643100(raw, mainWindow, mode, mode != 2, 0)`, then store the result with `0x140cbc920` and read it back. This supports lazy construction and an app/window parent argument. Use “guarded document pointer slot” here; the exact wrapper/reference semantics are not established by the excerpts.

The constructor body at `0x140643100` is now bounded through its RET (`0x1406432e5`). It calls the Qt QObject constructor with `this` and the parent argument, installs vtable `0x1424e0098`, initializes members at `+0x18`, `+0x20`, `+0x38`, `+0x40`, `+0x50`, `+0x58`, `+0x60`, stores constructor flags at `+0x28..+0x2a`, calls `0x1406433a0`, and registers seven main-window connections through `0x1402cfbe0` using static metadata pairs from `0x1424decd8` through `0x1424dee70`. The constructor is consequently more than a POD holder: seven registered connections are ordered effects. Their signal/slot text/behavior is not qualified by this capture, and the constructor's throwing/unwind behavior is not established.

No destructor body for the Report document or the native `SubProject` owner was found in the receipts inspected here. The native caller's Qt parent argument alone does not tell us when Save or Project Close clears the `SubProject+0x30` slot, whether the slot is QPointer-like versus owning, whether the parent deletes the document, or what is disconnected and in what order. The project-side adapters are not that proof: `Project::~Project()` is defaulted; `SubProject::~SubProject()` only records an adapter flow-receiver death; and `ConversionSession::~ConversionSession()` calls `releaseProjectStorage()` as exception/final adapter cleanup, explicitly not as native Close replay. The current model has no Report document owner field.

## Independently implementable boundary

One independent pure prefix is available: derive the Report `.DS` sibling path from the already-owned `Project.path`, using the qualified `QFileInfo::baseName`, absolute parent path, cleaned base name, directory separator and `.DS` suffix described in `analysis/resume52-report-ds-owner.md`. Its exact owner is `Project.path`, and it has no document allocation, QObject connections, viewer calls or file I/O. This can be implemented and tested as a pure path helper without changing the Report guard.

There is no independently implementable **stateful Report viewer prefix** established yet. The native viewer selector is `0x140d0d6f0`; the candidate current `Project::display` is only the Display passed into that unresolved helper. Reusing `displayRoiModel` is not justified by the nearby but distinct `0x140d0d670` target. Stop before the `0x140d0d6f0` call if creating/changing the display would be an externally visible effect; do not port the count/loop based only on its returned address.

## Smallest next evidence and acceptance

1. Resolve `0x140d0d6f0` and its returned object type/owner from immutable PE or retained source evidence; establish whether it is display-owned and whether it creates a named model. Then resolve the exact uses of object names and the `0x1408f37d0` / `0x1408f4760` pair, including file mode/failure effects.
2. Recover the exact Report document slot wrapper and the document/subproject/main-window destructor order at normal Close. Qualify connection teardown and shared/guarded pointer release separately from `ConversionSession` adapter cleanup.
3. Only then implement a Report Save helper under the existing public guard. Acceptance must check native callback order (vt+0x80, viewer path, Report F80/F90/FB0), lazy document ownership/reuse and `.DS` partial failures, the viewer entry count and first mutation, and Close release order. The property helper alone is not an end-to-end test.

## Evidence

- Immutable original PE at `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
- `analysis/resume63-report-viewer-bounded-capture.txt`: DUMPBIN function windows ending at exact RETs for `0x1412af0c0` and `0x140643100`, plus owner and caller decompile excerpts.
- `evidence/gui_mo/s1/mdt/L7/d3.c`, `FUN_1400b5ad0`: lazy slot access and construction/store order; `source/apps/moproj2mst/src/ops/report.cpp`: current Open refusal before document use.
- `source/apps/moproj2mst/src/engine/scene.cpp`, `displayRoiModel`: exact but distinct `0x140d0d670` path; `include/m2m/engine_scene.hpp`: documented display/model owners.
- `source/apps/moproj2mst/include/m2m/model.hpp`, `Project`/`SubProject`; `src/model/subproject_lifetime.cpp`; `src/engine/conversion_session.cpp`: current project/subproject/session destruction semantics and adapter/Close distinction.
- `source/docs/re/moproj2mst/mdt/L7/spec.md` and `source/apps/moproj2mst/src/engine/save_current.cpp`: Report Save slot order and retained guard.

Read-only static audit only. No source, build, GUI, runtime, or mutable REA changes.
