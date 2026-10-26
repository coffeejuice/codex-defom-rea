# CP160: exact read-to-publication witness for the nonempty DOE key

The actual nonempty Type2 SetOprHasDoe branch cannot advance from the retained immutable evidence alone. Its next missing fact is the branch result of the selected invocation's QtCore C-string-field read, followed by that same constructor's actual data publication. CP159's strict empty KwOperations branch skips the notifier; it provides neither fact for a nonempty list. Keep the actual key guard.

This investigation adds a register-accurate capture boundary, including the distinction between a field snapshot and the comparison the CPU actually used. It does not repeat CP146's conditional allocation, registry or map inventory. CP148 also excludes bootstrapping the late value from language setup: setup itself has an earlier fromAscii dependency and an unclosed writer interval. CP147's canonical process holder supplies identity with Unknown knowledge, not a reached native read.

Only this report was written. No source/test/build, GUI, original execution, live REA retry or mutable database was used. The reverse-engineer-anything skill was applied through existing qualified retained disassembly and immutable PE reads. Root owns capture scheduling and production source changes.

## The missing dynamic values

All addresses below are RVAs in the verified original modules; preferred disassembly bases are GUI 0x140000000 and QtCore 0x67000000. Actual loaded bases must come from the future owned run.

Let S be dispatcher RSP immediately before GUI+0x2cf41f. Static instructions establish the following relationships, but none supplies the actual addresses or values for a selected original occurrence:

| Boundary | Required observation and meaning |
| --- | --- |
| GUI+0x0d2995 -> +0x2cf3c0 | Select the actual derived-manager invocation, nonempty operation and exact key source; discriminate other invocations of the shared dispatcher. |
| GUI+0x2cf41f | RCX=S+0x98 is the QString object slot; RDX is the C-string source. Bind caller return +0x0d299a, notifier identity from [S+0x1a0], saved source [S+0x1a8], thread and occurrence. Record the actual IAT target. |
| QtCore+0x7412e, immediately after CMP +0x74126 | ZF is the actual null-comparison result. RCX still identifies the source and EDX is -1. A separate read of QtCore+0x2f3e10 before/after this instruction is a snapshot, not automatically the operand the CMP consumed. |
| QtCore+0x7418b, only on nonnull arm | RCX is the pointer actually loaded at +0x74184. This may differ from an earlier field snapshot; it is the borrowed codec passed onward. |
| QtCore+0x7247e, only on nonnull arm | R10 is the loaded vtable, [R10+0x18] the actual virtual target, RCX the codec, RDX the output QString slot, R8 the source and R9D the measured length. This is the concrete decoder frontier, not a codec name assumption. |
| QtCore+0x174be7, after helper return and before store | RAX is the helper's returned **data pointer**; RBX is the destination object slot. The following instruction publishes RAX into [RBX]. |
| GUI+0x2cf425, before spill | RAX is the returned **object pointer**, expected S+0x98; [S+0x98] is the completed data pointer. The spill [S+0x158] has not executed yet. Do not interpret old spill bytes as this return, or RAX itself as the data header. |

The constructor preserves its destination in RBX, calls fromAscii_helper and publishes the returned data. Its normal return chain is therefore source-closed; actual return, slot contents, data generation and ref cell remain unobserved. At +0x7412e the helper frame RSP is S-0x70 under these exact prologues, and its saved return address is at helper RSP+0x38, pointing to QtCore+0x174be7. This provides an additional correlation check; record the actual outer S instead of accepting an unrelated helper invocation by stack arithmetic alone.

No examined retained record joins these events for the selected actual nonempty key. Historical editor codec reads, static captures named “call”/“return”, and source-model tests are not that join. This is a bounded finding about the cited DOE/codec evidence, not a claim that every imported artifact has been exhaustively searched.

## Minimal capture, gated before any app-control acquisition

The parent lifecycle assessment reports unsupported_release_api, and `python ../../tools/gui_validation_status.py` again returned release_ready=false. Do not acquire GUI control or launch an interaction to obtain this witness until root verifies a supported release operation and its receipt/status. No control session was acquired here; no release is claimed.

After that gate changes, reserve one original owner and one owned fresh original process with one disposable nonempty Type2 project under C:\PROBLEM (paths shorter than 100 characters). Freeze and hash the exact project/study inputs before the run; retain their source qualification. Use the existing nonempty operation profile, not CP159's empty-list case. Retain process ID plus start/generation, thread ID, loaded module bases/paths/hashes, run ID and event ordinals. Do not synthesize a codec setter, call a decoder manually, modify registers/return values, or reuse a user process.

