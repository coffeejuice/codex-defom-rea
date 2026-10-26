# CP214 RFORMAT resume: retain the current evidence boundary

## Decision

No independently supported format, encoding, or numeric increment is ready from the reviewed CP200/CP214 queue. The SPRING2 unsigned narrowing boundary is already in source and its focused regression; the remaining smallest actionable witness is still one previously unobserved exact token at the original one-value `SW2SP.DAT` extraction. Until that original occurrence is captured with its caller-visible outcome, leave parser behavior unchanged.

## Reconciliation and current source

CP200's `resume200-format-source-ready.md` already closes the earlier CP123 defect: the original getter's positive decimal `4294967296` is accepted as a wide value then narrowed through `int&` to zero. Current `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` contains the SPRING2 reader implementation; `tests/mdt/test_simcontrol300_spring_reader.cpp:389-400` accepts `4294967296` as flag zero and checks adjacent `4294967297`→1 and `18446744073709551616`→0. Reopening this is duplicate work.

Current `simcontrol300_native_io.cpp:175-204` documents the bounded Qt4 base-0 integer profile, and lines 666-675 gate it to the one-value `SW2SP.DAT`/`STRETCH.DAT` route while preserving ignored stream status. `tests/mdt/test_simcontrol300_native_io.cpp:300-325` and `585-601` retain parser and comma-prefix/delimiter cases. CP171/200 already identify the correct next action: select a literal absent from CP84's finite oracle and observe it at the actual original `FUN_1410da690` one-integer call. Existing candidate unit cases are not original evidence and cannot establish that a new token is accepted.

Encoding is not an independent source-ready alternative. Explicit SPRING2 BOM and BMP digit profiles do not establish BOM-less UTF-8 or ACP behavior; that requires a same-process binding of startup codec identity, raw bytes, decoded QChars, extraction/status, unread cursor and caller result. ShapeOptim's rooted path and successful `mkpath` profile are also already accounted for; false-`mkpath` continuation still lacks the same-occurrence message delivery/dismissal trace and does not imply a format change.

## Next evidence action

Use the CP84 inventory to choose the first exact ASCII token not already captured. At the original one-value SW2SP reader occurrence, retain original GUI/QtCore image hashes, actual caller path and count, fresh stream state, exact bytes, selected getter branch, extracted value/status, unread QChar/cursor, and final destination/return. Implement only a newly observed caller-visible distinction under the existing one-value gate, with a matching exact-byte regression. If no distinction appears, keep source unchanged.

## Boundary

Read-only reconciliation of the subtree instructions, CP200 source-ready note, CP171/208 frontier, current parser and tests. No source, test, fixture, build, original process, GUI/app-control, or mutable REA state was touched. This file is the sole write.
