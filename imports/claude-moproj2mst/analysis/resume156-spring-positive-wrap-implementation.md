# CP156 explicit-positive SPRING2 decimal wrap implementation

Extended the existing count-one explicit-positive canonical decimal route beyond `INT64_MAX`. It now accumulates the decimal digits directly into `uint32_t` and stores the signed interpretation of the low dword. This matches Qt's 64-bit multiply/add wrap, positive-sign no-negation completion path, and `int&` low-dword store. The lexical gate remains `+[1-9][0-9]*` with ASCII whitespace/EOF termination and `allowSingleRowQualifiedToken`.

The change supersedes only the CP152 implementation's signed-64 magnitude cap for this positive-sign family. It preserves CP153 signed-leading-zero behavior and all other exact/profile gates. Negative overflow keeps its distinct checked/conditional-sign path. Leading-zero plus tokens, malformed suffixes, multi-row inputs, codecs/BOMs and other filenames remain outside this change.

## Focused tests added

- `+9223372036854775808\r\n` -> 0, first value above `INT64_MAX`.
- `+18446744073709551616` -> 0 at EOF, one 64-bit wrap.
- `+18446744073709551617\r\n` -> 1, low dword after 64-bit wrap.
- `+36893488147419103233\t` -> 1, multiple 64-bit wraps and tab delimiter.
- Existing `+4294967297` -> 1 and `+9223372036854775807` -> -1 remain covered.
- All accepted cases assert exact byte preservation for primary and companion files.
- The count-two refusal matrix now includes `+18446744073709551617`.

## Static preflight and freeze

- `git diff --check` passed for the owned source/test files.
- Inspected the positive branch ordering: it runs only after the ordinary int conversion fails; it uses the existing one-row canonical plus/no-leading-zero and complete-terminator gates, before the unsigned fallback. No new sign, suffix, count, or codec routes were opened.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `89FABC0920BD9B972684F88CABB749C2A91230BF68092CEC01E0E864AC3DB7B9`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `ADBFF916D98668FEA9A2FBD1F72CCF1C40AD585A2C284C0F3774F3D575A272B1`
