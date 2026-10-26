# CP150 design review: third fresh geometry recipient

## Result

The retained caller loop supports a third fixed-row recipient in the same initial high-version frame: after each helper return it reaches the same loop backedge, rechecks the current count, reloads the next object and MAR input, and invokes the helper again. The original code does not branch on a recipient limit of two. A fixed-capacity three-entry private receipt array is therefore a bounded candidate admission limit, not an invented native third-object behavior.

No design blocker found. The plan includes the key live-identity rule: recover each prior recipient from the current step's object table at its recorded ordinal, compare against the receipt identity before dereferencing it, then recheck its owner/allocation/registry/dimension receipts before third-recipient writes. It also correctly calls out a stale CP149 negative test whose third object is currently fully qualified and would now succeed.

## Checks

- **Native ordering:** the retained `d9b4..da47` caller region increments and compares against the actual step count, resolves the current object and MAR inputs, applies caller skips, then calls the node helper at `dbe9`. After return, control reaches the same `dc66 -> d9b4` backedge. Stop/AL1 occur only after exhaustion. There is no native two-recipient counter or first/second-specific branch.
- **Bounded storage:** extending the private recipient receipt array from two to three and replacing each capacity check with one named constant preserves fixed storage and separate stages/receipts. No public setting, vector allocation, generic N support, new query, arrays checkpoint, or A source allocation is needed.
- **Per-recipient proof:** each of the three must have its own actual object identity/ordinal, fresh three-double allocation, six BC attempt records, actual 28-entry receiver/body/dimension progress, and post-write 24-byte source/destination receipt. The frame-wide A allocation remains shared and valid at offset zero for every first1/d3 row. Each iteration rechecks all earlier completed records.
- **Failure retention:** third fresh admission refusal leaves its object untouched while preserving first/second writes. A refusal after third reset/allocation/lookup preserves only reached third effects and both earlier copies, with no Stop, no retirement and no retry on later refresh. A fourth positive object is refused before its reset/allocation.
- **Test discriminator:** distinct source rows A/B/C seeded at each actual final lookup and separate destination sentinels prove source selection and non-replay. Three distinct object/primary/positive-BC/registry identities, 84 receiver observations, one arrays checkpoint/one Stop, optional getter counts, and cached refresh checks cover normal completion. Identity swaps during the third's partial lookup must use a real spare object and RAII restoration; the prior object is re-looked up live before field access.

## Stale-test requirement

The existing CP149 `geometry_two_recipient_copies_remain_before_third_positive_or_later_MAR_guard` positive branch constructs a fully qualified third object with `freshMain(candidate, 4)`. Capacity three should now admit it. That branch cannot keep expecting a refusal. Move the positive-count refusal to a fourth fully qualified object and require the first three copies to remain, or intentionally make the third case unqualified and name/assert that narrower guard. The malformed-MAR branch still tests a valid later preload refusal, but its expected copy count and object ordinal should be updated for the third-recipient flow.

The other test cohorts should distinguish a true fourth-positive guard from a third-recipient fresh-admission failure. A third width/owner/primary guard must be reached only after preserving two prior copies; a fourth-positive refusal must preserve all three. Do not silently weaken the new third admission to keep old expectations green.

Nonempty variable resize/unwind remains correctly deferred. Existing size-zero leaf evidence does not qualify positive-size allocation, publication, or exceptional cleanup. No arbitrary N, wider row, offset, preexisting destination or source-span case follows from this extension.

This is a read-only design review; no source/build/test, GUI, original execution or REA action was performed.
