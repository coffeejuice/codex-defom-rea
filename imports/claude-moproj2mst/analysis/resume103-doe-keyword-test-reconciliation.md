# CP103 DOE Keyword-entry test reconciliation

Updated only the assigned DOE test files for the reached Keyword boundary.

- In test_variable_deferred_link.cpp, removed direct Keyword from the pre-append guard matrix and added a focused Keyword-entry case with LinkTo plus an earlier Description. It verifies the concrete header owner/list alias, ID, and Description remain after the unsupported stop, while the queued link remains null because reader finalization is not reached.
- The later-row pre-append guard now uses Geometry, preserving that unrelated guard without asserting stale Keyword behavior.
- In test_variable_notify_binding.cpp, renamed and revised the stale public Keyword test. Its input includes a queued LinkTo and an earlier Description; it now verifies the owner/header/Description partial state, unresolved link, and empty notifier attempts. The Keyword branch still throws before notifier delivery.

Static preflight: both files retain Testkit main entry points; no new includes were added; source checks found no C assert, unsupported REQUIRE_EQ, or trailing whitespace. No production/shared files changed. No build or test was run; root owns integration validation.

SHA-256:

- source/apps/moproj2mst/tests/doe/test_variable_deferred_link.cpp: 9EAF65EB555FAF155C9E6AEE05E0C670FD187474865B7E7DA93FE837590104BC
- source/apps/moproj2mst/tests/doe/test_variable_notify_binding.cpp: CF0485957BE6DE2426BDECC83E65297AC2178B57EE22B7F2A1DD4A1CFE49E505

