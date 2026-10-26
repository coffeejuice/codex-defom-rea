# CP118 bounded global Geometry editor producers

The fixed-string producer set for `2signalShowCentral2DGeoEditor()` is now classified through native caller instructions and Qt metadata. It contains Geometry Edit, geometry import/export, and primitive cross-section actions. No normal project Open, Save or Close producer is established in this bounded set. In particular, `SfPreGeometry::slotSaveGeo()` is a geometry-panel click route, not evidence that project Save invokes the editor or reads the stored DOE GEO files.

This is an analysis-only continuation of CP116/117. Original GUI PE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No source or public guard changed.

## Fixed producer inventory

CP117's exact ASCII/UTF-16 string scan found six copies of the exact key: one constructor registration and five emitting bodies. The new captures verify the four previously unclassified emitting bodies and their direct caller branches.

| Emitting body / call to global dispatcher | Bound caller roles |
| --- | --- |
| `0x1400faf60`, emit at `0x1400fb41b` | `SfMoDoeTable::slotGeomEditClicked()`, actual indexed `SfMoDoeTableFuncButton::clicked()` / sender-cast route, closed in CP117 |
| `0x1403629c0`, emit at `0x1403634dc` | Geometry import/load-from-library and explicit geometry Save actions; primitive define-cross-section controls, detailed below |
| `0x1403675f0`, emit at `0x14036792d` | DOE variable Geometry Edit control and other Geometry Edit controls |
| `0x14049d220`, emit at `0x14049d5ed` | `SfGeoPrimitive::on_m_pb3DExtrEditCrossSection_clicked()` |
| `0x14049d610`, emit at `0x14049d9dd` | `SfGeoPrimitive::on_m_pb3DRevEditCrossSection_clicked()` |

The sixth fixed-key reference is initialization `0x140011482`, which registers the main-window receiver/method. It is not an emitting producer. The global dispatcher, registered slot overloads and downstream `0x140032afe -> 0x1400d4560` loader remain as qualified in CP117/116. Their presence does not prove delivery in a selected lifecycle.

## Caller and owner bindings

Native Qt revision-6 metadata and the corresponding static-metacall jump tables close the following roles. Method IDs are local to each class; they are not globally comparable.

### Producer `0x1403629c0`

- `SfGeometry300` metadata (`0x1423e16e0`) methods 5 `slotImportGeo()` and 6 `slotLoadGeoFromLib()` dispatch through `0x140480380` to wrappers `0x140389d80` / `0x140389dc0`. Both call `0x140394a30`, whose native call at `0x140394ae4` invokes `0x1403629c0`. They are Geometry panel import/library actions, not the `.moproj` reader.
- `SfGeoPrimitive` metadata (`0x142458130`) methods 15/16 are `on_m_pb3DExtrDefCrossSection_clicked()` / `on_m_pb3DRevDefCrossSection_clicked()`. Static-metacall `0x140551ae0` calls wrappers `0x14049d1e0` / `0x14049d200`; these call `0x14049d190`, which invokes `0x1403629c0` at `0x14049d1aa`.
- `SfPreGeometry` metadata (`0x142c550c0`) method 0 `slotImportGeo()` dispatches to `0x140f9a780`, which calls the producer at `0x140f9a7e4`. Method 1 `slotSaveGeo()` dispatches to `0x140f9a8c0`, which calls it at `0x140f9a924`.

The last pair is explicitly wired to button events: setup `0x140f9c690` calls imported QObject::connect at `0x140f9f50a` with UI control `+0xc0`, signal `2clicked()`, actual panel receiver, slot `1slotSaveGeo()`, connection type zero; at `0x140f9f53d` it similarly connects control `+0xb0` to `1slotImportGeo()`. `SfGeometry300` setup contains corresponding Import/Save click connections at `0x140399867` and the following Save connection. Thus the word “Save” in this metadata is not an unbound alias for the normal project Save entry `0x1400983c0`.

### Producer `0x1403675f0`

