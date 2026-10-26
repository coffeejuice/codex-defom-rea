# CP80 independent review: project blob read-open gate

## Finding

The released `readAll` change matches the bounded PE branch in `analysis/resume80-save-pe-branch-next.md`: original helper `0x1400a1890` returns the default null `QDomElement` when the path is empty or QFile(ReadOnly).open fails. On successful open it performs `QIODevice::readAll`, closes the QFile, compresses/hex-encodes the returned bytes, and constructs `Data`; the captured helper has no post-read QFile error check. Candidate `readAll` now returns false only when its `ifstream` failed to open and otherwise returns the collected bytes. It therefore preserves the important distinction between unreadable existing file and readable empty file, and does not invent an additional read-error gate.

There is no failure-order regression visible in this slice. `blobElement` returns null for the failed disk read, and `write_internal.hpp::append` ignores a null child. The operation-list caller appends this result without a throw or early return, so the project body continues toward its later sub-project DOM replacement and serialization. The original decompile likewise returns a default QDomElement on the failed-open branch and its caller copies/appends the result without a branch at the call site. This remains a source-path conclusion, not a full native Save execution claim.

Earlier virtual-source precedence in `readProjectFile` (later delete, latest write, overlay, then disk) is unchanged. The new `ifstream::is_open()` check is after the existing `qt::fileExists` check, so an existing but sharing-denied file reaches the actual open attempt. The lock fixture verifies both conditions independently: file existence stays true while the actual probe open fails. The CP80 change does not alter `withBlobs` archive guards or shared Save stage ordering.

## Test coverage boundary

`tests/moproj/test_project_blob_open.cpp` covers missing versus readable-empty versus binary payload, the existing-but-read-denied helper result and recovery after unlock, and virtual write/overlay/delete precedence with the physical file still denied. The lock setup is strict `REQUIRE` plus a real `ifstream` probe; there is no skip path. It is appropriately narrow for the helper branch and checks the specific prior bug (unreadable existing disk being mistaken for empty bytes).

It does not invoke `operationList`, `saveProjectFileBody`, or a public Save. Consequently it does not inspect the serialized project's parent DOM to prove the `<Data>` node is absent, or directly observe that the output project stream still closes and later Save writers run. The inspected call path and null-append helper make continuation clear at source level, so I found no implementation change required. If root wants a higher-level acceptance case, the smallest extension is a minimal object-property project with an existing locked side.KEY, followed through the existing project writer service; assert the parent key has no `Data`, the project writer returns normally, and its post-close query is reached. A full Save continuation case would additionally assert later property/MST/remesh effects, but belongs to the separate Save I/O suite.

## Residual qualifications

- Original evidence is bounded static disassembly and import resolution, not an injected original read failure or file-residue capture.
- Candidate uses `std::ifstream` after `qt::fileExists`; standard stream read/partial-read details are not proven identical to QFile beyond the qualified open gate and successful payload cases.
- Do not add `in.good()` or a post-read failure branch without new native evidence: the inspected helper compresses the bytes returned by `readAll` and does not inspect QFile error before Data construction.
- Full archive `withBlobs` guards remain intentionally separate from this Data side-file helper.

No source, test, build, GUI, or mutable REA changes were made during this review.
