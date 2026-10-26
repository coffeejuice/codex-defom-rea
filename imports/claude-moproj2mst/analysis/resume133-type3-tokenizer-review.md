# CP133 independent review: Type3 line tokenizer

The private helper implements the retained `QString::split(QRegExp("\\s+"), SkipEmptyParts)` contract for its explicitly admitted BMP input profile. It clears output first, rejects any UTF-16 surrogate code unit before producing tokens, then splits at maximal runs of the shipped whitespace code units while omitting empty fields. Empty input and all-whitespace input produce no fields. It does not open a file or invoke the Type3 scalar dispatcher.

## Whitespace table comparison

I compared the implementation predicate with `bmp_space_code_units` in `analysis/wave2-doe-regexp-space-contract.json`, bound there to shipped QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. They match exactly: U+0009–U+000D, U+0020, U+00A0, U+1680, U+180E, U+2000–U+200A, U+2028, U+2029, U+202F, U+205F, and U+3000. In particular U+0085 is correctly retained within a token, while the shipped legacy U+180E behavior is correctly treated as whitespace. The test covers ASCII delimiters, U+0085 retention, representative shipped separators including U+180E/U+2028/U+2029, empty/all-whitespace lines, and a supplementary-character refusal. The code matches the entire retained table, though the test does not individually enumerate all 25 entries.

This comparison is limited to BMP code units. Rejecting surrogate input before tokenizing avoids silently substituting a modern Unicode classification for behavior the helper has not qualified. The output vector remains empty on that refusal because clearing occurs before the scan.

## Parser and integration boundary

The original parser's regex initializer and split arguments are statically bound in `resume132-type3-regex-next.md` to literal `\\s+`, case-sensitive QRegExp/RegExp options, and SkipEmptyParts. The helper accepts a caller-supplied `qt::String` line only. There is no QFile/QTextStream line loop, parser status, `.OPT` read, row ownership, or public ShapeOptim/Type3 activation in this change. `NUM_CON` and `REGIONS` remain guarded by the existing scalar dispatcher. Tokenization therefore does not imply that arbitrary `.OPT` files are admitted or that the full parser is reproduced.

No mismatch found within the helper's BMP contract. The Unicode membership is supported by static offline table/disassembly evidence, not execution of Qt's original regex engine. No build, test, GUI, original process, or mutable REA action was performed in this independent review.
