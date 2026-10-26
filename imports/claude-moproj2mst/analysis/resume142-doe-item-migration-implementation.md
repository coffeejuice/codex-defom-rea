# CP142 canonical Item worker source release

The assigned Item ownership and snapshot implementation is frozen. Legacy entries retain copied text semantics; a typed entry uniquely owns a stable concrete keyword constructor record, and snapshots borrow its pointer. The public Keyword reader, active Save, conversion-output and notification guards were not changed.

Only the following worker source files and this new receipt were written. Root owns the shared declarations in doe_model.hpp, emitter switch and P09 fixture correction. No worker build/CTest, GUI, original process or mutable REA action occurred.

| Worker file under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/doe/keyword_constructor_record.hpp | eb93b6d0e342364d794f2979b4f7983bdd66241fa569fda5524c25328280cb00 |
| src/doe/item_owner.cpp | 4b60cc1bef944cf9f81a2bc37d346ccbd579c90a54d6bf8e0175afb9befd03bb |
| tests/doe/test_item_owner.cpp | b9a272e33f0ee5665db0727a96bdaf61831d8422edf49ab8c332394e9be7ee17 |

The source freeze was sent to root before writing this receipt, so grouped validation need not wait for prose. No source edits followed that freeze.

## Implemented contract

The exact API from `analysis/resume142-doe-item-migration-plan.md` is implemented against root's private `storage_` variant. Out-of-line default/legacy constructors, destructor and noexcept move operations avoid exposing the opaque record's definition through the shared model header. Item remains noncopyable. `ownKeyword` accepts one unique concrete owner, including an explicitly null native entry; kind is derived from the variant alternative rather than an independently writable tag.

`KeywordConstructorRecord` is a move-only normal value prefix of9206e0. Its constructor accepts keyword ID, parameter name, unit type, nominal numeric value, operation IDs and entity IDs. Default text/flags/scalars, distinct +80 parameter and +84 Save selector, and +A0/+A1 data flags are explicit. Its three data fields have type `std::nullptr_t`, so this increment cannot adopt nonnull DYNARY data. This is neither a literal native0xa8 allocation nor a Qt ref/shared-null owner. Unknown/native fields not needed by this constructor prefix are not given invented meanings.

`snapshotItems` copies each LegacyItemText in order and borrows each typed record as `const KeywordConstructorRecord*`. It never owns a record or stores an Item-envelope pointer. Therefore moving/growing the Item vector preserves the record identity, while destroying the concrete owner invalidates the borrowed pointer. No shared_ptr or owner back edge was introduced.

`itemSnapshotText` returns the copied legacy text or reads typed upper/lower text at consumption. Explicit native-null refuses with M2M_E_UNSUPPORTED and a precise native-null message. The refusal is a reconstruction bound; the original emitter's null behavior remains unqualified. Legacy default/null/empty Strings remain supported and distinct.

The typed record is now owned by the same canonical Item vector in the actual HeaderVariableRecord base, without a second Header keyword list. Existing Study unique ownership and its borrowed Var aliases are unchanged. Nothing is adopted from the separate retained B121 scalar-prefix owner. Later construction/append, parser delivery and DYNARY conversion still require their own source-qualified binding.

## Focused tests and static checks

New target `m2m_doe_item_owner`, CTest `m2m.doe.item_owner`, contains six cases:

1. Default and two-String legacy entries, String null/empty distinction, and retained snapshot content after the source legacy entry changes.
2. Exact constructor inputs/defaults, separate selectors, ordered entities/operations and constructor-null data state under one unique owner.
3. Borrowed record identity through Item-vector growth and move assignment, with the moved-from entry explicitly native-null. No snapshot dereference occurs after owner death.
4. Copied legacy text versus live typed text reads at consumption; there is no mirrored typed text cache.
5. Explicit native-null versus legacy default: only native-null refuses and retains its distinct snapshot alternative.
6. Concrete Study/Header ownership and existing private prune behavior: a typed Item counts as a nonempty record; clearing that owner followed by prune removes the empty Header and its borrowed alias. The retained pointer snapshot is not dereferenced after clearing.

Compile-time assertions cover Item noncopyability/noexcept moves and record noncopyability. These assertions have not yet been compiled by the worker; root's grouped build is authoritative.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_item_owner.cpp` returned `ok: true`, no issues. This verifies entrypoint/Testkit/direct include paths, not C++ type correctness or execution. Direct UTF-8/newline/trailing-whitespace checks passed for all three new files. Root must run the Release compile and focused Item/P09 tests, plus its required grouped checks, to validate opaque-type linkage and the shared move-only migration.

## Remaining boundary

This release migrates ownership representation only. It does not execute816b40 on a public reader, change the CP138 nonempty Item stop, add a native allocation-failure branch, implement borrowed DYNARY output copying, or call SetOprHasDoe/DoeTableRefresh. Global manager selection, constructor QString ownership, actual parser/entity-pair provenance, conversion failure lifetime and active Save return remain guarded. The next independent source slice is the explicitly supplied fresh-empty-list constructor/append helper proposed in CP141, after root freezes this canonical API.
