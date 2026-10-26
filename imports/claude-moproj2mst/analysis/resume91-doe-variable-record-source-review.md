# CP91 independent review: header-only DoeVariable owner

## Result

The frozen source introduces a concrete Study-owned header record and publishes its Var alias only after securing its concrete owner. Its bounded import follows the qualified fresh mode-0/header-only profile and preserves the existing guards for unsupported child payloads and nonempty Save. The source hashes match the implementation receipt. No correction is indicated. This was read-only; no build or tests were run.

## Constructor, append, and field order

The implementation uses a distinct HeaderVariableRecord owned by unique_ptr<that concrete type>; it does not delete the derived record through Var, whose destructor is nonvirtual. The 0x48 native allocation size is retained as metadata, not asserted as host object size.

For the admitted mode-0 row, constructor state matches the retained record values: null name/description, numSamples from the row, f14=0, index=-1, number=0, geoType=100, dimension=3, empty lists and null link. Reader admission requires exact DoeVariable tag, nonempty name, present numSamples/varId/dimension, canonical positive int32 sample count and ID, exact dimension 3, empty/absent LinkTo, and no child nodes. It requires mode 0 and no existing variable/prefix owners. These are deliberate reconstruction bounds, not general native validation.

The helper secures the concrete owner, appends the borrowed Var pointer, then assigns name, selected index 0, all three sample invalid flags, and finally the positive ID and dimension. If pointer-list append throws, it removes only the uncommitted owner; after alias publication, later failures retain the record. That is host lifetime protection, not Qt allocator/COW fault parity.

The native creation helper's false assignment flag still performs the next-number query. On the admitted fresh record, constructor number is 0 and the query computes 1, but the setter is skipped; the reader then stores varId. The candidate correctly represents the persistent result rather than inventing an event. The outer reader reindexes after the full DoeVariables child scan; normal top-level completion still reaches its separate ID tail. The admitted ID is positive, so the existing tail preserves it. The tests directly check initial selected index -1, post-commit selected index 0, invalidations, index 0, number and same owner alias.

## Guard and partial-effect checks

- Missing/noncanonical/overflow values, unsupported dimension, nonempty LinkTo, and any child node (Keyword, Geometry, Description) retain the matched-row error before owner publication.
- Nonzero mode, preexisting Var aliases/owners, and retained scalar prefix refuse without replacing an owner. A second exact row refuses, preserving the first record and its pre-reindex index -1; this intentionally avoids native same-name lookup/duplicate-row semantics outside the fresh-owner cut.
- A later Constraints refusal keeps the admitted record and completed reindex, but prevents later siblings and the outer number pass. Actual outer-ID execution is not independently distinguishable for this fresh profile because admitted IDs are positive; the source retains the existing tail.
- The active Session fixture checks the same owned Study and alias, then the established LHS/FF/USR writes and current DEF_DOE.PAR read, with no variable notification attempts. It is a source-backed candidate path, not native runtime parity.
- Same-owner Save remains at nonempty-variable dimension reconciliation before XML open. The test checks record identity, old DOM, no text/XML output and no later project write. Existing InputTable writer refusal also remains. DOE output geometry dispatch still has its nonempty-variable mapping guard; no public output consumer or serializer was enabled.

## Disposition

No correction indicated. Keep the scope narrow: one fresh mode-0 header row only. Duplicate-name lookup, Description, Keyword/Geometry payloads, LinkTo resolution, general numeric conversion, Qt pointer-list allocation/refcount behavior, and public Save/output completion remain unclaimed.

