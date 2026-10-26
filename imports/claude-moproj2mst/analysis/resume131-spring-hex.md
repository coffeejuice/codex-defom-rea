# CP131 bounded SPRING2 `0xA0` hexadecimal implementation

## Change

The companion reader now admits exactly `0xA0` followed by ASCII whitespace or EOF when the primary record requests one flag. It returns 160 and advances past four token bytes; for `0xA0\r\n`, CR at offset 4 remains unread. The one-row regression checks the value and verifies byte-for-byte preservation of `SPRING.INI` and `SPRING2.INI`.

One-row neighbors `0xA1`, `0xA00`, and `0xA0G` remain guarded. The existing multi-row refusal set retains `0xA0`. The original parser accepts more case-folded and hexadecimal spellings, so these refusals are candidate coverage limits, not claims of original rejection. No general hexadecimal grammar or overflow behavior is admitted.

## Source qualification

The contract is recorded in `analysis/resume131-spring-next.md` and grounded in the PE-byte-verified Qt 4 `QTextStream::getInteger`/`operator>>(int&)` captures in `analysis/resume83-sw2sp-parser-independent-review.md`. Those captures are tied to original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The getter case-folds before hex-digit classification and accumulates hexadecimal digits. Thus `0xA0` consumes `0xA0`, computes 10*16+0 = 160, and leaves CR unread with successful extraction status zero.

The original `FUN_1410da690` SPRING2 helper copies the extracted integer, ignores stream status, and stops after the requested single flag. The public result is a static call-path deduction; no original-process differential test was performed. The fixture is ASCII and the value is within int range, so no ACP or overflow claim is involved.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `35CD7F6E8B9BAD099CDE74F07EAD2F88CE3D93626DF424B07372BF0AABA46035`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `10C4C26ECDF44D95BA92F3997B94D381DD0FB8EC4DF9FE08F5A8375D125BCA53`
