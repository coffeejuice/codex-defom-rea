# CP137 independent frequency text-edit source review

## Result

No blocker found in the frozen callback projection or its focused tests. The implementation preserves the source-qualified immediate single-row store contract and does not claim public Save or actual Qt event delivery.

## Native order and candidate behavior

The CP137 contract report records six slot-map handlers (rows CNT/TMP/DBW/MXW/VEW/YLD) with bounded code spans and hashes. Each handler resolves current SC, reads FRQNCY stride and computes row×stride, calls `QString::toInt(ok=null, base10)`, converts the signed int32 exactly to double, then obtains the values pointer and stores one value. The candidate follows that ordering: it checks binding/cache and candidate text admission; obtains current SC and verifies source-control identity/6×1×ncomp6; then calls the existing `qt::toInt`; then reads the live values pointer, checks nonnull, and performs one store. Empty text therefore converts to integer zero and positive double zero. No extra widget enable, combo, validator, Apply, Fill, Save, or I/O branch was introduced.

Canonical positive ASCII int32 admission and zero-length text are clearly labeled as a bounded candidate profile. Cache must already contain the delivered text; assigning the cache alone does not synthesize the event. Unknown, excluded, mismatched, or unsupported owner/table cases refuse before the frequency store. The tests exercise each of six rows, max int32, null/empty zero, repeat delivery, no cache-only/Apply store, excluded lexical cases, wrong bindings, and missing/replaced SC/table boundaries. Tests assert model/table effects rather than Qt delivery.

The helper and receipt explicitly retain the limits: no native widget delivery or validator acceptance is observed, the KnownC locale qualification is a declared condition, QLocale/provider/refcount effects are not claimed, and `completeForPublicSave` remains false. Existing Fill/Apply semantics and public constructor/palette gates stay unchanged.

## Frozen hash check

Both exclusive-file hashes match `analysis/resume137-editor-frequency-edit.md`:

- `src/engine/simcontrol300_owner.cpp`: `b7729388e0528dd988c1e8db0a57ca3b9f9ef29a4f79cf547e99433e8cf3841c`
- `tests/mdt/test_simcontrol300_frequency_edit.cpp`: `d546b4e3f84e2e6991b6402da3b9d4105cbcdaff201037f9d5dd6e4ac4f114e2`

This was a read-only static review; no build or test execution was performed.
