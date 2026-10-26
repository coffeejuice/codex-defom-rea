# CP111: private isolated job and terminal receipt

The new job implementation passes bounded immutable UTF-16 inputs to the explicit isolated worker, and commits a non-overwriting terminal receipt only after a qualified worker termination observation. It never terminates a process or claims that the parent relay has been observed. Root-owned headers, CLI entry, supervisor/worker, harness and CMake were not edited by this lane.

Frozen files:

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/cli/isolated_save_job.cpp` | `2eaf5b06156721deda592251f11a8b7ca47268442836547658af6ceb8f8a5ea5` |
| `tests/g0/test_cli_isolated_job.cpp` | `2bcc165fe1e95c6c2699e3f044c972f0bd955788f174408de03b2b402706873e` |

Root will register/link the private CLI library and `m2m_g0_cli_isolated_job` / CTest `m2m.g0.cli_isolated_job`. The test has nine Release-active cases and one Testkit main. Source implementation279 lines, test214 lines; no build or runtime acceptance is claimed here.

## Job transport contract

The three exact functions in root-owned `isolated_save_job.hpp` are implemented. The fixed exclusive filename is `isolated-save.job` inside the supplied existing supervisor leaf. Root added required `workingDirectory` and cryptographic `nonce` during this implementation; both are now serialized and validated. Header qualification inspected: `isolated_save_job.hpp` SHA-256 `de28e3e369a34eeff8d12513430eff4aa669e650dd91d4051a66f338b079102d`; `isolated_save.hpp` SHA-256 `b505e03bfdb01b2cf53e7395931adb09c896b3aa179cdab8af56aa41d5267be7`. These headers were read, not written.

All integer fields are explicit little-endian, independent of structure packing:

| Byte offset | Value |
| ---: | --- |
|0..7|magic bytes `4d 32 4d 49 53 4a 31 00` (`M2MISJ1` plus NUL)|
|8|uint32 schema version1|
|12|uint32 total bytes, exactly equal to physical file size|
|16|uint32 flags|
|20|uint32 codepage|
|24/28/32/36|uint32 UTF-16 code-unit counts: project/output/deform/workingDirectory|
|40/44|uint32 nonce low/high halves, combined nonzero uint64|
|48|uint32 reserved0|
|52..end|the four UTF-16LE payloads in that order, without terminators or padding|

Total is at most65536 bytes; each path is at most16384 UTF-16 code units. Counts are bounded before multiplication and payload reads, and must account for every file byte. Magic/version/reserved/size/trailing data are rejected. Paths must contain valid paired UTF-16, no NUL/control/invalid Win32 wildcard or namespace characters, no alternate-stream colon, and must be absolute when nonempty. Project and caller workingDirectory are required; output and deformDir may be empty for defensive reuse. Root's product parser requires all selected product paths absolute/nonempty. No project/output/deform existence or extension probe is performed here: actual Open/Save owns those outcomes, and no project is pre-opened in the parent. Flags must include WRITE_SAVE and contain only the four existing public flag bits; codepage uses the ABI's existing IsValidCodePage check. No mutable environment setting is read by this layer.

The storage leaf must be an existing ordinary local drive directory. Dot components, missing directories and reparse ancestors are rejected. A no-delete-sharing directory handle pins the leaf while the fixed child file is accessed. Job creation uses CREATE_NEW with no sharing, complete bounded writes, FlushFileBuffers and checked CloseHandle. Reading uses a regular non-reparse disk file with read-only sharing, excluding mutation/deletion while loading, and confirms its physical size before allocating. File handle final parent is compared with the pinned directory before data write/read, so a mismatched resolved backing parent is refused. A failed write/flush/close throws and cannot authorize child launch; any partial newly created file is retained rather than silently replaced or reinterpreted. No transactional filesystem or arbitrary hostile concurrent-rename guarantee is claimed.

## Qualified terminal receipt

`commitIsolatedTerminalReceipt` accepts only CandidateProcessTerminal, StopReasonNone, created/resumed/confirmed stopped child with nonzero PID, naturalC0000417, no requested supervisor stop or uncertain retained lease, RemeshV2 exactly two valid Started+Boundary frames, no Returned/backend failure/invalid/failure tag, present bounded SaveSnapshot with both project/MST attempted+returned, legal raw answer enums, and matching lowercase64-hex before/after worker image hashes. Earlier false Succeeded bits are preserved, not upgraded to success. Job validation including nonce applies again.

The requested receipt is absolute and has an ordinary filename. Its existing local regular parent is validated and pinned; trailing-dot/space aliases and reparse ancestors are rejected. Canonical comparison excludes the selected project tree and explicit output target. CREATE_NEW/no sharing never replaces an existing file, hard link, child-created target or directory. The function completes all writes, FlushFileBuffers and checked close before returning. Any failure throws, retaining an accurate partial artifact where created and preventing the caller from treating it as a committed relay receipt. No process termination or relay call is inside this implementation.

The bounded <=256KiB versioned JSON contains job nonce, original selected paths/workingDirectory/flags/codepage, actual worker PID/status/image identity/elapsed time, fixed validated Boundary metadata and every raw SaveSnapshot field. Paths use JSON UTF-16 escapes when required, an ASCII subset of valid UTF-8 that preserves supplementary code points without lossy codepage conversion. Qualification remains `candidate-fixed-endpoint;native-default-handler-prelude-unimplemented;no-native-Close-claim`. Handler model is explicitly StaticFreshDefaultAssumption/DefaultPrefixComplete/next CaptureContextUnimplemented with native_execution_observed=false. The worker termination is observed; `parent_relay.requested=true` and `parent_relay.observation=not_observed_in_this_receipt` describe only the next requested action. No future parent termination or native Close is claimed.

## Focused static coverage

Nine cases cover product-shaped Unicode/spaces/supplementary paths, all field/nonce roundtrip and caller CWD; optional output/deform reuse; exclusive no-replace jobs; bad relative/empty/UTF-16/NUL/stream/namespace/flag/codepage/nonce inputs before creation; exact65536-byte acceptance and overflow rejection; malformed binary magic/version/count/reserved/truncation/trailing data/UTF-16/nonce; missing, oversized or directory-backed jobs and relative storage; complete qualified JSON plus immutable duplicate rejection; malformed/untrusted observations rejected before receipt creation; earlier false MST status retained and project/output/relative receipt aliases refused. Receipt tests construct synthetic observations: they test serialization/admission, not actual worker termination or persistence under power loss. Physical failed flush/close and arbitrary external filesystem races remain runtime failure paths, not simulated passing observations.

Every quoted include and Testkit header resolves against the actual source/test/include roots. CHECK, CHECK_EQ and REQUIRE exist; no C assert or unsupported REQUIRE_EQ. One Testkit main, nine cases, balanced conservative delimiter scan, no trailing whitespace. No compiler, build, CTest/test process, original, GUI, REA, external project or installed application was used. Only the two assigned new C++ files and this unique report were written. Root owns integration/build, worker/CLI end-to-end acceptance and source freeze.
