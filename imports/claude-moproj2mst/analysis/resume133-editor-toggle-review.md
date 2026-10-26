# CP133 independent review: bound NSTART toggle

## Result

No blocking ordering or admission defect found in the frozen private toggle. Owner/page/context/OPI and delivered-checkbox checks precede the operation store. The current simulation-control lookup occurs only after that store. The refill carries checkbox history through the branches where the native callback does not call `setChecked`, and the code does not enable the public editor constructor/Save boundary.

This remains a private value projection: it is not proof of native signal delivery, Qt widget updates, public Session construction, or original runtime parity.

## Frozen source identity

The reviewed files match the CP133 receipt:

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/simcontrol300_owner.hpp` | `3865aaf7c4670cb63e5c88d91c7a4a8605235f6f888675d9bd44e7ac5b62b364` |
| `include/m2m/engine/simcontrol300.hpp` | `6cf4f9b8d444839f39a9ac3cb831f6c0f313fb0763e8aeac3d16963db12d67ed` |
| `src/engine/simcontrol300_owner.cpp` | `65af624f02b7a322a041d0f114bdf2bfba1aa5d22a9da538c20dd061c4378e8c` |
| `src/engine/simcontrol300.cpp` | `04b62ac1da494473cb26e1dcb3d1ae93ad087cf3c3fa4409f37e8ee6336f9a80` |
| `tests/mdt/test_simcontrol300_nstart_toggle.cpp` | `0d98957da0495ca7782d47292e3e776c37973beb6d176e884cab1d04d5fda0ed` |
| `analysis/resume133-editor-nstart-toggle.md` | `045dee6bb28deba410177589bf8e4210826dfae1d404c1b02a83ccd3683dfca8` |

The controlling report binds original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, callback `[140ecf910,140ecf977)`, setter `[14005a2c0,14005a2d7)`, and the complete refill `[140ebb110,140ebb3dd)`.

## Review findings

- `toggleNstart` first verifies the same Project, live OPI identity, GuiModel and RemeshState binding; completed private page metadata and saved source-model/remesh identities; mode 5 and matching MO operation in the current context and cached page; and that the cached checkbox already equals the delivered `checked` value. It does not call `pageState()` at admission, avoiding an early `simCtl()` access.
- After those checks it stores `operation.nstartGiven`, marks `OperationFlagStored`, then calls `binding.model.simCtl()`. Null or a pointer different from the page's saved `sourceControl` returns at the post-store boundary without dereferencing the saved pointer, changing the page or undoing the store. The refill uses the actual returned current control only after equality succeeds. Exceptions after the store are not swallowed or relabeled.
- The two-argument `fillStepCount` remains a wrapper with prior checkbox false, preserving fresh-fill behavior. The overload starts from prior checkbox state, leaves it intact for starts-from-key and null-operation paths, explicitly clears it for missing/nonwriting classes, and reads the given flag for the writable non-key class. The unsigned ADD/NEG behavior and spin clamps remain unchanged. Toggle does not write `startStepNo` or SC values.
- The caller does not shortcut repeated equal values: each admitted event stores the byte and refills. Only the step-count page is replaced. `completeForPublicSave` remains false, public `PrivatePaletteUnavailable`/`6504abb0` boundaries are unchanged, and no native-base completion or public factory admission was added.
- The eight focused cases are active Testkit cases with one `main`. They cover repeated toggles, SC wrap/clamp, primary-list changes, key history, nonwriting/missing classes, null/replaced SC after the store, pre-store owner/context/event refusals, partial construction, and replaced OPI identity. No test/build was run in this independent read-only review.

## Qualification caveat

The post-store SC guard compares the current control pointer against `pages_->sourceControl`; it does not carry an independent generation for `GuiModel::step`. The source report's replacement test retains the old step so the replacement has a distinct address. Same-address allocator reuse is not independently detectable from these identities. Keep the claim precise: a null or pointer-mismatched current SC is refused after the flag store. The method never dereferences the saved pointer.

## Validation boundary

No blocking defect found in the requested source review. Root owns compile/runtime validation. No source, test, build, GUI, original target or mutable REA operation was changed or executed by this review.
