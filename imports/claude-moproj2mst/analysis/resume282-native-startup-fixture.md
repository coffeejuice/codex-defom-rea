# CP282 benign native startup fixture

## Purpose and boundary

This checkpoint builds a fixed, harmless x64 executable whose exported `EntryProbe(View*)` is present in the mapped image at the initial `CREATE_PROCESS_DEBUG_EVENT`. Its primary thread calls the probe immediately from `main`; the probe reads a pointer at `View + 0x18`, stores that observation, and returns a fixed success marker when it is nonzero. Static export/RVA/byte inspection is read-only. The focused launch test starts only this fixed child, waits on its process handle for at most five seconds, and verifies exit code zero.

No original application, GUI, Frida, REA, or debugger observer was launched or used. This is only a harmless child-fixture result. It neither validates debugger event handling nor integrates CP281's observer, readiness gates, or startup-event arming. CP281 integration and any original execution remain separate and held.

## Build and identity

- Source: `admin/native_color_entry_fixture.cpp`, SHA-256 `88a52efc7d3544b7d84bba93cc7c41e897bddc370dff5b82f5028bf1d9bbf87b`.
- Compiler: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe`, MSVC 19.44.35228 for x64, compiler SHA-256 `88c8344236a27a6e727e0a8edc49aaa2690bdc7a9464b9d18cc7abe70a9f1c0d`.
- Build command arguments: `/nologo /std:c++17 /EHsc /O2 /MT /GS /Gy- <source> /link /INCREMENTAL:NO /OPT:NOICF /OPT:NOREF /Brepro /EXPORT:EntryProbe /OUT:<build/cp282-native-startup-fixture/native_color_entry_fixture.exe>`; the build script activates the installed `vcvars64.bat` environment first.
- Output: `build/cp282-native-startup-fixture/native_color_entry_fixture.exe`, 499,712 bytes, SHA-256 `2a07aecccca5562b0dfe02fd212b1aa84d0a69cc58d4ceddd7c1617f970dc962`.
- Parsed image: AMD64 PE32+, `EntryProbe` export RVA `0x1000`, inside executable `.text`.
- Exact first 16 bytes at the export RVA: `48 8b 41 18 48 89 05 5d 5c 07 00 48 8b 05 56 5c`.
- The version probe is `/nologo /Bv /Zs <source>` under the initialized VS environment. It reported compiler 19.44.35228.0, linker 14.44.35228.0, and exited successfully without the earlier missing-source diagnostic.
- `build.log.json` retains both successful `/Brepro` build command arrays and both SHA-256 values; each is `2a07aecccca5562b0dfe02fd212b1aa84d0a69cc58d4ceddd7c1617f970dc962`, with `reproducible_two_builds=true`.

## Focused child result

Command: `python admin/test_native_color_entry_fixture.py`. It independently reparses the PE and checks the export RVA/prefix and build hash, then uses `CreateProcessW` with an explicit application path and no inherited handles. All Win64 handle-bearing APIs have explicit `argtypes` and `restype`, including `WaitForSingleObject`, `TerminateProcess`, `GetExitCodeProcess`, and `CloseHandle`. The normal fixed invocation completed with exit code 0 (PID 15052; primary TID 8556). A second invocation of the same executable used only the fixed `--bounded-hold` mode: its 100 ms wait timed out, `TerminateProcess` on its owned handle succeeded, the 2,000 ms follow-up wait returned `WAIT_OBJECT_0`, exit code was 642 (`0x282`), and both handles closed successfully (PID 25080). The test records all of these fields in `test.json`; failure to terminate or observe a signaled process is retained as an explicit incomplete cleanup result.

Build and test receipts are under `build/cp282-native-startup-fixture/fixture.json`, `build.log.json`, and `test.json`. The build/test outputs stay inside converter build scratch.

## Limits

The fixture is a startup target, not a debugger test. This test does not prove that Windows delivered a creation event before the first user-mode instruction, that a debugger can read the child mapping at that event, or that any breakpoint/context/handle cleanup protocol works. The initialized `View` and nonzero object live in the executable's static data, and normal exit demonstrates the probe path completed in this run; no runtime address or object-layout claim about the original application follows. A future isolated debugger-integration test must independently validate event ordering and arm/read back its observer before continuing the creation event. No conclusion about original reachability or readiness is supported here.

Independent root rerun after review repair: python admin/test_native_color_entry_fixture.py exited 0; normal child PID 26296 exited 0, bounded-hold PID 35088 timed out at first wait, owned-handle termination succeeded, follow-up wait signaled, exit code 642, and no handle-close errors. Current executable SHA-256 was independently rechecked as 2a07aecccca5562b0dfe02fd212b1aa84d0a69cc58d4ceddd7c1617f970dc962. The earlier source-hash field was corrected to the current file hash above.
