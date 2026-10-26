# CP124 remesh handler and outcome frontier

## Finding

The retained source still establishes a useful bounded effect before the
uncertain native handler outcome: enabled remesh Save calls `fopen("wt")`,
passes its unchecked result to the first `fprintf`, and the null-FILE branch
sets the module CRT `errno` to `EINVAL` (22) before dispatching the module's
invalid-parameter handler. Under the retained fresh-default/no-replacement
assumption, the native handler then clears a module-owned dword, captures and
virtually unwinds a copied context for reporting, calls the process top-level
filter, and requests `TerminateProcess(..., 0xC0000417)`. These are static
control-flow facts. There is still no same-run original process capture of
the handler slot/context/filter outcome/status on the constructed B133
overlay. Candidate worker/CLI status receipts do not fill that gap.

I found no smaller newly source-closed production effect beyond the already
modeled errno and ordered handler prefix. Keep native failed-open behavior
guarded at the public adapter boundary until an original-process witness
binds the actual occurrence and its terminal result.

## Reconciled retained evidence

The governing original binary is `evidence/original-binaries/DEF_GUI_MO_64.exe`,
SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
`analysis/resume103-remesh-terminal-outcome-architecture.md` and
`analysis/resume109-remesh-handler-prelude.md` bind the Save/writer/CRT
dispatcher/default-handler path. `analysis/resume110-remesh-handler-model-implementation.md`
describes the candidate pure model, and
`analysis/resume116-remesh-invalid-parameter-occurrence.md` rechecks the
original caller ordering and unwind metadata without claiming runtime
execution.

The retained static path is:

1. Save checks the shared remesh enabled state and calls writer
   `0x1400abcc0` after project, property and MST attempts. The writer uses
   relative `LOCAL_REMESH.DAT` and calls `fopen("wt")`.
2. The unchecked `FILE*` reaches the first `fprintf`. For null, the CRT
   invalid-parameter branch stores `0x16` (`EINVAL`, decimal 22) and dispatches
   through the EXE's encoded handler slot at `0x1446ffb40`.
3. Startup stores encoded null in that slot before `main`. The retained setter
   scans do not prove there was no later replacement. Only under the
   fresh-default/no-replacement profile does the dispatcher take the default
   handler branch. It first clears module dword `0x1450540e8`, then captures
   a local context and uses `RtlLookupFunctionEntry`/`RtlVirtualUnwind` or its
   alternate caller-slot projection. This virtual unwind edits a copied
   context used for the exception report; it is not a live Save-stack unwind
   and does not run C++ destructors.
4. The handler constructs exception code `0xC0000417`, flags it
   noncontinuable, records the original caller return address, checks debugger
   state, calls `SetUnhandledExceptionFilter(NULL)`, then calls
   `UnhandledExceptionFilter`. All ordinary returning filter paths join
   `GetCurrentProcess` and `TerminateProcess` with status `0xC0000417`.

The ordered calls and stores justify a *conditional source prediction* of
first-format-attempt, EINVAL store, module dword clear, copied-context/filter
prefix, and terminal request. They do not establish actual current handler
slot contents, captured RIP/RSP/stack values, which unwind branch ran, filter
return behavior, debugger/error-mode state, whether the filter returned, or
the OS process status. A custom/nonreturning handler or filter and failure of
the termination API are separately unresolved. Arbitrary failed-open residue
is not established; only the controlled write-sharing denial used by a
disposable fixture can justify an unchanged remesh sentinel before truncation.

## What the retained executions prove

CP108/109 terminal-worker receipts and CP111 isolated-CLI acceptance are
candidate-owned executions on a constructed overlay. They show that the
candidate worker can publish its Boundary record, take its fixed endpoint,
and be observed with the requested status under its child protocol. The
candidate endpoint deliberately omits native context capture, virtual unwind,
debugger/filter calls and the original module-owned state. Its status and
no-unwind marker are not original-process observations. The historical B133
input itself has no remesh files and no recorded enabled-remesh Save
occurrence, so it cannot supply an original status comparison.

There is also no retained original receipt proving that the constructed B133
overlay reached the null-FILE boundary. The overlay and source order show how
to prepare a safe denial, not that the original loaded its fallback, retained
enabled state through the event loop, saved with the same current directory,
or encountered the expected handler.

## Exact remaining witness

The smallest decisive original experiment is one fresh, owned original
process over a disposable B133-derived copy, with a deterministic
pre-truncation write denial on the exact primary `LOCAL_REMESH.DAT`. A
single-run trace must bind:

- original executable hash and loaded module identity/base;
- overlay manifest and primary/fallback bytes, plus the parent's denial
  handle lifetime;
- successful Open, remesh enabled state at the Save call, and Save-time CWD
  resolving the writer's relative path to that locked primary;
- the actual null `fopen` result and first `fprintf` FILE argument;
- the same module's decoded invalid-parameter slot at dispatch, handler
  entry, module dword clear, captured context RIP/RSP, lookup result and
  selected unwind branch;
- debugger state, top-level-filter replacement/return behavior, reach of the
  `TerminateProcess` call and the observed child OS status; and
- the absence of subsequent writer/Save/Close continuation only after that
  exact terminal occurrence is recorded.

The probe must preserve the natural process outcome and record its evidence
from an external supervisor. It must not install a replacement handler or
filter merely to make the run succeed, and it must state any debugger/filter
perturbation introduced by instrumentation. Until that witness exists, adding
more pure projections or reusing candidate `0xC0000417` receipts would repeat
known static predictions rather than close the native outcome gap.

No source/build/test/GUI/original-execution/mutable-REA action occurred for
this read-only review.
