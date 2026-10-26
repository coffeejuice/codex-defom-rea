# CP131: low-record query reaches the arrays timer once

The initial low-record geometry invocation now records the original arrays-timer checkpoint after its directory query and recovered old-version loader return. The actual model remains parked before object assignment. Retry, reset and owner-change guards remain in place; there is no resume, query replay or barrier clearing path.

Root authorized only `src/engine/viewer.cpp`, `tests/mdt/test_viewer_box.cpp` and this new receipt. No other files were edited. No build/test execution, GUI, original process or mutable REA operation occurred. Root owns the grouped build and source-stable archive.

## Ordered implementation

The existing `GuiModelAccess::withStep` callback still installs the barrier before calling `step_dir_sizes`, then records the actual post-version and QueryCommitted stage. A stack-local completion marker is set only after those statements. The callback does not retain its borrowed native pointer.

After the borrow ends, the same initial invocation requires that completion marker, nonzero query result and signed postVersion<6000. Earlier admission already establishes the actual dimension-3/u1b58=0 profile. These conditions close native `14082bf10`'s true loader return; failed install, unfinished query or another version cannot authorize the new phase.

The new code then reads the existing actual `g_timer_read_arrays.count`. Negative count returns `HighVersionPostLoaderTimerPending` with the query/version/barrier intact and no arrays-timer mutation. Otherwise it calls existing `checkpoint("Assign the variable to the object data", nullptr)`, records ArraysTimerCommitted, and returns `HighVersionObjectAssignmentPending`. Nonnegative count>=100 is the existing checkpoint's normal no-op, and still consumes this phase.

This matches native loader return at `140828167..8173`, then caller checkpoint at `14082d8d6` on actual global `143dcdae0`. The new phase touches no step pointer after borrow exit. The barrier is already installed, so all subsequent geometry entries refuse before timers, COW, tables or query. It is a reached-phase record, not authority for future continuation.

Controlling evidence: `analysis/resume131-geometry-next.md` SHA-256 `eadc10b716bba6271703bdf91cc15df5deb38ad8e8d0b366e01ad979b542accf`, plus CP71/73/78 native loader/query/timer qualifications. The original EXE hash and exact gate/checkpoint byte hashes are recorded there. No additional native behavior was assumed.

## Focused regression boundary

The existing low-version test still exercises 5999, 0, -1 and INT32_MIN. Its helper now expects one arrays checkpoint on the first parked invocation and compares the **entire** post-first timer snapshot after the second invocation. High-record and invalid-admission helper calls retain the original unchanged-timer expectation.

One new Release-active Testkit case exercises initial arrays counts -1, 0, 99, 100, 101 and INT32_MAX, each with absent or populated connectivity. It reuses the existing timer assertion for exact label/null sub, count, untouched slots and fields, without fixing the clock sample. Negative count preserves all timer bytes; full count remains unchanged. Every profile retains low post-version, cache/array/object state and unsupported scene-box result. The second refresh must preserve the entire timer snapshot and execute no second coordinate getter.

The broad parked-model/owner-bypass test now permits the one initial checkpoint. Its subsequent full timer snapshots and all existing refusal checks remain: changed version/cache/tables, Viewer swap/reset, lost DB provenance, mutators, imports, reset and other entry paths cannot replay or retire the frame. No object assignment is enabled.

## Static preflight and frozen source

Static checks passed for both edited files: quoted includes resolve against their source/include roots; delimiters balance after stripping comments/literals; no trailing whitespace. Test source has `int main()` calling `testkit::run_all()`, 46 TEST_CASE declarations (one added), supported CHECK/CHECK_EQ/REQUIRE macros, no C assert and no REQUIRE_EQ. This is a static receipt, not compilation or test success.

| Frozen file under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| `src/engine/viewer.cpp` | `8e9483a5bc116e043ed7a7ef1df44d38e1be6653d3c72ab99aeb32cb039e9a3f` |
| `tests/mdt/test_viewer_box.cpp` | `c2c21c52e1d5cc9984916ea2885626cda70f2f03a9c9428f24686a71ffc96f84` |

Focused target: `m2m_mdt_viewer_box`; CTest name: `m2m.mdt.viewer_box`. Run it through root's active grouped build, then retain the normal full suite/freeze boundary. The source pair is frozen for that build/archive; no further source changes are pending in this lane.
