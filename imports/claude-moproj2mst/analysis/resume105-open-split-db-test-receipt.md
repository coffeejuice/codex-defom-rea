# CP105 Open caller-level split-DB test receipt

## Exclusive source

Added only `source/apps/moproj2mst/tests/mdt/test_open_split_db_default.cpp`.

- SHA-256: `0A43169574FE7F6EB3B5FF00005F66FF6EF4B7E3A02AAAFE132A6CC76415ED5D`
- CMake's existing test glob derives executable `m2m_mdt_open_split_db_default` and CTest `m2m.mdt.open_split_db_default`; no CMake change was made.

## Test contract

The test copies the retained `125_B_DBSTEP_NODB_before/COG_P1_B1.moproj` input into a unique directory under `C:\PROBLEM`. It creates a synthetic, format-valid, header-only predecessor/main pair named `COG_P1_B125.DB` (step 25) and `COG_P1_B1.DB` (step 26) using `deformdb::Writer` and a header read from the existing COG fixture. All generated fixture paths are checked to remain below 100 characters. Cleanup removes only the test-created child after resolving it and verifying its canonical parent is `C:\PROBLEM`.

It initializes a candidate Project/registry, then calls `openProject` itself. That reaches the retained caller order: suffix/path setup, `engine::openProjectDb`, `primaryDbPath`/`dbOpen`, then `.moproj` reading. Assertions cover accepted/parsed Open, selected DB path, loaded handle/cache, two-part predecessor/current order and directory ranges, the configured default-Yes warning, and the 175-pixel diagram-height result. It stops before subproject activation and performs no Save or Close.

This exercises the candidate Open caller and the synchronous split-chain/default profile. The pair is **synthetic**, not evidence of a production database split at 2 GB. The test does not show or dispatch a QMessageBox, qualify nested-modal timer/event delivery, or establish complete original GUI Open equivalence.

## Preflight and execution status

Static preflight confirmed all quoted project/test includes exist, the packet test glob registers the file without CMake edits, and there is no trailing whitespace. No build or test was run by this worker; root owns integration/build. No production/shared files, GUI, original executable, or mutable REA database were touched.
