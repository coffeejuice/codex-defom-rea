# CP196 RFORMAT successor selection after CP166–194

## Decision

No source-ready numeric, encoding, or path behavior survives the requested duplicate and ownership checks. The smallest evidence-backed numeric candidate remains the exact next-token witness for the single extraction from `SW2SP.DAT`; no unseen spelling is admitted. The CP165/CP171 frontier remains valid after CP166–194: intervening work is connector integration, R300 operation behavior, and adapter Unicode diagnostics, not a new original numeric/file-format observation. CP173's UTF-8 path test concerns conversion of an already-wide path in a missing-file diagnostic and does not establish original file decoding or parser behavior.

CP195 connector behavior and the L6 alias are excluded by ownership as requested. No attempted reallocation of either area is proposed.

## Current coverage and qualification

- `analysis/resume165-numeric-next.md` and `analysis/resume171-rformat-next.md` establish that the reachable `SW2SP.DAT` reader requests one integer, ignores stream status, and returns after one extraction. They identify the missing evidence precisely: one same-occurrence original trace for a token outside the finite CP84 oracle, including raw bytes, stream state/branch, converted value and status, unread cursor, and caller destination.
- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` routes the admitted one-value `SW2SP.DAT` profile through `detail::parseQt4Base0Int`. `tests/mdt/test_simcontrol300_native_io.cpp` has the bounded base-0 oracle, including comma-prefix cases, suffix handling, filename/count gates, radix and overflow behavior. A new literal without an original trace would broaden the oracle speculatively.
- The CP162 SPRING2 BOM and BMP decimal-digit behavior is already implemented in its separate private reader. The retained qualification does not establish BOM-less UTF-8/ACP decoding or arbitrary Unicode digit handling. CP173 ABI Unicode tests only verify equivalent UTF-16/UTF-8 API paths for a missing project; they are not evidence for a file-format parser.
- The CP164/CP122 ShapeOptim rooted `studyFolder` construction and `mkpath` attempt are implemented and tested for the admitted owner profile. The false-mkpath modal continuation remains unqualified; no later CP166–194 note inspected supplies that same-occurrence completion witness.

## Next witness / contract

For numeric work, acquire a same-occurrence original `SW2SP.DAT` observation of one exact ASCII byte sequence not already in the CP84 oracle. Bind the original GUI/QtCore image identities, caller path and count=1, fresh stream state, bytes, selected getter branch, extracted value/status, unread QChar or cursor, and final caller destination/return. Only a demonstrated new caller-visible result justifies a parser row and a narrowly gated regression using those exact bytes.

For encoding, any extension requires a same-process original observation tying startup codec identity to raw bytes, decoded QChars, extraction result/status, unread position, and caller result. For ShapeOptim failure behavior, the missing item remains a single occurrence tracing false `QDir::mkpath` through actual QMessageBox delivery/dismissal to continuation. Neither has a source-ready fixture today.

## Read-only boundary

Reviewed the CP165 and CP171 frontier notes, available CP166–194 format/Unicode-related notes, current parser and adapter tests, and the CP162/CP164 qualifications referenced above. No tests, build, original application, GUI/app-control, or live REA session was accessed. This note is the only write; source, tests, fixtures, build state, and shared runtime state were not changed.
