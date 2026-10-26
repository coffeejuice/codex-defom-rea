# CP64: isolated process-wide message duplicate cache

The new helper implements only the source-qualified initialized-cache duplicate branch. Equal content with clear=false skips the handler's color/append/cache-write/cursor path. Unknown initialization, clear=true, different content and reset requests remain guarded. No GUI effects or early cache assignment are available through this API.

The exclusive write set is three new files plus this receipt:

- `source/apps/moproj2mst/include/m2m/engine/message_area_cache.hpp`
- `source/apps/moproj2mst/src/engine/message_area_cache.cpp`
- `source/apps/moproj2mst/tests/mdt/test_message_area_cache.cpp`

No existing helper, shared Session/model interface, CMake, plan or public Close was edited. No build, CTest/executable test, GUI, QStatusBar, original target execution or mutable REA operation was run. Source edits are frozen for root's Release validation/archive.

## Source-qualified behavior

`analysis/resume63-close-status-nonnull.md` and its fixed metadata/direct disassembly/import bindings establish local method2 `slotShowMsgInArea(QString,QColor,bool)` ->dd61e5 ->d151f0. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; native d151f0..d15309 code SHA-256 `84b80ac99f9bafc280de6c6b882d47931b1cfa7fc06d79ab2c24926e6980be14`.

The handler reads initialization word1446b3bb8, conditionally sets bit0 before default-constructing global QString1446b3bb0 atd15234, and passes its cleanup callback through14146ed38 atd15241. Initialization precedes the clear virtual and comparison. The new helper does not execute these initialization/registration effects or treat unknown process state as initialized-empty.

For initialized state, clear=true reaches receiver virtual+1c0 atd1525d before comparing messages. That route remains guarded even when incoming text equals the cache. Clear=false compares atd1526f. Equality takesd1527c ->d15304 without color/append/assignment/cursor effects. Different content stops atd1528b before setTextColor. Native append isd1529b; only after it returns atd152a1 doesd152ad assign the global last-message QString. The helper has no post-append bypass/commit API and cannot overwrite the cache before those unimplemented effects.

The comparison uses existing `qt::String` content equality: UTF-16, case-sensitive and null equal to empty. Physical QString shared-data/refcount behavior is not newly reconstructed here. Incoming argument preparation, QColor copy, actual receiver liveness, shared notifier dispatch and any previous QStatusBar effects precede this isolated handler-entry seam and are not implied by creating progress.

## Process owner and guarded reset

`MessageAreaProcessCache` is independent of Session, Project and receiver classes. Root must create one owner for each represented native process and share that same object across all receiver invocations. A repeated qualification label does not intern independent objects into one process. This is an explicit native-state projection, not a C++ static QString or authenticated runtime process binding.

Its initial knowledge is Unknown. Explicit one-time bindings distinguish KnownUninitialized with raw bit0 clear from KnownInitialized with raw bit0 set plus an observed existing QString value and qualification. All raw word bits are retained. An initialized-value binding requires evidence of completed initialization, not just a set bit; the nonblank label itself does not prove it. Repeated identical bindings retain the first receipt. Conflicting word/value/nullness or initialized/uninitialized rebindings refuse, so observation APIs cannot substitute for native initialization, append assignment or reset.

KnownUninitialized stops befored1521e's initialization-bit/QString/registration prefix; it leaves the raw word and absent value unchanged. Unknown may later receive an explicit existing-state binding. A known-uninitialized record cannot be upgraded through this binding API to bypass native initialization effects.

`MessageAreaCacheProgress` weakly binds one process identity and captures one handler's message/clear arguments and exact occurrence. Copy/move are deleted on progress and cache owner. Comparison outcomes are retained; retries do not compare again or execute GUI work. A replacement process owner cannot reuse old progress even if addresses or labels coincide.

All reset requests return typed `NativeResetUnavailable` with no mutation. Normal Close and receiver teardown establish no reset in the inspected contract. ProcessExit points to141dfb72b's QString destruction, but actual CRT delivery/QString effects remain unqualified. Destroying the C++ cache owner releases adapter metadata only; it does not claim native process exit, QString cleanup or initialization-word reset. The helper neither clears the cache on receiver progress destruction nor treats it as a status-bar-private message field.

## Focused validation and freeze

Target: `m2m_mdt_message_area_cache`. CTest: `m2m.mdt.message_area_cache`.

```text
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.message_area_cache$ --output-on-failure
```

Nine Release-active Testkit cases cover unknown versus initialized-empty; known-uninitialized flag retention and initialization-before-clear guard; duplicate skip/retry; distinct-message refusal with old value retained and early-write rebind rejected; clear-before-comparison ordering; QString null/empty and supplementary UTF-16/case distinctions; one process owner shared across two receiver progress lifetimes; all three reset causes guarded; exact binding/raw flag preservation; qualification/source/foreign-or-expired process identity refusal.

These are explicit value/identity fixtures, not original GUI delivery or process-exit acceptance. Static checks passed for Testkit main/include, quoted include resolution, supported CHECK/REQUIRE, ownership assertions, valid supplementary UTF-16 universal literal syntax, absence of C assert/NDEBUG-dependent checks and trailing whitespace. No compilation or executable test was run by this worker.

| Frozen file | SHA-256 |
| --- | --- |
| message_area_cache.hpp | f3344759091159f9853c3d84b4bd9e906d9c3f0304fc028b23a5c5b44b4f6d11 |
| message_area_cache.cpp | 08f9cedf294a756c45b46383d047b75d1ba9062304aa9ad15abc3d1101d77637 |
| test_message_area_cache.cpp | bfda025008f6da445880e6865a8f3e4bb4c25f2720d85f3e40c65fc1aac324bd |

Future integration needs actual process/global QString ownership, initialization/cleanup-registration effects, receiver dispatch/lifetime, clear/color/append/cursor effects and the exact post-append assignment seam. It must share this process owner rather than allocate a cache per Session/receiver. Public Close, nonnull status effects, message append and native reset remain guarded.
