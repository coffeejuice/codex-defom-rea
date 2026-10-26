# Checkpoint 50: bounded SimCtrl numeric branch check

## Selected persisted field

Use `DSMAX` in `B:98_B_DS3F/Operations/Task00001/SimCtrl.KEY`. The retained original and the candidate produced from the disposable B98 copy both contain:

```text
DSMAX     2.1660000000E+000
```

Both files are 2,232 bytes and SHA-256 `50f8f88cf7ec26cd0d46b4c9816185b2abd24b7b9ed606884c613f700a58865c`. A byte-for-byte comparison of the full files found no differences. Other finite numeric records in that same file, including `DTSUB`, `DPLEN`, and both `CVGERR` operands, also have identical raw tokens; the hash equality establishes all 2,232 bytes match, not only the displayed lines.

Evidence paths:

- Original: `evidence/gui_mo/oracle_b/98_B_DS3F/Operations/Task00001/SimCtrl.KEY`
- Candidate: `build/b98-repro-checkpoint48/Operations/Task00001/SimCtrl.KEY`
- Candidate invocation/output hashes and historical qualifications: `analysis/resume48-b98-raw-repro.md`

## Source-qualified write path

The current side-file write path is `engine::saveSimCtrl` at `source/apps/moproj2mst/src/engine/save_current.cpp:278`, which calls `def::key::write_simctrl_key_stream`. That writer dispatches group 1 in `source/port/defkey/src/item_writers/task_key_writers.cpp:65`. With this case's `dttype.ndim == 0` and `time_flag == 0`, the selected branch invokes `write_dsmax` (`source/port/defkey/src/kw_g1a/write_g1a.cpp:82`), whose recovered format string is `"%-6s %20.10E\n"`. The value in the opened SimCtrl model is 2.166, serialized with ten fractional digits and a three-digit signed exponent. The original native wrapper is `0x1400597d0`; the retained contract records narrow `toLocal8Bit` path conversion then CRT `fopen("wt")`, while the numeric body is the recovered KEY writer.

This establishes a concrete *bounded match* for one persisted value and one selected writer branch. It does not reveal a wrong candidate branch in this fixture. The test specifically exercises a noninteger value so a locale-sensitive decimal mark would differ visibly; the observed original token uses `.` and the candidate matches it byte-for-byte.

## Locale and action qualifications

The original SimCtrl.KEY is a retained historical Oracle-B artifact. The candidate file came from checkpoint 47 CLI running on the disposable copied B98 project; no original executable was run in this validation. It is not a same-session or newly paired original action. The matching ASCII numeric record supports this data point only; do not infer the target process's general CRT locale from one historical file. In particular, it cannot prove behavior under a comma-decimal locale, other format strings, parser acceptance, malformed numeric input, rounding boundaries, or all SimCtrl branches.

The candidate's `rtcrt::fprintf` implementation is a source-level CRT-format compatibility layer. The current check validates its bytes against this retained output, but separate original-bound cases are still needed before claiming broad numeric-locale fidelity. Keep Win32 ACP and CRT `LC_NUMERIC` as distinct unknown process observations; neither follows from this matching line.

## Highest-value follow-up

On an already-required original probe, capture one active SimCtrl KEY after recording target-process `setlocale(LC_NUMERIC, NULL)`, selecting a persisted field whose recovered writer uses `%E` and whose value has a nontrivial fractional part. Compare exact full bytes and hashes, retaining the original and candidate outputs. If the original action cannot expose that CRT locale without exceeding its frozen probe contract, report the static format and this historical bounded match only; leave process locale generalization open. Do not add locale normalization to the comparator.
