# Oracle runner (`tools/oracle/def_pre_oracle.py`)

Runs the original `DEF_PRE_64.exe` on **copies** of test inputs and caches what it produced, so
every port test can compare against the original without re-running it. Phase 0 item of
[port-plan-def-pre-batch.md](port-plan-def-pre-batch.md).

Python 3.10+, standard library only. Windows (DEF_PRE is a Windows program).

## Quick start

```
python -I tools/oracle/def_pre_oracle.py m all C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB
python -I tools/oracle/def_pre_oracle.py key-export C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB 25 -1
python -I tools/oracle/def_pre_oracle.py cases tools/oracle/cases/test_rea.json
```

Each case prints one JSON line: `label`, cache `key`, `cached` (true = DEF_PRE was not run),
`exit_code`, `dir` (the cache entry) and `outputs` (files created/modified/deleted). The exit
status is 1 if any case failed to run.

From Python:

```python
import sys; sys.path.insert(0, "tools/oracle")
from def_pre_oracle import Oracle, Case, Input

oracle = Oracle()
r = oracle.run(Case.key_export(r"C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB", 25))
r.cached, r.exit_code       # False the first time, True afterwards
r.read_text("STEP_25.KEY")  # DEF_PRE's KEY export
r.stdout                    # bytes, the menu transcript
r.files                     # {"STEP_25.KEY": {"status": "created", "size": ..., "sha256": ...}}
```

`oracle.lookup(case)` returns the cached result or `None` without running anything.

## The three batch paths

| Path | CLI | Python |
|---|---|---|
| `-m header\|stepList\|lastStepNo\|simStepList -d <db> -o <file>` | `m <mode\|all> DB... [--out NAME]` | `Case.m_mode(mode, db)` |
| scripted stdin (menu protocol) | `stdin --script '\n2\n1\n...' --input PATH[=NAME] ... [args]` or `--script-file F` | `Case.script(stdin, inputs)` |
| keyword program via menu 2 → 1 (`\n2\n1\nCMD.KEY\n\nE\nE\nY\n`) | `keyfile CMD.KEY --input ...` | `Case.keyfile(text, inputs)` |
| KEY export of one step (`DBREAD n` / db / `KFWRIT` / `STEP_n.KEY`) | `key-export DB STEP...` | `Case.key_export(db, step)` |
| `DEF_MULTI.INI` | `multi DEF_MULTI.INI --input ... [--mkdir DIR]` | `Case.multi(ini, inputs)` |

`keyfile` covers DBREAD/KFWRIT/GENDB and any other command keyword; for example a CMD.KEY of
`DBREAD 25 / COG_P1_B1.DB / GENDB / NEW.DB` returns the generated `NEW.DB`. Negative steps are
written to `STEP_M<n>.KEY`. `key-export` runs one DEF_PRE process per step so each export starts
from a clean model.

`--input PATH[=NAME]` places a file at NAME in the scratch dir (default: its file name); a
directory is added recursively under NAME (`=.` puts its contents at the top level).
`--mkdir DIR` creates an empty directory first (DEF_MULTI.INI flows that write into
`DoeStudies/...`, for example). Global options go before the subcommand: `--refresh`,
`--offline`, `--timeout S`, `-j N`, `--copy-to DIR` (copy each result into `DIR/<label>`),
`--keep-scratch`, `--cache DIR`, `--exe PATH`.

## Case files

`cases` takes JSON files holding one case or a list:

```json
{"label": "my case", "args": [], "stdin": "\n2\n1\nCMD.KEY\n\nE\nE\nY\n", "timeout": 600,
 "dirs": ["DoeStudies/DoeStudy1/Control/Regions"],
 "inputs": [{"name": "a.DB", "source": "C:/x/a.DB", "link": true},
            {"name": "Materials", "source": "C:/x/Materials"},
            {"name": "CMD.KEY", "text": "DBREAD 1\na.DB\nKFWRIT\nOUT.KEY\n"}]}
```

