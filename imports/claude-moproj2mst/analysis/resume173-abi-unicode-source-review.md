# CP173 ABI Unicode diagnostic truncation source review

## Review result

The proposed test at `tests/g0/test_abi.cpp` matches the public adapter contract and the current `abi.cpp` implementation at source level. Its file SHA-256 is `daea0039bf5a46586da33d31ef7712e131e70aedc2656c21681f9d206d519e83`. The new test is deterministic under the intended Windows test environment: it creates a unique missing `.moproj` leaf beneath the configured scratch root, converts the same UTF-16 path to UTF-8 for the second API call, and checks `M2M_E_OPEN` from both calls.

No source behavior blocker found. One small test-hardening change is recommended before acceptance: make the missing-file precondition fatal (`REQUIRE`) rather than `CHECK`, since the subsequent error-code and Unicode-message assertions rely on that path remaining absent.

## Boundary and assertion audit

The filename contains U+1F600 as explicit UTF-16 units `0xd83d, 0xde00`. On Windows `wchar_t` is 16 bits, matching the ABI's Windows target. The test obtains the UTF-8 path with `WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, ...)`, excluding the resulting path terminator before passing it to `m2m_convert_utf8`.

The missing-project diagnostic is source-qualified in `src/core/convert.cpp`: failed open throws `M2M_E_OPEN` with `"cannot read " + qt::String(path).utf8() + ...`. Thus both public calls receive the same path-based UTF-8 diagnostic after decoding their equivalent paths. The 2048-wide-character and 4096-byte buffers are ample for this bounded scratch path; full UTF-8 decoding and exact equality against the full wide result detect truncation or conversion mismatch.

Let `p` be the index of the supplementary scalar in each full diagnostic. A vector size of `p + 2` includes one terminator slot, so the maximum payload is `p + 1`: that deliberately allows only the high surrogate in the wide case, or only the UTF-8 leading byte in the UTF-8 case. `putMsgW` backs off when the last copied code unit is a high surrogate; `putMsgUtf8` backs off while the last copied byte is a continuation byte, which correctly retreats from this partial leading-byte sequence to the scalar boundary. The test checks the NUL at `p`, the resulting prefix on each API, valid UTF-8 decoding, and equality between decoded UTF-8 and wide output.

## Small blocker and acceptance

Change `CHECK(GetFileAttributesW(missing.c_str()) == INVALID_FILE_ATTRIBUTES)` to `REQUIRE(...)`. If the supposedly missing path exists, the current test merely records a failure and continues, weakening the premise that both API calls must take the missing-file branch. The generated leaf is distinctive, so collision is unlikely, but the assertion should enforce its precondition.

After that adjustment, source-level acceptance is complete. Runtime behavior remains unverified because this review was explicitly read-only; no build or test was run. The report does not claim any new original DEF_GUI_MO Open/Save/Close behavior or public lifecycle acceptance. No source, test, fixture, build, GUI, live REA state, or shared runtime state was changed or used.

## Final verdict (CP172)

The final test source now hashes to `daf5d17361747d14626aa94889a834c1164ada6fb382b514dc857cc25fd4254c`; the absent-path prerequisite is fatal. The coordinator reports focused `m2m.g0.abi` passed in the CP172 candidate. The Unicode-safe buffer assertions are accepted for this adapter contract. Test scope is limited to diagnostic-buffer conversion/truncation on a missing-file path; it does not establish original application behavior, successful project Open/Save/Close, or runtime equivalence beyond this deterministic error path.
