# Initial local baseline

The selected imported commit is bb869387a85a89193f3fdd78b69442a6ea567866.
The first build uses its unchanged C++ and CMake sources, MSVC x64, Release,
Ninja and four build jobs. `admin/build_baseline.py` sets M2M_CORPUS explicitly
to the imported corpus and writes only beneath this takeover's build directory.
The parent DMO application is not built. Configure/build logs and artifact hashes
are retained by `analysis/baseline-build.json`.

An initial launcher attempt failed in Windows command-line quoting before CMake
ran; the launcher was corrected. This retry is not included in the successful
build invocation's elapsed time. Timings are observations, not efficiency gains.
Token usage has not been measured for this checkpoint.

The build succeeded: configuration 11.547 seconds, compilation/linking 100.422
seconds, full successful launcher invocation 113.517 seconds. All six selected
release/CLI checks passed. All eleven historical tier-2 comparisons passed under
the inherited comparator; nine raw MST outputs matched exactly. Cases
46_MO2_Boolean_Oprations and 52_BEND_2D differed in raw bytes and passed only with
the declared legacy-express-simtyp adjustments. No tier-2 project-copy mutation
was reported. Checks took 3.569 seconds including the two subprocess invocations.
These timings do not measure recovery speed or demonstrate token savings.

## Execution boundary

The first validation runs only the release checks, missing-input CLI check, and
the eleven active historical tier-2 cases. The tier list's twelve-case heading
is stale: 53_Cogging_Multiple_Ops_Lab1 is commented out. The inherited corpus
runner copies each project, hashes the copy before and after conversion, writes
MST outside the project, and retains scratch with --keep. The wrapper verifies
selected case paths remain inside their expected roots before execution.
Full Save flags are not enabled in this baseline check. No original application
process is launched by these checks.

The existing runner changes expected lines for declared waivers. Preserve those
results as inherited comparisons, and record raw output hashes separately;
waiver-adjusted PASS does not establish unmodified byte equality. Historical
saved outputs also do not establish a fresh runtime comparison with the original.

## Findings to resolve before the complete test suite

- CMake's default M2M_CORPUS points outside this project. It is overridden in the
  isolated configuration; the baseline source remains unchanged.
- Several MDT tests use M2M_ORACLE_B with a fallback to the old recovery store.
  Set the environment explicitly when those tests are run.
- test_cogging_save.cpp hardcodes the old MDT8_VDIE42 oracle directory.
- test_db_load.cpp hardcodes C:/PROBLEM/TEST_REA/COG_P1_B1 as an input source.
  These inputs must be redirected to imported copies before running those tests.
- test_mdt_write.cpp optionally reads installed PACK/pack.dat for a header check.
  Qualify that external read or provide an explicit local fixture.
- g0/test_abi.cpp accepts either M2M_OK or M2M_E_UNSUPPORTED for its real-project
  case. A passing result alone is not successful conversion coverage.
- The inspected convert.cpp accumulates file effects and applies them after MST
  generation, using atomic replacement. The original Save findings already
  identify failure-order differences; this build does not correct them.

The next recovery checkpoint must resolve test relocation, establish the complete
current failure inventory, and select an evidence-bound functional correction.
