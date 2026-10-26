# CP94 independent review: duplicate-name Variable rows

## Result

The frozen duplicate branch matches the recovered distinction between helper lookup and reader continuation. Existing-name lookup avoids creating or selecting a new record, while the caller continues to apply ID/dimension/Description setters to the last list record. The candidate models that on the coherent concrete Study owner; it does not update the matched non-last record or append a duplicate. No correction indicated. Read-only review; no build or tests were run.

## Existing-name path and state order

The helper first verifies that Model.vars and headerVariableOwners have the same length and exact non-null borrowed-pointer identity in order. It checks this before following aliases. It then checks the name against the owned records. On a match it selects headerVariableOwners.back() as the reader target and skips concrete allocation, owner/list append, name assignment, selected-index update, invalidation and numSamples change. The common reader continuation applies only the admitted positive ID, dimension 3 and nonempty direct Description values. Empty Description leaves prior text intact.

This is consistent with the native helper behavior: same-name branch returns the first matching record and skips the new-object block; the reader ignores that return and independently selects count-1 for following fields/children. For A,B,A, the candidate retains two objects; A stays unchanged and B receives the last row's ID and Description. A,A remains one object with the last row's ID/Description while its original sample count/name/identity persist. Case-sensitive content equality and duplicate positive IDs follow the bounded profile.

The test that sets selection and invalid flags explicitly before a duplicate confirms the helper-side state is unchanged; sample tables also remain. It does not imply native QString temporary/ref allocation parity. The signed count/index admission bound remains explicit, including duplicate path as a reconstruction admission constraint.

## Guards, partial effects, and public paths

Foreign, null, reordered, mismatched aliases and mixed Keyword/Geometry children still refuse before the duplicate's setters. The three obsolete duplicate-refusal fixtures were replaced with an unqualified ID/payload row or mixed Keyword row, preserving their partial-effect intent without masking the newly implemented duplicate path. The remaining notifier fixture remains an exact Keyword payload case and stays guarded before notification.

A later unqualified row retains duplicate ID/Description changes to the last record while all indexes remain unfinalized; a later top-level Constraints guard retains completed variable reindexing. Same-owner lazy loading continues ordered samples/current PAR and produces no notification attempts. Save still refuses at nonempty-variable dimension reconciliation before XML open, preserving owner identities and old SUB DOM; no public Save, output, or refresh path was widened.

## Disposition

No correction indicated. Keep the branch scoped to coherent concrete owners and the existing header/Description profile. Keyword/Geometry/LinkTo, notification, duplicate helper ref/allocation timing, and public Save/output completion remain outside scope.

