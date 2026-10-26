# CP130 bounded SPRING2 `0b12` partial-binary implementation

## Change

The SPRING2 reader admits exactly `0b12` followed by ASCII whitespace or EOF for one requested flag. Qt accepts binary digit `1`, then stops at invalid binary digit `2`, ungets that character and successfully returns the accumulated value `1`. The candidate returns flag 1 and leaves its cursor at the unread `2` (offset 3); with CRLF, the CR follows at offset 4. Both input files remain byte-for-byte unchanged.

The focused regression checks the one-row value and file preservation. One-row candidates `0b13` and `0b12x` remain guarded, along with malformed/bare/sign/other binary spellings; count-two `0b12` also remains guarded because it would require modeling the next extraction at the unread `2`. The candidate admits only this exact token, not general binary parsing.

## Original evidence

The retained independent Qt review is `analysis/resume83-sw2sp-parser-independent-review.md`, tied to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its PE-byte-verified corrected one-value table lists `0b12\r\n` as result 1, successful getter, next byte offset 3 (the `2`), and fresh integer-extraction status 0. The getInteger analysis documents case-folding before base-prefix selection and a binary digit loop that returns the accumulated value if it has consumed at least one digit before an invalid digit is encountered.

The original SPRING2 helper `FUN_1410da690` calls the same Qt integer extraction, copies the result and ignores stream status. Since the requested count is one, the helper exits without retrying at the unread `2`; it returns success with flag 1. This is static PE and caller-flow inference, not original-process differential evidence. The token is ASCII, the value is in range, and no ACP, overflow, or low-dword narrowing behavior is involved.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `4035164D5A420AA1E892E3D6B39C940EAEE1D9D5F83134F6C76009A2289DA613`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `0C9183B5EE38504295FFDADD62F4B4EA6F0A3AF855F0F0F426751C64312DA171`
