# Resume 59: Cogging two-reheat fault order and status boundary

## Original call and status path

The retained target is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, bound in `analysis/wave3-cogging-native-io-contract.json`. The Cogging property writer at `0x1407617e0` opens the output with `fopen(path, "wt")` before calling `masterWrite` (`0x14076a8c0`). Its null-file branch bypasses serialization and returns 1; after a nonnull open, normal completion also returns 1. The retained contract says `fprintf` and close results are ignored.

The Save loop evidence in `evidence/gui_mo/s1/mdt/save_d0.c` (`FUN_1400980d0` / `FUN_1400982e0`) calls each operation property writer in row order and does not branch on the Cogging writer's return value before advancing to later rows and the MST stage. Therefore a failed Cogging open is a handled skip, with later Save stages attempted. The retained source does not show a Cogging-specific failure dialog or error status for that branch.

That conclusion does not cover `NoReheats >= 2`. The retained static bounds analysis (`analysis/resume50-cogging-loop-bounds.md`, derived from `evidence/gui_mo/s1/mdt/a8c0.asm` and `annot_all.c`) says that for two records the second serialization-loop iteration passes a one-past pointer to the REHEAT writer. It emits the “Reheat 2” banner, then attempts a four-byte `NoPass` read beyond the allocation. If that read faults, the writer cannot reach its ordinary return-1 path and the enclosing Save loop cannot be assumed to advance. If it does not fault, the value and all later effects remain unspecified. Static evidence therefore does not determine process status, a GUI exception dialog, Save continuation, output residue, or MST outcome for the two-reheat case. Do not map the candidate `M2M_E_UNSUPPORTED` to an original dialog/status.

## Candidate distinction

Current `source/apps/moproj2mst/src/datafiles/mdt_write.cpp::masterWrite` throws `M2M_E_UNSUPPORTED` for count 2 or greater after appending the `NoReheats` record. Through public `M2M_FLAG_WRITE_MDT`, the conversion stages MDT bytes and applies the side files and MST only after successful conversion; the caught candidate error prevents those final writes. The candidate-only public regression in `source/apps/moproj2mst/tests/s/test_cogging_two_reheat_guard.cpp` pins this guard and unchanged inventory; it does not characterize original status or output.

With candidate `M2M_FLAG_WRITE_SAVE`, the separate direct-save path opens/truncates the Cogging file before serialization and then throws at the guard. That C++ exception prevents later property rows and the candidate MST path. The possible buffered prefix during unwind is untested. This is a candidate route ordering fact, not evidence for the original invalid-read result.

The CLI wrapper is also distinct from the original GUI: its `wmain` sets `SEM_NOGPFAULTERRORBOX` and returns the converter result code. This cannot establish how a fault in the original GUI would be surfaced.

## Smallest testable branch

The smallest branch with a retained, defined original comparison is one reheat. Use `42_vdie` with active diagram index 3 and its actual parsed `NoReheats 1` model. Resume 51 already ran candidate public `--write-mdt` on a disposable copy: exit code 0, with candidate MDT matching historical oracle-B except for the product banner. This is the positive public-path control.

For the two-reheat boundary, keep the new test candidate-only: make a disposable derived file with a complete second serialized REHEAT record, confirm the parsed count and vector size are both 2, invoke public `WRITE_MDT`, and assert only the explicit candidate refusal and no staged MDT/MST application. This is the smallest deterministic regression for the guard. There is no smallest assertion for original two-reheat status or dialog mapping in the retained evidence; that requires a separate original runtime observation on a disposable fixture, and any output must remain exploratory because the native path is undefined at the invalid read.

No source, build, CMake, GUI, or mutable REA state was changed for this audit.
