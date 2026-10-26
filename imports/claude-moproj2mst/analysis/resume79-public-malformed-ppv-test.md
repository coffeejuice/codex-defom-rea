# CP79 — public malformed-row PPV Open-to-Save regression

## Change

Added Release-active case public_ppv_malformed_second_row_keeps_first_row_through_save to source/apps/moproj2mst/tests/mdt/test_ppv_public_lifecycle.cpp. It copies the retained 93_B_MB3ACT_before project into a uniquely named test-scratch directory, selects the operation whose Task00002 SimCtrl.KEY exists, and injects a PPV file declaring two rows: one valid local row followed by a short malformed row.

The case invokes the public m2m_convert_w API with WRITE_SAVE. It requires M2M_OK and the MST output, then checks that the first row survived the parser failure and was normalized by Save to the expected seven-column output. The retained source manifest is rechecked unchanged. Production code and source fixtures were not edited.

## Static preflight

- Testkit include and entry point are present; the file uses supported CHECK/REQUIRE macros and has no C assert.
- The test uses the same relative support test_paths.hpp helper, fixture copy, and public DLL lookup as neighboring PPV lifecycle cases. The new scratch child is distinct: ppv_public_malformed_row.
- Static scan found no trailing whitespace in the edited test. The working copy reports the test file as untracked; no unrelated files were modified by this increment.
- No build, CTest, test executable, GUI, original process, or mutable REA action was run. Root owns validation.

## Expected contract

The PPV parser retains a row appended before a later malformed row and returns false. The native caller's reader result is ignored, then it updates counts and returns the SimCtrl.KEY result. Current loadSimCtrl continues for ParserReturnedFalse, and current Save writes the resulting local row. The test checks that integrated public consequence; it does not assert broader codec support, exact native parity, or post-open physical failure residue.

## Frozen file

source/apps/moproj2mst/tests/mdt/test_ppv_public_lifecycle.cpp SHA-256: 0FA0F04DD26A7C73506180ABB958D690DC333969162BA01281F29CB084F1C7B0


