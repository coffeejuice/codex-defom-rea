# CP137 bounded SPRING2 `09` EOF-short implementation

## Exact admission

The reader admits `09\r\n` only when the primary record count is two. It parses the first flag as 9; the next loop check skips the remaining CRLF and leaves the already-zero second slot at 0. The public values are `[9,0]`, and the byte-exact primary/companion fixtures remain unchanged. This candidate path models the public values; it does not emulate Qt's second attempted extraction or report its internal EOF status.

Tests cover exact count-two success, exact-byte preservation, refusal of mixed/space-varied count-two spellings, and refusal of exact `09\r\n` with count three. The one-row `09` profile remains unchanged. Other parser and count paths remain guarded.

## Original-source qualification

`analysis/resume137-spring-next.md` derives the behavior from the retained Qt table and `FUN_1410da690` caller evidence. The original executable SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

For the first extraction, the independent PE-verified table records `09\r\n` -> 9, success, next unread offset 2 (CR), status zero. `FUN_1410da690` prezeros the two flag slots and loops while `!atEnd()` and index<count. With CRLF still unread, the second loop iteration is attempted; QTextStream skips CRLF, reaches EOF without a digit, stores zero and gets EOF-side status 1. The helper increments to count two, ignores status and returns true after successful open. This exact two-extraction result is static combination of the PE parser/operator, atEnd, and caller control flow—not a captured original-process differential. The adapter preserves only the public values for this exact admitted file and does not claim internal status equivalence.

## Static preflight and freeze

`git diff --check` passed for the owned source and test. No build, test executable, GUI, original process, or mutable REA database was used; root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `EC8A68078CFF4515E4FEC9B7D3D70361E5B70DC31FC2569524EF59E2086C08A7`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `BACBA785E5274B920EF8B0F697CBB5E78C52AF503DC8FB8C0655FF5B0008E5D6`
