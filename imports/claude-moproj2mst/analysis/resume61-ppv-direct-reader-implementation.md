# Resume 61: PPV direct physical-file reader

## Increment

Added `readPpvFileLinesFromDisk(path)` to the existing PPV file reader API. It
opens the supplied Unicode path through the project's long-path-aware binary
file path, reads physical bytes, then passes them through the exact same bounded
decoder as the existing `readPpvFileLines(project, path)` overlay-first entry.
The existing overlay-based API remains unchanged. The new physical entry has
no `Project` parameter and therefore cannot consult `Project::overlay`.

The new test creates a disposable file under `M2M_TEST_SCRATCH` and stores
different line payloads for that same cleaned path on disk and in a `Project`
overlay. It asserts that the direct reader returns the disk row while the
existing reader continues to return the overlay row, and that the overlay is
unchanged. Assertions use active Testkit `REQUIRE`/`CHECK` macros; no C
`assert` is used.

## Limits

Both entry points share the same accepted profile and guards: BOM-free ASCII,
UTF-8 BOM with ASCII-only payload, well-formed UTF-16LE with BOM, and LF/CRLF;
locale-dependent BOM-less non-ASCII, non-ASCII UTF-8, malformed UTF-16,
unsupported BOMs, and embedded CR remain guarded. The original PPV source
qualifications are in `analysis/resume58-ppv-qt-codec.md` and
`analysis/resume59-ppv-codec-next.md`.

This is a physical byte-source seam, not a literal QFile/QTextStream object.
It buffers the full file before decoding/parsing. It does not model a post-open
read error separately from the bytes that were returned, or the native
reader's incremental line timing on a read fault. No claim is made about those
failure cases. The entry is not wired into `loadSimCtrl`; no public Open/Save,
CMake, or shared interface was changed.

## Validation and freeze

No build or tests were run; root owns integration validation. CMake's existing
test/source globs are expected to discover the modified/new files after
configure. `git diff --check` passed at freeze.

SHA-256 at freeze:

| File | SHA-256 |
|---|---|
| `simctrl_ppv_file_reader.hpp` | `88151c2b6dfa441c98c1d40c8935fc2a78e559240e3cbfc16f3c79828e403348` |
| `simctrl_ppv_file_reader.cpp` | `0cc868d1ce633b931e5beb0568ed496d49cb274edfae66ee7cfb34e7b743aadf` |
| `test_simctrl_ppv_file_reader.cpp` | `3ed52c7135140a386bc1b8bdb188555233506a5f257ef1a8a21fd989041e663` |
