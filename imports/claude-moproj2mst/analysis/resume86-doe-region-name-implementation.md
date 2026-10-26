# CP86: public RegionsOfInterest name reader

The public InputTable reader now loads direct Region names into the existing actual Study projection. It clears prior names at the child encounter, appends exact matching Region/name values in order and returns true. Duplicates, whitespace, missing/null versus explicit empty names are preserved. It does not recurse, create geometry payloads or advance output/refresh/Qt ownership.

## Source qualification and scope

Frozen evidence report: `resume86-doe-next.md`, SHA-256 `450c5f98636fbc35fdf69e6f2dcf80f802bb2078a287521681ec35ae47660198`. It records original PE identity, the full reader/body argument order, manager vtable slots and compact raw-byte hashes.

Original `0x140912b00` clears via manager vt+0xb8 before traversal, matches exact `Region`, reads `name` with a null default and calls vt+0xa8(manager,0,name). The append method `0x14091f1a0` places that zero pointer and QString name in a two-field record, appends and ignores no rows based on name content. Its temporary/record copy helpers touch that pointer and QString, without constructing a widget, file engine or geometry object. Source normal reader status stays true.

The candidate uses `Study::regions`, already consumed by the XML writer and the guarded output name prefix. No disconnected registry or payload capability was added. Its direct parsed-child loop avoids recursive collection and appends incrementally after the original clear. A later variable/constraint guard keeps completed names; an earlier guard or overall XML parse failure prevents the clear. Repeated lists each clear before appending, so the last reached list replaces earlier lists.

Native QList/QString/QDom reference allocation, detach and failure timing remain outside this value/container seam. The native16-byte record allocation is not claimed to equal the host string/vector layout, and the actual null payload owner is not represented as geometry readiness. No native durability or original Unicode-output byte parity is inferred. The name-value parser tests include Unicode; the immediate XML Save fixture uses ASCII names.

## Frozen files

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `f193ee8eeb0de9b7a4510edbde9909c727df662758a1d0d96a4bf6c40cab5fc5` |
| `source/apps/moproj2mst/tests/doe/test_region_name_reader.cpp` | `9a2aa8ccea0d6a780b5855e308b78c44f144fba750b913be71dde20ee33745e8` |
| `source/apps/moproj2mst/tests/doe/test_operations_reader.cpp` | `60f3b06da055b17d6bc5f4ff6a8ef7296ea97272e5fd64161e7222500cd30fcc` |
| `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp` | `c117cd7711d85b7d5fd4c4351accfa74ade0e3846274f29b677388d8654e5f81` |
| `source/apps/moproj2mst/tests/doe/test_empty_variable_reindex.cpp` | `f3ab3583044e49e33bb6dbd5c3bcfb416ad07eb51ad79145d75750df3ff5254c` |

The three existing tests changed only at obsolete region-refusal scenarios: the still-guarded populated Constraints sibling now follows the completed region list, and assertions check the retained names/count-reset. An earlier Constraints guard still prevents a later empty-variable reader where required. Existing variable payload guards and reindex/ID timing are preserved.

No header, CMake, Session, shared parser/writer, public ABI, native fixture or Qt owner file changed. `output_data.cpp` is untouched: nonempty regions still append the first known name then refuse before geometry payload consumption. Type3, DoeOutput/Report/task, nonempty variables/constraints and public Type2 completion guards remain.

## Focused target and coverage

New target `m2m_doe_region_name_reader`; CTest `m2m.doe.region_name_reader`. Eight Testkit cases cover:

1. Clear/append order, exact case/direct child selection, duplicate/whitespace/missing/null/empty and Unicode values, with unrelated Study state preserved.
2. Unknown/nested/case-variant/raw children producing no names while still clearing and returning true.
3. Repeated lists replacing rather than merging, including a final empty list.
4. A later nonempty variable guard retaining parsed names and leaving index/number state unadvanced.
5. Empty-variable reindex then region append before a later Constraints guard, with outer ID pass unreached.
6. Earlier variable guard and malformed overall XML preventing region clear.
7. Actual ConversionSession/live Project/active owned Type2 Study through `parseSubProject`, retaining the same Study and names and continuing existing sample writes/current-PAR read in order.
8. That same actual owner entering engine Save, serializing three ordered names (including duplicate and empty) into immediate XML before its exact existing main-window+458 refresh-owner guard. Previous Project SUB DOM remains, sample writes are not repeated and no final project write is planned.

The active fixture explicitly selects SUB1 and supplies recording input/current-PAR files; it does not change public C ABI selection, execute native notifications or load geometry. Seed writes are separately counted and every recording fallback probe is under configured test scratch. The returned XML is parsed to inspect existing writer name effects, not compared to an invented original region-file oracle.

## Static preflight and release

All five source/test files have no trailing whitespace. Quoted includes resolve; the new test has one Testkit main, eight active cases, supported CHECK/REQUIRE assertions and no C assert/REQUIRE_EQ. Standard declarations plus explicit qt4path (`kSep`) and test_paths (`scratchRoot`) headers are present. No obsolete `RegionsOfInterest reader` guard assertion remains. Existing source/test diff whitespace checks passed.

No worker compiler/build, CTest, original executable, GUI control or mutable REA operation occurred. Root owns focused/grouped validation; the eight cases are authored expectations. Suggested regressions: Operations reader, variable ID finalizer, empty-variable reindex, MDT DOE study/activation and `m2m.mdt.doe_output_data` (including its first-region geometry guard case). Source is frozen and held through root build/test/archive.
