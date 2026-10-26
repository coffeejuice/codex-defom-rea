# CP93 independent review: unique-name variable append

## Result

The frozen implementation admits additional header/Description rows only when each name is distinct and the existing Study pointer aliases exactly match its concrete HeaderVariableRecord owners. It preserves the native create/append and reader-last-variable sequence for that bounded profile. Duplicate-name behavior remains guarded because the native helper return differs from the reader's subsequent last-record selection. No correction indicated. Read-only review; no build or tests were run.

## Lookup, append, selection, and invalidation

The candidate checks each prior concrete owner pointer against the corresponding Model.vars alias before dereferencing it, then compares names through the established String value equality. Case-distinct names are admitted; exact duplicates are refused before any new allocation or row mutation.

For a distinct name, it constructs the source-default header record, secures the concrete owner, publishes its borrowed Var pointer, assigns the name, selects the newly appended index, invalidates all three sample modes, and then applies varId/dimension and direct Description values. The name index equals size-1 under the unique-name bound. Parsed IDs are retained, including duplicates and int32 maximum; the helper's false-assignment next-number result is not turned into a generated ID. Existing sample table contents remain unchanged.

The signed count/index bound refuses before append when adding another record would exceed the existing int surface. Owner/list size mismatch, null owner, foreign/null/reordered alias, or incompatible projection owners also refuse without adopting raw Var objects. If alias append throws, only that just-created host owner is removed; committed earlier records and subsequent effects are not rolled back.

## Partial effects and gates

Tests check distinct rows' stable object identities, headers/descriptions/indexes/selection, case-sensitive names, duplicate numeric IDs, sample table retention, exact duplicate-name refusal before current-row changes, later-row payload refusal preserving earlier records at index -1, completed reindex before a later sibling guard, and actual active Type2 Study/lazy sample continuation.

Duplicate exact names intentionally do not reproduce original existing-name helper return semantics or its later last-variable target. They remain guarded, so this is not a claim that the native reader rejects duplicate names.

Same-owner Save still refuses at nonempty-variable dimension reconciliation before XML open. The fixture preserves record aliases and old SUB DOM and observes no XML/text/project write, refresh or notification. Existing variable payload guards, the notifier boundary, geometry mapping refusal and public output/Save constraints remain unchanged; no serializer or output consumer was added.

## Disposition

No correction indicated. The bounded value model supports distinct-name multirow header/Description import only. Native QString/list allocation and failure parity, duplicate-name setter semantics, Keyword/Geometry/LinkTo, and public Save/output completion remain outside scope.

