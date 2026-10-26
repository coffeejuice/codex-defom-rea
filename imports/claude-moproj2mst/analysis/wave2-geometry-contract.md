# Wave 2 geometry scene rectangle recovery

The original scene selector uses the rectangle box only when its derived radius is ordered above1e-6. The port previously refused before this test. Empty item rectangles have zero radius and must fall back to the existing model3D box. The implementation now restores that branch and the cached item rectangle state needed to distinguish empty from nonempty curve-only geometry.

## Source and identity

The original EXE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained Ghidra source is paired with fresh offline PE/disassembly checks in [wave2-geometry-static-evidence.json](wave2-geometry-static-evidence.json). No original application/function or source build was executed by this worker. All machine-readable contracts, source hashes and fixed expected boundaries are in [wave2-geometry-contract.json](wave2-geometry-contract.json).

## Original selector and arithmetic

[a3.c](../evidence/gui_mo/s1/viewer_rect/a3.c) shows0x140c8e0f0: a2D model returns rectBox; otherwise the rect is eligible only when some view is shown and no shown view has geometry, mesh or flag0x2d. After radius getter0x14121a310 and sign clearing0x141470a78, COMISD at0x140c8e507 and JBE at0x140c8e50f fall back for less/equal or unordered values. PE literal0x142157420 is exact double1e-6. rectBox0x1412399d0 makes a valid box even from invalid zero rect endpoints, but that does not bypass the radius test.

The Box3 centre is a float-stored double sum followed by a float-stored double division by2. The max-minus-centre difference also rounds to floats. The imported QVector3D::length is independently resolved in the copied QtGui4_64.dll and [disassembled](wave2-geometry-qt-vector-length-disassembly.txt): MULSS for each square, ADDSS x+y then+z, widen float sum, tail sqrt. The new boxRadius follows those stages.

| Half extent float bits | Derived double radius | Select rectangle |
|---|---:|---|
| 0x358637bc | 9.9999988958187529e-07 | False |
| 0x358637bd | 9.9999999800209868e-07 | False |
| 0x358637be | 1.0000001064223102e-06 | True |

These are source-derived fixed expectations, not observed original outputs. NaN/Infinity endpoint tests exercise the ordered fallback.

## Item state and propagation

The PE vtable0x142d36608 maps slot+0x48 to0x14126d0c0 (SfDisp2DCrossSection). Its ctor, cached rect refresh and source resolver are retained in [item.c](../evidence/deform-rea-work/out/mdtV/item.c), [item2.c](../evidence/deform-rea-work/out/mdtV/item2.c), and [item3.c](../evidence/deform-rea-work/out/mdtV/item3.c). A fresh item has null view binding and an invalid zero rect. A bound item resolves its geometry index; missing source leaves the cache unchanged. A present source resets the rect and unions sampled curve child rectangles with tolerance0.01, followed by its mesh child if present.

[a0.c](../evidence/gui_mo/s1/viewer_rect/a0.c)0x140c8e640 resets modelRect/modelBox and, for each shown non-kind7 view, updates item0 through vt+0x48 then unions its cached rectangle into modelRect. The patch now retains binding/index/cache instead of only an item count, preserves a missing-source cache, and destroys it on rebind. Curve-only contributions reuse the already recovered sampler and rectangle union.

Geometry mesh is distinct from the object mesh.0x1408ee940 first checks node count via0x1400c3570 and element count via0x1400c3590; new offline disassembly confirms nested fields+0x008/+0x12c and+0x148/+0xe8. Empty counts contribute nothing. Positive-count mesh coordinate/connectivity getters and0x14124bec0 remain unrecovered; the patch tracks that unknown contribution and preserves an explicit refusal when the scene would require it. It also retains the existing flag0x2b refusal.

## Focused implementation and validation

Edited only assigned viewer.cpp and test_viewer_box.cpp. Added four cases covering bound/unbound empty rectangles, fixed threshold-adjacent floats and nonfinite fallback, refresh/missing-source/rebind lifetime, and empty-versus-populated bound mesh qualification. The existing corpus box test remains. No public interface or m2m ABI changed.

Coordinator build and test receipts are pending.131/131R are intended evaluation cases; their converter success and original parity have not been established here. Full original GUI acquisition remains deferred. A whole scene-selector native probe requires its direct Qt/list/model dependency chain; this work supplies static evidence rather than relabelling a mocked probe as whole original behavior.
