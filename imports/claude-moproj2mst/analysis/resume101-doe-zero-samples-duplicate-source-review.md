# CP101 duplicate-only textual-zero sample review

The frozen candidate keeps textual `numSamples="0"` behind an existing-name-only branch and leaves fresh zero-count creation guarded. Static review found no blocker for this bounded path; no build/tests or original runtime execution were performed.

## Why the duplicate branch is qualified

The original reader converts the attribute before calling manager `vt+f8` (`0x14090f01f`, then `0x14090f0ac`). Conversion validity is ignored. CP101's additional QtCore evidence shows the numeric parser checks the active locale's zero digit before its ASCII digit path; `QSystemLocale` can supply that field, so the conversion result for literal ASCII `0` is not proven independent of runtime locale. The default `QLocale` constructor may also publish process-global state before parsing. The candidate correctly avoids depending on either fact: it does not synthesize a numeric result for this profile, and it refuses to create a fresh record from textual zero.

For an existing name, CP94's complete helper evidence proves `0x140919de0` branches around allocation, constructor, append, name assignment, next-number query, optional-number store, selection update, sample invalidation, and design acquisition. The helper's `numSamples` argument is ignored. After the helper returns, the reader separately obtains the last record and applies the supported setters there. Thus the exact parsed integer is irrelevant to the bounded duplicate state transition; locale parse/global initialization itself remains explicitly unmodeled.

## Frozen implementation and tests

The source recognizes only exact ASCII text `0` as `textualZero`. `canonicalPositive("0")` remains disengaged, and the admitted path requires the same nonempty name, positive ID, and dimension 3 reconstruction bounds as CP99/100. It performs the source-qualified name lookup, then rejects `textualZero` unless an existing owner was found, before any constructor/owner/list mutation. Consequently `*samples` cannot be read in the fresh branch for this case. Existing positive, empty, and missing-count profiles retain their prior behavior.

Six focused tests cover same-name duplicate count/owner preservation, A/B/A last-record targeting, preservation of first-creator counts from positive/empty/missing profiles, partial mutations before later guards, fresh textual-zero refusal plus malformed/header/payload/mode/foreign guards, and actual Session-owned lazy sample continuation followed by the unchanged Save refusal before XML/notifier effects. The CP100 fresh `numSamples="0"` refusal remains explicit, so this patch does not claim a zero-count fresh owner.

Reviewed source/test: `source/apps/moproj2mst/src/doe/variable_record.cpp`, `source/apps/moproj2mst/include/m2m/doe_variable_record.hpp`, and `source/apps/moproj2mst/tests/doe/test_variable_zero_samples_duplicate.cpp`. The plan is `analysis/resume101-doe-zero-samples-plan.md`. This does not claim locale-independent numeric conversion, QLocale process-global lifecycle parity, arbitrary numeric strings, public Save completion, or native allocation/refcount fault parity. Root owns build/test validation.
