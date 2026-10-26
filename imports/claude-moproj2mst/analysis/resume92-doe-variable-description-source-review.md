# CP92 independent review: DoeVariable Description

## Result

The frozen implementation follows the bounded direct-Description path on CP91's concrete Study-owned record. It applies nonempty Description text after record publication and header fields, preserves default-null on empty text, and visits admitted Description children in source order. The borrowed Var alias and all public guards remain intact. No correction indicated. Read-only review; no build or tests were run.

## Source and order checks

The helper still requires a fresh mode-0 owner, canonical positive header fields, dimension 3, empty/absent LinkTo, and one row without unsupported direct element children. Its structural precheck allows only exact direct Description elements (plus raw non-element nodes) before allocation. Thus Keyword, Geometry, unknown/case-variant tags, nested Description under an unknown wrapper, and mixed Description-plus-unsupported payload remain pre-append refusals. The mixed-row refusal is an explicit reconstruction bound, not a claim of native partial ordering.

After concrete ownership and pointer-list alias publication, the helper assigns name, selected index, all three invalid flags, ID, and dimension. It then iterates direct row children in encounter order, skips raw nodes, obtains text only for admitted Description elements, and assigns only when nonempty. Empty Description preserves constructor-null or any earlier nonempty assignment. Repeated nonempty values overwrite in order; whitespace-only content is retained; no trim or newline conversion is applied. The existing xml::Node::text seam is used, so nested descendant text/entities/CDATA are included while comments/PI do not contribute.

Tests cover default-null behavior for empty/raw-only descriptions, nonempty and repeated assignments, empty nodes following nonempty values, space-only CDATA, LF preservation, Unicode/entity/descendant text, direct case and nested/mixed guards, partial records before later row/sibling refusal, the same actual Study owner/Var alias, lazy sample continuation, and Save refusal before XML open.

## Owner and public boundaries

The Description field belongs to the concrete HeaderVariableRecord retained by Study; the public Model.vars pointer remains an alias to that same record. No new owner, notification, reset, or readiness token is introduced. The actual Session fixture confirms same Study identity and no notification attempt.

Nonempty-variable Save still refuses at dimension reconciliation before XML open; the test retains record identity and old SUB DOM and observes no XML/text/project write. Keyword/Geometry parsing, LinkTo resolution, multiple-row/duplicate-name behavior, public output geometry and Save completion remain guarded.

## Disposition

No correction indicated. Keep claims to this header/Description profile and the host XML text seam. Native QString allocation/ref behavior, mixed payload failure timing, Keyword/Geometry/LinkTo processing, and public Save/output completion remain outside scope.

