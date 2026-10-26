# Resume 52 public Cogging locked-write regression

Added `save_cogging_locked_flush_continues_later_operation_and_mst` to `source/apps/moproj2mst/tests/s/test_save_io.cpp`.

The public test uses the existing `133_B_DBSTEP_BOOL3_before` full-Save fixture. Its Cogging operation is operation id 3, with an existing nonempty `Operations/Task00003/Cogging.MDT`; the project also has a later Boolean operation whose Save output is observable through `Task00002/Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI`. A shared byte-range lock (offset 0, 8 KiB) is acquired with read/write sharing before public conversion. Open can read the MDT through this shared lock; the Save CRT writer cannot write to the locked range.

The same-host three-run CRT probe at `analysis/resume52-cogging-lock-probe.md` establishes the exact platform outcome for the shared-lock pattern: Cogging's `CREATE_ALWAYS` open succeeds and truncates, the 4 KiB CRT buffer's first flush fails with zero bytes, and the final MDT is empty. Accordingly, the test requires `M2M_OK` from the candidate adapter, zero-length Cogging.MDT, Boolean markers, and completed MST. No earlier stage is removed by this test.

`M2M_OK` is the current adapter status. The original static contract is distinct: original `0x1407617e0` ignores Cogging printf/fclose failures and returns its handled result; top-level Save returns its prior successful check result, with the GUI wrapper/dialog result tracked separately. This case does not equate adapter `M2M_OK` with that original GUI status, nor claim fresh original fault-injection acceptance. It bridges the already-tested lower-level Cogging CRT behavior into the public Open→Save path.

No build/test was run by this worker. Only the assigned Save I/O test source and this receipt were changed; root owns validation.
