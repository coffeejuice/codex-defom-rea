# CP161 contextual empty-operation test source review

The released test exercises the intended actual Session-to-lazy-reader route and the precise scalar refusal. I found no blocking behavioral or interface defect. One small test-strengthening change is recommended before claiming the fresh-row invalidation transition is exercised: seed `study->samples.invalid.fill(false)` before the read. The current defaults are already true, so the final true assertions establish retained state but do not detect missing invalidation.

This review is read-only. No production/test source, fixture, compiler/build/CTest, GUI, live REA or database was changed or run. A read-only test-source preflight was run and passed; root's parallel compilation/runtime results remain separate.

## Actual route, owner and A,B,A order

`test_keyword_notifier_entry.cpp:148-215` reuses the actual Rig rather than calling the private helper. Rig owns a real ConversionSession and its Project, binds recording StudyIo to that Session's model-manager and notification owners, selects an owned Type2 SUB, seeds InputTable/current PAR and records the setup write baseline. Its read method sets Ctx.session, records that pointer and calls parseSubProject.

The current production chain is parseSubProject -> contextual readLazyStudy -> named InputTable read -> InputTableSource carrying the checked caller identities -> readInputTableFromSource -> exact DoeVariable and Keyword dispatch. The test's `rig.read(&rig.session)` therefore reaches the nonnull-source route. `routedSession` alone would only be a fixture assignment; the production source inspection and unchanged operation1 control in this same target establish the relevant handoff.

The fixture has real A,B,A XML: A number7, B number8 with LinkTo A and Description, then duplicate A number19 with Keyword and an unreached later Description. The actual duplicate-header code targets the last owner B, while the Keyword lookup uses original name A. Assertions check the two owner aliases, A number7, B number19/Description, selected index1, A's sole typed Item and B's empty Item list. B's null link verifies that the real queued LinkTo has not been resolved. The test's comment says B links “forward” to A, but A is the earlier row; this direction word is a documentation nit only.

The `study.get()==studyIdentity` comparison is locally tautological because the retained shared pointer is unchanged; the substantive owner/route checks are that this retained Study actually acquires the two expected records, its alias order matches them, and the actual Session notifier/I/O owner identities remain unchanged. No blocker follows, but a future assertion could compare `rig.sub->ownedDoeStudy.get()` directly for clearer owner wording.

## Baselines, notifier bypass and exact stop

The test sets suppressed=false and flag90=true after Rig setup and inserts a key sentinel before capturing keys/attempt count. These observations use the canonical Session owner, not a copied notifier. The read requires both diagnostic fragments `0x140910abf` and `before upper-text probe`, so an earlier generic unsupported error cannot pass the test.

The implementation's EmptyOperations branch executes its canonical append/re-fetch/scalar stores and throws before testing source.notifier. Its no-notifier behavior is therefore distinct from a suppressed notifier return. The test checks suppression remainsfalse, flag90 remainstrue, and keys/attempt count are unchanged. The existing operation1 unsuppressed case remains intact and observes actual flag90 clear/key-construction guard; it is an important control against a fixture that never carries context.

A's record checks exact TLOC ID8, empty operation list, nominalText with four spaces plus tab and0.1, parameter=-1 and false function/enumeration. The diagnostic and B Description establish the later Description was not reached. No callback return, key/data construction or public parser success is asserted.

## Sample and I/O coverage

All three sample tables and counts are seeded and snapshotted before the read. Equality after the refusal distinguishes those payloads from flags. The test checks all invalid flags true, matching the reached fresh-header creation prefix. However `SampleState::invalid` defaults to alltrue (`doe_study.hpp:33`) and this case never changes it before parsing. **Recommended coverage correction:** insert `study->samples.invalid.fill(false)` with the seeded payloads before line180's read. The same final assertion would then detect that fresh A/B header creation actually invalidated them. This is a test precision issue, not a newly found source-behavior defect.

The I/O baseline is captured after Rig seeds its InputTable and DEF_DOE.PAR. After the throw, the test requires exactly one additional read and checks that read is the exact named InputTable. It also checks textWrites/xmlWrites unchanged and the setup text-write count retained. This excludes subsequent LHS/FF/USR/current-PAR reads/writes without misclassifying fixture seeding as reader output. The seeded current PAR would expose accidental continuation. Later whole-container reindex, links, outer ID processing and sample loading remain unreachable on this throwing branch.

## Public limits and source checks

The implementation receipt correctly labels this as headless contextual reader coverage with an explicit guard, not whole public Open/Save acceptance or original parity. The new case makes no M2M_OK assertion. Existing nonempty-variable Save reconciliation remains unchanged and is asserted by the operation1 case in this target; source inspection confirms it also applies to the empty-operation partial Study. No production edit was included.

The file retains a Testkit main, valid TEST_CASE/CHECK/CHECK_EQ/REQUIRE use, and resolvable public/private includes. Read-only `admin/check_test_source_preflight.py` returned `ok:true`, no issues. UTF-8/final-newline/trailing-whitespace checks passed. None of this claims compilation or runtime success.

## Frozen reviewed identities

| Artifact | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp | 31a0e8c3fd234666fabd3694874364806b84bfd6c124d54c8c19269081c5d261 |
| analysis/resume161-doe-session-empty-test-implementation.md | 50fa5c6fd566864c852ffe86793d47eecf6c12146f962a279c8f51c383ddd22f |
| analysis/resume161-doe-session-empty-plan-review.md | 753397f31fb04ed68b4fe5dc9e1476f0dec59074afab2caf30c8825d06a36f6a |

Disposition: no blocking source/interface finding; the accepted integration-test goal is met for actual contextual routing, named/last-header distinction, scalar prefix, notifier bypass and later I/O refusal. The invalidation baseline should be strengthened before reporting that specific transition as regression-tested. Any subsequent edit must receive a new hash/validation receipt; this review binds only the snapshot above.
