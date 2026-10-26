# Resume 48: SimCtrl.PPV writer, reader delimiter, and seventh field

## Verified artifacts

The immutable target is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The retained reader source is `evidence/gui_mo/s1/mdt/L0/A/b0.c`, SHA-256:
`cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`.
The retained writer source is `evidence/gui_mo/out_s/s1.c`, SHA-256:
`e4e78f5ef1c94d103e50466632dffc363dc6389cbb9e1ff9bba10ea4abe75f27`.
The bounded complete writer copy is `analysis/resume48-ppv-writer-receipt.txt`,
SHA-256 `dcd1e9a5c130954ed782425f19f81b6f2c22008bd7c9abfe9da9445f9de3c453`.
These are immutable retained static artifacts; no new original execution or REA
query was performed.

## Writer `0x140d4afe0`

The function signature is `FUN_140d4afe0(listLocal, path, listShared, flags)`
according to the caller spec in `evidence/gui_mo/s1/mdt/L0/spec_A.md` and its
retained call sites. The function body itself establishes these arguments by
use:

1. Initialize return false. Call `0x140307410(listLocal)`. If that returns true,
   skip file creation and return false. Its exact predicate semantics are not
   established by this writer body; do not rename it as “empty” or “not empty.”
2. Otherwise construct `QFile(path)` and open with `QIODevice` mode flags passed
   as `(2, 0x10)` to helper `0x140031680`. If open fails, return false and do
   not write. The caller sets the operation path to `SimCtrl.PPV`.
3. On open, make `QTextStream`. The first line is the count:
   `size(listLocal) + size(listShared)` when `listShared != nullptr`, else
   `size(listLocal)`. It writes the integer followed by `endl`.
4. If both shared list and `flags` are non-null, write all shared records first.
   For shared record `i`, retrieve flag `flags[i]`, then the shared point record.
   The emitted fields are: `i`, `true`, point integer field A, point integer
   field B, point double field A, point double field B, flag integer; fields
   are separated by literal tab characters and the row ends with `endl`.
5. Then write all local records. The emitted fields are: `i`, `false`, point
   integer A, point integer B, point double A, point double B, literal `0`;
   fields are tab-separated and each row ends with `endl`.
6. If either list has size zero, its loop simply emits no rows. The count line
   still appears, including `0` when both list sizes are zero and the initial
   `0x140307410` predicate permits proceeding.
7. Close the QFile and return true. The code does not inspect QTextStream
   status, QFile close status, or a write result. There is no post-write row
   count or rollback branch. Thus open failure is the only checked I/O failure
   after the initial predicate; partial write/close failures are reported as
   success by this function.

The sequential getter calls in the decompile resolve shared point record
fields at offsets `+0`, `+4`, `+8`, and `+0x10`; local rows use the same layout.
Do not infer higher-level semantic names for these fields from the serialized
column labels alone.

### Literal delimiters and strings

The writer's literal data addresses resolve in the verified GUI-MO image:

- `0x142a138d4`, `0x142a13a64`, `0x142a13a94`, `0x142a13ae0`,
  `0x142a13ae4`, `0x142a13ae8`, `0x142a13aec`, `0x142a13af8`,
  `0x142a13afc`, `0x142a13b00`, `0x142a13b04`, and `0x142a13b08` are
  NUL-terminated tab strings (`09 00`).
- `0x142a13ad8` is `true`; `0x142a13af0` is `false`.
- The final shared-row value comes from `flags[i]`; the final local-row value
  is integer `0`.

These addresses were resolved from the raw PE image using image base
`0x140000000` and section mapping; file identity/hash is recorded above.

## Reader `0x140d4b400`: seventh-field index confirmed

