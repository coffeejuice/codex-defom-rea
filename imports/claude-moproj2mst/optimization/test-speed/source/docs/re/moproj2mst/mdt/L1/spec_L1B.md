# L1 part B: cogging geometry step at Save (SfOprCogging vt+0xd8)

DEF_GUI_MO_64.exe v11.0.2, static analysis only (headless Ghidra copy `gproj_mdtB`). Decompiles and
listings in `gui_mo/s1/mdt/L1/B/`: `b0..b21.c` (`b*a.c` = with strings), `f30.asm`
(0x140762f30), `e50.asm` (0x140763e50), `a_*.asm` (leaf accessors). Every address is a GUI VA
unless marked "PRE" (DEF_PRE_64.exe). Engine object offsets are DEF_PRE's `LLOBJ_DATA`
(`docs/re/model-layout.md`); keyword names from `docs/re/keywords/*.md`.

Notation as in L0: `OPI` operation item, `W` GUI model wrapper (`SfStepData`, 0x278 bytes),
`E` = `W+0xe0` = engine `DB_STEP_DATA`, `S` = `E+0x750` sim control, `obj(n)` = `0x140c7ae80(W, n)`
= engine object with number n (PRE 1401d6ea0, ported) or null. `M` = `CogModel`
(`apps/moproj2mst/include/m2m/data/cogging.hpp`), `C` = `M.cog` (`SfOprCogData`, the pointer at
`M+0x540`; the header embeds it, the binary holds a pointer: `0x14075dcb0` returns `*(M+0x540)`),
`B` = `C+0xd8` billet, `D` = `C+0x1170` die, `P` = `C+0x2e20` manipulator. Field names are the
header's.

All arithmetic below is `double` unless marked; the order of each sum is the listing's.

## 0. Call tree and what reaches what

```
0x14075c260  SfOprCogging vt+0xd8 (this = OB)
  props = dynamic_cast<SfOprCoggingProperties*>(0x1405ef320(this,0))   // 0x14146f8fc, RTTI
  M = *(props+0x40)                                                     // 0x1401cc6b0
  0x140762ec0(M, 1)
     0x140766200(M,1) (M,2) (M,4)   -> B, D, P fields  ===> Cogging.MDT (writer 0x1407617e0)
     0x140763e50(M)                 -> billet centring, dies 2/3/8/9, S    ===> objects / KEY
     0x140762f30(M)                 -> manipulators 4..7                   ===> objects / KEY
  0x1405f22e0(this)                 -> S OPRNAM/SIMNAM/CURSIM (MO mode)    ===> SimCtrl KEY
```

* **Reaches the MDT** (written right after by `0x1407617e0`): only what `0x140766200` stores in
  `B`, `D`, `P` (section 2). Neither positioning function writes the cogging model; they only
  read it. `0x140766200` runs **before** the positioning, so it sees the objects as loaded.
  Fields that reach the file: Billet `GeoTyp, Length, Dia, InDia, Height, Width, Radius`; Die
  `Length, Width, Height, Radius, MovType`, and through them `SPDDIE, CYCLES, TOTSTROKE`; Manip
  `Length, Width, Height, Radius3, Width1, Height1, Radius1, Radius2`. `P+0x490` (manip parameter 8)
  is not written by the MDT writer and has no header field.
* **Reaches the objects** (and through them the KEY files the Save writes later, OB vt+0x68):
  everything in sections 4 and 5: translations / rotations of objects 1..9, object copies
  (5, 6, 7 from 4; 3, 8 from 2; 9 from 8), inter-object records copied, movement (MOVCTL) and
  REFPOS fields, stroke vectors, deletions of objects 4..7 or 8, 9, sim control MDSOBJ and SMAX,
  OPRNAM / SIMNAM / CURSIM (section 6), and the operation's slot records (`0x140060100`, which
  may reach the `.moproj`, written later by the same Save).

## 1. The viewer bounding box (`0x140c82d50` / `0x1411f5040` / `0x14121a1a0/1f0/240`)

All box reads in this step go through the **GUI viewer**, not the engine:

* `0x140c82d50(W, &n)` (`SfStepData` viewer object n): grows the list `W+0x18` of viewer objects
  (class at vtable `0x142ce9c98`, 0x87c0 bytes, ctor `0x1411f07d0`) to at least n, syncs its
  length with the engine's tail object number (`0x140c82930` with `0x140c7c220` = PRE 1401d6df0),
  returns entry n-1; 0 only when n < 1. Viewer object fields: `+0x18` engine object (null = empty),
  `+0x38` geometry renderables, `+0x40` mesh renderables, `+0x48` geometry-list renderables,
  `+0x98` 2D box, `+0xd8` 3D box.
* `0x1411f5040(vo, out, update)` (all calls here pass `update = 0`): `vo+0x18 == 0` -> a
  default (invalid, all zero) box; engine object dimension `+0x39c0 == 2` -> box from the 2D box
  `vo+0x98` (`0x1412399d0`, z = 0); else **a copy of the cached 3D box `vo+0xd8`**.
