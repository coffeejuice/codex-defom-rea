# Checkpoint 53: isolated DBLIST reader

## Implemented files

- `source/apps/moproj2mst/src/doe/dblist_reader.hpp`
- `source/apps/moproj2mst/src/doe/dblist_reader.cpp`
- `source/apps/moproj2mst/tests/doe/test_dblist_reader.cpp`

The new `readDbListDirectChildren(const xml::Node&)` function returns an ordered vector of detached `{tag, text}` values for direct child elements. Text uses the existing `xml::Node::text()` implementation, which models concatenated descendant text. The helper is `const`, has no I/O, and does not access or mutate conversion/session/study state. It is not called by any public Open or DOE dispatch path.

The focused test covers the retained B127 observed shape (`DBNAME` with `../../../COG_P1_B1.DB`), direct-child ordering and text, and empty/repeated child mechanics. The unusual-shape test only asserts what this reader returns; it does not assert that the original application accepts or rejects those forms. This distinction is necessary because original `FUN_140655df0` behavior remains unavailable in the retained static evidence.

## Integration and verification boundary

No CMake file, existing production file, public dispatch, original application, GUI, mutable REA data, or build was changed or used. The test directory is under the existing `tests/*` discovery pattern, whose test source glob uses `CONFIGURE_DEPENDS`; integration/build execution remains for the orchestrator after the checkpoint53 source freeze. Therefore the new test has **not** been compiled or run here. `git diff --no-index --check` reported no whitespace errors for the three new files (only expected LF-to-CRLF notices).

## Frozen SHA-256

The new-file contents at handoff have these SHA-256 hashes:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/dblist_reader.hpp` | `F473337ED4A064A9D5DEB06DE8E886D595C495E7B1D1E0B055435BD1F75B8FC5` |
| `source/apps/moproj2mst/src/doe/dblist_reader.cpp` | `7F794DA9B13F977D3F015D39F9CCD582C27EBDA7990FD0D887F53120CCE3F368` |
| `source/apps/moproj2mst/tests/doe/test_dblist_reader.cpp` | `579D0FF4F87FCB6E2131BF4DC9FD8A77D870524E47FA4E02546C6B00EA3FEDE6` |

The relevant source context and evidence limits are in `analysis/resume53-doe-dblist-seam.md`. This is an isolated parser seam, not recovered native DBLIST ownership, format validation, path resolution, database opening, or active DOE support.
