# AGENTS.md: working on the DEFORM recovery

This is the canonical guide for AI agents (Claude Code, Codex, Cursor, Copilot and others) and for
people new to the repo. `CLAUDE.md` imports it. Keep this file short and current; the details live
in the documents it links. When a rule here and a linked document disagree, fix the one that is
wrong in the same commit.

## 1. What this repo is

A static reverse engineering of DEFORM v11.0.2 (SFTC) programs, recreated as modern C++20 that
behaves exactly like the original, followed by work that builds on that baseline.

| App | State | Code | Plan |
|---|---|---|---|
| DEF_PRE_64 batch mode | finished: tag `v1.0-def-pre` (= `d7819e3`), gates `gate/p0` to `gate/p6-7` | `port/` | `docs/port-plan-def-pre-batch.md` |
| `.DB` reader/writer (new API) | in use, C ABI for DMO | `deformdb/` | `docs/db-format-spec.md` |
| Solver harness (DEF_ARM_CTL3, DEF_SIM_64, remesh workers, 2D chain) | planned | (to come) | `docs/port-plan-solver-harness.md` |
| DEF_GUI_MO_64 (Multiple Operations GUI) | planned, stage 1 (MOPROJ to MST) running on `claude/m2m-*` | `mo/` (to come) | `docs/plan-gui-mo-reconstruction.md` |

The storage plan (`docs/storage-plan.md`) moves `port/` and `deformdb/` to `libs/` and the apps to
`apps/<app>/` at a quiet point. Until that lands, the paths below are the current ones.

Repo map:

```
port/            superbuild (CMakeLists.txt, build.cmd) and the DEF_PRE libraries:
                 defrt -> defmodel -> defkey, defdbio -> defcmd -> def_pre   (+ testkit)
                 each library has a MODULE.md card (section 6); the superbuild also builds
                 deformdb and harness/defcmp, and their tests are part of tier 1
deformdb/        independent .DB reader/writer, C ABI (deformdb.dll), dbinfo/dbraw/dbmanager
harness/         corpus (tier 2/3 runner), defcmp (DB comparator), mcli, shell, gate scripts
tools/           oracle (runs the original), int (integration and e2e), extprobe, agents
tests/commands/  recorded command cases (JSON) per keyword, replayed against the oracle cache
docs/            plans, specs, conventions; docs/re/ = ledger, status files, per-command specs
```

## 2. Two modes: faithful and evolve

Every change is in one of two modes. Say which one in the commit message or PR when it is not
obvious.

**Faithful mode** (the default for anything that recreates a DEFORM program, including the
solver harness and DEF_GUI_MO recoveries, and for bug fixes in ported code). The port must
produce the same bytes as the original: same stdout, same files, same floating-point bits.
* Recover behaviour from the binary's code only. The corpus and example DBs test algorithms
  already found in code; they are never used to discover behaviour, and a mismatch is traced back
  to the code, never patched to fit the expected output.
* Keep the original operation order and rounding points; read the assembly where Fortran `-O2`
  reassociated or the decompile groups a sum differently. Build flags stay as in
  `port/cmake/DefPort.cmake` (`/fp:precise`, no FMA contraction, SSE2, no `/fp:fast`, no AVX).
* Output text goes through `def::rt::crt` (`fprintf`, `sscanf`, `%Flf`) with the original
  format strings; time only through `def::rt::now()` / `pin_clock()`.
* A "bug" in the original is reproduced, not fixed, in faithful mode. Fixing it is evolve mode.
* Full rules: `docs/port-conventions.md` sections 3 to 6.

**Evolve mode** (optimisation, swapping a module for a better open-source one, a new API, fixing
a DEFORM bug, anything a paper measures). The faithful build is the regression baseline.
* The faithful path stays buildable and testable: put the change behind a CMake option or a
  separate target or library, and name the switch in the module's `MODULE.md`.
* Run the same oracle tests on the evolved build. Every difference from the oracle is either
  zero or listed with its cause in `docs/evolve/<topic>.md` and accepted by Aleksandr; "close
  enough" is decided per case, never by loosening a comparator globally.
* Measure before and after with the `profile` skill and record the numbers in the module card's
  "Perf baseline" section.
