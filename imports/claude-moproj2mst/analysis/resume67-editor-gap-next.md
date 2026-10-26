# Editor300 reachable-gap audit (checkpoint 67)

## Reachability boundary

The active-texture finish is present in `src/engine/editor_base.cpp`: the
retained `PaintDeviceNullImageFinishCall` consumes the moved active-copy image,
and its returned opacity advances to `OpacityDecision`. The ordinary cached
image cleanup path is skipped for this selected source. This is source
inspection only; this audit does not assert a build or runtime result.

Native editor reachability remains stopped before setupUi. The base constructor
continues to return `ApplicationWrappersUnrecovered` after the QWidget shared
tail (`applicationWrappersReturned == false`); the evidence does not close the
QPointer receiver list and notifier callback order. Separately,
`ConversionSession::beginEditor300Factory` requires the retained
`PrivatePaletteUnavailable` result and intentionally does not allocate setupUi.
Therefore the control and page behavior below is presently isolated projection
or helper-profile behavior, not evidence of a completed native editor.

## Remaining branch families

| Family | Present coverage/source shape | Remaining qualification |
|---|---|---|
| QWidget/application handoff | QWidget tail precedes wrapper construction; retained QPointer/notifier evidence gives partial order. | QObject guard lifetimes, exact receiver population, notifier delivery order. This is the reachability blocker. |
| SetupUi control tree | 681 identities, ordered scalar prefix, 14 internal toggle connections (including one duplicate), final indices and automatic-slot signatures are statically mapped. Generic toggles update requested local enable state. | Native widget allocation, effective inherited enablement, translation/resources, and real signal/callback delivery are not established. |
| Initial page/refill | Explicit profiles model nine initial fills, two repeat fills, temporary copies, backup/restore and six backed refreshes. | Real Qt callback ordering, full page ownership/reachability and unprofiled input combinations. |
| Validation/apply | Bounded helpers preserve qualified validation stop and ordered partial writes for profiled data. | Public Save is guarded; whole-editor validation, arbitrary callbacks and final file/view side effects remain unproven. |

These branch families must not be conflated: testing one projected event does
not satisfy the wrapper gate or make the public Save path reachable.

## Next smallest source-closed control edge

Prioritize `m_chkbxSW2SP` (`0x1480`) `toggled(bool)` to `groupBox72_2`
(`0x14f8`) `setEnabled(bool)`:

- `src/engine/editor_setup.cpp` names both objects, records the receiver's
  requested `Enabled(false)` at native return address `0x140f1780c`, and lists
  exactly one internal connection `{0x1480, 0x14f8}`. Its table comment states
  all listed routes are `toggled(bool) -> setEnabled(bool)` with AutoConnection.
- `analysis/wave3-editor-constructor-prefix-contract.json` independently
  records this sender, receiver, signal, slot and connection type.
- `SetupControlState` distinguishes `requestedEnabled` from effective
  `enabled`, and `receiveInternalToggled` consumes an already-emitted bool; it
  does not fabricate the native emission. The focused test file currently has
  no references to either `0x1480` or `0x14f8`.

Small acceptance test: in `tests/mdt/test_editor_setup.cpp`, use a fresh
`EditorSetupState`, bind `FreshDefaultQtModel`, apply
`NoActiveTranslator`, verify the receiver's requested enable is false while
effective `enabled` and sender `checked` remain unknown; verify delivery is
empty before connection and connection installation does not synchronize
state; then supply true and false and require one ordered sender/receiver
delivery each, changing only receiver `requestedEnabled`. Keep the case
Release-active. This validates the bounded projection contract only, not a
native click or parent-tree effective state.

Proposed ownership for that follow-up: `tests/mdt/test_editor_setup.cpp` only,
plus its checkpoint report. Do not edit `src/engine/editor_base.cpp`,
`src/engine/editor_setup.cpp`, CMake, GUI state or mutable REA state. Root owns
build/integration.

## Evidence identity and limitations

Current inspected source hashes:

- `src/engine/editor_base.cpp`: `F6C10D2B80F0E9659FFACC6EFE3CC33EEF1655FE9E7F69DAA3B13DA9B4C9A21A`
- `include/m2m/engine/editor_base.hpp`: `3DDBB205B1F05F4E09482243F1D90131F6E4D700BFDF0BC9E7641CC8BD415F29`
- `src/engine/editor_setup.cpp`: `F6B0075090D77C64FEF400BC9CB72C63F7558D6734E95B60184B7028DBACAB22`
- `tests/mdt/test_editor_setup.cpp`: `347C51C18DC787E9BC7A2A2EB4648BAD2AC68B8541FBA882009DDD3982EE5319`
- `analysis/wave3-editor-constructor-prefix-contract.json`:
  `DF9C9A07F9D11072208148C534B92D6619DE9FD339CE7338507BFEA02D6FB670`

Retained references: `analysis/resume48-editor-qpointer-gate.md` (SHA-256
`67704705461C5154288F126860D914EF5F5FBED57BD7FF80780BFB523BB19AC7`) and
`analysis/resume65-editor-selected-texture.md` (SHA-256
`548C767F676DC2B88A10F3AF8C03D21377E7A8C1213F784558C82C01A748118C`).
The latter predates the current finish wiring, so current source was checked
directly for that transition. No source, build, GUI or mutable REA action was
performed; no build/test success is claimed.
