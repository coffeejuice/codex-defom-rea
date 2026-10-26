# CP124: owned Constraint records to immediate Type2 XML

The actual Session-owned empty-variable Type2 Save path now serializes its bounded zero-valued Constraint records after Operations. It still stops at the existing actual table-owner/refresh-delivery guard, preserving the XML file effects reached first. This is an output prefix advancement, not public active Save completion.

Source qualification is `analysis/resume123-doe-active-next.md`, SHA-256 `A3D21A9260E01134881ECC951D46EAC0C3F3FDD3FBFFA01911492B6DE67567FC`. Native 0x14090a550's tail appends Constraints and direct Constraint children in list order, obtains Id(+4), Value(+18), ObjectNo(+8), RegionNo(+c), then inserts those four attributes in that order. It does not consume sourceField10Bits, category or text. Shipped QtXml's double attribute conversion uses %.16g with a g6 fallback; the current concrete record type's admitted +0.0 has exact text0 in both. General double conversion and Qt/CRT allocator/global-state parity remain outside this cut.

## Implemented source and guards

Only existing `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` and new `source/apps/moproj2mst/tests/doe/test_constraint_xml_prefix.cpp` were edited/created in the source lane. No API or public/shared header changed. No CMake, Session, shared writer, convert, DBLIST or refresh code changed.

Private `appendConstraintElements(const Study&, xml::Node&)` runs in the existing document builder immediately after Operations. It requires nonnegative coherent count==concrete owned-record count and nonnull zero-valued records. Incoherent or missing records retain an unsupported guard after successful file open. Empty coherent ownership adds nothing, preserving the previous empty XML. For each retained record it appends one row and uses existing xml::setAttribute/toString; duplicates remain distinct ordered rows, and attribute insertion is not confused with QHash serialization order. sourceField10Bits remains unknown and untouched. Concrete records and Study are never replaced or mutated.

The immediate open/truncate still precedes document construction. DOM construction completes before stream serialization. Existing text translation, accepted-span observations, explicit close, post-close QFile.error, normal writer-return observation and caller's RefreshBoundaryReached are unchanged. Ordinary open-false and post-close-error returns continue to the same refresh guard. Interrupted model/serialization admission after open records truncation/device cleanup, but no normal writer return or refresh boundary. Requested stream bytes are never promoted to durable bytes or replayed through FileEffects.writes.

Nonempty variables still stop before reconciliation; public Geometry import, output geometry, actual refresh receiver/timer/UI delivery and DBLIST remain guarded. Legacy writeInputTable's separate positive-constraint planner guard and DEF_DOE.INI consumer guard are unchanged. Original QList/QString allocation/refcount/exception parity is not claimed.

## Focused tests and static preflight

New target `m2m_doe_constraint_xml_prefix`, CTest `m2m.doe.constraint_xml_prefix`, contains five cases:

- Real ConversionSession/Project/Type2 SUB lazy read creates actual retained records, then engine::saveProject writes ordered XML with duplicate IDs, exact four fields, unchanged Study/record identities and unknown+10. Caller-owned journal observes actual accepted recording spans, normal success and refresh boundary before the existing main-window+458 refusal; old SUB DOM, sample tables/flags and later sample file effects remain unchanged.
- A path/ordinal-bound explicit recording open failure preserves the prior input file and returns false normally before the same refresh refusal.
- A bound post-close error7 preserves accepted XML and normal false/sticky-error observation before that refresh refusal.
- Count mismatch, null owned row and negative count refuse after successful truncation, with no body bytes/post-close query/normal return/refresh observation.
- Empty owned constraints preserve the prior complete empty-variable XML bytes including Windows Text-mode CRLF.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_constraint_xml_prefix.cpp` passed (quoted includes and Testkit main/macros). Direct whitespace/final-newline checks passed on both files, including the untracked new test. `git diff --check -- source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` passed. Static API inspection caught and removed a const qualifier from the fixture method because ConversionSession::project() has no const overload. No compiler, executable, CTest, full suite, GUI or mutable analysis session was run by this worker. Root owns runtime validation.

The root-owned existing `test_constraint_record.cpp:233-262` case expects the superseded valid-record serialization refusal. Root was sent the exact correction request before any shared test edit; this worker did not modify it. Existing test_type2_xml_prefix fixtures with positive compatibility count but no concrete records continue to refuse after open, preserving their partial-effect intent.

## Source freeze

Source released to root and independent reviewer before this receipt. All source held through root's grouped build/test/archive.

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/type2_xml_prefix.cpp` | `948D8FD0345E2534F797444F109DF71CF079723A72C1A671F74CFC4C42EC7ADB` |
| `source/apps/moproj2mst/tests/doe/test_constraint_xml_prefix.cpp` | `70C6BA58C1E04A927F483A957B471727BE6A460E3008745CD6EAE28802738EE5` |

The next active Save dependency is the actual Session's DoeTableOwnerState refresh delivery: main-window+0x458 occurrence/lifecycle, real SfMoDoeTable QTimer stop and UI receiver continuation. This increment does not bypass or seed that state. A separate source-qualified future output consumer must independently establish whether it needs unknown Constraint+0x10; this serializer does not.
