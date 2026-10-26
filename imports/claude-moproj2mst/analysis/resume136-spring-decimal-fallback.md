# CP136 bounded SPRING2 `09` decimal-fallback implementation

## Change

The SPRING2 reader now admits exactly `09` followed by ASCII whitespace or EOF when one flag is requested. It returns 9 and leaves the following CR unread for `09\r\n`. The focused test verifies one-row success, flag value, and byte-for-byte preservation of both input files. Other leading-zero spellings remain guarded; in particular, the existing one-row refusal list retains `08`, `019`, and `0180`. A two-row primary with `09\r\n` is also kept guarded by the count-two refusal set.

## Source qualification

`analysis/resume136-spring-next.md` binds this branch to `analysis/resume83-sw2sp-parser-independent-review.md`. The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; shipped QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The independently recaptured, PE-byte-verified `QTextStream::getInteger` table states `09\r\n` -> 9, successful extraction, next unread offset 2 (CR), and status zero. Its base-zero selection branch (`0x670c5417–0x670c5448`) sends a second digit of 8 or 9 to base 10 rather than octal.

Original SPRING2 helper `FUN_1410da690`, retained in `analysis/wave2-save-diffs-300-support-1410da690.c` and summarized in `analysis/resume119-spring2-normal-path.md`, extracts one requested integer, copies it without inspecting stream status, and returns true after successful open. The public one-row result is a static source/control-flow prediction, not an original-process differential. The fixture is ASCII and value 9 is in range; no ACP or overflow behavior is involved.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `4E09D88FBB75201C29E4766F56336B83EE4E1393ACB5DCC760AA51928FEA57CE`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `30F7A0D79DBB96198403BB2599AF97AA49A6A28CD06814CEA84E8527B1EF8FA7`
