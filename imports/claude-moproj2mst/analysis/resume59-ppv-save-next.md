# Resume 59: PPV Save ordering, row shape, and failure effects

## Qualified ordering

The immutable writer receipt `analysis/resume48-ppv-writer-receipt.txt`
(complete retained body at `0x140d4afe0`) and the gate qualification in
`analysis/resume58-ppv-save-gate.asm` establish this order:

1. Call `QListData::isEmpty()` on the local point list.
2. If local is empty, return false before constructing/opening QFile. Shared
   list size and flags are not inspected on this branch. Thus root-only points
   do not write or truncate `SimCtrl.PPV`.
3. Otherwise construct QFile and open with mode `(2, 0x10)` = WriteOnly | Text.
   Failed open returns false without entering the stream or payload code.
4. Construct QTextStream and emit the count line:
   `local.size() + shared.size()` when the shared pointer is non-null, including
   when the flags pointer is null; otherwise emit `local.size()`.
5. If both shared and flags pointers are non-null, emit every shared row in
   shared-list order, indexing `flags[i]` directly.
6. Emit every local row in local-list order. Its row index restarts at zero.
7. Close QFile, set return true, and destroy the stream. No stream/write/close
   result is checked.

## Count versus emitted rows

Each emitted point row has seven columns and six tab delimiters:

```text
index<TAB>shared-bool<TAB>intA<TAB>intB<TAB>doubleA<TAB>doubleB<TAB>flag
```

Shared rows use `true` and `flags[i]`; local rows use `false` and literal `0`.
Each row and the count line end with `QTextStream::endl`; with QFile Text mode
on the target, the isolated payload uses CRLF.

If shared is non-null but flags is null, the count still includes shared
points, but no shared rows are emitted. Local rows are then emitted normally.
The resulting declared row count can therefore exceed the number of emitted
rows by `shared.size()`. This asymmetry is explicit in the writer's independent
count calculation and `(shared != nullptr && flags != nullptr)` loop gate; it
must not be normalized.

When flags is non-null, the writer indexes `flags[i]` for each shared entry
without a visible length check. The retained evidence establishes the access
but not the behavior for an undersized list; callers must preserve the indexed
access precondition or leave that state unsupported. Integer narrowing/count
overflow for list sizes is also not qualified.

## Failure effects and limits

- Empty-local gate: no file open attempt, no payload, false return.
- QFile open failure after the gate: false return, no stream/payload writes.
- Successful open: writes count, shared rows (when both pointers exist), then
  local rows. The retained body explicitly closes QFile and returns true.
- Post-open stream/write/close failures: not inspected; the writer still
  reaches the true-return path. `endl` is used per line, but the exact bytes
  left on disk after an injected device failure are not retained evidence.
  Do not claim complete or partial output for such failures.

The writer contract and isolated payload tests already cover the normal
shared-before-local row shape, count including shared without flags, opaque
gate result, open failure, and zero-count line. The root-only gate helper test
also covers local-empty/shared-nonempty as a pure decision. Those tests have
not been run by this audit.

## Smallest next testable contract

The narrow remaining deterministic integration case is the writer payload
helper with local empty, shared nonempty, and flags nonempty while the gate is
true: assert that it does not attempt open, emits no bytes, and returns false.
That combines the resolved local-only gate with root-only ownership at the
existing writer-payload seam. It is a pure test of the already source-qualified
early return and should not be wired into public Save. Device-level partial
write/close behavior remains unknown and is not a suitable assertion without
new QFile/device evidence.

No source, CMake, GUI, mutable REA, or build change was made.
