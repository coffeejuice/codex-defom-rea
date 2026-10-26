# CP117 Geometry editor invocation and Save linkage

The stored-GEO loader is reached through a concrete Geometry Edit workflow. `0x140032800` is the main window's **show central 2D geometry editor** slot, not an unidentified Save callback. The DOE table has a real button-to-slot connection and a global notification route to that main-window slot. A table refresh can construct and connect the button; this does not establish a click or a Geometry editor invocation during the selected Open→Save. That occurrence remains unproved, so neither public Geometry admission nor a whole-lifecycle “no GEO read” claim follows.

## Main-window method and global route

Original GUI PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Native Qt metadata at `0x1421705f0` points to strings `0x14216f2d0`, method data `0x14216e830`, and an extra-data static-metacall pointer `0x1400db1a0` at `0x1421705e8`. Its class name is `SfMoMainWindow`. The revision-6 table contains these methods:

| Local method ID | Signature | Verified dispatch |
| --- | --- | --- |
| 34 | `slotShowCentral2dGeoEditor(LLOBJ_DATA*,SfStepData*,bool,bool,bool)` | jump target `0x1400db78b`, call `0x1400db7e5 -> 0x140032800` |
| 35 | same slot with four arguments | `0x1400db7ef`, call `0x1400db83a`, final Boolean default false |
| 36 | same slot with three arguments | `0x1400db844`, call `0x1400db881`, fourth/fifth arguments default true/false |
| 37/38 | `slotHideCentral2dGeoEditor(bool)` / no-argument clone | calls to `0x140033cf0` |

The native jump table `0x1400dc900` independently binds these IDs to their dispatch blocks. This closes the method identity beyond the older provisional decompile.

Main-window initialization `0x1400111c0` registers, at `0x140011482`, global owner `0x143d76700`, key `2signalShowCentral2DGeoEditor()`, receiver the saved actual main-window `this`, method `1slotShowCentral2dGeoEditor()`, flag zero. Registration `0x1402cfbe0` strips the Qt marker/signature and stores a bare method, as already qualified in CP51/56. Thus the empty parentheses in this registration string are not proof of a no-argument target or missing overload: delivery carries typed QGenericArguments and resolves the corresponding overload. The same initialization registers the hide route at `0x1400114a6`.

This is constructor source evidence, not proof of a live unsuppressed receiver in any particular Save. Existing dispatcher `0x1402cf3c0` selects the current snapshot, checks its receiver guards and invokes directly through QMetaObject; the result is ignored. Main-window lifetime, suppression, registration population and a normal callback return remain same-occurrence dependencies.

## Concrete DOE-table producer

`SfMoDoeTable` metadata at `0x14216a080` identifies local method 8 as `slotGeomEditClicked()`. Its native static-metacall branch calls `0x1400faf60` at `0x1400d99a1`.

The table's row-widget construction routine `0x1400ee400` creates an actual `SfMoDoeTableFuncButton`: constructor `0x1400ffcb0` delegates to imported QPushButton construction, then installs vtable `0x14218d678`; its RTTI identifies `.?AVSfMoDoeTableFuncButton@@`. It writes the variable index at button `+0x30`. At `0x1400ee646`, imported QObject::connect receives:

```text
sender = that button
signal = 2clicked()
receiver = the actual table
slot = 1slotGeomEditClicked()
connection type = 0 (Auto)
```

The slot at `0x1400faf60` is sender-dependent. At `0x1400faf76` it calls imported `QObject::sender()`, passes that object to cast wrapper `0x140100b20`, and returns at `0x1400faf8f` if the cast result is null. The wrapper invokes QMetaObject::cast on the button's `0x143d7e000` metadata object. The button's own metaObject virtual at `0x1400d9770` returns that same object on its ordinary static branch; the RTTI and virtual table bind its class. The slot then uses the sender's stored index to get a Var from the table's manager and checks that Var before preparing the Geometry editor context.

At `0x1400fb41b`, the slot emits global key `2signalShowCentral2DGeoEditor()` through `0x1402cf3c0`. Its first three typed arguments are `LLOBJ_DATA *`, `SfStepData *`, and a Boolean initialized false; subsequent arguments are empty. These map to the three-argument clone above, retaining the native default Boolean arguments. This closes a real conditional route:

