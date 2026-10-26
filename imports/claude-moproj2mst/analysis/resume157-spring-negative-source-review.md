# CP157 SPRING2 negative-overflow source review

## Result

No blocking defect found. The released reader/test hashes match `analysis/resume157-spring-negative-implementation.md`:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `5FBE75CD40647CD063AE27AB0128592BC6AA564D0B8CD499082C9B9B8F88BC43`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `348CF582BBE0BF6CD6915DC3FC187A3521A638AF88B027D6A24C7A380B49C154`

## Source behavior

The new behavior sits inside the existing integer-conversion-error path, after the exact `-2147483649` case and before the explicit-positive fallback. It requires the count-one qualification, a `-` followed by `[1-9]`, and an EOF/ASCII-whitespace terminator. It accumulates only the digit magnitude in `uint64_t`; unsigned multiplication/addition wraps modulo 2^64. It negates only when the result is nonzero and has its high bit clear, then converts the low dword to `int32_t`. This matches Qt's signed `jle` skip of `neg` for zero/high-bit-set wrapped accumulators and the following low-dword store. It avoids signed overflow.

The guard leaves signed-leading-zero tokens on their existing CP153 path, the explicit-positive wrap path unchanged, and attached suffixes rejected. For a multi-row call, the one-row condition is false; the negative overflow cannot enter this branch, and the later fallback refuses the signed prefix. The BOM decoder remains upstream at the SPRING2 callsite, so the numeric logic sees the decoded ASCII payload without modifying the raw sidecar.

## Tests

The test matrix covers `-2^63` -> 0, `-(2^63+1)` -> 1, `-(2^64-1)` -> -1, `-2^64` at EOF -> 0, and `-(2^64+1)` with tab -> -1. Existing `-4294967297` and `-9223372036854775807` controls remain. Accepted cases check both input files are unchanged. The one-row refusal set now explicitly checks `-18446744073709551617x`; the two-row refusal set includes `-9223372036854775809` and `-18446744073709551617`. The positive-wrap table remains present and unchanged in the same test file.

## Limits

This was a read-only source/test review; no build or CTest was run. The evidence is static Qt PE and caller composition, not a fresh original-process comparison. The conclusion applies to the bounded count-one canonical ASCII negative-decimal grammar and does not claim malformed-stream status parity or generalized Unicode/locale behavior.
