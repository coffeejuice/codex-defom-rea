# CP150 SPRING2 complete octal family implementation

Implemented a bounded count-one profile for complete unsigned octal integers in `SPRING2.INI`. When the token begins with `0`, has only octal digits, and ends at ASCII whitespace or EOF, the reader accumulates base 8 and exposes the resulting flag. Inputs remain unchanged. The existing exact `018` and `019` partial-token rules execute first and remain unchanged.

The new branch is limited to the already-qualified one-row route. It rejects signed values, leading whitespace, non-octal suffixes, values above `INT32_MAX`, and general count-two octal inputs. It does not alter primary `SPRING.INI`, SPRING2 file access, or save behavior.

## Focused coverage added

- `02\r\n` -> 2, `010\r\n` -> 8, `017\r\n` -> 15, plus `010` at EOF -> 8.
- Each accepted case asserts both input files retain their exact source bytes.
- One-row negatives cover signs, leading space, invalid suffixes, overflow, and existing `08`/partial-token cases.
- Count-two negatives cover `010\r\n` and `010 1\r\n`; existing exact count-two `018` behavior remains tested separately.

The branch follows the already retained QtCore `getInteger` base-zero selection and octal accumulation evidence documented in [resume150-spring-family-next.md](resume150-spring-family-next.md). The selected boundary intentionally keeps octal overflow outside this cut, despite the original low-dword store, because no overflow behavior is added without an assigned qualification.

## Static preflight and freeze

- `git diff --check` reported no whitespace errors for the two owned files.
- Inspected the parser ordering: exact `018`/`019` handlers precede the complete-octal branch; `allowSingleRowQualifiedToken` gates the new branch; generic parsing remains below it.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `C4340EA223AB9B6D701EB770328E5C888C7F0609BD07FEE6F17BA5A762FC968C`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `55F724E24288ABF8CBDBEFE6FB83AEF2953442D0F4099FD1F3A651E4B85B4261`
