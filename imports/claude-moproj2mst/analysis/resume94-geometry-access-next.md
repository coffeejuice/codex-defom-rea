# CP94 proposal: signed actual directory-index scalar borrow

The next safe source cut is `qualifiedGeometryReadiness`'s S.u740 read into
its local directory index. Use a short void withStep callback to copy that
signed scalar at the existing position. The helper cannot return integers;
copying into an existing stack value avoids widening its policy or retaining
a native pointer. This is read-only preparation while CP93 is frozen.

## Proposed source and exact lifetime

Own only viewer.cpp's local index acquisition and this report if root assigns
implementation. Existing viewer_box tests are sufficient unchanged. No helper,
header, CMake, native DB/query/layout, public ABI or frame identity changes.
Wait for root's explicit freeze/release before editing source.

Replace `const auto index = model->step->u740;` in place with:

```cpp
std::int32_t index = 0;
GuiModelAccess::withStep(*model, [&index](DB_STEP_DATA* nativeStep) {
    index = nativeStep->u740;
});
```

The callback captures only that local scalar by reference, assigns it on every
normal return and ends before any bounds or record access. Its zero initializer
does not become a native default or failure fallback: a preflight exception
leaves the function before index is consumed. Keep the native int32_t type and
all subsequent comparisons unchanged. Existing code performs no later index
mutation; removing local const adds no native/model/ABI effect.

## Actual owner and native effect order

The current order is actual node/element owner qualification; admitted byte/
dimension/version profile; real COW/global table/bounds/cache prefix; miss
timers; actual S.db presence; currentProject's strong dataDb lease and exact
model.db/reader identity checks; `receipts::current(dbOwner.get(),*reader)`;
selected scalar read; existing combined receipt/negative-index/physical-bound/
current-generation guard; record-version read; bounded query or pending frame.

Preserve the scalar read after receipt lookup and before `!receipt`, even when
receipt is absent. Do not hoist it, capture it earlier, skip it on a new receipt
short-circuit, move signed-negative rejection after size_t conversion, or change
record/query/barrier order. The actual model/step has already been qualified;
the callback does not acquire a new native owner or reader lease. It retains
the existing strong DB owner and synchronous/no-event prefix assumptions.

`analysis/resume71-geometry-cache-miss-loader.md` qualifies the complete native
directory query0x140825440..0x140825532, native-byte SHA-256
`4525b4f95fe70043fd1c70d20acced398afb237fa0b8331a12d0da131cd00ac3`.
Original DEF_GUI_MO_64 SHA-256 is
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The query indexes companion+0xad8 records with actual S+0x740 times0x28;
its explicit step argument is ignored. The existing source counterpart
`source/port/defdbio/src/dbread/w2_objarrays.cpp:506`, marked @src1401cc1a0,
uses m->u740, reads hsize/mar_size/a_size/version and conditionally writes
actual step_version afterward. No directory/array allocation is introduced.

CP77's owner/extent contract requires signed actual index>=0 and below the
actual live allocation receipt's elementCount, not logical reader.uac8,
operation ID or DB step number. Preserve that distinction and the current
query's partial version write/pending behavior. Keep all public parked gates
and newly pending returns; external raw mutation cannot retire the barrier.

## Meaningful existing tests

Use m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box, currently45 cases unchanged.

- `geometry_nonnull_directory_query_bounds_use_actual_extent_not_logical_count`
  checks actual indices-1/32/INT32_MAX with logical33/physical32, then permits
  actual31 with logical1/physical32. This distinguishes signed rejection and
  real extent from a copied logical bound or operation/step-number substitute.
- Actual-owner high/low directory-record tests check exact version/query and
  parked-frame behavior after the same scalar read.
- Reload/multipart and failed-reload tests retain current reader/allocation
  generation and physical bounds, including logical33 with actual32 after a
  failed read; missing/foreign/expired/unobserved owner tests retain earlier
  guard behavior. Parked retries preserve native state and query prefix.

