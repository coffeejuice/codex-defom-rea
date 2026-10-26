# Historical Save differences: RS55 and Diestress 98

RS55 is a confirmed reconstructed-state difference: two solver-parameter bytes.
Diestress 98 has a known absolute-path environment difference that the inherited
comparator does not relocate. No production source was changed in this package.

## RS55: exact bytes and original mutation

The frozen candidate CLI from `wave1-connector-final-v2-build.json` was copied,
hash-checked, and run with `--write-save` on a disposable copy of
`RS_55_Multiple_Operations_Example1_before`. It returned 0 in 0.094 seconds. All
outputs remain under `build/wave2-save-diffs/rs55-1792753172387017300`; imported
fixture hashes were unchanged. No original process ran.

`Operations/Task00002/SimCtrl.KEY` is 2,146 bytes in both outputs. Exactly two
bytes differ, at zero-based offsets **1439** and **1455**: candidate ASCII `0`,
original ASCII `1`. Line 51 is `SOLMTD       2       4`; line 52 is:

```text
candidate:         0       0       0       0
original Save:     0       1       0       1
```

Candidate SHA-256 is
`2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e`;
original Save SHA-256 is
`79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b`.
Every other byte agrees. This is not a path, banner or line-ending discrepancy.

Static inspection of the acquired original image
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
identifies the original mutation:

1. `SfPreSimControl300` iteration fill `0x140ec19f0` sees `SC+0x2e4 == 2`
   and calls `QAbstractButton::setChecked(true)` on the CG radio at UI `+0xa90`
   (`0x140ec21b6`). Its setup calls `QMetaObject::connectSlotsByName` at
   `0x140f21d67`.
2. Qt metadata at file offset `0x2c16300`, method 64, names
   `on_m_rbtnDefSolCG_toggled(bool)`. Dispatch table `0x1410dbe88` entry 64
   reaches `0x1410dbcf1`, whose call at `0x1410dbd05` reaches `0x140ed01b0`.
3. With enabled=true, that slot ensures parameter count 4 (otherwise defaults
   2,4,1,1), validates truncated S1 in 0..2 (otherwise stores 2), stores 1 at
   `SC+0x2f8` when truncated S2 <= 0 (`0x140ed0419`), and stores 1 at
   `SC+0x308` unless truncated S4 is 1 or 5 (`0x140ed0508`). S3 is not validated
   in this slot. The binary constants were decoded as doubles 1,2,4,5.

The original SOLMTD reader `0x140b69090`, branch `0x140b6a3bb`, directly copies
the four explicit integers to doubles at model `+0xa40..+0xa58`. It does not
perform this normalization. The preserved writer also prints existing values.
Therefore neither a generic KEY-reader clamp nor a writer substitution is a
faithful fix.

The current Forming editor model in `engine/loader_l3.cpp` reconstructs the
smaller `SfPreSimControl` fill/apply (`0x1417ec030/0x1417ec870`). It has no
`SfPreSimControl300` iteration state. The exact normalization is recovered, but
the selected fresh-session lifecycle's creation/fill activation boundary still
needs tracing before assigning a production call site. Six paired historical
SimCtrl files retain zero-valued S2/S4 (including RS55's inactive Task00001), so
normalizing every task or every engine model would overreach the evidence.

The recommended next recovery unit is this original UI-state dependency,
including count defaults, CVTTSD2SI truncation, enabled/disabled behavior and
the correct fill timing. Preserve tests for valid S4=5 and unchanged inactive
tasks. Source ownership must be assigned before any implementation.

## Diestress 98: exact environmental field

The immutable original `98_B_DS3F/Gear_blank.MST` is 2,756 bytes, SHA-256
`e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.
Its first KFREAD is line 29; the following line 30 starts at zero-based byte
offset **1338** and is:

```text
C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY
```

Original writer `0x1402b5410` obtains the absolute subproject DB path, replaces
`.DB` with `.KEY`, and passes it verbatim to `0x1401344a0`. The port's
`ops/diestress.cpp` follows that chain. A copied project therefore produces a
different absolute location by design.

The inherited `test_save_folder.cpp::relocated()` searches for its **imported**
expected directory. The original MST contains that prefix zero times and the
historical original prefix once. Its environment adjustment cannot match this
field after acquisition. The raw mismatch must remain visible. Any later
qualified view should map only the evidenced absolute case-prefix at line 30,
retain raw hashes, and prove the remaining bytes independently. No broad
normalizer or production path rewrite is justified. The sweep deleted its B98
candidate bytes, so this investigation does not claim a complete post-mapping
byte equality.

## Retained work

`wave2-save-diffs-diagnosis.json` records addresses, offsets and artifact hashes.
The capture receipt and byte inspector retain exact RS55 inputs/output identity.
`wave2-save-diffs-original-*.asm/json` preserve static disassembly, import names,
metadata dispatch and instruction-pattern hints. Hints are distinguished from
decoded function evidence. `tools/save_diff_capture.py` reproduces the isolated
candidate capture; `tools/save_diff_inspect.py` compares retained bytes without
executing a target; `tools/save_diff_pe_xrefs.py` performs read-only PE hints.
No full suite, build, REA session or original-code execution occurred. Historical
reference session qualifications remain applicable; token usage is unavailable.
