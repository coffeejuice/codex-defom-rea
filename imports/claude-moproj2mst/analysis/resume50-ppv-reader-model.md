# Resume 50: PPV reader and recovery-model contract

## Evidence boundary

This is a bounded contract from retained static decompilation and current source,
not a fresh original run. Primary native evidence:

- `evidence/gui_mo/s1/mdt/L0/A/b0.c`: reader `0x140d4b400` and its
  `0x1400594d0` SimCtrl caller.
- `evidence/gui_mo/s1/mdt/L0/A/f0.c`: FindOrAddPoint `0x140d4af00`, flag-list
  insertion `0x140d4bb70`, and local-list clear helper `0x1400670b0`.
- `evidence/gui_mo/s1/mdt/L0/spec_A.md`: bounded interpretation of those bodies
  and caller arguments.
- `analysis/resume50-ppv-pattern.md` and
  `analysis/resume48-ppv-reader-branch-receipt.txt`: verified immutable PE
  initializer `\\s+`, SkipEmptyParts, and exact-seven/index-5 reader branch.

Relevant current source is `include/m2m/model.hpp` (`RecoveryEntry`, OPI lists,
root list), `src/moproj/accessors_p01.cpp` (root list accessor),
`src/engine/loader.cpp` (`loadSimCtrl` still refuses PPV),
`src/engine/save_current.cpp` (Save still refuses recovery-B), and
`src/mst/emit_b_lines.cpp` (existing NRECVL/ERECVL KEY serialization).

## Native reader entry and ownership

Reader signature as source-qualified by its callers:

`ReadPpv(localPointList, path, sharedPointList, sharedFlags) -> bool`

At SimCtrl Open (`0x1400594d0`), when `SimCtrl.KEY` exists and `SimCtrl.PPV`
exists, caller obtains:

- `localPointList`: operation-local PPV point list, OPI `+0x20`;
- `sharedPointList`: active subproject data-root recovery list A,
  `0x14005a400` / root `+0x19a8`;
- `sharedFlags`: operation flag list, OPI `+0x28`;
- the selected operation-local count/list object used later to update each
  object's `ppvCount` (`+0xf0`).

If the operation has no diagram predecessor (`0x140050bc0`), caller invokes
`ReadPpv(local, path, null, null)`; otherwise it invokes
`ReadPpv(local, path, shared, flags)`. It then computes PPV point count as
`size(local) + size(shared)` in the predecessor case, otherwise `size(local)`,
and copies that total to each existing operation slot's subobject `ppvCount`.
The caller does not branch on the reader's returned bool in the retained
body. This count update follows the call even if a malformed row caused the
reader to return false. Existing unsupported C++ Open guard is at
`loader.cpp:239`; no current code parses the file.

## File and row processing order

Reader `0x140d4b400` does the following:

1. Starts with return `false`; opens path as `QFile` read-only. Failed open
   returns false without clearing the local list.
2. After successful open, clears only the first argument (local list) via
   `0x1400670b0`. It does not clear the shared root list or shared flags here.
3. Reads first line as decimal count. It calls `trimmed()` but discards the
   returned string, then parses the untrimmed line with `toInt(base 10)`;
   conversion status is ignored.
4. Count `<= 0` yields false after local-list clearing. Count `> 0` sets return
   true before entering the row loop.
5. Reads up to the declared count, splitting each row with the initialized
   regular expression `\\s+` and `SkipEmptyParts`.
6. Fewer than six resulting tokens sets return false and stops. Earlier list and
   flag changes are retained; there is no rollback. The file closes after the
   loop. There is no end-of-file/count-match validation after a successful loop.

The successful-positive-count return flag is not reset for ignored numeric
conversion failures. Extra tokens are ignored except that exactly seven tokens
activates the optional flag branch. Eight or more tokens do not.

## Token fields and mutations

For every row with at least six split tokens, the reader:

