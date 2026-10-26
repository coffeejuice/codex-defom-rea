# Checkpoint 75: safe headless probe for the Cogging one-past read

## Static evidence and guard assessment

The original PE evidence (immutable binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) proves a logical out-of-range access for two Reheat records, not a predictable access violation. The allocation helper requests `2 * 0x190 = 0x320` bytes; the writer's accumulated second-loop pointer reaches `base + 0x320`, and `0x14076cb50` attempts a four-byte read there at `0x14076cb80`. The helper retains no runtime usable-size or capacity value and does not set a per-allocation guard page. See the bounded native records in [resume64-cogging-memory-bound-capture.txt](resume64-cogging-memory-bound-capture.txt) and qualification in [resume67-cogging-onepast-capacity.md](resume67-cogging-onepast-capacity.md).

Keep the candidate two-Reheat guard. Even if a headless probe reads adjacent storage successfully, that value is undefined as a Cogging record and does not make two-Reheat serialization safe. If the probe faults, that result only describes the sampled heap layout. Neither outcome changes the logical bounds or defines stable serialized bytes.

## Smallest safe headless local probe

A probe that does not open or control the GUI and does not create/edit project files can ask the original allocator for exactly the same size and inspect the one-past address in the same process:

1. Run the immutable evidence copy under a headless native debugger in an isolated temporary working directory. Stop before the application event loop and any project actions, after CRT/module allocator state is initialized.
2. Invoke the original allocation wrapper `0x141473368` with `(count=2, stride=0x190)` in the debuggee. This allocates process memory but does not call Save or open a project file.
3. Save the returned base. Query `VirtualQuery(base+0x320)` for state/protection and the heap's usable block size where the allocator API is resolved. Attempt one four-byte read at that address and record whether it succeeds or raises an exception. Exit the disposable debuggee without resuming the app's normal UI path.

This is a bounded, one-allocation probe of allocator readability. It does not alter the original EXE or installed files; the only target-state change is the temporary allocation in a disposable process. It also does not prove the exact Save-time outcome: the real parser's allocation order and neighboring heap objects may differ from this isolated helper call. A conclusive actual-path observation would require reaching `0x14076cb80` with the real two-Reheat parsed model in an initialized original process. No retained evidence establishes a public headless route to that state without calling the GUI's project/save path.

## Local feasibility and limits

I checked the standard Windows Kits x64 debugger locations (`...Windows Kits\10\Debuggers\x64\cdb.exe` and `windbg.exe`); neither exists here. I did not launch the original executable, install a debugger, construct a harness, or perform any runtime read. If a debugger becomes available, the allocator-only probe above is the lowest-impact first observation. Do not treat it as a replacement for the actual Save-time pointer capture.

For an actual-path debugger run, the minimal decisive record would be: parsed count, allocated base/request size, `r11` at `0x14076cb80`, page state/protection at `r11`, the guarded load outcome/value or exception code, and whether the master writer returns. The prior unwind audit shows that if an AV propagates, the known Save/Cogging frames continue exception search; it does not establish higher-level handling. See [resume66-cogging-fault-containment.md](resume66-cogging-fault-containment.md).

No source, build, GUI, project file, or mutable REA state was changed. The probe was specified but not run.
