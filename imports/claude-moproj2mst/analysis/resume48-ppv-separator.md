# Resume 48: PPV separator initializer and reader field-index audit

## Immutable target identity

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`
SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
Image base: `0x140000000`.

Reader call site: `0x140d4b400` passes global `QRegExp` object `0x1446b4b20`
to `QString::split` with `QString::SkipEmptyParts` (`1`). The target address
maps to RVA `0x046b4b20`, inside `.data` at RVA `0x0354c000` plus offset
`0x01168b20`. The `.data` section's file-backed size is `0x0082a400`, so this
object lies in the zero-filled virtual tail and has no corresponding initializer
bytes in the PE file. Direct inspection of the immutable PE therefore cannot
recover the pattern. Retained `evidence/gui_mo/s1/mdt/L0/spec_A.md` also labels
this initializer an open question. No other retained decompile found a write or
constructor for this object.

**Separator result: unknown.** The evidence establishes the object address and
SkipEmptyParts behavior only. Do not infer whitespace, comma, tabs, or another
pattern from the file or output column layout.

## Bounded reader branch check

The complete reader decompile is preserved at
`analysis/resume48-ppv-reader-branch-receipt.txt`, SHA-256
`75d7b1c28f88152d5c0393a042040f5ff7fb9e45fccb033b28346255f8182cdd`.
Its source, `evidence/gui_mo/s1/mdt/L0/A/b0.c`, SHA-256
`cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`.

In function `0x140d4b400`, the row is split at the `0x1446b4b20` global. The
branch checks `splitList.size() == 7`; within that branch it calls the list
getter at index **5**, then converts that returned token with `toInt(base 10)`
and compares it with zero before the `0x140d4bb70` flag-list insertion. The
separate minimum-width check is `< 6`. Thus the retained static body confirms
an exact-seven-only optional flag and confirms the loaded token is index 5,
not index 6. This is an observed native implementation detail, not evidence of
a bug. The reader and writer are asymmetric: the writer emits seven columns,
with its final column drawn from the flag list, while the reader converts the
sixth token for the flag. Preserve this behavior unless a higher-quality
source-bound disassembly contradicts the decompile.

The reader compares the second token to `true` with case-insensitive Qt
comparison. The exact-seven flag path is inside the selected shared/global
record branch; the decoded flag is supplied to `0x140d4bb70`. For six tokens,
more than seven tokens, or a non-shared/local row, this branch does not supply a
true optional flag. The whole reader receipt preserves surrounding row parsing
and list-selection order.

## Smallest remaining immutable-evidence query

The pattern is runtime-initialized, so the PE's zero-filled global cannot answer
it. A read-only static initializer query against the existing retained analysis
is required (without mutating the shared REA DB): find constructor/startup
writes referencing global `0x1446b4b20`, then retain only the QRegExp
construction call and its pattern argument/string. If the current PE has no
retained initializer xref, record that absence and leave the pattern unknown;
do not broaden to unrelated parser routines. The seventh-field index needs no
further query unless the original bytes are explicitly disassembled to resolve
a decompiler disagreement: the bounded full-function body already shows the
index-5 getter and insertion branch.

## Disposition

- Pattern initializer: **unresolved**; no file-backed pattern bytes or retained
  constructor write found.
- Reader's optional field index: **settled from retained bounded decompile as
  index 5 under `size == 7`**; do not call it a bug.
- Target/source identities and hashes are recorded above.
- No source changes, builds, app control, or mutable REA operations were made.