Shorthands: `{"m": "header", "db": "..."}` and `{"key_export": "...DB", "step": 25}`. Relative
`source` paths resolve against the case file's folder. `tools/oracle/cases/test_rea.json` is the
set for the two test DBs.

## Inputs: copied, or linked read-only

Inputs are never used in place. By default each one is copied into the scratch dir (hashed in
the same pass, so a file that changes mid-copy is caught). `link: true` (`--link`, and the
default for `m` and `key-export`) instead hard-links a read-only copy kept in `cache/stage/`:
the 5 GB Inc718 DB is staged once and every later case on it starts instantly. Use it only for
files DEF_PRE reads; if DEF_PRE changes a linked input the run fails and asks for a copy.

## What is collected

Every case runs in a new empty directory with `TEMP`/`TMP` pointed at `.oracle_tmp` inside it.
After the run the directory is compared with its state before the run:

* `stdout.txt`, `stderr.txt` (raw bytes) and `exit_code`;
* every file created, every input whose content changed (`modified`), every input deleted;
* files up to 2 GiB are stored in `files/<path>`; larger ones are recorded by size and sha256
  only (`stored: false`).

Not captured: writes outside the working directory other than TEMP (install directory, registry,
`%APPDATA%`). None were seen for the paths above.

## Cache

Location: `$DEFORM_ORACLE_CACHE`, else `%USERPROFILE%\.deform-oracle`. It is deliberately not
under `AppData\Local`: packaged apps (the Claude desktop app) see a private redirect of that
folder, so other processes would not share the cache.

The key is sha256 over: runner schema, sha256 of `DEF_PRE_64.exe` (the build), argv, stdin bytes,
sorted (name, sha256) of every input, and the empty dirs. Labels, timeouts, source paths and
copy-vs-link are not part of it, so the same case reached two ways hits the same entry.

```
entries/<k[:2]>/<key>/result.json   case, inputs with hashes, exit code, duration, file list
                     /stdout.txt /stderr.txt /files/...
stage/<sha256>                      read-only staged inputs (hard-link targets)
hashes/                             file-hash memo keyed by (path, size, mtime)
builds/<sha256>.json                DEF_PRE builds seen (lets --offline work without the exe)
locks/, tmp/                        in-flight runs
```

Concurrency: any number of processes and threads may call the runner at once. Each run has its
own scratch dir under `tmp/`; a lock file per key (and per staged input) makes one caller run
DEF_PRE while the others wait for its entry; the finished entry appears with one directory
rename, so readers never see a half-written result. Locks of dead processes, or older than
timeout + 300 s, are broken. Tested: six processes × four threads on a fresh cache ran each of
five Inc718 cases once and staged the 5 GB DB once.

Failure handling: a non-zero exit code is a result and is cached. A timeout (default 600 s,
process tree killed) or a runner error caches nothing and keeps the scratch dir under `tmp/` for
inspection. `--refresh` re-runs a case and replaces its entry. `--offline` (or
`DEFORM_ORACLE_OFFLINE=1`) never runs DEF_PRE and fails on a cache miss, for machines without
DEFORM.

Not in the key: environment (`DEF_CFG_DIR` and DEFORM's own configuration files). If those
change, run with `--refresh` or bump `SCHEMA` in the script.

## Observed behaviour of the original (v11.0.2, build sha256 `f78a2a5c…`)

* All paths above run headless with no licence prompt.
* Every `-m` run creates an empty `NEWEQ.MSG`; KEY-export runs do not leave one.
* Times on the test PC: `-m` modes 0.04 s, KEY export of COG step 25 0.11 s, Inc718 step 690
  about 1 s (68 MB KEY). Hashing and staging the 5 GB DB the first time takes about 8 s.
* The BEND_2D example's DEF_MULTI.INI (`KFREAD BEND_2D_REM.KEY` / `KFWRIT ...`) needs `BEND_2D.DB`
  and the `DoeStudies/DoeStudy1/Control/Regions` folder, and prints NBCD warnings on step 338.
