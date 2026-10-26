# Resume 50: PPV separator initializer and seven-token reader branch

## Immutable target and bounded receipt

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`
SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
Image base: `0x140000000`.

Initializer bytes and argument setup are preserved in
`analysis/resume50-ppv-init-xref-receipt.txt` (receipt SHA-256
`1c2627bc8a089d9934960b0ea5df1c54b8e6805cf09a1369c4ea279646f51846`). The
capture is a bounded raw-PE instruction excerpt, not a mutable REA database
result.

## QRegExp constructor xref

A bounded RIP-relative reference in `.text` function at `0x141d57570` resolves
to the previously identified global object `0x1446b4b20`:

- `0x141d5757d`: `lea rdx,[rip+0x00cbc34c]` → `0x142a138d0`.
- `0x141d57584`: `lea rcx,[rsp+0x20]`.
- `0x141d57589`: indirect import call at IAT cell `0x141e04480`, resolved from
the PE import table as `QString::QString(char const*)`.
- `0x141d57590`: `xor r9d,r9d`; `0x141d57593`: `mov r8d,1`;
  `0x141d57599`: `lea rdx,[rsp+0x20]`.
- `0x141d5759e`: `lea rcx,[rip+0x0295d57b]` → global `0x1446b4b20`.
- `0x141d575a5`: indirect import call at IAT cell `0x141e04078`, resolved as
  `QRegExp::QRegExp(QString const&, Qt::CaseSensitivity, QRegExp::PatternSyntax)`.

The bytes at `0x142a138d0` are `5c 73 2b 00`, ASCII `\s+` followed by NUL.
Thus the constructor initializes the PPV split expression to **`\s+`**,
case-sensitive (`r8d=1`) with pattern syntax enum 0 (`r9d=0`). The constructor
writes to the global used by the reader at `0x140d4b400`. This resolves the
prior pattern uncertainty using immutable PE bytes and import identity; it does
not require the shared REA database.

The reader calls `QString::split(global, QString::SkipEmptyParts)` at
`0x140d4b400`. Therefore PPV rows split on one or more regex whitespace
characters and empty fields are discarded. This explains why repeated tabs or
spaces between fields do not create empty tokens.

## Exact-seven reader index: verified behavior, intent not established

The complete bounded reader decompile is already retained at
`analysis/resume48-ppv-reader-branch-receipt.txt` (SHA-256
`75d7b1c28f88152d5c0393a042040f5ff7fb9e45fccb033b28346255f8182cdd`). Its
branch requires split-list size exactly 7, then loads token element **5**,
converts it to base-10 integer, tests nonzero, and passes the boolean to
`0x140d4bb70`. The writer body at `0x140d4afe0` emits seven whitespace-separated
columns: index, `true`/`false`, two integer fields, two doubles, final flag.
The final flag is writer token index 6; reader's optional flag conversion is
from token index 5.

The bounded decompile establishes the observed native index. It does not
establish why the native code uses index 5. Treat this as compatibility
behavior and retain it in a faithful implementation. Do not relabel it as an
intentional design decision or a defect without contrary source evidence. The
input reader's exact-seven check, index-5 conversion, and flag-list insertion
are visible together in the retained complete function; no wider control-flow
query is needed to establish those facts.

## Disposition

- Separator pattern: **settled as `\s+`**, from direct PE bytes plus a
  constructor call whose import identity and destination global are verified.
- Split behavior: **settled as regex whitespace runs with `SkipEmptyParts`**.
- Exact-seven flag field: **native reader reads token 5**; writer places flag in
  token 6. Behavior is established, intent remains unknown, no bug inferred.
- No source edits, build, GUI operation, or mutable REA access was performed.
