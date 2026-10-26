# CP67 read-only Testkit source preflight

Added `admin/check_test_source_preflight.py`, a read-only command-line check for explicitly supplied changed/new `tests/**/test_*.cpp` files. It reports a missing `int main(...)`, a `main` body that does not call `testkit::run_all()`, and directly included quoted headers it cannot resolve. Its default include search includes the test’s own directory, converter `include/` and `src/`, `testkit/include`, and the port-library `include/` directories; target-specific directories can be added with repeatable `--include-dir DIR` arguments. Paths must remain under the selected converter project’s `tests/` tree, so baseline files are not automatically swept or mistaken for newly changed inputs.

Paths may be relative to the converter project root (`tests/...`) or checkout root (`source/apps/moproj2mst/tests/...`), or absolute. Example from the checkout root:

```text
python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/s/test_report_ds_path.cpp
```

The result is JSON and exits 0 only when all supplied files pass, 1 when source issues are found, or 2 for invalid paths/read errors. Run it on the changed test-file list before the grouped Release target build. For the CP65 failure, supplying the earlier `test_report_ds_path.cpp` snapshot without `int main` would have produced `missing_int_main`; the current version passes. The live file was checked read-only and passed with no missing direct quoted includes.

Path handling was corrected to accept both documented relative forms safely; resolved candidates must remain beneath the selected project’s `tests/` directory. A regression test covers the checkout-root-prefixed form. Focused validation: `python -m unittest discover -s admin/tests -p test_check_test_source_preflight.py -v` passed 8 cases, covering both path forms, valid entrypoint/include resolution, missing entrypoint, missing `testkit::run_all`, missing header with line number, comments, and path containment. `py_compile` passed for the checker and its tests. `python admin/check_continuation.py` reported `CONTINUE`, objective incomplete, 15 remaining items, and no errors.

This checker is an early diagnostic, not a C++ compiler. It checks direct quoted includes in supplied sources; it does not resolve transitive or conditional include graphs, validate declarations/types, compile/link target code, or replace the Release preflight and full build/CTest suite. No production source, fixture, build output, CMake file, GUI, or mutable REA state was changed.
