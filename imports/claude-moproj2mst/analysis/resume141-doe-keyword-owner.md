# CP141: keyword ownership and a concrete conversion-output producer

The keyword record's normal destruction chain and one actual GUI conversion-output producer are now bound. This closes two specific CP139 evidence gaps. The smallest defensible implementation proposal is a private constructor/append prefix on an actual HeaderVariableRecord whose keyword list is initially empty, with explicitly supplied decoded inputs and all three data slots null. It requires agreement on the single canonical Item owner representation; it does not release the public Keyword reader or active Save.

This checkpoint changes analysis only. No source/test/build, original process, GUI or mutable REA action occurred. The decoded artifact is the retained original GUI PE, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses from the separately ported DEF_PRE registry are not treated as GUI addresses.

## Record ownership is concrete

New bounded capture `analysis/resume141-doe-keyword-owner-bounded.asm` contains function-entry-aligned original bytes and DUMPBIN14.44.35228.0 disassembly. Its range headers give individual byte hashes. The normal destruction path is:

1. Variable destructor8169d0 begins traversal of its +30 list at8169f0 and obtains the end at816a03. It dereferences each pointer, skips null, and calls818750 with delete flag1 at816a4a. It then updates/erases the traversal position at816a6c and repeats.
2. Deleting wrapper818750 calls keyword destructor9208e0, then frees the record when flag bit0 is set. This is the concrete keyword record, not a polymorphic Var deletion.
3. Keyword destructor9208e0 visits nonnull data pointers in this exact order: nominal+98 at9208fa, lower+88 at920951, upper+90 at9209a8. Each is passed with delete flag1 to1009d0.
4. Wrapper1009d0 calls DYNARY destructor89b180 and conditionally frees its allocation. DYNARY destructor89b180 calls89b1f0 for its arrays, then recursively deletes a nonnull +68 DYNARY through1009d0. The captured89b1f0 includes its array-free branches.
5. After the three data deletes, keyword destruction releases containers at+68,+60,+58,+50, then QStrings at+20,+18,+10,+8,+0. Variable destruction later releases its containers and own strings.

The Variable list therefore owns these keyword allocations. A QList snapshot of pointers is a borrowed pointer collection; it does not independently own or keep the keyword records alive after Variable destruction. A future host snapshot must not accidentally turn its borrowed pointers into shared ownership that prolongs object life. Duplicate aliases to one record would also be unsafe under this deletion loop; an admitted prefix should create one distinct record and preserve that invariant.

This evidence closes normal lifetime for a constructor-prefix record with three null data slots. It does not prove allocation-exception unwinding, replacement of already-populated data slots, arbitrary cyclic DYNARY chains, or native allocator parity. CP139's setter bodies still store new pointers without freeing prior slot contents.

## Producer and append order

The actual manager wrapper91a5b0 uses its name lookup virtuals to select a Variable and calls816b40 at91a6fb. Wrapper selection is separate from an explicit-receiver helper; no caller-selected name or index should silently substitute for this actual manager occurrence in public code.

Native816b40 first reads keyword-list count at816b72, then stores its **supplied dimension** at Variable+24 at816b8f. It is not intrinsically a constant3 setter. Only afterward does it search existing records, except keyword ID0x131 bypasses that search. The search compares keyword ID+2c, parameter-name QString at+0, and entities-list+50 via d0ff0. The latter compares size and ordered int values, with a shared-container identity shortcut. Nominal value, unit type and operation list are not additional dedupe keys in this body.

For a miss,816c60 allocates0xa8;816ce1 calls9206e0 with keyword ID, parameter name, unit type, nominal numeric value, operations and entities. The resulting pointer is appended to Variable+30 at816d24. The following loop reads the supplied operation list and appends each ID absent from Variable+38, preserving encounter order and existing IDs. No notification call occurs inside816b40. Its returned normal path is therefore separable from the later reader's SetOprHasDoe delivery.

The existing retained B121 prefix already projects part of this constructor, but on a different concrete ScalarVariablePrefixRecord. It remains unsuitable for adoption into the actual HeaderVariableRecord list. The fresh empty-list bound avoids inventing legacy Item metadata or dedupe semantics. Native code also has a null-allocation branch that appends a null pointer before the operation-union loop; no host implementation should claim to reproduce that branch merely because std::bad_alloc exists.

## A real GUI output producer: FRCROT

The native GUI lookup922160 scans records at1427c06e0 with0x30 stride. FRCROT is ID276, table index297, record1427c3e90. Its fields are:

| Field | Original GUI value |
| --- | --- |
| name | ASCII FRCROT at1427c06b0 |
| group / flag | 3 /7 |
| handler1 | 140921da0 |
| handler2 | 140b51050 |
| handler3 | **140b51140** |

Raw48-byte record: `1401000000000000b0067c42010000000300000007000000a01d9240010000005010b540010000004011b54001000000`; SHA-256 `5f8a3babeb0fb36f7aa601ea14a8abd112175ade07309c4b343f4c6bacf32012`.

