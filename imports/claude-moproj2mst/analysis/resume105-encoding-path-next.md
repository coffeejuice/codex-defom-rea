# CP105 encoding/path audit

## Finding

No new path-encoding behavior can be declared wrong from the retained evidence. The strongest remaining public candidate is the native HT property-file Save path, where the original and candidate select different codec abstractions but the original process ACP has not been observed.

## Source-qualified path

- Original `FUN_1411b3d87` is identified at `source/apps/moproj2mst/src/engine/ht_save.cpp:114`. Its source comment records `QString::toLocal8Bit()` before the CRT file writer. This is the property-file path reached through the native Save option (`ctx->nativePropertyFiles`). The source also distinguishes this from Cogging's `QString` const-char codec.
- Candidate `writeText()` at `ht_save.cpp:109-118` calls `qt::encodeLocale(target, ctx->codepage)` and opens via `CoggingFile`; the non-native recording path uses a wide scratch filename and records target bytes. Thus the reachable native branch depends on configured `Ctx::codepage`, while the original is governed by the target process's Qt local codec / ACP.
- `engine_internal.hpp:47-48` independently defines the engine's `local8()` as `QString::toLocal8Bit()` (system ANSI code page). `convert.cpp:170,268` shows `codepage` is carried into `Ctx` and native path handling.

This is an unresolved equivalence question, not proof that the current byte output differs: if the configured codepage matches the original process ACP, the codecs may agree for the tested path characters. The host observation of ACP 1252 / OEM 437 is not a measurement of the original process. Do not generalize this to QFile paths or Latin-1 consumers.

## Smallest useful next evidence

At the next already-authorized original Save action that exercises a native HT property file, record the process's `GetACP()` and the exact bytes passed from `toLocal8Bit()` to the CRT open for one non-ASCII directory component. Tie the receipt to the original executable hash and the exact property-file writer/caller. Compare those bytes with candidate `encodeLocale(target, ctx->codepage)` under the same codepage. If no suitable non-ASCII Save input is already in scope, the missing dependency is an original-process ACP observation plus one path conversion; a fixture-only test cannot establish it.

## Scope and blockers

- No implementation change is justified yet: neither the original ACP nor a byte-level output from the original process is retained.
- This does not reopen the completed SW2SP, STRETCH, B_WEIGHT, NBC, or bounded SPRING reader cases.
- The broader RFORMAT validation item still lacks original-process locale/encoding/path fixture comparison. Static codec selection is known; runtime ACP and resulting path bytes remain open.
