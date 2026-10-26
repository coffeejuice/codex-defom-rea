# CP203 ROPS: operation/property dispatch static audit

## Result

The CP201 candidate is the correct current baseline for this converter audit: `analysis/resume201-root-integration.md` records the frozen candidate and source digest `2e90ced71ce9e116259a45104df0ceffb2bdd8b94d2a0167f10c0b39636df1de`. I compared the relevant files in its `build/wave3-checkpoint201-verified/source.zip` with the checkout; all three are byte-identical:

| CP201 source member | SHA-256 |
|---|---|
| `apps/moproj2mst/src/doe/frcrot_table_prefix.cpp` | `b20c309704cf7e512e2bc0c57afaa8adaeadb550f4b90ddb24ea24663aa664a9` |
| `apps/moproj2mst/tests/doe/test_frcrot_table_prefix.cpp` | `27b1dab1b68e0393af64ef4cff652dfd3fb559e92da79309ff14c6417e31ac5f` |
| `apps/moproj2mst/src/doe/keyword_reader_prefix.cpp` | `2f16a73723bf400121e66d66e671a5fc258c480bbb920e5dfe4fc03d8df8da2e` |

The narrow ROPS gap remains the actual Type2 FRCROT text-file/scanner dispatch occurrence. The retained original evidence proves the static branch and its ordering, but there is no qualified same-occurrence runtime witness for the relative `temp.KWD` identity/CWD, QFile mode/result, QTextStream bytes/codec, FILE reopen, `%39s` return/cursor, or group-3 parser result. Without that tuple, there is no source-ready behavior to promote from the private CP161 prepared-state projection into reachable operation dispatch. This audit does not propose another helper or repeat the historical Type3/ShapeOptim closure work.

## Evidence and source boundary

CP201 is frozen with inert QtCore TLS successor-effect descriptions only; no TLS edge executes and this says nothing about FRCROT. In the ROPS lane, CP161's `applyFrcrotTablePrefix` remains a private pure projection over prepared observations; `rg` shows it is only referenced by its dedicated unit test. CP164's retained native/static audit pins the real converter call at GUI VA `0x14091111d` into `0x140916530`. The all-nonnull result slots at `0x140911146..0x140911165`, rather than the converter return value alone, gate the later setters.

The earliest missing occurrence is inside the converter: QFile::open at `0x1409165d8` (mode `0x12`) and its success-only QTextStream output at `0x140916607..0x140916612`; then relative `temp.KWD` is reopened via `FILE*` at `0x14091668a`, scanned with `%39s` at `0x1409166b1`, and supplied to group-3 parser `0x140b667a0` at `0x140916756`. A false initial open stops at the error path, so downstream scanner evidence must not be fabricated for that case. Upper/lower phases are conditional on the prior parser return.

The CP164 evidence specifically excludes using `doe_study.cpp::keywordValues` as a substitute: its project-relative path, host write/scanner, per-phase lifetime and nondefault-handler rejection do not reproduce the original call. Prepared scan return codes also cannot establish whether the native scanner returned EOF, format failure, or another error for the actual file.

## Contract for resuming this gap

Before any implementation change that claims reachable dispatch, retain a single original invocation bound to its process/run/thread, input project hash, loaded GUI/QtCore identities, and actual working directory. For that same invocation and file identity, record:

1. The reached QFile open mode and return at `0x1409165d8`; on success, stream codec/state and exact file bytes after close at `0x140916612`.
2. The `FILE*` reopen result, identity and starting cursor at `0x14091668a`, followed by actual `%39s` result, destination bytes and cursor at `0x1409166b1`.
3. Entry state, dimension and real scanner sequence for parser `0x140b667a0` at `0x140916756`, plus return/status and partial table effects. Observe upper/lower only if the preceding native parser succeeds.

Once that evidence exists, a candidate change must wire only the observed file/scanner stage through the existing `keyword_reader_prefix.cpp` owner, preserve the native three-result commit gate, and return unsupported before any unobserved continuation. The independent test must assert path/codec bytes, reader cursor/return, parser state/status, and no later table publication on each failed predecessor. Freeze the candidate and fixtures before original comparisons and retain a held-out occurrence. Until then, the correct implementation contract is to keep the real dispatch unsupported at the file boundary.

The standing app-control gate in `analysis/gui-control-lifecycle-assessment.json` has no supported release operation plus verifiable receipt/status, so this read-only audit did not initiate GUI or original-process capture. No source, tests, build, fixture, GUI, original process, or mutable REA state was changed.

## Sources

- `IMPLEMENTATION_PLAN.md` (CP201 frozen / CP203 ROPS permitted alongside blocked runtime lane)
- `analysis/resume201-root-integration.md`
- `analysis/resume195-ops-next.md` (earlier ROPS disposition; source owner and witness boundary rechecked here against CP201 archive)
- `analysis/resume164-frcrot-file-next.md` and its cited native static captures
- `analysis/wave3-checkpoint201-validation.json`
- `source/apps/moproj2mst/src/doe/frcrot_table_prefix.cpp`, `source/apps/moproj2mst/src/doe/keyword_reader_prefix.cpp`, and their tests

