# Uninstrumented baseline crash evidence

## Finding

The no-Frida baseline process (PID 37824) crashed with access violation `0xC0000005` in `QtCore4_64.dll` at fault offset `0x32c0`. This matches the instrumented PID 20712 report's module and offset exactly. Both were launched from the same executable path and used the same executable SHA-256; this is strong evidence that the observed QtCore fault does not require Frida instrumentation. It does not establish that every causal environmental/input condition was identical, nor prove the underlying defect mechanism.

## Evidence

Windows Application log Event 1000 at 2026-10-25 23:59:10 local (EventRecordID and UTC timestamp are retained by the event record) identifies:

- PID `0x93c0` = 37824.
- Application: `c:\program files\sftc\deform\v11.0.2\def_gui_mo_64.exe`.
- Faulting module: `c:\program files\sftc\deform\v11.0.2\QtCore4_64.dll`, version 4.8.4.0.
- Exception `0xc0000005`; fault offset `0x00000000000032c0`.
- Application start time `0x1dd6506bcd9e34d`; Report Id `a5752414-d5dc-4594-9a07-8f5c5d9b6393`.

Matching Windows Error Reporting Event 1001 at 2026-10-25 23:59:14 records APPCRASH, P1 `def_gui_mo_64.exe`, P4 `QtCore4_64.dll`, P7 `c0000005`, and P8 `00000000000032c0`. Its fault bucket is 120501182858 and it carries the same Report Id.

The helper receipt in this run's `helper.events.json` reports PID 37824, image SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, command `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`, cwd `C:\PROBLEM\p202a`, suspended creation followed by one resume, and cleanup with verified process exit code `3221225477` (= `0xC0000005`), `exit_verified=true`, and `handles_closed=true`. The capture declares `instrumentation=false`, `frida=false`, and `guiInput=false`. `before.json` and `after.json` record the same executable SHA-256 and unchanged disposable manifest.

## Comparison and limits

The earlier instrumented observation for PID 20712 was reported as `QtCore4_64.dll` offset `0x32c0`. The baseline Windows event gives the same module/offset and access-violation exception. This establishes repeatability of the fault location across an uninstrumented run and the reported instrumented run.

The baseline run used the same executable path and SHA-256, working directory, and project command recorded in its helper receipt. The run records the disposable project unchanged between its before/after snapshots. This supports a controlled comparison, but the available receipts alone do not verify equality of all process environment, timing, system state, or external dependencies across runs. Therefore the defensible conclusion is that Frida is not necessary to reproduce this fault in the recorded baseline conditions—not that all environmental causes are excluded or that QtCore itself is definitively the root cause.

## Provenance

This audit reads the bounded Application log events for Event IDs 1000 and 1001 around the recorded crash and the baseline capture receipt files in this run directory. Event IDs 1000 and 1001 point to the same report ID and fault details. The helper's exit verification independently corroborates that the process ended with the access-violation code.

