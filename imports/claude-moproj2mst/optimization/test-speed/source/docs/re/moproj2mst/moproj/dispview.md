# .moproj writer: SfDispView (the 3D display's camera)

Packet moproj, 2026-10-22. Port: `apps/moproj2mst/src/engine/viewer_camera.cpp` (display, fit,
camera), `src/engine/viewer2d.cpp` (2D items and rects, a packet V extension), `src/engine/viewer.cpp`
(2D bind / recompute / refresh, `viewerSceneBox`), `src/moproj/write.cpp` (`dispViewElement`).

## Element (0x140cd57b0 -> 0x140d05a10 / 0x140d06610 / 0x140d07a00)

Children in order, each a `<Name>` with `<x>`, `<y>`, `<z>`, `<s>` as many as the field has, values
`QString::number(v, 'g', 6)`: VersionNum 1.2, Quaternion (toVector4D: x, y, z, scalar; floats),
EyeVector (camera +0x1a0, floats), ViewVector (+0x1ac), UpVector (+0x1b8), ZoomFactor (+0x1c8),
CenterX / CenterY (pan +0x398), Left / Right / Top / Bottom (+0x3b0..+0x3c8), Origin (+0x3d0).
A first extra child (0x140d46af0) needs a second model in the display (0x140d0c920); none here.

## When the camera is set

After the active operation is loaded the main window shows its model (0x140cc3a00 / 0x140ccdd10:
0x140cf9360, m+0x10 = display) and resets the view (0x140cfb5f0, display +0x708 == 0): 2D model
0x140d011d0 (rotation reset), 3D model 0x140cff570 (iso view: rotations by the
`displayOption/dispUpwardAxis` preference, unset = -1; then X -90, Y -90, Y -45, X 30), then the fit
0x140cfe9c0. Port: `openActiveOperation` calls `displayShow` and `displayResetView`.

## Fit 0x140cfe9c0

1. Refresh (0x140c8e640) of the shown model: view boxes, the 3D box m+0x148, the 2D rect m+0x108
   (with the axis widening to x = 0 for an axisymmetric (1) or type-3 model shown in a display whose
   +0x202 is 1, the ctor default).
2. Scene box 0x140cfd750: union of the shown models' boxes 0x140c8e0f0 (2D: rect m+0x108 as a box,
   z = 0; else m+0x148, the rect variant refuses).
3. 0x140d69e70 stores it at +0x298 when valid, radius >= 0.001 and window extents >= 0.001.
4. 0x140d69d60: window (+600) = the box's view volume (centre ± 1.05 r; 0x141219af0).
5. 0x140d69740: zoom 1, pan 0, eye (0x140d69820), view (0, 0, -1), up (0, 1, 0).
6. 0x140d67470: origin = -centre of the stored box.

Eye (0x140d69820 / 0x140d67080 / 0x140d68ee0): with a = W / H, the orthographic box is
(L a, R a, B, T); the scene point under the widget centre is unprojected; eye = window centre minus
that point, so eye.x = cx (1 - a) up to rounding and eye.y = 0 up to rounding.

## Profile (machine inputs)

* GL widget interior 3363 x 1709 (Oracle B, coordinator 2026-10-22, maximised fresh GUI on Sim2).
* dispUpwardAxis unset (-1).

## 2D items (viewer2d.cpp)

* Curve item 0x141267410 (tolerance 0.01 x 0.05): type 1 rows (x, y, r): straight points, filleted
  corners as arcs (0x141268af0 neighbours, 0x141268cd0 fillet, 0x141269180 arc in chordCount steps,
  fallback 0x141269680 tangent points); type 2 rows: lines by their ends, arcs from the start about
  the centre (0x141269bd0) in chordCount (0x141269a90, at least 20) steps. Rect: 0x141267530.
* Mesh item 0x14124be10 / 0x14124bec0: node positions as floats; rect 0x14124c3d0.

## Oracle B

All 16 cases of the moproj test match SfDispView byte for byte. R19, 79 and 82 were saved by
non-fresh GUI processes with another window aspect: their EyeVector is masked (82's -0.613922 against
-0.533441 in the fresh probe / x1 saves of the same project; R19 one ulp in y).

## Not traced / assumed

* 0x140cfe0f0's radius goes through 0x140c8f830 (another per-model box); the port takes the scene
  box radius (it only gates the store at 0.001).
* The quaternion's spin step (0x140d69060) is taken as zero (no spin is started on this path).
* Display modes +0x204 (0x140c80a20 / 0x140c80b00) are 0 in a fresh display.
