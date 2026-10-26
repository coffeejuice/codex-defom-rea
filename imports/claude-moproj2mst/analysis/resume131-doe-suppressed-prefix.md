# CP131: isolated DOE notifier prefix implementation

## Change

Added an explicit `RefreshNotifierPrefixOutcome` and `DoeTableOwnerState::enterRefreshNotifierPrefix(Session&)` in the table-owner API. It first requires the existing exact live Session/manager/runtime/notifier admission match; then:

- `suppressed=true` returns `SuppressedCalleeReturned` without changing `flag90` or inspecting table/key/receiver/timer state.
- `suppressed=false` clears `flag90` and returns `UnsuppressedDispatchRequired`, which explicitly does not claim key lookup, receiver delivery, timer stop, or slot completion.

`requireRefreshDelivery` now consumes that prefix helper and remains `[[noreturn]]`; it continues to refuse at the same unavailable `+0x458` or timer/UI boundary. `write.cpp` was not changed, and no public Save path was enabled.

## Focused coverage

Extended `tests/doe/test_refresh_notifier_prefix.cpp` with cases for both initial `flag90` values in the suppressed arm, unsuppressed reset with dispatch still outstanding, and foreign/released Session-scope refusal before scalar effects. The suppressed case deliberately keeps the table field unavailable, checks that keys/attempts and receiver/timer metadata do not change, and confirms the public Save guard still refuses.

Static preflight: the test includes the project Testkit entry point `int main() { return testkit::run_all(); }`; uses supported `CHECK`/`REQUIRE` macros and no C `assert`; CMake's existing `tests/*` plus `test_*.cpp` `CONFIGURE_DEPENDS` glob includes this test in its `doe` target. Header paths resolve under the target's `include/` directory. `git diff --check --no-index NUL` reported no whitespace errors for the changed test. No build or test was run, as instructed.

## Frozen file hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_table_owner_state.hpp` | `5F8A09C3B2BD0EFDEBE39925376350F2F3C956FA091F765FCB417912409A0885` |
| `source/apps/moproj2mst/src/doe/table_owner_state.cpp` | `2AB58B2564D08330978679A0FBACAA7C57BEBEB295F394180054051983F4E674` |
| `source/apps/moproj2mst/tests/doe/test_refresh_notifier_prefix.cpp` | `014624EB9EDB28EA92B65AEA871D2DD2F9EE895C73B59208780CFC6C2A60192D` |

The tests' `suppressed=true` is a diagnostic branch fixture only. No same-Session production producer for that value is claimed, and the isolated outcome does not certify native/public Save continuation.
