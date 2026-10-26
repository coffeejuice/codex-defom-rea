# CP135: isolated map-destructor proxy-free frontier

The next source-closed behavior is one actual proxy A free, followed on nonzero kernel success by the standalone map destructor's return. There is **no M.proxy=null store**. After success A and H must both be retired and every stored address treated as opaque history; no later API may dereference either block. This closes only the explicit isolated map destructor, not C/Project/Close or a constructor EH occurrence.

This receipt is read-only: only this file was written, with no source/build/test, GUI/original execution or mutable REA database. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses use the common14 prefix. Retained captures were checked with bounded DUMPBIN14.44.35228.0 and PE bytes/imports.

## Qualified next free

Standalone map destructor12f0cf0 calls inner cleanup12f0860, loads A from M+0 at12f0d0c, restores its frame, and tail-jumps to14dfc68 at12f0d14.14dfc68 tail-jumps to1471b7c. For actual nonnull A, that routine reads the current CRT heap144700810, calls HeapFree(heap,0,A) at1471b92, and on nonzero returns directly at1471bb8. The tail return completes this map destructor. It executes no M.proxy clear, provider deletion, HeapDestroy, context callback or Project action.12f5e30 has a similar proxy-only tail but is a distinct constructor-EH cleanup occurrence; current guards are not evidence it ran.

Actual zero instead enters the still-guarded errno path at1471b9c. Preserve raw BOOL, one-shot latch and pending error continuation; do not call host free/errno or replay. The successful sentinel-free prerequisite must be real: a pending/failed H free cannot enter the proxy free. Use the actual retained A and current canonical heap, with source-owned identity/provenance checks before the call; no arbitrary handle/pointer injection.

New proxy report identities should be uintptr_t tokens. Authenticate A's backpointer only while it is still live, then consume a retry latch before the kernel effect. After success, do not dereference A, read its backpointer, clear native M.proxy, or treat the stale field as a fresh capability. Keep host C/M backing/receipts retained but mark the isolated source map lifetime complete. Old constructor/clear/sentinel/context methods already have early stage/latch exits; proxy retry must also exit before any A/H validation. Context/Project admission remains unavailable.

Future isolated API: `FeatureMapProxyCall::resumeProxyFreePrefix()`. Reserve existing private runtime header/cpp and two model tests only when root releases CP134 source ownership. Positive actual tests should require nonzero free, verify current canonical heap/A/flags0, source return/no null store, one actual call, opaque historical address, no H/A dereference, unchanged allocations/context history and no replay after all inspection leases end. A declared zero-result model should have no real heap/frame authority and no map-destructor return claim. Do not inject invalid pointers, OOM or double-free.

## Error-path owner is separate

Bounded new decoding refines the missing error dependency.1474134 calls14814e8 at1474138. A nonnull result yields its+10 address; null yields fallback143b5e2c8. It cannot be replaced by the candidate host CRT errno pointer.

14814e8 first calls GetLastError at14814f2, reads module FLS index143b5e990 at14814f8, saves the error in EDI at14814fe, then calls **FlsGetValue at1481500**. Missing thread data can allocate0x2c8 bytes through147e440, call **FlsSetValue at148152c**, initialize thread state and GetCurrentThreadId, or free the temporary block if registration fails. The common tail calls **SetLastError at1481558** with the saved error before returning the thread-data pointer. PE IATs141e03430/03438/03448/033e8 bind FlsGetValue/FlsSetValue/SetLastError/GetCurrentThreadId.

Thus native provider completion preserves its incoming LastError before the outer free routine calls GetLastError at1471ba4. But the current candidate runtime binds only the CRT heap role, not the module FLS index/allocator/thread-data/fallback lifetime. Old parked failures did not capture the same-thread failure error before host return, so their later current LastError cannot stand in for this original value. A newly admitted failed-free occurrence could in principle execute the source provider's first GetLastError at14814f2 and stop before the unbound FLS-index read; that requires integrating it in that same occurrence, not retrospectively promoting an existing paused report. Full errno completion remains blocked by actual FLS/PTD owner and error-state continuity.

## Evidence receipt

| End-exclusive original range | SHA-256 |
| --- | --- |
|1412f0d0c..1412f0d19|`a4181b8d1fdee4f6f85389f8aad0ec4f961d1745d5ce97ac4de93a2eaebea9b0`|
|1412f5e30..1412f5e38|`af3275b3cd81cb8f5dc72c37d034ae083d186771d4b7e4109c462c6f6a927c6e`|
|141471b7c..141471bb9|`f9cc22f367e3f55d00ae8f63f4891c223244b13da199bfd53fb0fef7c52dd696`|
|141474134..141474154|`82fd11be9f0076f075cafa1b9e5c1466415a7f017d285cda92bb723d53800482`|
|1414814e8..14148156c|`191922765c03256b33eb9d0b8c15ddbbd206a54449906a4f86409a1a11ffebfa`|
|1414814e8..1414814f8|`244c4e0eccf2515c596aca88a34d0acab41228f5988b1a4f8da220e8addfcd84`|

CP134 implementation receipt `analysis/resume134-feature-sentinel-free.md` SHA-256 `c4b4b53c5a607c6fb41155b8386857fdb7e8c32225bb85f1f5e79e18dbd565fe`; inspected runtime header/cpp `b2fb04cb60a05a06188264456075a2b1f2700bc65087b388c011677a6be7ad54` / `c02fb82749f1cee9c552d4d2216856893c8cf8021f41cbfb25fd04317eb0a01d`. This is a source qualification, not a build or runtime receipt. No reconstructed executable/deployment was inspected or changed.