* A new API wraps the faithful classes (layouts stay at the binary's offsets) rather than
  changing them.
* New code with no binary counterpart gets no `@src` tag and no ledger row. A new library gets
  its own `MODULE.md` (the generator picks up any `port/<dir>` using `def_port_module`). The
  record of each change follows the template in `docs/evolve/README.md`.

**Fidelity classes** used in the module cards: **F1 bit-exact** (output bytes and FP bits match
the oracle: `defrt`, `defmodel`, `defkey`, `defdbio`, `defcmd`, `def_pre`), **F2 format-exact**
(files it writes are valid and compare equal with `defcmp`, internals free: `deformdb`),
**F3 free** (test, harness and tool code).

## 3. Hard rules (both modes)

* Never modify a DEFORM binary or the install (`C:\Program Files\SFTC\DEFORM\v11.0.2`), never
  attach a debugger to a DEFORM process, never run the original on its own files: the oracle
  works on copies.
* Never analyse, document or port licence code (Sentinel, `LManager*`, `Dnp*`, `CHKPAS`,
  `SDSPM_*`, the process monitor at `1403c4504`). Tag it `// @src <addr>  omitted: licence`.
  The recreated programs have no protection.
* Expected test values come from the oracle, from tables in the binary or from a hand
  calculation shown in the test, never from running the port.
* Pushing to `main`, deleting branches, removing worktrees or local copies, and anything written
  to the NAS outside the storage plan's steps need Aleksandr's explicit yes.
* Never keep state shared between processes or sessions (caches, staged inputs, results others
  read) under `%LOCALAPPDATA%`: the Claude desktop app is MSIX-packaged and its writes there are
  redirected to a private folder other processes cannot see. Per-run scratch in `%TEMP%` (which
  lives there), such as the e2e and integration run folders, is fine.

## 4. Build and test

Windows 11, MSVC 2022 Build Tools + Ninja, CMake 3.24+, Python 3.10+ (standard library only for
repo tools). Run Python tools with `python -I`.

```bat
port\build.cmd [Release|Debug] [-D...]        :: vcvars64 + configure + build + tier 1 (default Release)
port\build.cmd Release -DDEF_PRE_PORT_OFFLINE=ON   :: same, but tier 2/3 never start the original
ctest --test-dir port\build -L tier1          :: all tier 1 (217 tests at cfb2b94d)
ctest --test-dir port\build -L defcmd         :: one module; -L p67-3 for one packet's tests
cmake --build port\build -t tier2             :: 26 corpus smoke cases, before pushing behaviour changes
ctest --test-dir port\build -L tier3 -j 12    :: 1,907 corpus cases + defcmp_corpus (about 1 hour; gates)
python -I tools\int\e2e_stdout.py --all -j 8  :: every tests/commands case through def_pre.exe (usage in its docstring; no --help)
python -I tools\int\run_int.py                :: OBJPOS drop + GENCTC/INICTC replays, one label per process
python -I docs\re\fold_status.py --check      :: ledger fold + @src checks; exit 0 = no errors (warnings are fine)
python -I tools\agents\gen_module_cards.py    :: refresh the MODULE.md cards (--check in gates)
python -I docs\re\stub_sweep.py port          :: DEF_NOT_PORTED stubs whose owner code exists
```

Last full results (int-all `d7819e3`, now `v1.0-def-pre`): tier 1 216/216 (217 since the branch
fold `cfb2b94d` added one test), tier 3 1,907/1,907,
e2e 518 pass / 3 skip / 3 fail. The 3 failures are known and not port bugs: `DefMpi.tmp` holds a
PID (`DEFINT/defint_3d_cog_64`, `REMESH/remesh_3d_db_64`), and
`CNVT3D/cnvt3d_geometry_wp_rev90_obj2` (e2e does not pin the clock for `srand(time)`).

Tier 1 tests are `port/<module>/tests/test_<topic>.cpp`, ctest name `<module>.<topic>`
(`def_pre_core.<topic>` for def_pre), extra labels on `// ctest-labels: p1-04 registry` lines
(the first 20 such lines count). A few tests are added by hand in a CMakeLists.txt and named
differently: deformdb `codec`/`writer`, `defcmp_unit`, `def_pre_starts`,
`defmodel.keyword_registry_matches_binary` (skipped without DEFORM installed). Tier 2/3 read the
oracle cache and by default run the original on a cache miss; `-DDEF_PRE_PORT_OFFLINE=ON`
forbids that (use it on a machine without DEFORM, or to prove the cache is complete).

## 5. The oracle

`tools/oracle/def_pre_oracle.py` runs the original `DEF_PRE_64.exe` on copies of the inputs in a
scratch folder and caches stdout, stderr, exit code and every file created, changed or deleted,
keyed by a hash of the case. Cache: `%USERPROFILE%\.deform-oracle` (override
`DEFORM_ORACLE_CACHE`), all 1,907 corpus cases plus the command cases, about 54 GB. Usage:
`docs/oracle-runner.md`; recording a new case: the `record-oracle` skill.

* It sets `SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | ...)` before starting
  DEF_PRE, because some paths start `DEF_REM.exe`, which needs `mpich.dll` (absent on Sim2). The
  original fails the same way, so outputs still match, and no dialog blocks the run.
* The 3D `.com` wrappers start the 64-bit programs only when a file `64bit.DAT` exists in the
  working directory (`DEF_REM3_GUI` at `0x401240`, DEF_ARM at `0x404409`; `DEF_MPIp4penv.DAT` is
  MPI settings, not the switch). See `docs/re/commands/DEFINT.md`.
* The cache can hold several entries per label: take the newest `created`.
* `-m`, DBREAD, KFWRIT, GENDB and `DEF_MULTI.INI` run headless with no licence prompt.

## 6. Module cards

Every library has `MODULE.md` (`port/<module>/MODULE.md`, `deformdb/MODULE.md`): purpose,
interface, fidelity class, oracle tests worth knowing, perf baseline, swap notes (hand-written),
then a generated block (dependencies, file counts, `@src` addresses by ledger status/area/phase,
remaining stubs, owning packets, tier-1 tests). Read the card before changing a module; after
changing one, run `python -I tools/agents/gen_module_cards.py` and commit the card with the code.

## 7. Ledger, status files and ownership

* `docs/re/pre64_batch_ledger.tsv` lists every binary function in scope (5,257 rows) with
  `area`, `first_phase` and `status` (`todo`, `ported`, `verified`, `not-needed`). Nobody edits
  its status column by hand: each packet owns `docs/re/status/<packet>.tsv`
  (`address<TAB>status<TAB>note`; a header row starting `address` may name other columns, as in
  `keyword-registry.tsv`, as long as one is `status`) and `fold_status.py` folds them in.
* **One owner row per address.** When two packets ported the same function, the owner keeps the
  row and every other status file turns its row into a comment row
  (`# 0x140198230<TAB>-<TAB>owner row in p-dbread-w1.tsv`). The non-owner's C++ forwards to the owner's
  function ("copies forward") instead of keeping a second body, so each `@src` is counted once.
  The fold only rejects rows that disagree on the status; rows that agree pass silently. 198
  addresses still have agreeing rows in two or more status files (mostly the keyword packets), a
  known cleanup item and not a precedent.
* **The lower layer owns** a function both layers need, because dependencies only point down
  (`defdbio` cannot call `defcmd`): DBREAD owns `140198230`, `14027fe20`, `140280290`,
  `1402571b0`; GENDB owns `1402399b0`, `140239a30`; `rc_array.hpp` (C09) owns `1403e5cb0`,
  `1403e6320`, `1403e6260`.
* One `// @data <addr>` declaration per global; a duplicate is duplicate state and an error.
  Shared `.rdata` constants live in `defrt/rdata.hpp`.
* Run results and reports go in `docs/re/int/`, never in `docs/re/status/` (the fold reads every
  `status/*.tsv`).
* `fold_status.py --check` must report 0 errors before a merge to `main`.

## 8. Where the evidence lives

| What | Where |
|---|---|
| Live Ghidra (12.1.4, JDK 21 in `C:\tools`) | `C:\Users\alext\Projects\deform-rea-work\ghidra\deform.gpr` (master); one copy `ghidra_<packet>` per parallel worker |
| Headless scripts | `deform-rea-work\scripts\run.sh` (`GPROJ=ghidra_x run.sh DEF_PRE_64.exe Decomp.java <absolute path>.c <depth> <addr>...`, depth 0 = only the function, 1 = also its callees), `fn.sh <file> <addr>`; script list in `NOTES.md` |
| Decompiles, tables, briefs | `deform-rea-work\out\` (e.g. `out\p1\p1_all.c`, `pre64_funcs.tsv`), notes in `deform-rea-work\NOTES.md` |
| REA (rea-agents 6.7.0, MCP server `rea`) | for interactive single-function and call-chain work; headless Ghidra for bulk sweeps and persistent names (REA on Windows is read-only) |
| Corpus | `C:\Users\alext\Projects\DEFORM_corpus` (`$DEFORM_CORPUS`), manifests in `docs/corpus/` |
| Test DBs | `C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB`, `...\Inc718_D420_GFM_V_DIES_Angle2_18\...DB` (5 GB) |
| Port test runs | `~\.deform-port-runs` (failed runs are kept and grow large) |
| Archive (everything, at each gate) | NAS `\\forgelabNAS1\home\PROGRAMMING\DEFORM_REA\` (its own `AGENTS.md` indexes it); restore steps in `docs/RE-RECOVERY.md` |

## 9. Gotchas that cost hours before

* A `__has_include` switch on a header that was removed or renamed silently compiles the
  `DEF_NOT_PORTED` fallback. After a header appears or disappears, force a recompile (Ninja does
  not track it) and run `stub_sweep.py`.
* Multi-case oracle tests must reset process globals per case (`reset_globals()` per module,
  unit globals, `g_conv.english = 0`): the original ran each case in a fresh process.
* Construct model objects in zeroed memory: the original's are static, and some constructors
  leave fields unset (`UNIT` at +0xfc).
* `testkit` `CHECK_EQ` binds references: store a returned vector before indexing it.
* Python or shell heredocs that contain `\n` or `\x..` escapes can corrupt C++ string literals
  or truncate files; edit C++ with an editor tool.
* Hard links share the read-only attribute with their target; clearing it on a link clears it
  on the staged oracle copy too.
* Corpus KEY files keep CRLF (`.gitattributes -text`); some SIMNAM lines start with a lone CR.
* Worker build folders `port\build_<x>` are not ignored in worktrees; exclude them before
  `git add -A`. Parallel workers each need their own build folder and Ghidra copy.
* Never pass oracle cache paths to a shell (`start`, `cmd /c`): Windows opened "no app
  associated" dialogs on Sim2. Spawn programs directly.

## 10. Skills and where to start

Skills live in `.claude/skills/<name>/SKILL.md` (plain Markdown, usable by any agent):

| Skill | Use it to |
|---|---|
| `port-function` | recover one binary function into the port, faithful mode, with tag, status row and test |
| `record-oracle` | add a `tests/commands` case and record the original's output in the cache |
| `fold-check` | fold status files, check `@src`/`@data`, ownership and stubs |
| `gate` | run the full gate checklist before merging to `main` or tagging a gate |
| `profile` | measure wall time and peak memory of a build on an input, before/after a change |
| `swap-module` | replace a module or kernel with another implementation in evolve mode |

| Task | Start here |
|---|---|
| Fix a bug in ported code | reproduce with the oracle case, read the module card, find the binary function by its `@src`, fix in faithful mode, add the case to a test |
| Recover a new program (solver harness, DEF_GUI_MO) | its plan in `docs/`, then `docs/RE-RECOVERY.md` for the method, `docs/port-conventions.md` for the code rules |
| Optimise | `profile` skill, evolve mode (section 2), record the baseline in the module card |
| Swap a module for open source | `swap-module` skill, the module card's swap notes |
| New API | wrap `defcmd`/`defdbio`/`deformdb` from a new target; `deformdb/README.md` shows the current API style |
| Write a paper or report | numbers only from committed results (`docs/re/int/`, gate results here, ledger counts from `fold_status.py --dry-run`), method from `docs/RE-RECOVERY.md` |
| Hand over to another tool | this file is the entry point; module cards and skills are plain Markdown |

Further reading: `docs/port-conventions.md` (code rules), `docs/oracle-runner.md`,
`harness/corpus/README.md`, `harness/defcmp/README.md`, `docs/re/model-layout.md`,
`docs/re/commands/*.md` (per-command specs), `docs/re/keywords/` (keyword specs),
`docs/storage-plan.md`.
