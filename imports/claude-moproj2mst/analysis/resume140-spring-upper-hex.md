# CP140 bounded SPRING2 uppercase hexadecimal-prefix implementation

## Change

The candidate now admits exactly one requested companion flag with complete bytes `0X\r\n`. It returns public flag zero and leaves the CR at offset 2. The focused test checks success, value zero, and byte-for-byte preservation of both input files.

The candidate does not model internal Qt conversion status. It keeps exact `0X` at EOF, `0XG`, spaced variants, and count-two `0X\r\n` guarded. The existing lowercase `0x\r\n` profile remains unchanged. These negative cases are coverage boundaries, not original-format rejection claims.

## Source qualification

`analysis/resume140-spring-next.md` describes this branch. The original PE-verified Qt report `analysis/resume83-sw2sp-parser-independent-review.md` records `QChar::toLower()` before automatic base-prefix selection and hex-prefix validation, so uppercase `X` is recognized as the same hex prefix as lowercase `x`. Its `0xG\r\n` witness documents the following invalid-character path: the prefix is consumed, the invalid post-prefix character is ungot, and the operator stores zero with non-EOF status. For `0X\r\n`, case-folding selects the prefix and CR is the unread nonhex character at offset 2. This exact result is static derivation, not a separate captured Qt table row or original-process comparison.

The original SPRING2 helper `FUN_1410da690`, summarized in `analysis/resume119-spring2-normal-path.md`, enters extraction because bytes are initially present, copies the integer result, ignores status and returns true for a successful one-flag read. Therefore public flag zero is predicted. Original executable SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `C4D45F3FAEE3F95DAD0A2AFA3C3EC9E0C295CA944660050B8B564C6129E77C1F`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `581DC8004B40AFF72B1ACBB13D3EED04AD9F6E57B6A310BDDCCCADC6D3FDCB04`
