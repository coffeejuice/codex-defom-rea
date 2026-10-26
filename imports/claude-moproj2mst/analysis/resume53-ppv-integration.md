# Resume 53: PPV reader integration boundary

## Decision

The frozen PPV parser is now wrapped by a bounded I/O-layer function that binds
caller-owned, distinct PPV point and flag lists. The real `loadSimCtrl` path
remains explicitly unsupported. Retained evidence supports the reader's row
mutations, but current Project/Operation model lists cannot safely receive the
native point records without model/header work outside this assignment.

## Source-qualified native behavior

The reader contract is established by `analysis/resume50-ppv-reader-model.md`
and `analysis/resume52-ppv-dumpbin.md`:

- After successful QFile open, native `0x140d4b400` clears only operation-local
  points (`OPI + 0x20`), parses a positive decimal count, and mutates each row
  immediately. A short row returns false while earlier row changes remain.
- It splits on `\\s+` with `SkipEmptyParts`; rows need at least six tokens.
  Shared selection is case-insensitive token 1 equal to `true` only when the
  shared-list argument exists; otherwise the row goes to local points.
- The point record contains two integers and two doubles. `0x140d4b9e0` compares
  both integers exactly and both double deltas strictly below `1e-6`.
  `0x140d4b8c0` returns the first matching zero-based index from the supplied
  start; `FindOrAddPoint` passes zero, appending only when there is no match.
- Exactly seven tokens enables the optional flag conversion, but it converts
  token 5 (also the second double), not token 6. Shared rows insert a flag at
  the returned point index; repeated matches insert and shift existing entries.
- Open failure returns before clearing. A successful open with count `<= 0`
  clears local points and returns false. Missing/short rows after positive
  count fail without rolling back previous mutations. Extra file rows beyond
  the declared count are ignored.

## Why actual Project/Operation binding is not safe here

The qualified caller identifies three separate native destinations:

| Native destination | Current C++ candidate | Current representation |
|---|---|---|
| Local point list, `OPI + 0x20` | `Operation::recoveryB` | `std::vector<RecoveryEntry>`, each entry only `{int,double}` |
| Shared point list, DB `+0x19a8` | active subproject root `recoveryA` via `recoveryListA` | `std::vector<RecoveryEntry>`, each entry only `{int,double}` |
| Flag list, `OPI + 0x28` | `Operation::recoveryFlags` | `std::vector<bool>` |

The two candidate point lists are also read by the existing KEY
NRECVL/ERECVL serializer in `src/mst/emit_b_lines.cpp`. A native PPV point is
`{int,int,double,double}` and has an equality key spanning all four fields.
The current element type cannot hold it. Reusing or widening these existing
lists in this task would also change the state consumed by the KEY serializer.
No retained evidence qualifies a separate existing four-field PPV container
or a safe alias to one. Therefore this change does not mutate
`Operation::recoveryB`, `Project/SubProject::recoveryA`, `recoveryFlags`, or
the engine `SimControl` object.

## Bounded change

`src/engine/simcontrol300_io.cpp` adds
`readPpvForDetachedLists(opened, lines, localPoints, sharedPoints, flags)`.
It delegates to the frozen header-only reader and deliberately accepts only
caller-supplied PPV point/flag lists. It performs no filesystem access and has
no implicit Project, Operation, recovery-list, or current-directory lookup.
The one new test `tests/mdt/test_simctrl_ppv_binding.cpp` calls this boundary
and checks distinct list routing, shared-point reuse with flag insertion, and
open-failure preservation. Its declaration is kept local to the test rather
than expanding a public header.

## Public guard retained

`src/engine/loader.cpp::loadSimCtrl` still refuses an existing `SimCtrl.PPV`
file. The exact existing guard remains:

```cpp
if (qt::fileExists(ppv))
    unsupported("SimCtrl.PPV (point tracking, 0x140d4b400; its field separator QRegExp 0x1446b4b20 is "
                "not recovered)");
```

The guard is not weakened or moved. Its “field separator … not recovered” text
is now stale because the separator was recovered as `\\s+`; correcting that
message is outside the owned files. Public Save's nonempty PPV refusal is also
unchanged. This bounded list binding is not a claim that PPV Open/Save is
integrated.

## Next integration dependency

Before connecting the wrapper to `loadSimCtrl`, add separately reviewed model
fields/accessors for the native four-field local and shared PPV point lists,
and prove their ownership/lifetime relative to the existing recovery vectors.
Then integrate caller count propagation and preserve partial mutation/failure
ordering. Keep PPV Save guarded until its paired writer, point/flag ownership,
and public Save routing are integrated and validated.

No model/header, loader, Save, CMake, GUI, or mutable REA changes were made.
The added focused test was not run; no build was started.
