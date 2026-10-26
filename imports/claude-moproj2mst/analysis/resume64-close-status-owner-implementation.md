# CP64: typed nonnull status-owner prefix

The new helper reads a typed main/layout/status-item ownership chain. An unknown slot remains unavailable; a qualified null slot reaches only the QStatusBar allocation guard; an existing item reaches only the unqualified virtual-widget/cast guard. No result releases public Close or establishes nonnull Qt status effects.

The exclusive write set is three new files plus this receipt:

- `source/apps/moproj2mst/include/m2m/engine/close_status_owner_prefix.hpp`
- `source/apps/moproj2mst/src/engine/close_status_owner_prefix.cpp`
- `source/apps/moproj2mst/tests/mdt/test_close_status_owner_prefix.cpp`

No existing helper, shared Session/model interface, CMake, plan or public Close was edited. No build, CTest/executable test, GUI, original target execution or mutable REA operation was run. Root owns combined validation.

## Source contract

`analysis/resume63-close-status-nonnull.md` and its bounded captures qualify the native owner/order. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; retained QtGui4_64.dll SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

After qualified nonnull status entry, QMainWindow::statusBar reads main private+8 -> layout+1e0 and invokes653ccd80. That getter reads the layout's status-item+4f8. Native null returns0 at653ccdb0, after which the outer getter reaches allocation at653ca63f. Native nonnull calls the item's virtual+60 at653ccd98, then QMetaObject::cast; only a qualified returned QStatusBar may later receive showMessage.

The implementation stops exactly at those two boundaries:

| Actual candidate layout slot | Result | Next native boundary |
| --- | --- | --- |
| Unknown | StatusItemKnowledgeUnavailable | 653ccd84 status-item read |
| KnownNull | StatusBarAllocationUnavailable | 653ca63f allocation/constructor |
| KnownItem | ItemVirtualWidgetUnavailable | 653ccd98 virtual+60/widget/cast |

Missing actual layout owner stops earlier at653ca627. There is no hasStatusBar input, caller-provided QStatusBar result, invented default-null layout/item, widget accessor, cast or constructor execution. The bounded getter's code hash remains `e580f3f1e92d76d91515599a77898e74709184a9b4493962c5526a66136965af` for653ccd80..653ccdb7.

## Actual candidate owners and qualification limits

`CloseStatusMainOwner` retains the actual Session/weak ModelManager identity and a qualified main binding. Its private layout field initially has no binding. `observeLayoutOwner` explicitly creates/binds a `CloseStatusLayoutOwner` associated with that same main; repeated bindings retain the first object and receipt. Layout owns its status-item slot, initially Unknown even though its C++ metadata pointer is empty. `observeNullStatusItem` and `observeStatusItem` explicitly qualify the current slot. KnownItem owns one concrete `CloseStatusItemOwner` identity associated with that same layout. All owner types are neither copyable nor movable; foreign main/layout/item grafts are not exposed by the API.

These are typed isolated native-state projections. Actual candidate ownership means the helper reads those same owned fields/identities, rather than substituting a detached Boolean or returned-widget fact. It does **not** mean the code constructed original QMainWindow/QLayout/QWidgetItem objects, installed the owners into Session, authenticated native observations or proved a native global alias. Nonblank qualifications label supplied bindings. MainViewState is not treated as the global main. Root must bind this chain to the actual native-main lifecycle before production use; fresh Session existence cannot populate it.

Observation methods allocate/release only candidate metadata and explicitly do not perform native Qt allocation/deletion. Their ability to replace the observed slot does not model an original mutation's callbacks. A retained shared metadata lease does not prove native target liveness or QObject-boundary completion.

## Retained continuation

`CloseStatusOwnerPrefixProgress` binds one main identity and internally owns CP62's fresh status-entry guard. Every attempt checks the same live Session/actual manager and exact main identity. Supplied entry facts must use CP62's exact late status occurrence; unknown remains guarded and qualified null remains outside the nonnull prefix. Later nonnull facts cannot replace a consumed null outcome.

After nonnull entry, progress retains the actual layout object when available, then consumes the first qualified slot result. KnownNull retains the allocation boundary even if a later observation installs an item. KnownItem retains that same item metadata and virtual-call boundary even if the current slot later becomes null or is replaced. Retries do not replay the entry lookup or switch branches based on later state. The result distinguishes entry and slot consumption on this attempt.

This wrapper owns its own CP62 progress. Future integration must route the real entry through it once or introduce a root-owned handoff of an existing consumed entry; running an independent CP62 invocation and then treating this wrapper's entry as a second native lookup would be incorrect. Earlier accepted-Close phases remain prerequisites, not implications of constructing this progress.

## Focused validation and freeze

Target: `m2m_mdt_close_status_owner_prefix`. CTest: `m2m.mdt.close_status_owner_prefix`.

```text
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.close_status_owner_prefix$ --output-on-failure
```

Ten Release-active Testkit cases cover missing actual layout despite candidate MainView existence; Unknown versus KnownNull; null-slot allocation guard with first receipt retained; existing-item virtual guard and consumed identity after replacement; staged later slot qualification without entry replay; missing/null status entry; foreign main/Session rejection; blank main/layout/slot/progress receipts; unchanged actual Project/model/current/notifier/pending/OPI state on both guarded branches, including suppression; and old diagnostic owners rejected for a replacement Session despite a retained manager.

Fixtures explicitly declare their native-state bindings and do not establish runtime original ownership or accepted-Close reachability. Static checks passed for Testkit main/include location, quoted include resolution, supported CHECK/REQUIRE macros, ownership static assertions, absence of C assert/NDEBUG-dependent checks and trailing whitespace. Compilation and executable validation are pending root's build.

| Frozen file | SHA-256 |
| --- | --- |
| close_status_owner_prefix.hpp | 317781e28ffaec4c8a53f320abc866c0d2da8d5c58f521c200211621dc1d545a |
| close_status_owner_prefix.cpp | d3a610b38f1b7f5b420d5f09a0cc244714653fc11ad643b4c165575b3fefc1ec |
| test_close_status_owner_prefix.cpp | 671e85dfb2459cc8f4d1da728c340cbcdda8e07d0c9001b27a141bd5833c23f4 |

Remaining work includes native main/layout binding and lifetime, slot mutation semantics, allocation/constructor/installation, item widget virtual/cast, actual status-bar message/timer/Qt effects, ShowMsgInArea population/process-global duplicate cache, caller QString release and public return. No timer, message-cache or blanket global-reset claim follows from this prefix. Public Close remains guarded.
