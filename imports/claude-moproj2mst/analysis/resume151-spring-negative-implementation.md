# CP151 SPRING2 canonical negative decimal narrowing

Implemented the assigned count-one `SPRING2.INI` profile for canonical negative decimal tokens `-[1-9][0-9]*`, terminated by ASCII whitespace or EOF, with magnitude limited to `INT64_MAX`. For values outside the native `int` range, the reader accumulates the magnitude with an explicit checked unsigned-64 bound, applies two's-complement negation to the low 32 bits, and bit-casts that result to the signed flag. The earlier exact `-2147483649` branch remains in place and runs first.

The new branch requires the existing one-row qualified token gate. It rejects attached suffixes and values above the bound. The earlier leading-zero guard continues to reject `-010`; plus-sign overflow, multi-row input and other source-unqualified forms remain guarded. This follows the static Qt sign-completion and `int&` low-dword evidence recorded in `analysis/resume151-spring-next.md`; it is not an original-runtime comparison.

## Focused tests added

- `-2147483650\r\n` -> `2147483646`, exercising the family just beyond the existing exact witness.
- `-4294967297\r\n` -> `-1`, the requested distinguishing low-dword case.
- `-9223372036854775807` -> `1`, the upper admitted magnitude with EOF termination.
- Every accepted case asserts the SPRING and SPRING2 files remain byte-identical.
- One-row refusal cases cover attached suffix, plus sign, signed leading zero, signed-64 boundary, and values beyond the admitted signed-64 magnitude. The existing two-row negative-token matrix still rejects the new family outside its count-one gate.

## Static preflight and freeze

- `git diff --check` passed for the two owned source/test files.
- Inspected the conversion branch ordering: existing ordinary signed-int conversion and exact `-2147483649` behavior are retained; the new checked negative family runs only after `from_chars(int)` range failure and before the unsigned decimal fallback.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `3D39D2234423D326139EBE3B8FBD2D8782ED8F93B6D6FD247589BB8C21A26FB3`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `35736A964ABA90CBFF2346B1F390ADB994894097426BABED872F530221D04D28`
