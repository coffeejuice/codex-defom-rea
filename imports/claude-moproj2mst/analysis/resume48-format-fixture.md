# Checkpoint 48: fixture audit for encoding and numeric output

## Finding: no existing single fixture qualifies

The checked-in/copied `.moproj` files under `imports/claude-moproj2mst` contain no non-ASCII bytes (read-only scan of project files, excluding the optimization snapshot). The current regression suite has isolated codec coverage and isolated numeric stream coverage, but I found no existing copied fixture that combines a non-ASCII project/name with a locale-sensitive numeric field. Do not report a bundled parity fixture that does not exist.

## Closest existing paired evidence

1. **Copied project / numeric writer path:** `source/apps/moproj2mst/tests/moproj/test_moproj_write.cpp` includes case `B:109_B_FE7_before`, based on copied `Gear_blank.moproj`, with output `109_B_FE7`. The case is explicitly labeled “FormingExpress 7 digits (L4)” and is in the historical Oracle-B raw project-file comparison. `source/apps/moproj2mst/tests/s/test_save_current.cpp` also lists `109_B_FE7` as FormingExpress. The source-qualified writer `source/apps/moproj2mst/src/moproj/write.cpp` emits project strings such as `Project_Title.Name` and values via `QString::number(v, 'g', 6)` in its geometry/property serialization paths. The corresponding native Save writes the project `.moproj` file. This is a good existing numeric/round-trip anchor but its fixture names are ASCII.
2. **Codec behavior anchor:** `source/apps/moproj2mst/tests/p04/test_strings.cpp`, test `to_ascii_and_codec`, explicitly checks CP1252 bytes for `é`/`€`, replacement `?` for unmappable `中`, and a CP1251 byte for Cyrillic `А`. Its `stream_basic` test separately checks title plus numbers `2.5`, `-0.0`, and `1e-5`; the expected bytes are `*!TITLE  x\n*!UNIT   1\n\t2.5\t0\t1e-05\n\n`. This proves the converter's codec/stream unit behavior for the supplied profile, not original process encoding or a real project Save.

## Current path binding

- Project XML path: the Save implementation reaches `moproj::saveProjectFileNative` (`source/apps/moproj2mst/src/moproj/write.cpp`); the project writer's string serialization is Unicode/XML first, while encoded output bytes are produced by its native XML output path. Compare the complete resulting `.moproj` byte stream, including XML declaration/encoding and every `Name`/title field. Do not conflate this XML serializer with narrow CRT paths used by MDT/KEY data files.
- MST stream path: `source/apps/moproj2mst/src/mst/stream.cpp` converts buffered UTF-16 through `qt::encodeLocale(..., codepage_)`; `test_strings.cpp` fixes expected output only for explicit codepage 1252. Exact raw comparison must include the title/name bytes and numeric tokens, line endings, BOM absence, and trailing LF behavior.
- Current process path: ABI supplies a caller-configured codepage to `Ctx`; the unit fixture's 1252 value is explicit and does not prove the original process used ACP 1252. The original Qt path mechanism is `QString::toLocal8Bit` through `CP_ACP` unless Qt startup selects a locale codec. Therefore a raw project XML mismatch on non-ASCII text cannot be attributed to converter logic until the original process ACP/codec is known.
- Numeric qualification: the MST `QTextStream` closure is documented as using `QLocale::c()` defaults in `source/docs/re/moproj2mst/s1-02/spec.md`, giving `.` and no grouping. `QString::number(v, 'g', 6)` is a separate format path. These Qt paths do not establish CRT `LC_NUMERIC` for `%g`/`fscanf` code in other writers.

## Exact raw comparison proposal

Use the existing copied `Gear_blank.moproj` / `B:109_B_FE7_before` as the base. In a disposable copy only, set one project or operation display name to `Café-中` and retain a known finite numeric property from the existing FE7 case (record its exact input XML node/path and value before Save). Run candidate Save with explicit codepage 1252 and save raw output as `candidate.moproj`; do not alter the baseline copy or existing Oracle-B fixture. For the eventual original comparison, create the same disposable copy in the original application's test directory and Save once under the observed process profile.

Compare full bytes first (length, SHA-256, first differing offset and escaped/hex windows). Then separately report: (a) bytes in the changed name field, (b) exact numeric lexical token, and (c) unrelated byte deltas. Do not normalize encoding, decimals, XML whitespace, or line endings before recording the raw result. Preserve the input and output hashes for both implementations.

## What must still be observed in the original process

- `GetACP()` at the target process, and Qt's live locale codec/startup language state if the process may have a non-default codec. Host ACP 1252/OEM 437 and the absence of manifest `activeCodePage` do not substitute for this.
- For any CRT numeric formatter exercised by the selected Save path, `setlocale(LC_NUMERIC, NULL)` at the relevant stage. The statically recovered QTextStream C-locale contract applies only to that writer closure.
- The original's exact non-ASCII result (CP1252 byte, another codec byte sequence, or `?`) and raw numeric token from the same disposable fixture. Only this paired capture can turn the proposed combined case into original-bound validation.

The combined disposable case is a proposal, not an existing fixture and not an acceptance result. No fixture, source, build output, app-control state, or REA database was modified for this audit.
