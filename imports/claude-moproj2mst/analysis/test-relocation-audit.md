# Converter test relocation

The converter suite now receives explicit imported fixtures through CMake cache
settings. Test writes use separate disposable directories beneath the takeover's
`build/` directory. No behavioral library source or C ABI changed in this package.
Assertion values and accepted result codes were preserved.

## Configuration and execution

Reconfigure the existing isolated `build/baseline` project. New defaults resolve
from this checkout rather than an upstream worktree:

| Setting | Default relative to takeover root |
| --- | --- |
| `M2M_CORPUS` | `evidence/gui_mo/s1/corpus` |
| `M2M_ORACLE_B` | `evidence/gui_mo/oracle_b` |
| `M2M_DB_FIXTURE` | `evidence/test-rea/COG_P1_B1` |
| `M2M_REFERENCE_EXE` | `evidence/gui_mo/DEF_GUI_MO_64.exe` (PE reads only) |
| `M2M_TEST_SCRATCH` | `build/baseline/t` |

After the coordinator configures and builds, run from this takeover root:

```text
python -B admin/run_converter_tests.py --build build/baseline --inventory-only
python -B admin/run_converter_tests.py --build build/baseline --jobs 4
```

The launcher validates registered CTest fixture/output paths, retains inventory,
logs, JUnit and fixture manifests under a unique `build/test-runs/` directory,
and writes its compact scientific classification to
`analysis/test-relocation-results.json`. It records failing executables, logged
case skips/refusals, and diagnostic folder differences separately. Fixture byte,
size and modification-time changes make the launch fail. It does not configure,
build, repair expectations or launch the original application.

## Executable path audit

All CTest targets with the `m2m.` prefix receive explicit fixture settings,
`M2M_CLI`, a separate working directory and `TEMP`/`TMP`/`TMPDIR`. This also covers
release tests added by the release CMake fragment. C++ tests previously using
`temp_directory_path()` now require `M2M_TEST_SCRATCH`; Win32 `GetTempPathW` tests
receive the same directory through TEMP/TMP. Optional diagnostic dump roots and
each emitted dump file are checked against that scratch directory. Full folder
Save tests already copy inputs before writing; the sweep is explicitly enabled.

The test-owned Python corpus wrappers pass the configured corpus explicitly.
The keyword-table test passes the imported executable with `--exe --check` and
never executes it. Shared Python tools outside this package retain their old
standalone defaults; these test entry points override them. CMake Python commands
use `-B -I`, test wrappers disable bytecode before local imports, and the P11
generator subprocess uses `-B -I`. Both fixture generators require an output path
inside explicit scratch. P11's generated default output and comparison failure
files use local scratch. Tier-2/tier-3 custom targets also receive explicit local
scratch and retain project copies.

Remaining Windows path literals in unit tests are synthetic path-format,
missing-input or invalid-flag cases. The C:/Windows current-directory case only
observes path resolution. They are not original project input fallbacks. Comments
retain historical provenance paths. The installed `PACK/pack.dat` probe has been
removed: the optional check now requires `M2M_INSTALL_FIXTURE`; synthetic local
PACK checks still run with identical assertions.

## Adequacy and unresolved gaps

- CTest success is an executable result, not a case coverage denominator. Testkit
  treats a returned case or caught NotPorted as success. Several tests report
  SKIP text; some historical early-return branches report nothing. The runner
  preserves evidence and classifies visible gaps without claiming to detect
  every silent branch.
- `g0.test_abi.cpp` still permits either OK or UNSUPPORTED for its real project.
  This inherited assertion is preserved and explicitly qualified. It is not
  successful-conversion ABI coverage.
- `s.test_save_folder.cpp` contains no failure assertions for recorded folder
  differences or refusals. Its SAME/DIFF/REFUSED output is diagnostic. The runner
  separately marks logged differences, even when CTest reports success. Its
  comparator still masks declared view/banner profiles and relocates reference
  paths. Diagnostic SAME does not establish raw-byte identity.
- Full Save sweep cases requiring GUI interactions or nondefault Setup Type No
  remain excluded by inherited case selection and are logged as notes. Tests of
  internal nondefault branches remain useful unit checks, not lifecycle scope
  acceptance cases.
- Historical MultiBlow, view state and other Oracle-B expectations retain their
  recorded session qualifications. Fresh one-project Open→Save→Close acceptance,
  failure write order, cleanup/state isolation and complete ABI success coverage
  remain separate research work.
- Optional external deflate vectors are not acquired/configured. Their absence
  is now reported explicitly. No imported installation PACK fixture was found;
  its optional check is explicitly skipped. These skips do not remove the
  existing synthetic or corpus assertions.
- The generator oracles are historical Python reconstructions, not independent
  fresh original observations. Source fixtures and imported expectation files
  have not been changed to make the converter pass.

The worker checked syntax of all nine Python files without importing them or
writing bytecode. Actual configure/build/test execution belongs to the
coordinator. Exact package files are listed in `test-relocation-changes.json`.
Wall time and token savings have not been measured; token usage is unavailable.
