# Resume 60: Cogging native two-reheat fault evidence and next experiment

## Exact static access path and limits

The retained original PE is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, bound by `analysis/wave3-cogging-native-io-contract.json`. `evidence/gui_mo/s1/mdt/a8c0.asm` is a direct bounded dumpbin disassembly of the master writer at `0x14076a8c0`; it includes the full loop block `0x14076ac03..0x14076ac72`.

Directly visible in that disassembly: the second loop compares signed 32-bit index `i` to `nReheats`, sign-extends `i` (`0x14076ac38`), multiplies it by `0x190` (`0x14076ac40`), adds it to the previously stored pointer (`0x14076ac4c..0x14076ac4f`), then calls `0x14076cb50` (`0x14076ac6d`). For `n=2`, the first pass of the prior SIMLST loop leaves that pointer at element 1. At `i=0`, the reheat call receives element 1. At `i=1`, it receives `base + 0x320`, exactly one-past the two 0x190-byte elements. This call target and pointer arithmetic are directly present in the PE disassembly.

The exact first invalid data read is supported by retained decompilation, not by a retained instruction-level disassembly of `0x14076cb50`: `evidence/gui_mo/s1/mdt/annot_all.c` at `FUN_14076cb50` first calls the banner formatter for `Reheat %d`, then calls the formatted writer with `*param_1` as the `NoPass` value. The semantic first invalid access is therefore a 4-byte read at the one-past pointer, record offset 0, after the `Reheat 2` banner call. No retained `cb50` assembly artifact identifies the exact machine instruction address of that load. Do not present a byte address for the load.

## First effects before a possible fault

For the two-reheat path, the valid element-1 call (`i=0`) is attempted before the invalid call; its data is serialized under the label `Reheat 1`. The second call invokes the `Reheat 2` banner formatter before the decompiled `NoPass` load. Earlier sections have also been passed to the formatter, including the Simulation Data rows and `NoReheats`.

These are attempted writer calls, not a retained runtime observation of external file bytes. The original `FILE*` is buffered; formatter calls can leave bytes in its buffer. The retained I/O receipt establishes the ordinary 4 KiB buffer and that write errors do not suppress later formatter calls, but it does not establish what was flushed before an access violation, whether this read faults in the GUI heap layout, whether process termination flushes the stream, or the final file length/content. If the invalid read does not fault, the loaded integer and downstream output are still unspecified. The caller may continue only if the callee returns; the fault and continuation are unknown.

`evidence/gui_mo/s1/mdt/save_d0.c` shows the outer Save order: active operation Save and project-file write precede the per-operation property writer loop, and the MST writer follows that loop. The Cogging writer returns 1 on its normal path and on a null `fopen`; Save ignores individual property return values and advances to later rows. That establishes handled continuation for open failure and normal return. It says nothing about a fault before the Cogging writer returns. There is no retained evidence mapping such a fault to a particular dialog, message, or exit code.

The status tool remains a hard gate: `analysis/gui-control-lifecycle-assessment.json` reports `unsupported_release_api`; `python tools/gui_validation_status.py` reports `overall=blocked` and `release_ready=false`. Its stated reason is that no supported Windows release operation plus verifiable receipt/status is exposed. No GUI control was acquired for this audit.

## Smallest original experiment after the release gate clears

Run one control and one target on independent disposable copies of the already-qualified active Cogging project `42_vdie` (Active index 3 selects operation 4, `SfOprCogging`). Keep project paths short under `C:\PROBLEM`; retain complete before/after file manifests and hashes.

1. Control copy: retain the valid one-reheat MDT and invoke the original GUI Save once. Record whether the project opens, Save returns, Cogging MDT/MST are written, and any dialog. This checks that the fixture and public Save route are active.
2. Target copy: create a complete two-reheat Cogging MDT from the one-reheat file, with a matching `SIMLST` list and two serialized REHEAT blocks. Before opening the GUI, parse it read-only with the existing reader and require `nReheats == 2` and `reheats.size() == 2`. Keep Active index 3 unchanged. Invoke original Save once.
3. Capture only observed outcomes: whether Save returns; whether the process remains alive/exits; any exception code and dialog title/text/buttons; and hashes/sizes for the full copied project, Cogging MDT, MOPROJ and MST. Stop after that attempt; do not retry to make one unstable result look deterministic.

The target result is exploratory evidence about one original run, not a portable output contract. If it faults, preserve the observed exception and partial-file inventory; if it completes, preserve the exact bytes but do not treat the undefined `NoPass` value as a stable expected value. Until the release gate clears, the deterministic work remains the candidate-only `WRITE_MDT` guard regression; its `M2M_E_UNSUPPORTED` result is not an original status mapping.

No source, build, GUI, or mutable REA state was changed for this item.
