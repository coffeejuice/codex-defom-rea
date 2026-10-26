# CP164 FRCROT selector/text handoff review

## Decision

**Source-ready for a bounded actual-reader prefix, with a hard pre-call guard at `0x14091111d`.** The retained Type2 `Keyword` reader already recognizes one exact FRCROT function profile, appends constructor ID 276 with empty operations and entity `[1]`, reselects the original named owner, and commits function/parameter plus nominal, upper, and lower text before throwing at `0x140910b6f`. The next increment can replace that terminal throw with the reader-derived selector/text state and a new terminal `M2M_E_UNSUPPORTED` immediately before converter invocation. This is source readiness only: the selector/text state remains synthetic projection of a statically qualified input, not evidence of a reached original invocation.

## Reader dataflow and guard

In `keyword_reader_prefix.cpp`, `exactFrcrotFunction` admits only the specified six attributes, five ordered child elements, three exact text values, empty `KwOperations`, and one `KwEntity id="1"`. The selector source is that entity list, so the native builder's one ID maps to `[1,1]`; operation IDs must not supply it. The saved record's entity data therefore remains `[1]`, while an invocation-local selector value may be recorded as `[1,1]`. Keep registry ID 276, reader-local dimension 3, function true, enum false, and parameter `-1`.

Do not broaden the accepted XML/profile or add a converter-result interpretation. Keep all admission checks before `appendFreshKeywordConstructorPrefix`; after any admitted mutation, preserve the explicit unsupported exception. Stop before calling `0x140916530` at `0x14091111d`: no `temp.KWD` output, host file creation, scanner result, parser success, table projection, data setter, DYNARY update, notifier, sample continuation, or deferred-link completion is licensed. A nonnull/return-value shortcut would also be wrong because native proceeds to setters only after the three returned pointer slots are individually nonnull.

The existing failure text and comment identify `0x140910b6f` as the stop. Update both when moving the stop to `0x14091111d`, and keep the stop explicitly before the call instruction. Do not claim “conversion succeeded” or “file handoff completed.”

## Minimal contextual tests

1. Extend the existing A,B,A actual lazy-reader test to assert the exact value-only handoff: named A owns constructor ID 276, entities `[1]`, empty operations, selector `[1,1]`, dimension 3, parameter `-1`, function true, enum false, and the three exact text values. Confirm B retains only its already-reached Header/Description effects, selected index is unchanged by name relookup, samples are unchanged, and deferred links remain unresolved.
2. Assert the stop diagnostic names `0x14091111d` and `M2M_E_UNSUPPORTED`; recording StudyIo shows no new `temp.KWD` write or other conversion output, and notifier/DYNARY/sample continuation remain absent.
3. Keep and run the current exact-profile malformed-shape matrix. Add focused FRCROT mutations for a wrong/multiple `KwEntity`, a populated `KwOperations`, and raw/comment/text nodes under `KwEntities`/`Keyword` if not already present. Each must reject before a keyword append; the implementation's direct child-kind checks otherwise appear sound.

No converter/table-helper tests belong in this increment: they cannot establish file ownership or original scanner semantics. The retained `frcrot_table_prefix` accepts prepared observations and is specifically not the file owner.

## Exact stop

Stop the source path before `0x14091111d`, with the value-only selector/text handoff inspectable and all later conversion effects unsupported. Continue beyond that point only after obtaining the same-occurrence original-process evidence tuple described in `resume164-frcrot-file-next.md` (QFile open and stream codec/output bytes, actual `FILE*` reopen and scanner cursor/results, then parser outcome), followed by the later table/helper/output-lifetime witnesses. No GUI/live REA action is part of this source increment.
