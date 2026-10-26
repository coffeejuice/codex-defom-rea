# CP149 FRCROT active-owner admission audit

## Conclusion

The CP145 notifier wiring advances the real Type2 lazy-reader path for the retained TLOC-only prefix, but it still does not produce an active FRCROT Keyword owner or parser inputs. For any non-TLOC Keyword, `retainedProfile` fails before concrete keyword construction and the reader throws at the existing Keyword-entry guard. For TLOC, the actual contextual reader commits a constructor-only value, enters the validated notifier prefix, and stops before key construction / receiver delivery or post-notifier named-item lookup. Therefore the retained FRCROT pair helper remains a private prepared-value seam; neither active Open nor Save can currently reach it with source-owned nominal/upper/lower values.

## Current active Open trace

The real Type2 SUB path in `src/model/subproject.cpp` builds `InputTableNotifierContext` from its actual `ConversionSession`, Project, owned SUB/Study and Project DOE I/O, then calls the contextual `readLazyStudy` overload. The latter reads the actual `DoeInputTable.XML`, binds the borrowed context to its per-call `InputTableSource`, and calls `readInputTableFromSource`. The bytes-only API remains source-less.

Within the reader, exact direct `DoeVariable` rows reach `appendBoundedHeaderVariableFromSource`; an exact direct `Keyword` child reaches `readBoundedKeywordConstructorPrefix` with the original row name, reader-local dimension 3, source context. The prefix is restricted to the retained TLOC attributes/children. It resolves the matching actual concrete Header owner in `Study::vars` order and appends the canonical constructor record and operation union. The TLOC record’s `nominalText`, `upperText`, and `lowerText` fields are still default-null at that point.

If context exists, the function calls `enterSetOprHasDoeNotifierPrefix` with operation 1 and `true`. This validates same Session/Project/SUB/Study/I/O/manager/notifier identities. It then either returns the native suppression result or, on unsuppressed entry, clears `flag90` and reports that key construction is required. The reader throws at either boundary: suppressed return stops before named keyword lookup; unsuppressed stops after flag reset but before constructing the signal key. Neither branch records a key/map attempt, visits a registered receiver, or returns to the reader. Thus the native post-callback name/count-minus-one re-fetch and the keyword text setters remain unreachable.

For FRCROT specifically, no contextual notifier stage is reached: the exact TLOC profile predicate rejects its XML row before owner lookup/construction. The implementation consequently retains the old unsupported Keyword-entry behavior for all other Keyword shapes. The presence of a Session context does not make FRCROT admitted.

## FRCROT parser inputs still lack an owner

The retained `d1.c` caller obtains a Keyword’s nominal, upper and lower text, name, parameter name and ordered `KwOperations`, then passes the three values to `FUN_140916530` nominal→upper→lower. CP142 identifies the FRCROT ID `276` / `0x114`, group-3 parser `0x140b667a0`, and handler3 `0x140b51140`. CP143 closes the parser’s forward-pair-before-mode and reverse/mirror tail order; CP144 captures that suffix as a value-level helper.

In active code, `keywordValues` checks group range and refuses any non-default handler3 before it records the first `temp.KWD` write. FRCROT’s registry handler3 is non-default, so even an imagined direct call currently refuses before text parsing. A source search finds no direct production callsite for `keywordValues` in the subtree; the declaration, definition and focused tests are the only references. The `ids` argument is currently discarded. Consequently there is no active adapter binding a real canonical keyword record to three text writes, the ordered `KwOperations` selector, three temporary models, the FRCROT group-3 parser, or handler3 output copying.

The most immediate owner gap is the notifier/key/map/receiver return and named-item re-fetch, because native Keyword text setters occur only after that return. Separately, even populated fields would still need the source-side handler3 dispatch and text-value path; CP144’s pair helper does neither. It assumes already-decoded IDs/mode/value/table outcome.

## Save/output frontier and guard

`writeInputTable` refuses nonempty `Study::vars` before constructing the output DOM or recording bytes. `saveInputTable` has the same nonempty-variable stop before constructing the path. `keywordValues` has its own non-default-handler3 refusal before `temp.KWD` effects. This audit found no production source path that removes these gates for FRCROT. No Save success, FRCROT XML serialization or DOE output ordering should be inferred from the actual lazy Open prefix or private pair-prefix tests.

## Smallest safe next step and validation contract

No FRCROT code integration is presently source-closed. The safe boundary is to retain the public Keyword guard and keep the pair helper disconnected. The next necessary source/evidence increment is one of:

1. close the actual Session notifier’s key construction, actual registered receiver population and synchronous delivery/return, then the reader’s named count-minus-one lookup/text setter sequence; or
2. independently qualify one retained complete FRCROT Keyword instance with its named owner, three text values and ordered operation IDs, without extrapolating an unobserved owner path.

After that, a later source increment can bind the actual three texts and selector into `keywordValues`/the group-3 reader. Validation should use an actual owned `Study` and contextual Type2 lazy read, include A,B,A to distinguish named owner from reader-last Header, assert notification operation order and post-callback re-fetch, and verify nominal/upper/lower parser calls occur in native order. Failure tests must preserve each committed prefix and prove that deferred-link resolution, later sample I/O and Save output do not occur after the reached guard. Public Save remains refused before XML output while the nonempty-variable gate is retained.

The existing CP144 tests remain the valid private validation: they cover prepared forward/reverse pair order, mode-table writes retained on later table-reader failure, and early mode failure without those effects. They do not establish InputTable parsing or FRCROT public support.

## Evidence qualification

Read-only review of the current contextual `activation.cpp`, `input_table_reader.hpp`, `variable_record.cpp`, `keyword_reader_prefix.cpp`, `variable_notify.cpp`, `keyword_constructor_record.hpp`, `doe_study.cpp`, `subproject.cpp`, their declarations and CP145/CP144 receipts; retained native caller evidence in `evidence/gui_mo/s1/mdt/DOE/d1.c` and the CP142/CP143 reports. Original image SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No source, build, GUI, original execution or mutable REA action occurred.
