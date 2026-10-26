# CP118 bounded SPRING reader implementation

## Change

Replaced the exact count-one/count-two byte-string admission checks in `readSprings` with a count-driven parser for the bounded candidate grammar in `analysis/resume117-spring-general-parse-contract.md`.

The reader now accepts counts 1 through 256, ASCII whitespace between tokens, signed canonical decimal integers, and finite C-locale decimal reals with optional fractions and exponents. Each row has the original extraction order: object ID, two values, second object ID, three direction values. `SPRING2.INI` must remain absent, so each flag is zero. It stops after the declared row count and ignores any remaining ASCII suffix. The output vector remains empty until every required field has parsed.

The 256-row ceiling is explicitly a candidate resource policy, not a claim about an original limit. The integer and double conversions use full-token `std::from_chars`, avoiding the process-default locale behavior of the shared `qt::toDouble` helper. The byte decoder still rejects non-ASCII input, so the host-dependent default Qt codec is not generalized.

The focused test now covers the retained exact one/two-row seed and writer bytes, a changed two-row value, three distinct records, mixed whitespace, signed values, exponent notation, ignored trailing ASCII tokens, the 256-row cap, cap+1, malformed/partial rows, invalid/non-finite numeric cases, non-ASCII rejection, nonpositive and `x` behavior, and the present-companion guard.

## Static preflight and limits

`git diff --check` passed for the two owned files. Static inspection confirms the generic `readReal`/`readDecimalInt` routes used elsewhere are unchanged, and `readSprings` uses its new bounded readers only. The focused tests and application build were not run; root owns build/integration. No public Session admission, shared header, GUI, or mutable REA state was changed.

## Frozen file hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `0E91DBB6C2CE87158682F37077C71928EBE662146B34E3F5A2FEAB248F64E48A` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `8C82A9EAE9FFF8E05F43B1FF932CC28ABE63083B3828EC47C24D70273283D4AA` |

The retained evidence establishes the original extraction order, C numeric locale, absent-companion zero flags, no extraction after the declared row count, and partial-group write order. The newly accepted canonical token grammar and count ceiling are candidate-only admission decisions pending differential original-process validation.
