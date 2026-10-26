# CP254 next finite Editor300 leaf

Move the existing `springComboItemLabels` producer in `simcontrol300_lifecycle.cpp` into one finite GuiModel operation returning only owned label values. This removes the object/name native aliases from the lifecycle producer without changing SPRING behavior, broad meshing, cache1 admission or Qt delivery. It is independent of CP253's initial dimension preflight and needs no new original witness for this source-preserving migration.

Only this report is written. No source/test/build, shared recovery state, original process, GUI or mutable REA state changed.

## Current-source reconciliation

Title/frequency/StepCount owner commands, page/lifecycle identity queries, CP245 reaction coordinator and CP248 meshing writeback are already migrated. CP249 heating and CP251 MultiBlow belong to their own completed property/loader cuts. CP253 covers initial dimension admission; do not reassign it. Current lifecycle still exposes the finite object/name traversal in its anonymous `springComboItemLabels` helper, plus separate initial control and backup owner aliases. Select only the label helper here; do not merge native backup copying, whole page Fill, object incarnation issuance or CP165 meshing into it.

## Actual producer and call site

Current helper at `src/engine/simcontrol300_lifecycle.cpp`:

- Reads objectCount once.
- Refuses count outside 0..4096 before any object lookup.
- Creates the zero/None formatted placeholder.
- Requests current object numbers 1 through count; missing object refuses the whole result.
- Skips native kind 7.
- Scans the name at +0x828 for NUL within 0x54 bytes, checks printable ASCII, reads the signed object number at +8 and formats through the existing 64-byte `%-3d - %.*s` buffer.
- Copies the completed first label vector into second and returns both with current indices zero.

The `Page::Extra3D` Fill branch passes `[&model] { return springComboItemLabels(model); }` to the deferred `fillExtra3D` overload. It must remain deferred: the current caller test intentionally creates objects during the final scalar sidecar read, and labels must observe them afterward, before spring rows are read. Do not precompute at editor preflight, initial page entry or lambda construction.

## Proposed value-only owner boundary

Add a forward declaration of `simcontrol300::SpringComboLabels` beside the existing page declarations in engine.hpp, then a named reference-output method such as:

```cpp
bool GuiModel::copyEditorSpringComboLabels(
    simcontrol300::SpringComboLabels& result) const;
```

Reference parameters avoid incomplete optional instantiation/include cycles. The complete existing record lives in `m2m/engine/simcontrol300_io.hpp`: two vectors of owned qt::String plus two integer current indices. Include that header in gui_model.cpp as needed; do not include it back into engine.hpp or create a native pointer-bearing record.

Move the existing helper algorithm intact into this method, using a local `SpringComboLabels labels{}`. On the current nullopt paths return false without modifying result. Only after the entire traversal and `labels.second = labels.first` succeeds move the complete labels into result and return true. Preserve ordinary exceptions; no noexcept/catch/rollback guarantee is added. An assignment allocation exception is not a normal false result.

The object pointers, name pointer and temporary string_view remain synchronous locals inside the method and never escape. It returns no native alias, callback or opaque key. Keep lookup by the current object number exactly as today; it is not a claim of identity with an earlier Fill incarnation.

Replace the deferred lambda body with a local initialized output, one call to the method, nullopt on false and the completed value on true (give the lambda explicit `std::optional<SpringComboLabels>` result type). Remove the old anonymous helper rather than leaving permanent forwarding machinery. Keep all fillExtra3D calls, scalar/spring I/O order, labels/selected-row parsing and page report handling unchanged. Remove now-unused lifecycle includes only after scoped search proves they are unused; cstdio/string may have other consumers.

## Exact guard and formatting requirements

Do not add a whole-method geometry guard. objectCount is currently unguarded, and the first geometry check occurs only in each object(i) lookup. Thus a parked model with count zero can still produce the placeholder, while a positive-count parked traversal throws at its first object lookup. Negative or oversized counts refuse before lookup. Moving a guard to entry would alter these reached outcomes. Keep one count read and one existing guarded object lookup for each visited index; do not substitute a precollected object list/snapshot or repeated count query.

