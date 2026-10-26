# CP87: source-qualified `STRETCH.DAT` integer successor

## Selected reachable mismatch

`STRETCH.DAT` has a reachable one-value integer read in the same Extra3D Open routine as `SW2SP.DAT`. In the retained original decompile `analysis/wave2-save-diffs-300-support-140ec8110.c`, the `FUN_140ec8110` call at line 65 passes count 1 to `FUN_1410da690` for the string at `UNK_142b14e78`; the call at line 79 also passes count 1 for `UNK_142b14e90`. `analysis/wave2-save-diffs-300-extra3d-keywords.json` maps those addresses to `SW2SP.DAT` and `STRETCH.DAT`, respectively. The second call's result is displayed as a decimal integer in the stretch-kind field (`_setNum_QString...` at line 88), so it is observably part of the page state.

`FUN_1410da690` is retained in `analysis/wave2-save-diffs-300-support-1410da690.c` and the sidecar decompile JSON. It initializes the requested int destination to zero, checks/opens a QFile ReadOnly, constructs a default QTextStream, performs `operator>>(int&)` while not at end and below the supplied count, closes the file and returns true after successful open. The QtCore getter and int operator are the same functions used by the SW2SP contract; the PE SHA, parser behavior, and default-C-locale/no-custom-locale qualifications are recorded in `resume83-sw2sp-parser-independent-review.md`, `resume84-sw2sp-parser-plan.md`, and `resume67-sw2sp-locale.md`.

The candidate already has a correct single-extraction parser helper, but `NativeExtra3DIo::readInts` routes it only when the path is exactly `SW2SP.DAT`. A one-element `STRETCH.DAT` span currently reaches the legacy generic integer parser instead. For a minimal distinguishing test, `STRETCH.DAT` bytes `0X10\r\n` should yield 16 under the original case-folded base-0 Qt path, while current generic candidate behavior takes its legacy uppercase-prefix fallback and yields zero. The analogous lowercase-prefix/uppercase-digit input `0xA\r\n` should yield 10 in Qt but currently fails in the generic candidate branch. The first case is a simpler, finite adapter test.

## Exact source and test boundary

- Original executable qualification: `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` as recorded in `wave2-save-diffs-300-extra3d-keywords.json`.
- Original caller: `FUN_140ec8110`, second `FUN_1410da690(..., 1)` call, source decompile line 79; string VA `0x142b14e90` is `STRETCH.DAT`.
- Shared original reader: `FUN_1410da690`, source decompile `wave2-save-diffs-300-support-1410da690.c`; one-element, default QTextStream `operator>>(int&)` path.
- Candidate caller: `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`, `fillExtra3D`, `io.readInts(qt::L("STRETCH.DAT"), integer)` with the same one-element array used for SW2SP.
- Candidate owner/test seam: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` and `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`.
- Smallest regression: write exact `STRETCH.DAT` bytes `0X10\r\n` to the disposable native-adapter fixture directory; call `readInts(qt::L("STRETCH.DAT"), std::array<int,1>)`; assert adapter success and value 16. Keep the parser gate exact to `values.size()==1` and path `SW2SP.DAT` or `STRETCH.DAT`. Verify another filename and a multi-value span still take the legacy path. Do not alter the later three-double `STRETCH.DAT` read or its file-open behavior.

## Bounds and missing evidence

This extends only an already reachable one-int caller that shares the same original reader and Qt stream class; it does not rely on inventing a multi-value call. It should reuse the existing parser/helper and corrected 20-row oracle, not implement another numeric conversion. Keep the ASCII decoder/input constraint and ordinary QFile path/open conditions. No claim is made for ANSI/ACP, BOM or malformed bytes, custom stream locale/options, subsequent extraction, or the original process's installed environment. Static evidence suffices for this finite input because the source caller, reader, default stream constructor and case-folding branches are all independently retained. A fresh original-process comparative run would add runtime confirmation but is not required to decide the parser contract.

No source, test, build, GUI or REA state was changed during this read-only audit.
