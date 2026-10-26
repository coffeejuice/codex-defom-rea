# Checkpoint 54: Extra3D integer base-detection gap

## Concrete reachable input

The original Extra3D initial fill reads `SW2SP.DAT` as one integer. Retained `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` binds the original integer reader at `0x1410da690`: it constructs `QFile(QString)`, checks `exists`, opens ReadOnly, constructs `QTextStream`, and extracts into each integer destination using `QTextStream::operator>>(int)`. The retained `analysis/wave2-save-diffs-300-native-reader-contract.json` qualifies the default integer base as 0 (automatic base detection), including radix prefixes. The Extra3D fill decompile at `0x140ec8110` and current `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp` bind this reader to `SW2SP.DAT`, with count 1. This read is part of the initial Extra3D page fill in the Open-to-Save lifecycle; it is not the later write/delete path.

Use the one-token ASCII file contents `0x2\n` as a discriminating input. Under the retained Qt default-base contract, the extraction yields integer 2. The current candidate's `NativeExtra3DIo::readInts` in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` calls `readDecimalInt`; that helper scans only an optional sign and decimal digits, then rejects `x` as a non-whitespace delimiter with `M2M_E_UNSUPPORTED`. This is a concrete candidate refusal where the original static conversion contract accepts the value.

## Smallest independent test seam

Use the existing candidate-only `makeNativeExtra3DIo` seam from `tests/mdt/test_simcontrol300_native_io.cpp` (or a separate focused test alongside it): create a short disposable directory, write `SW2SP.DAT` with exact bytes `0x2\r\n`, construct the native adapter on that absolute directory, and call `readInts(qt::L("SW2SP.DAT"), oneIntSpan)`. The reference assertion for the original contract is `value == 2`; the current candidate branch should be captured as an unsupported-format error. To keep the seam narrow, do not call the UI/lifecycle layer, apply the page, or mutate any project/REA data.

The evidence-backed fix boundary, if authorized by integration, is `readDecimalInt` / `NativeExtra3DIo::readInts` only: implement the original QTextStream integer base-0 lexical conversion and preserve the already qualified whitespace, failure/status, and array-initialization behavior. Do not infer the entire Qt integer parser from the one hexadecimal token; octal, binary-prefix support, signs, overflow, partial extraction, and invalid-token status need their own source-bound cases before broadening the guard.

## Qualification and limits

The original result is based on retained static decompilation and Qt operator qualification, not a fresh original runtime capture. This note does not claim a numeric locale effect: the discriminating token is ASCII hexadecimal syntax, and base detection is the relevant distinction. Existing candidate `KnownC` input and decimal fixtures remain useful for their stated profile, but they do not cover this radix branch. No source, tests, build, GUI, or mutable REA state was changed for this audit.
