# CP90 signed step-version access source review

## Result

The selected `step_version >= 6000` check now uses a separate bool-returning `GuiModelAccess::withStep` callback at the same place in `qualifiedGeometryReadiness` (`src/engine/viewer.cpp:566-572`). It reads only `nativeStep->step_version` and returns the signed comparison. The field is declared `std::int32_t` at `port/defmodel/include/defmodel/layout/db_step_data.hpp:94`; there is no unsigned conversion, narrowing, default value, or saved pointer/version. The callback result is consumed immediately, and the borrowed pointer does not escape.

## Guard and effect order

The preceding checks remain in this order: nonnull coordinates and canonical node-variable registry; if present, E-owner identity against the actual live step; for a nonnull N-owner, nonnull model/step and exact N-owner identity; guarded `u1b58 == 0` byte test; 3D dimension test; guarded signed version predicate. Only after the version callback returns true does the existing path enter coordinate COW/getters, index-table selection, cache/DB decisions, timer effects, or barrier installation. A false version result falls through to the same existing fast path/getters. No allocation, COW, timer, table, DB, or pending-frame effect moved across either callback.

The callback has no nullable fallback, but the nonnull step and pointer identity were already established by the preceding N-owner checks. For a parked model, normal viewer refresh returns from its existing pending preflight before this readiness predicate is reached; if the helper were reached while parked, its own pre-borrow availability check refuses before reading the version. Existing parked retry tests continue to assert unchanged state and timers. The adapter's profile classification remains distinct from the later original native synchronization gate; this migration makes no new claim about that native effect order.

## Test coverage

`geometry_actual_node_zero_flag_signed_version_gate_preserves_getter_cow` retains 28 rows: seven signed values (`INT32_MIN`, `-1`, `0`, `5999`, `6000`, `6001`, `INT32_MAX`) crossed with actual/null element owner and populated/null connectivity (`test_viewer_box.cpp:1239-1306`). It verifies the expected getter count and arguments, COW body transition/use count, preserved variable values and identities, node/connectivity/owner pointers, flags/version, and high-version table/cache behavior. CP89's additional nonzero-byte rows still exercise the earlier byte bypass independently.

## ABI and limits

No model field, access helper, public C API, or native layout changed. Current source hashes match the CP90 receipt: `viewer.cpp` `1f1192efda5844a0a7cd53445528b6eeceb89ffc9646995780caa86046a3dd62`; unchanged `test_viewer_box.cpp` `58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98`. Helper/header/export hashes also match the CP90 receipt. This is a narrow borrow migration; raw fields and unrelated raw step reads remain legacy limitations.

No blocker found. Review was read-only; no source/test edits, builds, or tests were made.
