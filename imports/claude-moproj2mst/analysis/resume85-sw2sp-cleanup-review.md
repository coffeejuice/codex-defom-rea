# CP85 independent review: dead SW2SP ladder cleanup preserves reachable behavior

No integration blocker was found. The cleanup removes branches that could only execute when an internal flag was true, while the sole remaining caller always passed false. The CP84 one-value SW2SP parser path remains intact. The generic false-profile behavior, including its legacy exact 010→8 rule, is preserved. No new public regression test is needed solely for removing this unreachable implementation; root's existing focused/full checks and freeze remain required.

Only this report was written. No source/test edit, compiler, executable test, GUI or mutable REA action occurred. Paths below are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Hash-bound comparison

Inspected cleanup receipt: `analysis/resume85-sw2sp-parser-cleanup.md`, SHA-256 `721b7969cb55e2046b75b836079119bd835b4d051431ad395fd9cb631540d8a5`. Active native adapter SHA-256 matches that receipt: `7899defd95f9b6f853afcf5096a91326ad07bf182855440e383424c129c97a0c` (266 lines).

The comparison baseline is the actual CP84 archive, recorded source tree `a516dd460fec17492a8a86b842b02f04c89835494a790a8b6a2c61b21f19f3a4`, archive SHA-256 `f9f4983754631c18496cdb7ff13ab11d3d03055665c8245caecb75a48074a6d6`. The archived adapter had 412 lines and SHA-256 `40b7accdb7a2a9d63d4b7788755f7ad1f29831c99d42e3aa58276109d0562598`.

A direct normalized-text comparison removed the readDecimalInt function slice from each version, then normalized only the old literal call `readDecimalInt(bytes, cursor, false)` to the new two-argument call. The remaining translation-unit text was exactly equal. Thus all source outside that helper is unchanged apart from deleting its unreachable flag argument.

Two dependencies were compared byte-for-byte with the archive and remain identical:

| Component-relative file | SHA-256 |
|---|---|
| src/engine/simcontrol300_qt_int_parser.hpp | 7784b177f2df89c2ea8ba413a40b51b5a985a2d40d7f5cae0e21a976ed05a8d7 |
| tests/mdt/test_simcontrol300_native_io.cpp | 1a865df23f8e5e68ef07fc67eeb0f966ad850fb5aba1c7df681ce2e31fbee224 |

The test file still contains 19 Testkit cases, including the independent 20-row parser table. No new acceptance result is inferred from unchanged bytes.

## Static call-path proof

Source search in the old and new translation unit finds exactly two readDecimalInt name occurrences: its definition and one direct call in NativeExtra3DIo::readInts. The function is in the anonymous namespace, and there is no other reference, stored callable or alternate true-profile invocation. CP84's sole call passed the constant false. Every removed literal block depended on allowSw2spOneIntProfile; none could succeed on that call path. The final signed-leading-zero condition is simplified by replacing `!(hasSign && false)` with true, which preserves the same rejection rule.

The route before the fallback remains exactly one requested integer and path spelling SW2SP.DAT. That route calls parseQt4Base0Int, enforces its sourceQualified guard, maps the fresh int operator and continues past the legacy helper. It therefore still implements the corrected case folding, radix selection, signed-high-bit completion and radix-specific rollback reviewed in CP84.

The ordinary Extra3D fill calls readInts on SW2SP.DAT with a std::array<int,1>, as shown in simcontrol300_io.cpp. A different path or count reaches only the surviving legacy helper. Removing a boolean argument does not widen that dispatch.

## False-profile semantic preservation

| Reachable generic behavior before cleanup | Behavior after cleanup |
|---|---|
| Skip ASCII space; return zero at end | Same cursor movement and return |
| Lowercase 0x prefix and lowercase hex digits; uint64 wrap and low-dword result; no-digit throw | Same checks, accumulation, delimiter cursor and error |
| Signed 0x/0X or unsigned uppercase 0X decimal-zero fallback | Same predicate, zero result and unread x/X cursor |
| Exact unsigned 010 followed by whitespace or EOF returns 8 | Explicitly retained; it was never behind the removed flag |
| Other multi-digit leading-zero generic decimals, including signed leading-zero forms | Same unsupported guard under the old false profile |
| Invalid decimal token/suffix or out-of-int32 qt::toInt result | Same validation and error mapping |

These are preservation statements about the candidate's legacy generic profile, not a new claim that those rules fully match Qt for every caller. In particular, its uppercase zero fallback remains a legacy restriction outside the qualified one-value route; CP83's native corrections are still supplied by the separate CP84 parser.

## Open, codec, path/count and SPRING boundaries

The unchanged outer code still zeros the span, opens the existing file, returns false on failed open, and returns true without decoding when the requested span is empty. Nonempty requests still pass asciiBytes before the one-value route; high bytes/BOM/undecoded input remain guarded. Ordinary path validation and exact filename matching remain unchanged.

SPRING.INI still differs from SW2SP.DAT, so its count is read by the same generic profile as before. Missing/nonpositive count still returns false; positive count still reaches the explicit unsupported populated-spring boundary. Legacy 010 can still produce positive count 8 before that boundary, which is intentional preservation rather than new SPRING allocation support. Multi-value requests retain the same legacy continuation/refusal behavior and have not become a full Qt status/pushback implementation.

## Validation decision

The CP84 tests assert externally visible values, path/count/decoder isolation and the corrected parser result/cursor/status table. They do not inspect the removed internal flag or branch layout, so their unchanged source is appropriate. A new public/ABI test of the deleted ladder would exercise no additional reachable behavior or boundary. There is no need to mirror the cleanup with an implementation-shaped test.

Root should retain the existing focused `m2m_mdt_simcontrol300_native_io` run and required complete source-stable launcher/archive. Runtime validation, source/executable bindings and overall fidelity limits remain root-owned; this review is static and makes no new passing-test or original-runtime claim. Token measurements are unavailable.