| Token | Native operation | Consequence |
|---|---|---|
| 0 | Decimal integer conversion | Parsed to a local, then overwritten by FindOrAddPoint result; no retained use in this body. |
| 1 | String comparison, case-insensitive with literal `true` | Selects shared/root list only when equal to `true` **and** shared list argument is non-null. Otherwise selects local list. |
| 2 | Decimal integer | First integer stored in the PPV point record. |
| 3 | Decimal integer | Second integer stored in the PPV point record. |
| 4 | Double conversion | First double stored in the PPV point record. |
| 5 | Double conversion | Second double stored in the PPV point record. Also, only when token count is exactly 7, the same token 5 is separately parsed as decimal integer and converted to the optional flag boolean. |
| 6 | No direct read in this reader body | Writer emits a final flag column, but reader's exact-seven branch does not fetch token 6. |

On the shared path, it calls `FindOrAddPoint(shared, int2, int3, double4,
double5)`. If flags argument is non-null, it inserts the row's boolean into
the flag list at the returned point index (`0x140d4bb70`). On the local path,
it calls `FindOrAddPoint(local, ...)` and does not mutate the flags argument.
The flag defaults false; it is parsed only for exactly seven tokens. The
native behavior is the index-5 read, even though the matching writer emits its
flag as the seventh field. Treat this as a compatibility fact; intent is not
established and no bug is inferred.

## FindOrAddPoint record and deduplication side effects

`0x140d4af00` constructs a native point record from exactly four values:

- integer A at record offset `+0`;
- integer B at `+4`;
- double A at `+8`;
- double B at `+0x10`.

It queries the destination point list through `0x140d4bab0`. When the lookup
reports no match, it appends the constructed record through `0x140067140` and
returns new list size minus one. When a match exists, it returns the matching
index through `0x140d4b8c0` and does not append a duplicate. For shared rows,
that returned index then controls the flag insertion.

The exact lookup equivalence predicate inside `0x140d4bab0`, duplicate-flag
replacement behavior for repeated shared rows, and relationship between a
preexisting shared list and row order are not established by the retained helper
body. Do not claim exact bitwise/epsilon double comparison or flag overwrite
semantics without that helper's source.

## Distinguish PPV point lists from KEY recovery entries

The currently modeled `RecoveryEntry` in `model.hpp` contains only `int type`
and `double value`. `src/mst/emit_b_lines.cpp` uses this typed structure to
serialize NRECVL/ERECVL counts, flag vectors, and values into KEY content. That
is not sufficient to represent the four-field native PPV point record
(`int,int,double,double`) or its deduplication key by itself.

The native caller describes root/local PPV point lists, while current source
uses one typed `RecoveryEntry` list for KEY recovery state. Treat these as
separate state contracts pending an ownership/layout reconciliation; do not
silently widen `RecoveryEntry` and assume the NRECVL/ERECVL serializer should
consume point records. Before implementation, resolve whether the root PPV
point list aliases another native recovery list or is a distinct field stored
adjacent to the modeled root recovery list. The operation's PPV local list is
also not represented as a two-int/two-double sequence by `RecoveryEntry`.

## Smallest remaining evidence to implement faithfully

1. Inspect only `0x140d4bab0` and `0x140d4b8c0` to establish point equality and
   returned-index semantics; inspect `0x140d49e30` only if constructor
   initialization beyond its four explicitly assigned values matters.
2. Resolve the object-layout relationship for root list A / local list / flags
   from the reader caller and native list accessors before choosing C++ types.
3. Recover writer `0x140d4afe0` guard `0x140307410` and write failure order if
   implementing paired Save behavior; the body, format, and direct-write
   behavior are retained in the writer receipt, but the guard's full semantic
   effect and call order must be reconciled with the current model.

No implementation or test change is recommended until these ownership/equality
facts are closed. Current source's explicit unsupported behavior should remain
in place while the representation is unresolved.

## Status

- Separator and split semantics: settled as `\\s+` plus `SkipEmptyParts`.
- Reader gate, count/open/clear order, row width and partial mutation: settled
  from retained static decompile.
- Two-int/two-double point record and append-or-return-index effects: settled
  from `0x140d4af00` plus the qualified helper notes.
- Deduplication equality, flag duplicate handling, and native root/local field
  layout: unknown.
- Current converter: PPV Open/Save explicitly unsupported; no parser implementation
  exists to compare token behavior against.
- No source edits, builds, GUI, or mutable REA operation were performed.
