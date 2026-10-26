# CP90 actual Study-owned bounded Constraint records

The active InputTable reader can now retain a source-qualified positive-integer/zero-value Constraint row on its actual Study, continue to the existing ID finalizer and lazy sample effects, and preserve earlier records when a later row refuses. Public Type2 Save remains guarded after its existing XML open/truncate effect. No GUI, mutable analysis database, build or CTest was used in this lane.

Source qualification is `analysis/resume89-doe-constraint-owner.md`, corrected SHA-256 `2B5E463DA9E6B4332BC6E6657CEB40CD7F7D11948EA651FD2FB92520332D5B7E`. This supersedes that report's initial C513 hash. Its retained PE identities, selected normal-body hashes, native manager/record ownership, unwind handlers and numeric admission limitations remain controlling evidence.

## Implemented owner and decision

`Study::constraintRecords` owns concrete `ConstraintRecord` instances through unique_ptr; there is no Study/Session back edge or unrelated-base deletion. Existing `Study::constraints` is reset at the source clear encounter and updated only after a successful current-record append. Repeated lists remove earlier model records in reverse order, then process their children in DOM order. This is host model cleanup, not a claim that native implicitly shared QList data reached its final reference.

Exact direct `Constraint` rows with all four attributes are admitted only for canonical positive int32 `Id`, `ObjectNo`, `RegionNo`, and exact `Value="0"`. This is an explicit reconstruction bound, not original validation. The source reader `0x140912d10` reads Id/Value/ObjectNo/RegionNo in order and calls manager virtual+8; `0x14091e110` appends the record. Id1010/1012/1080 maps to category0, Id1081 to1, all other admitted IDs to2. Final known fields and non-null empty text are represented. Native allocation size0x28 is named metadata; host sizeof is not asserted.

The source-unwritten/copied eight-byte field+0x10 is `const optional<uint64_t> sourceField10Bits=nullopt`. It is never defaulted to0, read as double or inferred from parsed Value. Native constructor `0x14091fab0`, copy constructor `0x140919aa0`, append/copy, QString destructor and reverse final-owner cleanup are distinguished in the qualification report. General locale conversion, arbitrary Value, native COW/refcounts, raw-null allocation, and original fault timing remain outside this model.

Unadmitted exact rows retain the existing matched-row error after earlier clear/appends. Unknown/case-different/nested children retain their previous skip behavior. A host allocation or vector growth failure does not increment the count or publish the current record; RAII cleans the uncommitted host record and prior committed entries remain. This is not deterministic emulation of native allocation failures.

## Focused tests and preflight

New target `m2m_doe_constraint_record`, CTest `m2m.doe.constraint_record`, has nine Testkit cases:

- Known fields, explicit unknown+0x10 and non-null empty text; native-size metadata only.
- All category branches, ordered duplicates and distinct simultaneous record identity.
- Missing/noncanonical/overflow/nonzero attributes retaining the matched-row guard, plus int32 maximum admission.
- Earlier committed row and header surviving a later refusal.
- Repeated lists replacing earlier records and an empty list clearing the owner/count.
- Earlier/later matched-variable guard timing and prior reindex stores.
- Normal completion reaching the existing outer ID pass.
- Actual ConversionSession/Project/Type2 SUB/Study/I/O retaining the same owner and ordered LHS/FF/USR writes followed by current PAR read, without notifications.
- Actual same-owner Save opening/truncating XML, then refusing constraint serialization before body bytes, normal writer return or refresh boundary; old project DOM and records remain, and no later output writes occur.

The last test intentionally does not assert an absence of XML effects. Current immediate Type2 writer opens/truncates before its positive-constraint serialization guard; the legacy file planner's earlier refusal is a different path. The journal's canonical recorded file is asserted empty after truncation, without a durable native-file claim.

Static preflight resolved every new quoted include against the source directory or target include roots. In particular `qt::kSep` is supplied by `m2m/qt4path.hpp`. The test has a Testkit main and uses only supported CHECK/CHECK_EQ/REQUIRE macros, no C assert or REQUIRE_EQ. No trailing whitespace was found; git diff --check passed for the edited tracked sources. Existing source glob admits the new cpp. No shared CMake, Session, writer, convert or C ABI file changed. Root owns compilation and runtime validation; these tests are not reported as executed.

## Source freeze

All paths below are relative to `source/apps/moproj2mst`. Source held after this release through root's grouped build, tests and archive.

| File | SHA-256 |
| --- | --- |
| `include/m2m/doe_constraint_record.hpp` | `736ADB61900D9F5AADBE1B3236A82AB9BB86789AEDB17ED2E9AFFB6044B117F5` |
| `src/doe/constraint_record.cpp` | `9F95D68FBA6DC386C48653BBF21188FAAADB7C102449241ACBDB89EEA0B4FBF1` |
| `include/m2m/doe_study.hpp` | `B51FA3685D9D23CB9C11A0B54B95B4F0B95832A5174235E74104CAAB79E3FC09` |
| `src/doe/doe_study.cpp` | `602B5CF148DE522ADC5C0B242B5EE43CDE1A5782A28ECD408B2C60E74D5780FB` |
| `tests/doe/test_constraint_record.cpp` | `06247D151A773D10F53A8106D0507351B835CFA7F8871C82D8DDC084210076A9` |

The next independent constraint dependency is general QString numeric/default-locale conversion or source-qualified serialization/consumption of record fields, particularly unknown+0x10. Matched variable parsing, output geometry, native Qt allocation parity and actual Save refresh receiver remain guarded. This slice advances actual public read ownership; it does not claim public Save completion or original constraint oracle parity.
