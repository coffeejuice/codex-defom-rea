# CP119 bounded `SPRING2.INI` reader implementation

## Change

The native spring reader now admits a present, readable, ASCII `SPRING2.INI` alongside its existing bounded `SPRING.INI` profile. It creates `count` zero flags first, parses up to `count` canonical signed decimal int32 tokens, maps them to row flags in order, and leaves any EOF-short suffix at zero. It stops after the declared row count and ignores later tokens. Missing or unopenable companions still produce all-zero flags. The main record parser, count ceiling, field order, and ASCII-only codec boundary are unchanged.

Malformed integer spellings, radix forms, overflow, non-ASCII content and BOMs remain guarded. For `SPRING2.INI`, the reader explicitly rejects a UTF-8 BOM before the shared ASCII decoder can apply any other file profile's BOM handling. Companion parse failure leaves the public output empty. The read path performs no write or delete operation.

The focused test adds complete two-flag input, EOF-short input, a present empty file, trailing extra tokens, malformed/radix/overflow/non-ASCII/UTF-8-BOM refusals, and byte-for-byte checks that neither `SPRING.INI` nor `SPRING2.INI` changes during reading. Existing absent-companion tests continue to cover zero flags.

## Static preflight and limits

`git diff --check` passed for the two owned files. Static inspection confirms `SPRING2.INI` is read after the main count and before record rows, and the candidate uses a temporary zero-initialized flag vector before publishing records. No write, delete, page, header, public Session, or GUI code changed. No build or tests were run; root owns integration validation.

The CP118 receipt's statement that a present companion remains refused is superseded by this bounded extension. Its implementation-only changes do not establish malformed QTextStream cursor/status behavior or actual process default-codec handling.

## Frozen file hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `8776E3E6C5F36D85CB71661370584FF1D0CDD435E04366B624D8AD626927F2E8` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `63489658B782F037B8495C2651284558E9779045076ED0E8243C7006D5BB47EC` |

The exact source and candidate contract are documented in `analysis/resume119-spring2-normal-path.md`. This receipt supersedes its pre-BOM-repair hashes. No original-process execution was performed.
