# CP161 FRCROT table-reader suffix implementation

Added a private prepared-observation projection in `source/apps/moproj2mst/src/doe/frcrot_table_prefix.hpp/.cpp` and focused Testkit coverage in `source/apps/moproj2mst/tests/doe/test_frcrot_table_prefix.cpp`.

The helper models only the qualified forward table state. Modes 0 and unknown return before allocation/scanning/line-read; modes 1/2 select codes 1/4, normalize negative prepared counts to zero, and require an explicit successful allocation/zero-fill qualification. The candidate count bound is 0..7. Ordered observations accept only -1/0/1: success assigns immediately, zero leaves the slot zero and continues, and -1 stops with table status 1 while retaining the already-written prefix. The result exposes X_i/Y_i destination order and the ignored trailing line-read boundary. It does not parse text or claim actual owner, public Save, or mirror payload behavior.

Preflight: the test includes `<testkit/check.hpp>`, resolves its quoted header relative to the test source, uses only supported Testkit macros (`CHECK`, `CHECK_EQ`, `CHECK_BITS`), contains `main() { return testkit::run_all(); }`, and has no C `assert`. No build, test, GUI, live REA action, or mutable database operation was run, per task instruction.

SHA-256:
- `frcrot_table_prefix.hpp`: `a155a0e468dea82bbe1148afb51ce491c0c0c042a25db096ae2448361f6e216c`
- `frcrot_table_prefix.cpp`: `9c324bdeb2eb1ec183c672d2c669566f35a8a5ea9bd919f1f60d2cba4a2f470c`
- `test_frcrot_table_prefix.cpp`: `ff88abf52467a350f1b65c39365886987b97b15929faaa07acd4480d6e6b444d`

Release: these three assigned files and this report are ready for integration review. No existing source, public header, CMake file, or unrelated file was edited by this worker.

