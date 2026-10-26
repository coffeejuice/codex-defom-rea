# CP159 Type2 lazy-reader test handoff

Extended `source/apps/moproj2mst/tests/mdt/test_doe_activation.cpp` with a focused `readLazyStudy` case using `ScriptIo` and a fully admitted fresh `DoeVariable` row (`name='TLOC row'`, `varId='1'`, `dimension='3'`, `numSamples='3'`). The exact direct scalar Keyword contains the strict self-closing `<KwOperations/>` shape. The existing call chain is reachable: `readLazyStudy` reads the input table, `parseInputTable` dispatches the direct row to `appendBoundedHeaderVariableFromSource`, that reader admits the explicit row and calls `readBoundedKeywordConstructorPrefix` for its direct Keyword child.

The test expects the scalar nominal-text guard, then checks retained canonical HeaderVariableRecord ownership, empty operation union, exact nominal text and selectors, earlier outer ROI/study state, and that the only I/O was the input-table read. It does not claim project-open route reachability, public Save, sample/PAR/DOM completion, or native success.

## Preflight

- The test uses the file's existing `<testkit/check.hpp>` entry point and supported `CHECK`, `REQUIRE`, `CHECK_EQ` macros; no C `assert` or `REQUIRE_EQ` occurs.
- Added `../../src/doe/keyword_constructor_record.hpp`; the quoted include resolves from `tests/mdt` to `source/apps/moproj2mst/src/doe`. Existing CMake test targets include source and public include roots, and Testkit is already used by this file.
- No build or test was run, as assigned.
- No GUI or mutable REA activity occurred.

## Identity

`test_doe_activation.cpp` SHA-256: `BF69AE11C0503AF11121226C3A6561F317E5DCE34AED013DCECD095CC7C48DDE`.
