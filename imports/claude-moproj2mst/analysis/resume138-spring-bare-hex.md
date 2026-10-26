# CP138 bounded SPRING2 bare-hex EOF implementation

## Exact change

The reader now admits only the one-row companion whose entire content is the two bytes `0x`. It returns public flag zero and records the input cursor at EOF. The candidate deliberately does not model Qt's internal conversion status. The test verifies successful read, flag zero, and byte-for-byte preservation of both files.

Trailing-whitespace form `0x\r\n` remains refused, as do exact `0x` under count two, malformed `0xG` alternatives and other neighboring prefixes. The existing status-2 `0xG\r\n` one-row profile is unchanged. This narrow result does not generalize bare prefixes or multi-row retry behavior.

## Original evidence

The source contract is in `analysis/resume138-spring-next.md`, based on the retained PE-verified Qt 4 table in `analysis/resume83-sw2sp-parser-independent-review.md` and the original SPRING2 helper path summarized in `analysis/resume119-spring2-normal-path.md`. The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

For `0x`, Qt consumes the automatic hexadecimal prefix and reaches EOF without a digit. The retained table reports getter failure, destination zero, offset 2 at EOF, and fresh `operator>>(int&)` status 1. The original helper checks `atEnd()` before extracting; the input initially has two bytes, so it calls the operator, copies its zero result, ignores status and returns true after its single requested flag. This is static Qt/caller inference, not an original-process differential. The candidate matches the public result only, not Qt status.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `B666F52FE00A0F59C60BE1E64746098EC66E5130E254DAFD583C9B54B34A63FF`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `E918760E41613538B82D7141E40ABB27C127D0203C6CD914382A59E80CDB81CC`
