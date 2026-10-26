# CP145 FRCROT owner admission and Save frontier

## Finding

There is no source-closed next actual-reader integration for FRCROT beyond the CP144 prepared pair helper. The pair helper accepts already decoded IDs, mode/value, and table-read outcome; it deliberately has no XML reader or owner callsite. The current actual `DoeVariables` reader admits one narrow direct TLOC Keyword profile, appends a constructor-only canonical record to a named `HeaderVariableRecord`, then throws before the first `SetOprHasDoe` notifier call. FRCROT needs the keyword's populated nominal/upper/lower text and operation selector on the downstream conversion path, and those values are not supplied by that stop-before-notifier prefix.

The next required owner link is the actual named Keyword constructor plus notifier delivery and the later reader text/selector setters, producing a coherent owned FRCROT record whose `nominalText`, `upperText`, `lowerText`, and ordered `operations` are known. Only then can a source-backed invocation adapter build the three parser inputs and call the pair projection. The existing native call order is source-qualified, but the required active owner/value inputs are not.

## Evidence in the current source

`src/doe/variable_record.cpp::appendBoundedHeaderVariable` recognizes direct `Keyword` children after the reached row-header/Description effects and calls `readBoundedKeywordConstructorPrefix(study, name, 3, *child)`. `src/doe/keyword_reader_prefix.cpp` accepts only the exact B121 TLOC record profile, resolves the original row name against coherent owned Header aliases, appends the canonical constructor value/operation prefix, and throws before notifier delivery. Other Keyword profiles retain the entry guard.

The canonical `KeywordConstructorRecord` (`src/doe/keyword_constructor_record.hpp`) has `nominalText`, `upperText`, and `lowerText` default-null. It receives constructor `nominalValue`, operation IDs and metadata, but the CP144 stop point does not run the native later text setters. That is a material gap for FRCROT: the retained d1 caller reads the current keyword's nominal/upper/lower QStrings, name, parameter name and `KwOperations`, converts name/parameter to local8, then calls `FUN_140916530` in nominal→upper→lower order. CP142 binds that call to the FRCROT parser and handler3. The reader-level Text values cannot be inferred from CP144's TLOC-only fixture or from the CP144 pair helper's prepared inputs.

The current `doe::keywordValues` implementation checks group range and rejects a registry record whose handler3 is not the default before it records any `temp.KWD` write. FRCROT is registry ID 276 and uses nondefault handler3 `0x140b51140`; therefore this function currently stops at the handler3 guard. A source search finds no production callsite for `keywordValues` in `source/apps/moproj2mst`; its visible uses are the definition, declaration and focused tests. This is not a reachable public FRCROT value path today.

Separately, `writeInputTable` refuses any nonempty `Study::vars` before DOM creation or output effect; `saveInputTable` also refuses nonempty variables before path construction and delegating. FRCROT owner admission alone cannot enable public DOE Save, and this task does not propose removing either guard.

## Retained native order and remaining dependency

CP142/CP143 establish the original sequence:

1. The reader recognizes a Keyword entry and dispatches the record on the original Variable name.
2. The derived manager selects/constructs the named keyword and appends it to the owned list; it invokes `SetOprHasDoe(operationId, true)` in supplied operation order.
3. Only after notifier returns does the reader re-fetch the named last keyword and apply keyword flags/nominal data setters.
4. The d1 caller later copies keyword nominal, upper, lower texts and ordered `KwOperations`, then `FUN_140916530` writes and parses the three text inputs in that order.
5. For FRCROT ID `0x114`, group-3 parsing establishes the forward pair before mode/value parsing, and after mode success reaches the later table reader and common reverse/mirror tail. The CP144 private pair helper models this suffix only from prepared observations.

The exact missing witness at the active boundary is therefore not another pair-list helper. It is a real named-owner path through notifier delivery/return and text-value population (or an independently qualified native capture that supplies those concrete values and their owners). It must also retain whether callback delivery can mutate the keyword list before the reader's count-minus-one lookup. CP144 explicitly stops before that callback, so neither a borrowed constructor pointer nor a presumed last-item pointer can substitute.

Even after that owner link is closed, FRCROT text generation is not fully modeled: `keywordValues` has no production callsite, and public input/output serialization still has the variable guards described above. The private pair helper must remain disconnected from conversion until the source can bind the real record's values and the reader's actual delivery/re-fetch boundary.

## Smallest safe candidate and validation contract

The smallest source-ready increment now is a **private owner-boundary adapter that remains refusal-only**, not FRCROT parsing: preserve the current exact TLOC constructor prefix and return/receiver guard, while adding no FRCROT row admission. The one already implemented reusable source seam is CP144 `applyFrcrotPairPrefix`; it needs no further integration or mutation for this checkpoint.

For any later FRCROT owner increment, require all of the following before claiming reader-level reach:

- an exact retained FRCROT InputTable fixture or equivalent source-qualified row profile, including direct child order, attribute presence, nominal/upper/lower text and ordered `KwOperations`;
- a coherent actual `Study`/`HeaderVariableRecord` owner selected by original row name, including A,B,A duplicate behavior;
- a qualified notifier argument order, actual Session/receiver identity, delivery result and post-callback named count-minus-one re-fetch;
- tests that verify partial effects before and after notifier, that failure leaves deferred links unreconciled and skips later sample I/O, and that all three FRCROT texts are passed nominal→upper→lower;
- isolated prepared-pair tests continue to assert mode failure vs later table-reader failure, recognized table fields retained, and reverse-tail order without asserting unknown row/mirror payload bytes.

Public Save validation must continue to prove the existing `keywordValues` handler3 refusal happens before `temp.KWD` effects and the nonempty-variable Save guards still stop before `DoeInputTable.XML` output. No FRCROT public result should be claimed based only on passing private pair-prefix tests.

## Qualification

Read-only source review of `src/doe/variable_record.cpp`, `keyword_reader_prefix.cpp`, `keyword_constructor_record.hpp`, `doe_study.cpp`, `activation.cpp`, their declarations/tests, and retained `analysis/resume142-frcrot-input-provenance.md`, `resume143-frcrot-parser-order.md`, and CP144 keyword-reader receipt. Original GUI PE SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No source/build/GUI/original execution/mutable REA action occurred.
