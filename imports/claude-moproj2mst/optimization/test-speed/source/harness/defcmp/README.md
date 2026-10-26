# defcmp: output comparator for the DEF_PRE_64 port

Phase 0.2 of [port-plan-def-pre-batch.md](../../docs/port-plan-def-pre-batch.md). Compares what the
original `DEF_PRE_64.exe` wrote (usually an entry of the oracle cache, see
[oracle-runner.md](../../docs/oracle-runner.md) on branch `claude/oracle-runner`) with what the port
wrote, and says where the first difference is.

| Output | Rule | A mismatch reports |
|---|---|---|
| KEY file | byte for byte | first differing line and column, the keyword block it is in (nearest line above that starts with a DEF_PRE registry keyword) |
| `.DB` file | byte for byte outside a mask list of volatile words | the first differing step, header word / region / keyword (decoded with `deformdb`), or the record 2 field |
| stdout / stderr | line by line after masking dates, times and paths | first differing line and column, after masking |
| any other file | byte for byte | first differing byte offset |

Every comparison digests both sides first (XXH64 of the masked or normalised content) and only
diffs on a mismatch. A digest printed by `defcmp hash` can be stored next to an oracle result and
passed back with `--expected-digest`; a passing comparison then never reads the expected file.

## Build

```
harness\defcmp\build.cmd            :: Release build + tier-1 tests (MSVC 2022 BuildTools, Ninja)
```

Builds `deformdb` from `../../deformdb` as part of the project. Output: `build\defcmp.exe`,
`build\test_defcmp.exe`.

## Command line

```
defcmp compare EXPECTED ACTUAL                 :: kind from the extension (.KEY, .DB, stdout.txt/stderr.txt)
defcmp compare --kind transcript a.log b.log   :: force a kind: key | db | transcript | bytes
defcmp compare --mask +header-only a.DB b.DB   :: DB masks, see below
defcmp dirs    EXPECTED_DIR ACTUAL_DIR         :: every file of two output directories
defcmp oracle  ENTRY_DIR ACTUAL_DIR            :: an oracle cache entry against a port run
defcmp hash    [--mask SPEC] FILE...           :: digest as compared (DB digest depends on the masks)
defcmp masks   [--mask SPEC] FILE.DB           :: the byte ranges a DB comparison ignores
```

Exit code: 0 equal, 1 different, 2 error. Example of a report:

```
db: DIFFERENT at step -1 (slot 1), A[1564], region LLCRZ word 1, keyword RZ, row 1 comp 1, object 1 node 1
  expected: -1000 (0xC08F400000000000)
  actual:   -1000.0000000000001 (0xC08F400000000001)
```

### `oracle`

