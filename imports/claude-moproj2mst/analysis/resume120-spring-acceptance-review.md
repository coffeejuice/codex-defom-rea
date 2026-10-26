# CP120 SPRING acceptance review

## Highest-priority gap

The highest-risk remaining acceptance gap is the combo-label transition, which is still not represented by the inspected candidate source. The current `fillExtra3D` path accepts `std::span<const int>` and checks `containsId`; lifecycle code still constructs `springComboItemIds` from model object fields. I found no label-string parser, current combo index, or transition function in the current source/test tree. Therefore current page tests can establish a bounded numeric projection and serialization, but cannot accept the native row-selection or item-click behavior described by the retained original.

The original combo selector scans rendered `itemText` in index order, parses each with `QString::toLocal8Bit` and `sscanf("%d")`, then selects the first matching index. Empty or failed parsing yields zero; no match preserves the old current index. The original label builder prefixes `%-3d - ` to the object name. These source facts are documented and hash-bound in `analysis/resume119-spring-itemclick-transition.md` and its capture receipts. Root/editor's accepted candidate admission of labels no longer than 64 ASCII bytes including NUL is a conservative candidate policy; it is not the original parser limit. The native parser copies unsafely into a 208-byte character region, so tests must establish rejection before conversion for strings outside candidate bounds.

### Required acceptance before claiming the transition

A focused private test should exercise actual label strings and ordered indexes, including: generated short ASCII numeric-prefix labels; prefix text such as `12suffix`; duplicate parsed IDs where the first index wins; empty and nonnumeric strings mapping to parsed zero; no match preserving the previous selected index; and length/non-ASCII rejection under the <=64-byte admission. It should then verify click-transition ordering against the original: commit the previous row first, parse the two displayed combo labels, load the new row and exact `flag == 1` state, and update the stored callback row. Programmatic current-row movement alone must not invoke the click transition. Keep the existing public `InitialEditorSession`/palette guard intact; this is a private page-model test, not GUI or public-conversion acceptance.

Until this lands, CP115-118 fixtures with integer arrays remain useful for numeric page and writer behavior, but their “item-data” producer claims must remain corrected and they do not close native combo fidelity.

## SPRING2 reader review

The current `readSprings` implementation has a narrow, explicit companion profile. It zero-initializes `flags`, treats missing/unopenable `SPRING2.INI` as all zero, reads up to the declared row count, leaves an EOF-short suffix at zero, and ignores additional canonical tokens after the count. The focused tests in `tests/mdt/test_simcontrol300_spring_reader.cpp` cover complete companion values, EOF-short values, empty file, malformed tokens, non-ASCII bytes, and UTF-8 BOM. Error cases assert an empty result and byte-preservation of both sidecars.

The BOM test correctly records candidate `M2M_E_UNSUPPORTED`; it does not prove original rejection. The retained original helper only establishes a default `QTextStream` integer-extraction loop with pre-zeroed values and ignored conversion status. The available profile deliberately excludes BOM, non-ASCII, malformed tokens, and overflow, so the candidate must describe those as unsupported input, not parity. Likewise, although the original loop stops after `count` values, candidate-wide ASCII validation may reject a non-ASCII or malformed suffix the native loop never consumes; that is a conservative admission boundary and should not be described as equivalent behavior for arbitrary tails.

No public Open/Save lifecycle, GUI event, original runtime, write-failure or post-close behavior is established by these reader tests. They are focused native-I/O candidate tests.

## Scope and outcome

Read-only source, test, and retained-receipt review. No source edits, builds, test executions, GUI actions, original-process execution, or mutable REA operations were performed. The immediate acceptance gate is a source-frozen label-aware transition plus tests against the retained first-match parser semantics; the current int-membership model remains candidate-only.
