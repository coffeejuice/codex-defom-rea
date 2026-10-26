# CP86: public region-name list reader

The next source-ready active DOE branch is `RegionsOfInterest` **name-list loading**. Original `0x140912b00` clears the manager's list, then appends the name of each direct `Region` element in source order, passing a null payload pointer. It returns true. The current Study already has the canonical name projection and a writer for these names, but its public reader refuses every populated list. This increment can use that same actual Study without QWidget, QFileInfo, geometry-file loading or a new native QString/ref owner.

This read-only lane owns only this report. No source edit, build/test, GUI/native app execution or analysis database mutation occurred while CP85 source is frozen. Raw disassembly was viewed through read-only DUMPBIN stdout; no additional capture files were written.

## Current guard reconciliation

- CP85 empty DoeVariables can now reindex existing owned records at the child encounter; CP84 normal outer completion then repairs nonpositive numbers. Nonempty variable elements still refuse before public payload construction/notification.
- CP83 Operations retains normal true/no-mapping behavior and an explicit native QDom traversal/fault limitation.
- Public region loading currently performs only the required clear, then refuses any element child. Its existing `Study::regions` vector stores names used by `writeInputTable` and DoeOutput's first-name prefix.
- Populated constraints still lack actual record storage beyond a count projection and remain guarded.
- DoeOutput/Report/task construction, Type3 optimisation Study ownership and ShapeOptim geometry/file effects still have their existing owner/UI/path dependencies. The stale ShapeOptim Study-name folder guard is not a better independent route because the original folder path reaches QFileInfo before that read.
- Region **geometry/payload consumption** is separate from name loading. `finishCurrentOutputClassData` currently appends only the first known region name before its geometry payload guard; that guard must remain. InputTable Save can serialize names using the existing writer, then still meets the actual refresh-owner guard. No successful public Save or geometry ownership follows.

## Original reader and manager owner

Original GUI PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained `wave2-doe-lazy-input-empty-readers-decompile.json` contains the completed reader. Bounded original disassembly confirms its complete body `0x140912b00..0x140912cfb`:

1. Local return status is initialized true. At `0x140912b32`, manager vt+0xb8 clears existing records **before** child traversal.
2. It walks raw firstChild/nextSibling order, converts each direct child to an element and skips non-elements.
3. It compares the exact tag against the literal at `0x1427bb7b4`, verified directly as `Region`. Unknown tags, case variants and nested Region descendants under another child are ignored.
4. For each matching direct child, it reads attribute literal `0x1427bb7bc`, verified as `name`, with a default-constructed null QString. It does not reject missing, empty, duplicate or whitespace names, nor validate a file/operation/geometry reference.
5. At `0x140912c63`, it calls manager vt+0xa8 with `(manager,0,name)`. The appended result/index is ignored. It continues to the next child and returns unchanged true status.

`evidence/gui_mo/s1/vtables.tsv` binds both relevant manager classes: vt+0xa8 -> `0x14091f1a0`, vt+0xb8 -> `0x14091f290`, count+0xc0 -> `0x1400d7ee0`, name+0xd8 -> `0x14091f380`.

The append body builds a two-field temporary: native record+0 is explicitly assigned the caller's pointer argument at `0x14091f1cc` (zero on this reader route), and record+8 receives the QString name. It appends to manager+0x20 with `0x14091fce0`, computes count-1, destroys the temporary and returns that index. The temporary constructor `0x14091faf0` only constructs QString+8; it does not initialize the pointer field, which is correctly assigned afterward by the caller. Its destructor `0x1409196a0` only destroys QString+8 and does not delete the pointer payload.

Append's selected native node-construction branch `0x140920240` allocates16 bytes and calls record copy constructor `0x140919b40`. That constructor copies the pointer field and copy-constructs QString+8; no QWidget/QObject, file engine or geometry loader is constructed. The native container helper also has detach/storage branches. These ref/allocation/failure internals remain outside the existing candidate value/container seam; do not imply `std::vector<String>` is a native16-byte record allocation or that host allocations reproduce Qt fault timing.

This closes the **normal name state** and null payload argument at read time. It does not establish a canonical actual payload-pointer owner for later geometry consumption. There is no need to invent/store such a capability merely to add the names: current consumers use the name projection or stop before payload access. A future payload-capable branch needs its own typed owner/provenance and must not infer readiness from names.

## Bounded candidate increment and order