* Box type (0xe8 bytes, ctor `0x1412195a0`): `+0` valid byte, `+0x08` min, `+0x30` max, `+0x60`
  centre, `+0x58` radius etc. Points are a 0x28-byte wrapper around a Qt 4.8 `QVector3D` (three
  **float**; `x()/y()/z()` = `0x14121c8e0 / 900 / 920` return the float widened to double;
  `0x14121c6a0` / `setX/Y/Z` round the double to float). The centre is recomputed whenever a box
  is built from min/max (`0x141219dd0` -> `0x141219af0`, also after every union):
  per axis `sum = (float)((double)min + (double)max)` (`0x14121c260` stores the double sum in a
  `QVector3D`), then `centre = (float)((double)sum / 2.0)` (`0x14121c340`). Both roundings to
  float matter for the die x shifts of section 4.
* `0x14121a1a0 / 1f0 / 240(box)` = `max.x - min.x`, `max.y - min.y`, `max.z - min.z` (double
  subtraction of two float-valued doubles, max first). `0x14121a360(box)` = centre `+0x60`.
  `0x14121a700(box, out, &sx, &sy, &sz)` = corner point: each coordinate from max when its
  selector is non-zero, else from min.

**Which box is read**: the cached `vo+0xd8`, i.e. the box computed at the last
`SfStepData::updateBoundingBox()` (`0x140c8e640`, debug string "JO need to check
SfStepData::updateBoundingBox()") from the object's renderables **as they were built at the
object's last viewer rebuild**. Rebuilds happen only in `0x140c80280(W, obj, 1)` (one object) and
`0x140c7ff80(W, ...)` (all objects); the engine-side moves (`0x140890fb0`, `0x1408911f0`, router)
do not touch the viewer. So a box read after an engine move but before `0x140c80280` of that
object returns the **pre-move** box. This matters below (REFPOS of dies 2 and 3, manip 4).

Rebuild of a 3D object (`0x1411f17f0` -> `0x1411f2050`) and box rule after `0x140c80280`:

1. Geometry renderable, when `obj+0x3990` (surface flag) != 0: built from surface 0
   (`obj+0x3c00[0]`, `0x14086c7c0`): node count `0x1400c34d0`, coordinates `0x1401b2680`.
2. Mesh renderable, when `obj+0x87c` != 0 and the node check `0x1408340f0` passes: built from all
   mesh nodes (count `obj+0x880`, coordinates `0x140854af0(obj+0x8a0)` = PRE 1401808d0).
   `0x1408340f0` checks the first min(nnode, 10) nodes, `ndim = *(E+0x15f8)` components each:
   any value > 1e9 or < -1e9 fails it **and renames the object** `"*Corrupt* " + name`
   (strncpy of `0x3f - 10` chars, strcpy, strcat on `obj+0x828`), an engine side effect.
3. Renderable box (`0x141235bd0` -> `0x141257ed0` -> `0x141257be0`): vertex buffer `+0x1c0`,
   10 floats per vertex, position at floats 7..9 = `(float)` node coordinate (`0x1412568e0`, slot
   3 of vtable `0x142d19028`); per axis min/max over **all** nodes of the renderable (count
   `+0x170`, unused nodes included), compares in double on the float values.
   A second path (renderable byte `+0xf9` set: `0x141287090`, per-patch boxes) is not analysed
   (open Q2).
4. Display flags (`0x1411ee800(props, obj, &1)` -> `0x1411ee920`): flag 1 (visible) = 1;
   3D, object type `+0xc == 0` (rigid): flag 3 (geometry) = surface flag, else flag 2 (mesh) =
   has mesh; other types: mesh first, else geometry. Flag 43 (geometry-list entries) = 0. At
   most one of flags 2 / 3 is set.
5. `0x1411f5540` (from `0x140c8e640`, for every viewer object whose engine object type != 7 and
   flag 1): box = union (`0x14121a9f0`: an invalid operand is skipped; component min/max) of the
   geometry renderable box (flag 3 and surface), the geometry-list boxes (flag 43; never here) and
   the mesh renderable box (flag 2 and mesh). Nothing valid -> all zero box (extents 0).

**Port rule (converter, no viewer)**: keep per object a snapshot `VB(n)` = per-axis min/max of
`(float)` coordinates of the surface-0 nodes (rigid: surface first, else mesh) or the mesh nodes
(non-rigid: mesh first, else surface), taken (a) at the end of the operation load (every object
loader `0x14005b030/0x14005b2f0` and the material read `0x140c7cf80` call `0x140c80280` /
`0x140c7ff80(w,1,1)`, so the last rebuild sees the loaded object; open Q1) and (b) each time the
original calls `0x140c80280(W, obj(n), 1)` below. Store it as float min/max; extents are double
differences of the floats, the centre uses the two float roundings above. Viewer list growth / deletion is UI only.

## 2. `0x140766200(M, n)`: model geometry from object n (n = 1, 2, 4)

`void CogGeometryFromObject(CogModel* M, int n)`. UI-free except the box.

```cpp
void CogGeometryFromObject(CogModel* M, int n) {
  OPI* op = *(*(M+0x548) + 0x20);                // 0x1405f8d90 (owner item -> OPI)
  W*   w  = op_model(op);                         // 0x14005f770
  Obj* o  = obj(w, n);                            // 0x140c7ae80
  if (!o) return;
  int t = o->primType /*+0x3aa4, 0x1401b2620*/;
  #define PAR(k) o->primPar[k]   /* 0x140198b20: +0x3ab0 + 8k for 0 <= k < 20, else 0.0 */
  if (n == 1) {
    CogBillet& B = C->billet;                     // 0x14075dcb0, 0x14044b5d0 (+0xd8)
    if (t - 6 < 0) {                              // user geometry
      VO* vo = viewer_object(w, &n);              // 0x140c82d50
      if (vo) {
        B.geoTyp = -1;                            // +0x408
        B.length = box(vo).dx;                    // +0x410  0x1411f5040 + 0x14121a1a0
        B.width  = box(vo).dy;                    // +0x430  0x14121a1f0
        B.height = box(vo).dz;                    // +0x428  0x14121a240
        B.dia    = box(vo).dz;                    // +0x418  0x14121a240 (z extent again)
      }                                           // inDia, radius untouched
    } else {
      B.geoTyp = t - 6;
      B.length = PAR(0); B.width = PAR(1); B.height = PAR(2);   // +0x410 +0x430 +0x428
      B.dia    = PAR(3); B.inDia = PAR(4); B.radius = PAR(5);   // +0x418 +0x420 +0x438
    }
  } else if (n == 2) {
    CogDie& D = C->die;                           // 0x14075dcf0 (+0x1170); geoTyp NOT set
    D.length = PAR(0); D.width = PAR(1); D.height = PAR(2); D.radius = PAR(3);
                                                  // +0x410 +0x418 +0x420 +0x428
    D.movType = o->move.type;                     // +0xc78 = obj+0x21c8 (MOVCTL type)
    if (D.movType == 1)      D.spddie    = o+0x2250; // +0xc88: speed table T@2248 constant (+8)
    else if (D.movType == 4) { D.cycles  = o+0x2480; // +0xca0: MOVCTL type 4 e1
                               D.totStroke = o+0x2478; } // +0xc90: type 4 e0
  } else if (n == 4) {
    CogManip& P = C->manip;                       // 0x14075dcd0 (+0x2e20); geoTyp NOT set
    P.length = PAR(0); P.width = PAR(1); P.height = PAR(2);    // +0x410 +0x418 +0x420
    P.radius1 = PAR(3); P.radius2 = PAR(4); P.radius3 = PAR(5); // +0x430 +0x438 +0x428
    P.height1 = PAR(6); P.width1 = PAR(7);                      // +0x448 +0x440
    *(double*)(P+0x490) = PAR(8);                               // no header field, not in MDT
  }
}
```

* Four separate `0x1411f5040` copies of the same box are read (no difference). The box is the
  snapshot of section 1 at load (the step runs before any positioning).
* This is the inverse of the load-side `0x140766740` (`geo_d0.c`), which writes the same
  parameters into `+0x3ab0` and the type `+6` into `+0x3aa4`.
* Oracle B `42_vdie` (spec.md section 6): the re-save gives `Dia == Height` and float-looking
  values (4.0432001E+002), consistent with the box path (geoTyp -1, dz used twice).

## 3. `0x140762ec0(M, withGeometry)`

```cpp
void CogSyncAtSave(CogModel* M, bool g) {         // g = 1 from 0x14075c260
  if (g) { CogGeometryFromObject(M,1); CogGeometryFromObject(M,2); CogGeometryFromObject(M,4); }
  CogPlaceBilletAndDies(M);                       // 0x140763e50
  CogPlaceManipulators(M);                        // 0x140762f30
}
```

## 4. `0x140763e50(M)`: billet centring, dies 2 / 3 (/ 8 / 9), sim control

Locals (stack offsets of `e50.asm`): `op`[0xa8], `w`[0x180], `o1`[0x170] billet, `o2`[0x98] upper
die, `o3`[0xf8] lower die, `sizes = E+0x15f8`[0x268.. b50].

```cpp
void CogPlaceBilletAndDies(CogModel* M) {
  OPI* op = owner_opi(M);  if (!op) return;       // 0x1405f8d90
  W* w = op_model(op);     if (!w) return;        // 0x14005f770
  int* sizes = E(w) + 0x15f8;                     // 0x140c7acd0 + 0x15f8 (state-var layout)
  double org[3]  = {0,0,0};                       // [0xe0]  rotation origin
  double xaxis[3]= {1.0,0,0};                     // [0x140] rotation axis
  double d[3]    = {0,0,0};                       // [0x128] work vector
  int  nDies   = (C->die.four == 1) ? 4 : 2;      // 0x140765b00 (die+0x14a8)
  bool same    = true;                            // [0x169] copy o3 from o2
  bool four    = true;                            // [0x168] always 1
  if (M->useDifferentDies /*byte +0x14, 0x14075dc10*/) same = false;   // both branches 0
  Obj* o2 = obj(w,2); Obj* o3 = obj(w,3);
  double zTop = 0.0;                              // [0x1c0] billet max z after centring
  VO* vb = viewer_object(w, 1);                   // 0x140c82d50(&1)
  Obj* o1 = obj(w,1);
  if (!o2 || !o3 || !o1) return;
  if (vb) {
    double zMax = corner(VB1, 1,1,1).z;  double yMax = .y;  double xMax = .x;  // [0x78] [0xc8] [0x160]
    double zMin = corner(VB1, 0,0,0).z;  double yMin = .y;                      // [0x1d0] [0x208]
    double cy = (yMax + yMin) / 2.0;              // [0x1c8]
    double cz = (zMax + zMin) / 2.0;              // [0x1d8]
    if ((fabs(cy) > 0.001 || fabs(cz) > 0.001) && !M->rotationalSymmetry /*+0x15*/) {
      double t[3] = {0.0, -cy, -cz};              // sign by XOR (exact negation)
      char a = 0, b = 1;  ObjTranslate(o1, t, &a, &b);    // 0x140890fb0
      yMax = yMax - cy;  zMax = zMax - cz;
    }
    zTop = zMax;
  }
  ViewerSync(w, o1, 1);                           // 0x140c80280: rebuild VB(1) (now centred)

  // ---- upper die (object 2) -------------------------------------------------------------
  double c2x = 0.0, z2min = 0.0;                  // [0xd8] [0x1a8]
  VO* v2 = viewer_object(w, 2);
  if (v2) { z2min = corner(VB2,0,0,0).z;  c2x = centre(VB2).x; }
  double refZ = z2min;                            // [0x158]
  CogPass* ps = pass(C, 1, 1);                    // 0x1407692b0 (message 0x140b2f9d0 if invalid)
  SimCtrl* S  = op_simctrl(op);                   // 0x14005f7e0 (W ? W->E+0x750 : 0)
  double tol = 1e-5;  double dist = 0.0;          // [0x118] [0xb0]
  char flag = 1;  int nc = 0;  int* list = 0;  char tchg;   // [0x19c] [0x198] [0xc0] [0xb8]
  if (ps->diePosMethod /*+0xa8*/ != 3) {
    d = {xMax - c2x, 0.0, 0.0};                   // xMax: billet max x read above ([0x160])
    char a = 0, b = 1;  ObjTranslate(o2, d, &a, &b);
  }
  // stale read: VB2 was not rebuilt after the translation
  Vec3 c2 = centre(VB2);                          // [0x278]
  o2->refpos.mode /*+0x1ec8*/ = 1;                // REFPOS; dead else-branch: mode 2 + node
  o2->refpos.x /*+0x1ed0*/ = c2.x;                //   0x140765520(M,o2,2,0) into +0x1ee8
  o2->refpos.y /*+0x1ed8*/ = c2.y;
  o2->refpos.z /*+0x1ee0*/ = refZ;
  double dir[3]; DieDirection(M, 0, dir);         // 0x140766010: (0,0,-1)
  char one = 0;  double G[3];
  Router(E(w), o2, o1, dir, &tol, &dist, G, flag, &nc, list, &tchg, &one);  // 0x1408c0b50, rc unused

  if (!same) {                                    // ---- lower die positioned on its own ----
    VO* v3 = viewer_object(w, 3);                 // no null check
    double z3max = corner(VB3, 0,0,1).z;          // [0x2b0]
    double c3x   = centre(VB3).x;                 // [0xd8]
    d = {0.0, 0.0, 0.0};
    if (ps->diePosMethod != 3) d[0] = xMax - c3x;
    d[2] = -(zTop + z3max);                       // [0x138] = (zTop + z3max) then sign flip
    char a = 0, b = 1;  ObjTranslate(o3, d, &a, &b);
    DieDirection(M, 1, dir);                      // (0,0,1)
    char one3 = 0;  int nc3 = 0;
    Router(E(w), o3, o1, dir, &tol, &dist, G, flag, &nc3, 0, &tchg, &one3);
    refZ = z3max;  if (fabs(d[2]) > 0.001) refZ = d[2];
    Vec3 c3 = centre(VB3);                        // stale (pre-translation)
    o3->refpos.mode = 1; o3->refpos.x = c3.x; o3->refpos.y = c3.y; o3->refpos.z = refZ;
  }
  S->mdsobj[0] = 2; S->mdsobj[1] = 3; S->mdsobj[2] = 3;      // S+0x288/+0x28c/+0x290 (MDSOBJ)
  S->mdsobj4 = ps->minDieDis;                                  // S+0x298 = pass+0x18
  if (M->processType /*+0, 0x1413ee240*/ == 1) {
    S->smax[0] = 0.0; S->smax[1] = 0.0;                        // S+0x180, +0x188 (SMAX)
    S->smax[2] = M->swagingStroke;                             // S+0x190 = M+0x4f8 (0x14075dd30)
  }
  ViewerSync(w, o2, 1);
  if (o3) {                                       // always true here
    if (same) {
      bool p4=0,p5=0,p6=0,p7=0,p8=0,p9=1;
      llobj_copy_from(o3, o2, sizes, &p4,&p5,&p6,&p7,&p8,&p9);   // 0x140835430 = PRE 1401613a0
      CopyInterObject(M, o2, o3);                 // 0x1407653a0
      ObjRotate(o3, org, xaxis, 180.0);           // 0x1408911f0
    }
    SyncSlotFromObject(op, 3);                    // 0x140060100
    ViewerSync(w, o3, 1);
  }
  if (nDies == 4) {
    Obj* o8 = obj(w,8);
    if (o8) {
      if (four) { copy(o8 <- o2, flags as above); CopyInterObject(M,o2,o8);
                  ObjRotate(o8, org, xaxis, 270.0); }
      ViewerSync(w, o8, 1);
    }
    Obj* o9 = obj(w,9);
    if (o9) {
      copy(o9 <- o8);  CopyInterObject(M,o8,o9);  ObjRotate(o9, org, xaxis, 180.0);
      ViewerSync(w, o9, 1);                       // o8 == null here crashes the original
    }
    SyncSlotFromObject(op, 8); SyncSlotFromObject(op, 9);
  } else {
    DeleteObject(w, 8); DeleteObject(w, 9);       // 0x140c7ca50
  }
  // ---- die strokes and directions -----------------------------------------------------
  double s = fabs(o3->stroke[2] /*+0x22e0*/);     // [0xd0]
  if (M->processType == 0 && o2->move.type /*+0x21c8*/ == 4) {
    double h = pass(C,1,1)->minDieDis;            // +0x18
    s = o2->move.e0 /*+0x2478*/ - (zTop - h / 2.0);   // DIVSD, SUBSD x1=zTop-h/2, then e0 - x1
  }
  for (int i = 0; i < nDies; ++i) {
    Obj* o = obj(w, DieObjectNo(M, i));           // 0x140765f30: 2, 3, 8, 9 (no null check)
    double v[3]; DieDirection(M, i, v);           // i: (0,0,-1) (0,0,1) (0,-1,0) (0,1,0)
    for (int k = 0; k < 3; ++k) {
      o->move.dir[k] /*+0x21b0+8k*/ = v[k];
      o->stroke[k]   /*+0x22d0+8k*/ = s * v[k];   // s * 0.0 is -0.0 when s < 0
    }
    if (i != 0) { o->move.speedRef /*+0x22c0*/ = 2; o->move.speedRefVal /*+0x22c8*/ = 1.0; }
  }
}
```

Notes
* `corner(VB, sx, sy, sz)` = `0x1411f5040` + `0x14121a700`; `centre` = `0x14121a360`. All are
  viewer snapshots (section 1). `VB1` before the billet move is the load snapshot; the centring
  is computed on it.
* The REFPOS x/y of dies 2 and 3 are the centres of the **pre-translation** boxes (no rebuild
  between the translation and the read); z of die 2 is its pre-move min z; z of die 3 is its
  translation dz when `fabs(dz) > 0.001`, else its pre-move max z.
* `+0x22c0 / +0x22c8` are MOVCTL type 1 "1001..1999" form (`sub-1000`, value): dies 3/8/9 follow
  object 2 with factor 1.0 (MOVCTL.md). They are written whatever the object's movement type is.
* Rotations: origin (0,0,0), axis (1,0,0), degrees 180 (`0x142147108`) / 270 (`0x142245870`).
* `0x140b2f9d0` in `pass()` is the "SfOprCogData::GetPassPtr : RH_No %d : PS_No %d Invalid\n"
  message (P11 owns `0x1407692b0`); it still returns the computed pointer.

## 5. `0x140762f30(M)`: manipulators 4 .. 7

Locals (`f30.asm`): `op`[0x68], `w`[0xc8], `sizes`[0xd8], `o4`[0x60], `o1`[0x1a8], `o5`[0xd0],
`o6`[0xc0], `o7`[0x70]; `xaxis`[0xa8] = (1,0,0), `org`[0x78] = (0,0,0).

```cpp
void CogPlaceManipulators(CogModel* M) {
  OPI* op = owner_opi(M); if (!op) return;  W* w = op_model(op); if (!w) return;
  int* sizes = E(w) + 0x15f8;
  double xaxis[3] = {1.0,0,0}, org[3] = {0,0,0};
  if (!C->useManip /*byte +0xc8, 0x140765960*/) {
    DeleteObject(w,4); DeleteObject(w,5); DeleteObject(w,6); DeleteObject(w,7);   // 0x140c7ca50
    return;
  }
  double L1 = 0.0, L4 = 0.0, x1max = 0.0;         // [0x200] [0x1f8] [0x1a0]
  VO* v1 = viewer_object(w, 1);                   // VB1 = centred billet (rebuilt in 4)
  if (v1) { L1 = box(VB1).dx;  x1max = corner(VB1,1,1,1).x; }
  Obj* o4 = obj(w,4); if (!o4) return;
  CogManip& P = C->manip;
  int    spring = P.useSpring;                    // +0x8b4
  double sp     = P.startPos;                     // +0x458
  double x4max = 0.0;                             // [0x198]
  VO* v4 = viewer_object(w, 4);
  if (v4) x4max = corner(VB4, 1,0,0).x;           // (centre read into [0x1d0], unused)
  double d[3] = {(x1max - x4max) - sp, 0.0, 0.0}; // SUBSD, SUBSD
  { char a = 0, b = 1; ObjTranslate(o4, d, &a, &b); }
  double tol = 0.001, dist = 0.0; char flag = 1; int nc = 0; int* list = 0; char tchg;
  Obj* o5 = obj(w,5);
  bool couple = false;                            // [0x1b8]
  if (spring) {
    o4->move.type /*+0x21c8*/   = 5;              // MOVCTL type 5 (spring)
    o4->move.spring.k /*+0x2518*/ = P.stiffness;  // T@2510 constant (+8)
    o4->move.d4 /*+0x2598*/ = P.preload;          // +0x480
    o4->move.d3 /*+0x2590*/ = P.maxDisp;          // +0x488
    o4->move.d2 /*+0x2588*/ = 0.0;
    o4->move.dir = {1.0, 0.0, 0.0};               // +0x21b0..+0x21c0
    if (QFile::exists("MAN_COUPLE.DAT")) couple = true;   // relative: process current directory
  } else {
    MovementReset(o4 + 0x2160);                   // 0x14085f6d0 (ported, moproj2mst s1-04)
  }
  double c[3]; ObjCentre(o4, c);                  // 0x1408731c0 = PRE 1401ff360 (rc unused)
  double dA[3] = {0.0, -c[1], -c[2]};             // [0x110]
  double dB[3]; Scale(dA, -1.0, dB);              // 0x14093c1c0: dB = (-0.0, c1, c2)
  double G[3]; char one = 0;
  if (Router(E(w), o4, o1, dA, &tol, &dist, G, flag, &nc, list, &tchg, &one))
    ViewerSync(w, o4, 1);                         // VB4 rebuilt only when the router succeeded
  if (o5) {
    copy(o5 <- o4, p4..p8 = 0, p9 = 1);  CopyInterObject(M, o4, o5);
    ObjRotate(o5, org, xaxis, 180.0);
    if (couple) { o5->move.speedRef = 4; o5->move.speedRefVal = 1.0; }  // +0x22c0 / +0x22c8
    char one5 = 0;  Router(E(w), o5, o1, dB, &tol, &dist, G, flag, &nc, list, &tchg, &one5);
    ViewerSync(w, o5, 1);
  }
  VO* v4b = viewer_object(w, 4);
  if (v4b) L4 = box(VB4).dx;                      // stale when the o4 router failed
  d = { -((L1 - L4) - 2.0 * sp), 0.0, 0.0 };      // MULSD 2.0*sp, SUBSD, SUBSD, sign flip
  Obj* o6 = obj(w,6);
  if (o6) {
    copy(o6 <- o4);  CopyInterObject(M, o4, o6);
    { char a = 0, b = 1; ObjTranslate(o6, d, &a, &b); }
    o6->move.dir[0] = -1.0;                       // +0x21b0 only
    char one6 = 0;  Router(E(w), o6, o1, dA, &tol, &dist, G, flag, &nc, list, &tchg, &one6);
    ViewerSync(w, o6, 1);
  }
  Obj* o7 = obj(w,7);
  if (o7) {
    copy(o7 <- o4);  CopyInterObject(M, o4, o7);
    { char a = 0, b = 1; ObjTranslate(o7, d, &a, &b); }
    o7->move.dir[0] = -1.0;
    ObjRotate(o7, org, xaxis, 180.0);
    if (couple) { o7->move.speedRef = 6; o7->move.speedRefVal = 1.0; }
    char one7 = 0;  Router(E(w), o7, o1, dB, &tol, &dist, G, flag, &nc, list, &tchg, &one7);
    ViewerSync(w, o7, 1);
  }
  SyncSlotFromObject(op,5); SyncSlotFromObject(op,6); SyncSlotFromObject(op,7);
}
```

* The copies of 4 are made after 4 was translated, given its movement and moved by the router,
  so 5, 6, 7 inherit position, movement type, spring data and direction before their own
  changes. Object 4 itself gets no `0x140060100`.
* `dist`, `G` and `tchg` are shared by all router calls (outputs, not read here).

## 6. `0x1405f22e0(OB)` and `0x140054540(OPI)`

```cpp
void BaseAfterSave(OB* ob) {                       // base vt+0xd8
  if (APP->procType /*0x143d766b8 +0xc, 0x1402cb7a0*/ == 5 && opi_of(ob) /*0x1405ef2b0*/)
    SetSimNames(opi_of(ob));                       // 0x140054540
}
void SetSimNames(OPI* op) {
  if (APP->procType != 5) return;                  // re-tested
  SubPrj* sp = op_subproject(op); if (!sp) return; // 0x140055050
  QString name = op->name;                         // 0x140050ac0 (OPI+0x118)
  int idx = op_index(op);                          // 0x140050b20
  QString simName;  int simNo = 0;
  QList<SimRec*> l = sim_records_of_op(sp, idx);   // 0x1400b8540 (ported, P14 simcount.cpp)
  if (l.size() > 0) {
    simName = l[0]->name;                          // 0x1400654f0 (+0x10)
    simNo   = l[0]->first;                         // 0x140cbc680 (+0xc)
    SimCtrl* S = op_simctrl_via_model(op);         // 0x14005f770, 0x140c7b2d0
    if (S) {
      strncpy(S->oprnam /*+0x51*/, name.toLocal8Bit().data(), 0x40);   // OPRNAM
      S->cursim[1] /*+0xe8*/ = idx;
      strncpy(S->simnam /*+0x92*/, simName.toLocal8Bit().data(), 0x50); // SIMNAM
      S->cursim[0] /*+0xe4*/ = simNo;
    }
  }
}
```

`strncpy` (`0x141470020`) pads with zeros and does not terminate a 0x40 / 0x50-byte source; the
fields are 0x41 / 0x51 bytes, so the last byte keeps its old value (0 after construction).
DEF_GUI_MO always runs with procType 5, so this always runs in the GUI.

## 7. Helpers

### 7.1 `0x140890fb0(o, d, &flagA, &flagB)` ObjTranslate: no masked twin; the same sequence as
PRE `14021d0b0` (`def::cmd::objpos::translate`, verified): `d` null -> return; near-zero test with
1e-30 (`+0x39c0 == 2` (`0x140065800`) -> 2D compare `0x140a27c30`, else 3D `0x14093bbd0` =
PRE 14023aa90); not zero -> `*flagB` ? `0x140873f00` (PRE 1402000a0 geometry); `0x140874470`
(PRE 140200610 mesh); `0x140891090(o, d, flagA)` (PRE 14021d190 parts). All callees verified.
Every call here passes flagA = 0, flagB = 1.

### 7.2 `0x1408911f0(o, org, axis, deg)` ObjRotate: L0 spec_B section 5 (= PRE 14021d2f0 except the
die-geometry test `0x1408777e0`).

