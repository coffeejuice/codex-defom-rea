# CP181 connector implementation: private +0x28 qword zero

Implemented only the qualified selected-path QtCore interval `[0x131220,0x131224)`. The retained private state gains optional uint64 `private28`, initially absent. Admission reuses the owner/control-block, ordinal, original private allocation, sizes, written address points, root bindings/commit markers, equal-version provenance and earlier zero guards, then requires the complete CP180 sparse prefix and unobserved private28. Only private28=0 and the cursor advance to BeforePrivateFlags20Read are published. The boundary is RVA 0x131224 before the unknown-input private+0x20 read. No private20/flags/EAX value, QObject completion, backlink, cache, map or counter effect is introduced.

The existing diagnostic absolute stop budgets 1..5 still retain exactly the five CP180 prefixes, including budget 5 before +0x28. Default budget 6 admits CP181. Values outside 1..6 refuse before initialization. The factory header was necessarily edited to update this existing hook.

Test source now migrates default boundary assertions to RVA 0x131224, resets private28 when deliberately reconstructing predecessor fixtures, includes it in full snapshots, extends the owner/root/version/predecessor refusal matrix through the CP181 entry, and adds actual ConversionSession single-store/equality-to-predecessor, same/different-key reentry, exact sparse predecessor, premature zero/nonzero private28, and ordinal refusal cases. CP180 budget tests continue to check every earlier prefix; the resumed path then reaches CP181. Static assertions retain no private20 member and establish the private28 qword width.

## PE and provenance verification

Read the source qualifications in analysis/resume181-connector-next.md and its retained disassembly. Independently parsed PE section headers: .text RVA 0x1000 maps to raw 0x400; RVA 0x131220 maps to raw 0x130620. The four bytes are 48 89 7b 28, SHA-256 004d4c4c5e01ae7e597c0f9e23d125455cb71368395152f5095c20414a587d17. Full QtCore4_64.dll SHA-256 is bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160. The following unaccepted RMW bytes remain 8b432025f0fcffff83c860894320. Retained assembly confirms RBX=RCX at RVA 0x131178 and xor edi,edi at RVA 0x1311b5; the selected equal-version branch has no intervening definition or call before this store.

This remains static evidence for the selected non-null equal-version path in an exception-directory unwind region. It establishes no live occurrence, allocator contents, complete constructor, mismatch path, cleanup or public QObject publication.

## Source hashes (implementation handoff; not a freeze)

| Path | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `61e3183bd1e58b6dd08b0e5c283c19414175e7ba841365eb179c6e5b77553a75` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `9c9cbbdcc335705921f5a584c0612c4dc332f2ad3f2aaa1d2c520f0194bd0bd7` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `51a7beacefacd9001d45bbc5d42617ed300a5c53e32803938bbba8b42f6d60d6` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `6ddddc4ef8a69903c3eaa9fe0cf3218c5bb050171137ccd0e7ceb5575e5b64db` |

Retained initializer assembly SHA-256: 006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad. Retained public-constructor assembly SHA-256: 6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9. Decision report SHA-256: 0f467499a94064401a5ee832c5740550d7459b9413bee8dc78ba495c0ca18af0.

No configure, build, executable generation, focused/full tests, source freeze, original comparison, GUI or live mutable REA operation was performed by this worker. Parent must run the required gates and review this handoff before claiming validation. No shared plan/state was changed. No GUI-control session was acquired. File ownership is released to the parent with this report.

Parent focused runtime feedback found two older ordinary reentry diagnostic assertions still expecting the CP180 equal-branch text. Updated both to RVA 0x131224 and clarified that their initial continuation includes CP181 +0x28. Bounded search found exactly those two stale ordinary reentry expectations; the follow-up static check confirms none remain, both sites retain BeforePrivateFlags20Read, and source hashes above reflect the repair. This worker ran no build or tests during the repair. Ownership released again.
