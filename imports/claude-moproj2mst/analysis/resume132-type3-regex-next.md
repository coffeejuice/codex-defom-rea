# CP132 Type3 regex and classifier frontier

## Conclusion

The missing Type3 delimiter is now recovered from immutable PE evidence. Global object `0x143dd9a28` is initialized by code at `0x141d32ba0` from the literal `0x1427b9f10`, whose bytes are ASCII `\\s+` followed by NUL. The QRegExp constructor is called with raw options `r8d=1`, `r9d=0`; the same QtCore IAT signature is independently bound in `analysis/resume50-ppv-pattern.md` to `QRegExp(QString const&, Qt::CaseSensitivity, QRegExp::PatternSyntax)`, with 1 = case-sensitive and syntax 0 = RegExp. The parser's `QString::split` calls pass split behavior 1, SkipEmptyParts. Thus the source-backed line tokenization contract is `QString::split(QRegExp("\\s+"), SkipEmptyParts)`; this finding does not by itself qualify every Qt Unicode/regex edge or public Type3 activation.

The next smallest private prepared-value seam is the stateless `DefOptModel +0xa0` tag classifier at `0x14091f0b0`. Its body and embedded tables are recoverable without row ownership. This can derive the classifier input needed by the CP131 prepared `NUM_CON` row API, while leaving the parser and row keys guarded.

## Regex initializer evidence

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256 was rechecked as `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. `dumpbin /headers` shows `.data` virtual address `0x14354c000`, raw size `0x82a400`; the regex object at `0x143dd9a28` is `0x88da28` bytes into the section, in the zero-filled tail beyond the raw file data. Its value therefore comes from initialization code rather than a literal PE object image.

Bounded dumpbin windows around the two RIP-relative references found by a read-only `.text` scan show:

- `0x141d32ba0` builds a temporary QString from `0x1427b9f10` using QtCore IAT `0x141e04480`; the literal bytes at that VA are `5c 73 2b 00` (`\\s+`).
- It calls QtCore IAT `0x141e04078` with `rcx=&0x143dd9a28`, `rdx=&patternQString`, `r8d=1`, `r9d=0`; then destroys the temporary QString.
- It registers cleanup function `0x141df9e20`; bounded code at `0x141df9e20` passes `0x143dd9a28` to QtCore IAT `0x141e040a0` and returns, matching the QRegExp destructor call. The neighboring static initialization/destruction stubs operate on QString globals at `0x143dd9a20` and `0x143dd9a30`.
- The parser's retained `QString::split` calls at `0x1409088ed`, `0x140908c24`, and `0x140908e97` pass the same QRegExp object and `r9d=1`.

The PE literal inventory `analysis/wave2-doe-parser-literals.json` independently lists `\\s+` at `0x1427b9f10`, but describes its hits as byte-pattern candidates. The constructor-to-global instruction sequence above supplies the missing reference and call identity. For Qt split behavior and the QRegExp whitespace class, retain the narrower detailed qualification in `analysis/wave2-doe-regexp-space-contract.md`; it establishes the shipped QRegExp `\\s` category/range behavior, including that U+0085 is not a separator. No REA database import was attempted.

## Exact classifier value map

`evidence/gui_mo/s1/vtables.tsv` binds DefOptModel `+0xa0` to `0x14091f0b0`. Bounded dumpbin through `0x14091f120` shows:

1. Start result at `-1`.
2. Compute the 32-bit `tag - 0x3f2` value.
3. If unsigned result is greater than `0x5c`, return `-1`.
4. Otherwise use the byte table at `0x14091f134` (file offset `0x91e534`) to choose one of four jump targets in the dword table at `0x14091f124`; jump arms store result 0, 1, or 2, while table class 3 reaches the unchanged `-1` return.

The raw 93-byte table maps:

| Result | Input tag values |
|---|---|
| 0 | `0x3f2`, `0x3f4`, `0x438` |
| 1 | `0x3fd–0x3ff`, `0x406`, `0x410`, `0x41f–0x421`, `0x424`, `0x442–0x444`, `0x44c–0x44e` |
| 2 | `0x439` |
| -1 | Every other integer, including values outside `0x3f2–0x44e` and class-3 entries in the table |

The lower/out-of-range behavior follows the unsigned `ja` after subtraction, so values below `0x3f2` also reach `-1`. The classifier has no observed writes to optimizer state.

## Smallest next code seam

A private pure helper `classifyType3OptimizerRowTag(std::int32_t tag)` can implement the exact bounded map above and be tested over every mapped input plus lower/upper out-of-range values. `NUM_CON` parsing can remain guarded; its prepared append API can later receive the classifier result from this helper. This avoids taking on QRegExp line splitting, numeric conversion, static-object lifetime, native vector allocation, or public Type3 admission in the classifier increment.

The alternative next tokenization cut is now evidence-possible: split a supplied QString line on the recovered `\\s+` expression with SkipEmptyParts using the already-qualified QChar whitespace table. Keep that separate from QFile/TextStream parser ownership, regex-engine generalization, and public `.OPT` loading; the CP132 classifier helper is the smaller independent code seam.

## Qualification

This pass used the immutable original PE hash above, retained parser/literal evidence, bounded dumpbin windows for initializer/destructor and classifier, and direct raw PE bytes for the classifier tables. No source/build/test/GUI/original process or mutable REA action was performed. No public Type3/ShapeOptim support is claimed.
