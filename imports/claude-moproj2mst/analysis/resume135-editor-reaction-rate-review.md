# CP135 ACVCOF reaction-rate callback review

## Result

No actionable source blocker found in the frozen private callback/test. The method keeps the native-recovered order: project the four control values first; query the actual current simulation control; then, if it matches the page's source control, set current curve index 3 and write the selected live/temporary variable fields. It preserves the selected-zero dimension exception and does not allocate/free or replace either curve's storage.

## Contract checks

- Admission validates the live Project/OPI generation/model/remesh and completed Environment page. The temporary ACVCOF owner must be present, distinct from the other temporaries, and be the same shared owner as the page curve. These guard failures are pre-projection; they are candidate admission constraints, not claims of original native validation.
- Selection 0..3 projects `{selection, selection==0, selection==0, selection!=0}` before the current-SC query. Missing or replaced SC returns `UnsupportedSnapshot` at `ControlsProjected`, preserving the projected control state and stopping before current-index or curve data writes. No retained stale pointer is dereferenced and no replacement SC is substituted.
- With the qualified SC, current index is written as 3. Selection 0 writes `acvcof.ndim=0`, then live `var[0]=0`, then temporary `var[0]=0`; selections 1/2/3 write only live and temporary variable codes 3/12/1 and leave live dimension unchanged. All choices leave temporary dimension untouched.
- The implementation additionally rejects a temporary curve aliasing any live SC environment curve after projecting controls but before index/data stores. This guard preserves the source ordering of prior control effects without treating malformed alias topology as a native result.
- The tests allocate real table-backed DYNARY owners and assert pointer/count/payload retention, temporary lifetime/identity, selection-zero live-only dimension clearing, repeated-event writes, missing/replaced SC partial effects, and early owner/selection refusal. They do not claim physical widget state, click delivery/reentrancy, or public Session reachability.
- No Fill/Apply replay, file effects, OPI updates, or public construction admission were added. `completeForPublicSave` remains false; actual selected constructor palette witness and later unrelated editor tails remain unresolved.

## Evidence binding and limits

The retained native callback range `[140eccf10,140ecd2e5)` establishes control updates before current-SC lookup, then index 3 and the selection-specific live/temporary writes. Its static trace supports the selection-zero live dimension clear and absence of temporary dimension/table mutation. This is source-derived, not an original-process differential.

Reviewed source SHA-256 `c36e0fcd801cf5c7d7534f046b308f74bcf520772c653b83e29d6a617bb1c9de`; focused test SHA-256 `6c7107ad558895d30aaefaa72ce349fb29a40373aac2c061162d46cbabb3d82a`, matching `analysis/resume135-editor-reaction-rate.md`. No build or test execution was performed in this review.
