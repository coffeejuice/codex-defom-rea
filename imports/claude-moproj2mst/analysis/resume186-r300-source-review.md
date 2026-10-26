# CP186 R300 InterObject source review

## Verdict

**Pass for the proposed helper-level lexical borrow change; no source-review blockers.** Compiled/runtime acceptance remains root's integration responsibility. This review ran no build or executable tests and used no GUI, app control, or REA database.

Reviewed `resume185-r300-next.md`, `resume186-r300-interobject-plan-review.md`, `resume186-r300-interobject-implementation.md`, current production/test sources, and retained `source/docs/re/moproj2mst/s/spec.md` sections 1–2. The latter qualifies the recovered native sequence and writer twin; this review supplies no new native observation.

## Identity and scope

Current SHA-256 values match the implementation release:

- `source/apps/moproj2mst/src/engine/save_current.cpp`: `35e58afda87b9beab10c39b74197dd6a0bde0c99627351a7e8db01df2212f3ab`.
- `source/apps/moproj2mst/tests/s/test_save_current.cpp`: `5c965f9622c72e1970ea1ebdf9f7a466db2ade6584519741a6fe42549934410e`.

An independent read-only comparison against `build/wave3-checkpoint185-verified/source.zip` shows the production delta confined to the `saveInterObject` closure. The test delta consists of three include/declaration additions and the new case. The predecessor's material fixtures remain unchanged. The implementation report reconciles the earlier proposal/review identities with this frozen predecessor; no residual unexpected source delta appeared in this comparison.

## Borrow, ordering, and pair effects

`save_current.cpp:528–542` performs exactly one direct `opModel(p, op)` lookup. Its immediately invoked callback takes the nullable raw step, rejects null before any lubricant, flag, path, or writer work, and returns only bool. For a present step, order remains optional `rebuildSystemContacts`, `contactFlagsFromEngine`, `interObjectPath`, `writeTaskKey(..., 1, ...)`, then `r == 0`. Both existing file/stream serializers are retained. No geometry or Stopping guard was added.

“One lookup” applies to this helper body: when enabled, `rebuildSystemContacts` still performs its existing transitive lookup through `withOpStep`. The rebuild clears/repopulates the existing step's contact list, imports an optional lubricant template, and appends default pairs/mirrors; it does not replace the step owner. The outer borrow consequently observes rebuilt contacts before flags and serialization. `contactFlagsFromEngine` retains its kind-2 slave/kind-1 master flag selection and enable behavior. The changed wrapper adds no pair filtering or reordering.

`writeTaskKey` invokes its selected writer inline. `captureKey` also invokes its callback inline; neither stores a writer or raw step. The file writer adds the banner and delegates to the same stream/group writer. That writer iterates list order, stops on the first nonzero result, and preserves `write_pair` filtering (zero IDs, slave less than master, and equal-ID kind-1 records). The existing native prewrite hook remains unchanged; lexical confinement does not establish safety against hypothetical reentrant step replacement, and the report correctly excludes such a claim.

## Fixture and restoration

`save_interobject_step_scope` follows the existing operation-2 corpus load sequence and requires a real step with two loaded objects. It sets `lubSystem=false` for its positive writer probe, preserving any existing pair list. If empty, it creates constructor-initialized records with actual object IDs: kind 2 and a copied, reversed kind-1 mirror. IDs are nonzero, distinct, bounded for flags, and checked against the loaded object list. The two opposite orientations ensure one survives the existing slave/master serialization filter. Expected INICTC flags are derived with the retained kind-specific rule.

Assertions cover helper success, exactly one captured write at `interObjectPath`, nonempty bytes, no deletes, expected flags, stable owner identities, and unchanged pair count. Nonempty bytes alone could include the banner; the report appropriately claims helper mechanics rather than contact-byte equivalence. The positive system-lubricant rebuild branch is inspected statically and is not exercised by this case, as explicitly stated in the implementation report.

The noncopyable detached-step guard holds the original unique owner and moves it back in its noexcept destructor. While detached, only this helper is invoked; `lubSystem=true` strengthens the refusal check. Normal and explicit exception paths verify restored model/step identity and count, with no captured effects or flag mutation. Testkit assertion unwinding also traverses the destructor. The detached list reference stays valid because the guard retains ownership of the same heap object.

These results support the bounded helper refactor only. They do not establish public Save dispatch/admission, native byte equivalence, aggregate closure, cache1 publication, or a model-wide parked barrier. Root should complete the focused test group, source-stable regression, and freeze before runtime acceptance.

Only this report was written. Token measurement unavailable.
