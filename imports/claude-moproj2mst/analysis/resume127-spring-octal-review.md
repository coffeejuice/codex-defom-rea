# CP127 independent review: bounded SPRING2 `018`

## Finding

The frozen branch is correct for its exact one-row profile: with a readable ASCII companion beginning with `018` and followed by whitespace or EOF, the candidate returns flag `1` and leaves both source files unchanged. Retained Qt evidence establishes that base-zero parsing chooses octal because the second digit is `1`, consumes `01`, then ungets the non-octal `8`; extraction succeeds. The SPRING2 helper requests only one flag and stops at its count, so that unread byte is not retried in this case.

No defect was found within this admitted profile. This is static source qualification, not a fresh original-process differential. The narrow token gate excludes other byte sequences; it does not establish that Qt rejects them.

## Native evidence

`analysis/resume83-sw2sp-parser-independent-review.md` is based on the PE-byte-verified Qt 4 `QTextStream::getInteger` and `operator>>(int&)` captures, tied to QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The base-selection branch at `0x670c5417–0x670c5446` selects octal for a second digit 0–7, and decimal for 8 or 9. Its corrected table gives `018\r\n` -> value `1`, getter success, next unread offset `2` (the `8`), and fresh extraction status zero.

The retained `FUN_1410da690` source/call-path evidence (`analysis/wave2-save-diffs-300-support-1410da690.c`, summarized in `analysis/resume119-spring2-normal-path.md`) shows a count-limited extraction loop after successful file open. It copies the extracted integer and ignores stream status. With one requested row the loop ends after the successful value `1`; it does not request another integer at the unread `8`.

## Candidate and test boundary

`simcontrol300_native_io.cpp` recognizes only the exact `018` spelling followed by ASCII whitespace or EOF when `count == 1 && i == 0`. It returns `1`, leaves its cursor at `start + 2`, clears pending comma state, and then the one-slot loop ends. The test checks that result and byte-preserves both `SPRING.INI` and `SPRING2.INI`.

The neighboring candidate refusals are not native rejection claims. In particular, `08` is source-qualified decimal fallback and yields `8`; it is a separately omitted input profile. The implementation also does not admit general octal, arbitrary suffixes, or additional requested flags. For example, the retained parser facts imply that a following extraction would encounter the unread `8`; this review does not qualify a broader multi-row malformed-input result.

No build, tests, GUI, original runtime, source edit, or mutable REA operation was performed.

## Frozen identity checked

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `36C50890F0AD319FA2FB98E87D91D9844F69A3C6097D2F2D9E634B5F520C8CA1`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `BF7866B93C4193F271812BE4426AAB60DD41F3D5F4F3EDCB6095F7DD7D280CC5`
- implementation receipt: `analysis/resume127-spring-octal-prefix.md`
