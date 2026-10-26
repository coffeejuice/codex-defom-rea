# CP143 private actual-Header keyword constructor/append prefix

The source-qualified fresh-empty-list constructor prefix is implemented on the canonical Item owner. It validates actual Study/Header ownership before mutation, stores the supplied dimension, constructs and appends one concrete record, then unions operation IDs in encounter order. It stops before reader notification, text/selector continuation and conversion. No public parser or Save guard was changed.

Only these three new worker files and this new receipt were written. Root owns build/integration; no shared header, emitter, Study/Session, public reader, CMake, original process, GUI or mutable REA file/action was changed.

| File under source/apps/moproj2mst | Frozen SHA-256 |
| --- | --- |
| src/doe/keyword_constructor_prefix.hpp | 18519459e43775f6894744d7d75afb544e943a18dfb633dc7bb3e2af2958e09e |
| src/doe/keyword_constructor_prefix.cpp | 799f0b0fb17226091828f125810838bea2a1f27e1f562fbfb5df3d28e28d1c3a |
| tests/doe/test_keyword_constructor_prefix.cpp | ea83340bf9c591a637b03c5518ba923e3c21f2543e88c6527c8423384a8674da |

The source freeze was sent to root before writing this receipt. No source edit followed it.

## Interface and owner admission

`KeywordConstructorPrefixInput` contains already-decoded keywordId, parameterName, unitType, dimension, nominalValue, operations and entities. `appendFreshKeywordConstructorPrefix(Study&, HeaderVariableRecord&, const KeywordConstructorPrefixInput&)` returns `KeywordConstructorPrefixResult` with a borrowed concrete record pointer. Its `completeForReader` and `completeForPublicSave` are both compile-time false. The returned pointer does not own or extend the Header/Item lifetime and is not a native callback-return receipt.

Admission requires matching counts of concrete Header owners and borrowed Var aliases; all Header owners and aliases must be nonnull, every alias must belong to an owner, aliases must be distinct, and the explicit receiver must occur in that coherent owner set. Scalar/geometry prefix owner domains and retained scalar-prefix state remain outside this bounded Header profile. The receiver's canonical Item list must be empty, including refusal of legacy values, existing typed owners and explicitly native-null entries. These are reconstruction admission checks before any prefix write, not a claim about native validation/failure ordering.

Alias order need not equal owner-vector order. The explicit actual receiver is used; no last-record/name lookup, manager selection or fallback is performed. Header ownership remains concrete, never through unique_ptr<Var>. There is no additional keyword list or owner back edge.

## Native normal order retained

The source contract is `analysis/resume141-doe-keyword-owner.md` and its bounded original-byte capture. Native816b40 stores its supplied dimension at816b8f before search/allocation; the fresh-empty-list bound skips search, including the special ID0x131 bypass. The constructor9206e0 then supplies normal record values, append occurs at816d24, and only afterward does the operation-list loop run.

The implementation follows that order as separate statements:

1. Store `receiver.dimension = input.dimension`.
2. Construct one move-only KeywordConstructorRecord using the already-decoded values. Its data slots remain constructor-null; later text/selector setters have not run.
3. Transfer its unique concrete owner into the sole canonical Item vector. Retain only a borrowed result pointer.
4. For each supplied operation ID, append it to receiver.ops only if it is absent. Existing duplicates remain; the record's operations/entities lists retain the original supplied order and duplicates.

No numeric parser or registry check is invented on these explicit values. Empty operation/entity lists, null parameter name and signed dimension values are normal value inputs to this private slice, not public XML admission. No sorting, pre-reservation of the operation union, rollback or sample invalidation is added. A host exception retains any completed earlier statements naturally, but no forced allocation-failure test or claim of native allocator/Qt refcount/exception parity is made. The normal native null-allocation branch is not manufactured by this helper.

## Focused test contract

New target `m2m_doe_keyword_constructor_prefix`, CTest `m2m.doe.keyword_constructor_prefix`, has six cases:

- Actual Study-owned Header receives one concrete typed record; constructor metadata/defaults/null data and original entities/operation lists are exact. Existing Variable ops `[5,5,-1]` with supplied `[5,8,8,-2,9]` become `[5,5,-1,8,-2,9]`.
- A non-last explicit receiver with permuted coherent aliases is changed; the other Header and alias order remain untouched.
- Explicit empty lists, null name, ID0x131 and signed values remain value inputs without invented parser/registry validation.
- Eleven foreign/null/duplicate/mismatched/legacy/typed/native-null/other-owner profiles refuse before dimension, operation, Item or sample mutation.
- Retry after a committed first prefix refuses the now-nonempty list, retaining the original pointer, dimension and operation union.
- Description, IDs/index, geometry row/link, selected index, regions and all sample payload/flags remain unchanged. Existing public `saveInputTable` still refuses at0d2b60 with no planned file effects, even after the private prefix has run.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_keyword_constructor_prefix.cpp` returned `ok: true`, no issues. Direct UTF-8/final-newline/trailing-whitespace checks passed for the three source/test files. This is source preflight only; no worker compiler, build, CTest or executable was run. Root's grouped Release build and tests remain authoritative.

## Remaining boundary

The actual public Keyword reader still refuses at its existing child entry; no source-qualified QString constructor occurrence, manager lookup, SetOprHasDoe delivery, later type/text/selector setter or reindex is inferred from these arguments. CP138 still stops at nonempty Item snapshot/conversion. Global operation-manager ownership, conversion/parser/entity-pair provenance and DYNARY output failure lifetimes, active Save reconciliation and refresh receiver return remain guarded. This prefix is not a public Save-completion token.
