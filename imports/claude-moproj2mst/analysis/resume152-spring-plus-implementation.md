# CP152 SPRING2 explicit-positive decimal narrowing

Implemented a bounded count-one `SPRING2.INI` branch for complete explicit-positive decimal magnitudes from `INT32_MAX + 1` through `INT64_MAX`, with ASCII whitespace or EOF terminators. The reader uses checked unsigned-64 magnitude accumulation and stores the low 32 bits as the signed flag. Existing in-range signed parsing is unchanged. The CP151 negative branch and earlier unsigned, octal, exact-prefix, and malformed-prefix routes remain in their prior order.

The branch requires `allowSingleRowQualifiedToken`, requires a plus sign followed by a nonzero decimal digit, and rejects attached suffixes, leading-zero spellings, malformed sign sequences, and magnitudes above the selected bound. The generic leading-ASCII-whitespace skip remains in effect, matching the earlier Qt `QChar::isSpace` source finding; non-ASCII whitespace/codecs remain outside the candidate's ASCII decoder profile. Existing count-two refusal cases for `+4294967297` and `+2147483648` remain in the test matrix.

## Focused coverage added

- `+4294967297\r\n` -> `1`, the requested discriminator and public Save normalization case.
- `+2147483648\t` -> `INT32_MIN`, covering a tab delimiter.
- `+9223372036854775807` -> `-1`, the upper admitted magnitude at EOF.
- Every accepted case asserts byte-for-byte preservation of both input files.
- One-row guards cover attached suffix, signed leading zero, malformed double sign, and `INT64_MAX + 1`.

The source basis is recorded in `analysis/resume152-spring-next.md`: recognized positive signs select decimal, the parser accumulates the digits in 64-bit arithmetic, the positive completion path does not negate, and the int extractor stores the low dword. This is static qualification, not original-process parity testing.

## Static preflight and freeze

- `git diff --check` reported no whitespace errors in the owned source/test files.
- Inspected routing: existing in-range `from_chars(int)` behavior remains first; the explicit-positive branch is reached only after int-range failure and before the unsigned decimal fallback, under the one-row gate.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `09D6FCD852F3D5F2C5942318816A10F734C796E5C107C3125C087A057EFA16B8`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `52F359644B6E12C7F538252A0A73062AE260420FA180813ED3F05ADA8ECE0541`
