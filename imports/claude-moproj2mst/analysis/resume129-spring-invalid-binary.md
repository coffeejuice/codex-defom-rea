# CP129 bounded SPRING2 `0B2` invalid-binary implementation

## Change and scope

The companion reader now admits exactly `0B2` followed by ASCII whitespace or EOF when one flag is requested. It returns flag zero and keeps the parser cursor at the start of the token, matching Qt's full prefix rollback. The single-row gate is the existing `allowSingleRowQualifiedToken` parameter; a count-two `0B2` input remains guarded, so no retry/status sequence is widened.

Focused tests verify one-row success, zero result, and byte-for-byte preservation of both source files. One-row negative neighbors include `0B3`, bare `0B`, signed-prefixed `+0B10`, and lower-case candidate spelling `0b2`; the multi-row guard loop also includes `0B2`. The lower-case refusal is only the exact-witness candidate boundary: the original case-folds `b` and therefore has equivalent source behavior for `0b2`.

## Original Qt qualification

`analysis/resume129-spring-next-token.md` relies on the immutable, independently recaptured Qt PE evidence in `analysis/resume83-sw2sp-parser-independent-review.md`. The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

For `0b2`, the corrected Qt table records failed integer extraction, destination zero, fresh status 2 and next unread position 0 with the whole binary prefix restored. The getter lowercases before base selection; the binary invalid-first-digit path ungets both the stored `b` and the leading `0` (`0x670c595f–599b`). Thus uppercase `B` in `0B2` follows the same branch. `operator>>(int&)` stores zero on failure. The original SPRING2 helper `FUN_1410da690` copies the destination and ignores status; for one requested flag it completes without another extraction. Public flag zero is therefore a static PE/caller prediction, not an original-process differential result.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `286BE2D4F1A77DAB737D315410F9DC012C695824CF5E55E59D9FBD1AABDE6040`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `13D305842209CB7AD533B86CD5925E92BC0D7AAF4D9B811D270C90E203D29B7E`
