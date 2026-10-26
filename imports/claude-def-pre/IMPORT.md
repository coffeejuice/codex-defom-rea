# Imported DEF_PRE C++ project

This is a byte-preserved copy of the portable project files from
`C:\Users\alext\Projects\claude-deform-rea`, imported on 2026-10-23.
The source directory was read only. Its Git HEAD at import was
`19f4aaa17a44ec670932bd6c7205c63ef7c7d67d`; the manifest hashes the actual
working files, including any differences from that commit.

The import contains 2,560 files (17,211,771 bytes): the DEF_PRE port, its runtime,
model, KEY/DB and command libraries, the independent DB reader/writer, comparator,
tests, fixtures, tools, and documentation. The complete per-file SHA-256 manifest
is `../../analysis/claude-def-pre-import-manifest.json`.
Git metadata, Claude configuration, generated build directories and root scratch
artifacts were excluded. The source agent guidance is preserved under
`provenance/source-AGENTS.md` and `provenance/source-CLAUDE.md` as historical
documentation; it does not replace this repository's instructions.

## Build the imported application

From the containing Codex project:

```powershell
pwsh -NoProfile -File tools\build_claude_def_pre.ps1
```

The wrapper builds with Visual Studio 2022 x64 and runs the imported tier-1 tests.
Logs and all generated build files stay under `build\claude-def-pre` in this
repository. The executable is `build\claude-def-pre\Release\DEF_PRE_64.exe`.
The Codex integration sets the output filename; the logical CMake target remains
`def_pre`. This naming change is separate from the byte-preserved import manifest.
The original executable is not required for compilation. Floating-point and
assembly fidelity flags remain those of the copied project.

Every executable build now allocates a new semantic version through the CMake
`def_pre_version` dependency. `tools/bump_def_pre_version.ps1` follows DMO's
highest-worktree/local-branch/remote-tracking-branch patch allocation method.
The source is `src/app/Version.h.in` in the containing checkout. The generated
header drives `--port-version` and Windows file/product version resources.
The build wrapper deploys each successful build to all three installed paths;
UAC approval may be required. Reports must state the verified instance version.
Deployment receipts record version and hash; originals remain `.original` backups.

Manual configuration is also supported:

```powershell
cmake -S imports/claude-def-pre/port -B build/claude-def-pre -G "Visual Studio 17 2022" -A x64 -DDEF_PRE_PORT_OFFLINE=ON
cmake --build build/claude-def-pre --config Release --parallel 4 -- /nr:false
ctest --test-dir build/claude-def-pre -C Release -L tier1 --output-on-failure --no-tests=error -j 4
```

The application superbuild is `port/CMakeLists.txt`. The historical root
`CMakeLists.txt` belongs to an earlier keyword-library project and is preserved
for provenance, not used as the DEF_PRE application entry point.

## Scope and verification

The source project marks its DEF_PRE batch-mode recovery finished and also
contains a text menu shell and editing modules. Importing it does not by itself
independently establish complete equivalence for every original workflow.
Import-specific validation is recorded in
`../../analysis/claude-def-pre-import-validation.json`.

The imported corpus harness registers 26 tier-2 and 1,907 tier-3 cases. Those
comparisons additionally need the external corpus and cached original results;
they are not bundled into this source import. Configuration uses
`DEF_PRE_PORT_OFFLINE=ON` so corpus runs cannot silently launch the original.
Tier-1 tests that consult external cached results must be read according to their
own pass/skip reporting; a green CTest entry alone does not prove cache coverage.

The existing Codex loader API, sources, build and unfinished recovery candidate
remain separate. No ABI replacement or merge between the two model libraries is
implied. This preserves a usable imported application and both evidence histories.
