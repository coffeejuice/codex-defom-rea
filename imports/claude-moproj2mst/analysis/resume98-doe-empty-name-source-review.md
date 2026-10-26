# CP98 explicit-empty-name implementation review

The frozen implementation matches the bounded native evidence. Static review found no blocker; no build or tests were run here.

## Native value and owner behavior

`resume98-doe-empty-name-evidence.md` traces both quote forms through the retained QtXml parser: an immediate closing quote materializes a zero-length value through QString resize/data, producing shared-empty value E, distinct from CP97 default-null N. The DOM attribute setter/getter copies that value without converting it to null. The native reader has no empty-name validation. Existing name lookup compares content, so null and empty compare equal; on a duplicate, the helper skips name assignment and the parser's following setters target the last owner.

The source now removes the presence-gated `name.isEmpty()` refusal and continues to assign the parsed/default `name` to a newly allocated record. That preserves nullness for a missing name and nonnull emptiness for a parsed `name=""`; it creates no synthesized name. The equality scan and last-owner target are unchanged. The header comment explicitly distinguishes absent/null and parsed-empty values while keeping the scope bounded.

## Tests and preserved guards

The six new cases cover double and single quotes, empty-first and null-first cross-value duplicate matching, unchanged first-creator nullness, no duplicate owner or sample-count replacement, empty/Named/empty last-owner targeting, partial state before later list/sibling guards, and actual Session lazy sample continuation followed by the unchanged Save reconciliation refusal with no XML/notifier effects. The “present empty” case is now admitted only with otherwise-valid sample metadata.

The old explicit-empty refusal was removed from the absent-name test's guard vector. The old broad-profile guard vector still contains `<DoeVariable/>`, which remains unsupported because required `numSamples` is absent, and retains malformed samples, LinkTo, Keyword, and Geometry refusals. The new empty-name guard vector likewise retains missing/invalid samples, payload, and foreign-owner checks. No public Save, owner topology, shared interface, or CMake change is part of this cut.

## Boundaries

This is normal value-semantics support only. It does not claim Qt DOM/QString reference-count, allocator-failure, or cleanup-order parity. Numeric conversion remains restricted to canonical positive samples/IDs and dimension 3; mixed payloads, arbitrary Qt conversion, and public Save completion remain guarded. Root owns build/test validation.

Reviewed files: `source/apps/moproj2mst/src/doe/variable_record.cpp`, `source/apps/moproj2mst/include/m2m/doe_variable_record.hpp`, `source/apps/moproj2mst/tests/doe/test_variable_empty_name.cpp`, `source/apps/moproj2mst/tests/doe/test_variable_absent_name.cpp`, and `source/apps/moproj2mst/tests/doe/test_variable_record.cpp`.
