# CP153 signed leading-zero SPRING2 implementation

Implemented a count-one exception to the SPRING2 leading-zero guard for complete signed decimal tokens whose parsed values fit in `int`. The exception requires the existing `allowSingleRowQualifiedToken`, a leading `+` or `-`, at least two digits after the sign, a zero first digit, and ASCII whitespace or EOF immediately after the digit run. The existing `from_chars(int)` conversion then computes the ordinary signed decimal result.

This matches the retained Qt path: a recognized sign selects decimal before unsigned base-zero leading-zero dispatch. Unsigned `010` still follows the CP150 octal path and yields 8. Incomplete suffix forms, multi-row values, signed leading-zero overflow, non-ASCII/codec cases, and other routes remain guarded. Existing CP151/CP152 signed narrowing paths apply only after the leading-zero check and remain unchanged.

## Focused test coverage

- `-010\r\n` -> `-10`.
- `+010\t` -> `10`.
- ` -0012` -> `-12`, covering leading ASCII whitespace and EOF.
- `+000\r\n` -> `0`.
- All accepted cases assert byte-for-byte preservation of both input files.
- One-row malformed suffixes `-010x` and `+010x` remain refused; two-row `-010` and `+010` remain refused. Existing unsigned octal expectations are retained.

## Static preflight and freeze

- `git diff --check` passed for the two owned files.
- Inspected the guard placement: only a complete signed one-row leading-zero token bypasses the old refusal. It then uses the existing `from_chars(int)` range conversion; over-range tokens still fall through to the guarded overflow profiles.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `E1BD637DBE55A875C9BBED604663ECACD3599647AF77DBADCF0D365479B4AD8E`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `4C946D622384E5BD30DC9599D17998D98287167BA3A87D35D6A78749F7502508`
