# CP142 DOE Item owner migration: static review

## Blocker: standalone P09 emitter target omits the new out-of-line owner implementation

`tests/p09/test_emit_b.cpp` remains a `ctest-standalone` target. Its directive lists `src/mst/emit_b_doe.cpp` and P09 doubles, and CMake's standalone branch compiles only those listed sources plus a few hard-coded storage/witness sources; it links `testkit`, not `m2m_core`. The migrated `emit_b_doe.cpp` now calls `doe::snapshotItems` and `doe::itemSnapshotText`, both defined only in the new `src/doe/item_owner.cpp`. The P09 test also constructs/destroys `doe::Var`, so its `vector<Item>` destruction requires `Item::~Item()` from that implementation. Since `item_owner.cpp` is absent from the standalone directive, `m2m_p09_emit_b` is expected to fail at link time with unresolved owner/snapshot symbols.

Minimal correction: add `src/doe/item_owner.cpp` to the existing `// ctest-standalone:` source list in `tests/p09/test_emit_b.cpp`. It is a source-relative path, gets the target's existing include directories/definitions, and avoids changing the standalone/linkage boundary or introducing `m2m_core`. Root should confirm this in grouped focused linkage before full regression. This review did not edit the source.

## Ownership and lifetime assessment

- `Item` keeps a private `variant<LegacyItemText, unique_ptr<KeywordConstructorRecord>>`; clients cannot independently set a kind and payload. Copy is deleted and the only native record owner is concrete `unique_ptr` ownership in the `Item`.
- The incomplete record appears only under `unique_ptr`; destructor, move members, factory and accessors are defined out of line in `item_owner.cpp`, which includes the complete private record. This is the standard safe opaque-`unique_ptr` arrangement. Root compilation still must establish all translation units instantiate/link its destructor correctly.
- `ItemSnapshot` copies the two legacy Strings, but stores only a const borrowed record pointer for typed entries. Heap allocation keeps that record address stable through vector envelope relocation. Emitter traversal is synchronous and does not invoke a callback that mutates/deletes the owning Study. The tests explicitly avoid dereferencing a snapshot after owner destruction.
- `ownKeyword(nullptr)` remains distinguishable from default/legacy-null text and throws at typed projection before dereference. The throw is not claimed as recovered native null behavior.
- A scoped search found no whole-`Var`/`HeaderVariableRecord` value-copy site; the one initializer-list Item fixture was replaced with `emplace_back`. Existing count-only construction remains available through default `Item`.

## Emitter parity and guard review

`emitDoeVariables` takes one snapshot at the old vector-copy location and preserves variable/link/name handling, item order, line counting, DOEVAR numbering, separators and trailing output. Legacy text comes from the copied snapshot, preserving entry-time values and null/empty distinction. Typed entries project upper/lower Strings at consumption, as specified in the migration plan. The existing P09 golden fixture text is unchanged; only its construction syntax changed. No runtime byte-parity execution was performed here.

The reviewed `variable_dimension_prefix.cpp` still writes the recovered dimension first and stops at `KeywordItemSnapshotBoundary` on any nonempty Item vector. Empty-variable prune still tests `items.empty() && files.empty()`, so typed and explicit-null entries continue to count as present. B121/B122 empty-item guards are unchanged. Keyword child parsing remains guarded at the existing entry, and nonempty Save remains unsupported before reconciliation/prune/XML. No public parser, Save or notification admission was added in these files.

## Reviewed source identities

| File | SHA-256 |
|---|---|
| `include/m2m/mst/doe_model.hpp` | `388AAC1F072164C85085D93CD776FD9E29B2948EC6806951CDD4CF39B608DBF0` |
| `src/mst/emit_b_doe.cpp` | `BFC4F9AFD82A6216B85BDAA7D16673335FCFEAD3EB74962F2306DFE8EC30580F` |
| `src/doe/keyword_constructor_record.hpp` | `EB93B6D0E342364D794F2979B4F7983BDD66241FA569FDA5524C25328280CB00` |
| `src/doe/item_owner.cpp` | `4B60CC1BEF944CF9F81A2BC37D346CCBD579C90A54D6BF8E0175AFB9BEFD03BB` |
| `tests/doe/test_item_owner.cpp` | `B9A272E33F0EE5665DB0727A96BDAF61831D8422EDF49AB8C332394E9BE7EE17` |
| `tests/p09/test_emit_b.cpp` | `04138204E248462494A93978346BC149B8EE0FE3F758317BDDD79E17186F4ABD` |
| `CMakeLists.txt` | `2D4E0D5EF2ADDCE9011DF45DB49304E127D20FFD929EE7177AC96916E1D89CB5` |

Read-only static source/target review only. No source/build/test/GUI/REA mutation or execution occurred. The standalone link dependency above is a concrete blocker; opaque-type linkage and golden output remain to be established by the root-owned build/tests.