GUI handlerb51140 reads entity-vector element0 without a size guard. When it is >=1, it resolves the indexed pair from scratch model+13c8 with8e6b70; otherwise it reads elements1/2 and uses pair lookup8e6bd0. For a nonnull pair it stores **borrowed pair+240 DYNARY** into the supplied output pointer atb511ef. A missing pair leaves that pointer unchanged. It destroys the passed vector and returns0 in either case. Thus status0 alone is not a live-output witness.

This agrees with the separately ported FRCROT handler's semantics (`source/port/defkey/src/kw_g3/registry_g3.cpp:104`), but its documented DEF_PRE address3cac80 is not the GUI function address. A bounded GUI decode there is not a function entry. The registry row, GUI922160 lookup and b51140 body are the admissible binding, rather than numerical address similarity across executables.

## Copy ownership across conversion

In native916530, the three registry calls at916c96,916d76 and916e56 obtain borrowed results from the nominal, upper and lower scratch models. A nonnull result is separately allocated at0x78 and copied with898f40 at916cd3,916db3 or916e93 before publication through the three caller output pointers. The scratch models are destroyed later at916f16,916f84 and916fec. Therefore the caller does not receive the borrowed pair field directly.

Copy constructor898f40 initializes its fields, calls899010(type0), then89ad60. The captured89ad60 copies scalar descriptors, calls89b840 to allocate data storage, copies each available component array, then the additional+48 array. This is an owned deep-copy path, not a pointer alias or a QString assignment. Failure/allocation semantics of89b840 and the899010 type dispatch are not closed by their call sites alone.

CP139's caller result gate remains unchanged:916530 status is ignored; both upper and lower output pointers must be nonnull before optional nominal, then upper, then lower record commits. Scratch temp.KWD writes and model/parse effects precede that gate. No code here proves that an arbitrary FRCROT text actually parsed a pair, that its entity vector selected that pair, or that a normal conversion returned. Public conversion must remain guarded until those input/parser effects and deep-copy failure lifetimes are qualified.

## Smallest proposed source slice

After a new source assignment, the meaningful bounded slice is a private `appendFreshKeywordConstructorPrefix` receiver on an actual Study-owned HeaderVariableRecord, admitting only an initially empty canonical keyword list. Inputs are already-decoded keyword ID, parameter name, unit type, supplied dimension, nominal double, entity IDs and operation IDs. Proposed order: verify actual concrete Study ownership and empty list before mutation; store supplied dimension; construct the typed record with normal constructor defaults and null data slots; append its single owned pointer; perform ordered operation-ID union. Stop with a constructor-prefix diagnostic before public reader notification/text/selector continuation. No helper-minted success receipt closes earlier parser/codec or manager selection.

This requires a **single canonical owner representation** for Item. The current `Item{before,after}` values cannot own the native record, represent a native null pointer, or carry its metadata. Root must own `include/m2m/mst/doe_model.hpp` and the copied-list consumer in `src/mst/emit_b_doe.cpp` while choosing that representation. Keep legacy value-only Items explicitly unknown and refused by the new helper. Do not add a competing HeaderVariableRecord keyword list. A borrowed pointer snapshot must preserve record identity without owning it; actual owner destruction deletes each concrete record once. Native COW/ref/allocation parity is outside this normal value prefix.

Worker scope can then be exclusive NEW `src/doe/keyword_constructor_prefix.hpp/.cpp` and NEW `tests/doe/test_keyword_constructor_prefix.cpp`, plus `src/doe/variable_record.cpp` only if root wants the implementation placed beside its receiver. Shared canonical type/consumer edits require root coordination first. No public InputTable/Save call site belongs in this slice.

Focused tests should use a real Session/Study-owned HeaderVariableRecord, then explicit private decoded inputs: dimension precedes append, constructor null/default metadata is retained, entity/operation list order is exact, operation union is stable, one distinct owner is created, and a nonempty/legacy/foreign list refuses before mutation. Record/snapshot lifetime tests must distinguish borrowed views from owners. Public Keyword entry and nonempty Save must retain their existing refusal and earlier partial effects. Do not introduce synthetic allocator-failure claims or real temp.KWD I/O into these tests.

This is a source-ready **normal constructor/append value prefix once the canonical owner API is agreed**, not a ready conversion implementation. A subsequent FRCROT conversion cut needs exact group3 reader/handler2 input provenance that constructs the selected pair in all three scratch models, matching entity-vector contents, and899010/89b840 failure/cleanup contracts. Those are now concrete static targets; neither another codec scan nor another no-effect snapshot helper resolves them.

## Existing guards

Keep CP138's nonempty Item boundary, the public Variable Keyword entry guard, global/current-MO/lazy operation-manager ownership, nonempty Type2 Save reconciliation, refresh key/receiver/return guards and all Geometry/import guards. The new evidence does not move prune, XML, notification or PAR writes earlier. No source implementation was made in CP141.
