# CP82 independent review: QString resource-owner source

## Assessment

The null-codec test profile creates one canonical `QStringPathOwner` record with reference word **1**, then adopts that same record into the destination lease without incrementing it. Later path aliases use the same `impl_->owner`, so they advance the same counter (`1 -> 2 -> 1 -> 0`) rather than copying it into a parallel owner. The implementation preserves Unknown, Null and NonNull as distinct codec states and gates allocation/decoder work at the corresponding boundary.

This is still an isolated, caller-created process **profile**, not a production process/Session owner or a live GUI receiver admission. The public source entry deliberately always refuses because the actual Session/receiver occurrence is missing. No QFileInfo/private prefix, icon `addFile`, conversion session, or public DOE/Save call site is connected. The implementation therefore must remain a candidate internal profile and must not advance the existing QFileInfo guard.

## Requested checks

- **Source-origin count 1 and one canonical owner:** `QStringPathOwner::constructFixedResourceInput` privately creates the resource record with origin `ResourceNullCodecProjection`, fixed resource text and one atomic initialized to 1, stores it once in `records`, then publishes an active destination lease using `increment=false`. The resource owner receives the returned handle and stores it in that same `QStringPathOwner`; later alias/release calls delegate to it. `test_qstring_resource_owner.cpp` verifies initial 1, alias 2, alias release 1, and final source release 0. The shared-null owner remains absent; count 1 is not copied to the distinct shared-null identity.
- **Codec Unknown/Null/NonNull gate:** default is Unknown. An occurrence captures the current state once and records epoch at `readIsolatedCodec`; stale pre-read tokens refuse first. Unknown records its reached guard then refuses before allocation; NonNull records its selected branch and refuses before a virtual decoder; Null advances to the source allocation stage. A later profile change does not rewrite an already-selected Null branch, while the next occurrence observes the new epoch/state. This matches the qualified branch shape as a test profile; the state source is still `setIsolatedCodecProfile`, not the native codec global at RVA `0x2f3e10`.
- **Allocation and publication:** `ReturnNull` records request size 104 and `AllocationReturnedNull`, then refuses without data, code units, destination lease or constructor-return state. On modeled success, the record is initialized (including fixed text and ref 1), inserted into the canonical owner, and only then the destination source slot is adopted; the occurrence receives its data/field snapshot and `ConstructorReturned` afterwards. The test checks both success publication and the no-publication allocator-null branch. Host metadata allocation failure is not injected: after the simulated native allocation succeeds, an exception while constructing/pushing host metadata leaves the stage at `AllocationAccepted` and no constructor return. The receipt should preserve that as an incomplete adapter prefix, not call it a native rollback or a fully modeled host-allocation failure.
- **Process versus Session scope:** one `QStringResourceProcessScope` object owns one codec snapshot and path owner; this is preferable to a host static and matches an explicit isolated process-profile boundary. But each public constructor creates an independent scope, with no enforcement/injection of one shared context across Sessions and no revocable Session/Project/ModelManager binding. Two scopes may each select Null and construct equivalent resource records. This is safe only while they represent distinct diagnostic process profiles; it does not yet prove that multiple Sessions in one reconstructed process share one codec/ref owner. `constructSourceResource` always raises the specific missing-receiver guard, even after an isolated test construction.
- **No accidental QFileInfo/public admission:** source references to the new API are contained in its implementation and focused test. The actual-source constructor is a refusal stub; there is no call from `DoeFileInfoPrivateArena`, `QFileInfo`, `addFile`, conversion/session code, or public Type2 Save. `QStringPathOwner`/`QStringResourceProcessScope` remain standalone.

## Recommendation

Accept this as a test-only resource-constructor branch projection, with the process/Session and receiver limits stated directly in the implementation receipt. It establishes the native source's count-1 initialization rule for the explicitly modeled Null branch and reuses one owner cell afterward; it does not establish that the original runtime codec was Null, that this actual GUI receiver reached the resource literal, or that a fresh process has this owner. Before production advancement, bind the codec to the one externally owned process state, make Sessions borrow that same owner, and mint source occurrence admission only at the qualified receiver. Keep `constructSourceResource` refusal and the QFileInfo/public guard until then.

## Snapshot and hashes

Read-only review snapshot:

| File | SHA-256 |
|---|---|
| `include/m2m/doe_qstring_path_owner.hpp` | `61D028F5F9FD80DF03C2A6DDE9EB4B78C4228FE670E20F9076EABB945E39EFE1` |
| `src/doe/qstring_path_owner.cpp` | `AE63E9C8E5C1B72B26A09D5D682C8E3EC1C7721C95EABC415C3923C416023BC4` |
| `include/m2m/doe_qstring_resource_owner.hpp` | `1F00CFAF4FFB8A03ED0A9D6C37FC712DD3784E86E2B5387C7E39E3620209568F` |
| `src/doe/qstring_resource_owner.cpp` | `D85A2EDB00084A5DAF2E14AF3B93B4974ABBDED1585B93057BEB0455178158D9` |
| `tests/doe/test_qstring_resource_owner.cpp` | `F0208CE1B1D6217CD65A2CC5AA83927B34FF14FB25BE967C36C3B7AA5D0741FD` |

No source edits, builds, tests, GUI actions or mutable REA operations were performed. This report is the only written artifact.