The complete retained reader in `b0.c` reads the first line as decimal count.
After each row, it splits with `QRegExp* 0x1446b4b20` and
`QString::SkipEmptyParts`. It requires at least six resulting tokens. When the
resulting token count is **exactly seven**, it performs
`toInt(t[5], base 10) != 0` and associates that value with the shared-point
flag. The load is explicitly from token index **5**, not index 6. This is not
merely a prose comment: the retained decompile repeats the call
`FUN_140004b40(splitList, 5)` in the `size == 7` branch before the integer
conversion and `0x140d4bb70` call. Writer ordering explains why that is
observable: output columns are `[index, shared, intA, intB, doubleA, doubleB,
flag]`, while the reader's seventh-column branch reads the sixth token, which
is the textual doubleB field. Preserve this asymmetry as original behavior; do
not “correct” the index.

The read's optional field branch sets false initially, runs only for exactly 7
tokens, and passes the resulting boolean to the flag-list insert only for the
shared-list path with a non-null flag list. Rows with six tokens leave that
flag false. Rows with more than seven tokens also leave it false because the
equality check is exact. Earlier rows remain inserted if a later row has fewer
than six tokens; result becomes false and parsing stops.

## Empty/count and failure matrix

| Case | Reader `0x140d4b400` | Writer `0x140d4afe0` |
|---|---|---|
| File absent / open fails | Returns false; reader list clear is not reached. | Returns false if initial predicate is false and path open fails; no output. If predicate is true, returns false before trying the path. |
| File opens, count `<= 0` | Calls `0x1400670b0(list)` first, then returns false because success flag is set only for `count > 0`; no row parse. | Not applicable; writer derives count from list sizes. |
| Both lists empty and initial predicate false | Not applicable. | Opens/truncates, writes `0` plus endl, no rows, closes, returns true. |
| Positive count, valid rows | Returns true after the declared loop. Missing numeric conversions are unchecked. | Writes list sizes and corresponding rows; stream failures unchecked. |
| Malformed row with fewer than six split tokens | Sets false, stops early, closes; previous row mutations remain. | Not applicable. |
| Exactly seven split tokens / seven emitted columns | Reader interprets token 5 as integer flag, not token 6. | Shared row writes `flags[i]`; local row writes literal 0 as token 6 in zero-based index 6. |
| More than seven tokens | Reader does not enter optional-flag branch; remaining tokens are ignored by this function. | Writer always emits exactly seven columns per row. |
| Writer post-open I/O error | Not applicable. | No explicit check; closes and returns true. |

The writer's `0x140307410` precondition remains a separate unknown branch. The
table avoids assigning it an empty/nonempty meaning.

## Separator regex object remains unresolved

Reader call site `0x140d4b400` passes static object address `0x1446b4b20`
to `QString::split` with `SkipEmptyParts`. In the verified PE, image base is
`0x140000000`, so this is RVA `0x046b4b20`, in the zero-filled virtual tail of
the `.data` section (not file-backed bytes). The retained `spec_A.md` explicitly
leaves its static QRegExp initializer as open question 4. No initializer or
pattern literal appears in the searched retained decompiles. Therefore the
pattern itself is still unknown; `SkipEmptyParts` and its target address are
settled. Do not call this whitespace, tab, comma, or any other delimiter until
the initializer is recovered.

Smallest needed query, without changing the shared REA database: a read-only
initializer/xref search for global `0x1446b4b20` in the immutable target, limited
to constructor writes to the QRegExp object and its pattern-string argument.
Alternatively inspect an already retained startup/constructor artifact that
writes that global. No other writer/reader body needs re-extraction.

## Contract disposition

The prior resume48 note is superseded on writer grammar and seventh-field index:
writer and reader behavior above are now source-bound to retained decompiles.
Remaining open items are (a) meaning/order effects of `0x140307410`,
(b) exact regex pattern initializer, and (c) semantic structure-field mapping
only if an implementation needs names beyond byte-faithful record storage. The
reader's count/open/partial-mutation order and writer's list order, zero-count
branch, open failure, and unchecked post-open I/O are established.
