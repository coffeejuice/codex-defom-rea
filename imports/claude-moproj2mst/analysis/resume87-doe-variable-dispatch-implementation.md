# CP87: exact direct DoeVariable dispatch

The public DoeVariables reader now skips nonmatching direct children and keeps the existing refusal for an exact direct DoeVariable. A list with no recognized rows reaches the existing reindex and outer number finalizers. No variable header, record, sample acquisition, LinkTo/keyword state, geometry or notification is newly parsed or constructed.

## Source contract

Evidence proposal `resume87-doe-next.md`, SHA-256 `6f8d34a539ee8c982c9399555e085210f0b2814b750f51cdf222edd7ffc6d706`, is frozen. Original `0x14090ee20` converts each immediate child to an element and compares its tag at `0x14090eef6` with PE literal `0x1427bb508`, verified as `DoeVariable`. Only exact matches enter payload code. Others reach sibling advancement without recursive descent, then list exhaustion runs reindex. Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; retained completed reader decompile is `wave2-doe-lazy-input-empty-readers-decompile.json`.

The candidate scans direct parsed children in order. Exact matched rows throw the same prior DoeVariables payload error before attributes/construction; no earlier ignored sibling triggers finalization. Unknown tags, case variants and raw children are skipped. A nested DoeVariable under an unknown wrapper is not direct and is ignored. After the entire no-recognized-row list finishes, the same actual Study records are reindexed; IDs still finalize only after normal completion of all top-level readers.

Native DOM/QString/container ref and allocation/fault behavior remains outside the existing value seam. No diagnostic traversal ledger, failure profile, native pointer owner or public readiness token was added. B121/B122 helper projections are not called. Nonempty recognized variables, populated constraints, output geometry, Type3, codec/QFileInfo and public completion guards remain.

## Frozen source

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `77a08465c67e581207bc6efb555f0771c7f161703d2de6bcf02470b2a01293f2` |
| `source/apps/moproj2mst/tests/doe/test_variable_child_dispatch.cpp` | `8afbb7e2e432bc7ab654da16681b72337a798fac5a18ad6dc89ea31669a86831` |
| `source/apps/moproj2mst/tests/doe/test_empty_variable_reindex.cpp` | `70e760c244a73576fe369a61018816ca23c08a32a395bcaebc842580187b211d` |

The existing test changed only by removing its obsolete Unknown-only refusal block. Its direct DoeVariable and earlier/later payload checks remain. No header, CMake, shared Session/parser/writer, Qt owner, ABI or original fixture file changed.

## Focused tests and guards

Target `m2m_doe_variable_child_dispatch`; CTest `m2m.doe.variable_child_dispatch`. Nine cases cover no-row unknown/raw/case/nested selection with no owners created; existing records reindexed/numbered on the same aliases; recognized direct row after ignored siblings refusing before finalization; repeated no-row readers with only one outer number pass; prior region/reindex effects retained before later constraint refusal; existing reindex-null fault; actual Session-owned active Study read/sample order with no notifications; same-owner zero-variable Save writing empty DoeVariables then reaching the existing refresh guard; and pre-existing-variable Save still reaching variable-dimension reconciliation before XML.

Actual-owner fixtures explicitly select SUB1 and seed recording input/PAR files under configured test scratch. A pre-existing record fixture owns concrete plain Vars in the current Study owner vector; it does not imply payload parsing/notification happened. Ignored metadata is absent from regenerated XML, consistent with no imported variable state; no unknown-node preservation feature or native byte/durability parity is claimed. Existing source data and prior SUB DOM remain available at guarded Save boundaries.

Static preflight resolved every quoted include, verified the SessionNotificationState declaration/attempts member and explicit qt4path/test_paths headers, and checked one Testkit main, nine active cases, supported assertions, no C assert/REQUIRE_EQ and no trailing whitespace. Existing source/test diff whitespace check passed.

No worker build/compiler, CTest, original executable, GUI or mutable REA action occurred. Root owns validation and existing empty-variable/ID/region/Operations regressions. Source is frozen and held through root grouped build/test/archive.
