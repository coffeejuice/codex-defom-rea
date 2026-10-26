# CP124 independent review: feature CRT heap producer

## Result

The actual Win32 call sequence and failure/retention boundary match the retained producer evidence for this isolated candidate experiment. One receipt naming issue should be corrected before these fields are reused as evidence: `sourceSlotObserved` sounds like observation of the original executable's global slot `0x144700810`, but the implementation writes and observes only its candidate-side `Storage::sourceHeap` member.

## Call arguments and branches

Retained original evidence is bound to `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the bounded producer ASM is `analysis/resume119-feature-heap-producer-141484800.asm`, SHA-256 `a9efa00fe5209dffb9572fbeb0b604120b79a7a42d410255a199d00d0ca098bd`.

The original pushes `HeapCreate(0, 0x1000, 0)` (`:808–812`), stores the returned handle to global `0x144700810` before testing null (`:818–81f`), returns zero immediately for null (`:822–847`), and only for a nonnull handle calls `HeapSetInformation(handle, class 0, &DWORD(2), 4)` (`:824–83c`). It ignores the compatibility Boolean and returns one (`:842–84b`).

Candidate `attemptProducer()` uses the same three HeapCreate arguments and exact ordered effects (`feature_crt_runtime.cpp:123–136`). It stores the return before null handling, skips HeapSetInformation after null, stores the DWORD value 2 and passes enum value 0 and length 4, and returns a producer result of one regardless of the actual HeapSetInformation Boolean. A null result is retained as a terminal producer result; it is not retried. Repeated `attemptProducer()` calls are idempotent by stage check (`:119–121`), not by substituting a new heap. The source producer makes no retry either.

The private static shared owner intentionally survives local leases, has no static destructor, and never calls HeapDestroy (`feature_crt_runtime.cpp:108–112`). The implementation pins the host module containing `moduleAnchor` before allowing the producer (`:94–105`); that is candidate lifetime instrumentation, not an original call. This retention is coherent for the stated standalone test process and does not claim native CRT teardown. The focused test exercises one actual candidate-process HeapCreate and compatibility call, checks repeat idempotence and local-lease retention (`test_feature_crt_runtime.cpp:75–116`). Declared null/false compatibility cases are correctly kept in a separate pure projection and are not passed off as actual Win32 outcomes.

## Actionable receipt qualification

In the header, `FeatureCrtProducerReport::sourceSlotObserved` (`feature_crt_runtime.hpp:32`) is set by `recordSlotPublication()` (`feature_crt_runtime.cpp:27–37`) after storing the handle in `Storage::sourceHeap` (`:83, :126–127`). That assignment is a candidate model slot. It does not write or read the original executable's global `0x144700810`; the experiment executes a new HeapCreate in the test process and gets a distinct host heap. The source comment at `:83` even states that physical initialization is not an observed source slot, which conflicts with setting the report bit true immediately after the candidate assignment.

Rename or qualify this bit as candidate producer-slot storage/publication (and its tests) before downstream code treats it as an observed original slot. Keep the report's actual kernel-call fields and `origin == ActualWin32` scoped to the candidate Win32 call. Likewise, `moduleIdentity`/`processId` identify the candidate test's code module/process, not `DEF_GUI_MO_64.exe`'s current live CRT owner. This is a claim-labeling defect; no HeapCreate/HeapSetInformation argument or control-flow discrepancy was found.

No source, build, GUI, or REA state was changed or executed by this review.

## Frozen reviewed source hashes

- `feature_crt_runtime.hpp`: `49A8B79A4D01EC446B2ED10F528847353695BBE32C9C779DE7B3A1BC6E7F8565`
- `feature_crt_runtime.cpp`: `BF4B7AF1447D0948E83855C0D5D9DE0C970F1BD3957B5BAEBDD75C5EA1941751`
- `test_feature_crt_runtime.cpp`: `00A6B4EE4C0991BEFFAF612C7439E2FC576500B482A3C65D94DB62EEAE393AA5`
