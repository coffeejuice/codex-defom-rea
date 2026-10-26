# CP163 FRCROT actual-reader source and test review

## Decision

**Accept for the root build and test gate.** I found no source correctness or guard defect that blocks compilation or execution of the bounded prefix. This is a read-only review of the implementation and tests; no build, test, GUI, live REA, or original-process action was performed here.

The reviewed SHA-256 identities are:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` | `df1135df9b0d4410d551281845fb7e7c599b89d058b47056203242552d1a8a31` |
| `source/apps/moproj2mst/tests/doe/test_keyword_reader_prefix.cpp` | `54b656a7cc21daaf9f6d2291c405c65a5a7dfac3ac88431716ecb259bb1687de` |

The new admission checks six Keyword attributes and five direct element children in exact order. Each text wrapper has one direct `string`, and each `string` has one direct Text child with the required decoded ASCII value. `KwOperations` must have no raw children; `KwEntities` must contain only childless `KwEntity id="1"`. This closes the raw-comment/text and extra-child widening risk identified in `resume163-frcrot-plan-review.md`. `retainedProfile` runs before `appendFreshKeywordConstructorPrefix`, so malformed FRCROT entries cannot append a canonical Keyword. The valid branch appends ID276 with empty operations and entity `[1]`, reselects the original named Header owner, sets parameter/function/enum and three texts, then throws at `0x140910b6f`. It never reaches the notifier or converter. The exact branch preserves the existing TLOC profiles.

The accepted contextual test uses `ConversionSession`, Type2 lazy reading, recording StudyIo, and A,B,A ownership. It checks A's Keyword state, B's earlier Header/Description effects, unchanged sample state, absent notifier attempts, absent extra recorded writes, and unresolved deferred link. The guard loop covers identity, extra attributes, malformed text, newline/non-ASCII value, nonempty/raw operation children, and extra/wrong entities.

## Actionable nonblocking findings

1. `keyword_reader_prefix.cpp:19` still says rejected entries are outside a retained **TLOC** profile. A malformed FRCROT profile now receives that message. Update the guard text to name both admitted profiles, or use a neutral “outside the retained Keyword profiles” phrase. This affects diagnostics only and can follow the build gate.
2. `test_keyword_reader_prefix.cpp` does not explicitly exercise raw comment/text children directly under `Keyword` or `KwEntities`, although the implementation rejects them through its direct-child checks. Add those two malformed variants if the root test gate exposes a guard regression or when the predicate next changes. This is a coverage gap, not evidence of current widening.

The original parser's success, QTextStream/`temp.KWD` effects, scanner results, table-helper ownership, and output equality remain outside this prefix, as stated in `resume163-frcrot-integration-next.md`. The review does not elevate the synthetic values to observed original inputs.
