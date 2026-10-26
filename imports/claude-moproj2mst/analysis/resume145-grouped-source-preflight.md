# CP145 grouped source preflight

Added an explicit, repeatable `--preflight-source FILE` option to the grouped `build_candidate.py --then-full` flow. Each named file is passed once, in argument order, to the existing read-only `check_new_cpp_bindings.check_files` checker before configure. Checker findings, expected checker read errors, or a source fingerprint change during the check stop the grouped sequence before any configure/build command and are retained in an additive `source_preflight` receipt field.

With the option absent, the checker is not called, no source-preflight receipt field is added, and the existing grouped configure, focused build, full build, drift, and artifact gates are unchanged. The option requires `--then-full`; it does not scan a directory implicitly. Successful opt-in runs continue through all existing compile and full-build gates.

## Validation

Ran the focused admin suites:

```text
python -B -m unittest admin.tests.test_build_candidate_grouped admin.tests.test_check_new_cpp_bindings -q
Ran 30 tests in 0.410s — OK
```

Coverage includes repeated CLI arguments and their order, checker-before-configure ordering, fail-fast on checker issues and source drift, rejection outside grouped mode, and proof that the default grouped CLI does not invoke the checker. These are mocked command tests; no converter build or full validation run was performed.

The checker/builder test suite took 0.410 s in this run (wall time from unittest). This is only the focused harness runtime, not a measured build-time saving. The opt-in check is intended to catch obvious binding errors before configure; it does not replace compilation.

## File SHA-256

- `admin/build_candidate.py`: `b382a76bcdcef79e5a6bd3b1d4afe6b8032050bc92cd2182df11d9e67100e3de`
- `admin/check_new_cpp_bindings.py`: `7419dd2842698b18075e7fde151aca1306ce33b18f52be9d0eb7bfc0be95c535` (unchanged by CP145)
- `admin/tests/test_build_candidate_grouped.py`: `f19185bab39fc031ec74a43e42a01dfc76a8e451e8c4c642507bc1d90936820b`
