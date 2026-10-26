# Corpus test harness (Phase 0.5)

Runs the original `DEF_PRE_64.exe` (through the oracle cache, [oracle-runner.md](../../docs/oracle-runner.md))
and the port (`port/def_pre`) on the same copies of each corpus input and compares what they wrote
with [defcmp](../defcmp/README.md). Test tiers follow §6 of
[port-plan-def-pre-batch.md](../../docs/port-plan-def-pre-batch.md).

## Commands

```bat
port\build.cmd                                  :: configure + build (MSVC, Ninja) + tier 1
ctest --test-dir port\build -L deformdb         :: tier 1 of one module (deformdb, defcmp, def_pre)
cmake --build port\build -t tier2               :: tier 2: the 26 smoke cases, ctest -j 8
cmake --build port\build -t tier3               :: tier 3: all 1 907 cases, one ctest process each
cmake --build port\build -t corpus_table        :: the same 1 907 cases in one process: pass/fail table
                                                :: + port\build\corpus_report.tsv (one row per case)
```

Without CMake: `python -I harness/corpus/runner.py --port <def_pre.exe> --defcmp <defcmp.exe>
(--tier 2|3 | CASE_ID...) [--primary] [--kind dbkey] [-j N] [--report F.tsv] [--offline]`.
`--offline` (or `-DDEF_PRE_PORT_OFFLINE=ON`) never starts the original and errors on a cache miss.

ctest names are `t2:<id>` and `t3:<id>`; labels `tier2`, `tier3`, `tier3_primary`
(SI only), `corpus`, and the case kind (`m`, `dbkey`, `kfrw`, `cmd`).

## Cases (`cases.py`)

Listed from `docs/corpus/*.tsv` only; the corpus files (`$DEFORM_CORPUS`, default
`C:\Users\alext\Projects\DEFORM_corpus`) are read when a case runs.

| Id | What runs | Count |
|---|---|---:|
| `m/<db>/<mode>` | `-m header\|stepList\|lastStepNo\|simStepList -d <db> -o <mode>.txt` | 8 |
| `dbkey/<db>/<step>` | `DBREAD <step> / <db> / KFWRIT / STEP_<n>.KEY` (every step of both DBs) | 822 |
| `kfrw/<path>` | `KFREAD <file> / KFWRIT / KFWRIT_OUT.KEY` (every KEY, KWD and STD file) | 1 029 |
| `cmd/<path>` | the command file (`.MST`, `.d2mo`, `.d3mo`, `.INI`) as `CMD.KEY`, in a copy of its folder | 48 |

Both `KFREAD\nfile` and `KFREAD 1\nfile` give the same output from the original (checked on
`CREEP_Example1_SI.KEY`).

## Tier 2 (`select_tier2.py` → `tier2.tsv`)

Each case carries features: every registry keyword and command its input holds (KEY side from
`key_coverage.tsv`, DB side from `db_coverage.tsv`) and its batch path (each `-m` mode, DB export
of a positive and a negative step, KFREAD+KFWRIT per file kind and dimension, command files per
extension). Tier 2 is a greedy set cover of all features of the primary (SI) cases, ties to the
smaller input, then redundant picks dropped. Result: 26 cases, 264 features (all 247 keywords and
commands the SI corpus has), 96 MB of input. `tier2.tsv` lists each case's unique features.
Re-run at phase gates: `python -I harness/corpus/select_tier2.py`. Five keywords exist only in
British-unit files (`DSTROKE GENAXS GRNMDL TCYCLE TXTODF`); they are in tier 3.

## Port runs

The port runs in a scratch directory laid out exactly like the oracle's (same inputs, argv,
stdin, `TEMP` inside it), through the oracle's own run code (`runner.PortRunner`), so its output
directory has the cache-entry layout `defcmp oracle` reads. Runs live in `$DEFORM_PORT_RUNS`
(default `%USERPROFILE%\.deform-port-runs`); passing runs are deleted, failing ones kept (the
report's `port_dir` column). DB inputs are hard links to the oracle's read-only staged copies.
Delete `.deform-port-runs\entries` when it grows: a failing tier 3 with a real port keeps up to
~55 GB of Inc718 KEY exports.

## Filling the oracle cache

```bat
python -I harness\corpus\populate_oracle.py -j 8 --log %USERPROFILE%\.deform-oracle\populate.log
python -I harness\corpus\populate_oracle.py --status
```

Resumable: cached cases are skipped, so a stopped run is just started again. Start it detached
(`Start-Process python -ArgumentList ... -WindowStyle Hidden`) for long fills.

## Phase 0 exit check (2026-10-21, stub `def_pre`)

Oracle cache: all 1 907 cases (both DBs: 8 `-m` runs and 822 step exports; 1 029 KEY/KWD/STD
round trips, SI and British; 48 command files), no runner errors, 54 GB. The original exits 0 on
every case except 6 command files (exit 1).

`cmake --build port\build -t corpus_table` against the stub, 11 s:

| Cases | PASS | FAIL | ERROR |
|---|---:|---:|---:|
| cmd (24 SI + 24 British) | 0 | 48 | 0 |
| dbkey (822) | 0 | 822 | 0 |
| kfrw (482 SI + 547 British) | 0 | 1 029 | 0 |
| m (8) | 0 | 8 | 0 |

All fail as expected: the original writes a transcript and at least one file in every case, the
stub writes nothing. `ctest -L tier3 -j 12` (one process per case) takes 36 s, tier 2 about 2 s.
