# CP145 focused fixture repair

The failed focused case was a fixture-contract violation, not a notifier production defect. Only `source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp` was repaired. Its frozen SHA-256 is `7458a06ddddf2b80d090da5c45bc1a4e5ae994fc2e7ee6a2f0bd6c1f0b6e4b6a`.

Root reported `m2m.doe.keyword_notifier_entry`, case `keyword_notifier_context_identity_refusals_preserve_flag_and_committed_owner`, throwing `DOE notifier state must bind a nonnull shared owner before observations` before the intended entry assertion. The loop's profile5 attempted to replace the already-bound old I/O notifier with a foreign owner. `StudyIo::bindSessionNotificationState` (`src/doe/session_notifications.cpp:36..40`) explicitly rejects replacement of any existing bound owner. This happened during fixture setup, outside the expected notifier-entry refusal.

Profile5 now constructs fresh recording I/O, binds its ModelManager and its foreign notification owner once, installs that I/O on the Project, and updates the explicit negative context's pointer/weak identity to it. This allows the entry's I/O identity checks to pass before it rejects the mismatch between observed notifier and actual Session canonical owner. The fixture no longer calls a prohibited rebind. It also asserts that the retained old-I/O lease differs from the new current I/O in both replacement profiles.

All prior assertions remain: the actual canonical flag90/keys/attempt count are unchanged on refusal, the constructor prefix stays committed, and no receiver/key/lookup or parser success is inferred. No production source, suppression semantics, ownership validation or public gate changed. The original implementation receipt/source hashes remain historical; only the test hash above supersedes its previous test hash.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp` returned `ok: true` after the final edit. Direct final-newline/trailing-whitespace checks passed. No worker build/CTest, GUI, original process or mutable REA action occurred. Root owns rebuild and full validation; the repair is not claimed runtime-verified here.

The first failed-run evidence and `build/logs/resume145-focused-preflight-02.log` were not modified. Source was frozen to root before this new repair receipt was written. All worker source is held.
