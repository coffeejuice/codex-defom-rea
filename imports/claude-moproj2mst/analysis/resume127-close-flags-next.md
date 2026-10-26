# CP127: OPI+28 flags-wrapper null skip

The next source-closed action after CP126 is an explicit actual `Operation::recoveryFlags` pointer-null test. Native05073b skips both flags-wrapper deletion and the owner-null assignment, reaching05078f immediately before the+160 item-model guard read. Nonnull containers, including allocated empty flags, must remain refused before the direct deleting-wrapper call. This report proposes that private continuation only; no source, test, CMake, build, GUI, original execution or mutable REA database was changed or used.

## Immutable evidence and exact order

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses below use the common140 prefix, so050736 means140050736. Retained `analysis/resume56-opi-cache-140050690.asm` SHA-256 `7bacc92563c959bafabed471fdc83c0efb102cc39a030060429d0dbab12a5a14`, bounded DUMPBIN14.44.35228.0 and section-mapped PE byte reads establish:

1.After the+20 cleanup/skip,05072e reloads OPI.050736 compares qword[OPI+28] with zero;05073b jumps to05078f if null.
2.For nonnull,050745 loads the wrapper and copies it to locals. After a redundant local-null check,05076a calls **direct typed deleting wrapper065400(pointer,1)**. It is not virtual dispatch.
3.065400 calls2081a0 at065412.2081a0 reads the wrapper's first qword and calls IAT141e042e8 at2081b1. The PE import table labels that slot `QtCore4_64.dll!QBasicAtomicInt::deref` (`?deref@QBasicAtomicInt@@QEAA_NXZ`). If the returned AL is zero,2081cb calls025650; otherwise it skips that call. The backing-store cleanup target is not reconstructed here.065400 then tests flags&1 and calls14dfc68 at065427 when set; this caller supplies1.
4.Only after065400 returns does050787 assign OPI+28=null. Thus the owning field stays nonnull during destructor/deallocation. The null branch05073b bypasses this assignment as well as deletion.
5.05078f reloads OPI,050797 selects its+160 guard address, and05079e callsC1AEF0. The proposed increment stops at05078f **before this guard read**, preserving item-model knowledge as unavailable. Downstream,0507a6 skips to05080e when the guard result is null; nonnull reaches virtual+18 at0507e5 and only afterward callsCBC920(null) at050809. None of those+160 decisions/effects belongs to this flags step, and05080e's function return alone is not QObject destructor completion.

End-exclusive byte-range hashes:

| Range | SHA-256 |
| --- | --- |
|140050736..14005078f|`c78f70647d73644ad2b943afac6bebf07068d326d15e3def859c7952980351db`|
|140065400..140065436|`eb014d601ec4d1980a3c03c1d61bdb291ac3733fc23159eca3b4eb54fcb04846`|
|1402081a0..1402081d5|`46be1cd735a1fc9779b69b9532307b09bd53f082d563be4dffc807267889c6ff`|
|14005078f..140050813|`7519150bc60d3df060877b1c8b1dac76800c4c1c69fe816077c440d41297abc4`|

These are offline original-image qualifications, not new REA Evidence IDs or live accepted-Close witnesses.

## Actual owner and limits

`source/apps/moproj2mst/include/m2m/model.hpp:200` represents native+28 with the actual `shared_ptr<vector<bool>> Operation::recoveryFlags`. A bare Operation leaves it null, but `operation_rescue.cpp:201` allocates it in operationDefaults during ordinary initialized creation. Bare Operation also allocates recoveryB, which already stops CP126; an isolated declared null-B fixture is required to exercise this next null-flags branch. No ordinary Open/Save failure is established to supply the complete null-prefix state.

The native branch tests the wrapper pointer, not entry count. An empty allocated flags vector is nonnull and enters destruction. Shared_ptr reset would change field publication timing and can retain aliased storage; vector<bool> destruction does not reproduce the source Qt atomic/backing-store path merely because it is empty or uniquely owned. Implement neither nonnull deletion nor scalar/content inspection in this increment.

Read source hashes: model.hpp `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`; operation_rescue.cpp `88cdda9dd7eda054b1ead2b9ea994b47a423316cfa947c270d96d623302456b3`. CP126 cleanup header/source are `4231a58f2bbffa7f435860e835b6e6c425c7c067349986a8b98c513659c79bbc` / `52f811446f517211889cbd6d83697567b20286b9a876354b0858057b7ee941bf`; its receipt is `analysis/resume126-opi-recovery-null-prefix.md` SHA-256 `e497884631bd80ee90d286dff1a0de071dd8ae7df4cd5581591a9556925e8fe0`.

## Proposed private API and focused tests

After explicit root assignment, add `attemptOperationInfoRecoveryFlagsCleanup(Operation&, OperationInfoCleanupProgress&)` in the same exclusive existing header/source/test. Require this progress's RecoveryFlagsCleanupBoundary, reached by every explicit previous step including CP126's null-B skip. Validate the same live OPI generation before fields, then revalidate Props/cache absence, empty object/position lists and null recoveryB before reading the actual recoveryFlags pointer.

Null advances to a proposed ItemModelCacheBoundary and returns NullRecoveryFlagsSkipReached without reset, delete, assignment, entry inspection or+160 read. Nonnull records only a weak canonical owner plus opaque uintptr_t token, latches RecoveryFlagsDeleteBlocked before05076a and returns NonNullRecoveryFlagsDeleteUnavailable. Append enums to preserve old values. Old APIs retain their opt-in stop at RecoveryFlagsCleanupBoundary; after explicit advancement they revalidate/report the reached guard without replay. Stable retries remain guarded; reset/replacement, same pointer with another control block, reappearing flags after a null skip, or reappearing predecessor owners refuse. Do not retain the flags allocation or expose a dereferenceable historical pointer.

Reserve only `include/m2m/engine/operation_info_cleanup.hpp`, `src/model/operation_info_cleanup.cpp`, and `tests/mdt/test_operation_info_cleanup.cpp` beneath source/apps/moproj2mst. No model.hpp or shared Session/main interface edit is needed. Existing target: `m2m_mdt_operation_info_cleanup` (currently32 Testkit cases); root owns build/test/freeze.

Focused cases should cover explicit opt-in/prerequisite order with declared null fixtures; actual null-flags advancing only to the+160 read boundary with current/OPI/list storage unchanged; allocated empty and populated flags refusing without entry/ownership changes; stable retry and reset/replacement/control-block mismatch refusal; no strong allocation lease; late flags/B/earlier-owner reappearance; foreign/expired OPI rejection; and earlier APIs preserving their original stop. Keep Release-active Testkit checks/main/includes and no C assert. An optional independent OperationInfoModelCache fixture should remain Unknown across this step; no default-null commit or lookup is authorized by the flags skip.

## Remaining dependency

The later+160 state cannot be inferred from flags nullness, engineModel(+38) absence, or a standalone diagnostic cache receipt. Existing `OperationInfoModelCache` explicitly distinguishes Unknown from a source-qualified defaults event; it is not by itself a bound actual native+160 owner producer. Its eventual cleanup integration requires that same-OPI producer/lifetime contract and must preserve noncreating guard identity versus destructor expiry. The source-qualified next stop is therefore before05079e, with no automatic+160 null admission.

Nonnull wrapper cleanup, later members/QObject expiry and registry/editor effects remain guarded. The actual accepted Close application/current-main/busy/modal/nested Save and Project teardown chain is still required independently. This conditional private prefix cannot authorize public Close or adapter failure unwinding. Only this report is released; no source implementation or tests ran.
