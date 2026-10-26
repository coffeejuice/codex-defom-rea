# Checkpoint 59: bounded PPV UTF-8 BOM reader support

## Change

The isolated PPV file adapter now recognizes the three-byte UTF-8 BOM `EF BB BF`. For a UTF-8 BOM file, it removes the marker and accepts the ASCII subset of the UTF-8 payload, then uses the existing line splitter. It rejects every payload byte at or above `0x80`, so this increment does not claim UTF-8 multibyte decoding. BOM-less high bytes remain rejected because the original default stream uses `codecForLocale()` and its original process code page is not known. The existing UTF-16LE BOM, BOM-less ASCII, LF/CRLF and embedded-CR guard remain as before.

The production edit is limited to `source/apps/moproj2mst/include/m2m/engine/simctrl_ppv_file_reader.hpp` and `source/apps/moproj2mst/src/engine/simctrl_ppv_file_reader.cpp`. The focused test edit is limited to `source/apps/moproj2mst/tests/mdt/test_simctrl_ppv_file_reader.cpp`.

## Evidence boundary

`analysis/resume59-ppv-codec-next.md` and its referenced bounded captures qualify the original default stream's BOM dispatch: the constructor initializes its current codec with `QTextCodec::codecForLocale()`, the BOM helper selects UTF-8 MIB `106` for `EF BB BF`, and no matching BOM returns the current locale codec. Thus the implementation matches the source-qualified UTF-8-BOM codec-selection branch for its ASCII payload subset. It intentionally does not infer a code page for BOM-less high bytes.

The retained capture directly qualifies BOM-to-codec selection. It does not separately identify the UTF-8 codec's treatment of arbitrary malformed or multibyte payloads. This implementation rejects both malformed and valid non-ASCII UTF-8 for now. It strips the three-byte marker before decoding; the `EF BB BF` detection and selection are binary-qualified, while exact internal handling of that marker by Qt's chosen codec is not independently claimed in this small increment.

## Test coverage added

The existing focused test now checks:

- UTF-8 BOM followed by ASCII PPV lines is accepted, the BOM does not become part of the first parsed line, and LF line splitting is preserved.
- UTF-8 BOM followed by valid non-ASCII UTF-8 remains guarded.
- UTF-8 BOM followed by an invalid UTF-8 sequence remains guarded.
- BOM-less non-ASCII remains guarded.
- Existing UTF-16 malformed and embedded-CR guards remain present.

No build or test executable was run; root owns the build. This report does not claim runtime equivalence beyond the qualified byte subset.

## Freeze hashes

SHA-256 after these edits:

- `source/apps/moproj2mst/include/m2m/engine/simctrl_ppv_file_reader.hpp`: `6A1E10D28F0DF65B771B95F65506712AD892FD24A4F7C1E31A9C6FB3AD3EEA89`
- `source/apps/moproj2mst/src/engine/simctrl_ppv_file_reader.cpp`: `0A35712EADE42C839AB71FC3363A332A3A852F9A0904461EE27078D62E64EC9E`
- `source/apps/moproj2mst/tests/mdt/test_simctrl_ppv_file_reader.cpp`: `99E64A51D6D03B19ED3EC387263E6969336578B1B60B047BECC64A477421B276`
