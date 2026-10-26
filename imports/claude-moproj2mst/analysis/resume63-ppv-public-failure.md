# CP63: public PPV Save-open failure

## Test added

Added a third Release-active case to
`tests/mdt/test_ppv_public_lifecycle.cpp`. It copies the retained
`93_B_MB3ACT_before` project into a unique test-scratch directory, changes the
copied project's zero-based active diagram index to select operation 2, and
places one valid local PPV row at `Operations/Task00002/SimCtrl.PPV`. It then
marks only that scratch PPV file read-only before calling exported
`m2m_convert_w` with `M2M_FLAG_WRITE_SAVE`.

The assertions require `M2M_OK`, the requested MST output, unchanged PPV bytes,
and an updated `SimCtrl.KEY` last-write time. The key timestamp is set to a
fixed old date before conversion; after the call it must be newer. This shows
that the public Save did write the operation KEY while the existing PPV input
remained intact. The original source manifest is checked after the call, and
the PPV read-only attribute is restored in scratch.

## Why the failure setup is deterministic

The retained writer receipt in `analysis/resume48-ppv-writer-contract.md`
qualifies writer open failure as returning false without writing. The native
Save call in `analysis/resume58-ppv-save-root-gate.md` establishes
SimCtrl.KEY-before-PPV order and that the PPV helper result is ignored. In the
candidate, `src/mst/file_device.cpp::FileDevice::FileDevice` opens a PPV path
with `CreateFileW(GENERIC_WRITE, OPEN_ALWAYS)` and truncates only if the handle
opens. Windows denies a write handle for a file carrying
`FILE_ATTRIBUTE_READONLY`; read access remains available to the Open parser.
The public test sets that attribute on the disposable PPV file after writing
and verifies the bytes and attribute survive the Save.

`src/engine/save_current.cpp::saveSimCtrl` writes SimCtrl.KEY first, then
creates the PPV `FileDevice`. Its failed-open result is translated by the
writer adapter to `OpenFailed`; the caller emits only a payload when status is
`PayloadReady` and otherwise proceeds to return the SimCtrl.KEY result. Thus
the public expectation is success, a written KEY, and no PPV rewrite.

## Qualification

This case tests the candidate's externally visible public behavior for one
local-point PPV open failure. It does not execute the original GUI, prove
byte-for-byte Save parity, validate non-Windows filesystems, or establish
post-open partial-write behavior. The retained static evidence supplies the
original call order and ignored-return contract; the test supplies the
candidate-only scratch outcome. The test was not built or run in this lane.

Root integration added an attribute-restoration destructor after the test
marks the scratch PPV read-only. It restores the original attribute even if a
later `REQUIRE` returns early. The integrated test SHA-256 is
`1cdd0bab6bd0e75826dab3a506d40f02796ecc993f2c6704cf277119b6b1f0f0`;
the worker's earlier hash records the pre-integration revision.
