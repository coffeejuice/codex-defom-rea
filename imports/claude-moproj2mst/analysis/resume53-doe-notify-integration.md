# Actual-session SetOprHasDoe binding boundary

The existing converter binds `Project.doeIo` to `ConversionSession::sessionNotificationStateOwner()` in `src/core/convert.cpp`. This permits a mechanically checked owner hook. It does not establish the native flow-chart QObject receiver population, registration order, lifetime, or current diagram backend. The new hook therefore retains the unavailable-population guard and does not enable public B121 parsing.

## Frozen files and API

Only these implementation/test files changed in this lane:

- `source/apps/moproj2mst/src/doe/variable_notify.hpp`
- `source/apps/moproj2mst/src/doe/variable_notify.cpp`
- `source/apps/moproj2mst/tests/doe/test_variable_notify_binding.cpp`

```cpp
std::unique_ptr<SetOprHasDoeSessionBinding>
bindSetOprHasDoeSessionPrefix(engine::ConversionSession& actualSession);

void SetOprHasDoeSessionBinding::dispatch(
    engine::ConversionSession& actualSession, int operationId, bool hasDoe);
```

The factory requires a live session Project, its existing StudyIo, and pointer equality between the StudyIo's observed notifier and the session's existing notifier owner. It never invokes the lazy notifier accessor, constructs an I/O object, or rebinds an owner. The caller retains the hook. The hook retains only weak I/O/notifier leases and a comparison-only Project identity; it does not keep released Project storage alive.

Every dispatch revalidates the live Project, the same I/O object, the session notifier, and its required Project identity before representing native effects. Replacement I/O, foreign sessions, expired leases, or released Project storage refuse with `M2M_E_UNSUPPORTED` before flag/map/receiver effects and before adding a generic dispatcher attempt.

`notifier()` exposes the explicit existing helper bindings for a future actual lifecycle owner. Its receiver population starts unavailable. The hook does not derive registration order from `Project.subs`, select only the active sub-project, infer null backends from headless models, or claim native constructor completion. A required-Project identity is checked at the existing receiver ModelIdentity boundary, after suppression, flag reset, snapshotting, and prior receiver visits. Standalone helper bindings leave that optional constraint null, preserving their existing behavior.

## Exact retained guards and ordering

The controlling native evidence remains `analysis/resume51-doe-variable-notify.md` and its hashed PE captures. It establishes `slotSetOprHasDoe(int,bool)`, suppression before flag reset, ordered snapshot traversal, backend-null before logical-item search, and the icon allocation/backend boundaries. This integration adds no new native behavioral claim.

For a newly created valid hook:

1. Actual shared suppression returns normally without requiring receiver population or changing `flag90`.
2. Unsuppressed dispatch sets the actual owner's `flag90=false` and then throws `M2M_E_UNSUPPORTED`: `2signalSetOprHasDoe() prefix: ordered receiver population is unavailable`. The attempt records Population refusal and the retained reset.
3. An explicitly qualified actual receiver list preserves order, inactive receivers, weak lifetime handling, and earlier effects. A foreign Project receiver stops at ModelIdentity. Unknown backend remains guarded; qualified null backend and missing logical item may finish a receiver. A matching live backend still stops before QIcon allocation (`true`) or primary-icon clear (`false`).

The public `readInputTable` guard remains `DoeVariables reader 0x14090ee20 has no oracle case yet` for a nonempty DoeVariables element. The isolated scalar prefix still stops after its committed keyword/operation mutations at `2signalSetOprHasDoe() delivery is unresolved after committed keyword/operation prefix`. Neither parser calls this new hook. No scalar-double `temp.KWD` creation is introduced. Root owns any future shared hook/parser continuation after actual population/backend ownership is established.

## Verification and limits

Nine new focused tests cover absent/unbound/foreign owner, actual-owner Population refusal after reset, shared suppression, explicit actual inactive/active receiver order, foreign receiver refusal after reset, replaced I/O, another session, released Project storage, and retention of the public B121 guard. Receiver lifetime/backend values in those tests are explicit diagnostic fixtures, not original startup observations.

Static review confirmed the actual ConversionSession/StudyIo APIs and converter owner binding, nine test declarations, no trailing whitespace, and both unchanged parser guards. No build or test execution occurred in this lane. Parent validation should build `m2m_doe_variable_notify_binding`, run `m2m.doe.variable_notify_binding`, and retain the existing notifier/scalar-prefix regression tests. Existing CMake CONFIGURE_DEPENDS globs discover the new test; no CMake change was made.

No shared SessionNotificationState header, public Save, parser, original evidence, runtime application, GUI, mutable REA state, build, or deployment was changed.
