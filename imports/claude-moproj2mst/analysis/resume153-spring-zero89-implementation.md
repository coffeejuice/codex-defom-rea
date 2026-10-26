# CP154 SPRING2 unsigned `08`/`09` decimal fallback implementation

Added a count-one complete-token reader path for unsigned ASCII digit strings beginning `08` or `09`. It scans the full decimal digit run, accumulates the low dword with defined unsigned 32-bit arithmetic, and leaves the first ASCII whitespace delimiter unread. The existing exact `09` handler remains before this family, preserving its prior one-row and separately qualified count-two behavior. The complete unsigned octal branch remains before both; signed-leading-zero values continue through their CP153 path.

The branch is gated by `allowSingleRowQualifiedToken` and requires ASCII whitespace or EOF after the digit run. Malformed suffixes and count-two forms remain refused. This follows the retained Qt second-digit base selector (8/9 selects decimal), decimal accumulator, and int low-dword store summarized in `analysis/resume154-spring-next.md`. It is a static qualification, not an original-process comparison.

## Focused test coverage

- `08\r\n` -> `8`.
- `089\t` -> `89`.
- `090` -> `90` at EOF.
- `09223372036854775807\r\n` -> `-1` through decimal low-dword narrowing.
- Each accepted case asserts exact byte preservation of both input files.
- `08x`/`089x` remain one-row refusals, and count-two `08`, `089`, and `090` remain refused. The exact `09` case and unsigned `010` octal case stay separately tested.

## Static preflight and freeze

- `git diff --check` passed for the owned source/test files.
- Inspected ordering and gates: exact `09` precedes the new one-row family; the new family only matches a leading zero followed by 8 or 9 and a complete ASCII decimal run; CP150 octal and CP153 signed-zero routes are unchanged.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `8AD57C52B787DD68B49AE3B44BF201C614C6ADC0AC299BF4FCF1ED53B8A31841`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `7A5994C23A94095B64EE938A5A2E649C44E686675A86161B8CAAB82FA64EEFB8`
