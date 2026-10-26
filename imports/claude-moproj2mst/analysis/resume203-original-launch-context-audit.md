# CP203: original-app converter launch-context audit

## Scope and result

This audit uses only actual historical original-application sessions and retained original project evidence under `evidence/gui_mo/oracle_b/98_B_DS3F_before` and the `analysis/resume202-*` receipts. It excludes standalone `moproj2mst.exe` candidate runs.

The passive original-app attempt 2 and the uninstrumented baseline both record the same launch context: installed `DEF_GUI_MO_64.exe` at `C:\Program Files\SFTC\DEFORM\v11.0.2`, arguments `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, and CWD `C:\PROBLEM\p202a`. Both records identify the same executable SHA-256 (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`). Both ended in the reported QtCore4_64.dll access violation at offset `0x32c0` (attempt 2 was instrumented; baseline was not). Thus there is **no evidenced mismatch in executable, command line, CWD, or disposable project** between these two original-app trials that explains the repeated crash.

This paired evidence shows the fault reproduces without Frida under the recorded launch setup. It does not identify the underlying cause or rule out differences in unrecorded process environment, system state, timing, or external dependencies.

## Evidence by session

- `analysis/resume202-passive-oracle-run-2026-10-26T04-45-03-155Z/summary.json` records the original executable path, argv, CWD, host, helper argv and PID 20712. Its `loaded-module-file-identities.json` and summary identify the executable, QtCore and QtGui module paths/hashes. The attempt was instrumented and incomplete: Frida attached, a partial hook observation was collected, then the process terminated. `analysis/resume202-original-probe-attempt2-crash.md` ties PID 20712 to Event 1000/1001 with access violation `0xC0000005` in QtCore4_64.dll at `0x32c0`.
- `analysis/resume202-uninstrumented-baseline-2026-10-26T04-59-10-181Z/result.json` records a no-Frida, no-GUI-input launch of the same executable hash with the same argv and CWD. The helper receipt records suspended creation, one resume, and verified process exit code `3221225477` (`0xC0000005`); source and disposable hashes were unchanged. `analysis/resume202-baseline-crash-evidence.md` binds PID 37824 to matching Windows Application Error and WER records for QtCore4_64.dll offset `0x32c0`.
- `analysis/resume202-original-probe-attempt1.md` records an earlier suspended original-app attempt that failed before resume because the first hook could not be installed. It is not a completed launch comparison and supplies no crash result.
- `analysis/resume202-b98-copy.md` says the B98 source fixture was copied byte-for-byte by relative path to `C:\PROBLEM\p202a`; it identifies the project as the disposable fixture and links its manifest evidence. The instrumented trial's `disposable-before.json` and baseline `before.json`/`after.json` provide the corresponding session-level unchanged-input checks. `evidence/gui_mo/oracle_b/98_B_DS3F_before` is the retained source tree; its presence is not a separate successful original-app run.

## What the records establish about context

- **Command line:** same for the two crash-producing original-app sessions: `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`.
- **Working directory:** same: `C:\PROBLEM\p202a`.
- **Executable identity:** same installed path and SHA-256.
- **Project files:** B98 source was copied into p202a; recorded before/after disposable hashes remained stable for the uninstrumented run, and the instrumented capture retained a disposable-before manifest. This makes a recorded fixture mismatch an unsupported explanation for the pair.
- **Environment:** no full environment-block snapshot or Qt-specific variables are present in these receipts. The instrumented receipt does record QtCore/QtGui loaded paths and file hashes, but there is no equivalent loaded-module inventory in the uninstrumented baseline receipt. Equal executable and command context therefore do not establish equal full environments or equal loaded Qt dependency resolution across sessions.

## Conclusion and limits

No concrete launch-context mismatch is evidenced between the two original-app runs. In particular, the successful B98 standalone converter record is not used here as an original-app comparator. The original-app evidence instead shows the same launch command, CWD, executable identity and fixture, with the same fault location both with and without Frida. This makes Frida unnecessary for reproducing the observed fault in these recorded conditions; it does not prove QtCore itself is defective, identify the invalid state/caller, or exclude unrecorded environment/system differences.
