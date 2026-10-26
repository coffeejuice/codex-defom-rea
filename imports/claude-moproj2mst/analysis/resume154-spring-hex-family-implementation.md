# CP155 complete unsigned SPRING2 hexadecimal implementation

Added a bounded count-one reader route for complete unsigned `0[xX][0-9A-Fa-f]+` tokens. It accumulates hexadecimal digits into a `uint32_t`, preserving the original low 32 bits after narrowing, and stops before ASCII whitespace. The route is gated by `allowSingleRowQualifiedToken` and requires a whitespace/EOF terminator.

Existing exact `0xA`/`0XA`, `0xA0`, bare-prefix and `0xG` handlers stay ahead of the general route. The complete-token path does not accept a missing digit or an attached non-hex suffix. Count-two input remains guarded. Signed forms, existing sign behavior, and non-ASCII/BOM codec handling are unchanged. This follows the Qt case-folded prefix/digit loop and low-dword store documented in `analysis/resume155-spring-next.md`; it is static source qualification, not original-process comparison.

## Focused tests added

- `0x1\r\n` -> 1.
- `0xA1\r\n` -> 161.
- `0XA0` -> 160, preserving uppercase prefix behavior through the general route.
- `0XfF\t` -> 255, covering mixed-case prefix/digits and tab delimiter.
- `0x80000000\r\n` -> `INT32_MIN`.
- `0xFFFFFFFF` -> -1 at EOF.
- `0x100000001\r\n` -> 1 after low-dword narrowing.
- `0xA00\r\n` -> 2560.
- Each admitted case asserts exact byte preservation of both input files.
- `0xA0G` remains a one-row refusal; count-two `0x1`/`0XA1` remain refused. Existing bare/invalid-prefix tests remain unchanged.

## Static preflight and freeze

- `git diff --check` passed for the owned source/test files.
- Inspected route ordering: exact prefix/status approximations remain first; the new family is count-one-only and catches only a complete recognized hex digit run; existing octal and decimal branches remain after it.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `62F972D295798FA02E84EA84FE09D1E6243D382903C4BD57AAF0DB36B91B17D4`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `A528A831FEE8F024F6C9EA52D0A1A819C1CD3B721C9680268A70B676E8854912`
