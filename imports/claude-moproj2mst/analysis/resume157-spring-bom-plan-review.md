# CP157 SPRING2 BOM plan review

## Conclusion

The proposed ASCII-only BOM extension is source-ready with the stated positive-count/256-row candidate gate and malformed/non-ASCII exclusions. The retained Qt evidence establishes the five BOM signatures and their required precedence, and the original SPRING2 path constructs a default QTextStream with Unicode autodetection. No source change or runtime comparison was performed in this review.

## Decoder contract and precedence

The retained `codecForUtfText` bytes dispatch in this exact order: UTF-32BE `00 00 FE FF`, UTF-32LE `FF FE 00 00`, UTF-16BE `FE FF`, UTF-16LE `FF FE`, then UTF-8 `EF BB BF`. UTF-32LE must be checked before its UTF-16LE prefix. The current `asciiBytes` helper already follows that order in its B_WEIGHT-only BOM branch and decodes UTF-16/32 code units according to endianness, rejecting incomplete units and values above ASCII; UTF-8 strips its BOM then the common high-byte gate rejects non-ASCII payload bytes. A SPRING2 caller can reuse that existing constrained conversion through a format-local opted-in call, while preserving the BOM-less high-byte rejection and making no ACP or arbitrary-Unicode claim.

The byte decoder must recognize signatures before decoding payload, and must keep these boundaries:

- `FF FE 00 00` selects UTF-32LE rather than UTF-16LE.
- A partial UTF-32LE prefix such as `FF FE 00` falls through the Qt two-byte UTF-16LE signature but leaves an incomplete code unit; the candidate should reject it rather than claim malformed-input parity.
- Partial/unrecognized UTF-8/UTF-32 signatures remain high-byte inputs without a recognized BOM and reject.
- A recognized BOM with truncated UTF-16/32 payload rejects on incomplete code-unit length; non-ASCII code units reject. A second BOM in the decoded payload is non-ASCII and rejects.
- BOM-only streams are a valid empty decoded stream; under the source-qualified caller they leave pre-zeroed flags unchanged.

This keeps endianness and truncation behavior explicit. The original Qt decoder may have replacement/status behavior on malformed input, which the proposal correctly leaves unclaimed.

## SPRING2 caller compatibility

The retained helper opens `SPRING2.INI`, constructs a default QTextStream, extracts while `!atEnd()` and `i < count`, and leaves the pre-zeroed suffix untouched on EOF. Consequently a decoded ASCII payload `1 0` maps row-order flags `[1,0]`; a shorter canonical stream retains zeros at its tail. The existing writer serializes nonzero flags in decimal, so a nonzero flag originating from a BOM-encoded token is normalized to ASCII on Save. Count zero returns before companion decoding in the candidate. The 256-row ceiling is a candidate safety policy and should not be described as original behavior.

The current call is `asciiBytes(*companion, false, true)`: BOM decoding is disabled and UTF-8 BOM is rejected. The proposed decoder must be enabled only at this SPRING2 callsite (or via an explicit format-neutral helper option used only there), not by relaxing shared byte policy globally. Existing exact malformed-token cursor/retry profiles should not be generalized by BOM support; after decoding, only the already-qualified canonical signed decimal int32 tokens plus EOF-short suffix should enter this slice. Byte-preservation assertions should read the original raw files, not the decoded string.

## Test and guard assessment

The proposed two-row `1 0` fixtures under UTF-8, UTF-16LE/BE, and UTF-32LE/BE exercise all signatures; the UTF-32LE fixture specifically protects precedence. Each should assert flags `[1,0]` and unchanged raw primary/companion bytes. Useful negatives are BOM-less high bytes, malformed UTF-8 body, incomplete UTF-16/32 code units, a decoded non-ASCII code point, malformed numeric content, and an additional encoded BOM. Retain EOF-short behavior and the existing count/path gates. If the public writer fixture is available, assert normalized ASCII `1` output separately; reader decoding alone does not prove Open→Save integration.

## Limits

The evidence proves the Qt BOM selection and default-stream construction, while the candidate B_WEIGHT path supplies a constrained ASCII-code-unit decoder implementation. It does not establish malformed decoder replacement/status, BOM-less ACP decoding, non-ASCII numeric characters, or original-process runtime behavior. No source, test, build, GUI, or mutable REA state changed for this review.
