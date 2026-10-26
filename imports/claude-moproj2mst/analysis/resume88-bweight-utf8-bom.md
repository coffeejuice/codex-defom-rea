# CP88: reachable `B_WEIGHT.DAT` UTF-8 BOM decoding gap

## Exact source-qualified input

The reachable Extra3D Open fill reads one double from `B_WEIGHT.DAT`. In `analysis/wave2-save-diffs-300-support-140ec8110.c`, the `FUN_140ec8110` call at line 112 invokes `FUN_1410da510(..., 1)` with the QString built from `UNK_142b14eb8`; `analysis/wave2-save-diffs-300-extra3d-keywords.json` maps that address to `B_WEIGHT.DAT`. On successful read, the one double is formatted into the weight control at lines 120–123. Candidate `fillExtra3D` has the matching `std::array<double, 1> weight` and `io.readDoubles(qt::L("B_WEIGHT.DAT"), weight)` in `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`.

The original `FUN_1410da510` decompile constructs `QTextStream(QIODevice*)` without setting a codec and extracts a double through the QTextStream double operator. The retained QtCore constructor/decoder captures establish that this default stream starts with Unicode auto-detection enabled and passes its initial byte buffer through `QTextCodec::codecForUtfText`. The helper selects UTF-8 codec MIB `0x6A` for `EF BB BF` and consumes that BOM before subsequent characters are decoded. These shared Qt behavior facts are documented with exact call paths and ranges in `resume58-ppv-qt-codec.md` and `resume59-ppv-codec-next.md`. The QTextStream real getter then consumes decoded characters, and the stream's default numeric locale is C as qualified in `resume67-sw2sp-locale.md` and constructor captures.

For exact bytes `EF BB BF 31 2E 35 0D 0A` (UTF-8 BOM followed by ASCII `1.5\r\n`), the original stream therefore reads the ASCII numeric payload as 1.5. This does not depend on the original process ACP: the recognized BOM explicitly selects UTF-8, and all numeric body bytes are ASCII. The original reader requested one value and returns success after QFile opens.

The current candidate path reaches `NativeExtra3DIo::readDoubles` for `B_WEIGHT.DAT`, then `asciiBytes`; `asciiBytes` throws `M2M_E_UNSUPPORTED` on any byte >= 0x80. The BOM bytes therefore cause a concrete refusal before `readReal` can parse the payload. This is an actual reachable Open-path gap and a useful single-file codec increment after the integer-sidecar work.

## Smallest test and implementation boundary

Use the existing candidate-only `Rig` in `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`:

1. Write the eight exact bytes `EF BB BF 31 2E 35 0D 0A` to `B_WEIGHT.DAT` in binary mode.
2. Call `readDoubles(qt::L("B_WEIGHT.DAT"), std::array<double,1>)`.
3. Assert open/read success and `value[0] == 1.5`.
4. Retain a nearby negative with malformed BOM payload or BOM-less non-ASCII bytes guarded; do not infer malformed UTF-8 conversion.

Keep any first implementation tightly scoped to the identified BOM plus ASCII payload. A shared private decoder may remove only a recognized UTF-8 BOM before passing the remaining validated ASCII bytes to the existing real parser, but route admission should remain explicit for the reached `B_WEIGHT.DAT` count-one read unless the other QTS numeric callers are separately reviewed. Do not enable BOM-less UTF-8/ANSI decoding, malformed/truncated byte handling, other BOM encodings, or host-codepage assumptions. This contract affects the B_WEIGHT scalar before its existing finite-value check; no Save writer behavior changes follow from it.

## Qualification limits

This result combines the source-qualified GUI-MO caller with shared QtCore default stream codec behavior. The Qt codec captures were first made for a PPV `readLine` path; however, BOM detection resides in the common default QTextStream constructor/initial byte decoder used by `FUN_1410da510`, which also constructs a default stream over QFile. The active decoded characters reach `getReal` through the same QTextStream. No original process or mutable fixture was run. Only a valid UTF-8 BOM followed by ASCII numeric bytes is claimed; whether malformed UTF-8 is replaced, omitted or reported remains unknown.

No source, test, build, GUI or REA state was changed for CP88.
