# CP163 editor allocation-only plan review

## Decision

Source-ready for the stated post-query allocation-only prefix, subject to the bounded profile and acceptance checks below. The proposed stop is correctly placed after normal A publication and before selected-length inspection/cache11 publication. It advances CP161 by one native producer effect while leaving the later reader and timer work guarded. This is a static plan review; no source, tests, build, original execution, GUI or live REA was touched.

## Evidence and scope

The plan's CP161 baseline is pinned to source/test hashes `dc56989070aa3a934580eb4ab3c4e5230c956a6d3747fd102a23ccbb3fe4c829` and `c3168dba07b1cff6ce6f87aa3d219f6d50b690acc6a77e95126e886ede3fa82d`; CP161's source review documents the actual leased reader, ready/current directory receipt, pre-query `>=6000` admission and real four-output `step_dir_sizes` call. The plan retains these conditions and keeps the query effect before any new header/count refusal.

The cited native loader ranges establish the relevant order: the actual A allocation and publication precede the selected-region test; cache11 is written only in the later empty-region branch, and DB read/cache publication is later still. The retained allocator/diagnostic disassemblies support the distinction between normal null return (diagnostic then null publication) and a non-returning/throwing path (no publication). The existing `bc::calloc_doubles` uses `std::calloc` and `step_free_contents` uses `std::free`, so candidate ownership is coherent. The plan correctly avoids claiming private-heap or original CRT failure parity.

The related CP143 producer review is useful corroboration for the count guard and owner lifecycle, but is not authority to copy its broader empty-region cache commit into this editor lane. CP163 explicitly excludes that commit. Its allocation-only contract also leaves `u1b74`, MAR/header metadata, coordinate content, arrays timer, selected-length effects, DB/body reads and Stop unchanged.

## Required implementation and acceptance boundaries

- Keep CP161's admitted query path and all owner, reader, receipt, index and version guards. Only after the query, allow the same actual step through the stated high-version/dimension/name-state profile, positive header fields, cache0, fresh A and defined count range. Do not suppress the query's version effect for a profile that CP161 already admits.
- Use the existing `bc::calloc_doubles(count)`, publish its return directly to the actual step's `u1b78`, and return `CoordinateSync` immediately. A normal null return is published as null; an exception/non-return leaves the store unreached. Do not add a temporary owning wrapper, rollback, cache11 write, or assertion that candidate allocation failure proves native failure behavior.
- Keep the selected-region length out of control flow in this cut. Both zero and positive lengths must stop at the same point. This means no cache1 fast path is enabled on retry, and a later invocation with preexisting A must retain the established refusal.
- Test fresh allocations for counts 2/3/6 and zero contents, all requested record versions, and selected lengths zero/positive. Check the exact query version effect, retained pointer/content on a second fresh-page invocation, cache0, unchanged timer and coordinate data, unchanged metadata, one getter/dispatch, and absence of DB read/Stop. Include the listed negative, boundary and overflow counts plus existing-A and missing/zero/negative-header refusals; query-admitted cases must still demonstrate query-before-refusal.
- Preserve CP161's lower-version, physical-index, receipt and owner failures, plus CP158/159 completed-cache and table/registry/foreign-node controls. No changes to public structures, C ABI, timer or viewer are implied.

## Remaining qualifications

The plan does not establish strict original allocator failure-owner/runtime parity, called-handler exception outcomes, CRT errno/new-mode state, or all exceptional continuations in the diagnostic path. Those are explicitly qualified and do not block this normal-return, bounded positive-count producer cut. Do not widen acceptance to OOM claims or native parity without the specified same-occurrence original witness.

The review does not establish full reader completion, cache11 semantics, array assignment, public Save/Open reachability, or application recovery completion. Root validation should compile and run the focused `m2m_mdt_simcontrol300_stopping_cached` target and its stated DB/query support regressions before source-stable full-suite validation; this review performed no checks.
