# Resume 56: PPV reader bound to existing model owners

## Change

`src/engine/simcontrol300_io.cpp` replaces the detached-list entry point with
`readPpvForOperation(opened, lines, operation, hasPredecessor)`. It resolves
the native list roles as follows:

- local point records are mapped to `Operation::recoveryB` (OPI `+0x20`);
- when `hasPredecessor` is true, shared point records are mapped to
  `recoveryListA(operation)` (the active project or type-4 subproject root at
  `+0x19a8`);
- flags are passed as the operation's optional `recoveryFlags` (OPI `+0x28`)
  only in the predecessor case. Without a predecessor, the reader receives no
  shared list and no flags, so a `true` row follows the native local route.

`RecoveryEntry` conversion uses the frozen model mapping: `type` is PPV's
first integer; `ppvSecondInt`, `ppvFirstDouble`, and `ppvSecondDouble` hold the
other equality keys; `value` remains the KEY field. Newly constructed PPV
entries set `value` to zero, matching native construction at BC `+0x18`.
Existing shared records that match remain unchanged, including their KEY
value. New shared records append in list order, while flag insertions happen
directly on the actual `recoveryFlags` vector at the parser's first-match
index.

## Adapter and guards

The frozen `readPpvLines` API accepts `std::vector<simctrl_ppv::Point>&`, not a
model-entry container. The wrapper therefore creates call-local point
snapshots, invokes that reader, and commits its list effects to the actual
owners. These snapshots live only for the call; the model has no second
persistent PPV list. Local records are committed after every successful-open
parse attempt, including count/row failures, preserving local clearing and
partial results. Shared point additions are appended after the call; existing
shared records are not rewritten. Flags are mutated in place, preserving
partial insertions on later row failure. A QFile open failure returns before
any snapshot or owner mutation.

If the local owner is null, if a predecessor is declared but the root accessor
cannot provide a list, or if a non-null flag list is shorter than the existing
shared point list, the wrapper returns false without invoking the parser. The
last condition protects the frozen helper's explicit `insert(index)` range
precondition. This is a guarded model binding, not permission to weaken
`loadSimCtrl`'s public unsupported guard. Public Open and Save remain
unsupported pending loader count propagation and paired Save integration.

## Tests and limits

Updated `tests/mdt/test_simctrl_ppv_binding.cpp` to exercise the actual
Operation and Project owners: tolerant first-match reuse and in-place flag
insertion; appended root record fields and zero KEY value; local routing when
there is no predecessor; and open-failure preservation of all three owners.
The test was not built or run, as requested.

The call-local adapter reproduces list and flag outcomes for normal completion
and parser row/count failures. It is not a native object alias and makes no
claim about exception/allocation-failure ordering. Original OPI and DB-root
cleanup ownership remains the evidence gap recorded in
`analysis/resume54-ppv-owner.md`. No public loader/Save, CMake, header, GUI, or
mutable REA change was made.
