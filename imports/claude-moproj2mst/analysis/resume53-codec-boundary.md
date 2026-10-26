# Checkpoint 53: ACP and CString codec boundary

## Concrete distinction

U+20AC EURO SIGN is a useful discriminating input. On the retained Windows host profile, `GetACP()` is 1252 while `GetOEMCP()` is 437. Windows Qt `QString::toLocal8Bit()` uses the ANSI code page, so under ACP 1252 this character becomes byte `0x80`. A null/default Qt CString codec instead follows the retained Cogging contract's Latin-1 `QString`-to-`const char*` behavior; Latin-1 cannot represent U+20AC, so the current `qt::toAscii` helper emits `?` (`0x3f`). OEM 437 is neither of these contracts. It is not a fallback encoding for either path.

## Reachable current paths

The ACP path is implemented by `engine::local8` in `source/apps/moproj2mst/src/engine/loader.cpp`: it calls `WideCharToMultiByte(CP_ACP, ...)`. It is used by current engine operations, including the Cogging load path that copies the generated equipment name into the engine object's fixed byte field (`source/apps/moproj2mst/src/engine/cogging_load.cpp`, `local8(name)` at the `strncpy` call). The nearby operation is reachable when Cogging post-load imports equipment. This is the candidate counterpart for original explicit `toLocal8Bit` use; its bytes depend on process ACP.

The separate CString path is `CoggingFile::CoggingFile(const qt::String& path)` in `source/apps/moproj2mst/src/datafiles/cogging_file.cpp`, which delegates to `qt::toAscii(path)`. The retained [Cogging native-I/O contract](wave3-cogging-native-io-contract.md) qualifies the shipped original Cogging filename path as QString-to-`const char*`, with the default null CString codec using Latin-1. That branch is not supposed to use ACP 1252. A generic conversion of every QString path through `local8` would therefore change this branch (`0x80` versus `0x3f` for U+20AC).

## Smallest test boundary and owner

The narrowest candidate-only test boundary is a focused codec test for the two helpers, owned with their implementations: `engine::local8` in `engine/loader.cpp` and `qt::toAscii` in `qt4/string.cpp`. Give each the same QString containing U+20AC and assert the distinct contracts under the qualified codepage:

- `local8` under ACP 1252: one byte `0x80` (with no OEM conversion).
- `toAscii`: one byte `0x3f` under the default Latin-1 CString profile.

Because `local8` uses the process ACP, a host-specific test must first record `GetACP()` and must not assume ACP 1252 on other machines. A deterministic cross-host test needs an injectable codepage/codec seam; that would be a code change and is outside this audit. OEM 437 should be recorded as host context only, not as the expected Qt conversion. For an original runtime parity claim, the existing evidence must bind a particular original call to one of these conversions; this note does not upgrade static evidence into an observed runtime result.

## Qualification

This is a concrete **codec-contract distinction**, not a claim that the current candidate is wrong: the candidate currently has separate ACP and Latin-1 helpers, corresponding to the two qualified original idioms. The actionable risk is accidental consolidation or a test that uses OEMCP in place of ACP. No source, test, build, GUI, fixture, or REA state was changed for this audit.
