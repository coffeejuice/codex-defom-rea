# CP138: retained error mapper is ready; FLS/PTD is not

The smallest disjoint source-closed cut is the pure14740ec mapper under an explicit **retained-initial-table profile**, taking a uint32 error argument and returning the original numeric CRT code. It must not fetch LastError or write errno. Direct FLS/PTD production is not ready after CP137: later startup, lock, callback and per-thread owners remain missing. No live failed-free/Close admission follows from the mapper.

Read-only: only this report was written. No source/build/test/GUI/original execution or REA database work. Immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses use the common14 prefix. Bounded DUMPBIN14.44.35228.0 and PE byte/import reads corroborate prior CP136 evidence.

## Exact mapper semantics and snapshot qualifier

14740ec loads table address143b5e160. It compares ECX with the first DWORD of each8-byte row, stopping on the **first** match. Native bound is45 rows (2Dh), ending143b5e2c8. A matching row returns its second DWORD at147412d. Duplicate key6 occurs with outputs9 and22; first wins. Sorting/deduplicating with last-write semantics would introduce a source mismatch.

If no row matches,147410a computes uint32(error-0x13); unsigned <=0x11 returns13. Otherwise1474118 computes uint32(error-0xbc); unsigned <=0x0e returns8, else22. This is32-bit wrap arithmetic, not signed range subtraction. Table lookup always precedes the fallback ranges; no host errno macros or modern error-mapping policy should replace the retained numeric outputs.

Retained ordered45 pairs, decimal key:code:

```text
1:22 2:2 3:2 4:24 5:13 6:9 7:12 8:12 9:12 10:7 11:8 12:22 13:22
15:2 16:13 17:18 18:2 33:13 53:2 65:13 67:2 80:17 82:13 83:13
87:22 89:11 108:13 109:32 112:28 114:9 6:22 128:10 129:10 130:9
131:22 132:13 145:41 158:13 161:2 164:11 167:13 183:17 206:2 215:11 1816:12
```

The table occupies writable.data (section characteristics c0000040), not read-only code. Its initial bytes are immutable in this retained PE; no current original-module table observation or exhaustive runtime writer closure is established here. Name/profile the helper accordingly. The adjacent errno fallback word at143b5e2c8 is outside the45-row table; reading a46th row would change error0/default behavior.

## Proposed exclusive pure implementation

After root assignment/freeze, reserve only three NEW files beneath source/apps/moproj2mst:

- src/model/feature_win32_error_map.hpp
- src/model/feature_win32_error_map.cpp
- tests/model/test_feature_win32_error_map.cpp

Suggested private API: `std::int32_t m2m::detail::mapRetainedFeatureWin32Error(std::uint32_t) noexcept`. Keep the retained profile in the name/documentation and code/table hash qualification. Use an ordered fixed45-row array, linear first-match lookup and explicit uint32 wrap subtraction. No default/current-error argument, Win32/TLS/CRT calls, global writes, allocation, runtime-role promotion or integration into the actual failure cursor. Existing source/test globs supply m2m_model_feature_win32_error_map / m2m.model.feature_win32_error_map; no shared runtime/model/CMake edit is needed.

Meaningful tests should use source-derived golden values: representative/all44 unique table keys; key6 returns9 despite later22; error0 returns22;18→2 then19/36→13 then37→22;187→22,188/202→8,203→22;206→2 and1816→12; full uint32 extremes/underflow cases default22. Explicit modeled inputs are not observed LastError. Verify the helper leaves caller state untouched and requires no runtime owner. Do not turn its result into a live errno store or a paused-free success. Keep Release-active Testkit checks/main and original numeric outputs even where surprising.

## Why direct FLS/PTD remains guarded

CP137 stops before146ea0d after only the first encoded role.146e9f4 still has seven more null-role setter calls and an encoded nonnull callback-role store, followed by147f9cc lock setup.1481708 then calls **FlsAlloc(callback1481590) at1481723**, stores index143b5e990 at1481729, allocates0x2c8 PTD at148173e, FlsSetValue at1481754 and initializes thread state. It is not equivalent to allocating an index with a null/generic host callback.

Newly bounded callback1481590 starts with a null-pointer fast return1593 ->16c4. Its nonnull route frees pointers at PTD+38/48/58/68/70/78/80, conditionally+ A0 against a static default, enters locks13 and12, decrements/refines locale/refcount state at+B8/+C0, and finally frees PTD at16b5. The null leaf alone does not provide a complete registered callback or its same-module/thread/heap lifetime. An unsupported nonnull callback cannot be silently returned as if native teardown completed. Existing FeatureCrtRuntime owns only candidate heap and first encoded-handler role, not those FLS/locks/PTD owners.

1474134's errno address is PTD+10 or fallback143b5e2c8.14814e8 saves LastError before FLS lookup/allocation and restores it before returning; outer free then calls GetLastError and14740ec. Previously paused failures lack that same-thread error snapshot. A new genuine failed-free occurrence could execute the provider's first source GetLastError inline and guard before the unbound FLS index, but no later host LastError may be relabeled as the old value. Mapper reconstruction neither supplies this missing witness nor closes provider completion.

Context/Close remains separate: CP132 still stops before first buffer call with C+1450 unknown; CP135's isolated map return does not construct C or admit its destructor, and source-map ownership cannot be replayed. No ContextEnvelope host destruction or error report may invoke native Project/Close cleanup.

## Receipt

| End-exclusive original range | SHA-256 |
| --- | --- |
|1414740ec..141474133 mapper|`f44e86936fa3b398df977c4dbe968eff2c5fec42fd3c07a7dc4adc02d61bce17`|
|143b5e160..143b5e2c8 initial45-row table|`dc5aaf854d47b0fa4deddacb6ff02e9a7d99cb1ac06c16e03e587d4599067fbe`|
|141481590..1414816c5 PTD callback|`b3145fdde39ef1c91cff69477eac4211d1e7136fb3f32c22b5b4f6c25de0bc31`|

CP136 report SHA `da58f8912165bbe0e105e84053447bdbac0f76227c0a249e5780f99730eb0e6f` retains FLS/errno producer and context unwind hashes. CP137 implementation receipt SHA `1655c51fcecb52c14a3a7fac1d9738343d655d54437bae905eb94e10dd2c289d` qualifies the current first-role boundary. This report is not a build/runtime witness. No reconstructed executable/deployment was inspected or changed.
