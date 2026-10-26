# CP143: fresh DOE keyword constructor-prefix review

## Review result

Static review found no behavioral blocker against `analysis/resume141-doe-keyword-owner.md` or the canonical move-only `Item` architecture. The implementation is a private value-prefix helper on a concrete `HeaderVariableRecord`, with an actual `Study` owner/alias admission check. No public reader/Save route calls it.

## Mutation order and ownership

`appendFreshKeywordConstructorPrefix` validates Study/Header coherence and an empty canonical Item vector before writes. It rejects mismatched owner/alias counts, null owners/aliases, foreign aliases, duplicate aliases, a receiver not represented by the concrete Header owner, scalar/geometry-prefix owner domains, retained scalar-prefix state, and any legacy/typed/native-null Item entry. These refusals precede changes to dimension, Item ownership, operation union or samples.

After admission, statements follow the required normal path: supplied dimension store; construct exactly one move-only `KeywordConstructorRecord` from decoded ID/name/unit/nominal/operations/entities; move its unique owner into `receiver.items`; then append only operation IDs not already present, in input encounter order. The record preserves duplicate/order in its own operation and entity vectors. The stable union keeps existing duplicates and appends first-seen new IDs: existing `[5,5,-1]` with input `[5,8,8,-2,9]` becomes `[5,5,-1,8,-2,9]`. The result pointer is borrowed from the actual Item owner; it is not a receipt that prolongs ownership or marks parser/Save completion.

The shared Item remains the only keyword collection. The constructor record is concrete, noncopyable, and held behind `unique_ptr`; its three data slots remain explicitly null, with no DYNARY owner. The snapshots from the prior CP142 migration remain non-owning for typed records. No alias, shared owner, parallel keyword vector, allocator-failure emulation, or native layout cast appears in this helper.

## Test and guard evidence

The six-case test covers actual Study/Header ownership, record defaults and identity, supplied operation/entity order, permuted-but-coherent aliases targeting an explicit non-last receiver, unvalidated already-decoded values including ID `0x131`, refusal before mutation across eleven invalid owner/list profiles, retry refusal after the first commit, and preservation of other Study/Header fields. It also calls `saveInputTable` after appending and verifies the existing nonempty-variable refusal leaves file effects empty and the typed owner/dimension intact.

The existing dimension-prefix behavior still checks `!variable.items.empty()` and stops at `KeywordItemSnapshotBoundary`; this helper does not widen that path. Existing empty-variable prune and B121/B122 item-empty guards retain their `items.empty()`/nonempty checks. The public input-table reader still has its separate, highly qualified B121 TLOC profile and produces a scalar projection; no reference to the new helper exists outside its declaration/definition and the new private tests. `saveInputTable` still refuses any nonempty Study at the existing `0x1400d2b60` boundary before file effects. No notification call, public Keyword admission, conversion call, later selector/text setter, or active Save reconciliation was introduced.

## Limitation

This was a read-only source review. It does not establish compilation, generated target linkage, runtime order, or behavioral parity under allocation failure. The root-owned grouped Release build and focused/full tests remain necessary. In particular, the test demonstrates prefix state and preserved refusal; it does not establish native `SetOprHasDoe` delivery or public Save completion.

## Source binding inspected

| File | SHA-256 |
|---|---|
| `src/doe/keyword_constructor_prefix.hpp` | `18519459E43775F6894744D7D75AFB544E943A18DFB633DC7BB3E2AF2958E09E` |
| `src/doe/keyword_constructor_prefix.cpp` | `799F0B0FB17226091828F125810838BEA2A1F27E1F562FBFB5DF3D28E28D1C3A` |
| `tests/doe/test_keyword_constructor_prefix.cpp` | `EA83340BF9C591A637B03C5518BA923E3C21F2543E88C6527C8423384A8674DA` |
| `src/doe/keyword_constructor_record.hpp` | `EB93B6D0E342364D794F2979B4F7983BDD66241FA569FDA5524C25328280CB00` |
| `include/m2m/mst/doe_model.hpp` | `388AAC1F072164C85085D93CD776FD9E29B2948EC6806951CDD4CF39B608DBF0` |
| `src/doe/variable_dimension_prefix.cpp` | `1922502BFBC94967A29B489DFA562DF79C8B5D12478A54C5940060992C365BC4` |
| `src/doe/doe_study.cpp` | `D9DEECAE5E5AF237778E842120B05337DCE37A1AB97F098BB8FFDA7014DC3E2B` |

Only this new analysis report was written. No source/build/test/GUI/mutable REA operation occurred.
