# CP100 explicit-empty sample-count source review

The frozen source and focused cases match the retained native value and ownership evidence for the bounded profile. Static review found no blocker; I did not build or run tests.

## Parsed value and conversion

CP98's parser evidence applies to any attribute key: both immediate closing quote forms materialize a length-zero nonnull QString, store it on the DOM attribute, and return it through the found-attribute getter without substitution. CP100 binds the reader's `numSamples` key at `0x14090ef98`, the attribute call at `0x14090eff1`, and `QString::toInt(nullptr, 10)` at `0x14090f01f`. CP99's conversion captures show both null and empty length-zero values fail default-locale numeric parsing, retry the C locale, and return integer 0; the caller ignores the validity flag. The default QLocale constructor may initialize/publish Qt global locale state before length detection. The candidate explicitly does not emulate that process-global effect or claim literal Qt refcount/allocator parity.

The new source retrieves `sampleText`, distinguishes attribute presence, and maps either missing or `isEmpty()` sample text to engaged value 0. Present nonempty values still use the prior canonical-positive restriction, so whitespace, textual zero, signs, malformed values, and overflow remain guarded. The explicit-empty branch is admitted only with an explicit nonempty name, positive ID, and exact dimension 3, alongside the existing mode0/concrete-owner/LinkTo/child bounds. This is a declared reconstruction bound, not native validation. Positive-sample optional-header profiles from CP97/98 remain intact.

## Fresh and duplicate record effects

On a fresh matching name, the manager constructor stores the integer at record `+0x10`; for either empty or missing sample text that value is 0. Mode0 skips design acquisition, and supported later ID/dimension/Description setters, list reindex, and outer finalization continue. The duplicate helper finds the name before its constructor path and ignores the passed sample count. The source keeps the reader's separate last-record target. Tests verify A,B,A with empty sample text preserves counts 3 and 5 while updating B's later fields, and verifies existing count preservation across empty/missing/positive duplicate input.

The six focused tests cover both XML quote forms on a fresh zero-count record, duplicate sample ignore/last-target behavior, first-creator sample-count preservation across profiles, partial state before later guards, retained malformed/guarded combinations, and actual Session-owned lazy sample reads followed by the unchanged Save reconciliation refusal before XML/notifier effects.

## Obsolete fixture and limits

The old `numSamples=""` case was removed from the CP99 missing-samples guard vector because it is now covered by this admitted profile. The remaining missing-header combinations, whitespace/sign/textual-zero/overflow, payload, mode, and foreign-owner guards remain. Empty-name and absent-name profiles still refuse zero/default sample counts under the explicit-header bound. No broader Save, output, refresh, or notifier completion is claimed.

Reviewed: `source/apps/moproj2mst/src/doe/variable_record.cpp`, `source/apps/moproj2mst/include/m2m/doe_variable_record.hpp`, `source/apps/moproj2mst/tests/doe/test_variable_empty_samples.cpp`, and the updated `source/apps/moproj2mst/tests/doe/test_variable_missing_samples.cpp`. Root owns build/test validation.
