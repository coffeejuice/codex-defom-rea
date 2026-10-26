# CP77: isolated primary window identity projection freeze

The new `WidgetAreaPrimaryWindow` helper projects the source-closed primary-address relationship over an existing canonical WidgetAreaWindowOwner/allocation lease. It has no Session/public binding, creates no WidgetState, executes no native base effects and never initializes `+3c8`, stack, impl, registrations, timers or native liveness.

Only three new source/test files plus this report/static-preflight receipt were written. Existing CP75 canonical-cell and shared interfaces remain frozen. No build, tests/full suite, GUI, original execution or mutable REA ran in this lane.

## Implemented contract

- `bind` requires the actual canonical window record, its same live allocation control block and nonempty source qualification. These are diagnostic identity prerequisites, not original constructor-completion evidence. Native-alive stays unknown; known-dead/released/expired owners cannot admit aliases.
- `alias` exposes SfMoMainWindow, SfMainWindowBase, QMainWindow, QWidget and QObject as offset0 roles of one primary owner. QPaintDevice is a symbolic offset0x10 relationship; no arithmetic is applied to the adapter allocation and no secondary pointer/vtable is fabricated.
- Alias comparisons require both canonical record and allocation generation/control block. Equal raw addresses alone, including independent control blocks or separately created records, do not establish the same primary object. Invalid roles and forged owner leases refuse.
- Escaped aliases validate owner/lifetime at use. They survive projection-wrapper release while the same canonical owner remains admitted, but expire/revoke with that owner or its allocation. Diagnostic retention does not claim Qt object parenting/destruction.
- `requireQtBaseEffects` and `requireWidgetComponent` always refuse. WidgetState's existing construction ownership is private/shared interface territory; the new-files-only implementation does not bypass it or substitute MainView's widget. Source/native fields remain whatever separately qualified observations established, initially unknown.

This follows the immutable-PE address lineage in `resume77-boolean-mainwindow-qt-lineage.md`: application stack object ->10490 ->c98b00 ->QMainWindow653ca3f0 ->QWidget65070f00 ->QObject, with the distinct paint-device relationship. Qt private allocations and initializer toolbar/global/settings descendants remain outside this primitive.

## Static preflight and focused validation

Target: `m2m_mdt_widget_area_primary_window`.

Eight new Testkit cases cover equal primary roles; symbolic secondary offset; no shared-cell initialization/allocation request; different owners/colliding raw addresses/duplicate records; forged record/empty qualification/invalid role; revocation and known-dead parent; expired allocation; and alias lifetime independent of its wrapper. The explicit observed null/dead inputs in lifetime tests are synthetic guard probes, not effects performed by the helper.

`resume77-boolean-primary-window-preflight.json` records resolved quoted includes, Testkit `run_all` entry, supported CHECK macro, no C assert, no trailing whitespace and balanced lexical brace counts. Existing CMake `src/*.cpp` and test CONFIGURE_DEPENDS globs include the new helper/target, linked with m2m_core/Testkit. This is static preflight, **not** compilation or passing-test evidence. Root owns grouped/full validation and any integration.

## Frozen source files

Paths below are relative to `source/apps/moproj2mst/`.

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/widget_area_primary_window.hpp` | `0262846de326e9d401850d59424dfc225ee0dcf2e8aadaa71ad4f32a07a0d422` |
| `src/engine/widget_area_primary_window.cpp` | `73439f0ba28b48315dcb0099003f3759ef3f51f4fbee441a9d8e3c56f01e2b0d` |
| `tests/mdt/test_widget_area_primary_window.cpp` | `1fdc0c8f76a8932a2d092b230ce1a45c87338a9e93f6cccbff162f1fccbb2c62` |

No constructor cursor or actual current `+3c8` witness was added. An admitted reached184fa occurrence or actual18477 owner/value read remains necessary for that separate state edge. Area/impl/receiver population, first-Open Apply and public PostPending are guarded. Source owner remains on hold for root validation.
