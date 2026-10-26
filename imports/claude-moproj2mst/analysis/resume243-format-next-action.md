# CP242 RFORMAT next increment: no newly qualified source change

## Decision

No safe new encoding, numeric-conversion, or path increment is supported by the current original evidence. Keep the converter unchanged. The smallest remaining numeric candidate remains one exact token outside the finite CP84 `SW2SP.DAT` oracle, observed at the original one-value extraction with its caller-visible result. The SPRING2 `4294967296` narrowing correction is already present and tested; reopening it would duplicate completed work.

## Queue and later audit reconciliation

`analysis/takeover-state.json` marks RFORMAT active/partial, with validation incomplete. Its current `next_action` asks for a same-occurrence original SW2SP token trace outside CP84. CP200, CP203, CP208, CP214, and CP196 all reconcile to that same boundary. CP203 separately establishes that the retained DEF_PRE dispatcher evidence identifies command names and a downstream `SCRATCH.KEY` existence/read check only; it does not identify DEF_CVT3 grammar, encoding, numeric behavior, path resolution, child status, or output lifetime. The current source has no RFORMAT converter implementation to extend from that evidence.

Current code routes the one-value `SW2SP.DAT` case to the bounded `parseQt4Base0Int` profile. Its focused tests already cover the finite radix/prefix/overflow and comma-prefix examples. `SPRING2.INI` decimal narrowing of `4294967296` to zero and adjacent wrap cases are implemented in the separate reader and its test. SPRING2 BOM/BMP-digit handling and the admitted ShapeOptim rooted path plus successful `mkpath` attempt are also already implemented at their qualified limits. No reviewed CP200-and-later audit adds an original observation that justifies broadening any of those profiles.

## Exact missing witness / dependency

For a numeric change, the missing dependency is one original occurrence at `FUN_1410da690` reading one value from `SW2SP.DAT`, using an exact ASCII byte sequence absent from CP84. The witness must bind original GUI and QtCore image hashes, actual caller path and requested count, fresh stream state, exact bytes, selected `getInteger` branch, extracted value and status, unread cursor/QChar, and the final caller destination/return. A candidate-only parser example cannot establish a new accepted spelling.

For encoding, the missing dependency is a same-process link from startup codec identity through raw bytes and decoded QChars to extraction/status, unread position, and caller result. SPRING2 BOM evidence does not establish BOM-less UTF-8 or ACP behavior. For ShapeOptim failure behavior, the missing dependency is a same-occurrence false `QDir::mkpath` return traced through actual QMessageBox delivery/dismissal to continuation. Candidate simulation does not qualify that path.

## Boundary

Read-only reconciliation of the local instructions, RFORMAT queue entry, CP200-and-later format evidence, and current source/test seams. No source, test, fixture, build, shared recovery state, original application, GUI/app-control, or mutable REA state was touched. This note is the only write.
