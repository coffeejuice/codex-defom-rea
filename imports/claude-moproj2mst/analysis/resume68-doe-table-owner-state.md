# DOE table owner admission state

The three new C++ files represent the recovered main-window `+0x458/+0x460` stage order without constructing a QObject, executing a timer or admitting refresh success. This is an isolated state skeleton for later canonical Session integration. It is not a completed DOE receiver, an actual native table owner or public active-DOE acceptance.

Only these files and this report changed in this lane:

- `source/apps/moproj2mst/include/m2m/doe_table_owner_state.hpp`
- `source/apps/moproj2mst/src/doe/table_owner_state.cpp`
- `source/apps/moproj2mst/tests/doe/test_table_owner_state.cpp`
- `analysis/resume68-doe-table-owner-state.md`

No shared Session, `engine.hpp`, conversion, public Save, notifier interface or CMake file changed. No build, executable test, GUI interaction, original execution or mutable REA occurred.

## Exact interface and scope

`m2m::doe::DoeTableOwnerState(engine::ConversionSession&)` captures weak identities of that actual Session's ModelManager, notifier and WidgetRuntime. Each operation receives the live actual Session again, verifies the same pointers/control blocks and `manager->ownerSession()==&session`, and rejects a foreign/released admission scope. No saved Session pointer is dereferenced. Project identity is deliberately not frozen: releasing Project storage does not imply native table deletion.

The mutation API is:

```cpp
void record(engine::ConversionSession&, TableOwnerEvent,
            const TableOwnerObservation&);
const TableOwnerReport& report() const noexcept;
void releaseAdmissionScope() noexcept;
[[noreturn]] void requireRefreshDelivery(engine::ConversionSession&) const;
```

Every admitted event requires a nonempty qualification. Opaque `shared_ptr<const void>` leases identify declared observations and preserve diagnostic history; they are not native allocations, QObject weak guards or ownership proof. A qualification label does not execute/certify an effect. A future production binding must supply source-qualified actual occurrences, integrate the holder once into Session and preserve the enclosing native caller effects. This lane exposes no production provider that can do so.

## Preserved source order

The qualification is CP65/66/67 native static evidence, including `resume67-doe-table-owner-lifecycle.md`, `resume65-doe-refresh-getter.asm`, `resume65-doe-refresh-ctor.asm`, `resume65-doe-refresh-dock-call.asm` and `resume67-doe-table-lifecycle-pe.txt`. Original `DEF_GUI_MO_64.exe` SHA-256 was reverified as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Event | Native boundary and admitted metadata |
| --- | --- |
| InitializeFields | `0x140019760`, stores at `1976a/1977a`: unavailable table/dock fields become known null once. Never a reset/cleanup substitute. |
| PublishDock | `0x14001a48f`: record the returned dock identity at `+0x460`; independent of the table getter. |
| BeginGetter | `0x1400196e7`: cached hit records existing generation; known-null miss starts a new allocation generation. Unknown field refuses. |
| AllocationReturned / AllocationReturnedNull | allocator call `0x1400196f6`: nonnull lease enters constructor pending; explicit null enters null-publication pending. Unavailable is neither outcome. |
| TimerParentRecorded | `QTimer(parent=this)` call `0x1400eb826`, later `+0xb0` store: require the exact pending table identity and a distinct timer identity. No timer execution/state is inferred. |
| ConstructorPrefixReturned | boundary before global registration call `0x1400ebc5c`: require prior timer receipt; records that the preceding constructor prefix, including initial UI refresh, returned. It does not implement that prefix. |
| RegistrationReturned | `0x1400ebc61`: exact table and actual Session notifier receipt, before constructor return. No notifier population is changed. |
| ConstructorReturned | getter `0x140019714`: require exact allocation/result pointer **and** control block. |
| PublishCache | `0x140019738`: only returned constructor result or explicit allocator-null result may be published. Cache remains null through preceding constructor/registration stages. |
| DockInstallReturned | call `0x14001a52a`: exact published dock and getter result, including explicit null result. Earlier table publication/registration remain if this later boundary is unavailable. |
| BeginDelete | helper test `0x14001979e`: null early return changes no lifecycle state; cached table enters deleting pending. |
| DeletingDestructorReturned | `0x1400197e0`, after virtual `+0x18` call at `197dd`: record completion but retain cached field until its following store. |
| ClearDeletedCache | `0x1400197f5`: clear `+0x458` only after destructor return; retain `+0x460` and generation history. |

