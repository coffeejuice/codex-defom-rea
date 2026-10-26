# CP105: Cogging.MDT open failure continuation

## Original contract

The original `Save` path (`0x1400980d0`) calls the property-save loop (`0x1400982e0`) before the main MST writer (`0x1400a20d0`); the loop invokes each operation's property-save virtual and ignores its returned value (`analysis/save-semantics.md`, original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`). For Cogging, `0x1407617e0` opens `Cogging.MDT` with CRT `fopen("wt")`; a null result branches to `0x140761c4b`, sets the helper result to 1, and skips the writer body. The property loop then continues to later operation properties and the MST call. This establishes call order and the failed-open return branch; it is static evidence, not a fresh original fault run. It does not establish the main GUI's displayed status or exact CRT/Windows residue beyond the failed open leaving the existing file unmodified.

## Candidate and coverage

The public converter already has the corresponding narrow path in `datafiles::saveCoggingMdt` (`src/datafiles/mdt_write.cpp`): native open failure returns true before `writeMdt`. `core/convert.cpp` continues its operation loop and then reaches the native MST writer. Existing `save_cogging_locked_flush_continues_later_operation_and_mst` covers the different post-open case: the sharing-compatible handle lets `CREATE_ALWAYS` succeed, its byte-range lock makes later output fail, and the candidate test expects an empty Cogging file plus subsequent Boolean and MST output.

The missing discriminant is specifically failed Cogging open with an existing file. A candidate-only public test can reuse the disposable 133_B_DBSTEP_BOOL3 fixture and existing `ProjectOutputObservation::aboutToCloseAction` hook in `tests/s/test_save_io.cpp`. After Open and project serialization have completed, have the callback acquire a read handle on `Operations/Task00003/Cogging.MDT` with `FILE_SHARE_READ` only, retaining it through conversion. This permits the earlier input read but denies the native writer's write access. Assert the Cogging bytes remain the sentinel/original bytes, the later Boolean BWZ/INI markers and main MST are produced, and the candidate adapter result is `M2M_OK`. Keep that candidate status separate from the original GUI Save status.

This is a focused assertion opportunity, not evidence of a current production mismatch: the inspected candidate branch already appears to implement the contract. No production, test, build, GUI, original-execution, or mutable REA change was made for CP105.
