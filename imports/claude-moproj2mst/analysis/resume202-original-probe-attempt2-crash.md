# Converter original attempt 2 crash evidence

## Finding

Windows recorded an application crash for the process associated with PID 20712 (`0x50E8`) at 2026-10-26 04:45:05 UTC (2026-10-25 23:45:05 local, CDT). The process was `c:\program files\sftc\deform\v11.0.2\def_gui_mo_64.exe`. Application Error event 1000 reports an access violation (`0xc0000005`) in `QtCore4_64.dll` version 4.8.4.0 at fault offset `0x32c0`. WER event 1001 at 04:45:08 UTC classifies it as APPCRASH and gives the matching report ID `baaa6d93-a653-4371-a9e2-d5620761636f`.

## Evidence and limits

The event's faulting process ID is `0x50E8` (decimal 20712), directly matching the detached/exited PID under investigation. Its faulting application start-time field is `0x1DD6504C490834D`. The event establishes an unhandled process crash, rather than a clean exit, and identifies the faulting module and exception.

The available event evidence does not establish whether this was a natural startup defect or was induced by instrumentation. It contains no call stack or causal attribution. Therefore instrumentation-induced versus naturally occurring remains unknown. The WER record points to an archived report directory under `C:\ProgramData\Microsoft\Windows\WER\ReportArchive`, but no dump or report contents were inspected for this bounded investigation.

## Collection

Read-only query of the Windows Application event log bounded to 2026-10-25 23:44–23:47 local time, filtered to Application Error/WER events mentioning PID 20712 (`0x50E8`) or `def_gui_mo_64.exe`. No process was launched and no GUI/session was touched.
