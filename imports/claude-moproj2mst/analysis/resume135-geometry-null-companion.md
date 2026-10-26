# CP135: null-companion first-reader completion for edges

Actual null-companion geometry reads now complete the recovered first reader and accept connectivity-null edge-only geometry. Populated connectivity remains guarded before its native repeated coordinate getter. No DB query, version/cache mutation, A allocation, main-array copy or optional-mesh copy was added.

Root authorized viewer.cpp, test_viewer_box.cpp and this new receipt. No shared headers/CMake/public ABI were edited. No build/test execution, GUI, original launch or mutable REA operation occurred. Root owns grouped validation/archive.

## Reused synchronous behavior and ownership

The main-object state/skip/zero-+0x87c loop and actual node Stop are factored into one TU-local synchronous function. It receives the actual currently borrowed step and id/flag, with scalar diagnostic progress by reference. It preserves ordered actual state comparisons, pre-skip MAR selector bounds/load, null/type7/byte3610 skips, same-step object owner, zero node gate and count2-before-Stop. Dead record-address arithmetic remains omitted; no MAR/A pointee is consumed. Progress is stored as the same scalar fields inside the private low-record frame, preserving reached diagnostics on refusal.

CP134's low-record callback calls this shared behavior, then uses its unchanged exact-frame completion checks. The completer still has one callsite inside that initial borrow, and historical frame->step still has no consumer. The new null path never invokes the completer or constructs query-success/version evidence.

The null callback acquires the actual unparked step, rechecks its equality to the geometry node owner, null DB companion, dimension3/u1b58=0/high-version/cache0 profile, then performs arrays checkpoint and the recovered assignment/Stop sequence in that same borrow. Negative arrays count or any assignment/table/owner refusal preserves prior effects and returns pending. No pointer escapes, no frame is cleared/reacquired, and fresh null-path invocations retain their existing fresh-prefix semantics.

After successful first-reader return, its coordinate prefix is marked complete so its first COW is not replayed. The actual connectivity getter runs once. Null returns the existing Skip/edge contribution. Nonnull returns private `HighVersionRepeatedGetterPending` before the builder/second coordinate getter.

The distinction is necessary: null companion leaves version>=6000 and cache0, so the GUI second getter would repeat reader/timers. The DEF_PRE-based getter currently omits that GUI path. Low-record completion and cached high reads keep their existing distinct second-round behavior. Later fresh null reads repeat one first-reader invocation and its timers; they do not inherit the low-record no-timer-replay rule.

## Focused regression contract

The null companion regression now covers versions6000/10999/11000/INT32_MAX, arrays counts-1/0/99/100/101/INT32_MAX, and absent/populated connectivity:48 profiles. It expects exactly one first-getter COW per refresh. Nonnegative timer profiles reach Stop/count3 and append arrays checkpoint subject to capacity; negative count keeps two entry checkpoints and no arrays write. Connectivity-null completed profiles accept scene edges. Populated profiles remain scene-unsupported and never execute a second getter. Version/cache/MAR/A/optional-data identities remain unchanged.

The older generic cache-miss/timer and canonical table tests now recognize the newly recovered null edge outcome and third Stop checkpoint. Opaque DB and malformed initial table profiles retain their guards. Timestamp bytes at reached timer slots are actual clock samples, not fixed or required to differ between fresh calls.

## Focused-run expectation repair

Root's first focused run reported two existing viewer Testkit cases failing eight scene-unsupported assertions: former line1092 twice and1289 six times. Raw `build/logs/resume135-focused-ctest.log` is preserved unchanged, SHA-256 `70448c7c66d809b45c66d8d72d6651f900bb932e9b21603a7cf123152eadda30`. This was a failed focused run, not a validated candidate; root must rerun after this repair.

The first case's mode0/mode6 with populated=false are exactly the new null-companion/version6000/cache0/actual-node-owner profile, with E owner either null or actual step. The signed-version case's versions6000/6001/INT32_MAX, each with ownerless or actual-step E and populated=false, account for the six other failures. Populated, missing-coordinate/id and foreign/opaque owner cases remain unsupported. No other assertion failed in those cases, and the new focused null-path tests passed in that reported viewer run.

The ownerless E branch is source-qualified independently: retained `resume65-geometry-connectivity-owner-getter.asm` SHA `13e21714a4ba7e73de754d6f1fbf07bc83be9316679c5fa7b39d7e1b2db5932a` shows `140857ee6` jumping directly to `140857f06`, then loading E+8 and returning, with no allocation or sync call. Thus null E owner does not make these null-connectivity outcomes an over-admission.

Only the eight newly valid expectations were changed to actual edge scene maxY0, with explicit null DB/cache0/same-owner zero-main-gate/nonnegative-timer preconditions. Scoped timer restoration isolates their newly completed effects. Production viewer.cpp is unchanged from the first freeze. Static preflight was repeated; no successful rerun is claimed here. The repaired test hash below supersedes its earlier freeze.

One new six-profile test exercises the actual null callback with normal completion, foreign main-object owner, nonzero node gate, both MAR index bounds and true assignment state. Refusals retain the first COW/arrays checkpoint, no Stop, unchanged node/optional coordinate storage and cache/version. A second fresh invocation repeats the prefix rather than inventing a sticky low-query frame. Scoped restorers restore varied object owner/gate/flag before destruction. Existing low-record retirement and parked-model tests are retained.

## Static checks and freeze

Static preflight passed quoted/defmodel include resolution, balanced delimiters after comment/literal stripping, trailing whitespace, Testkit main and supported CHECK/CHECK_EQ/REQUIRE macros. There are49 TEST_CASE declarations (one added), no C assert and no REQUIRE_EQ. Static search finds the shared skip function plus its two synchronous calls, and exact-frame completer plus its single low-record call. No compilation/runtime pass is claimed.

| Frozen file under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| `src/engine/viewer.cpp` | `1d483eaf5b674a28639f03cf5f803c793dcc6ba84c4c7ab4663ace0d77482532` |
| `tests/mdt/test_viewer_box.cpp` | `808ece12ac9e7f6b75173e031d896f48a19ba718cec65ce6845274202b560280` |

Target `m2m_mdt_viewer_box`; CTest `m2m.mdt.viewer_box`. Controlling evidence/test plan is `analysis/resume135-geometry-next.md` SHA-256 `e50275898a872abddb159cea7242ab4e2f327754e00d81d369f0bd9178ca914a`, including null loader bytes and which0 high-version tail walk. Root must run focused/full validation and archive matching sources. The source pair is frozen for that boundary; populated null-companion second-getter behavior and broader Editor300/public reconstruction gaps remain guarded.
