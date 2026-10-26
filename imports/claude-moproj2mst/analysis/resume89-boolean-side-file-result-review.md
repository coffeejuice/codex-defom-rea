# CP89 — independent Boolean side-file result seam review

Read-only source review after the implementation freeze. No source, build, test, fixture, GUI, original execution, or mutable REA operation was performed by this reviewer.

## Conclusion

The result seam matches the retained native Boolean helper contract for its declared operation/property/data profile:

- Native `BooleanData+0x10` is the owning `SfOprBooleanProperties*`; its `+0x20` QPointer resolves the operation item. Missing data owner or null/expired operation guard returns `-1` before either file attempt.
- For a valid owner, BWZ is attempted first; its `0`/`1` result is saved; INI is attempted once even when BWZ returned `0`; and the saved BWZ result is returned.
- Open failure is a returned `0`, not an exception. Successful open takes the selected close path and returns `1` without checking stream sticky status. The void Save slot discards the result, so it does not add a public error status or alter downstream Save continuation.

I found no order/result mismatch in the implementation. There is one malformed-alias boundary to retain: candidate `BooleanData` has no owner backpointer, so a manually shared data object referenced by more than one `BooleanProps` cannot be proven to have the same canonical owner as native `data+0x10`. The ordinary Boolean factory creates a fresh data object for each new `BooleanProps`; the current production writer passes the data from that same property object. Thus this is not a normal-path defect, but the helper's standalone API should remain qualified to that one-to-one actual pairing unless it adds an explicit uniqueness/backlink guard.

## Evidence and source comparison

Original `DEF_GUI_MO_64.exe` is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained `s1-04/spec.md` §1.1/§1.2 and `wave3-boolean-lifecycle-failure-contract.md` qualify:

1. `1411e1820` tests `BooleanData+0x10`; if absent, returns `-1`.
2. It resolves that property's `+0x20` QPointer through `0x1405f8d90`; if null, returns `-1`.
3. It calls BWZ `0x1411e1970`, stores its result, calls INI `0x1411e0e40` unconditionally, then returns the stored BWZ result.
4. Each file writer returns false on QFile open failure without creating a directory. On a successful-open path it closes and returns true without testing QTextStream's sticky error. The caller ignores the returned result.

Candidate `BooleanData` omits the native `+0x10` backpointer; `BooleanProps` owns `shared_ptr<BooleanData>` and inherits `Props::owner`. The new helper reconstructs a normal bound relation by requiring `expected.props` to be `BooleanProps`, pointer-equal `properties->data == &data`, `properties->owner` to be a member of the supplied `Project.operations`, `owner == &expected`, `expected.project == &project`, and `owner->props == properties`. It checks Project membership before dereferencing the raw owner pointer. It then reads the existing `OperationInfoIdentity` guard; a committed QObject boundary can make that guard null while the raw owner remains set, matching the native null-target early return as a candidate projection.

The normal factory path `BooleanClass::makeProps` creates a new `BooleanProps` and a new `BooleanData`; `setOprProps` assigns that property object to one operation and sets `Props::owner` to that operation. `l5_slots.cpp` obtains the same BooleanProps and passes its `*bp->data` to the writer. These source relationships support the intended ordinary call profile.

## Results, ordering, and callback boundary

`nativeBooleanSideFileSequence` performs all owner checks before inspecting callbacks or invoking them. Both callbacks are required before the first attempt. The nested four-pair tests verify exactly `BWZ, INI` order and return the BWZ enum's integer for all `0/1` combinations. C++ callback exceptions propagate; they are not converted into QFile open failures or followed by an invented INI attempt after a BWZ exception.

`booleanSideFilesNativeResult` first requires `ctx.nativeMstSideFiles`, then uses `FileDevice` at final paths. Its emitter returns `OpenFailed` if the file did not open; otherwise it serializes, explicitly closes, and returns `OpenedPath` without checking stream status. BWZ and INI serializers are shared with the prior staged path. The existing void `booleanSideFiles` delegates to this result helper only in native mode and discards the integer. Recording/staged mode remains separate and does not infer native result values.

Test coverage is appropriate for this boundary: both callback result pairs; missing props/null owner/expired OPI guard; foreign project/owner and data/props mismatches; missing callbacks; exceptions; opened-path despite a synthetic stream failure; physical BWZ-open failure with INI continuation; physical INI-open failure retaining BWZ result `1`; both opens failing/missing parent; and void-slot versus staged-mode behavior. Disk cases are contained under configured test scratch and use real candidate Project/Operation/BooleanProps/data relationships.

One fixture-hygiene note: `DiskFixture` creates unique PID/serial scratch paths but has no destructor cleanup. The paths remain under `M2M_TEST_SCRATCH`; the test itself refuses a preexisting path. This does not affect behavioral assertions, but root may prefer an RAII cleanup guard if repeat-run scratch accumulation becomes operationally material.

## Owner-alias qualification

The test rejects a foreign supplied data object, but not this manually assembled shape:

```text
Project.operations contains Operation A and Operation B
A.props = Props A; Props A.data = shared BooleanData D; Props A.owner = A
B.props = Props B; Props B.data = same D; Props B.owner = B
call nativeBooleanSideFileSequence(Project, B, D, ...)
```

Current checks accept B's internally consistent operation/property/data links. Native `D+0x10`, however, is one property pointer, so this shape cannot faithfully identify both Properties A and B as owner of D. Production creation currently uses one new D per Props and the L5 path passes that pairing. To make the helper's malformed-model rejection total, either add a source-backed unique-owner invariant/scan for the supplied `data` within `project.operations`, or explicitly keep shared BooleanData aliases outside this helper's qualified profile. Do not silently treat an arbitrary alias as the native null-owner `-1` branch.

## Timing and acceptance boundary

The new helper does not modify `convert.cpp`, the L5 slot signature, `ops/boolean.cpp`, C ABI, public first-Open admission, event scheduling, or any timer callback. It does not create Boolean engine objects or decide when Save/Open reaches this writer. Its result applies only to the local side-file helper call; no public Save result is derived from `-1`, `0`, or `1`, and no first-Open/PostPending parity is claimed.

The implementation report is `analysis/resume89-boolean-side-file-result-implementation.md`. Frozen source hashes recorded there are: header `5a13d8e782801f6a88bed9bf18226d1da98ec42c591240d1dffe6731b29e80ed`, result implementation `cf8d9eee2ab0b042d7418617554cf4ba7e06dd90132bf0dd8dba50e5e6941330`, Boolean editor `764db1428155c2770423f23a854354620383cb3e2a65b14f7659b437f117dd6e`, and test `e85c2e7d111177bce8619bc624402062e732669ba6df88452a6c4d04981d4fae`. Root reported the grouped focused build and six target tests passed; this review did not run or independently reproduce them. Full-suite and source-stable checkpoint acceptance remain root-owned.
