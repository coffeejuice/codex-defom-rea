# CP165 numeric/encoding/path API mismatch: no new source cut

## Decision

No additional source-ready mismatch exists in the requested numeric-conversion, encoding, or path-API area outside CP162 SPRING2 and CP164 current files. The narrowest candidate previously visible, the one-value `SW2SP.DAT` comma-prefix case (`2,345` → 2 with comma unread), is already implemented and tested. Existing hex/radix/numeric cases are consolidated in the bounded Qt 4 base-0 parser. The CP164 ShapeOptim `studyFolder` path construction and `QDir::mkpath` effect are likewise implemented for their admitted owner profile. Do not add another literal parser exception, detached helper, or duplicate test.

The exact next witness for a nontrivial remaining numeric extension is a **same-file, one-value original `SW2SP.DAT` trace for a comma-separated spelling outside current oracle rows**, binding the original `QTextStream::getInteger` extraction result/status and unread cursor at the comma, plus confirmation that `FUN_1410da690` ignores status and returns after its single extraction. A generic statement that commas may delimit or a locale record showing comma as group separator is insufficient to admit arbitrary grouped forms. Until such a witness identifies a new caller-visible result, keep the current exact oracle/profile and refusal boundary.

## Evidence checked

- `analysis/resume73-sw2sp-next.md` and `analysis/resume75-sw2sp-next.md` source-qualify comma as an unconsumed delimiter under the default stream's zero number-options; `1,234` and `2,345` are the specific parser examples. CP84 incorporated the comma-prefix behavior in the one-value parser oracle.
- `analysis/resume84-sw2sp-parser-plan.md` records the corrected bounded one-value oracle, including `1,234` → 1 with comma unread, alongside radix, prefix-failure, signed-decimal and overflow distinctions. `analysis/resume85-sw2sp-next.md` confirms production routing is already through that parser only for the reachable one-value `SW2SP.DAT` call.
- Current `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` SHA-256 is `1f3df812602c0cda75b394750ffc523a2cf938e01cdb32b01449f4a032ef5313`; its `readInts` route dispatches that one-value filename to `detail::parseQt4Base0Int`.
- Current `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp` contains the finite parser oracle and a native adapter regression for `1,234` (lines around 581–599). The source-level current identity matches CP162's reported frozen source identity for this file; this read-only pass does not claim a new build or execution.
- `analysis/resume164-format-next.md` says the ShapeOptim rooted `DoeStudies/<name>/Control/Opt/` construction and `mkpath` attempt timing are already implemented/tested for the supported owner profile; the false-mkpath modal-delivery continuation remains guarded pending its same-occurrence original witness.
- `analysis/resume67-sw2sp-locale.md` binds the original QtCore default stream to C locale and the actual SW2SP one-int call. `analysis/resume86-sw2sp-next.md` rules out multi-value SW2SP expansion as a reachable successor absent a real multi-value caller.

All binary addresses, locale data and behavior statements above are limited to those retained source qualifications. No fresh original-process observation is claimed.

## Required witness and guard

If numeric work resumes, select an exact ASCII byte sequence not already in the CP84 oracle and capture it from the actual original `SW2SP.DAT` reader occurrence. Preserve original GUI/QtCore image hashes, caller path/count, stream fresh-state, exact bytes, getter branch, conversion status, unread QChar/cursor state and caller destination on return. The next parser change, if any, must add only the first newly discriminating result under the existing `values.size()==1 && path==SW2SP.DAT` route; it must not generalize comma grouping or affect SPRING count parsing.

For path work, the CP164 missing witness is separate: one same-occurrence original false `QDir::mkpath` return through the constructed QMessageBox and dismissal to actual continuation. No static path formula or candidate simulation establishes that modal-delivery behavior. Keep the failure continuation guarded.

## Guarded tests / exclusions

Preserve CP84's exact one-value SW2SP model, ASCII/path/count gates and all unsupported grammar cases. Do not re-open CP162 SPRING2 Unicode parsing, CP164 ShapeOptim path construction, multi-value SW2SP, or CP164's assigned connector/FRCROT/RIO files. No new test is justified without the missing witness above.

## Read-only receipt

This note is the sole write. No source, test, fixture, shared plan/state, build, GUI, application, or live REA data was changed or run. No live REA session or app-control session was acquired. No reconstructed executable or deployment is discussed.
