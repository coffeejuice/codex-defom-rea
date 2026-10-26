# CP157 SPRING2 BOM auto-detection successor

## Recommendation

The next independent SPRING2 reader increment is Unicode BOM auto-detection for an otherwise canonical ASCII flag stream. This advances the codec boundary rather than adding another count-one numeric spelling. Extend only the existing positive-count, bounded SPRING profile: a present readable `SPRING2.INI` may begin with one recognized UTF-8, UTF-16LE/BE, or UTF-32LE/BE BOM, after which every decoded code unit must be ASCII and the payload must remain within the existing complete canonical signed-decimal flag/EOF-short profile. Keep BOM-less high bytes, malformed/truncated Unicode payloads, non-ASCII scalars, and malformed numeric tokens guarded.

The decoder can share the already implemented ASCII-unit BOM conversion used for one-value `B_WEIGHT.DAT`, but SPRING2 must call it through an explicit, format-local admission so the B_WEIGHT path/count gate does not change. Do not claim arbitrary Unicode conversion or ACP behavior.

## Original source evidence

Original application identity: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Qt identity: `QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/resume58-ppv-qt-codec.md` and its retained captures establish the default `QTextStream(QIODevice*)` path: private stream initialization enables Unicode autodetection, the first byte-buffer decode calls `QTextCodec::codecForUtfText`, and the returned codec replaces the current locale codec before decoding. This is the same Qt constructor used by the SPRING2 helper; the stream is constructed after a QFile read-only open and no explicit codec or `setAutoDetectUnicode(false)` call appears in the retained `analysis/wave2-save-diffs-300-support-1410da690.c` path.

`analysis/resume58-qt-codec-for-utftext-overload.asm` records the helper's exact BOM dispatch and precedence at `0x6714E05D–0x6714E0F7`: UTF-32BE (`00 00 FE FF`), UTF-32LE (`FF FE 00 00`), UTF-16BE (`FE FF`), UTF-16LE (`FF FE`), then UTF-8 (`EF BB BF`). The same mapping is independently exercised in the bounded B_WEIGHT implementation receipt `analysis/resume90-bweight-all-bom-ascii.md`. For this proposed ASCII subset, decoded code units after the BOM are ASCII numeric characters/whitespace, so the existing C-locale integer extraction contract applies without requiring ACP knowledge.

The SPRING2 caller and loop are qualified in `analysis/resume119-spring2-normal-path.md`: `FUN_1410da690` opens a QFile, constructs a default QTextStream, extracts at most the main row count into a pre-zeroed flag vector, ignores conversion status, and returns true after successful open. Thus EOF-short flags retain zeroes. The retained Save writer in `analysis/resume120-spring2-writer-order.md` serializes nonzero flags as decimal and removes a readable companion when all flags are zero. These observations establish that an admitted nonzero decoded flag reaches ordinary Save output. This is static PE composition, not original-process runtime comparison.

## Candidate gap and bounded contract

The current `readSprings` path calls `asciiBytes(*companion, false, true)` at `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp:619`; the final argument explicitly rejects UTF-8 BOM, while the helper only decodes BOMs under its B_WEIGHT-specific flag. The reader therefore refuses a class that the original default stream detects before integer parsing.

Source-ready slice:

- Main file remains a supported positive-count `SPRING.INI` input (candidate count cap 256 remains a candidate resource policy).
- Companion must be present/readable and have one exact recognized BOM from the five-entry list above.
- After decoding, admit only ASCII bytes and the existing canonical signed decimal int32 tokens separated by Qt whitespace; complete values map in row order, and EOF-short suffix remains zero as in the existing companion reader.
- Reject a BOM with an incomplete code unit, non-ASCII decoded code point, malformed numeric token, overflow outside existing integer behavior, additional BOM, and any high byte without an initial recognized BOM. Do not broaden malformed-stream cursor/status, signed/radix/overflow profiles, or path/count gates.
- Preserve both source files byte-for-byte during read. Do not strip or rewrite the input on disk.

Focused tests should use a two-row primary fixture and distinct companion flags `1 0` encoded as each of UTF-8 BOM + ASCII, UTF-16LE/BE ASCII code units, and UTF-32LE/BE ASCII code units. Assert flags `[1,0]`, byte-for-byte immutability of both input files, and that the existing writer normalizes the nonzero companion value to ASCII `1` (when the public page/writer test is available). Keep count-zero, BOM-less UTF-8/ACP, malformed BOM payload, non-ASCII Unicode, and malformed flag fixtures as negative controls. Include UTF-32LE before UTF-16LE prefix handling.

## Limits

The Qt binary proves BOM selection and codec dispatch, and the retained SPRING2 decompile proves default-stream construction and extraction order. The ASCII code-unit conversion is directly supported by the candidate's already reviewed bounded B_WEIGHT reader work. This does not establish malformed decoder replacement, truncated-unit status, BOM-less ACP results, non-ASCII flag characters, or original runtime behavior. Those remain guarded.

No source, tests, build, GUI, or mutable REA state changed in this CP157 evidence pass.
