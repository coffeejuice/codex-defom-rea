# CP82 DB access migration review

## Result

The `readDbStep` and `readDbEntry` migration places the geometry check before all DB/model/viewer effects. Both enter `GuiModelAccess::withStep` first; the helper checks the model barrier before retrieving the nullable `step` pointer or invoking the callback. No step pointer escapes the callbacks through their return values: each returns only `bool`, and the local `nativeStep` is used synchronously. `readDbStep` still routes the matched directory index into `readDbEntry`; the second guarded helper call is redundant at the barrier level but preserves the independently callable entrypoint's protection.

## Ordering and behavior

- `readDbStep`: after the gate, it checks `m.db`, gets the reader through guarded `modelDbReader`, resolves the requested step index, then returns false if the model has no step. This preserves the prior ordering (reader/index lookup precede the nullable-step return) and has no model write before that return.
- `readDbEntry`: after the gate, it checks both DB and the nullable step before any reset. If `resetViewer` is true it clears viewer items and then `viewerMirrorPlanes`, followed by writing `u1b58` and reading the DB into that same step. If false, it leaves the mirror/viewer reset phase skipped. After a successful native read, the cache/table updates, `viewerRebuild`, and finally `state100 = 1` remain in the same order. A failed DB read still returns before table updates/rebuild/state assignment, while retaining the original pre-read `u1b58` write and optional viewer clear behavior.
- Inside the callbacks, step access is through `nativeStep`; there is no `m.step.get()` or `m.step->` use. `modelDbReader(m)` is independently guarded before returning its local reader pointer.

The nonpending DB fixture covers a successful `resetViewer=true` read, a successful `false` reread preserving the `step` and `viewer` identities, a successful `readDbStep(false)`, object count/step identity fields, and `state100 == 1` (`test_db_load.cpp:169-197`). Its missing-step model checks both reset modes return false while leaving `step`, `viewer`, `state100`, and mirror-plane contents unchanged (`199-205`). A missing DB directory index is checked for both reset modes against the full `DB_STEP_DATA` byte snapshot and viewer identity (`test_db_load.cpp:253-266`).

The parked-model regression exercises both reset modes for both APIs. It snapshots the step bytes, viewer/mirror state, `state100`, tables, timers, and barrier state, and expects refusal before any change (`test_viewer_box.cpp:1922-1950`). This specifically closes the old `readDbEntry(..., false)` write-before-rebuild bypass. The callback-not-invoked test for `withStep` further verifies that a parked model is rejected before a borrow is handed to DB code.

## Remaining narrow coverage note

The source guarantees that a parked model is rejected even when `step` is null, because the gate precedes the null check. The DB suite separately covers missing-step behavior on an unparked model and parked behavior on a live-step model; it does not combine those two conditions. A focused parked-with-null-step assertion for both reset modes would lock down that ordering, but inspection shows no implementation defect. `readDbStep` also retains its existing precondition that the caller supplies a non-null `step` number pointer; the migration neither adds nor changes that contract.

No blocking source issue found. Review was read-only; no source or test edits, builds, or test runs were made.
