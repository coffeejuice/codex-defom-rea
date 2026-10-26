# Checkpoint 62: audit of generalized lowercase hex reader

## Finding

No new omission was found inside the currently implemented lowercase `0x[0-9a-f]+` profile. The candidate parser matches the bounded original QtCore path for those inputs:

- Prefix selection requires lowercase `x`; the accepted digit set is `0`–`9` and `a`–`f`.
- The candidate consumes at least one digit. If the first following byte is not a hex digit, the candidate leaves it at the cursor. The original getter's non-hex branch calls `ungetChar` after at least one digit.
- Candidate accumulation uses unsigned 64-bit shift/add, which wraps modulo 2^64 like the original register operations. It then bit-reinterprets the low 32 bits as signed. Original `getInteger` stores the 64-bit value and `operator>>(int&)` copies the low dword.
- EOF, CRLF, and a non-hex suffix on the one-value SW2SP reader are represented in the current finite matrix. Malformed prefix/no-digit cases remain guarded.

This is a bounded static comparison, not a new runtime result. The original QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the evidence is in `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, summarized in `analysis/resume61-general-hex-next.md`.

## Remaining boundary

Sign-prefixed and uppercase forms are still guarded in the candidate. The original base-0 decision tree does not select the lowercase hex branch for them: it checks lowercase `x` after a leading zero and handles nonzero/sign starts through other branches. Their exact fallback cursor, result, and operator status are not implemented or claimed by this audit. Do not broaden their candidate behavior without a separate branch-level qualification and focused fixture.

For a one-integer caller, `0x10x` returns the valid numeric prefix and leaves `x` unread in both paths. For multi-value reads, a subsequent extraction at that `x` enters an invalid-token/status path; the candidate still raises its guard error rather than representing Qt's sticky stream status and initialized-zero continuation. The source has not qualified that whole-row behavior for candidate support.

## Scope and hashes

No source or test edit was made for this audit. No build or CTest was run. No GUI or mutable REA action occurred.

Observed hashes during the audit:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `733D5DB9F3D8C86D2E5DDEA331C236B1838B014D9512B38274BB44090C7CDD63`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `BF8C620142F0CBE6D85307EF62BA4568B7FBD75076CF70296863C9ED494D5A6E`

The test hash reflects the current shared working file at the time of capture; the finite matrix includes an added `0x20` case.
