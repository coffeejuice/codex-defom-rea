# CP181 connector independent source review

## Verdict

**Accepted for build and executed validation; no blocking source defect found.** The released four-file snapshot observes only private28 zero and stops before the unknown-input private20 flags read. Default diagnostic ordinal 6 makes the new stage reachable while preserving explicit CP180 stops 1..5. Source hashes match the implementation release. This review does not establish compilation, executed tests, freeze or original-comparison acceptance.

## Native boundary and source admission

The reviewed native bytes at QtCore RVA `[0x131220,0x131224)` are `48897b28`, SHA-256 `004d4c4c5e01ae7e597c0f9e23d125455cb71368395152f5095c20414a587d17`, in canonical DLL digest `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Qualified RBX private and zero RDI survive the equal-version path. The qword destination +0x28..+0x2f fits within 0x88. Following `8b432025f0fcffff83c860894320` reads/transforms/writes private20 and is excluded; no allocator-zero input is inferred.

At batch ordinal 5/BeforePrivateZero28, the shared guard revalidates retained Report ordinal/pointee, allocation-time private identity, sizes/written address points, store/root control block and pointee pairing, canonical committed first QList/QString, canonical committed QVector/optional qualifier, second same-root QList/marker, original seven zeros and exact retained version/module/load/call. Sparse matching requires all five CP180 fields zero. Before CP181 writes, private28 must be disengaged; zero and nonzero preobservations both refuse.

The only admitted effects are private28 uint64 zero and BeforePrivateFlags20Read. All prior fields/bindings/markers, three deltas/observations, identities/counts/maps and incomplete/backlink flags stay fixed. No private20, flags result or EAX state is added. Terminal reentry at 0x131224 reaches only the unsupported diagnostic. Static evidence qualifies the selected owned non-null/equal path, not live contents, mismatch effects, exception recovery, cleanup or full QObject completion.

## Stop control and tests

Internal stop range is now 1..6, default 6, validated before initialization. Stop check precedes the ordinal-5 CP181 block: configured 5 still refuses with unknown private28; configured 6 admits the sixth store and breaks before out-of-range phase indexing. Stops 1..4 retain their exact earlier sparse prefixes. Private friend test access remains outside the public C ABI. This resolves the prior source-review reachability hazard without removing the old diagnostic boundaries.

The inherited owner/root/version/predecessor matrix now loops through ordinals 0..5, explicitly reaching CP181 entry; full snapshots include private28. Dedicated CP181 sparse tests exercise missing/nonzero each five CP180 predecessor fields, private28 premature zero/nonzero, and invalid retained ordinal with same/different-key no-mutation checks. Existing false marker, owner/control-block/expiry/provenance/address/identity/completion cases carry forward into the extended entry matrix.

Actual Session test at `test_doe_task_factory.cpp:4630` first stops at ordinal 5, verifies private28 unknown and different-key reentry unchanged, then raises to 6 and observes exactly private28/cursor. It temporarily restores only those two effects and checks the complete predecessor snapshot, proving all other state unchanged; subsequent same/different/custom-key terminal refusals preserve the complete final snapshot. CP180 prefix/resume tests retain all original diagnostic targets and explicitly continue through the new default successor. Existing independent history and observation preservation remains covered.

Header/type/fresh checks establish optional<uint64_t> private28 unknown initially and no private20 model. Ordinary default boundary assertions migrate to 0x131224; earlier reconstructed predecessors reset private28, including the shared CP180 prefix helper. Prior correct reference arithmetic is unchanged. Invalid-hook tests now reject outside 1..6, retaining before-allocation and retained-prefix no-mutation coverage. No later flags read, public task/private backlink or completion is published.

## Exact release SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `51a7beacefacd9001d45bbc5d42617ed300a5c53e32803938bbba8b42f6d60d6` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `61e3183bd1e58b6dd08b0e5c283c19414175e7ba841365eb179c6e5b77553a75` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `9c9cbbdcc335705921f5a584c0612c4dc332f2ad3f2aaa1d2c520f0194bd0bd7` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `695c60f9dc6cb46bb0a9a796e5df92bab1eacacb9d4b5ebad66a0795f5c23cea` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state changes occurred. Required executed gates remain with the parent.

## Final hash addendum after executed diagnostic repair

The initial review missed two stale historical reentry diagnostic literals at test lines 3677 and 4083. The parent-owned focused run exposed them; both now expect `RVA 0x131224`, matching the CP181 terminal diagnostic, and their comments describe the followed successor stores. This corrects the earlier migration assessment; no runtime implementation change was needed.

Rechecked all literal diagnostic strings containing an RVA, qualified-version wording or branch-taken wording in the factory test file. No stale equality-branch reentry diagnostic remains. Remaining older RVAs are intentional CP180 absolute-stop entries or earlier malformed-seam expectations; ordinary/reentry final assertions use 0x131224. Phase and complete-state checks remain alongside those repaired assertions. Factory/runtime/factory-header digests are unchanged from the release table above. The **final test-file SHA-256**, superseding its earlier table entry, is `6ddddc4ef8a69903c3eaa9fe0cf3218c5bb050171137ccd0e7ceb5575e5b64db`.

`analysis/resume181-reentryfix-build.json` SHA-256 `ec1a2eedabf547ce2b988a05c967d29a32e140d10b88cbc1e6e94a0e0c328aff` records completed configure, focused preflight, all six focused tests and build, each exit zero, with identical before/after source-tree hash `ddfbb8a08f0a9299ed05aaae442c6b9bbd1f2872a40c9cd3adcaae2b14a4eaf8` and `source_stable_during_build=true`. Parent additionally reports full-suite pass; that full-suite result is not independently established by this build receipt alone.

Final source verdict remains accepted with no unresolved blocker. This addendum only updates review scope and final receipts; it performed no build, tests, source edits, GUI, live REA or shared-state mutation.
