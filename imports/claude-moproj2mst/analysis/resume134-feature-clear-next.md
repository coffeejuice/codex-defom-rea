# CP134: isolated sentinel-free success continuation

One source-closed continuation exists after CP133: attempt **one actual sentinel HeapFree on the same canonical candidate CRT heap**, then, only on a nonzero actual result, execute the caller's M.head=null and M.count=0 stores. Stop before proxy free. Actual zero must park inside CRT free before its errno-provider call; it is not a completed native error return or permission to replay. No Project/Close promotion or full context destructor is needed for this explicitly isolated map suboperation.

Only this report was written. No source/test/CMake edits, builds/tests, GUI/original execution or mutable REA database. Evidence is immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained captures and bounded DUMPBIN14.44.35228.0/section-mapped PE reads. Native addresses below use the common14 prefix.

## Exact call, result and store order

CP133 ends at12f08ac loading H from actual M.head, before12f08b0 calls14dfc68. Newly rechecked14dfc68 is a direct tail jump to1471b7c.

1.1471b7c tests the pointer argument. Null jumps directly to return1471bb8 without a heap read. **This shortcut is not available here:** successful sentinel construction and completed empty clear require the real nonnull allocated H. Do not null the argument to avoid deallocation.
2.For nonnull,1471b86 moves the pointer to R8,1471b89 reads the **current** heap slot144700810 into RCX,1471b90 sets EDX=0, and1471b92 calls IAT141e032f8. The PE import table binds that slot to KERNEL32.dll!HeapFree. It uses the current module heap role, not a caller-injected handle, process heap or remembered arbitrary allocator.
3.1471b98 tests raw EAX. Nonzero1471b9a ->1471bb3 restores the stack and returns1471bb8 with no errno/GetLastError path.
4.Zero first calls **1474134 at1471b9c**, retains that returned address in RBX, then calls IAT141e035e8 at1471ba4 (PE-bound KERNEL32.dll!GetLastError), maps the error through14740ec at1471bac and writes EAX to[RBX] at1471bb1 before returning. That provider/mapping state is unrepresented. Stop before1471b9c on actual zero; do not call host errno/GetLastError early just to populate a diagnostic, since native obtains LastError only after its provider call.
5.Only after the source free routine returns does12f0860 zero a register at12f08b5, store **M.head=null at12f08b8**, then **M.count=0 at12f08bc**, and return12f08c5. Even though count is already zero, this second source store remains part of the caller continuation.
6.Standalone map destructor12f0cf0 next loads A from M.proxy at12f0d0c and tail-calls14dfc68 at12f0d14. The smallest proposed cut stops **before that call**, retaining A/M/C/runtime. There is no native M.proxy=null assignment in that tail. Normal context destructor12f0d20 has a different proxy-free tail after its own optional-member phase; do not claim that caller ran.

Native free is not modeled as a Boolean-returning map destructor: HeapFree's raw result selects its internal branch, and later caller stores depend on completion of the CRT wrapper. Actual zero does not imply native Close aborts. It means this staged candidate cannot execute the remaining error path yet.

## Bounded actual-owner API

After root assigns exclusive source, add explicit `FeatureMapProxyCall::resumeSentinelFreePrefix()` and a separate nested free report/cursor. Minimal files are existing src/model/feature_crt_runtime.hpp/.cpp and the two existing tests/model/test_feature_crt_runtime.cpp and test_feature_context_prefix.cpp beneath source/apps/moproj2mst. No Project/model.hpp/Session/public ABI/CMake edit is needed.

Require the actual same retained canonical runtime, completed CP133 empty-clear cursor at the sentinel-free guard, actual M.head==the recorded live head allocation H, and no earlier free attempt/retirement. Keep owner-before-call locks and authenticate current candidate heap/module lifetime. Reread the current candidate heap slot at this new free occurrence; use that current value for the source call. If heap-role/owner provenance is no longer qualified, refuse before the effect rather than substituting an old allocation-time handle or guessing native behavior.

Prepare every report/ledger/host storage item before the source phase. Latch the one-shot attempt before the kernel call, record actual heap/flags0/H arguments and raw BOOL return, and do not allocate or dispatch callbacks inside the phase. On actual nonzero:

- The sentinel storage is retired. Never dereference H again, including for validation, repeated report calls, tests or subsequent cleanup. Historical opaque identities/previous known values may remain in receipts, but are not live allocation capabilities.
- Execute M.head=null and M.count=0 in that order after the free wrapper's qualified success return. Retire the internal headAllocation ownership metadata without adding a source store into freed H or prematurely clearing M.head. Preserve A and its M backpointer, all constructor/context receipts and their unknown fields.
- Record the map-inner destructor return and load A for the next proxy-free boundary if modeling those pure operations, then stop before12f0d14. No HeapDestroy, proxy free or automatic context/Project cleanup.

On actual zero, record an error-continuation-pending boundary before1471b9c. Do not execute M.head=null, repeat the free, assume errno/LastError values or certify allocation retirement. Retain/quarantine prior owner metadata at this partial-effect boundary; do not inspect uncertain storage. A declared false-result projection can test order without becoming an actual allocation/free witness. Kernel OOM, invalid heap handles, corruption and intentional double-free are inappropriate negative fixtures.

Existing CP133 clear/context exclusion remains sticky: clear resume cannot reread/rewrite H after a later free attempt, and context prefix cannot be adopted/replayed. Old8/48-byte APIs must never recreate a block. The old empty-clear report's static sentinelFreeEntered=false describes that earlier phase only; a new actual-free report must make the phase distinction explicit rather than relabeling historical receipts as live state.

## Focused validation design

Extend both actual retained-frame tests after their current clear guard. Require actual nonzero HeapFree as the positive branch prerequisite; environmental zero must be retained/reported as pending, not converted to a passing fake result. Verify exact current canonical heap/H/flags0, one actual call/result, free-return before head-null/count-zero, same M/A/C and no proxy free/HeapDestroy, no change in8/48 allocation counts and context55-store history/unknownness. After retirement use only host metadata, never raw H reads or post-free heap queries. Repeated free/clear/old allocation/context resumes must not replay any effect; dropping inspection leases must preserve the remaining owner and pending state.

A separate declared zero model should prove the guard precedes errno-provider/GetLastError and caller stores, with no fake real handle or source-return claim. Existing target names stay m2m_model_feature_crt_runtime and m2m_model_feature_context_prefix; root owns execution/freeze. No tests ran in this read-only lane.

## Reproducibility and remaining admission

| End-exclusive original byte range | SHA-256 |
| --- | --- |
|1414dfc68..1414dfc6d|`6648cd6b98e65e1617c9f4cab20900bf9382741e32b04fc6be2d054aef811bde`|
|141471b7c..141471bb9|`f9cc22f367e3f55d00ae8f63f4891c223244b13da199bfd53fb0fef7c52dd696`|
|1412f08ac..1412f08c6|`698e5bbad78fbf708bf9d2a445c90a515508e84d8dc1c1eef8af9db837b1c9aa`|
|1412f0cf0..1412f0d19|`4943ebf4417fcb30e864b0b172dad1017561242facc0631e0dc4299717c0275c`|

Retained free capture analysis/resume119-feature-allocator-141471b7c.asm SHA-256 `d8ad91d7c47a3f6610809dee457c102415bbc6b5550267d3a8b96860b3b597af`; CP133 receipt `8e54d157d46bebcebf74bf2729cbd5c7597db29412cd68edabeabc51bb02ee86`. Inspected runtime header/cpp `1164febe2b9befdeb49c39bde91ed5fc52c2f4fef50fc487a9d1c09604838a73` / `7f26fa734a47931b29e8829133e8e1e3ac4b0d4c038a36f9b46a5b77ccfc00f3`. CP119/125/127 retain heap producer/module-lifetime/error qualifications; CP131 retains destructor caller distinctions.

Proxy free/error continuation, full context strings/optional-owner/provider lifetime, native EH, Project admission and accepted Close's application/main/busy/modal/manager chain remain guarded. This isolated owned-frame action must not be installed in a shared_ptr destructor, inspection-lease cleanup or default conversion path. No reconstructed executable/deployment was inspected or changed.
