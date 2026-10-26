# CP72 actual cache-miss timer prefix before DB loading

Qualified high-version cache misses now update the actual process-global
node-reader StepTimer with Start and ReadSingleAArrayFromDB checkpoints, then
remain stopped at HighVersionDbLoaderPending. No DB-loader call, cache/body
write, later arrays-timer checkpoint or public cache-miss success is enabled.
The real COW/table/timer effects are retained in production.

Exclusive edits are `source/apps/moproj2mst/src/engine/viewer.cpp`,
`source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` and this report. No shared
timer/model/helper/CMake edit, build, CTest, GUI or mutable REA action was
performed. Source is frozen for root validation. Token measurement is unavailable.

## Actual owner and native qualification

`analysis/resume71-geometry-cache-miss-loader.md` retains the original/tool
identities, full timer method/initializer hashes, exact fields/labels and DB
mutation limits. Original GUI SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Native global143DBE000 is constructed with the node-reader name and flag0 by
141d32af0. It maps to the existing actual source global
`g_timer_read_obj_node_state_variable`, not a new local timer or a copied owner.
Native reader14082d7d0 calls start("Start") at82d895 and
checkpoint("ReadSingleAArrayFromDB",nullptr) at82d8ab before DB-loader82d8c0.
The source StepTimer methods have the verified count/label/time/sub layout and
same reset/checkpoint order. The actual module lifetime already owns this global;
no new observation/default binding is required or fabricated.

Timer reset changes only count; the two checkpoints set count2, write their
labels/null subs/real clock samples in slots0/1, and preserve unused slots,
name and flag. The source continues using its existing real host CRT clock
profile. Native/source clock epochs are not asserted identical; no timestamp is
invented, clamped or replaced with a constant. CP71 documents that external-clock
qualification separately from the deterministic state/ordering contract.

## Source behavior

The previous combined pending check is split: invalid actual MAR header index
or signed id limit remains HighVersionReaderPending with no timer change. Only
after valid bounds and actual step.u26c4[0x0b]==0 does the helper call the actual
global timer's start/checkpoint, then return HighVersionDbLoaderPending. Cached
nonzero markers retain CP70's complete normal return and do not touch the timer.

COW and actual table selection remain before this timer prefix. The new phase
still propagates rectKnown=false before builder arithmetic and selected scene
unsupported. The adapter does not read/dereference the companion DB pointer,
invoke the directory query, change actual version/cache/header lengths, allocate
A storage, call the different arrays timer, stop/report a timer or copy model
object arrays. No resumed native frame or completed reader is implied. Repeated
fresh public invocations reset/record this same entry prefix each time, as new
bounded calls; they do not append a third checkpoint or resume a fictitious frame.

All preallocation/canonical registry, N/E actual-identity, signed-version and
nonzero-byte fast, zero-count, malformed-builder and raw-fallback guards remain.
The source retains every committed partial effect rather than rolling it back.

## Focused regressions

Two new Testkit cases bring the file to31 unique cases:

- Miss-prefix state/order covers null/opaque companion DB and populated/null
  connectivity (four combinations). It seeds actual node-timer history, then
  requires one real coordinate COW traversal before table/timer mutation, final
  current dimension3, exactly two entry labels/null subs and count2. Name/flag/
  pad, unused labels/times/subs, and bytes past the two copied label terminators
  remain unchanged. All four other actual timers remain byte-for-byte unchanged.
  Real timestamp slots0/1 are opaque runtime samples, not fixed/monotonic oracle
  values. A second fresh call observes the unique body and previous count2 during
  COW, then again ends at count2 with the same retained unused history.
- Nonmiss/refused paths cover markers1/-1, invalid MAR index, invalid id limit,
  opaque N/E owner, id0 registry, missing coordinates, zero count, older-version
  fast return and nonzero-node-flag fast return (11 profiles). Every actual timer
  remains unchanged; existing success/skip/pending results and getter counts are
  asserted.

The miss case also checks companion pointer, actual step version, cache marker,
MAR/A lengths/body count/stored length and null body pointers remain unchanged.
An opaque DB pointer is restored before fixture destruction; it is never treated
as a valid companion body. Source coordinate/connectivity storage and N/E owners
remain unchanged. Public miss results remain unsupported, proving no stale/partial
timer result is presented as completed geometry.

Test-only RestoreGeometryTimers snapshots all five actual globals and restores
their existing history after owned fixtures. Copies contain borrowed sub-pointer
values only, with no pointee ownership. Index-table RAII remains test isolation.
Production has no timer/table reset-to-default or rollback beyond the original
timer start's count reset. Historical CP70/71 reports/receipts remain unchanged.

The forwarding trace now records actual node-timer count before/after coordinate
dispatch, proving timer entry is not executed before COW. The source's explicit
table-selection-before-cache/timer call order is statically qualified; tests
observe the established COW/table final effects and timer boundary without adding
instrumentation inside shared timer/initializer helpers.

Static preflight confirmed the existing timer header resolves,31 case names are
unique, Testkit entry/CHECK/REQUIRE remain active, no C assert or unsupported
REQUIRE_EQ was introduced, no trailing whitespace is present and
`git diff --check` passes. No compilation or test execution is claimed.

Root target **`m2m_mdt_viewer_box`**, CTest **`m2m.mdt.viewer_box`**. Root owns
focused/full validation. Directory-owner/index bounds, DB loading, allocations,
post-loader timers/model-object copies and native runtime parity remain open.

## Frozen source SHA-256

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `d4f07a8d301c94b12d13b0d2f63be6e7dd4843207269c70ecded2a1cb51b384b` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `90e89aa4c92f1953afa40377ab36d1a22acaefe2348d4dab80152cc9e38a5875` |
