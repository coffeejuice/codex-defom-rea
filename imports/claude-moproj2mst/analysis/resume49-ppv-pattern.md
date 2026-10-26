# Resume 49: PPV separator initializer search and index qualification

## Target and retained evidence

Immutable target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, PE image
base `0x140000000`.

The reader function `0x140d4b400` is retained in
`evidence/gui_mo/s1/mdt/L0/A/b0.c` (SHA-256
`cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`). The
writer function `0x140d4afe0` is retained in `evidence/gui_mo/out_s/s1.c`
(SHA-256 `e4e78f5ef1c94d103e50466632dffc363dc6389cbb9e1ff9bba10ea4abe75f27`).
The relevant open-question record is `evidence/gui_mo/s1/mdt/L0/spec_A.md`,
which explicitly says to find the static QRegExp initializer for question 4.
No initializer or constructor body for this object appears in the retained
source set searched.

## Separator initializer result

Reader `0x140d4b400` passes global object `0x1446b4b20` to `QString::split`
with `QString::SkipEmptyParts`. Address mapping gives RVA `0x046b4b20`, within
the `.data` section's zero-filled virtual tail: the object is not backed by
bytes in the immutable PE file. Literal scanning for its absolute 64-bit
address in the PE also finds no file occurrence, consistent with RIP-relative
references and runtime initialization rather than a file-backed global
initializer. This scan does not prove that no code reference exists.

**Pattern remains unknown.** No pattern string or QRegExp constructor write was
found in the preserved static evidence. Do not infer the pattern from tabbed
writer output or `SkipEmptyParts`.

## Exact-seven branch: observed behavior, intent unknown

The retained full reader decompile shows:

- Split the line at `0x1446b4b20` using `SkipEmptyParts`.
- Reject a row with fewer than six resulting tokens.
- When `size == 7`, load token index **5**, convert it to base-10 integer, test
  nonzero, and pass the resulting flag into the shared-list flag insertion
  path `0x140d4bb70`.

The retained writer `0x140d4afe0` emits seven tab-separated columns per row:
`index`, `true`/`false`, two integer fields, two double fields, and a final
flag (`flags[i]` for shared rows, literal `0` for local rows). This confirms the
reader/writer asymmetry: the reader uses token 5 for the optional flag while the
writer places the flag in token 6. The reader also executes the flag branch only
for exactly seven tokens; more than seven tokens do not enter it.

This is the native implementation shown by the retained decompilation. Its
**intent is unknown**. Do not describe it as an intentional feature or as a bug;
for compatibility work, preserve the observed index unless better original
instruction evidence contradicts the decompiler. The previous bounded reader
receipt at `analysis/resume48-ppv-reader-branch-receipt.txt` (SHA-256
`75d7b1c28f88152d5c0393a042040f5ff7fb9e45fccb033b28346255f8182cdd`) preserves
the enclosing row parse, size test, element getter, conversion, and insertion
call. The writer receipt is
`analysis/resume48-ppv-writer-receipt.txt` (SHA-256
`dcd1e9a5c130954ed782425f19f81b6f2c22008bd7c9abfe9da9445f9de3c453`).

## Smallest remaining query

The available evidence cannot reveal the pattern initializer. Ask the owner of
the existing static analysis database for one read-only bounded query: list
references/constructor writes to `0x1446b4b20`, retaining only the QRegExp
construction call, its pattern argument/string, and function address. Do not
mutate or open the shared database in this task. If that database has no
initializer xref, leave the separator unknown; no wider analysis is justified
by current evidence.

No further query is needed to establish the decompiled token index. If an
independent instruction-level check is wanted, inspect only the bounded
`size == 7` block of `0x140d4b400`, from the size comparison through the
argument to `0x140d4bb70`; preserve instruction bytes/addresses and verify them
against the same target hash. Do not reopen or relabel the behavior as a defect
without such contradictory evidence.

## Disposition

- QRegExp pattern: **unknown**; immutable PE storage is zero-filled and no
  retained initializer artifact is available.
- `size == 7` reader token: **observed index 5** in the complete retained
  decompile; semantics/intent remain unknown, and no bug is inferred.
- Correctness policy for reconstruction: preserve observed index 5 pending
  source contrary evidence; keep the seven-column asymmetry explicit.
- No source, build, GUI, or mutable REA operation was performed.
