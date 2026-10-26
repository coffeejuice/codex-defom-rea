# CP133 Type3 line tokenizer

## Result

Added a private value-only tokenizer for a supplied `qt::String` line. It implements the retained Type3 contract `QString::split(QRegExp("\\s+"), SkipEmptyParts)` for the qualified BMP profile. It does not read files, construct or retain the native static QRegExp, parse optimizer rows, or change public Type3 admission.

## Evidence boundary

`analysis/resume132-type3-regex-next.md` ties the original PE's global regex at `0x143dd9a28` to literal `\\s+` at `0x1427b9f10`, QRegExp constructor options case-sensitive/RegExp, and split behavior `SkipEmptyParts`. The original executable hash recorded there is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

The splitter uses the shipped QtCore QChar membership set retained in `analysis/wave2-doe-regexp-space-contract.md`: TAB–CR, space, U+00A0, U+1680, U+180E, U+2000–U+200A, U+2028, U+2029, U+202F, U+205F, and U+3000. In particular U+0085 remains inside a token. This is static table-derived BMP evidence, not execution of the original regex engine.

## Implementation and limits

The helper clears the destination first, scans the complete UTF-16 input for surrogate code units, then returns `UnsupportedSurrogateInput` with no tokens if any occur. Supplementary-plane regex behavior is therefore explicitly outside the admitted profile. For accepted BMP input it collapses whitespace runs, omits empty fields, and preserves token code units and order. Empty or all-whitespace input yields no fields.

The focused tests cover ASCII separators, the retained U+0085/U+180E/U+2028/U+2029 distinction, empty input, and explicit surrogate rejection. The all-whitespace U+3000 fixture is constructed directly from UTF-16 code units; `qt::L(const char*)` is Latin-1 and would not express that character. Root owns focused/full validation. No file tokenizer, line-ending normalization, parser, row schema, regex static lifetime, or public Type3 behavior is claimed.