### 7.3 `0x1408c0b50` Router = PRE `14019fa00` (verified; `defcmd/objpos_ifd.hpp`
`fun_14019fa00`). Arguments in binary order: model, mover, target (billet), direction, &tol,
&dist (out), G (out), flag (byte, 1), &nc (0), list (null), &tchg (out), &flagA. The last one is
a `char*` (the port declares `int*`; it is passed on as translate's flagA); 0 in every call here.
With nc = 0 the list is never read. Its path branch prints to stdout (GUI: discarded).

### 7.4 `0x1408731c0(o, c)` ObjCentre = PRE `1401ff360` (verified, `objpos::own_centre`); its 3D
callee `0x140872630` has no masked twin but the same logic as PRE `1401fe7d0`
(`drop::axis_point`, ported): temporary surface from the mesh when only a mesh exists
(`0x140870590` = PRE 1401fc730, removed by `0x140873d30` = PRE 1401ffed0), Fortran mass
properties `0x140b427b0` (PRE 1403e8510 analogue; not matched, open Q5).

### 7.5 `0x1407653a0(M, src, dst)` CopyInterObject (GUI only, cogging):
```cpp
bool CopyInterObject(CogModel*, Obj* src, Obj* dst) {
  bool r = 0;
  DB_STEP_DATA* e = src->owner;                    // 0x1400c35b0 (+0x3a98)
  Pair* ps = pair_find_flag(e+0x13c8, src->no, 1, 2);   // 0x1408e6c40 = PRE 1401fad60
  Pair* pd = pair_find_flag(e+0x13c8, dst->no, 1, 2);
  Pair* pr = pair_find_flag(e+0x13c8, 1, dst->no, 1);
  if (ps && pd) {
    pair_copy_keep_ids(pd, ps);                    // 0x1408e5d60 = PRE 1401f9e80
    r = 1;
    for (int i = 0; i < pd->windows.count /*+0x188 +0x14*/; ++i) {
      Win* wd = winlist_nth(pd+0x188, i+1);        // 0x140a1ce30 = PRE 140269900
      if (wd && wd->orient /*+0x36c*/ == src->no) wd->orient = dst->no;
    }
  }
  if (pr) MirrorPair(pd, pr);                      // 0x140360d30 (returns when pd null)
  return r;
}
void MirrorPair(Pair* a, Pair* b) {                // 0x140360d30, GUI only
  if (!a || !b) return;
  pair_copy(b, a);                                 // 0x1408e57e0 = PRE 1401f9900
  b->master = a->slave; b->slave = a->master;      // +0x168 <- +0x16c, +0x16c <- +0x168
  if (a->dir == 2) b->dir = 1; else if (a->dir == 1) b->dir = 2;   // +0x170
}
```

### 7.6 `0x140c7ca50(W, n)` DeleteObject: `n > 0` and object count (`0x140c7c1e0`) > 0:
viewer entry n released (`0x140c82c10`, UI), engine delete `0x1408bdf10(E, n)` = PRE 14019cdc0
(object `0x14084ed10` = PRE 14017aaf0 found -> `0x1408cad60` (PRE 1401a9640), inter-object
entries of n `0x1408e6d70` (PRE 1401fae90), `0x1408352f0` (PRE 140161260), virtual delete,
`0x1408bdfc0(E,&0)` (PRE 14019ce70)); if PDIE (`0x140c7b5a0`, S+0x168) == n: PDIE = 0 then
`0x140c7b630` re-picks it (count < 2 -> `0x140823e60` (PRE 1401cabc0); count 2, or PDIE object
type 7 -> `0x140823fe0` (PRE 1401cad40) when > 0, else the first).

### 7.7 `0x140060100(OPI, n)` SyncSlotFromObject: `0x1400601b0(op, slotRec(n-1), obj(n))` and,
when the slot code is an object-manager id (> 0), the same for that manager item (`0x140061360`).
`0x1400601b0` (ported, MDT owners): type `+0xc`, name `+0x828`, has-geometry, has-mesh, `+0x2bd8`,
slot code switch to -2 / manager id by type 5 (`0x140057c60`).

### 7.8 Accessors: `0x14075dcb0` `*(M+0x540)`; `0x14075dcd0` `C+0x2e20`; `0x14075dcf0` `C+0x1170`;
`0x14044b5d0` `C+0xd8`; `0x14075dca0` `M+0x15` byte; `0x14075dc10` `M+0x14` byte; `0x14076b060`
`C+0xc8` byte; `0x1413ee240` `*(int*)M`; `0x14075dd30` `M+0x4f8`; `0x140766010(M, i, out)`
die direction table (i 0..3 as in section 4; other i: zeros); `0x140765f30(M, i)` die object
number 2, 3, 8, 9 (other: 0); `0x140765b00` die count; `0x1407692b0(C, rh, ps)` pass pointer
`*(C+0xd0) + (rh-1)*400 + 8 -> + (ps-1)*0x108`; `0x1401b2620` `o+0x3aa4`; `0x140198b20(o,k)`
`o+0x3ab0+8k` (0 <= k < 0x14, else 0.0); `0x14093c1c0(v, s, out)` `out[k] = v[k]*s`.

## 8. Engine calls

| GUI | DEF_PRE twin | port status | role |
|---|---|---|---|
| 0x140c7ae80 | 1401d6ea0 (via 0x140830270) | ported (moproj2mst MDT owners) | object by number |
| 0x140c7acd0 / 0x14005f7e0 / 0x140c7b2d0 | - | ported (moproj2mst) | E, S |
| 0x140890fb0 | (14021d0b0 by sequence) | twin verified; GUI wrapper to port | translate |
| 0x140873f00 / 0x140874470 / 0x140891090 | 1402000a0 / 140200610 / 14021d190 | verified | translate parts |
| 0x1408911f0 | (14021d2f0 except 1408777e0) | twin verified; GUI wrapper (spec_B) | rotate |
| 0x1408c0b50 | 14019fa00 | verified (defcmd) | interference router |
| 0x140835430 | 1401613a0 | ported (defmodel llobj_copy_from) | object copy |
| 0x1408731c0 | 1401ff360 | verified | own centre |
| 0x140872630 | (1401fe7d0 by logic) | ported (drop::axis_point); diff not checked | 3D centre |
| 0x14085f6d0 | none | ported (moproj2mst s1-04, express.hpp) | movement reset |
| 0x1408e6c40 | 1401fad60 | verified | pair (m, s, flag) |
| 0x1408e5d60 | 1401f9e80 | verified | pair copy keep ids |
| 0x1408e57e0 | 1401f9900 | verified | pair copy |
| 0x140a1ce30 | 140269900 | verified | window n |
| 0x140360d30 | none | **GUI only, to port** | mirrored pair |
| 0x1407653a0 | none | **GUI only, to port** | inter-object copy |
| 0x1408bdf10 | 14019cdc0 | **not in ledger, unported** | delete object |
| 0x1408cad60 / 0x1408e6d70 / 0x1408bdfc0 | 1401a9640 / 1401fae90 / 14019ce70 | **not in ledger, unported** | delete parts |
| 0x14084ed10 / 0x1408352f0 | 14017aaf0 / 140161260 | ported | find / clear object |
| 0x140823fe0 / 0x140823e60 | 1401cad40 / 1401cabc0 | verified / ported (gui_twins, no ledger row) | PDIE pick |
| 0x140854af0 | 1401808d0 | ported | node coordinates |
| 0x14086c7c0 / 0x1400c34d0 / 0x1401b2680 | - (accessors) | - | surface 0, nodes |
| 0x1408340f0 | none | **GUI only** (node check + "*Corrupt* " rename) | viewer rebuild gate |
| 0x140c80280, 0x140c82d50, 0x1411f5040, 0x14121a1a0/1f0/240/360/700, 0x140c8e640, 0x1411f17f0, 0x1411f2050, 0x1411f5540, 0x141235bd0, 0x141257be0 | none | GUI viewer: replace by the box snapshot of section 1 | bounding box |
| 0x140c7ca50, 0x140c7b630 | none | **GUI wrapper, to port** | delete object + PDIE |
| 0x140060100 / 0x1400601b0 | none | 0x1400601b0 ported (MDT); 0x140060100 to port | slot sync |
| 0x1400b8540 | none | ported (P14) | sim records of op |
| 0x1407692b0, 0x140766010, 0x140765f30, 0x140765b00, 0x140765960 | none | ported (P11) | cogging accessors |

UI-only and droppable: viewer list growth/shrink and renderable construction (keep only the box
snapshot rule), `0x1411ee800` flags (fold into the rule), the dead REFPOS mode-2 branch.

## 9. Open questions

* Q1: the box snapshot at Save is the one of the last viewer rebuild during the load; the port
  takes the loaded engine state. They differ only if a loader step changes an object's geometry
  after its last `0x140c80280` / `0x140c7ff80` (the post-load `0x1407507b0` KEY reads were not
  checked for geometry changes).
* Q2: the renderable path with byte `+0xf9` set (`0x141233950`, box `0x141287090`) was not
  analysed; what sets `+0xf9` (`0x141233800(r, 0x1411f6210(vo))`) decides whether it ever applies.
* Q3: union rule when both flags 2 and 3 are set happens only if `0x1411ee800` never ran for the
  object (flags of the ctor, all 1); not expected on the load path.
* Q4: `P+0x490` (manip parameter 8) has no field in `CogManip`; add it if the load side keeps it.
* Q5: `0x140b427b0` (Fortran mass properties of the GUI centre) assumed equal to PRE 1403e8510.
* Q6: crash paths of the original (object 8 missing with NoDies 4 when 9 exists; viewer index
  above the engine tail number) need a refusal policy in the port.
* Q7: `MAN_COUPLE.DAT` is looked up in the GUI process's current directory, not the project or
  operation folder; the converter needs a policy (treat as absent).
