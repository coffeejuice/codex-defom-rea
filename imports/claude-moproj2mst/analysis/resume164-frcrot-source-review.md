# CP164 FRCROT selector/text source and test review

## Decision

**Accept the bounded Type2 reader increment for the build and test gate.** The reviewed source forms the invocation-local `[1,1]` selector from the one retained entity, commits the three admitted text values to the named Keyword owner, and throws `M2M_E_UNSUPPORTED` before converter call `0x14091111d`. I found no path in this admitted branch to a file operation, scanner, parser, table setter, notifier, sample continuation, or deferred-link completion. This is a static source/test review, not a successful native conversion or a test-run result.

| Reviewed file | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` | `2F16A73723BF400121E66D66E671A5FC258C480BBB920E5DFE4FC03D8DF8DA2E` |
| `source/apps/moproj2mst/tests/doe/test_keyword_reader_prefix.cpp` | `AA31C4563E562E0557C12D104E8BCA2F93582D9AEE3446F31D0CFB205923A788` |

Paths in this report are relative to the converter checkout. The hashes match `resume164-frcrot-implementation.md` at review time.

## Exact findings

1. `keyword_reader_prefix.cpp:36-57` admits exactly six FRCROT attributes and five ordered direct element children. Nominal, upper, and lower wrappers each require one `string` with one exact Text child. `KwOperations` is empty and `KwEntities` has only childless `KwEntity id="1"`. `retainedProfile` and reader dimension/mode checks run before `appendFreshKeywordConstructorPrefix` at lines 94-114, so rejected shapes do not append a Keyword.
2. The Type2 row calls this reader with the original row name and reader-local dimension 3 (`variable_record.cpp:183-188`). The reader resolves the original named Header owner before append and again before the text setters (`keyword_reader_prefix.cpp:98-128`). The constructor receives ID 276, operations `[]`, and entities `[1]`; the saved record retains those lists. It sets parameter `-1`, function true, enum false, and the exact nominal/upper/lower strings at lines 130-140.
3. The local `selector` at lines 138-141 copies the retained entity twice, checks `[1,1]`, and is not stored on the record or passed to a converter. The diagnostic and throw at lines 142-145 explicitly stop before `0x14091111d`. This satisfies the planned value-only boundary; it does not establish a reached native converter invocation.
4. The A,B,A contextual test at `test_keyword_reader_prefix.cpp:280-324` asserts named A owns the committed record, B has only its earlier Header/Description effects, selection and samples are preserved, the deferred B link remains unresolved, and recording StudyIo has only seed writes with no notifier attempts. The malformed-profile matrix at lines 326-357 covers wrong/multiple entities, populated/raw operations, direct raw/comment/text under Keyword or KwEntities, and malformed text. These checks are present in source; I did not run them.

## Nonblocking limits

- The test's `invocationSelector` at lines 310-311 is independently reconstructed from the record; it does not directly observe the reader's local vector. The local source and its explicit `[1,1]` check provide the guard evidence. A passing test alone would not prove that local expression remained correct after future edits.
- The recording StudyIo assertions show no extra recorded writes in the contextual test. The direct reader path contains no file API before its throw; this review cannot establish original `temp.KWD` bytes, path, codec, scanner outcome, or converter success. The static evidence qualifications in `resume163-frcrot-integration-evidence.json` and `resume163-frcrot-file-static.json` explicitly exclude a reached original FRCROT occurrence.
- `keyword_reader_prefix.cpp:19` still describes rejected profiles as outside retained **TLOC**, and `keyword_reader_prefix.hpp:9-12` still describes only scalar/TLOC profiles. These comments/diagnostics are stale for the admitted FRCROT shape. They do not change this branch's behavior; update them with a later documentation cleanup.

No source or test file, build, GUI, live REA session, original process, or shared plan/state was changed by this review.
