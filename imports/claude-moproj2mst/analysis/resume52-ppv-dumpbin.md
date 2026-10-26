# Resume 52: bounded DUMPBIN recovery of PPV point equality and index

## Target and method

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, PE
image base `0x140000000`. The target hash was checked before disassembly. The
local Microsoft DUMPBIN was Visual Studio 2022 Build Tools 14.44.35228.0. It
disassembled only these VA windows:

- `0x140d4bab0..0x140d4bb60`: lookup;
- `0x140d4b8c0..0x140d4b9df`: matched-index search;
- `0x140d4b9e0..0x140d4baaa`: point comparison;
- `0x141470a78..0x141470b63`: floating absolute-value helper used by both
  double comparisons.

Captures are the adjacent `resume52-ppv-{find,index,equal,abs-helper}.asm`
files. Capture SHA-256 values:

| Capture | SHA-256 |
|---|---|
| `resume52-ppv-find.asm` | `dfb9a541735a854b600bbf02205cfa4bef2bb8a9527ccb3c66580e46a79d149f` |
| `resume52-ppv-index.asm` | `74bf838941776c980c5d98103564a1d663cc000e5006742276a47ff9c48b6b68` |
| `resume52-ppv-equal.asm` | `fbc8a74c4b8fa010f848633a7cff261ad559402380e3b675ba70844a0b6ced46` |
| `resume52-ppv-abs-helper.asm` | `7542dcb29c9a1d8ad749000548ab345276e594d40fff8c51860912d19f9b71d7` |

No REA target/session was opened or modified for these captures. Ghidra was not
retried.

## Exact point equivalence predicate

`0x140d4bab0` iterates the point list and calls `0x140d4b9e0` on each candidate.
The comparator at `0x140d4b9e0` establishes this predicate for the record layout
already shown at `0x140d4af00`:

```text
candidate.int0 == query.int0
&& candidate.int1 == query.int1
&& abs(candidate.double0 - query.double0) < 1.0e-6
&& abs(candidate.double1 - query.double1) < 1.0e-6
```

Both integer tests are direct 32-bit `cmp` instructions and reject on `jne`.
For each double, the code subtracts query from candidate, calls
`0x141470a78`, loads the same threshold constant at `0x142157420`, compares
threshold with the helper result, and rejects on `jbe`. `jbe` rejects when
threshold is less than or equal to the absolute difference (and also on an
unordered comparison); equality requires a **strictly smaller** difference.

The constant is file-backed at PE RVA `0x02157420` (file offset `0x02155c20`)
with bytes `8d ed b5 a0 f7 c6 b0 3e`; interpreted as little-endian IEEE-754
binary64, this is `1e-6`. The helper's ordinary numeric path removes the sign
bit before returning the double, establishing absolute difference for finite
inputs. Therefore both double fields use the same strict absolute tolerance;
they are not bitwise-equal comparisons.

## Lookup and matched-index behavior

`0x140d4bab0` starts at the beginning of the list and advances candidate by
candidate until the comparator succeeds or the end is reached. It returns a
nonempty match indication when a candidate compares equal and an empty result
at end. The caller `0x140d4af00` then calls `0x140d4b8c0(list, query, 0)` to
obtain an index. Thus lookup followed by index selection deliberately performs
a second scan.

`0x140d4b8c0` uses its third argument as the start index. A negative start is
converted to `size + start`; a start at or past size returns `-1`. It scans
forward from the selected point through the list end, testing each candidate
with `0x140d4b9e0`. On the first match it returns the element pointer's byte
offset from the list begin pointer divided by 8, i.e. the zero-based index in
the QList pointer sequence. It returns `-1` if there is no match from that
start to the end; there is no wraparound. The actual `FindOrAddPoint` call
passes start index `0`, so it reuses the **first matching point in list order**.

This closes the duplicate semantics: points with equal integer fields and
both double fields within the strict tolerance deduplicate to the earliest
matching index. Values exactly `1e-6` apart in either double field do not
match. The index returned controls the shared flag insertion described in
`analysis/resume50-ppv-lookup-contract.md`; repeated matching rows still insert
additional flags at that index rather than replacing an earlier flag.

## Reconciliation and implementation implication

The remaining equality/first-match uncertainty in
`analysis/resume50-ppv-lookup-contract.md` is now **settled**. If PPV state is
implemented, the standalone PPV point lookup must compare two integers exactly
and each double with `abs(delta) < 1e-6`, and return the first match from the
start index (currently zero). Keep separate local points, shared points, and
flags, and preserve insertion behavior. This does not resolve the native
DB-owned shared point list's mapping to current C++ structures; that layout
boundary remains open. Current unsupported loader/save guards should stay
until that ownership mapping and the paired Save contract are settled.

No source, build, GUI, or mutable REA changes were made.