1. Arm a caller-filtered observation at GUI+0x2cf41f. Bind S, actual source bytes including NUL, constructor destination, notifier and originating manager/reader call. A matching string in another call is insufficient.
2. Observe the same natural helper execution immediately after its CMP at QtCore+0x7412e (or at +0x74133 before JE, since the intervening MOVs preserve flags). Record ZF and confirm the taken edge. This closes null/nonnull selection even if a separately sampled global has a race. A capability that cannot preserve/report the reached flags must report branch selection unresolved instead of substituting a memory snapshot.
3. On null, follow the natural helper/constructor return through QtCore+0x174be7 to GUI+0x2cf425. At the latter, read the actual destination slot, data header/ref cell and bounded payload. The source-qualified null expectation is a fresh 21-unit key with request 74 bytes and initial ref1; do not report the request as an observed allocator call unless captured. Preserve unknown flag/padding bits.
4. On nonnull, add only the conditional observations at +0x7418b and +0x7247e, recording actual codec/vtable/virtual target and mapped-module identity. Pair the natural decoder return at wrapper+0x72482/helper+0x741a4 with the output slot, its data/ref fields, then the constructor publication. Returned length/data/aliasing must be observed; 21 ASCII source bytes do not force an arbitrary codec's result length or fresh ref1. Pointer/vtable snapshots and an actual virtual call do not establish the codec's entire registry lifetime; qualify that remaining owner history explicitly.
5. Stop the evidence window at GUI+0x2cf425, before map lookup +0x2cf454. An exception, missed event, timeout, unreadable data or absent natural return is an incomplete/failing occurrence, with no invented completed key. Capture the exception location/outcome when available. Do not restart the original merely to replace an inconvenient result. Root then ends app-control through the supported release API and verifies its receipt; owned-process cleanup is a separate action, never a substitute for release.

Header reads should be bounded to the native data-header fields and a fixed maximum payload, with the observed length retained. If the returned length exceeds that maximum, mark payload evidence incomplete rather than truncating it into an accepted key. Reading [data+0x10] gives the payload pointer; do not assume an allocation-relative pointer for nonnull decoder output. A new successful key witness would still stop before the independent map/list/receiver frontier and would not complete Type2 Open–Save acceptance.

## Bounded identity receipt

Immutable files were freshly hashed here: GUI `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. PE section mapping was used to hash these end-exclusive file-backed ranges:

| Module RVA range | Bytes | SHA-256 |
| --- | ---: | --- |
| GUI 2cf40f..2cf43d | 46 | fe640436067bbae2d69f2f601e886f1c1aa90a5aa18e980cdbf9c6c3072f20ae |
| GUI 0d2987..0d299f | 24 | c3b444cdb45ac44cfbe524875a54b8841598246aad41d7edbdb20aa4762cd444 |
| GUI 2166b40..2166b56 | 22 | 7f87e73d9c9c945cc51722b661daac8b5a495237c93c9b125186d393b05a6941 |
| QtCore 174bd0..174bf3 | 35 | c0d51f6f9935e26fddcd285653db8c9f5c6f007ef70663857b9cac7f420d311b |
| QtCore 74120..741d5 | 181 | 582c5dacfd3f6edb8acfe48a648f52369b8fa88e8a45de00700dd9a4ab71f3de |
| QtCore 72460..7248b | 43 | d3833d1fc44590ec3dd3863a1d78c7c813ab370266fa48123000adf04eded745 |

Retained decoded bodies rehashed: `resume81-doe-qstring-source-67174bd0.asm` = `39562aa6405de1900dd8e7d5263aa6828b3f457d98cade572b6d5fcd070149e2`; `resume81-doe-qstring-source-67074120.asm` = `0dcd3d48f97c3f157d4df4bc9fb5d65bb99cf0690d8bf04d8e667a3296174f53`; `resume81-doe-qstring-latin1-67072460.asm` = `5b651fe2093e6f98930c3d7d424516af73d747ebd9da1bbcc3f583e62c6570c5`. Caller/dispatcher retained hashes match CP146: `9c0786431b4f422f21b7fced854e244293fdcf9493b1d651a53cac9b4a2ee078` / `9ab15e4a88c891082cbfc05d07d01893d082aa204330970c893e783773dc3b64`.

Next action: keep the production guard and queue this single branch-to-publication observation behind verified app-control release capability. No additional static setter/zero-fill scan or inert holder extension can replace its unobserved dynamic operands and data owner.
