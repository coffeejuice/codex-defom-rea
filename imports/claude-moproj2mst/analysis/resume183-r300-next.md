# CP183 R300 next raw-step borrow: base contact-pair builder

## Decision and exclusive ownership

Close the function-scope `DB_STEP_DATA* s` borrow in `baseDefaultPairs(Project&, Operation&)` in `source/apps/moproj2mst/src/engine/save_current.cpp`. This is a single bounded base-class contact-pair builder, disjoint from CP182's `feDefaultPairs` in `save_l3.cpp`. The pointer is used to clear/read the inter-object list while the function builds a scalar-only `std::vector<ContactPair>`. An immediate synchronous void callback can contain the pointer and preserve the existing return vector.

Reserve exactly `source/apps/moproj2mst/src/engine/save_current.cpp` for production and `source/apps/moproj2mst/tests/s/test_save_current.cpp` for focused test coverage. Verify and record their current hashes before implementation. No header, C ABI, model layout, CP163 source, cache state or installed program is in scope. Root should reserve both files against concurrent edits.

## Exact transformation and order guard

Keep `out` construction, the single `opModel(p, op)` lookup and nullable step acquisition at the current point. If step is null, return the same empty vector without clearing/counting/accessing objects. For a present step, keep `inter_obj.clear()` before `m->objectCount()`, preserve the existing 0-to-n loop, null/kind-7 filtering, candidate selection, object-entry reads, master/candidate/defined counters, dimension/property decisions and pair construction in their exact order. Keep all of those operations inside the immediate void callback; only `out` leaves the callback. Do not capture or retain step, object, pair-list, or entry pointers. Preserve any existing outer Save admission and do not add a geometry or Stopping guard: this local borrow closure is not a parked-model barrier.

## Focused evidence and tests

Use the existing `m2m.s.save_current` test fixture(s) that exercise base default contact pairs. Confirm the exact dispatched class reaches `baseDefaultPairs`; if no fixture directly reaches it, add a narrow helper-level case using a valid admitted operation and real model. Assert the expected scalar `ContactPair` fields and ordering, and seed a nonempty inter-object list so the test proves the existing clear occurred. For the null-step branch, move the actual step out only during a direct helper call, restore on normal and exceptional exits, and verify empty output plus unchanged detached list count and model/step identity. Do not interpret a synthetic direct helper call as public Save or native equivalence.

Static review must confirm one model lookup, null return before effects, clear-before-object-count, unchanged object/pair order and no escaped pointer. Run the focused `m2m.s.save_current` test after integration; root retains full source-stable suite/freeze responsibility. Any assertion must execute against the copied corpus fixture, not merely compile.

## Guard semantics and qualifications

Native source qualification is the documented class base virtual slot `SfOprBase` `vt+0x198`, with the existing source annotation in `save_current.cpp` identifying clear-before-count and master selection; this proposal adds no new native-runtime observation. Original evidence remains DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Do not claim public Save acceptance, same-occurrence reader-tail behavior, cache1 authorization, or a model-wide parked barrier. Other raw step aliases and aggregate routes remain open.
