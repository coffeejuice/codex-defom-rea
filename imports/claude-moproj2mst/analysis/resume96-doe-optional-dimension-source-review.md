# CP96 independent review: absent dimension setter skip

## Result

The implementation distinguishes an absent dimension attribute from a present value. New records retain constructor dimension 3; duplicate rows with no dimension leave the reader-selected last record's current dimension untouched. Present dimension is admitted only as exact 3 and invokes the setter. This matches the retained branch. No correction indicated. Read-only review; no build or tests were run.

## Field and ordering checks

The bounded profile still requires mode 0, nonempty name, canonical positive numSamples, coherent concrete owner aliases, empty LinkTo, Description-only/raw child elements, and the existing scalar/geometry/retained-owner restrictions. It now records dimension presence separately. A present value other than exact 3 remains a preappend refusal; absent does not mean a present empty/invalid value.

After create-or-duplicate selection and optional varId handling, the candidate writes dimension only when hasDimension is true. On a new row, constructor initialization already provides 3. On a duplicate, omitted dimension skips the store on the actual last record; explicit dimension 3 performs the known store there. No synthesized local-default field or value is persisted.

The tests use a diagnostic existing-record dimension 2 to distinguish a skipped store from an unconditional store. That fixture does not admit dimension2 XML or claim GUI initialization/output behavior. It then checks explicit dimension3 overwrites the last record while absent varId independently preserves its number. New-record, absent-ID/finalizer, mixed-row and sibling-guard cases preserve expected constructor/index/number ordering.

## Public and shared boundaries

The source change does not touch the CP84 outer ID finalizer, owner vectors, Session/reader/writer integration, or public selection. The active owner fixture verifies same Study alias, lazy sample write/read order, and no notification. Save remains guarded at nonempty-variable dimension reconciliation before XML open; old DOM and record aliases remain. Keyword/Geometry, deferred links, general dimension conversion, and public output/Save completion remain guarded.

The obsolete missing-dimension entry was removed from the preappend-invalid vector; present invalid dimension cases remain there. This is the narrow expectation change required by the new absent-attribute behavior.

## Disposition

No correction indicated. Keep claims to the setter-presence branch: constructor3 survives absence; a present exact3 stores3; other present values remain refused.
