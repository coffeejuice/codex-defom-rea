# CP126 one-row SPRING2 invalid-hex implementation

## Change and boundary

The SPRING2 reader now admits the exact one-row companion token `0xG` followed by ASCII whitespace or EOF. It returns flag zero and preserves both input files byte-for-byte. The branch is enabled only for `count == 1 && i == 0`; this is deliberate because Qt leaves `G` unread after consuming `0x`, and a second extraction has distinct retry and sticky-status behavior that remains guarded.

The focused reader regression checks successful one-row parsing, the zero flag, and unchanged `SPRING.INI`/`SPRING2.INI`. Follow-up CTest review exposed two candidate-boundary bugs in the refusal loop. An explicit exact-token refusal now applies when the one-row gate is false, preventing decimal fallback from treating `0xG` as zero and then reinterpreting `xG`. A separate exact bare `0x`-before-whitespace/EOF guard prevents the generic lowercase-x prefix rule from accidentally accepting the neighboring unsupported token. With a two-row primary record, `0xG` remains rejected. Nearby negative fixtures retain refusal for bare `0x`, `0xH`, `0xG0`, broader valid `0xA0`, and mixed-case `0Xa`. These refusals describe candidate coverage only; the original accepts `0Xa` as valid hexadecimal, and only the exact `0xG` failure path is newly admitted here.

## Original evidence

`analysis/resume126-spring-next-token.md` cites `analysis/resume83-sw2sp-parser-independent-review.md`, which independently recaptured and PE-byte-verified Qt 4 `QTextStream::getInteger` and `operator>>(int&)` against QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

For `0xG`, base-0 parsing consumes the hex prefix, fails to classify `G` as a digit, ungets `G`, and reports conversion failure. `operator>>(int&)` writes zero and sets conversion status. Original `FUN_1410da690` copies the integer without checking stream status; with a single requested companion value, the helper then completes normally. Thus the candidate's exact one-row public result is statically source-qualified. This is not an original-process differential test; multi-row retry, bare-prefix-at-EOF, other invalid digits, and other numeric forms remain outside this branch.

## Static preflight and freeze

`git diff --check` passed for the two owned files. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `BCF338275B54D5B61CABCD00581E9F48425F190BDC99C077FA8A454E05EEFA82`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `DDD75E47879CE4736A4BABB7856F12AE83EFF154492E37D10BE1E266442F264F`


