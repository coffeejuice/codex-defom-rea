# Resume 58: bounded PPV file-line reader

## Increment

Added an isolated `readPpvAsciiLines` adapter in
`source/apps/moproj2mst/include/m2m/engine/simctrl_ppv_file_reader.hpp` and
`src/engine/simctrl_ppv_file_reader.cpp`. It obtains bytes through the existing
project file-byte seam, reports open failure separately, and returns lines only
for an explicitly bounded ASCII/LF profile. It does not call the PPV parser or
public `loadSimCtrl`.

Added `tests/mdt/test_simctrl_ppv_file_reader.cpp`. It checks overlay-backed
ASCII LF splitting, absent-file status, and fail-closed handling of a UTF-8 BOM
and CRLF. The test uses Testkit `REQUIRE`/`CHECK` macros; no C `assert` is used.
The source tree CMake discovers `src` and `tests/<packet>/test_*.cpp` by glob,
so these new files require no CMake edit; this has not been build-verified here.

## Qualified and guarded behavior

The original reader decompile preserved in
`analysis/resume48-ppv-reader-receipt.txt` shows `QFile::open` with mode value
1 (`ReadOnly`), construction of a default `QTextStream` on that QFile, and
successive `readLine()` calls. This qualifies the access mode and line-reading
API. It does not settle the default codec's BOM detection for this target Qt
build, raw behavior for non-ASCII bytes, or whether CR is removed from CRLF
lines.

Accordingly, this adapter accepts only bytes below 0x80, LF as the line
separator, and ASCII horizontal tab among control bytes. It rejects BOMs,
other non-ASCII bytes, NUL/control bytes, and any CR. It does not silently
reuse `readTextLines`, whose source implementation rejects BOMs but otherwise
strips CR and CP_ACP-decodes. `UnsupportedEncodingOrLineEnding` denotes a
successful byte read with an input outside this bounded profile; it must not be
mistaken for native QFile-open failure.

The implementation uses `datafiles::readFileBytes`, which first consults the
project overlay and then reads disk bytes. This supplies the converter's
existing project-file abstraction and a deterministic unit fixture; it is not
a literal QFile handle. `OpenFailed` is the adapter's missing/unreadable-byte
result. No claim is made that all native open errors map identically.

## Validation status and next action

No build or tests were run, per task scope. The parent integration owner should
run the focused test and compile checks. Keep BOM and CRLF rejected until
Qt-version-specific behavior is established; do not enable this adapter in the
public loader. A later source-qualified expansion can add UTF-16 BOM decoding
or CRLF behavior with positive regression cases rather than weakening these
guards by assumption.
