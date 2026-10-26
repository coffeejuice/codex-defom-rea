# CP161: sample invalidation assertion repair

## Change

In `source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp`, the empty-operations Session-route test now seeds all sample invalid flags to `false` before parsing and retains the post-read assertion that all three are `true`. This makes fresh-header invalidation observable. Corrected the comment to state that B's `LinkTo=A` points to the already encountered A row.

No production, CMake, or shared files were changed. Testkit entry point and supported macros remain present, quoted/direct includes resolve, there is no C `assert`, and the paired invalidation baseline/assertion is present. `git diff --check` passed. No build, test, GUI, live REA, or mutable database operation was performed.

## Owned-file identity and release

Test file SHA-256: `81d1c32dd14af74d470037d8b5ce4478fbd1b6836c067b604121be5988938057`.

Exclusive file ownership is released after saving this report. No app-control session was acquired.
