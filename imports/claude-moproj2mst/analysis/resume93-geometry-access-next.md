# CP93 proposal: leased-reader identity comparison

The next safe short borrow is the existing comparison between the actual
DbHandle-owned reader and S.db in qualifiedGeometryReadiness. Keep its
null-reader shortcut and the already pinned DB owner. A bool callback may
compare identities without returning/retaining nativeStep or S.db. No new
mutable owner, reader lease or helper policy is required for this cut.

This is read-only preparation while CP92 is held. Do not edit source before
root confirms freeze and assigns implementation.

## Exclusive source cut

Only viewer.cpp's existing reader mismatch expression and this report are
needed. Reuse unchanged viewer_box tests. No helper/header/CMake/native layout,
ABI, DB receipt/lifecycle, Boolean/format or subsequent query/frame edits.

Replace:

```cpp
if (!reader || reader != model->step->db)
```

with:

```cpp
if (!reader || GuiModelAccess::withStep(*model, [reader](DB_STEP_DATA* nativeStep) {
        return reader != nativeStep->db;
    }))
```

Keep the existing pending return as its body. Null reader must short-circuit
before the callback. Capture only the already resolved peer reader by value;
do not capture/return a step/companion pointer or introduce a delayed callable.
The callback runs synchronously, reads actual S.db once and returns mismatch.
The existing local shared_ptr dbOwner stays in the same surrounding scope
through receipt/query/frame work. Its unique reader has not been replaced by
an event or unknown callback between resolution and comparison. Existing
serialized/non-reentrant lifetime assumptions remain, without widening them.

## Source/native qualification and exact order

`analysis/resume77-geometry-directory-query-binding.md` qualifies the actual
callback ownership chain: View.ownerModel observer resolves the live GuiModel;
GuiModel.db is raw; currentProject's actual ProjectScope supplies Project.dataDb;
copy that shared owner before touching the raw DB identity; require equal raw
handle identity; obtain its unique reader; require nonnull and exactly equal
to S.db before dereferencing it for receipt/query work. Preserve that adapter
provenance gate. It is not a newly discovered native owner-comparison branch.

Native field provenance remains CP70/71's original S+0x3390 companion pointer
load/gate in0x140828083..0x1408280aa, native-byte SHA-256
`4d8807237793a6aac3af8b9721183a84da7f311dbc2c46899aea8297522132cc`.
The original target SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The source member is Cls_1401dbfe0* DB_STEP_DATA.db at+0x3390. Comparing the
pointer must not dereference an arbitrary S.db to find ownership.

Preserve current order: actual owner/byte/dimension/version; real COW/table
selection and index bounds; cache zero; actual node Start/checkpoint; CP92
S.db presence; currentProject; local Project.dataDb lease; nonnull/same model.db
identity; resolve reader; null-reader shortcut and selected mismatch check;
then current allocation receipt, signed physical index/bounds, record/query
and optional pending-frame effects. No condition is hoisted or cached across
these effects. The short borrow ends before any receipt/query work.

Public parked viewer entry guards and newly pending returns remain unchanged.
This read is reached only on an unparked invocation before later frame
installation. Existing null/non-null DB branch and timer partial effects stay
intact. Public raw fields/preheld pointers cannot retire the pending barrier.

## Existing meaningful tests and limits

Use m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box, currently45 cases, unchanged.

- Actual-owner high-version directory queries exercise matching reader/receipt
  and preserve pending/partial effects after the bounded query.
- `geometry_nonnull_directory_query_rejects_missing_foreign_expired_and_unobserved_owners`
  covers eight profiles. Its mode5 supplies a forged nonnull S.db, which must
  be rejected by identity before receipt dereference; mode6 replaces the actual
  owned reader and supplies it to S.db, but still lacks a qualified allocation
  receipt. Missing scope, expired/foreign handle and unobserved allocation
  profiles retain their existing earlier guard behavior.
- Bounds, live reload/multipart and failed-reload cases check the actual
  current reader/receipt generation. Null companion and cached rows retain
  their bypass behavior; parked owner/retry tests preserve pre-effect refusal.

These tests establish useful matching/mismatch and lifetime-boundary coverage.
There is no separate direct null-owned-reader runtime row in this eight-mode
fixture; preserve that already explicit left short-circuit statically rather
than invent a new reader owner/API just for this expression migration. No
runtime execution is claimed by this investigation.

## Scope boundary and later blockers

No blocker prevents this identity-only bool borrow. Later scalar index reads
may fit separate void callbacks. The pending frame's deliberate retained step
identity is a different architecture boundary: assigning the borrowed step
into frame->step inside withStep would escape the current synchronous borrow
contract. It needs an explicitly qualified observer/identity API and lifetime
review, not a pointer-return policy expansion or incidental callback capture.
That durable frame path is excluded. modelDim's const helper is also excluded.

## Static, layout and ABI gates

Reverse only this comparison replacement to restore the complete starting
viewer.cpp SHA-256. Require unchanged !reader short-circuit, exactly one
selected S.db comparison, capture of only existing reader, no companion
dereference/nativeStep escape/null fallback, and exact prior lease plus later
receipt/query/frame order. No whole-function raw-free claim.

Helper/model/public headers and ABI source stay unchanged. Retain void/bool
callback policy, rejected pointer/reference results, DB_STEP_DATA size/offsets,
GuiModel step type/nonpolymorphism, APIversion1 and six C signature compile
gates. Root owns source-stable build/tests/export/ABI/archive acceptance.

## Read-only starting SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | b94efd57a7beb810b7d73498eb43f0f5c52aba320150ca9dd07aa5d11f4cbf38 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| analysis/resume77-geometry-directory-query-binding.md | 5b0664d813ab0ddc97b17b942074fd2cf4e22264b7431917965bf247c13f2e27 |
| analysis/resume70-geometry-id0b-reader.md | 250a77926b272ada42a4a126533f23b7915064febd20766afdef7f830ab480a2 |
| analysis/resume71-geometry-cache-miss-loader.md | 9ff0bc3243382220d4964e5fb4ff0a43a9ac64c91ef97b81b449dc7226f54c7b |

Worker wrote only this report. No source/build/CTest/GUI/mutable REA action.

## Authorized implementation and static release

Plan freeze SHA-256:
`77284b38d2e2f4ed0765a4300e5175aad9ee6cba616e8dee4035a1424624f42c`.
After CP92 freeze, root authorized only viewer.cpp's selected reader mismatch
expression. It now preserves `!reader ||` and invokes the bool callback only
for a nonnull already resolved reader. The lambda captures that peer pointer
by value and compares it with actual nativeStep.db; neither step nor companion
pointer leaves the synchronous call. Existing dbOwner remains in the original
scope and all later receipt/query/frame statements remain unchanged.

Static preflight passed: unchanged left short-circuit; one selected identity
comparison; exact prior timer/presence/Project/strong-owner/reader and later
receipt/index/frame order; no companion dereference or retained nativeStep.
Reversing only the comparison replacement restores the full CP92 viewer.cpp
SHA-256. Existing45-case viewer tests and helper/model/C ABI sources are
byte-for-byte unchanged. Root owns runtime/build acceptance, including the
explicitly documented direct-null-reader runtime coverage limit above.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 33f209956f40e5e0dfdd8c64a8182a78cb9619529d387ac05928546caae30703 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen/released for root's m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box and grouped source-stable validation. Worker performed no
build, CTest, GUI or mutable REA action.
