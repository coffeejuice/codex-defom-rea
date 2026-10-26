# CP171 PACK fixture requalification

## Read-only fixture check

The existing installation fixture is present at:

`C:\Program Files\SFTC\DEFORM\v11.0.2\PACK\pack.dat`

Read-only inspection returned 11 bytes, SHA-256 `f82967bf8117671aad02e8621549adbb6caf1e5265c55818ec806d9d6b250c10`, and text `VERSION 2\r\n`. This matches the fixture binding recorded by the earlier CP124 focused check (`analysis/resume124-pack-focused-check.md`). The path is passed as the installation directory (`C:\Program Files\SFTC\DEFORM\v11.0.2`), not as the pack.dat file itself.

## Current test contract and CP170 status

Current test source `source/apps/moproj2mst/tests/mdt/test_mdt_write.cpp`, `header_text_from_pack_dat`, reads `M2M_INSTALL_FIXTURE` as a DEFORM installation directory, requires `<directory>/PACK/pack.dat`, and checks `mdtHeaderText` returns `DEFORM-2D/3D 11.0 (Service Pack 2)  `. If the variable is unset, it prints `SKIP optional imported PACK/pack.dat header check: M2M_INSTALL_FIXTURE unset`. This is an optional fixture assertion; it does not launch or probe the installed application. The test also removes its own PID-specific scratch directory under `m2m_test::scratchRoot()`; the prior CP124 run directed scratch to its disposable test directory.

The CP170 full-run receipt `analysis/resume170-compilefix-tests.json` still marks `m2m.mdt.mdt_write` as passed with the unset-variable event. CP124 demonstrated the assertion against its then-frozen source snapshot; that historical result does not clear CP170's logged event. CP171 needs a focused run against the current build/source to do so.

## Exact focused invocation for the next authorized run

From `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst`, set the fixture environment variable for the test process and invoke only the existing target:

```powershell
$env:M2M_INSTALL_FIXTURE = 'C:\Program Files\SFTC\DEFORM\v11.0.2'
ctest --test-dir build\candidate -R '^m2m\.mdt\.mdt_write$' --output-on-failure
```

Before relying on that command, confirm `build\candidate` is the intended current CP171 build and that its registered CTest name is exactly `m2m.mdt.mdt_write`; do not rebuild as part of fixture requalification. Record the source/build identity, fixture hash before and after, test exit/status, and ensure the event is absent while retaining any other independent diagnostics. If the CTest name or environment propagation differs, use the existing test executable with the same variable and the same isolated scratch contract recorded in its build/test configuration; do not broaden the run to the full suite.

## Qualification

The fixture identity is verified, and the test's variable/path/value contract is verified from current source. The CP170 receipt proves the current full-run gap is specifically an unset environment variable. No test was run here, so the CP171 logged event remains open until a source/build-bound focused run consumes this fixture. A successful focused run would establish this one optional assertion only; it would not clear the `m2m.mdt.mdt_write` status in the frozen CP170 full-run receipt or change the other five logged-gap suites, the diagnostic-difference suite, or historical 93 SAME / five DIFF accounting.

No source, build, fixture, executable, GUI, live REA session, or shared state was modified or run for this requalification.
