# RE-RECOVERY: how a DEFORM program is recovered, and how to restore the evidence

This is the method that produced the DEF_PRE_64 batch port (tag `v1.0-def-pre`), written so that
the next recovery (the solver harness, DEF_GUI_MO_64, or another DEFORM program) can follow it,
and so that a paper or a hand-over can describe it. Part B explains how to bring back the Ghidra
and REA evidence from the NAS, and records the restore drill that tested it. Rules for the code itself: `AGENTS.md` and
`docs/port-conventions.md`.

## Part A: the method

### A.1 Ground rules

* Static recovery from the binary's code. The original runs only as an **oracle** on copies of
  inputs; it is never modified, never debugged, and its licence code (Sentinel and the process
  monitor) is never analysed or ported.
* Test data (the corpus, the example DBs) checks algorithms already found in code. It is never
  used to discover behaviour, and a mismatch is traced back to the code.
* The goal of the first pass is **faithful**: the same bytes out for the same input. Improvement
  (speed, new APIs, better libraries) comes after, in evolve mode, with the faithful build as the
  baseline.

### A.2 Tools and environment

| Tool | Version, place | Use |
|---|---|---|
| Ghidra | 12.1.4, `C:\tools\ghidra_12.1.4_PUBLIC`, JDK 21 in `C:\tools\jdk-21` | master project with full analysis; headless scripts for bulk decompiles, tables, function lists, xrefs |
| REA | rea-agents 6.7.0 (npm), MCP server `rea` with the Ghidra provider | interactive per-function recovery and call chains (read-only on Windows) |
| MSVC 2022 Build Tools, Ninja, CMake, Python 3.12 | | the port and every repo tool |
| DEFORM v11.0.2 | `C:\Program Files\SFTC\DEFORM\v11.0.2`, licensed | the oracle only |

A blind comparison on three functions (2026-10-21) found REA 12-35 % faster per function with no
tool failures and headless Ghidra better for sweeps and persistent names, so each is used for
what it does best.

### A.3 Phases

1. **Survey.** Identify the program, its compilers (DEF_PRE_64: PathScale Fortran + MSVC C++,
   2014 build), source-module boundaries, entry points and the batch control flow. Write the plan
   (`docs/port-plan-def-pre-batch.md` is the model): scope, phases, risks, numbers anyone can
   reproduce.
2. **Ledger.** Export the function list (`FuncList.java`), pointer tables, vtables and
   address-taken functions (`docs/re/ghidra/IndirectRefs.java`), then compute the closure of each
   phase's entry points (`docs/re/batch_scope.py`, following direct calls, function pointers,
   tables and live vtable slots). The result, `docs/re/pre64_batch_ledger.tsv`, is the list of
   every function in scope with its phase; each row ends `ported`, `verified` or `not-needed`.
3. **Phase 0: the harness before the code.** The oracle runner (`tools/oracle`), a DB comparator
   with documented masks (`harness/defcmp`), the corpus copied and hash-listed (`docs/corpus/`),
   and the corpus runner with test tiers (`harness/corpus`). Exit: one command compares the
   original and a stub port on the whole corpus.
4. **Foundation.** Recover class layouts at their binary offsets (`docs/re/model-layout.md`),
   the runtime behaviour the code depends on (CRT printf rounding, libm, Fortran I/O), naming and
   tagging conventions, and the module tree with downward-only dependencies.
5. **Packets.** Split each phase into packets of a few hundred functions that own disjoint files
   (`docs/phase*-packets.md`). Each packet runs in its own worktree, Ghidra copy and build folder,
   writes its own status file, and calls unfinished neighbours through `ext` seams with
   `DEF_NOT_PORTED` defaults. Up to 17 packets ran in parallel.
6. **Specs where behaviour is wide.** For keyword groups and command keywords, write a spec first
   (`docs/re/keywords/`, `docs/re/commands/<CMD>.md`): every option, message and file, with the
   oracle cases that cover them (`tests/commands/<CMD>/`).
7. **Integration and gates.** Merge packets onto an integration branch, rewire seams to their
   owners (one owner row per address, the lower layer owns), and prove the gate: tier 1, tier 3,
   e2e, fold check (`.claude/skills/gate`). Tag the gate and archive the evidence to the NAS.

### A.4 Numbers for DEF_PRE_64 (at `v1.0-def-pre`)

| Item | Value |
|---|---|
| Functions in scope (ledger) | 5,257 (`python -I docs/re/fold_status.py --dry-run` prints them per phase and status) |
| Corpus cases | 1,907 (8 `-m`, 822 DB step exports, 1,029 KEY read/write, 48 command files) |
| Tier 1 / tier 3 / e2e | 216/216, 1,907/1,907, 518 pass / 3 skip / 3 known failures |
| Gates | `gate/p0`, `gate/p1`, `gate/p2-3`, `gate/p4`, `gate/p5`, `gate/p6-7` |

Per-module counts (ported functions, stubs left, tests) are in each `MODULE.md`.

