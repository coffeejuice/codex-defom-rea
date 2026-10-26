# CP144 DOE Keyword reader source review

## Result

No blocking defect found in the released bounded integration. The concrete receiver and current reader target are kept distinct: `appendBoundedHeaderVariable` continues to apply the row's header, earlier Description and deferred LinkTo effects to its last-record `variable`; `readBoundedKeywordConstructorPrefix` scans the manager-order aliases, finds the first matching concrete `HeaderVariableRecord` by the original row name, and passes that object to `appendFreshKeywordConstructorPrefix`.

The A,B,A test exercises the distinguishing case. It expects A's canonical Item and operation list to receive the TLOC constructor record and reader-local dimension3, while B receives the reached row ID/Description changes and retains the queued link without resolution. It also asserts `selectedVariableIndex == 1`, preserving B as the last fresh selection. This matches the retained `91a5b0` lookup path reviewed in CP144: pointer lookup by manager name/index does not write model `+0x74`. No selection-state regression was introduced.

## Boundary and effect checks

- `retainedProfile` is an exact value admission: exactly the five expected attributes, exact TLOC/unit/type/parameter/nominal spellings, no parameterName attribute, one NominalData wrapper whose first child is the exact string node/text, and one nested KwOperation id1. Bounds, enum/entity/function shapes and alternative child order are rejected before constructor mutation. Hardcoded registry ID8 is separately bound by original table evidence and is described as a value projection, not a process-bound codec/registry execution.
- The decoder does not call `toLocal8Bit`, the PE registry, or the Session notification helper. It calls `appendFreshKeywordConstructorPrefix` for the decoded values, then throws the explicit unresolved `SetOprHasDoe` guard. The actual lazy-owner test verifies no notification attempts, sample/file effects, or reader success follow. Constructor fields remain null/default, including uncommitted nominal text and data pointers.
- Prior mutations survive: the tests check fresh-row Header/invalid-sample prefix and A,B,A's earlier B ID/Description writes; later Description/Geometry and deferred links remain unreached. Retry on a nonempty canonical list is refused without replacement. Public Save remains refused before InputTable reconciliation/XML; the test checks old DOM identity and no additional XML/text/project-write effects.

The B121 source-fixture case uses the retained complete `DoeInputTable.XML` through `readInputTable`, then deliberately observes an unsupported partial result before notifier continuation. This does not establish a normal native Keyword read or public Type2 Save. The existing `writeInputTable` nonempty-variable refusal remains unchanged.

One optional coverage clarification: the integration tests exercise malformed Keyword contents, retry, and later non-Keyword children, but not two direct Keyword siblings. The helper is intentionally a per-keyword prefix and throws at the first notification boundary, so a later sibling is unreachable in the implemented profile; if future scope describes the whole DoeVariable shape as exactly one Keyword, add a parent-level admission test/guard. This is not a blocker for the current stop-before-notifier claim.

## Frozen source identities

Static source hashes at review time:

- `src/doe/variable_record.cpp`: `B105F2E2C2678CA7F08FE04399A47696562473DA74B613E254AEFDD1E8A028C6`
- `src/doe/keyword_reader_prefix.hpp`: `AEFD31950706D15BCDC8FE615087F67375C7ADB11842047BA83C0F75CFC36C95`
- `src/doe/keyword_reader_prefix.cpp`: `62D0C3010F5FAE117F3B29306D09EFBFF0E30E99314CFC81B0C286BEF79CC97E`
- `tests/doe/test_keyword_reader_prefix.cpp`: `4839425CC6775E978B40017381C4BAD4E52685212D9772872741FDDEE764C9AD`
- `include/m2m/doe_variable_record.hpp`: `3E1976E79B4205D3A5003F6ADC0D439EFC1C70ED285461E04348587ADEFF4AE8`

This review was static only. No tests/build, original process, GUI or mutable REA action was performed.