One additional bounded read-only dumpbin selection confirmed the dock publication preceding the existing CP65 dock-call capture. The decisive bytes are retained here rather than adding another artifact:

```text
000000014001A487: 48 8B 84 24 38 01 00 00   mov rax,qword ptr [rsp+138h]
000000014001A48F: 48 89 81 60 04 00 00      mov qword ptr [rcx+460h],rax
```

The selected startup caller therefore publishes the dock, calls `addDockWidget` at `1a505`, calls the table getter at `1a513`, then calls `setWidget` at `1a52a`. The class models the independent field/getter methods rather than imposing dock existence on every getter call; their native getter has no such prerequisite. `DockInstallReturned` is an observation of the returned call, including its caller prerequisites, not an implementation or elision of `addDockWidget`/`setWidget`.

`ConstructorInterrupted` is an explicitly marked adapter stop with source site zero, not a recovered native unwind implementation. It retains allocation/timer/registration receipts and the unpublished null field, then guards retry because native cleanup/reentrancy remains unresolved. A null-cache delete observation does not erase that interrupted allocation. A deleted then reallocated address/token gets a new generation; old history cannot become a live receiver. `releaseAdmissionScope()` blocks further admission but neither deletes native objects nor clears fields.

## Guard and unresolved effects

`requireRefreshDelivery()` always throws `M2M_E_UNSUPPORTED`. A bound but unavailable field reports `DOE table owner: main-window +458 is unavailable`; after field initialization its continuation guard is exactly:

```text
DOE table owner: signalDoeTableRefresh() timer/UI receiver delivery remains unsupported
```

Even a fully recorded constructor/publication/deletion history leaves timer active/stopped/id, Delete-widget enabled state and actual native receiver liveness unavailable (`nullopt`). Registration receipts do not imply complete refresh population, suppression dispatch, Qt metadata invocation or live QObject delivery. Native timer stop, `+0x98=1`, QWidget state and later current-Project lookup remain outside this skeleton. It does not resolve/freeze Project ahead of those effects or substitute the just-saved Study.

The existing public XML continuation remains unchanged and guarded by `Type2 study Save: signalDoeTableRefresh() receiver delivery remains unsupported`. The independent nonempty B121 prerequisites also remain guarded. A later root integration should own one canonical holder in the actual Session, admit only justified source stages and consume it at the reached XML-to-refresh phase while preserving false-result continuation and preceding XML/error effects. This holder alone cannot release that guard.

The bounded model selects normal single dock publication/installation and explicit lifecycle receipts. Arbitrary repeated dock construction, native constructor unwind, Qt child/timer teardown, all external field writes and runtime receiver population are not admitted. Refusal before metadata mutation preserves every earlier accepted stage; rejected observations are not presented as completed native receipts.

## Static preflight and freeze

Nine Testkit cases use Release-active `CHECK` assertions and `testkit::run_all()`. They cover unknown versus initialized fields; constructor/registration/publication ordering; timer parent and actual notifier identity; foreign control blocks; interrupted constructor retention; explicit null allocation/publication/dock result; cache hit and dock refusal; destructor-before-clear with retained dock/new generation; independent Project release; foreign Session and adapter-scope release. They verify Qt facts remain unavailable and no notifier dispatch/population is changed. Opaque integer fixtures are explicitly declared diagnostic profiles, not native observations.

All new quoted includes resolve against source include roots; no C `assert` or unsupported Testkit macros occur. No tests were compiled or executed in this lane. Root owns target/CMake registration and grouped preflight; suggested target `m2m_doe_table_owner_state`, CTest `m2m.doe.table_owner_state`.

| Frozen source | SHA-256 |
| --- | --- |
| `include/m2m/doe_table_owner_state.hpp` | `e3521511037812a260a527647ba63e039938772856dfd7cf1022db9826797034` |
| `src/doe/table_owner_state.cpp` | `d57b8a6ba6f6dac0d7d9efb515b73a6dafbc10ba4213e5681c37744faf5fcfcf` |
| `tests/doe/test_table_owner_state.cpp` | `06aa08498fba72bf2ad8134594e0f6638c19a68a60a2a36e86f0c309130c3df2` |

Token measurements are unavailable. This lane makes no runtime parity or whole-feature completion claim.
