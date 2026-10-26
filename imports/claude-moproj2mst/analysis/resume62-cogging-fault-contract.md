# Resume 62: Cogging two-reheat access and failure contract

## Static evidence added

A new bounded disassembly excerpt is retained at `analysis/resume62-cogging-cb50-bounded.asm` (SHA-256 `6b999dccc7d820c6d570e17cce2bdb39d563008380ac325efc45e7198f152252`). It was extracted from immutable `evidence/original-binaries/DEF_GUI_MO_64.exe` (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) with `dumpbin /disasm:bytes` limited to the start of `0x14076cb50` through its first NoPass formatting call. The caller loop remains retained in `evidence/gui_mo/s1/mdt/a8c0.asm`.

The caller disassembly shows the two-reheat arithmetic: at loop index 1, the accumulated pointer is `base + 0x320`, one-past two `0x190`-byte records, and the caller passes that pointer to `0x14076cb50` at `0x14076ac6d`.

The new callee excerpt directly shows this order:

1. `0x14076cb76` calls `0x14076b450` with the reheat number, which emits the `Reheat 2` banner.
2. `0x14076cb7b` reloads the record pointer.
3. `0x14076cb80` executes `mov r9d,dword ptr [r11]`, the first four-byte model-data read from record offset 0.
4. Only after that read does `0x14076cb96` call the formatter for the `NoPass` line.

Thus the exact static read site is now instruction-address-qualified. The decompiler had already shown this sequence, but the new bounded PE bytes corroborate the first read itself. The disassembly cannot determine whether `r11` is mapped readable at runtime: for a given heap layout the load may return adjacent bytes or raise an access violation. It proves neither outcome.

## What is and is not determined

Static code determines that the writer calls its banner formatter before the invalid read, and that it has attempted earlier sections and the valid element-1 record (`Reheat 1`) before entering this second call. It also determines that the `NoPass` formatter and subsequent heat/pass calls are not reached if the load at `0x14076cb80` faults.

These are formatter-call/order facts, not guarantees about bytes already visible in the file. The original `FILE*` uses a 4 KiB buffer. Whether earlier buffered writes were flushed successfully, whether the `Reheat 2` banner itself flushes, and whether process fault handling closes or flushes the stream are not established for this invalid-read path. The retained native writer ignores `fprintf`/close results on ordinary returns, but that does not specify termination at this load. No original process status, exception dialog, final/partial file bytes, or later MST/property continuation follows from static evidence alone.

The retained Save wrapper (`evidence/gui_mo/s1/mdt/save_d0.c`) advances past a Cogging writer that returns normally, including the handled `fopen`-failure return. A fault at `0x14076cb80` prevents the writer from reaching its normal return, so wrapper continuation cannot be inferred. The current candidate `M2M_E_UNSUPPORTED` is its own explicit guard policy, not an original failure/status result.

## Smallest next experiment

No GUI trial is eligible under the current release gate. At repository root, `analysis/gui-control-lifecycle-assessment.json` still reports `unsupported_release_api`; `python tools/gui_validation_status.py` reports `release_ready=false`. Do not acquire GUI control until a supported release action and verifiable release receipt/status exist.

When that gate clears, use the existing active-Cogging `42_vdie` fixture as a one-reheat control and a separate copy with a complete second serialized REHEAT block as the target. Verify the target parses with count and vector length both 2 before opening it in the original GUI. Invoke Save once on each disposable project. Capture process survival/exit, any observed exception code or dialog, and complete pre/post file inventories with hashes, including MOPROJ, Cogging MDT, and MST. Stop after the target attempt; its output is exploratory because the invalid read remains undefined. Until then, retain the candidate public two-reheat refusal guard and its candidate-only regression.

No source, build, test, GUI, or mutable REA state was changed. The only new evidence is the bounded assembly excerpt and this analysis.