Keep null-hole refusal, kind-7 skip, name scanning, ASCII and buffer caps, signed object number and vector order exactly. NUL must appear before index 0x54; n<0 or n>=64 from snprintf refuses. Empty terminated name is currently admitted. Do not rename None, change padding, sort/deduplicate labels, use QVariant item data or UTF-8 conversion, broaden encoding/length admission, or append partially accumulated output on refusal. Both result indices must be reset to zero on success rather than preserving caller sentinel indices.

## Meaningful tests and ownership

One owner needs engine.hpp, gui_model.cpp and simcontrol300_lifecycle.cpp, serialized with any other common API owner. Tests can stay in `tests/mdt/test_simcontrol300_lifecycle.cpp`, which already includes native layout helpers and the real producer caller. No new target is needed.

Retain `initial_editor_spring_combo_projection_observes_model_after_scalar_sidecar_reads`. It observes the exact scalar-call sequence, creates objects only at STRAIN_DST, then requires selected IDs 10/20 from the later producer. An eager projection would fail it. Add assertions for the resulting owned combo label vectors where accessible, including exact placeholder `0   - None`, identical first/second catalogs and zero initial current indices when a direct producer call is used.

Add direct owner-method cases:

- No objects gives exactly the placeholder in both vectors, with both indices zero, replacing a distinctive prior output sentinel.
- A small dense numbered fixture with distinct ASCII names includes eligible objects in request order and skips kind 7. Assert exact padded strings, not merely parsed IDs. Seed/verify kind fields explicitly rather than assuming objectWrap's defaults.
- A missing numbered lookup within reported count returns false with every output field unchanged. Construct the bounded native fixture deliberately; do not change production lookup/count policy to make the test convenient.
- Nonterminated 0x54-byte name, non-ASCII byte and formatted length at/above 64 each refuse with unchanged output; a just-admitted ordinary name succeeds. Retain/restore modified native name/number/kind fields where later cleanup/work needs them.
- Negative/over-4096 reported count refuse before traversal; do not create thousands of objects solely for a cap test. Restore any temporary count metadata immediately and retain allocation ownership.

Use Testkit CHECK/CHECK_EQ/REQUIRE, no C assert or public raw test hook. Check includes against actual target directories. Existing SPRING page I/O tests (`tests/s/test_spring_page_io_roundtrip.cpp`) supply selected-row parsing/first-match/repeat transition regressions but do not alone test this producer.

A narrow extension of the existing parked-viewer fixture can assert positive-count call throws through its existing refused helper and output remains unchanged. A separately valid parked count-zero setup can assert placeholder success if practical; do not corrupt an active fixture's native count just to claim it. If not added, qualify that zero-count/no-entry-guard behavior by source inspection rather than claiming runtime coverage. No new GUI-control session is involved.

Root focused group: existing lifecycle and SPRING page I/O targets, plus viewer_box if extended; confirm configured target names (`m2m_mdt_simcontrol300_lifecycle`, `m2m_s_spring_page_io_roundtrip`, `m2m_mdt_viewer_box`). Retain stopping_cached in integration and complete source-stable suite/freeze. No results are claimed here.

## Original/source qualification

CP119 and CP120 retain original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, setup `0x140ec8b00`, builder `0x140ec98e0` and parser/selection captures. **Read CP120's placeholder reconciliation:** CP119's earlier bare-None statement is superseded; zero goes through the same width-three formatter, yielding `0   - None`. The builder capture byte hash is `b2376f94e7b476ea5a1d5d98a6d3326babd96534ff6dc4b17ee7aac6debde0c0`.

CP120's integration review establishes the bounded static producer sequence: accumulated list is abandoned on null before insertion into either combo, eligible objects use number/name and both catalogs receive the same visible text. The 4096 cap, NUL/ASCII profile and 64-byte candidate formatting buffer are reconstruction admission rules, not original safe limits. Preserve them; do not use this refactor to claim native acceptance of arbitrary encoding, long/unterminated names or actual QWidget population. Original parsing uses visible item text, not item data.

This is a reconstructed owner boundary, not a new original symbol/interface, fresh differential witness, physical Qt/reentrant proof or full editor/Save completion. ObjectKey/currentness and broad ownership closure remain unresolved. No R300 cache1 or original accepted-Close witness gate changes follow.
