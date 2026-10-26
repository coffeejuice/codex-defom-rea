# CP86 independent review: DOE region-name reader

## Review result

The reader change matches the qualified normal name-list branch: it clears the existing names at the RegionsOfInterest encounter, then appends only immediate element children with the exact tag Region, in source order. It preserves duplicates and name values, including absent/null versus explicit empty attributes. Constraints and variable payload guards remain effective. The same-owner Save test demonstrates immediate InputTable XML recording before the still-blocked refresh boundary. No source correction is indicated. This is a static review; no build or tests were run.

Reviewed files:

- source/apps/moproj2mst/src/doe/doe_study.cpp
- source/apps/moproj2mst/tests/doe/test_region_name_reader.cpp
- source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp
- analysis/resume86-doe-next.md

## Reader behavior and partial order

The RegionsOfInterest branch clears s.regions first, then iterates the parsed node's immediate children. It filters null/non-element nodes and exact-matches Region; it does not recurse into a Group or accept a case variant. Each match appends name with a null default. There is no trim, deduplication, nonempty-name check, or file/geometry validation. The reader then reports true. The host abstraction preserves the intended name state/order, but it does not claim Qt container allocation/refcount fault parity.

Coverage verifies clear/replace behavior, text/comment/CDATA and unknown children being ignored, exact case, nested exclusion, duplicate and whitespace preservation, Unicode XML attribute value, and the difference between absent and explicit empty name attributes. Repeated RegionsOfInterest children clear again, so the last reached list replaces the earlier one.

The clear and each append happen incrementally. A later nonempty DoeVariables guard leaves the completed region prefix and prevents CP84's outer ID finalizer. The later Constraints branch clears its count, then refuses populated payload; names and empty-DoeVariables reindex effects remain while IDs remain unchanged. An earlier guard and malformed XML leave the preexisting region list intact. The three adjusted older tests retain their original intent by asserting these later guarded payloads after the now-supported region reader.

## Session owner, Save effects, and payload boundary

The active fixture reaches the reader through a real ConversionSession, active Type2 SubProject, ownedDoeStudy and its doe alias. It checks replacement of the prior list, preserved owner identity, and continuation through the ordered LHS/FF/USR sample writes and current DEF_DOE.PAR read. Vars/constraints remain empty in this fixture.

The Save fixture uses that same owner and confirms the InputTable XML attempt opened and closed, reached the refresh boundary, and returned normal writer success. The recorded bytes contain the three ordered region names, including the empty-name element. The refresh-owner guard still throws, the old DOM Sub_Project is not replaced, and no replayable project write is produced. This is evidence of an immediate recorded partial Save effect, not successful public Save completion or durable-file parity.

The source/test path only reads and writes the region-name projection. It does not construct or validate the native region record's pointer payload; original reader passes null for that pointer. Output geometry consumption remains a separate guarded path (finishCurrentOutputClassData has only its bounded first-name prefix before the geometry payload guard per the CP86 evidence). No readiness or geometry ownership should be inferred from the newly loaded names. Public populated variable/constraint, DoeOutput geometry, Type3 and refresh delivery guards remain intact.

## Disposition

No correction is indicated. The supported claim is name-list loading on the current Study owner with source order and clear/append partial effects. Native QString/QList allocation-failure behavior, geometry payload availability, and completed public Save remain outside scope.

