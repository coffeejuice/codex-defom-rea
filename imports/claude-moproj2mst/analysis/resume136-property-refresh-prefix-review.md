# CP136 independent property-refresh prefix review

## Result

No source blocker found in the frozen helper/test relative to the CP135 native ordering contract. The helper remains a declared-observation model; it does not claim Qt activation/dispatch, Boolean E, OPI, Apply, or public lifecycle integration.

## Contract checks

- `planPropertyRefreshNotifier` refuses missing notifier identity/lifetime, occurrence, qualification, suppression state, or (when suppression is zero) nullable payload. A nonzero suppression observation completes without requiring or reading the payload, matching the bounded native `owner+0x141` early-return evidence in `resume135-boolean-property-refresh.md`.
- The local3 emission request is separate from slot entry. The slot frame requires the declared owner mapping plus matching notifier/occurrence/payload and a delivery ordinal before it issues a browser-clear request. No QMetaObject activation or receiver delivery is executed or inferred.
- The payload store occurs only after a matching clear request identity has an explicitly observed normal return. `Interrupted` cannot store, and a later contradictory return is rejected. Null then completes this modeled prefix; non-null stops at `PropertyListPending`, before native property enumeration/refresh.
- The helper does not represent callback effects or claim old state remained unchanged during clear. `observeProperties` only records an external/reentrant field observation; the later incoming payload store remains unconditional, matching the qualified native ordering.
- Tests cover the above barriers, owner/emission identity, wrong payload, early/wrong/missing clear identity, expired admission, interruption, replayed delivery ordinal, null/non-null outcomes, and declared external change. Assertions stay on the model state and explicitly named observations.

## Frozen-file binding

The three exclusive file hashes match `analysis/resume136-property-refresh-prefix.md`:

- Header: `e2b10d1b7bba9119b9d86dc4cdc26f0a6f8d1407497aa74b306ef964800dc97e`
- Implementation: `e4b4387699afc1f0be8f36c1d275a3f660e1ba68f4c5f26b070763837ca21e18`
- Test: `eda30347a035d7f32801b62c63f453a099e3d546582db3bffbb9d375499908d6`

This was a read-only static review. It does not verify compilation or execution; root owns those checks.