Keep `s.regions.clear()` at the same encounter. Replace only the blanket populated-child refusal with a direct, ordered scan of the parsed children: for each immediate element tagged `Region`, append `attribute("name", String())` to the actual `s.regions`. Preserve missing-name nullness versus an explicit empty attribute in the value model, duplicates, order and whitespace. Do not recurse, trim, deduplicate, validate files or synthesize payload objects.

Mutate the actual vector incrementally after clear. Do not stage all names in a separate list and swap only on success, because earlier clear/appends must survive any later unsupported child/sibling effect. Use the existing parsed-DOM/String abstraction and state its limits; no QDom ref/QString shared-null/native container allocator parity is newly claimed. An extra diagnostic fault profile is not evidence of native allocator behavior.

A later nonempty variable or constraint guard must keep the completed region prefix. Earlier guards prevent this region child from being reached. Repeated RegionsOfInterest siblings each clear then append, so the last reached list replaces prior names rather than merging them. Normal outer ID finalization remains after all child readers; a later thrown payload guard still prevents that pass.

This is reachable through actual `parseSubProject` -> owned Type2 Study -> `readLazyStudy` using an InputTable with empty variables and populated names. Existing sample write/current-PAR continuation remains in order. Same-owner Save with no variables/constraints can serialize these names into immediate XML through the existing writer and then retain its exact refresh-owner guard. Source-backed synthetic fixtures must not be labeled original region-file byte-parity captures.

## Exclusive source/test proposal

After CP85 archive and root assignment, propose one lane owning:

- Existing `source/apps/moproj2mst/src/doe/doe_study.cpp`: only the region reader guard replacement/ordered name append.
- New `source/apps/moproj2mst/tests/doe/test_region_name_reader.cpp`: focused normal state, ignored child, repeated list, partial order and actual owner read/Save tests.
- Existing `tests/doe/test_operations_reader.cpp`, `tests/doe/test_variable_id_finalize.cpp` and `tests/doe/test_empty_variable_reindex.cpp`: revise only their obsolete populated-region refusal expectations. To retain earlier-order/ID tests, put the still-guarded populated Constraints child after the now-completed region list and assert that appended names survive; use that earlier Constraints guard where a later empty DoeVariables must remain unreached.

No header, CMake, Session/shared parser/writer, Qt owner, public ABI or original fixture change is required. The Study region vector is the existing canonical name projection; do not create a disconnected duplicate region registry.

Expected new target `m2m_doe_region_name_reader`, CTest `m2m.doe.region_name_reader`. Focused cases should cover clear-before-append and exact direct-child/case selection; duplicate, whitespace, Unicode value, missing/null versus explicit empty names; no recursion/unknown/raw-child effects; repeated lists replacing prior names; later guard retaining the region prefix and preventing ID normalization; earlier guard preventing clear; and actual Session-owned lazy read/sample sequence followed by same-owner Save XML containing the ordered names before its refresh guard. Keep nonempty variable/constraint and output geometry/Type3 guards. Use ASCII names for the immediate locale-byte Save fixture unless encoding evidence is separately supplied.

## Compact immutable byte receipts

Selected normal bodies were inspected with local DUMPBIN `/disasm:bytes /range:<start>,<end-1>` on the hash-verified original PE. The ranges through their normal RET boundaries have these raw hashes; no `.pdata` boundary alone is treated as whole logical-function proof.

| Entry / bytes | SHA-256 |
| --- | --- |
| `0x140912b00`,508 | `15050b025773fbf3f76f643a9d2a65d6f37a404f129b177f2a4faab5be776474` |
| `0x14091f1a0`,124 | `3cc3cb9ae357cf741f80252a384ccf9922205ac42ccc3deb5fefd9c18d035a83` |
| `0x14091f290`,28 | `d4ba1613a25d3da9365edec06748765945e2de94b56f2fa54bede75b8e9dba56` |
| `0x14091faf0`,34 | `3541ff3adc04407600476d682910207f186eb251d3f25bfde76cde5dc1b051c4` |
| `0x1409196a0`,29 | `4a4163c8478870dcf96deea6d39e3b349376134abb7b4c9c38720208d42e3e29` |
| `0x14091fce0`,209 | `60abd58cc9fa71d7a32692c1eac2628f5ccb1c4676287e9d7658987f51b88124` |
| `0x140920240`,227 | `96898df4650989b3323dd1f025d58ded210d18cd778ba28da94b09a327f59f93` |
| `0x140919b40`,64 | `5f4f2c8c318932c48db8b7b7846c9361704923ecba7e27ff1d939c63c7422fc0` |

The retained reader decompile remains the argument/order authority alongside these bytes. No compiler or test execution was performed in this read-only lane.
