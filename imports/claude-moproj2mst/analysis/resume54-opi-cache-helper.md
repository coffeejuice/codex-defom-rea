# Checkpoint54 isolated OPI+160 cache helper

Implemented only the three assigned new source/test files and this receipt. Shared Operation/model headers, actual OpClass current, destructors, public Close, CMake/build/plan and GUI/REA state were not changed. Eight tests are frozen for root validation; no worker build/test was run.

Files beneath `source/apps/moproj2mst/`:

- `include/m2m/engine/operation_info_model_cache.hpp`
- `src/model/operation_info_model_cache.cpp`
- `tests/mdt/test_operation_info_model_cache.cpp`

## Implemented contract

OperationInfoModelCache binds nonowning metadata for one actual live OPI identity and starts Unknown. An unrepresented guard result is never treated as null. `commitNativeDefaultNull(operation, sourceQualification)` models only the source-specific050550@05067b defaults event; it requires the same live OPI and a nonblank label, preserves the first receipt on repeated known-null commits, and does not execute or certify that native event. The production defaults call-site binding remains root-owned and unintegrated here.

`lookup(operation, create=false)` returns null only after that KnownNull event. Repeated noncreating lookup retains the same OPI binding/knowledge/receipt and executes no constructor, clear, delete, event or callback. Unknown lookup refuses with M2M_E_UNSUPPORTED. `create=true` with KnownNull refuses before unrecovered0c4890/SfMoItemModel/Qt construction without changing knowledge or current. No live model is manufactured; SfMoItemModelState is only a forward-declared typed result. There is no virtual-delete or allocation-bypass API.

Wrong/expired OPI identities refuse before state mutation. The helper does not own Operation, Props or model allocation and does not commit QObjectBoundary expiry. Knowledge remains a diagnostic last observation after the OPI dies; actual lookup still refuses. All calls use the caller's serialized lifecycle.

## Source qualification

Controlling audit: `analysis/resume54-opi-expiry-owner.md`. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Native054f40 tests create flag, returns current+160 on create=false, and allocates only for create=true/current-null. It allocates0x50, calls0c4890, and writes the same guard via cbc920. Exact054f40..054fdc unwind-byte hash: `2d5dae26240126418419a455d9462f2f66444ac69afed9dfe7e1c9f9dddba297`.

Constructor0c4890 installs vtable14215e7b8; RTTI identifies SfMoItemModel, distinct from GuiModel/operation manager. Its constructor/selection/helper/Qt payloads remain guarded. Native defaults050550 assigns+160 null through cbc920 at05067b. An arbitrary C++ Operation object does not itself establish that this source event ran; helper initial state is therefore Unknown.

Changed-class050c80 calls054f40(false), then QStandardItemModel::clear if nonnull. It preserves cached model identity; this helper does not yet implement that nonnull/clear route. OPI teardown's+160 virtual deletion and guard-null/deregistration also remain guarded. Public Close is not enabled.

## Frozen focused tests

Eight cases verify:

1. Unknown noncreating/creating lookup refuses without inventing null.
2. Qualified default-null allows repeated noncreating lookup while actual OpClass.current and OPI identity remain live.
3. Creating lookup stays guarded and preserves KnownNull/receipt/current.
4. Empty/blank defaults receipts refuse; repeated commit preserves first receipt.
5. Wrong Operation cannot commit/look up another OPI cache.
6. Cache lookups do not advance actual Props-first helper progress or expire current.
7. External declared fixture OPI expiry makes lookup/commit/construction refuse, without relabelling last knowledge.
8. Escaped cache metadata does not retain Operation allocation and cannot bind a later different row.

Static assertions forbid cache copy/move. Testkit declaring header and `int main() { return testkit::run_all(); }` were checked. Exact three files pass trailing-whitespace checks. Implementation contains no model/Props reset, delete, allocation, or expiry call. No compiler or tests were run by this lane.

Exact root target: `m2m_mdt_operation_info_model_cache`; ctest `m2m.mdt.operation_info_model_cache`.

```powershell
cmake --build build/candidate --target m2m_mdt_operation_info_model_cache
ctest --test-dir build/candidate -R '^m2m\.mdt\.operation_info_model_cache$' --output-on-failure
```

Use root's active converter build directory. Existing standalone packet link sets need no new helper source unless root later makes those callers use this isolated API.

## Frozen hashes

- Header: `80819268703b3af8a9712e637fa4d43bb956bfc26ef2be2e19e198933eb0f059`.
- Source: `6d4793964bb3dd297fd9e62e2c01eeae28a229c3b414669b58c8a2b95708e0ad`.
- Tests: `cdfe1d842329c82295af27f00367324216e84d7927abb7994b13a835b00eac2f`.

No reconstruction executable was built or deployed here. Native nonnull owner construction/deletion, actual defaults integration and full OPI/Close acceptance remain incomplete.
