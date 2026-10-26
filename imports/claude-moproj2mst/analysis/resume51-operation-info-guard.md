# Isolated OperationInfo guard implementation

Status: reviewed guard source/test restored into production source/test globs after root confirmed checkpoint49 frozen and checkpoint50 active. SHA-256 hashes match the staged originals. No existing source, OpClass.current, public Close, CMake, shared plan or build was changed. This helper does not enable accepted Close.

Files:

- `source/apps/moproj2mst/include/m2m/engine/operation_info_guard.hpp`.
- `source/apps/moproj2mst/src/model/operation_info_guard.cpp` (restored from staging unchanged).
- `source/apps/moproj2mst/tests/mdt/test_operation_info_guard.cpp` (restored from staging unchanged).

## Implemented contract

`OperationInfoIdentity` borrows one stable Operation address and owns only shared liveness metadata. Root must anchor one identity to the real OPI lifetime; the isolated helper cannot detect deletion of an arbitrary external Operation or prevent callers constructing duplicate live identities for one address. Copy/move of the identity is deleted. It is explicitly independent of the outer SfMoOprItem identity currently folded into Operation.

`OperationInfoGuard` binds that metadata without owning Operation. Copies are independent bindings: resetting/rebinding one leaves aliases bound to their original identities. Expiring the old identity does not clear a guard rebound to a different identity. Equal-identity reset is unchanged. Unbound and expired get() return null. The helper uses the caller's serialized lifecycle and makes no cross-thread Qt guarantee.

`commitQObjectBoundary(sourceQualification)` explicitly publishes the qualified OPI QObject guard boundary. It requires a nonblank qualification before mutation; the string labels caller evidence, not a proof that native payloads were executed. First commit stores the receipt and nulls all matching guarded observations. Repeated qualified commits return false and preserve the first receipt. No payload deletion, native signal, registry scan, UI/event delivery, model manager cleanup or Close decision is invoked.

Identity destruction expires still-live metadata with `AdapterIdentityLifetime`, preserving the distinction from `QObjectBoundary`. If the explicit boundary was already committed, its reason/receipt survives identity destruction in escaped metadata guards. This fallback is an adapter lifetime safeguard and must not be reported as the earlier native OPI boundary. Metadata ownership cannot prolong Operation allocation lifetime.

## Source qualification

Native order from `analysis/resume50-close-destructor-next.md`:

- SfMoOprItem045cb0 calls045dc0 to delete its OPI via virtual+18, before item+50 QString and QStandardItem-base destruction.
- OPI050440 executes050690 payload cleanup and member destructors, then QObject::~QObject at050531.
- Original OpClass setter5ef280 calls cbc920 on class+20. cbc920 skips identical old/new pointers, otherwise calls QMetaObject::changeGuard.
- Shipped QtCore QObject destructor calls clearGuards6712de20 at6712f8bd on the recovered nonwidget/registered-guard branch. clearGuards writes zero to each matching registered guard address at6712df79. This precedes admitted destroyed notification delivery at6712f97b.

Original reference DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. That report retains exact bounded code hashes and unresolved payload qualifications. No new original execution or static engine mutation occurred here.

## Focused validation

Seven tests plus static identity/guard copy/move assertions are restored:

1. Default unbound guard, same-identity binding/reset and null reset.
2. Shared aliases expire together; unrelated OPI remains live; late binding cannot resurrect an expired identity.
3. Rebinding one alias survives expiration of the old identity.
4. Missing/blank qualification refuses without mutation; terminal commit preserves first receipt.
5. Real controlled Props cleanup observes current still bound; explicit OPI boundary then gives a null observer while the outer Operation row remains allocated. This is a declared isolated fixture, not an original Close trace.
6. Adapter identity fallback is separately classified, and a retained guard does not retain the actual Operation allocation (a real Props destructor probe observes deletion).
7. Committed boundary receipt survives identity scope without being relabelled adapter fallback.

Only static review and whitespace checking were performed. No compiler/build/test was run. After root configures its active converter build, the main CMake glob should create target `m2m_mdt_operation_info_guard` and ctest name `m2m.mdt.operation_info_guard`:

```powershell
cmake --build build/candidate --target m2m_mdt_operation_info_guard
ctest --test-dir build/candidate -R '^m2m\.mdt\.operation_info_guard$' --output-on-failure
```

Use root's actual active build directory. The source includes only the new guard header and common.hpp; it does not require complete Operation layout or shared engine startup.

## Integration limits for root

Do not switch OpClass.current until a bounded call-site compatibility audit chooses explicit get/reset or a reviewed pointer-like wrapper. Do not automatically commit the boundary from the start of an Operation destructor, or assume expiry after the whole folded row equals native OPI expiry. A full native driver must preserve Props/model/list/member cleanup, commit the OPI QObject boundary before destroyed observers, then finish outer item destruction. Unknown earlier payloads and callbacks must keep the existing Close guard. This helper has no authority to declare those dependencies complete.

## File hashes

- Header: `d7dce6715eb2563dcab96f6bd6176bf0d5b6d3d5e7cc7f0334dbbffd48228fe5`.
- Restored source: `218c19d1eb175c06bcc0ac8b23343d1d267a975de34fa0f8bd796e77c3555138`.
- Restored tests: `8a2fbd6d72e1eb0e50c5f3498183d6e8453d23d51f98fdc7ec20c60a648a16d1`.
