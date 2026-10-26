# CP89 correction: distinguish UTF-32LE before UTF-16LE

Independent review found that the initial CP89 implementation treated any `FF FE` prefix as UTF-16LE. The retained Qt BOM helper checks the four-byte UTF-32LE marker `FF FE 00 00` before its two-byte UTF-16LE check (`analysis/resume58-qt-codec-for-utftext-overload.asm`, `0x6714E083–0x6714E09F` selects MIB `0x3FB`; UTF-16LE follows at `0x6714E0C6–0x6714E0D6`). The code now explicitly rejects `FF FE 00 00` before entering the UTF-16LE decoder, matching the planned decoder boundary.

Added an exact UTF-32LE encoding of `1.5\r\n` as a negative test under the one-value B_WEIGHT gate. The test requires it to remain unsupported. Existing UTF-16LE ASCII, UTF-8 BOM ASCII, odd-code-unit, non-ASCII code-unit, path/count, and zero-count behavior remains unchanged.

Static preflight only: `git diff --check` reported no diagnostics. No build or tests were run. This correction supersedes the CP89 source/test hashes in `resume89-bweight-utf16le-implementation.md`.

Current SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `9EE38E9D92DB970A6095C185969757585F09E44356C9B447343534252746B13C`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `F5D82005CE84C663FC08086E77E571EA564B204FD33143278E205DDBB21EAD4D`
