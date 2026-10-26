# CP161 empty-operation test repair review

The narrow repair resolves the sample-invalidation coverage finding. No blocker remains in the reviewed source. Root's rebuild/full-suite results are still required; this review did not compile or execute the test.

The repaired `keyword_empty_operations_actual_session_route_commits_named_scalar_then_stops` case seeds `study->samples.invalid.fill(false)` after recording I/O/notification setup baselines and before the actual `rig.read(&rig.session)`. It retains the post-read assertion that all three flags are true. Fresh A/B header creation is now observable as a transition, while the three seeded table payloads and counts must remain unchanged. The fixture's exact scalar `0x140910abf` / upper-text-probe refusal still distinguishes this reached state from an earlier guard.

I verified the complete change, rather than only inspecting the added line: reversing the new false-flag seed and changing the corrected comment's “back to A” to its prior “forward to A” wording reconstructs the exact previously reviewed file SHA-256 `31a0e8c3fd234666fabd3694874364806b84bfd6c124d54c8c19269081c5d261`. Those are the only two edits. No owner, notifier, A/B alias/Description/link, scalar-record, exact diagnostic, Session-route, sample-payload or I/O assertion changed. The comment now correctly describes B referring to the already encountered A.

Consequently the earlier accepted review remains applicable: the test uses the actual Session/Type2 contextual reader route; suppressed remainsfalse, flag90 remainstrue, canonical keys/attempt count stay unchanged, exactly the named InputTable read occurs, and no later sample/PAR read/write or public parser/Save success is claimed. The existing contextual operation1 control remains intact.

Testkit main/macros and includes are byte-identical to the previously preflighted snapshot. Direct UTF-8, final-newline and trailing-whitespace checks passed. No production/test source, build, executable test, GUI, live REA or mutable database was changed or run by this reviewer.

| Reviewed artifact | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp | 81d1c32dd14af74d470037d8b5ce4478fbd1b6836c067b604121be5988938057 |
| analysis/resume161-doe-session-empty-test-repair.md | b8985f84aa92d4dbc36f25aed0a1f649fcea12773d4c57777ae069d5d9750449 |
| Prior independent source review | a14d6e8aff86b4b2db3ebd68ab39d8b8fbda48cb412b29bc418e18a9bbb792f6 |

Disposition: source review accepted, no remaining blocker. This is repaired contextual candidate coverage, with unchanged original/public acceptance limits.
