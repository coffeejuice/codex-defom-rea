# CP62: isolated status-entry null-main skip

The new helper consumes a fresh qualified status-entry main-window observation once. Null reaches the native status-work skip and stops before the caller's QString temporary release/public-return boundary. Nonnull remains guarded before QMainWindow::statusBar. Neither outcome completes public Close or resets application state.

The exclusive write set is three new files plus this receipt:

- `source/apps/moproj2mst/include/m2m/engine/close_status_prefix.hpp`
- `source/apps/moproj2mst/src/engine/close_status_prefix.cpp`
- `source/apps/moproj2mst/tests/mdt/test_close_status_prefix.cpp`

Existing helpers, shared Session/model interfaces, CMake, plans and public Close were untouched. No build, CTest/executable test, GUI, original target execution or mutable REA operation was run. Root owns combined validation.

## Source qualification

`analysis/resume62-close-global-reset.md` and its eight bounded ASM captures supply the controlling source qualifications and hashes. Original reference: DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; retained QtGui4_64.dll SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

At normal accepted-Close status occurrence029ce4 ->2d0bd0,2d0bef freshly reads the global main-window pointer through2c8090. The null branch at2d0bff/2d0c01 returns through2d0c6b without QMainWindow::statusBar, QStatusBar::showMessage, QColor construction or ShowMsgInArea delivery. The helper models only this conditional skip and the opposite guard.

Nonnull main reaches QMainWindow::statusBar at2d0c08. That Qt getter may construct/install a status child; later showMessage(timeout0) can delete an existing status timer before the message-area notification. Those effects are not implemented here. Suppressed shared notification cannot bypass them. Earlier false-setter main facts are not fresh observations after its synchronous receivers, so this helper requires its own exact occurrence string.

After the null skip, the caller still releases its QString temporary at029cea/029cf2 and loads true at029cf8. The helper stops before those effects. It does not construct/translate the caller's message, own the native QString, destroy it, return a Close Boolean or infer a blanket process reset.

## API and retained identity

`CloseStatusPrefixProgress` is bound to a live ConversionSession and the weak identity of that Session's actual ModelManager. Each attempt obtains the actual manager and rechecks both identities/ownerSession before consuming observations. Stored Session addresses are compared only, never dereferenced. Copy/move are deleted; an external old-manager lease does not authorize a replacement Session.

The constructor requires exact `closeStatusSourceOccurrence` and nonblank qualification. `CloseStatusMainObservation` independently requires this live Session, the same fresh occurrence, explicit optional main presence, and nonblank qualification. Unknown is distinct from null. Qualification labels supplied evidence; it does not authenticate a native observation or create a production global-main binding. MainViewState existence is not used as a substitute for the global pointer.

Missing facts retain `AwaitMainLookup`. A qualified lookup latches its null/non-null result. Null retains `NullSkipBeforeCallerQStringRelease`; nonnull retains `NonNullStatusBarGuard`. Repeated attempts neither re-query nor replace the completed outcome with later facts. In particular, later null cannot bypass a previously reached nonnull status-bar guard. `lookupCommittedThisAttempt` means the isolated progress consumed its supplied lookup result, not that this helper executed a native getter.

Only progress metadata changes. Model/current/Project/OPI, native busy/global projections, tree mode, notifier suppression/+90/maps/population, receiver identity, pending diagram timers and status owners are untouched. A future coordinator must still establish preceding accepted-Close/teardown/false-notifier completion. This helper does not provide a shortcut to the late occurrence.

## Focused validation and freeze

Target: `m2m_mdt_close_status_prefix`. CTest: `m2m.mdt.close_status_prefix`.

```text
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.close_status_prefix$ --output-on-failure
```

Eight Release-active Testkit cases cover fresh qualified null versus candidate MainView existence; missing/unqualified/stale/foreign observations; nonnull lookup retained against later null; null skip retained against later nonnull without caller release; unchanged actual Session/manager/Project/notifier/pending/OPI state on both branches even with suppression; foreign live Session refusal; constructor occurrence/qualification refusal; and old progress rejected after original Session destruction despite a retained manager.

Fixtures explicitly declare their observation facts. The unchanged-state fixture retains adapter Project/model storage deliberately to verify that the isolated helper does not release it; that fixture is not evidence of normal-Close reachability or a runtime original status capture.

Static checks passed for Testkit entry point/include location, quoted include resolution, supported CHECK/REQUIRE macros, ownership static assertions, absence of C assert/NDEBUG-dependent checks, and trailing whitespace. No compilation or executable validation was performed by this worker.

| Frozen file | SHA-256 |
| --- | --- |
| close_status_prefix.hpp | 0be391a71590f585f2fd04e11e2a69eaec5ef8b24344329334d726aabf633c0a |
| close_status_prefix.cpp | 95caad4ff328a722800445280948d7ff45262ed83afd2ac76e003baf48470250 |
| test_close_status_prefix.cpp | 21cc984fd498ce54d35873083d92e2b396035b8d31e4acd93797fd3abf3dae10 |

Remaining dependencies are native global-main observation/storage ownership, prior Close phases, nonnull status child/timer/Qt effects, actual ShowMsgInArea population/callbacks, native caller QString release and final public return. Public Close remains guarded.
