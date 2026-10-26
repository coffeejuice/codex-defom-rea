# CP127 bounded SPRING2 `018` octal-prefix implementation

## Change

The companion reader admits exactly `018` followed by ASCII whitespace or EOF when the primary record requests one flag. It returns `1`, records the next unread position at the final `8`, and does not mutate either input file. The one-row gate is shared with the prior bounded one-token profiles. With two requested flags the token remains rejected; no second extraction at the unread `8` is modeled.

Focused regressions check the one-row value and byte preservation, one-row refusal of `019`, `0180`, and `08`, and two-row refusal of `018`. These neighboring candidate refusals are scope boundaries, not original-format rejection claims: retained Qt evidence says `08` uses decimal fallback and returns 8, while broader octal/partial-token branches are not admitted here.

## Source qualification

`analysis/resume127-spring-next-token.md` binds the exact token to `analysis/resume83-sw2sp-parser-independent-review.md`. The latter records PE-byte-verified Qt 4 `QTextStream::getInteger` and `operator>>(int&)` code from QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, paired with `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

At `0x670c5417–0x670c5446`, base-zero selection tests the digit after the leading zero: 0–7 selects octal, while 8 or 9 selects decimal. In `018`, the initial `01` is consumed as octal value one; `8` is invalid for that radix and is ungot. The corrected witness table states value 1, successful getter, next byte offset 2, and fresh status zero. The original `FUN_1410da690` SPRING2 helper copies the int result, ignores status, and ends after the single requested flag. Therefore flag 1 is the statically predicted public result. This is offline static evidence, not an original-process differential run.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `36C50890F0AD319FA2FB98E87D91D9844F69A3C6097D2F2D9E634B5F520C8CA1`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `BF7866B93C4193F271812BE4426AAB60DD41F3D5F4F3EDCB6095F7DD7D280CC5`
