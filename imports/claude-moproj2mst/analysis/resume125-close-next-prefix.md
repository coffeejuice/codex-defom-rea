# CP125: next OPI recovery-wrapper null skip

The smallest closed successor to CP124 is the actual OPI+20 **pointer-null** branch at1400506d5. It can advance a private cleanup cursor to the +28 condition without deleting, resetting or assigning the recovery owner. An allocated empty recovery container takes the native nonnull deletion path; its emptiness cannot admit this skip. This is a read-only implementation plan. No source, CMake, test, build, GUI, original execution or mutable REA database was changed or used.

## Evidence and ordered native boundary

The immutable original DEF_GUI_MO_64.exe v11.0.2 is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All addresses below are full-image VAs with the common140 prefix; for example0506d5 means1400506d5. Bounded DUMPBIN14.44.35228.0 and section-mapped PE bytes corroborate the retained `analysis/resume56-opi-cache-140050690.asm` (SHA-256 `7bacc92563c959bafabed471fdc83c0efb102cc39a030060429d0dbab12a5a14`). No new live observation or REA Evidence ID is claimed.

1.0506c8 calls061f70, the position-list cleanup covered by CP124. On return,0506cd reloads this OPI.
2.0506d5 compares qword[OPI+20] with zero.0506da jumps directly to05072e when null. It bypasses both065440 deletion and050726's explicit owner-null assignment.
3.For a nonnull pointer,0506e4 copies it to locals. After the redundant local-null check,050709 calls **direct typed deleting wrapper065440(pointer,1)**. This is not virtual dispatch.
4.065440 calls065480 at065452;065480 calls067070 at06548e.067070 reads the wrapper's first qword, calls IAT141e042e8, and conditionally calls067830. This establishes additional internal destruction work; this report does not need or infer a complete container/type contract from that import.065440 then conditionally calls14dfc68 when flags&1 is set. The caller supplies1.
5.Only after065440 returns does050726 assign OPI+20=null. Therefore the native owning field remains nonnull while its wrapper destructor/deallocation runs. A failure before the return cannot be modeled as a completed null assignment.
6.05072e reloads OPI, and050736 tests qword[OPI+28]. This is the exact next guard. The proposed increment does not inspect or execute it. The later+28 direct wrapper065400 and+160 item-model destruction remain outside the increment.

End-exclusive native code-range hashes:

| Range | SHA-256 |
| --- | --- |
|1400506d5..140050736|`283fbc1988268f009a60785aa141783c20040f0d5a2f82838b6ed4089cf73551`|
|140065440..140065476|`59a2fef8b15a47618a50316136decf5e6f3baa69e84d48905d1c6747493a205d`|
|140065480..140065498|`9878eb582d1984046cc72d3d439f65791188dac49b672d3e3d7d25138b5b29e6`|
|140067070..1400670a5|`fb1313412d1b6e1470f7badd2d9250fa7c2fc00043ef0be45a8838cecf674d58`|

## Actual candidate owner and admission limits

`source/apps/moproj2mst/include/m2m/model.hpp:198` represents+20 as the actual `Operation::recoveryB`, a `shared_ptr<vector<RecoveryEntry>>`. The adjacent `recoveryFlags` represents+28. The field default already allocates recoveryB. `src/model/operation_rescue.cpp:178` calls `operationDefaults`; at200-201 that function allocates both recoveryB and recoveryFlags. Consequently neither a bare Operation nor normal initialized operations establish a null+20 branch merely because their recovery entries are empty. No supported Open/Save failure is established here to clear this field.

`shared_ptr::reset()` is unsuitable as an unqualified native deletion replacement: it changes the published owner before destruction, may retain the allocation through aliases, and does not establish native wrapper/backing-store cleanup. `use_count()==1` and vector.empty() do not close those representation and ordering gaps. Nonnull deletion needs a separately reviewed owner/destructor contract, including the wrapper's refcount and contained storage semantics. It remains refused in this proposal.

