# CP65: native Type2 project-writer prefix test

## Added test

Added `tests/doe/test_type2_native_open_guard.cpp`, an automatically discovered
`m2m.doe.type2_native_open_guard` test. It copies the retained
`120_B_DOE` fixture under `M2M_TEST_SCRATCH`, verifies a recursive file/hash
manifest against the source, and reads the copied `.moproj` into a Project
DOM. The test builds a fresh `ConversionSession`-owned Project with its
default Type 0 SUB and an owned Type 2 SUB at index 1, using `NativeStudyIo`
bound to that Session's ModelManager and notification owner. It loads the
retained `DoeInputTable.XML` into that owned Study and checks the empty-variable
and zero-constraint preconditions for the InputTable XML-output boundary.

The test then calls `moproj::saveProjectFileNative` on the copied
`COG_P1_B1.moproj` path. This is an internal writer test, not a public C ABI
or complete Open/Save test. It requires the exact
`M2M_E_UNSUPPORTED` message
`Type2 study Save: InputTable XML output device remains unsupported`.
It checks that the copied `.moproj` is zero bytes afterward, that the prior
Type2 `<Sub_Project>` node remains the same in-memory node with identical
serialized contents, and that the retained source manifest is unchanged.

## Why the assertions are source-qualified

- `src/moproj/write.cpp::saveProjectFileNative` constructs `mst::FileDevice`
  before entering `saveProjectFileBody` and returns immediately only if the
  file open fails. `src/mst/file_device.cpp::FileDevice::FileDevice` opens
  existing files with `OPEN_ALWAYS` and calls `SetEndOfFile` immediately after
  a successful handle, so the scratch `.moproj` is already truncated before
  the body runs.
- `saveProjectFileBody` updates the outer DOM roots/lists, then calls
  `subProjectElement`. That function builds a pending Type2 node and reaches
  `appendType2SaveNodePrefix` before replacing the existing SUB node or
  constructing the output stream. The prefix returns the zero-variable
  `XmlOutputDevice` boundary; `subProjectElement` throws its exact guard
  message. Thus no project XML bytes are written after the truncate, while the
  previous Type2 subtree remains installed. Earlier root DOM mutations are
  expected and are not mistaken for total DOM rollback.
- The retained `120_B_DOE/COG_P1_B1.moproj` is pinned at 34,774 bytes and
  SHA-256 `60addc2722e85f92a088fa07f77e8a966928ab5b934f161df87dcf6a417ed30e`.
  The copied `DoeInputTable.XML` has an empty `<DoeVariables/>`; the test
  parses this into the owned Study and verifies the target branch precondition.
- The retained source tree is only read and re-manifested after the call. The
  zero-byte result and DOM mutations are confined to the disposable copy and
  the test-owned in-memory Project.

Passing this test establishes only the candidate's native `.moproj` open and
truncation prefix, the precise current Type2 InputTable refusal, and retention
of the previous Type2 DOM node. It does not establish InputTable serialization,
complete Type2 project Save, public C ABI Type2 selection, or original output
parity. Root owns build and runtime validation; this lane did not build or run
the test and used no GUI or mutable REA session.