### A.5 Starting the next program

1. Copy the plan structure: survey, ledger, phases, packets, risks, defaults for the user.
2. Create its master Ghidra project in `deform-rea-work` (full analysis once), and record the
   analysis options and Ghidra version in its plan.
3. Reuse the harness pattern: an oracle runner for the new program's batch path, a comparator
   for its outputs, a corpus with manifests, tier 1/2/3.
4. Reuse the libraries: the solver harness and GUI MO plans consume `defrt`, `deformdb` and the
   DEF_PRE engine libraries (`docs/storage-plan.md` section 7 moves them to `libs/`).
5. Keep the conventions: `@src` tags, layouts at offsets, one owner per address, status files,
   module cards, gates with NAS snapshots.

## Part B: restoring the evidence from the NAS

The NAS (`\\forgelabNAS1\home\PROGRAMMING\DEFORM_REA\`, "NAS" below) keeps everything at each
gate; C: is the working copy (`docs/storage-plan.md`). Its own `AGENTS.md` lists the layout. The
steps below were run end to end on 2026-10-22 (section B.7). Never unpack over a live project that
has unsaved work: close Ghidra and check for lock files (`*.lock`, `*.lock~`) first.

### B.1 What is where

| NAS path | Content | Unpack to |
|---|---|---|
| `evidence\def_pre_64\2026-10-22_v1.0-def-pre\ghidra-gzf\` | `DEF_PRE_64.exe.gzf`, `DEF_DBM.exe.gzf` and one `<program>.annotations.json` each, from the master project | open directly (B.3) |
| `...\ghidra-projects.7z` | every Ghidra project folder of the app: master `ghidra\` and 133 worker copies `ghidra_<packet>\` (28 GB unpacked) | `C:\Users\alext\Projects\deform-rea-work` |
| `...\ghidra-annotations.7z` | `ghidra_<x>\<program>.annotations.json` for every worker copy (names that exist only there) | anywhere, text |
| `...\out.7z`, `scripts.7z` | decompiles, listings, tables, briefs; headless scripts and `NOTES.md` | `C:\Users\alext\Projects\deform-rea-work` |
| `...\rea.7z` | REA's Ghidra projects (`rea-ghidra-<hash>\`) | `%TEMP%` |
| `oracle\def_pre_64\entries-<0..f>.7z`, `p89-capcache.7z` | the oracle cache entries (1,907 corpus cases and the command cases), split by the first hex digit of the key; the Phase 8-9 capture cache | `%USERPROFILE%\.deform-oracle\entries`; `deform-rea-work\out\p89\capcache` |
| `repos\claude-deform-rea.git`, `repos\*.bundle` | bare mirror (all branches and tags, `archive/*`, `gate/*`, `v1.0-def-pre`); bundle from before the branch cleanup | `git clone` |
| `corpus\DEFORM_corpus\`, `corpus\TEST_REA\` | test corpus, plain files | `C:\Users\alext\Projects\`, `C:\PROBLEM\` |
| `deform-install\v11.0.2.7z` | the DEFORM install as on Sim2 (running it still needs the licence) | `C:\Program Files\SFTC\DEFORM\v11.0.2` |
| `tools\*.7z`, `versions.md` | Ghidra 12.1.4, JDK 21, rea-agents 6.7.0 | `C:\tools`; REA: `%APPDATA%\npm\node_modules` |
| `context\claude-memory-<date>.7z`, `claude-sessions-<date>.7z` | Claude project memory and session transcripts | memory: `%USERPROFILE%\.claude\projects\C--Users-alext-Projects-claude-deform-rea\memory` |

Every `X.7z` has two side files: `X.7z.sha256` (the archive) and `X.7z.files.sha256`
(`sha256  size  path` of every file inside, paths relative to the unpack folder). Copied folders
have `<folder>.files.sha256`.

### B.2 Copy and verify before unpacking

Work on a local copy; never verify on the NAS itself (`nas_archive.py verify` rewrites the
archive's `.sha256` side file).

```
robocopy \\forgelabNAS1\home\PROGRAMMING\DEFORM_REA\evidence\def_pre_64\2026-10-22_v1.0-def-pre C:\restore\gate /E /Z /J
python -I tools\nas\nas_archive.py verify C:\restore\gate\ghidra-projects.7z    :: 7z t + archive SHA-256
python -I tools\nas\nas_archive.py verify C:\restore\gate\ghidra-gzf            :: folder against ghidra-gzf.files.sha256
```

After unpacking a whole archive into a folder `D`, copy `X.7z.files.sha256` to `D.files.sha256`
and run `nas_archive.py verify D` to check every unpacked file. Archives drop empty folders (for
example an oracle entry's empty `files\`); nothing reads them, so this is not a difference.

### B.3 Ghidra: the fastest way back (`.gzf`)

A `.gzf` is one program with its analysis, packed. It opens in Ghidra 12.1.4 without re-import
or re-analysis:

* GUI: open any project, File > Import File..., pick the `.gzf`.
* Headless, into a new project, running a script straight away:

  ```
  set JAVA_HOME=C:\tools\jdk-21
  C:\tools\ghidra_12.1.4_PUBLIC\support\analyzeHeadless.bat C:\restore\gzfproj drill ^
      -import C:\restore\gate\ghidra-gzf\DEF_PRE_64.exe.gzf -noanalysis ^
      -scriptPath C:\Users\alext\Projects\deform-rea-work\scripts -postScript Decomp.java C:\restore\f.c 0 0x1402f76c0
  ```

The master project's names are mostly Ghidra's defaults; the names recovered by the packets live
in the worker copies and in `ghidra-annotations.7z` (B.5).

### B.4 Ghidra: the full projects

```
7z x C:\restore\gate\ghidra-projects.7z -oC:\Users\alext\Projects\deform-rea-work            :: all 134 projects
7z x C:\restore\gate\ghidra-projects.7z -oC:\Users\alext\Projects\deform-rea-work "ghidra\*" :: only the master (225 MB)
```

The master for DEF_PRE_64 and DEF_DBM is `deform-rea-work\ghidra\deform.gpr`; the headless
scripts work on it again (`GPROJ=ghidra bash deform-rea-work/scripts/run.sh DEF_PRE_64.exe
Decomp.java out/x.c 0 0x1402f76c0`). Unpack `out.7z` and `scripts.7z` into `deform-rea-work\` too
for the decompiles, briefs and `NOTES.md` that the specs cite.

### B.5 Ghidra: from text

`<program>.annotations.json` (written by `tools/nas/ghidra/NasExport.java`) holds the program's
SHA-256, image base, functions, non-default symbols, comments, bookmarks and data type names
(DEF_PRE_64 master: 15,876 functions, 2,545 comments). It is diffable between gates and outlives
the project format. If a future Ghidra cannot open these projects, import `DEF_PRE_64.exe` again
from the install (its SHA-256 must equal the JSON's `sha256`), analyse it, and re-apply the
names and comments from the JSON. There is no import script yet; write one beside
`NasExport.java` when it is first needed.

### B.6 REA, the oracle cache, the corpus, the repo, the context

* REA: unpack `rea.7z` into `%TEMP%`; each `rea-ghidra-<hash>\` is the Ghidra project REA built
  for one binary. REA can rebuild them from the binary, so they only save analysis time. REA
  itself: unpack `tools\rea-agents-6.7.0.7z` into `%APPDATA%\npm\node_modules` (paths and the
  MCP registration: `tools\versions.md`).
* Oracle: unpack every `entries-<x>.7z` into `%USERPROFILE%\.deform-oracle\entries\` (never under
  `%LOCALAPPDATA%`, whose writes the Claude app redirects). Tests then run without DEFORM:
  `-DDEF_PRE_PORT_OFFLINE=ON`, or `--offline` for the oracle runner. Without the DEFORM install
  the runner also needs `builds\<sha256>.json` in the cache; it is written on any machine that
  ran the original once (copy it from Sim2's cache if needed).
* Corpus: copy `corpus\DEFORM_corpus\` back and check it against `docs/corpus/manifest.tsv`.
* Repo: `git clone \\forgelabNAS1\home\PROGRAMMING\DEFORM_REA\repos\claude-deform-rea.git`, or
  `git fetch nas` in an existing clone. Keep the folder name `claude-deform-rea`: Claude's project
  memory is keyed to it.
* Context: unpack `context\claude-memory-<date>.7z` into the memory folder above.

### B.7 Restore drill (2026-10-22)

Run in `C:\Users\alext\restore-drill`, nothing written on the NAS or in the live folders:

| Check | Result |
|---|---|
| Copy `ghidra-gzf\`, `ghidra-projects.7z`, `ghidra-annotations.7z`, `out.7z`, `scripts.7z`, `rea.7z`, `entries-0.7z` (4.5 GB) | ok |
| `nas_archive.py verify` on each (7z t, archive SHA-256, `.gzf` folder manifest) | all ok |
| Unpacked files against their `.files.sha256` (master project 12/12, scripts 250/250, oracle shard 1,341/1,341) | all match |
| `.gzf` imported headless into a new project, `Decomp.java` on `0x1402f76c0` | 5 s; output identical to the live master |
| Restored master project opened read-only, same script | identical to the live master |
| Annotations JSON `sha256` vs the installed `DEF_PRE_64.exe` | equal (`f78a2a5c...`) |
| Oracle: every recorded `tests/commands` case whose key is in shard 0 (35), looked up offline in the restored cache | 35/35 found, contents identical (3 lack only an empty `files\` folder) |
| `git clone` of the NAS mirror | `main` = `origin/main`, `v1.0-def-pre` = `d7819e3`, 195 `archive/*` tags |

Not exercised: the full 28 GB project unpack, REA re-opening a restored database, the
DEFORM-install and tools archives, and a port build running tier 3 against a fully restored
cache.