- `SfMoDoeVarDef` metadata strings at `0x14216a4a0` and method data at `0x14216a390` identify local method 3 as `on_m_tbEditGeo_clicked()`. Native initializer `0x141cad3ae..0x141cad3d8` publishes those string/data pointers and extra-data pointer `0x14216a580` into metadata object `0x143d7e040`; the extra data contains static-metacall `0x1400da270`. Thus this owner binding is established by explicit native pointer stores, rather than inferred from neighboring strings. Static-metacall jump-table case 3 calls `0x14010a400` at `0x1400da2fe`. That method's qualified branch calls the producer at `0x14010a6c5`. It belongs to the DOE variable definition/edit workflow; it can create or select a variable before requesting the editor. This is not the lazy InputTable reader.
- `SfGeometry300` method 17 `slotEditGeoClicked()` dispatches to `0x14038be20`, whose call at `0x14038be6b` invokes `0x1403675f0`.
- `SfPreGeometry` method 12 `slotEditGeo()` dispatches to `0x140f9b270`, whose call at `0x140f9b2ed` invokes `0x1403675f0`. Its setup connects UI control `+0x28`, signal `2clicked()`, to that slot at `0x140f9f3b4`.

These helpers still have their own input/type/context predicates. For example, the captured `0x1403675f0` returns for a null first argument or its explicit mode-3 branch before constructing arguments or emitting. A producer function name/address does not imply an unconditional signal.

### Producers `0x14049d220` / `0x14049d610`

The `SfGeoPrimitive` static-metacall jump table at `0x140551cd8` maps local IDs 17 and 18 to blocks `0x140551c76` / `0x140551c82`, which call these producers at `0x140551c7b` / `0x140551c87`. The metadata names are the extruded/revolved **Edit Cross Section** clicked handlers. IDs 15/16 above are the separate Define Cross Section handlers. The native distinction is preserved; these are not one generic startup or Close producer.

## Open, Save and Close disposition

All newly bound direct caller frontiers are geometry action slots or cross-section clicked handlers. No inspected native call ties the normal project Open, project Save, accepted project Close, or `SfMoDoeTable::slotUpdateUI()` to an emitting call in this set. CP117 separately closes refresh-to-button construction/connection; it does not close an actual click. Here, the newly classified producer routes do not supply that missing event either.

This is **not** a proof of whole-program absence. The rel32/RIP scanner only supplies reference candidates, and fixed string enumeration cannot exclude dynamically assembled keys, indirect function calls, Qt method-name invocations, signal reentrancy or another object invoking an action slot programmatically. Qt metadata shows that a method is callable; connection source shows how a signal may reach it; neither proves that signal or call occurred in the selected lifecycle. The new native captures establish the local direct calls and their static method IDs, not a fresh runtime registration/population or a normal return.

There is therefore no supported reason to insert a Geometry editor or a GEO payload read into the XML Geometry reader. The demonstrated stored-path loader remains a conditional consumer on the editor workflow. Actual normal Open→Save linkage is unproved. The active Save refresh guard, native QFileInfo owner/conversion guards, and public Geometry refusal remain appropriate at their existing reached boundaries.

## Exact next boundary

The static uncertainty has narrowed from “which producer?” to **whether a selected normal lifecycle actually delivers one of these identified geometry-action events**, or programmatically invokes one of their slots. For the DOE table, capture/prove its actual indexed button `clicked()` and sender-cast result. For the other paths, bind the actual `SfPreGeometry`/`SfGeometry300`/`SfGeoPrimitive` action receiver, relevant signal or method invocation and context before its producer call. Then bind the same global dispatcher receiver snapshot and main-window overload delivery/return. A generic dispatcher return, a new connection, or a synthetic helper call cannot stand in for that occurrence.

The smallest next evidence action is a closed normal Open/Save/Close callback trace showing those action-slot entries (or excluding them within that traced interval), with exact receiver and sender identities, while retaining indirect/event-loop uncertainty beyond the interval. Until that is authorized and available, this lane is evidence-complete at the bounded caller inventory; no further isolated source helper closes it.

## Evidence records

`resume118-doe-global-show-producers-static.json` binds the four new producer captures; `resume118-doe-caller-meta-static.json` binds their static-metacalls and small action wrappers; `resume118-doe-action-callers-static.json` binds the direct caller bodies; `resume118-doe-action-connections-static.json` binds the small QObject::connect windows. Owner names/signatures and method-table/jump-table values are retained in `resume118-doe-producer-owner-metadata.json`, `resume118-doe-variable-dialog-methods.json`, `resume118-doe-known-method-bindings.json` and `resume118-doe-additional-method-bindings.json`. `resume118-doe-var-meta-init-static.json` adds the variable-dialog metadata publication stores. Reference-candidate receipts retain their non-exhaustive qualifications. Final `resume118-doe-global-show-producers-capture-index-v2.json` binds each evidence file hash and preserves the earlier index as historical; adjacent/partial instructions after window ends or initializer RET are excluded from behavioral claims.

No source, tests, build, GUI, original process or mutable REA database was touched.
