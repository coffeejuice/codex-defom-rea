# CP203 CLI Open and QtCore crash static semantics

## Finding

The recorded invocation is syntactically consistent with the application's CLI Open route, and the absolute project path names the retained disposable `.moproj` input. The static route is `main` at `0x140001410` → loader `0x14001ca30` → `postOpen` `0x14002a0e0` → recent-path update `0x14001dc00` → splash finish → `QWidget::showMaximized` → startup timer registration → `QApplication::exec`. The exact retained argument vector is `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, with cwd `C:\PROBLEM\p202a`.

Both retained crash records identify an access violation in `QtCore4_64.dll` RVA `0x32c0`, exported as `QListData::size() const`. The uninstrumented baseline establishes Frida is not needed for this fault under the recorded conditions. Available evidence does **not** place the fault inside any particular call above: neither Windows Event 1000/1001 nor the helper receipts contain a native stack, faulting registers, or a main-image instruction address. The final explicit application call before entering the Qt event loop is `QApplication::exec` at `0x140001fb6`; `showMaximized` at `0x140001f98` and `QSplashScreen::finish(main)` at `0x140001f8d` are the preceding explicit Qt calls. The fault could arise during either synchronous Qt call or later event dispatch. Calling `exec` the immediate crash caller would exceed the evidence.

## Static call sequence

The selected dumpbin disassembly is the original shipped PE, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; its retained capture is qualified as the `0x140001410` unwind region, not a complete transitive call graph. Within that region:

| Address | Static operation and boundary |
|---|---|
| `0x140001c8f` → `0x14001ca30` | Calls the project loader with the same stack main object (`rsp+0x40`) and the selected path string. |
| `0x140001c94` | Receives/stores the loader result. This is loader return, not completion of the larger CLI Open sequence. |
| `0x140001ca9` → `0x14002a0e0` | Calls `postOpen` with the returned project and same main object. |
| `0x140001cae` | `postOpen` return. |
| `0x140001cbb` → `0x14001dc00` | Updates the recent path. |
| `0x140001ccf` → `0x140001e4e` | Joins the selected CLI path after cleanup. |
| `0x140001f8d` → IAT `0x141e0bfe0` | `QSplashScreen::finish(QWidget*)`, passed the main object; IAT symbol retained as `?finish@QSplashScreen@@QEAAXPEAVQWidget@@@Z`. |
| `0x140001f98` → IAT `0x141e0bfe8` | `QWidget::showMaximized()` on that main object; IAT symbol retained as `?showMaximized@QWidget@@QEAAXXZ`. |
| `0x140001faf` → IAT `0x141e04478` | Registers `QTimer::singleShot(1000, main, "1slotRelaxDockSizeLimit()")`. |
| `0x140001fb6` → IAT `0x141e0bff0` | Calls `QApplication::exec()`. If it returns normally, ordinary main cleanup follows; the retained crash report does not establish such a return. |

`resume173-headless-oracle-next.md` independently summarizes the postOpen/show/timer/exec order. The selected `resume82` disassembly review explicitly distinguishes CLI loader return `0x140001c94`, postOpen return `0x140001cae`, and selected startup-branch completion/join `0x140001e4e`. None is an observed runtime boundary for the crashing process.

The QtCore export evidence in `analysis/wave2-save-diffs-qtcore-exports.txt` maps RVA `0x32c0` to `QListData::size() const`; `analysis/resume202-qtcore-crash-static.md` records the matching QtCore hash `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The export identifies the entry symbol, not the invalid object, failing operand, or caller. `QListData::isEmpty() const` is adjacent at RVA `0x32d0`; adjacency does not identify which container or Qt subsystem supplied the list.

## Command and path assessment

The no-Frida baseline receipt records this exact argv and cwd, and its before-manifest contains `Gear_blank.moproj` at the corresponding relative path with 10,932 bytes and SHA-256 `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`. The baseline report says the after manifest had no content changes. Thus the absolute path was a real, hash-identified fixture input in the recorded launch; it is not a guessed or relative path. The PE's main routine has a selected CLI path that passes a nonempty selected path into the loader, and then performs postOpen/recent-path handling.

This supports the assessment that `-m open -f <absolute .moproj>` is the intended and valid form for this route. The launch receipt alone records supplied argv, not an argument-parser success result. The crash event has no stack and the retained instrumentation did not hook the CLI parser, loader entry, or postOpen; therefore these records do not prove that this particular process reached `0x140001c8f` or successfully loaded the project. In particular, a pre-Open initialization failure remains possible. The path/file identity is established; parser acceptance and successful Open are strongly consistent with the static route but not runtime-bound for the crashing PID.

## Bounded runtime facts and limit

The baseline event identifies PID 37824, application `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, exception `0xC0000005`, and `QtCore4_64.dll` offset `0x32c0`. Its helper receipt records instrumentation/Frida/GUI input all false, the exact argv/cwd, and verified process exit `3221225477`. The earlier instrumented PID 20712 report has the same module/offset. The baseline and instrumented attempts therefore agree on fault location, while the baseline shows instrumentation is not necessary. The instrumented trace did not cover the fault site; no native dump, call stack, or usable fault context was retained. Static symbol mapping plus route order only narrows the candidate interval; it cannot select among splash finish, showMaximized, startup callbacks, or other event-loop activity.

The next evidence that would settle the caller question is a same-build full user-mode dump or exception callback that retains RIP, register context, and accurate stack for the baseline fault, bound to the exact PID and loaded module identities. This report does not request or perform another run.

## Sources

- `analysis/wave3-editor-owned-open-entry-140001410.asm` and `analysis/wave3-editor-owned-open-entry-static.json` (bounded shipped-PE disassembly and binary digest).
- `analysis/wave3-closure-next-can-close-static.json` (retained IAT symbol identities for splash finish, showMaximized, and QApplication::exec).
- `analysis/resume82-boolean-first-open-pe.md` (loader/postOpen/branch-return boundary distinction).
- `analysis/resume173-headless-oracle-next.md` (compact CLI Open → showMaximized → event-loop sequence).
- `analysis/wave2-save-diffs-qtcore-exports.txt`, `analysis/resume202-qtcore-crash-static.md` (QtCore export/hash qualification).
- `analysis/resume202-baseline-crash-evidence.md` and `analysis/resume202-passive-oracle-run-2026-10-26T04-45-03-155Z/{summary.json,disposable-before.json}` (no-Frida command, path, identities, and crash).

Read-only static/retained-evidence audit. No GUI control, original process, mutable REA, source, build, or fixture was touched. No reconstructed DEF_PRE_64 instance is discussed.
