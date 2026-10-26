# CP78: original GUI headless Open→Save→Close access

## Finding

No supported headless or CLI Open→Save→Close route for the original `DEF_GUI_MO_64.exe` is established by the retained evidence. The evidence instead documents a GUI-driven harness. A detached process and command-line import are insufficient to claim a batch interface or to guarantee safe disposable execution.

The immutable original is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes). Retained startup/import analysis `analysis/wave3-closure-next-close-splitter-startup-entry-static.json` records a `GetCommandLineA` import at `0x141e03350`; that only establishes the API is imported, not that a documented flag selects automation or that Save can run without the GUI event loop. The same report is explicitly offline PE/DUMPBIN evidence, not runtime evidence.

## Exact launch evidence

`evidence/gui_mo/oracle_b/_tools/launch.py` launches the installed path `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe` with `[EXE] + args`, detached/new-process-group flags, and a caller-supplied working directory. It has no argument validation, Open/Save automation, or headless mode of its own.

`evidence/gui_mo/oracle_b/_tools/probe.py` documents an interactive GUI probe. Its `start <case>` branch starts the executable with no application arguments, waits for a window titled `DEFORM Multiple Operation`, drives **File > Open** through a dialog, and captures GUI state. Its `save <case>` sends **File > Save** through the GUI menu and waits for an `.MST`; `quit` drives **File > Close**, answers a dialog, and sends `WM_CLOSE`. `run.py` likewise describes and implements GUI menu/dialog automation. These are explicit GUI control flows, not headless CLI flows.

The retained scripts point at the installed v11.0.2 location (and `run.py` uses a fixed historical oracle workspace root). They do not show an isolated executable copy, profile/config redirection, or a proved method to prevent registry/license/configuration/session effects. Starting the installed executable in a “disposable” process would therefore still be an installed-app interaction, and the documented Open/Save/Close sequence requires GUI input.

## Path and sandbox constraints

The recovery instructions require DEFORM test projects/generated paths under `C:\PROBLEM` and shorter than 100 characters. This task did not create that directory or launch anything. Current workspace sandbox writable roots do not include `C:\PROBLEM`; prior CP78 planning also records it as outside this task's writable roots. Thus there is no authorized disposable target path available from this static investigation, even if a supported headless route were later found. Do not substitute the converter's workspace scratch: it is not the original GUI harness's configured root and does not resolve the original application's configuration/session isolation.

## Limits and next discriminating action

This read-only audit did not run the original, inspect or modify user sessions, acquire GUI-control input, or change files other than this report. Import evidence cannot prove absence of undocumented command-line switches. To establish a headless route would require a retained source-qualified command parser/dispatch path showing a noninteractive Open→Save→Close path and the required isolation inputs; the current `GetCommandLineA` import and GUI harness do not meet that bar. Until such evidence exists, the next valid original comparison remains gated on the GUI-control release status and a compliant short disposable project path; do not launch the original as a speculative CLI probe.
