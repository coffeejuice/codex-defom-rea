# CP130 Type3 tokenized ENDDATA control result

## Change

Added `Type3ScalarLineResult::Terminate` to the private tokenized scalar dispatcher. A line whose first field is `ENDDATA` now returns `Terminate` without mutating any scalar field. Skipped headers/unknown keys continue to return `Ignored`; `NUM_CON` and `REGIONS` remain `RowBodyNotImplemented`. The dispatcher still does not read files, split lines, close QFile/QTextStream, or integrate with ShapeOptim.

## Evidence and limit

`analysis/resume127-type3-opt-parser.asm` contains the bounded parser body tied to original `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. CP128 records that ENDDATA exits the top-level line loop to common close/cleanup at `0x140909400`. CP129 independent review noted that returning `Ignored` left this stop decision outside the tokenized helper. The new result represents only that already-tokenized control distinction; no file cleanup or parser fidelity is claimed.

## Test and preflight

Added one focused case proving ENDDATA returns `Terminate` and leaves every modeled scalar unchanged; removed ENDDATA from the no-op skipped-key set. No CMake changes are needed; the existing test glob picks up `test_type3_optimizer_scalar_prefix.cpp`, target `m2m_doe_type3_optimizer_scalar_prefix`. Static checks confirm private include resolution, testkit entry point/labels/identifier-form cases, no C `assert`, and clean whitespace. No build/test execution was run. Public Type3 and ShapeOptim guards remain unchanged.
