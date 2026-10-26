# Shipped Qt DOE whitespace classification

U+0085 is not a separator in the original DOE sample parser's `QRegExp("\\s+")`. The candidate now keeps it inside tokens instead of refusing the input. This closes the earlier regex-routing uncertainty; it does not establish complete Qt text or numeric-conversion equivalence.

The fixed original DLL is `evidence/original-binaries/QtCore4_64.dll`, SHA256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred image base `0x67000000`. All evidence below comes from offline DUMPBIN disassembly and direct PE table reads. No target code, DLL, original function, UI, or REA session was executed or loaded.

## Exact membership path

| Original site | Established behavior | Retained selection |
|---|---|---|
| `0x67063b30`, `0x67063b6a`, `0x67063bac` | Token reader clears category mask and range vector, then sets negation to false before dispatch. Its backslash switch case reaches `0x67063ef9`, which calls escape parser `0x670629d0`. | `wave2-doe-regexp-token-reset.asm` |
| `0x67062b0e`–`0x67062b2e` | Escape switch subtracts `0x30`, indexes selector table `0x67063adc`, then RVA table `0x67063a98`. For lowercase `s`, selector 13 resolves to `0x67062ceb`. The preceding short-escape lookup contains only `afnrtv`, so it cannot consume `s`. | `wave2-doe-regexp-escape-space.asm`; literal bytes in JSON |
| `0x67062ceb` | Adds category mask `0x380` through `0x6705af10`, adds inclusive range 9–13 through `0x67062420`, and returns character-class token 7. Negation remains false. | `wave2-doe-regexp-escape-space.asm` |
| `0x6705ada0`, `0x6705af10`, `0x67062420` | Fresh class has zero mask/false negation; category addition invalidates every cache bucket with zero. Range addition stores start plus inclusive length and invalidates affected buckets. The membership cache's `INT_MAX` shortcut means immediate nonmembership, not a positive cached match. | `wave2-doe-regexp-class-init.asm`, `add-categories.asm`, `add-range.asm` |
| `0x67059cc0` | Tests the category bit, then range membership; an unmatched positive class returns false. Calls `QChar::category()` at `0x67069fb0`. | `wave2-doe-regexp-membership.asm` |
| `0x67069fb0` | Reads the two-level UTF-16 property index at `0x671f4050`, then the 20-byte property record at `0x671ff7b0`; returns its low byte as category. The subsequent x86 `bt` uses category modulo 32. | `wave2-doe-regexp-category.asm` |

For U+0085, the first index is 6384, slot 6389, property record 46, and category byte 10. Bit 10 is absent from `0x380` (only bits 7–9 are set), and 133 is outside 9–13. The result is therefore false. Adjacent U+0084 and U+0086 are also false. U+180E, U+2028 and U+2029 remain separators in this shipped table; modern Unicode-library classifications must not replace it.

The public matching route is also connected by decoded calls: retained QString regex-split evidence calls `QRegExp::indexIn` at `0x670686a0`; its call at `0x67068740` reaches `0x67066840`, which calls matcher `0x670644e0` at `0x6706691a`/`0x67066937`. Matcher calls, including `0x67064808`, reach membership `0x67059cc0`. These are instruction-decoded call edges, not merely byte-pattern search results. The exact parser and global `\\s+` initialization remain linked in the prior [lifecycle report](wave2-doe-lifecycle.md).

## Candidate scope and validation

Only the U+0085 refusal was removed from `parseSampleText`; the existing recovered separator set was preserved. The new `doe_sample_parser_shipped_regexp_whitespace_boundaries` test checks a sole NEL-containing token is dropped as an id-only row, and distinguishes NEL/neighboring nonseparators from original control and Unicode separators. Boundary characters in ignored row labels avoid turning a whitespace test into an assumption about numeric conversion.

`tools/doe_regexp_space_static.py` independently evaluates the shipped tables for all 65,536 BMP code units and asserts equality with the retained QChar-space set. It captures ten bounded disassembly selections and hashes the DLL, decoded byte intervals, evidence files, reproducer and three modified source files in [the machine-readable receipt](wave2-doe-regexp-space-contract.json). This is static evaluation, not execution of original regex code. The coordinator owns the candidate build and focused/full tests; this worker performed no build.

Original `QString::toDouble` first attempts default QLocale and then C locale; the candidate helper remains C-only. Default-locale and other numeric edge cases remain unresolved. Earlier evidence artifacts and their narrower QChar-only qualification are preserved unchanged; this receipt supersedes only the U+0085 regex-routing uncertainty.
