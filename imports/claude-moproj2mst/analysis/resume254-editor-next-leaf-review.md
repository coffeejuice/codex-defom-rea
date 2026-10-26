# CP254 SPRING label owner-boundary review

**PASS — the reference-output owner operation can preserve the current finite producer and deferred call timing.** Keep the two failure/timing qualifications below explicit during implementation. This is a read-only contract review, not a compilation/runtime result.

Reviewed the proposal, current `springComboItemLabels`, Extra3D's deferred-label dispatch, model count/numbered lookup implementations, `SpringComboLabels`, the scalar-sidecar timing fixture, and CP120 placeholder reconciliation/integration review.

## Value boundary and traversal

The existing record owns two qt::String vectors and two integer indices initialized to zero; it contains no model/native aliases. A forward declaration suffices for `SpringComboLabels&` in `engine.hpp`; the implementation requires the complete IO header. Use a local initialized record and publish by move only after full traversal and copying first into second. Ordinary false paths must leave all prior output fields untouched. Do not add catch/noexcept or promise exception rollback beyond that existing local-build behavior.

Retain one unguarded `objectCount()` read, range refusal before label allocation/traversal, first-vector reserve, formatted zero placeholder, and requests `object(i)` for each integer 1 through count. This is lookup **by number**, not ordinal `objectAt`, and no new incarnation key is issued. Missing numbered object aborts the local catalog. Kind 7 is skipped before touching name bytes. Preserve bounded NUL scan at +0x828, printable ASCII check, signed +8 number formatting, original loop order, duplicate labels and final `labels.second = labels.first`.

Do not alter the 64-byte snprintf rule: negative length or length >=64 refuses; length 63 succeeds if all other admission conditions hold. NUL at name index 0x53 is within the bounded name scan; NUL only at 0x54 is too late. Empty terminated names remain admitted. Preserve exact `%-3d - %.*s` formatting, including placeholder `0   - None`. Reset both output indices to zero on successful direct production, regardless of prior sentinel values.

## Geometry and failure semantics

Current `objectCount()` reads nullable step/u1378 without a guard. Current `object(i)` checks geometry before nullable lookup and `object_list_find`. Therefore do not guard the new method at entry or replace its traversal with an admitted snapshot. Count zero, including an absent step, returns the placeholder without geometry admission. A parked zero-count model has the same behavior by source inspection; positive parked count throws at the first numbered lookup after local placeholder creation. Invalid reported counts return false before any lookup, including while parked.

No accumulated local labels may escape on false or exception. Successful output owns its strings after native objects change or disappear. No native object/name pointer or string_view may be retained in result or model fields.

Important distinction: producer false becomes **nullopt from the deferred lambda**, matching today. In current `fillExtra3DImpl`, absent deferred labels do not themselves immediately abort the entire page: it still calls `readSprings`, and later selection logic follows its existing actual-label-versus-legacy fallback policy. Do not replace lambda nullopt with a new immediate Extra3D refusal, skip the spring read, or manufacture a placeholder success after producer failure. Geometry exceptions currently propagate and likewise must not be converted to false/nullopt.

## Deferred I/O and tests

Retain the lambda at the existing Extra3D Fill call. Invoke the method only when `fillExtra3DImpl` calls deferredLabels, after NBC/SW2SP/conditional STRETCH/B_WEIGHT/STRAIN_DST reads and before `readSprings`. Earlier scalar/locale refusal must still prevent producer invocation; do not precompute during lambda capture or page entry.

The existing ordered-I/O test is a strong caller regression because it creates objects only during STRAIN_DST and requires the later labels to support IDs10/20. Keep its complete read ordering assertions. The combo labels subsequently used by InitialLoad may have selected indices changed from zero; assert zero indices on a **direct producer call**, not indiscriminately on the populated SPRING page. Exact catalog strings/copies and selected IDs should be checked at their respective stages.

The proposed direct tests meaningfully cover placeholder success, explicitly seeded dense request ordering/kind-7 skip, holes, count caps, encoding/NUL/buffer bounds and unchanged sentinel outputs. Include a just-admitted length alongside a rejected length. Construct holes/reported counts deliberately without dangling access and restore temporary native metadata before unrelated work/destruction. This does not authorize production behavior changes to accommodate fixtures.

A positive-count parked call with sentinel output proves the new entry preserves per-object admission; no additional GUI is needed. A valid parked-zero runtime fixture is optional. If absent, qualify zero-count parked behavior as inspected source semantics. Existing viewer tests or arbitrary mutation of a live native count should not be claimed as that runtime witness. Existing SPRING page IO tests cover selection/parsing, not this producer's traversal by themselves.

## Original qualification

CP120 explicitly supersedes CP119's bare-None statement: the captured zero uses the same width-three prefix builder and yields `0   - None`. Its retained static evidence supports abandoned accumulation on missing objects, number/name label text and identical catalogs; actual selection parses visible text rather than QVariant data. Candidate count/NUL/ASCII/buffer caps are reconstruction admission rules, not recovered native safe limits.

The extraction recovers no new original symbol/interface, fresh differential result, physical combo population, arbitrary encoding support, ObjectKey safety, public editor/Save completion or accepted-Close witness. Larger lifecycle/control/backup aliases and Stopping/cache1 limits remain separate.

Only this review report was written. No source, tests, builds, shared recovery state, GUI, original process, or mutable REA state was changed.
