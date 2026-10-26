# CP128 bounded SPRING2 `0B10` binary-prefix implementation

## Change

The companion reader now admits exactly `0B10` followed by ASCII whitespace or EOF when the primary file requests one flag. It returns `2` and advances the candidate cursor past the four token characters; for `0B10\r\n`, the CR at byte offset 4 remains unread, matching the original stream boundary. The reader does not write either input file.

The focused test verifies the one-row flag and exact byte preservation. It retains one-row refusal for malformed/bare/signed/adjacent binary candidates (`0B2`, `0B`, `+0B10`) and broader tokens, and count-two refusal for `0B10`. Lowercase `0b10` is also outside this exact candidate witness, although the original Qt case-folds the prefix; that refusal is a candidate coverage boundary, not an original-format rejection claim. Binary overflow and repeated extraction remain guarded.

## Source qualification

`analysis/resume128-spring-next-token.md` cites `analysis/resume83-sw2sp-parser-independent-review.md`. That report records independently recaptured, PE-byte-verified Qt 4 `QTextStream::getInteger` and `operator>>(int&)` regions tied to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its call trace shows `QChar::toLower()` before base-prefix dispatch (`0x670c53bf`); its corrected table gives `0B10` -> 2, successful extraction, next offset 4 at EOF, status 0. With the CRLF fixture, the same four token bytes are consumed and CR is left unread.

The original SPRING2 helper `FUN_1410da690` copies the extracted integer, ignores conversion status and stops after its one requested flag. Therefore flag 2 is the static public-result prediction. No original-process differential was run. The token is ASCII; the result is within int range and makes no host ACP, overflow, or narrowing claim.

## Static preflight and freeze

`git diff --check` passed for the two owned files. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `2F362D26E682741FE19F721F1803C8672D575E0F8EA0D2DECFA1EC4D42A06AA0`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `280B6AFE0B3C7D6D03DB325552EF22C18094D2D0CFEF0043042F473D91F52764`
