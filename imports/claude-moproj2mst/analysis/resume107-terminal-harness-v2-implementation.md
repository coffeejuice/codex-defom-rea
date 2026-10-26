# CP107 RemeshV2 protocol and owned preparation lease

The approved harness source is frozen for root build/validation. This adds the64-byte RemeshV2 codec/site2, explicit decoder selection and harness-only preparation/inspection lease. The existing v1 safety test is byte-identical to CP105 (11 cases). The new target is `m2m_mdt_terminal_harness_remesh_v2`, CTest `m2m.mdt.terminal_harness_remesh_v2`, with8 Release-active cases. No core/convert, remesh writer, service header, public C ABI, product CLI or CMake file was edited.

## Protocol implementation

ProtocolProfile defaults to SafetyV1. V1's40-byte encoding/checksum/state behavior is retained; helpers are generalized only by array extent. Separate FrameV2/encodeV2/CollectorV2 implement the agreed version2/site2 layout: fixed magic/version/kind, nonce, process status, EINVAL/failure tag, PID/ordinal, profile/proof and full SaveSnapshot flags/count/raw answer/pendingDefault, with checksum over the first60 bytes. No pointers, Session objects or public result layouts enter the wire.

Snapshot flags match the CP106 bit table. The decoder rejects reserved bits, unknown dialog values, returned-without-attempted states, an absent snapshot with nonzero fields, and a Boundary without both project/MST attempted+returned. Succeeded bits are preserved and interpreted only with reached attempted/returned flags; no default success is fabricated. FreshDefault/proof are explicit wire1 values, separate from native enum ordinals. Boundary requires statusC0000417, errno22 and ordinal1. Backend failure tags are explicitly1..3 and have empty metadata/snapshot. Started cannot carry a snapshot; Returned may carry an absent or partial valid snapshot. V1/v2 mixing never selects or upgrades a decoder from incoming data.

Transcript records its selected profile, optional snapshot and failure tag. CandidateProcessTerminal is distinct from SelfTestTerminal and requires a decoded v2 boundary/snapshot plus matching process status and no sticky supervisor failure. Timeout, publication/backend failure, inspection failure and malformed data cannot be promoted by a terminal-looking exit. These classification tests use synthetic wire values, not observed remesh behavior.

## Preparation, stop and inspection ownership

Options appends protocolProfile and prepareOwnedCase. The preparation callback receives only the already validated exclusive new leaf and returns unique_ptr<PreparedCase>. The supervisor copies Options before invoking it, so callback mutation of its caller's executable/profile cannot change the selected launch. A missing lease or thrown preparation error prevents launch and records Preparation. A callback finishing past the deadline prevents subsequent launch.

The lease is declared before all job/process handles, so it outlives their cleanup. After the owned child is confirmed stopped, inspectStoppedCase receives the bounded Observation while parent denial/fixture handles remain held. It may capture manifests or fail; thrown inspection errors set sticky Inspection unless an earlier failure already dominates. The normal lease is destroyed only after inspection and child/job handle teardown. No callback is retained in a Session, Project, public result or child wire.

The catastrophic branch is explicit: when a child was created but OS stop could not be confirmed, the supervisor does **not** destroy the prepared object. It releases the unique_ptr without deleting and flags preparationHeldUntilParentExit. Its OS resources stay held until parent teardown; the existing job handle still closes on function return. The result remains HarnessFailure/Timeout with childStopped=false, never cleanup success. This intentional fail-closed retention is bounded to one lease per such failed run and is not the normal resource path or native cleanup emulation. No test induces an actual unkillable child.

Preparation/inspection callbacks are trusted harness code and must bound their own local fixture operations. The supervisor checks elapsed deadline after preparation; it cannot asynchronously preempt arbitrary C++ callback code or an OS/storage failure. Child protocol monitoring retains the available-byte-only pipe reader, single deadline, capture cap, restricted inheritance, owned job and bounded cleanup.

## Tests and provenance

The new executable handles the explicit `--terminal-harness-remesh-v2-child` role as protocol/control fixtures only. It does not call conversion, the core bridge or the remesh service. It rejects direct v2 terminal endpoint modes. Malformed/partial/full-boundary stall controls emit clearly synthetic protocol data and then return/stall/fail; none is accepted as an actual source-bound terminal execution.

The8 cases cover exact fixed offsets/full raw snapshot round trip; optional/partial returned snapshot; reserved/incoherent/wrong source/profile/proof/status/errno/ordinal; checksum/truncation/mixing/conflict; explicit backend tags and sticky status classification; actual parent prepared read-sharing/write-denial lease through child read/denial and post-stop inspection; preparation/null/deadline and inspection failures; and bounded v2 malformed/half-frame/full-boundary-stall controls. The prepared-file control reads KEEP and verifies write denial, while the parent inspector independently sees denial still held; after supervise returns the test verifies release. Callback mutation of caller options is exercised. Earlier Timeout remains selected when inspection also throws.

Successful test runs retain one bounded `terminal-v2-last-success.txt` summary in checkout/build scratch and delete only the canonical exclusively owned case root. Unexpected test failures preserve receipts/captures. Existing v1 success/failure evidence behavior remains unchanged. Expected executable hash in these tests is the measured own-image identity; root's pre/post binary/source binding remains required for independent build provenance.

## Static release

The read-only Testkit source preflight passed for both targets: supported main/run_all and direct includes resolve. Direct checks found balanced delimiters, no trailing whitespace and11 unchanged v1/8 new v2 cases. No compiler, build, Testkit child, original/native target, GUI or REA execution occurred. Root owns the first compile, focused runtime validation, repairs, full suite and freeze. This source prefix does not satisfy the pending real remesh vertical conversion/legacy DLL cases or original default-handler prelude.

Frozen files, relative to `source/apps/moproj2mst`:

| File | SHA-256 |
| --- | --- |
| `src/engine/terminal_harness_protocol.hpp` | `f416f1ed095223eee218efa666d87959f0d4c18748f185876403e8b339d8cda0` |
| `src/engine/terminal_harness_protocol.cpp` | `81699936c0b4380def8cc1056498986fea6027151a5fa86b9bd5d7c670ae6638` |
| `src/engine/terminal_harness_supervisor.hpp` | `f95bf5338e8173a5aa1da5f7f998deeca095b97b588313dc106e9371d06f45a7` |
| `src/engine/terminal_harness_supervisor.cpp` | `2d22656a825ae76d44ba5a917cc03b38aa0875190f83a6404725197265c37b22` |
| `tests/mdt/test_terminal_harness_safety.cpp` | `73bff8feaf3f46afa32433120ee7118d8dcaa0479842710f059b2e22ef371ca5` |
| `tests/mdt/test_terminal_harness_remesh_v2.cpp` | `1d18eea05ff6bbfb7b4776193f9e469dd1826518e7500f4a1ce8b65b0530edaf` |
