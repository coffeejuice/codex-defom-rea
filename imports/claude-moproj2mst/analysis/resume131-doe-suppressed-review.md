# CP131 independent DOE notifier review

## Finding

No actionable blocker found in the frozen helper or focused test. The helper is a prefix model only; the public Type2 Save route remains guarded and cannot report successful completion from this branch.

## Source review

- `DoeTableOwnerState::enterRefreshNotifierPrefix` first validates the same Session plus manager/runtime/notifier owner identities, and refuses if the notifier owner is unavailable (`src/doe/table_owner_state.cpp`, lines 233–244).
- The suppressed branch returns before touching `flag90`; this matches the ordering documented for native `2cf3f1` in the helper. The unsuppressed branch clears `flag90` before any key/map/receiver work, matching the stated `2cf408` prefix. Its return value explicitly leaves dispatch outstanding (`include/m2m/doe_table_owner_state.hpp`, lines 23–29).
- `requireRefreshDelivery` remains `[[noreturn]]`. After the prefix, unavailable main-window `+458` refuses; even with that field admitted, timer/UI receiver delivery still refuses (`src/doe/table_owner_state.cpp`, lines 246–253). Thus the new suppressed return does not accidentally continue as a successful Save.
- The public writer `src/moproj/write.cpp` is unchanged in the frozen receipt. Its Type2 route calls `requireRefreshDelivery` after observing the XML writer return and refresh boundary, then reaches the existing unsupported guard if that call were ever to return. The source still does not authorize a public Type2 completion.

## Test review

`tests/doe/test_refresh_notifier_prefix.cpp` covers both scalar values of `flag90` under suppression and checks that keys, attempts, table-owner generation/receipts, and receiver/timer fields remain untouched. The isolated test calls the helper and separately verifies `requireRefreshDelivery` refuses. A companion actual `engine::saveProject` fixture enters the suppressed path after a normal XML writer return and refresh boundary; it checks the `+458` refusal, retained DOM, no further text writes, no project writes, and no notifier attempts. This supports the report's guard claim without equating the isolated helper result to public Save success.

The unsuppressed fixture confirms `flag90` clears before refusal while keys/attempts and receiver effects remain untouched. Foreign and released scopes refuse before either notifier is mutated.

## Qualification

This is static source/test review; CP131 receipt states no build or focused test execution. The `suppressed=true` values are diagnostic fixture inputs, not evidence of a production producer or a native startup state. No key lookup, receiver delivery, timer stop, slot completion, or full Save continuation is established. The source hashes and these limits are recorded in `analysis/resume131-doe-suppressed-prefix.md`.
