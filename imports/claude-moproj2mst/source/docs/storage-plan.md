# Storage plan: NAS archive, local layout, monorepo

Status: **proposal, waiting for Aleksandr's approval** (2026-10-22). Nothing has been copied, moved or deleted.
Inventory taken on Sim2 on 2026-10-22. Sizes are on-disk bytes (GiB, 1 GB = 2^30 bytes) from a full directory walk.

Rules this plan keeps:

* DEFORM binaries (anything from `C:\Program Files\SFTC\DEFORM\v11.0.2`, and SFTC's relink libraries) are recorded by
  SHA-256 and path only, never copied to the NAS. The exporter refuses any file whose hash is in the install manifest.
* Everything stays private: the NAS share and the private GitHub repo only. Nothing goes to Git LFS on GitHub.
* Originals are never deleted. Local copies are removed only after their NAS export is verified **and** Aleksandr approves.
* Live Ghidra projects and builds stay on the local disk.

## 1. NAS check

`\\forgelabNAS1\home\PROGRAMMING\DEFORM_REA` from Sim2: folder exists (empty), listing works, a write probe was
written and removed, **30,406 GB free of 42,888 GB**. The local disk C: has 3.05 TiB free, so moving things to the NAS
is about safety and order, not space.

## 2. Inventory

The project created **355 folders, 204 GB** on Sim2; every one is listed with size, purpose and class in
[storage-inventory.md](storage-inventory.md). About 80 GB of it is evidence worth keeping; the rest is build output,
staging copies and leftovers from test runs. The table below groups them and adds the inputs the project only reads.

| What | Path | Size | Class |
|---|---|---:|---|
| Port repo, git objects | `Projects\claude-deform-rea\.git` | 35 MB | **keep**. GitHub has all of it except 11 local-only branches (`census-merge` +31, `p8-02-int-local` +12, and 9 small ones) |
| Worktrees (124 folders) | `Projects\claude-deform-rea-*`, `.claude\worktrees\*` | 14.8 GB | sources are in git; 13.7 GB of it is `build*` output (**scratch**) |
| Stray untracked files in the main checkout | `claude-deform-rea\{CMakeLists.txt,cmake,src,include,data,fp.exe,ghidra-vs-rea-comparison.md}` | < 5 MB | **keep** the report; the rest predates `port/` and must be sorted before the monorepo step |
| Ghidra master project (DEF_PRE_64 + DEF_DBM) | `deform-rea-work\ghidra` | 0.21 GB | **evidence**, live |
| Ghidra worker copies of the same two programs (133) | `deform-rea-work\ghidra_*` | 29.2 GB | names applied by workers are **evidence**; the rest duplicates the master |
| Ghidra project of DEF_GUI23 | `deform-rea-work\ghidra_gui` | 0.13 GB | **evidence**, live (GUI) |
| GUI MO research (Ghidra project, xref pickles, notes) | `deform-rea-work\gui_mo` | 0.50 GB | **evidence**, live; holds 2 DEFORM exes (excluded) |
| Decompiles, listings, tables, briefs | `deform-rea-work\out` (without `p89\capcache`) | 0.7 GB | **evidence** |
| Phase 8-9 capture cache (DEFORM outputs) | `deform-rea-work\out\p89\capcache` | 11 GB | **evidence** (oracle data, regenerable only with licensed DEFORM) |
| Headless scripts and notes | `deform-rea-work\scripts*`, `NOTES.md` | 14 MB | **evidence** |
| Solver relink spikes | `deform-rea-work\simrelink`, `simrelink2d` | 0.19 GB | recipes (`.bat`, `.amake`) are **evidence**; SFTC `.lib` and relinked `.exe` are excluded |
| DB scratch copies | `deform-rea-work\p112_dbcopies`, `scratch_build` | 4.9 GB | **scratch** |
| Oracle cache, entries | `%USERPROFILE%\.deform-oracle\entries` | 50.3 GB | **evidence** (1,907 cases; refill needs licensed DEFORM and hours of runs) |
| Oracle cache, `tmp` + `stage` | `.deform-oracle\tmp`, `stage` | 19.8 GB | **scratch** |
| Port run dirs (failing runs kept) | `%USERPROFILE%\.deform-port-runs` | 56.6 GB | **scratch** (0.24 GB `entries` is a regenerable cache) |
| kw-g1a corpus copy | `%USERPROFILE%\.kwg1a-corpus` | 1.2 GB | **scratch** (duplicate of corpus files) |
| Test corpus (hash-checked copies) | `Projects\DEFORM_corpus` | 5.9 GB | **keep** (KEY/KWD/CMD from the install examples + 2 DBs) |
| Corpus originals | `C:\PROBLEM\TEST_REA` | 4.7 GB | **keep**; DBs are byte-identical to the corpus copies, stored once |
| REA databases | `%TEMP%\rea-ghidra-*` | 0.3 GB | **scratch**: REA on Windows is ephemeral and read-only; its findings are already in `out\` and `docs\re\` |
| Test leftovers in Temp | `%TEMP%\dbread_*`, `*_cmdoracle_*`, `gendb_e2e_*`, `p112_db_*` | about 7 GB | **scratch** |
| Codex attempt (separate repo `coffeejuice/codex-defom-rea`) | `Projects\codex-deform-rea` | 18.9 GB | out of scope; listed so it is not forgotten |
| Not part of the recovery | `DEFORM_examples` (16 GB), `Inc718_*`, `C:\PROBLEM\{Inc718_*,COGGING_*,DMO_*}` | | left alone |

Binaries inside the work folders that the exporter must skip: 286 `.exe/.dll/.lib` files (0.33 GB), mostly in
`scratch_build`, `simrelink*`, `scripts_p67*` and `gui_mo`.

### Tools and versions

| Tool | Version | Location | On the NAS |
|---|---|---|---|
| Ghidra | 12.1.4 PUBLIC | `C:\tools\ghidra_12.1.4_PUBLIC` (0.84 GB) | installer zip |
| JDK | Temurin 21.0.12.1 | `C:\tools\jdk-21` | installer zip |
| REA | rea-agents 6.7.0 (npm) | `%APPDATA%\npm\node_modules\rea-agents` | `npm pack` tarball |
| MSVC | VS 2022 Build Tools, toolset 14.44.35207 | `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools` | version only (licensed) |
| Absoft Pro Fortran | 11.1 | `C:\Absoft11.1` | version only (licensed) |
| CMake / Ninja | 4.4.3 / in `C:\tools\Ninja` | | version only |
| Python | 3.12.10 (also 3.14, 2.7) | | version + `requirements.txt` |
| Node / git / git-lfs / 7-Zip | 24.19.0 / 2.56.0 / 3.8.0 / 26.04 | | version only |
| DEFORM | v11.0.2, 16,424 files, 4.98 GB, 527 exe/dll | `C:\Program Files\SFTC\DEFORM\v11.0.2` | **SHA-256 manifest only** |

## 3. NAS layout

```
\\forgelabNAS1\home\PROGRAMMING\DEFORM_REA\
  AGENTS.md                         index: what is where, the rules above, how to restore
  repos\
    claude-deform-rea.git           bare mirror of the port repo (all branches and tags)
  evidence\
    def_pre_64\<gate>\              one folder per gate, e.g. 2026-10-22_int-all_d7819e3
      MANIFEST.sha256               every file inside every archive + the archives themselves
      gate.json                     repo commit, tool versions, DEFORM install manifest hash, who approved
      ghidra-annotations.7z         symbols, signatures, comments, types (no program bytes; see 6.1)
      ghidra-projects.7z            only if Aleksandr chooses .gzf (see 6.1)
      out.7z                        decompiles, listings, tables
      scripts.7z                    headless scripts, briefs, NOTES.md, relink recipes
    def_gui_mo_64\<gate>\  def_sim_64\<gate>\  ...
  oracle\
    def_pre_64\entries-<shard>.7z   .deform-oracle\entries, one archive per first hex digit (16 archives)
    def_pre_64\p89-capcache.7z
    def_pre_64\MANIFEST.sha256
  corpus\
    DEFORM_corpus\                  plain files (already hash-listed in docs/corpus/manifest.tsv)
    TEST_REA\                       non-DB files; DBs point to the corpus copies by hash
  tools\
    ghidra_12.1.4_PUBLIC.zip  jdk-21.zip  rea-agents-6.7.0.tgz  versions.md
  deform-install\
    v11.0.2.sha256                  path + hash + size of every install file (no files)
  releases\<app>\<ver>\             our own built port binaries + MANIFEST.sha256
  docs\                             PDF/HTML snapshots of plans and specs at each gate
```

Estimated NAS use: about 70 GB raw, about 15-20 GB as 7z. A test on one oracle shard
(`entries\0a`, 403 MB) compressed to 78 MB (19 %) in 47 s with `7z -mx=5`.

## 4. Local layout

```
C:\Users\alext\Projects\
  claude-deform-rea\                working clone (main checkout); remotes: origin (GitHub), nas (bare mirror)
  claude-deform-rea-wt\<name>\      new worktrees go here instead of 124 sibling folders
  deform-rea-work\                  live Ghidra: master project + copies for the app being worked on
  DEFORM_corpus\                    corpus (unchanged path; manifests point here)
C:\Users\alext\.deform-oracle\      oracle cache (kept local: tier 3 reads it for every case)
```

I recommend **keeping the folder name `claude-deform-rea`**. Renaming it to `deform-rea` would move Claude's project
memory (it is keyed by folder path), the folder approved for remote sessions, and the absolute paths written in
docs and test cases. The rename buys nothing the monorepo step does not.

### Two states per app

| | Live app (now: DEF_GUI_MO_64, DEF_SIM_64) | Finished app (now: DEF_PRE_64) |
|---|---|---|
| Ghidra on C: | master + worker copies, used daily | removed after export, verify and approval |
| NAS | snapshot at each gate | final snapshot, the only copy of the Ghidra work |
| Oracle cache | local + NAS archive at each gate | local while its tests run; NAS archive is the backup |
| Restore | not needed | `tools/nas/restore.py def_pre_64 <gate>` re-imports into a fresh Ghidra project from the licensed install and re-applies the annotations |

Disk space freed on C: when DEF_PRE_64 moves to "finished":

| Item | Freed |
|---|---:|
| 133 worker Ghidra copies + master | 29.4 GB |
| `out\p89\capcache` (NAS copy kept) | 11 GB |
| `p112_dbcopies`, `scratch_build` | 4.9 GB |
| `out\` text and `scripts*` | 0.7 GB |
| **Total for DEF_PRE_64** | **about 46 GB** |

Scratch that can be cleared at any time, independent of the NAS (also only on approval): `.deform-port-runs\tmp`
56.4 GB, `.deform-oracle\tmp` + `stage` 19.8 GB, worktree builds of merged branches up to 13.7 GB, Temp leftovers
about 7 GB, `.kwg1a-corpus` 1.2 GB: **about 98 GB**.

## 5. Source to destination

| Source | Destination | Raw | Method |
|---|---|---:|---|
| port repo, all refs incl. 11 local-only branches | `repos\claude-deform-rea.git` | 35 MB | `git push nas` (6.2) |
| `deform-rea-work\ghidra`, `ghidra_*` | `evidence\def_pre_64\<gate>\ghidra-annotations.7z` | 29.4 GB in, about 50 MB out | headless export script (6.1) |
| `deform-rea-work\ghidra_gui`, `gui_mo` | `evidence\def_gui_mo_64\<gate>\` | 0.63 GB | same, as a live snapshot |
| `deform-rea-work\out` (no capcache) | `evidence\def_pre_64\<gate>\out.7z` | 0.7 GB | 7z |
| `deform-rea-work\scripts*`, `NOTES.md`, relink recipes | `evidence\def_pre_64\<gate>\scripts.7z` (recipes under `def_sim_64`) | 14 MB | 7z |
| `.deform-oracle\entries` | `oracle\def_pre_64\entries-<0..f>.7z` | 50.3 GB | 7z, 16 shards |
| `out\p89\capcache` | `oracle\def_pre_64\p89-capcache.7z` | 11 GB | 7z |
| `DEFORM_corpus` | `corpus\DEFORM_corpus\` | 5.9 GB | robocopy + hash check against `docs/corpus/manifest.tsv` |
| `C:\PROBLEM\TEST_REA` non-DB files | `corpus\TEST_REA\` | about 30 MB | robocopy + SHA-256 |
| Ghidra, JDK, REA installers | `tools\` | about 1.2 GB | copy + versions.md |
| DEFORM install | `deform-install\v11.0.2.sha256` | 16,424 lines | hash only |

Not copied: scratch rows from section 2, SFTC binaries, `codex-deform-rea`, the projects that are not part of the recovery.

## 6. Sync method

### 6.1 Ghidra: annotations, not program bytes

A Ghidra project (and a `.gzf` export) **contains the full bytes of the DEFORM executable** it analyses. Copying it to
the NAS copies DEFORM itself, which the first rule forbids. So the default export is a headless script
(`ExportAnnotations.java`, to be written) that writes, per program: function entries, names, signatures, plate and
EOL comments, data types, labels and bookmarks, as JSON. It runs on the master and on every worker copy, and keeps only
names that differ from the master, so the 133 copies shrink to one merged annotation set. Restore imports the
program again from the licensed install (checked against `deform-install\v11.0.2.sha256`), runs auto-analysis with the
recorded options, and re-applies the JSON.

If Aleksandr prefers exact `.gzf` snapshots on the NAS anyway (the NAS is private), the exporter adds
`ghidra-projects.7z` with the master `.gzf` only. That is a decision for him (question 2 at the end).

### 6.2 Git

* `git init --bare \\forgelabNAS1\...\repos\claude-deform-rea.git`, then `git remote add nas <that path>`.
* Sync: `git push nas "refs/heads/*:refs/heads/*" "refs/tags/*:refs/tags/*"`, **without** `--mirror` or `--prune`, so a
  branch deleted locally is never deleted on the NAS.
* When: at every gate (part of the gate checklist) and by a daily scheduled task on Sim2.
* Gates get an annotated tag (`gate/def_pre_64/2026-10-22`) so evidence folders and commits point at each other.

### 6.3 Evidence export at gates

`tools/nas/export_evidence.py <app> <gate>` (to be written):

1. Builds or refreshes `deform-install\v11.0.2.sha256` and refuses any file whose hash is in it, plus every
   `.exe/.dll/.lib/.obj/.pdb` from SFTC folders.
2. Packs each source into 7z (LZMA2, `-mx=5`, multithreaded) in a local staging folder.
3. Writes `MANIFEST.sha256` (per file inside each archive, and per archive) and `gate.json`.
4. Copies to the NAS with `robocopy /Z /J`, then re-hashes the archives on the NAS and runs `7z t` on them.
5. Commits the small manifest to the repo as `docs/evidence/<app>/<gate>.sha256`, so history records exactly which
   NAS archive belongs to which commit.
6. Never deletes anything. Removal of local copies is a separate command that prints what it would remove and the
   matching verified manifest lines, and runs only with Aleksandr's approval.

### 6.4 Git LFS or 7z archives

**Recommendation: 7z archives on the NAS with SHA-256 manifests; no Git LFS.**

| | Git LFS | 7z + manifest on NAS |
|---|---|---|
| Privacy | GitHub LFS would put DEFORM outputs on GitHub storage | stays on the private NAS |
| Server | needs an LFS server; a bare repo on SMB has none (only unmaintained folder-store agents) | plain files |
| Fit | built for files that change with each commit | evidence is written once per gate |
| Size | 70 GB of oracle data in LFS, re-downloaded by every clone | about 15-20 GB compressed, fetched only when needed |
| Integrity | per object | per file and per archive, checked after copy |
| Link to history | pointer files in git | manifest committed in git |

## 7. Monorepo

Today `main` has `port\` (libraries `defrt defkey defmodel defdbio defcmd testkit` and the app `def_pre`), `deformdb\`,
`harness\`, `tools\`, `tests\`, `docs\`. The GUI MO plan already counts on reusing the DEF_PRE engine libraries, so
they belong in a shared place.

Target:

```
CMakeLists.txt          superbuild (replaces port\CMakeLists.txt)
build.cmd               replaces port\build.cmd
libs\                   defrt defkey defmodel defdbio defcmd testkit deformdb
apps\def_pre\           today's port\def_pre and its app-level tests
apps\def_gui_mo\        new (plan docs/plan-gui-mo-reconstruction.md)
apps\def_sim_64\        new (plan docs/port-plan-solver-harness.md)
harness\  tools\  tests\  docs\   shared, unchanged
```

Steps, without breaking history or the build:

1. **Quiet point.** Do it right after a gate, when no packet branch is open on `port\`. Tag `pre-monorepo`.
2. **Sort the stray untracked files** in the main checkout (a root `CMakeLists.txt`, `cmake\`, `src\`, `include\`, `data\`
   from 2026-10-20). They would collide with the new root `CMakeLists.txt`. Proposal: move them to the NAS under
   `evidence\early\` and commit `ghidra-vs-rea-comparison.md` to `docs\re\`.
3. **Commit A: pure renames.** Only `git mv`, no content changes, so git sees 100 % renames: `git log --follow`,
   `git blame` and merges across the move keep working.
4. **Commit B: path fixes.** CMake, `build.cmd`, and the 67 files that name `port/` (harness, tools, tests, docs). Keep
   a one-line `port\build.cmd` that calls the new root `build.cmd` for one phase, then remove it.
5. **Prove nothing changed:** tier 1 216/216, tier 3 1,907/1,907, e2e same 518/3/3 as before, `fold_status --check`
   0 errors, and the built `def_pre` binary compared with the one from `pre-monorepo`.
6. **Branches still open** merge across the move with git's rename detection (`merge.renameLimit` raised); branches
   that are done get merged or closed before step 1.
7. New apps start directly in `apps\<app>\` and depend on `libs\` through CMake targets.

## 8. What I need from Aleksandr

1. Approve this plan (NAS layout, local layout, sync method, monorepo steps)?
2. Ghidra on the NAS: **annotations only** (recommended, no DEFORM bytes leave the install) or also `.gzf` snapshots?

After approval the order is: install manifest, git mirror, corpus and tools, oracle archives, Ghidra annotation
exporter, DEF_PRE_64 gate export with verification, then the list of local copies to remove for a separate yes.
