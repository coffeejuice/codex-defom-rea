# Checkpoint 46 format fidelity audit

## Scope and current state

This read-only audit covers the converter subtree's RFORMAT lane: encoding/codepage, numeric locale, and paths. The current recovery cursor is checkpoint 46. Its RFORMAT item is `ready`, but implementation and integration are partial and validation is incomplete (`analysis/takeover-state.json`, RFORMAT entry). The required acceptance remains original-bound locale/encoding/path fixtures with raw differences preserved.

The current source has explicit codepage plumbing (`Ctx.codepage`, `m2m_set_codepage`, and `qt::encodeLocale` call sites), but that is an explicit converter profile. It does not establish the original DEF_GUI_MO process's locale at launch. The host observation reports ACP 1252/OEM 437 only for the host PowerShell process; its own qualifications say this is not an original-process query. The original executable's manifest has no `activeCodePage` override, but that static fact also does not report the process ACP. Thus 1252 is a plausible test profile, not yet an accepted original default.

## Evidence-qualified findings

1. **Path/string codec choice is consumer-specific.** `wave3-ht-native-contract.json` ties a HeatTreatWizard.HTWZ path to `QString::toLocal8Bit` before CRT `fopen("wt")`, while the same function uses Latin1 for `userKey`; media names use local8Bit. `wave3-native-simctrl-contract.json` independently records QString local8Bit then CRT open for SimCtrl.KEY. `wave3-cogging-native-io-contract.json` qualifies a Cogging path constructor using `toAscii`. These are distinct contracts; one global “all paths use ACP” rule would overgeneralize. The Qt decompilation records codecForLocale/fromUnicode and null-codec Latin1 fallback (`wave3-ht-native-contract.json`), but process codec discovery and fallback activation remain unbound.

2. **The codepage field is not itself evidence of the original codepage.** `source/apps/moproj2mst/src/core/convert.cpp` copies the caller-supplied codepage into `Ctx`; the wide and UTF-8 ABI entrypoints pass `g_codepage`. `source/apps/moproj2mst/src/engine/engine_internal.hpp` describes `local8` as system ANSI codepage, while path writers increasingly encode using the explicit `Ctx.codepage`. The audit needs to show where this configured adapter matches the original's runtime locale and where it is only a deterministic profile.

3. **Numeric locale remains a separate open branch.** Current SimControl lifecycle call sites pass `NumericLocale::KnownC` (`convert.cpp`, `conversion_session.cpp`), and format-sensitive code rejects unknown locale (`simcontrol300_lifecycle.cpp`, `simcontrol300_remesh.cpp`). This is a useful guard, but no reviewed original-process observation here binds `LC_NUMERIC`/CRT locale for the target process. A codepage observation cannot close that question. Existing numeric tests show formatting boundaries, not original parity.

4. **Path evidence is strongest for representable-byte paths, not edge paths.** Current native I/O contracts retain narrow CRT semantics, MAX_PATH restrictions for engine KEY paths, and guards for unsupported or unqualified path shapes. The audit should distinguish a proven path byte conversion from proof of behavior for unrepresentable characters, embedded NUL, relative paths/current-directory changes, extended UNC paths, and paths at/over CRT limits. Treating guarded unsupported paths as recovered behavior would be incorrect.

## Highest-impact next fix / fixture

Add one **original-bound locale fingerprint fixture at the next already-required original probe**, rather than schedule a new GUI-control session solely for this lane. In the owned original process, record `GetACP()` and CRT `setlocale(LC_ALL, NULL)` (or at minimum `LC_NUMERIC`) immediately before the existing conversion action. Use a disposable copy of a small project and add a non-ASCII path/name with distinct byte encodings under the observed ACP and UTF-8, plus one numeric value whose decimal rendering differs under C versus a comma-decimal locale. Capture exact raw bytes and directory effects for the selected Save output. Keep the host observation and original observation as separate fields.

This single fixture resolves the largest ambiguity in the current wiring: whether the explicit codepage used by `Ctx` matches the target's actual process ACP, and whether `KnownC` is justified for numeric serialization. It also makes the path claim falsifiable without changing the existing byte comparator. Do not edit implementation from the host-only 1252 observation. If the original capture cannot expose `GetACP`/CRT locale before the Save action, record those fields unavailable and keep the corresponding profile assumptions open; do not infer them from OS defaults.

## Suggested execution order after the probe

1. Bind the observed ACP and CRT numeric locale to the exact source paths/formatters using existing decompiles, keeping QString local8Bit, explicit Latin1, `toAscii`, and numeric formatting distinct.
2. Compare raw files and directory effects on the fixture. Report byte-level deltas before any normalization; do not tune the comparator to make the fixture pass.
3. Add focused converter fixtures for the observed profile and one deliberately different profile, verifying that explicit codepage overrides remain deterministic and do not silently mutate the process locale.
4. Keep unrepresentable-name, malformed-surrogate, embedded-NUL, relative-path, and path-length behavior separately qualified until original source/capture evidence binds their exact outcomes.

## Qualification

The cited contracts and receipts are static/decompilation evidence; the listed codec call paths are source-qualified but do not establish whole-project parity. The 1252/437 numbers are from the host, not the original. No new original capture, source edit, build, test, GUI action, or REA database mutation was performed for this audit.
