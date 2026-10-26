# CP208 RFORMAT next step: no source-ready increment

## Decision

The CP200 SPRING2 boundary correction is already present in production source and a focused regression: the original 64-bit getter result for `4294967296` narrows to zero in the source-qualified `int&` extraction, and the reader test accepts that row. The adjacent wrap cases are present as well. Reopening this boundary would duplicate completed work.

No remaining numeric, encoding, or path gap has a newly qualified original result that supports a source change. Keep the current finite SW2SP parser, explicit SPRING2 BOM/digit profiles, and ShapeOptim rooted-path profile unchanged. Do not broaden grammar, encoding, or false-mkdir continuation from inference.

## Reconciliation

- CP203's converter-boundary audit establishes no evidence for the external `DEF_CVT3` parser, encoding, numeric grammar, path resolution, child status, or `SCRATCH.KEY` lifetime. This is separate from the in-process sidecar reader work; it does not authorize mapping converter choices into candidate parsing behavior.
- CP200 reconciled the older CP123 SPRING2 unsigned-boundary issue against current source/test. `simcontrol300_spring_reader.cpp` now contains the exact `4294967296` row and expected narrowed flag; neighboring wrap cases remain covered.
- CP171/165/196 still identify the smallest actionable numeric witness: one exact token not present in the finite CP84 oracle, captured at the actual one-value `SW2SP.DAT` reader occurrence. The candidate already gates `parseQt4Base0Int` to the one-value SW2SP/STRETCH profile, and current tests cover the admitted comma-prefix examples. No new literal is supported by existing evidence.
- Encoding still needs a same-process link between startup codec identity, exact raw input bytes, decoded QChars, extraction result/status, unread cursor, and caller-visible result. The explicit SPRING2 BOM evidence does not establish BOM-less UTF-8 or ACP decoding.
- The path lane's existing ShapeOptim `studyFolder` construction and successful `mkpath` profile are implemented. Its false-`mkpath` behavior still needs one same-occurrence trace through actual message delivery/dismissal to continuation; candidate simulation is insufficient.

## Smallest evidence action

Capture one exact ASCII byte sequence absent from CP84 at the original `FUN_1410da690` one-integer `SW2SP.DAT` extraction. Bind the GUI and QtCore image hashes, actual caller path/count, fresh stream state, raw bytes, selected getter branch, extracted value and status, unread QChar/cursor, and final caller destination/return. Select the first discriminating token based on the retained oracle inventory; do not assume a particular spelling is accepted. If the result adds a caller-visible distinction, implement only that row behind the existing one-value SW2SP gate and add its exact-byte focused regression. Otherwise preserve the current source.

This witness is the smallest actionable RFORMAT step because it can directly change one bounded candidate parser row without requiring an unqualified external converter artifact or broad runtime codec claims. No source/test change is justified until it exists.

## Boundary

Read-only reconciliation of CP203, CP200, CP196/171/165 and current scoped source/test call sites. No production source, test, fixture, build, original process, GUI/app-control, mutable REA state, or shared plan/state was modified or accessed. This resume note is the only write.
