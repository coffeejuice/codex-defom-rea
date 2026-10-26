# CP256 format, encoding, numeric, and path audit

## Result

No independent source-only implementation leaf is justified by the current format/numeric/path evidence. The most concrete previous numeric candidate, SPRING2 positive-decimal low-dword narrowing, is already present in source and test. Remaining extensions require missing original-call evidence. RFORMAT remains a separate hard blocker because the retained DEF_PRE evidence stops at the dispatch boundary and does not identify the external converter implementation.

## Reconciled completed work

- `analysis/resume200-format-source-ready.md` and `analysis/resume148-format-next-admission.md` identify the former SPRING2 gap: positive decimal values beyond signed 32-bit range narrow to their low 32 bits. Current `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` implements the SPRING2 reader path; `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` contains accepted cases for `4294967296` → 0, `4294967297` → 1, `18446744073709551616` → 0, and `18446744073709551617` → 1. This workstream is complete at its stated ASCII positive-decimal profile. It should not be duplicated or broadened to signs, leading zeros, prefixes, or malformed suffixes without new evidence.
- `analysis/resume165-numeric-next.md` and `analysis/resume200-format-source-ready.md` reconcile the one-value `SW2SP.DAT` comma-prefix parser, existing radix cases, SPRING2 BOM/Unicode profiles, and the admitted ShapeOptim rooted path construction/successful `mkpath` behavior as already implemented at their stated boundaries.
- `analysis/resume203-format-encoding-static.md` correctly distinguishes the external `DEF_CVT3` converter from DEF_PRE's dispatcher. The retained dispatcher evidence establishes command literals and a subsequent `SCRATCH.KEY` existence/read gate only; it does not reveal the converter's grammar, encoding, path resolution, numeric conversion, output lifetime, or failure behavior.

## Exact remaining evidence blockers

1. **SW2SP numeric extension:** capture a same-file original `SW2SP.DAT` reader occurrence for an exact token not already in the finite CP84 oracle. Required observation: fresh stream state, exact bytes, `getInteger` branch/result/status, unread cursor/QChar, and caller destination/return at `FUN_1410da690`. A generic locale or delimiter claim cannot establish a new caller-visible result. Until then, no parser change or new test is justified.
2. **ShapeOptim path failure continuation:** the rooted `DoeStudies/<name>/Control/Opt/` construction and successful `mkpath` attempt are already covered for the admitted profile. The false-`mkpath` modal continuation needs a same-occurrence false return, QMessageBox delivery/dismissal, and actual continuation witness. Static path construction alone does not resolve it.
3. **RFORMAT external converter:** identify and hash the authorized `DEF_CVT3` artifact first, then recover one selected format's invocation/working directory, path quoting, encoding, numeric grammar, `SCRATCH.KEY` effects, and child-failure behavior from that converter itself. The current DEF_PRE dispatcher evidence cannot support any implementation leaf here; do not infer converter behavior from the command names.

## Qualification

This audit reconciles existing receipts and source/test coverage only. No source, test, fixture, shared state, build, GUI, original process, or mutable REA database was changed or run. No fresh original observation or parity claim is made. This file is the sole write.
