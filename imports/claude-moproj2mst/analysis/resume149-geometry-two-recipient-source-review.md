# CP149 independent source/test review

## Conclusion

The frozen Viewer implementation satisfies the reviewed two-recipient design. Each completed recipient has an independent fixed-slot progress record and 24-byte copy receipt; the second recipient is admitted only after the first receipt matches the live object currently found at its recorded step ordinal. The loop keeps the same initial frame and source A, executes one Stop only after loop exhaustion, and high retirement validates every started recipient. The tests independently seed source rows for the two copies and cover admission failure, mid-second-recipient failure, a third positive recipient, a later MAR refusal, and no replay.

No blocker found. In particular, the live-table safety condition from my design review is present: `geometryCompletedMainRecipientsMatch` calls `step_object_at(step, recipient.ordinal + 1)`, compares the result with the saved object pointer, and only then dereferences it for descriptor/node/registry validation.

## Source checks

- The frame has fixed `mainRecipients[2]` storage and `mainRecipientsStarted`; no growable receipt collection or ownership transfer was added. Each entry carries its own stage, ordinal, primary pointer/count, BC attempts/pointers/counts, 28 receiver identities and dimension snapshots, and post-copy source/destination/size/ordinal receipt.
- Active progress requires the current installed high frame, same step/source/query/directory receipt, the exact current recipient record and stage, and successful revalidation of all previously completed recipients. The saved prior object pointer is compared against a fresh `step_object_at` result before any field access. Prior and active primary allocations are checked for distinct identity; duplicate recipient object/ordinal and third-start attempts refuse.
- Fresh recipient admission occurs before incrementing `mainRecipientsStarted` or freeing/resetting the object's existing arrays. After start, each allocation and callback-capable registry lookup advances only that recipient's stage. The first committed copy is never reset or rewritten when the second recipient refuses.
- Each successful recipient copies exactly `3*sizeof(double)` from the current recorded A allocation into its own actual primary allocation. It stores its receipt only after `memcpy`. Subsequent source changes do not retroactively change first destination bytes.
- The outer actual loop still loads the current object and MAR input before its skip conditions. Successful recipient helper completion returns to the same loop; only after all objects finish does it validate all copy receipts and perform the single Stop. If a later recipient/object guard fails, Stop is absent and the original frame remains parked. The high completer checks each started entry before clearing the exact initial frame. Low/zero profiles are not broadened.

## Test checks

- `geometry_two_fresh_recipients_copy_independent_rows_and_complete_once` covers versions 6000/11000, populated/null connectivity, and zero/positive BC counts. At the final sizing lookup for each actual node owner, it writes a different row into source A and a separate destination sentinel. It then compares exact bytes for both recipients and final source, confirms distinct primary/BC allocations and distinct registry bodies, retains all 56 ordered sizing observations, and checks one arrays checkpoint/one Stop plus the expected one- or two-getter result. Cached refresh leaves both destination rows unchanged and does not repeat sizing.
- `geometry_second_recipient_partial_lookup_preserves_first_and_rechecks_live_table` changes width, owner, or the live linked object order during the second recipient. It verifies first copied bytes remain, second partial effects are retained, second destination is not overwritten, node timer remains before Stop, scene stays unsupported, and later repair does not resume the parked frame. The reordering case restores links with RAII before destruction.
- `geometry_two_recipient_copies_remain_before_third_positive_or_later_MAR_guard` verifies two distinct copies persist when a third positive object refuses or a later MAR index refuses; third destination remains untouched and no replay occurs after raw repair.
- `geometry_second_recipient_fresh_admission_guards_preserve_first_copy` covers a preexisting second primary pointer, wrong descriptor/first-node state, and foreign ownership, proving refusal before second reset while first bytes persist.

The adversarial source-row writes are candidate test instrumentation, not fresh original observations. Assertions use active Testkit checks; no zero-iteration-only loop was observed in the new profiles. No build or tests were run for this review.

## Frozen files

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `AC7A0603ADFA0C397441223D61A44AABDEFD170C8023A658C13E589C0327D716` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `2F288B00A17567A7E1E4326583297CE43902F7126FB8CA72EB1BC6942C34E33B` |
| `analysis/resume149-geometry-two-recipient-implementation.md` | `05766DF0D32C052D947C74432360983C062DEF8DDE93FF62EA58410967A71E90` |

Read-only review only; no source/test/build/GUI/REA mutation or original execution occurred.
