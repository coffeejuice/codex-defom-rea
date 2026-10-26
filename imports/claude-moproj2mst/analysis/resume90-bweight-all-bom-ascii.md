# CP90 implementation: recognized Unicode BOMs for one-value B_WEIGHT

## Change

Extended the existing exact one-value `B_WEIGHT.DAT` BOM admission to handle the remaining Unicode BOMs recognized by the retained Qt4 `codecForUtfText` helper. The code selects signatures in the PE's precedence order and maps each endian layout to ASCII code units only:

1. UTF-32BE `00 00 FE FF` (MIB `0x3FA`)
2. UTF-32LE `FF FE 00 00` (MIB `0x3FB`)
3. UTF-16BE `FE FF` (MIB `0x3F5`)
4. UTF-16LE `FF FE` (MIB `0x3F6`)
5. UTF-8 `EF BB BF` (MIB `0x6A`)

These are the original helper's signature tests and order in `analysis/resume58-qt-codec-for-utftext-overload.asm` at `0x6714E05D–0x6714E0F7`. UTF-32LE is checked before the overlapping UTF-16LE prefix. The default stream auto-detection and decoder path are captured in `resume58-qt-stream-init.asm` and `resume58-qt-stream-decode.asm`.

For UTF-16/32, the candidate strips the BOM and decodes complete code units only when every decoded scalar is ASCII (`<= 0x7f`); odd/incomplete units and non-ASCII code points remain unsupported. UTF-8 BOM handling continues to strip the BOM and then rejects any non-ASCII payload byte. BOM admission remains limited to `values.size()==1 && path==B_WEIGHT.DAT`; count-zero still returns open success before decoding, while count>1 and other paths retain the old high-byte rejection. No BOM-less decoding, malformed-byte replacement, other numeric files, or output paths were enabled.

## Regression coverage

The focused adapter tests now cover ASCII `1.5\r\n` under UTF-8, UTF-16LE, UTF-16BE, UTF-32LE, and UTF-32BE BOMs, each yielding 1.5 on the one-value B_WEIGHT read. Guard checks include malformed UTF-8 body, odd UTF-16LE payload, non-ASCII UTF-16LE code unit, incomplete UTF-32LE code unit, multi-value B_WEIGHT, other filename, and count-zero no-decode behavior.

The source call qualification is the one-value B_WEIGHT read in `FUN_140ec8110` via `FUN_1410da510(..., 1)` at source line 112; the literal mapping is in `analysis/wave2-save-diffs-300-extra3d-keywords.json`. This is a static contract derived from the SHA-qualified original QtCore image, not an original-process runtime comparison.

## Validation and current hashes

Static preflight only: `git diff --check` reported no diagnostics. No build, CTest, GUI, original-process session, or mutable REA state was used; root owns build/integration validation. This receipt supersedes the prior CP89 statement that UTF-32 BOM inputs remain guarded. The UTF-32LE signature confusion found in review is addressed by matching the original four-byte-before-two-byte precedence.

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `C043D36FF5FE5BF54809C7316DB01B3AA90F1783D6F848D807B468271F4755C7`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `6A4A3741D59E0DD42ABFB7E2A4908B095830FFB974BB88FB99761E3871647FE9`
