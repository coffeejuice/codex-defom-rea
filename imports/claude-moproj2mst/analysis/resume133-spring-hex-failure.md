# CP133 bounded two-row SPRING2 hexadecimal-failure implementation

## Change and exact profile

The reader now admits a two-row primary record only when the complete companion byte string is exactly `0xG\r\n`. The first extraction consumes `0x`, returns zero and leaves `G` unread. The second exact retry at offset 2 returns zero and leaves `G` unread again. The resulting two flags are `[0, 0]`; the original input bytes are unchanged.

The retry is gated by both `count == 2` and full-byte equality with `0xG\r\n`, so mixed tokens, duplicate token spellings, leading or trailing whitespace, and count-three input remain guarded. Tests cover those cases plus the positive exact profile and verify that refused reads leave `rows` empty and source bytes unchanged. Existing one-row `0xG` behavior is preserved.

## Original-source basis

The retained contract is in `analysis/resume133-spring-next.md`. Qt extraction mechanics are independently recaptured and PE-byte-verified in `analysis/resume83-sw2sp-parser-independent-review.md`, tied to original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

For the first extraction, the hex failure leaves `0x` consumed and ungets only invalid digit `G`, so cursor is offset 2. `atEnd()` is based on unread buffer/device input and remains false. The integer operator attempts another getter even with prior nonzero status; at `G`, decimal parsing fails and ungets it again, storing zero while preserving the existing status. Original helper `FUN_1410da690` prezeros slots, loops on `!atEnd()` and count, and increments after each extraction without examining status; with exact count two this produces two zeros and returns true after successful open. These are static PE and caller-flow conclusions, not an original-process differential result.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `0B4832377E1EB03A5C68AEEF15A4519F1D9844D1193DB115857D1BA087A79B8B`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `C9A4B3FA1CF51EC77B0C02EB21B942D0A7826C450D419E2EF0E535922C1496D5`
