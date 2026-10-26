# Checkpoint 51: Extra3D QFile path contract and bounded change proposal

## Original path handling

The retained original scalar readers `0x1410da510` (double) and `0x1410da690` (integer) construct a `QFile` with a `QString` argument, call `QFile::exists`, and call `QFile::open(ReadOnly)` before making a `QTextStream` over that file. The writer `0x1410da9e0` likewise constructs `QFile(QString)` and opens `WriteOnly`. The enclosing writer `0x1410db030` builds content as QString fragments, including numeric `QString::number(..., 'g', 6)`, and passes fixed ASCII basenames. These call paths are retained in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`; the bounded reader/content contract is `analysis/wave2-save-diffs-300-native-reader-contract.json`.

This statically establishes that the original call sites pass Unicode QString paths into Qt QFile APIs. The source does not show an intermediate `QString::toLocal8Bit` conversion for these read/write calls. It does **not** prove a successful open under every Unicode path, filesystem namespace, Qt version, or operating system. The evidence set contains no original runtime test under a non-ASCII directory.

## Candidate behavior and guard scope

`NativeExtra3DIo` currently calls `ordinaryNarrowPath(directory)` in its constructor, before any QFile-equivalent `openRead` or `openWrite`. The helper rejects any code unit >= 128 and paths outside ordinary `MAX_PATH`; `native_extra3d_directory_and_delete_path_profiles_are_explicit` locks that refusal for a directory ending in U+4E2D. This makes both read and write branches unreachable for such a path even though candidate `openRead` resolves a QString path and uses `StudyIo`, and `openWrite` resolves a QString path and uses `mst::FileDevice`/`CreateFileW`.

Deletion is different. `removeIfReadable` intentionally calls `ordinaryNarrowPath(resolve(path))`, then `fopen("rt")`, `fclose`, and `remove`. The source annotation ties the original cleanup call to `0x1407fbc10`, which uses narrow literal names; full absolute-Unicode-path parity for that cleanup has not been established. Removing a single constructor-wide guard without retaining a guard at this narrower site would silently expand an unqualified branch.

The project input path remains Unicode through the wide CLI API. Sidecar filename encoding is separate from sidecar content decoding: original Qt QTextStream reads values and constructs output text; current native input is explicitly ASCII/KnownC bounded, and the original whole Qt BOM/Unicode decoder and non-C locale behavior remain open. Do not infer a codec from the path type.

## Minimal source change to consider after review

Do not delete the constructor guard solely from the static QFile signature. If the team elects to extend the qualified profile, the smallest structural change is to separate checks by actual consumer:

1. Keep absolute-path validation in the constructor. Do not relax MAX_PATH or device/extended namespace rules in this change; their original outcomes are not established here.
2. Remove only the blanket ASCII rejection from the QFile-backed constructor/read/write admission path, while preserving the existing ordinary-narrow guard in `removeIfReadable` until its original path/CWD contract is qualified.
3. Keep current ASCII/KnownC validation for sidecar **contents** unchanged. A Unicode directory test must not be interpreted as Unicode content-decoder support.
4. Keep `openWrite` on the current wide `FileDevice` and `openRead` on the current path-aware StudyIo implementation. Do not substitute ACP conversion for a QFile path.

This is a reviewable implementation sketch, not an instruction to make the source change now. Before landing it, verify `StudyIo::openReadFile` indeed accepts the resolved UTF-16 path through its Windows-wide backend, and that path effects still match its existing share/access flags.

## Focused candidate-only fixture (no original-app control)

The existing `test_simcontrol300_native_io.cpp` already creates a disposable `Rig` and directly tests `makeNativeExtra3DIo`. Extend that same isolated test seam (if source/test ownership permits) to:

- create a short temp directory whose final component contains U+00E9 (`Café`) or U+4E2D, with an ASCII-named scalar file such as `NBC.DAT` containing the already qualified ASCII scalar grammar;
- construct `makeNativeExtra3DIo(qt::String(unicodeRoot.u16string()), 1252, ...)`, then read the scalar file and verify its numeric value;
- write a separate fixed ASCII sidecar name through `openWrite`, close it, and verify raw bytes and the real file path under the Unicode directory;
- assert readable-delete remains explicitly unsupported for the same non-ASCII resolved path (or preserves its existing narrow behavior), and assert missing/readability outcomes do not create parents.

This verifies the candidate's Windows-wide read/write path after the proposed guard separation. It does not establish original parity; only a future original-first capture can show the original runtime opens the same Unicode directory. The fixture can run entirely in the candidate test harness and requires no original app, no new GUI-control session, and no mutable REA database.

## Qualification / next step

Current status is a conservative candidate refusal against an original source path type that appears Unicode-capable. It is an evidence-backed discrepancy candidate, not a proven behavioral mismatch. The next safe implementation review is the consumer split above, paired with the candidate-only test; retain the public guard until that source/test patch is reviewed and then keep original parity explicitly open. No source, build, fixture, GUI, or mutable REA state was changed for this contract.