Current source snapshots read: model.hpp SHA-256 `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`; operation_rescue.cpp `88cdda9dd7eda054b1ead2b9ea994b47a423316cfa947c270d96d623302456b3`. CP124's cleanup header/source/test freeze and its null/empty prerequisites are qualified in `analysis/resume124-opi-position-list-prefix.md` (`dca790eebada769c8b4de4170cb380f06a6e3665c3cce177f5b0cfb07ee15593`).

## Proposed opt-in private contract

After root assigns the files following its source freeze, add an explicit `attemptOperationInfoRecoveryWrapperCleanup(Operation&, OperationInfoCleanupProgress&)` alongside the existing APIs. Append stage/outcome entries to preserve existing enum values. Suggested new stages are RecoveryWrapperDeleteBlocked and RecoveryFlagsCleanupBoundary; suggested outcomes are NullRecoveryWrapperSkipReached, NonNullRecoveryWrapperDeleteUnavailable and RecoveryFlagsCleanupUnavailable.

1.Validate the same live OPI generation before dereferencing any Operation field. Require this progress's existing RecoveryCleanupBoundary, reached by its explicit Props-null, owned-cache-null, empty-object and empty-position steps. Current field values alone cannot manufacture that history.
2.Revalidate those earlier actual owners/counts, then inspect the actual recoveryB pointer. Do not call a recovery lookup, inspect its entries, or use vector size as admission.
3.If null, advance only to RecoveryFlagsCleanupBoundary, returning NullRecoveryWrapperSkipReached. Perform no reset, deletion, null assignment or +28 read. Preserve current registry/editor guards, Props/cache/list storage, flags and the live OPI identity.
4.If nonnull, latch RecoveryWrapperDeleteBlocked before050709 and return the deletion-unavailable outcome, including for an empty vector. Preserve the field, control block strong count and contents. If recording owner identity, use a weak canonical owner plus opaque uintptr_t diagnostic token, never a strong lease or escaped dereferenceable pointer.
5.Stable retries remain at their reached guard. External reset/replacement after refusal cannot unlock the null branch; same pointer with another control block also cannot impersonate the original owner. After a null skip, reappearing recoveryB or any earlier owner/list content refuses. Expired/foreign OPI rejects first. Earlier public helper entry points may revalidate/report the reached guard, but must not automatically opt into this new step.

This is a conditional diagnostic cursor extension, not a normal Close success. The source-closed null branch itself has no native cleanup effect. It must not hook Operation/Project destructors, adapter failure unwinding or public Close.

## Exclusive future write set and focused validation

The minimal future reservation is only these existing private files beneath `source/apps/moproj2mst/`:

- `include/m2m/engine/operation_info_cleanup.hpp`
- `src/model/operation_info_cleanup.cpp`
- `tests/mdt/test_operation_info_cleanup.cpp`

No model.hpp, Project storage, Session/main, editor, lifecycle, CMake or public ABI edit is needed. Root has not assigned CP125 source changes at the time of this report. The existing focused target is `m2m_mdt_operation_info_cleanup`; root owns its Release build/test and source-stable grouped validation.

Meaningful added cases should cover: explicit prerequisite/opt-in order; a declared preexisting-null fixture advancing without owner/list/flag/current/OPI effects; default allocated-empty and nonempty wrappers refusing without ownership changes; stable retry plus external reset/replacement/control-block change refusal; no strong lease retained by progress; recovery or earlier payload/list reappearance after the skip; foreign/expired OPI rejection; and old APIs retaining their opt-in boundary. A null fixture must be labeled a conditional candidate state, not normal Open evidence. Keep Testkit main/includes and Release-active CHECK_EQ/REQUIRE checks; no C assert.

The later accepted normal Close root still requires actual application/current-main/busy/modal/nested Save and Project/slot cleanup predecessors. Neither this branch nor CP124 proves those prerequisites. Ordinary nonnull Props/cache or nonempty lists stop earlier, and ordinary initialized recoveryB stops at the proposed nonnull guard. +28/+160 cleanup, member destruction, QObject boundary expiry, registry/editor invalidation and manager reset remain separate dependencies.

Only this report is released for CP125. No source or test execution is claimed.
