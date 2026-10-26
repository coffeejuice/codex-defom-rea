# CP82 constant-zero path implementation freeze

The existing advanced-page time guard now admits live movement-kind6 objects
whose actual embedded path DYNARY at object+25a8 has ndim0 and a constant value
equal to zero. Both signed zeros qualify; NaN, infinity, nonzero constants and
all table paths remain refused. This follows the original all-zero predicate's
constant branch, so the native caller skips the unrecovered position/evaluation
chain rather than approximating it.

Evidence/selection report is frozen separately:
`analysis/resume82-editor300-next-ready.md`, SHA-256
`2579ec647029c9d8dd4e30401b7ade130635fe0a40265b35287a78e85c51de71`.
Its original PE, exact byte ranges and retained decompile/assembly receipts are
the source qualification. Root approved the exclusive three-file implementation.

## Source changes

- `simcontrol300.cpp`: `advancedTimeProfile()` reads the actual model-owned
  embedded path only after existing object-validity/movement-kind checks. It
  rejects ndim!=0 before reading the constant, otherwise rejects value!=0.0.
  It does not copy, allocate or mutate a curve. Existing Fill and Apply consumers
  both retain the live guard. Existing Apply checks occur before its first store.
- `simcontrol300.hpp`: narrow the documented movement-kind6 restriction without
  changing any signature, type layout or ABI.
- `test_simcontrol300_snapshot.cpp`: four Release-active focused cases added.
  No existing guard expectation was removed. The existing fresh-object table
  path remains outside the newly supported constant profile.

The cases exercise:

1. +0 and -0 through Fill and Apply with both times changed, preserving the
   exact path constant bits and all three position words, including NaN/-0
   sentinels. The existing final advanced remap still runs.
2. Positive/negative nonzero, smallest positive subnormal, both infinities and
   quiet NaN remain refused by both Fill and Apply; the live time store stays
   untouched. Changing the same actual curve back to -0 permits Apply.
3. A valid allocated all-zero table remains guarded; its axis/value data and
   current time survive refusal.
4. A new later live object with an unsupported path causes refusal despite an
   earlier supported object and a previously filled page. Marking that later
   object kind7 reaches the existing native validity skip, preserving its path.

## Static preflight and root validation target

Compared only the exclusive files with CP81's archived source. Quoted includes
resolve through each source directory or the target's existing include/src
directories. Added code uses existing DYNARY definitions and Testkit macros;
no new includes/build registration are needed. Testkit entry point is present
exactly once, all 77 cases are registered, and no C assert is present. Delimiters
are balanced and the three files have no trailing whitespace. Inspected the
actual DYNARY release helper: fixture array cleanup clears all array pointers,
so conversion from the default allocated path to the constant test profile does
not leave a duplicate release. The default type7a path is ndim1, so the old
blanket-refusal fixture remains a table refusal.

Root target: `m2m_mdt_simcontrol300_snapshot`; CTest name
`m2m.mdt.simcontrol300_snapshot`. Case count is 77 (73 existing +4 new).
No build, CTest, executable test, GUI, original execution or mutable REA was run
by this worker. The static checks are not execution results.

## Frozen source hashes

Paths below are relative to `source/apps/moproj2mst/`:

| File | CP82 SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `d64f9245e8f8140a51aa56389698f78ae9e999286c83f471f02c6c5d70b93718` |
| `src/engine/simcontrol300.cpp` | `813e14c4157537beb9223a600644e93933c40826973d29be5d45977ce9da1b6b` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `c49c53850276dcf8e8affd3cc378bf883864f1ef1cc430cd19d2ef9cdf6ea47d` |

Files are released and frozen for root's grouped build/test/archive. No shared
runtime, palette owner, CMake, public mode300 path or private-palette guard was
edited. The public entry still stops at its earlier native widget dependency.

Next advanced-time dependency is the table all-zero traversal (axes before
values, actual shape/owner bounds) or the nonzero path evaluation/rotation
chain. Neither is enabled here. A future table increment should qualify that
owner/shape branch directly instead of treating all empty/zero-looking tables
as constant zero or invoking the inherited broad helper without GUI-source
qualification.