```text
actual geometry button clicked
  -> same table slotGeomEditClicked / sender cast / indexed Var
  -> global ShowCentral2DGeoEditor notification with typed arguments
  -> live matching main-window registration / direct overload invocation
  -> 0x140032800
  -> qualified branch at 0x140032afe -> 0x1400d4560
  -> stored-path loader and read-text probe recovered in CP116
```

The show handler has context/type/owner decisions before `0x140032afe`. Its existence does not make every delivered show call reach the loader. The full bounded handler and the CP116 loader report preserve that conditional scope. No candidate callback, Boolean or synthetic sender is substituted for this route.

## What refresh reaches

The existing actual-table route is `slotUpdateUI 0x1400f4e10 -> timer stop -> virtual refresh 0x1400f47e0`. The refresh's nonempty-context paths include `0x1400f4dd1 -> 0x1400f4250`, then `0x1400f479c -> 0x1400ef670`. That row-population routine calls `0x1400ee400`, where the button and connection above can be created. Another refresh branch calls `0x1400f31b0`, which populates other table/list presentation.

The inspected refresh, population and button constructor bodies contain no identified direct call to `0x1400faf60`, no identified emission of the editor-show key, and no identified call that clicks this button. The button constructor installs state; the connection records a future signal route. Those facts cannot be promoted into a `clicked()` occurrence. The native slot's sender cast also prevents treating an arbitrary direct table call as equivalent to the observed button route.

This is a bounded negative result. UI methods and virtual calls can have signal/callback effects that are not closed merely by a rel32 scan. The selected Save's actual receiver list, widget state and synchronous/queued emissions have not been captured. Other copies of the ShowCentral2D key have producer candidates in `0x1403629c0`, `0x1403675f0`, `0x14049d220` and `0x14049d610`; this report does not classify their complete invocation graphs or exclude them from every lifecycle. It therefore does not assert that live refresh can never reenter editor presentation, or that all Open→Save executions omit GEO reads.

## Exact remaining boundary

For the table route, the missing edge is a **same-occurrence `SfMoDoeTableFuncButton::clicked()` emission/delivery**, with that sender, its index and actual selected manager/Var, during the accepted Open→Save callback chain. An alternative requires a same-occurrence producer of `2signalShowCentral2DGeoEditor()` from one of the other sites. After an emission, actual global receiver selection and normal main-window editor-slot return must still be established. Source connection setup and metadata resolvability alone do not close either occurrence.

The smallest next static action is to classify the remaining producer methods and their Qt connection/caller roles, then close whether any normal Open/Save handler actually invokes them. The decisive runtime witness, if separately permitted, would correlate the Save refresh entry/return, actual editor-show emission (or its absence within the closed callback trace), invoked receiver/method and any `0x140032afe` loader call. No new runtime evidence was acquired here. Keep current public Geometry, QFileInfo/conversion and active Save refresh guards; this report does not authorize a synthetic editor or an extra GEO read in the XML parser.

## Evidence receipts

All new files are analysis-only. The native report is backed by `resume117-doe-mainwindow-methods.json` (Qt metadata), `resume117-doe-editor-selected-bindings.json` (jump-table IDs), `resume117-doe-main-editor-meta-static.json` and its assembly (dispatch), `resume117-doe-editor-registration-140011440.asm` (registration window), `resume117-doe-editor-producer-static.json` (producer/refresh descendants), `resume117-doe-editor-widget-connect-static.json`, `resume117-doe-edit-button-static.json`, `resume117-doe-editor-sender-static.json`, `resume117-doe-button-metaobject-static.json`, `resume117-doe-editor-widget-owner.json` (RTTI), `resume117-doe-editor-show-handler-static.json`, and the compact key/import/reference receipts. The capture index records their hashes. Byte-pattern references remain locators until matched to native instruction context; partial instructions outside the cited registration window are excluded. Existing CP51/56 dispatcher/registration and CP65 refresh receipts retain their original qualifications.

No source, test, CMake, build, original execution, GUI or mutable REA operation occurred.
