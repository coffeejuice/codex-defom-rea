# CP159: timer-prefix source review

The production timer cut matches the accepted plan, but **two definite test blockers** prevent acceptance of the reviewed release. Both were reported promptly to root. No compiler/test execution was needed to establish the contradictory fixture conditions below. Only this report was written; no source edits, build/test, GUI/original execution or REA operation occurred.

## Blocking fixture defects

1. In `stopping_cached_reader_preserves_current_tables_and_miss_partial_prefix`, cache11 is set to0 for profiles0 **and1**, but only profile0 allows timer mutation; profile1 calls `globals.timersUnchanged()`. Profile1 has valid MAR/id bounds (including when initial module dimension3) and therefore must execute start/checkpoint. All three profile1 iterations contradict production and the accepted native contract. The new supported formula also removes the old valid altered-current3 cached case. A narrow repair is to restore CP158's cache assignment `profile==1 ? 0 : -1` and supported formula `profile!=1 && (initialDimension!=3 || profile==0)`, then assert count2 for profile1 and unchanged timers otherwise.

2. `stopping_cache_zero_records_only_native_timer_prefix_on_each_fresh_miss` never sets cache11=0; Fixture initializes it to1. It therefore runs cached success rather than the expected one-getter CoordinateSync/timer prefix. Set cache0 explicitly. It also reuses one page for both iterations: the first genuine miss commits its node-spin value before refusal, so a same-value Fill does not emit a fresh callback on iteration2. Construct an explicitly fresh delivery/page for each invocation (or an independently source-qualified changed-spin delivery), rather than weakening production's remembered-spin behavior. The plan specifically calls for repeated **freshly delivered** misses.

These are test defects, not reasons to alter the two-call production prefix. A repaired test/hash binding is required before this review can accept the release.

## Production inspection

The only production difference from CP158 is the declaring StepTimer include and the small split of the old combined guard. Bad MAR index/signed id limit refuses before any timer access. Existing actual allocation/key0 COW, same-owner recheck, const canonical variables and real module selection remain before that guard. Nonzero cache still takes CP158's normal cached route unchanged.

Valid cache0 now obtains the actual process-global `g_timer_read_obj_node_state_variable`, calls `start("Start")`, then `checkpoint("ReadSingleAArrayFromDB", nullptr)`, and returns CoordinateSync immediately. This is exactly the bound order before82d8c0 loader entry. No companion pointer read, version/MAR/A/cache mutation, arrays checkpoint, Stop/report or object copy was added. Existing real samples/history semantics and public constructor/Save guards remain unchanged. The preceding source comment saying refusal is "before timers/DB/body work" is now stale for a valid miss; update it to distinguish the new timer prefix from the still-guarded DB/body tail.

The implementation correctly leaves start's reset/count behavior to existing StepTimer rather than adding a false flag/count gate or inventing timestamps. It does not claim native clock epoch, EH or positive cache-miss completion. The source include resolves to the actual timer declaration; no static compile issue was found.

## Remaining test inspection

Five unique Testkit cases/one main are present; CHECK/REQUIRE macros and includes are valid by static inspection, with no C assert/REQUIRE_EQ. Existing cached, owner/type and private constructor cases retain their scope. The constructor miss expectation was correctly changed to count2/exact labels and restores the timer before the following cached fixture.

The new sentinel checks correctly distinguish name/flag, unwritten label tails and slot99 from the overwritten0/1 entries without asserting clock values. Actual Globals RAII restores module cells and timer state even on unwind; the manual per-iteration timer restore is test-only. Arrays timer is checked only by count in new miss assertions, rather than the full bytewise untouched-state check specified in the plan; an opaque companion control and old-timer-at-COW observation are also not present in this release. Those are coverage limitations to resolve or explicitly qualify separately from the two failing fixtures. They do not change the static conclusion that production stops before loader.

## Reviewed hashes and disposition

Implementation receipt was read at its corrected converter location, SHA-256 `a09b71cc5f16e331d6b5aed549e58bcfd279939e3b046614af892fa7e0bb1064`. Reviewed files under source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|src/engine/simcontrol300.cpp|`ef7d756a6b055d17a4abd0b0b1cb025e994b0b408e661192521b6de7f397c447`|
|tests/mdt/test_simcontrol300_stopping_cached.cpp|`4f081193d396b39bc974e755068b56f64dc46689f109cd89f4d92127ae033996`|

The accepted plan review SHA is `228e578de35e119ca0b25786c54df79c01bdcfef6b0457d5769565dfc7cc88a6`; its evidence proposal SHA is `317df60b47552d3a64183a0861fa9d51a3bafe3631d3d794eddbd909b2ed79d8`. Those retain the verified original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and six ASM/native hashes. No reconstructed executable/deployment was inspected. Production is consistent with the plan; this exact test release remains blocked pending repair and root execution.