Those independent bounds/partial-effect expectations already cover the selected
read. No mirrored scalar-access test, new native stub or public hook is needed.
Root owns runtime execution and broader source-stable validation.

## Scope boundary and static/ABI gates

One scalar read is safely migratable now. Pending frame->step deliberately
stores a persistent native identity alongside an observer; assigning withStep's
borrow to it would escape the synchronous contract. That path requires an
explicit observer/identity API and lifetime review, and remains excluded.
This scalar snapshot already flowed into frame->queriedIndex; preserving that
existing value flow introduces no native pointer lifetime.

For implementation, reverse only the local acquisition replacement and require
the whole starting viewer.cpp hash. Inspect exact prior receipt and later
signed/physical/current-receipt/record/query/frame order; one u740 read in the
void callback; capture only &index; no nativeStep escape or native write; no
new integer callback-result policy. Other raw accesses remain explicit legacy.

Helper/model/public headers and C ABI source stay unchanged. Retain existing
bool/void and pointer/reference rejection, native size/offsets, GuiModel step
type/nonpolymorphism, APIversion1 and six C signature compile gates. Root owns
build/tests/export/ABI/archive acceptance.

## Read-only starting SHA-256 bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | 33f209956f40e5e0dfdd8c64a8182a78cb9619529d387ac05928546caae30703 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| analysis/resume77-geometry-directory-query-binding.md | 5b0664d813ab0ddc97b17b942074fd2cf4e22264b7431917965bf247c13f2e27 |
| analysis/resume71-geometry-cache-miss-loader.md | 9ff0bc3243382220d4964e5fb4ff0a43a9ac64c91ef97b81b449dc7226f54c7b |
| source/port/defdbio/src/dbread/w2_objarrays.cpp | c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359 |
| source/port/defmodel/include/defmodel/layout/db_step_data.hpp | 9070ac47f16163b68451569a8f40e22bc8dcbf0ac3b73b7da19bdabc75092617 |

Worker wrote only this report. No source/build/CTest/GUI/mutable REA action.

## Authorized implementation and static release

Plan freeze SHA-256:
`7962f3389289c6c6b4150fe6338034b622337605d400551735db3e6932fbbf5d`.
After CP93 freeze, root authorized viewer.cpp's selected index acquisition.
It now initializes a local signed int32_t and always assigns actual S.u740
through the short void callback before that local is consumed. Only &index is
captured; the native pointer does not escape and no model field is written.
Receipt lookup remains before the borrow; original !receipt, negative index,
physical extent, current-generation, record/query/frame checks remain after it.
No helper result-policy or durable frame identity change was made.

Static preflight passed: one selected native scalar read, correct signed member
type, void callback and exact surrounding owner/receipt/bounds/query/frame order.
Reversing only the local acquisition replacement restores the complete CP93
viewer.cpp SHA-256. Existing45-case viewer test and helper/model/C ABI source
hashes remain unchanged. Root owns all runtime/build acceptance.

Final SHA-256 receipt:

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/viewer.cpp | aee911933a4d05fb4dbf7ddb98f7c5dde6147fdd500e1434a9ad6e5baa286b49 |
| source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp (unchanged) | 58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98 |
| source/apps/moproj2mst/src/engine/gui_model_access.hpp (unchanged) | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| source/apps/moproj2mst/include/m2m/engine.hpp (unchanged) | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |
| source/apps/moproj2mst/include/moproj2mst.h (unchanged) | 34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40 |
| source/apps/moproj2mst/src/dll/abi.cpp (unchanged) | 6bf5e8bb306461440b7ba8019233449a3b2b133081f73a02430e0f06d3fca848 |

Source is frozen/released for root's m2m_mdt_viewer_box / CTest
m2m.mdt.viewer_box and grouped source-stable validation. Worker performed no
build, CTest, GUI or mutable REA action.