`ENTRY_DIR` is `%USERPROFILE%\.deform-oracle\entries\<k[:2]>\<key>` (`result.json`, `stdout.txt`,
`stderr.txt`, `files\`). `ACTUAL_DIR` has the same layout: `stdout.txt`, `stderr.txt`, the written
files in `files\` (or at the top level), and the exit code in `result.json` (`"exit_code"`) or in a
one-line `exit_code` file. Rows:

* `exit code`: a non-zero exit is a cached result like any other and must match;
* `stdout.txt`, `stderr.txt` as transcripts;
* every file the original created or modified, including the empty `NEWEQ.MSG` every `-m` run leaves;
  a file the port writes that the original did not is a difference;
* a file the original deleted must be absent;
* files the oracle kept by size and sha256 only (over 2 GiB) are `SKIP`.

### DB masks

`--mask` takes comma-separated items applied to the default set, left to right. A plain name as the
first item replaces the set; `+name` adds; `-name` removes; `bytes:OFFSET:LEN` masks a raw range.

| Name | Bytes | Why it varies | Default |
|---|---|---|---|
| `smi2` | record 2, 88–95 | release date of the writing program | off |
| `smi3` | record 2, 96–103 | date the simulation was first written | on |
| `smi4` | record 2, 104–111 | date of the last write | on |
| `istart` | record 2, 112–115 | directory entry last written by the pre-processor | on |
| `ho-dir` | directory words 10 and 13 of header-only entries | stale record numbers ([task-dbmanager-purge-merge.md](../../docs/task-dbmanager-purge-merge.md) §6) | off |
| `ho-h7h8` | step-header reals H7, H8 of header-only entries | stale array sizes left by the solver (same §6) | off |

Groups: `dates` (smi3, smi4), `default` (dates, istart), `header-only` (ho-dir, ho-h7h8), `all`,
`none`. Examples: `--mask none`, `--mask +header-only`, `--mask all,-smi2`.
The header-only fields are where `deformdb::Writer` deliberately differs from DEFORM; a faithful
port should pass without them.

How a DB difference is located: the first unmasked differing byte is found by streaming both files;
if it is in record 1 or 2 the field is named (a moved NXTREC is first traced to the step that
caused it). Otherwise the step owning that record is decoded on both sides and compared in this
order: directory words 1–9, step header, region tables without their stored-size and record words,
MAR, A, dynamic arrays; then a full pass over all steps, and finally the record placement
(directory words 10–13, region-table records). So a value change that makes the RLE data longer
and shifts every later record is reported as the value, not as the moved records. Values are
compared bit for bit (`-0.0` differs from `0.0`).

### Transcripts

Dates (`10/21/26`, `2026-10-21`, `21-Oct-2026`, `Oct 21, 2026`, ctime strings, weekday names),
times (`09:31`, `09:31:50.12`, `9:05 PM`) and absolute paths (`C:\...`, `\\server\...`, blanks
allowed when the next word still contains a separator) become `<DATE>`, `<TIME>`, `<PATH>`.
`--root C:\scratch\case1=<CASE>` replaces a run directory first (case-insensitive, either slash),
so the rest of the path is still compared. `--mask-regex RE` masks more; `--keep-dates`,
`--keep-times`, `--keep-paths` switch a mask off. Version strings like `11.0.2` are not dates.

## Library

`include/defcmp/defcmp.hpp`: `compare_key`, `compare_db`, `compare_transcript`, `compare_files`,
`compare_dirs`, `compare_oracle_entry`, the digest functions and `DbMasks::parse`. Link
`defcmp` (CMake target), which pulls in `deformdb_core`.

## Tests

* `test_defcmp` (ctest labels `defcmp`, `tier1`, well under a second): XXH64 vectors; KEY reports
  (data line, keyword line, text values such as object names, missing line, CRLF); transcript
  masking; synthetic DBs written with `deformdb::Writer` on two dates, with planted ISTART, title,
  A-region (LLCRZ → keyword RZ, row/component), MAR-region (IIFNC), RLE-length-changing,
  step-header, header-only H7 and directory-word changes, and a truncated file; oracle entries
  (deleted file, sha256-only file, exit code, missing NEWEQ.MSG).
* `test_defcmp_corpus` (labels `defcmp`, `tier3`), configured with real files:

  ```
  cmake -S harness/defcmp -B harness/defcmp/build -G Ninja ^
    "-DDEFCMP_TEST_DBS=C:/PROBLEM/TEST_REA/COG_P1_B1/COG_P1_B1.DB;C:/PROBLEM/TEST_REA/Inc718_D420_GFM_V_DIES_Angle2_18/Inc718_D420_GFM_V_DIES_Angle2_18.DB" ^
    "-DDEFCMP_TEST_KEYS=<oracle entry>/files/STEP_25.KEY;..."
  ctest --test-dir harness/defcmp/build -L tier3 --output-on-failure
  ```

  Each file is copied twice into the build tree and only the copies are read; differences are
  planted in one copy (SMI4 date, ISTART, the first stored LLCRZ word, an RZ data line, the NSTART
  line) and both copies are deleted afterwards.

Results on 2026-10-21 (both test DBs, KEY exports of COG steps 25 and −1 and Inc718 steps 690 and
100 from the oracle cache): all checks pass. Timing on the 5 GB Inc718 copy: identical files 5.3 s,
with a cached expected digest 2.6 s, locating a planted LLCRZ change 5.1 s; the 68 MB KEY of step
690 compares in 0.14 s. Also passing on copies of three SI install examples (2D
`ResistanceSpotWelding/..._Cooling_off_cycle_SI.KEY`, 3D `Extrusion/ALE/European_Benchmark_2011_ALE.KEY`,
3D `Coupled_Diestress/3D_ConRod_SI.KEY`, up to 66 MB).
