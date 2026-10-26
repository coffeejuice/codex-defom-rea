# Packet V: per-object 3D viewer items and the cached bounding box (DEF_GUI_MO_64)

Scope: only what is needed to reproduce the box that `0x140766200` (Cogging geometry at Save) reads
through `0x140c82d50(model, &objNo)` -> `0x1411f5040(view, out, false)`.
Decompiles in this folder: `v0.c` .. `v18.c` (`v<n>a.c` = with strings), assembly `a*.asm`;
older ones in `..\view_d*.c`, `..\geo_d0.c`, `..\cur_d3a.c`.

Conventions: `m` = SfStepData model wrapper (`m+0xe0` -> engine = DEF_PRE `DB_STEP_DATA`),
`obj` = engine `LLOBJ_DATA`, `view` = viewer item (0x87c0 bytes), `set` = display settings item
(0x48 bytes). QList helpers: `140004ae0` size, `14172d7e0` append, `14014b5b0` at, `140307410`
isEmpty, `14020db90` clear, `14006daf0`/`140dfc380`/`140dfc080`/`140de9800`/`140c51520` = Java
style iterator over a QList (foreach).

**Correction to the brief:** settings flag 3 = *geometry* (`0x140834d50` = DEF_PRE `140160cc0`
object_has_geometry; rep list `view+0x38`, built from `LLOBJ_DATA+0x3c00[0]` OBJ_GEO3D), flag 2 =
*FE mesh* (`0x140834c80` = DEF_PRE `140160bf0` has_mesh, `+0x87c != 0`; rep list `view+0x40`).

## 0. Result in one paragraph (what a headless converter needs)

For a 3D object (`obj+0x39c0 != 2`) in a 3D model (`engine+0x1c == 3`), at Save the box is the one
computed by the last viewer refresh `0x140c8e640` (end of every `viewerSync 0x140c80280` and of
every rebind-all `0x140c7ff80`) by `0x1411f5540`:

```
box = empty
if (flag3 && object_has_geometry(obj)) box = box U box(geometry rep)   // vertices of OBJ_GEO3D obj+0x3c00[0]
if (flag43 && geoListItems > 0)        box = box U ...                  // flag 0x2b is 0 by default: skipped
if (flag2 && has_mesh(obj))            box = box U box(mesh rep)        // boundary polygons obj+0xbf8 over node coords obj+0x8a0
```
with the flags set by `0x1411ee800` at the object's last `viewerSync(m, obj, true)`: exactly one of
flag 2 / flag 3 is on: rigid objects (`obj+0xc == 0`) show geometry if `obj+0x3990` else mesh;
all other types show the mesh if `obj+0x87c != 0` else geometry. A rep's box is the per-component
min / max of the **float-rounded** node coordinates `(float)x, (float)y, (float)z` of every node
referenced by its faces (first 3 or 4 entries of each 4-int face, absolute value, 1-based), taken at
the time the view was last **bound** (`0x1411f17f0`); `0x1411f5540` only re-reads the reps' float
vertex buffers, never the engine. The mesh rep exists only when the first `min(nodes, 10)` nodes
have all coordinates in [-1e9, 1e9] (`0x1408340f0`, which otherwise prefixes the object name with
`"*Corrupt* "` and leaves the mesh rep empty). An empty box has min = max = (0,0,0).
`0x140766200` then stores `(double)max.c - (double)min.c` (one SUBSD of the two float values
widened to double) for c = x (+0x410), y (+0x430), z (+0x428 and +0x418).
Nothing in the viewer applies a transform; there is no scaling, no axis swap, no padding.

## 1. Data structures

### 1.1 Box3 (0xe8 bytes; ctor `1412195a0`, dtor `141219690`, reset `1412196f0`)

| Off | Type | Meaning |
|---|---|---|
| 0x00 | u8 | valid (0 = empty, `1412197d0` returns `*p == 0`) |
| 0x08 | Vec3 | min |
| 0x30 | Vec3 | max |
| 0x58 | double | radius (`14121c200(center, max)`: length of the difference, not traced deeper) |
| 0x60 | Vec3 | center |
| 0x88 | double | radius * 0.05 (`1421ec2e0`) |
| 0x90/0x98 | double | center.x - r - m / center.x + r + m |
| 0xa0/0xa8 | double | same for y |
| 0xb0/0xb8 | double | center.z - r*3.0 - m*10.0 / center.z + r*3.0 + m*10.0 (`1421ce530`=3.0, `142142078`=10.0) |
| 0xc0 | Vec3 | zero, unused here |

Vec3 (0x28 bytes; ctor `14121c480` = `QVector3D::QVector3D()` + `14121c580` (zero the three
doubles at +0x10/+0x18/+0x20, then `14121c890` = set(0,0,0))): **a Qt 4.8 `QVector3D` at +0
(three floats xp, yp, zp)** plus 3 unused doubles. Getters `14121c8e0/900/920` = imports
`QVector3D::x/y/z() const` returning `qreal` (double, the float widened). Setter `14121c6a0(v,
&dx, &dy, &dz)` = `setX/setY/setZ(double)` (stores `(float)d`). `14121c630(v, const float* p)` =
`setX((double)p[0]) ...`. `14121c510(v, &x, &y, &z)` = construct + set. `14121c4b0(v, const
double* p)` = construct + set(p[0], p[1], p[2]).

`1412196f0` reset: `valid = 0; min = max = (0,0,0); +0x58 = 0; center = 0; +0x88..+0xb8 = 0;
+0xc0 = 0`. So an empty box reads min = max = 0.

`141219610(box, &min, &max)` = construct the four Vec3 then `141219dd0`.
`141219dd0(box, &min, &max)`: `valid = 1; box.min = min; box.max = max` (0x28-byte copies);
`141219af0(box)`.
`141219af0(box)` (derived fields, order as in the binary):
```
Vec3 s;  s = (min.x+max.x, min.y+max.y, min.z+max.z)   // 14121c260: double adds, z first, stored as float
center = (s.x/2.0, s.y/2.0, s.z/2.0)                   // 14121c340 (DAT_142101520 = 2.0), stored as float
box+0x58 = 14121c200(center, max)
box+0x88 = box+0x58 * 0.05
box+0x90 = (center.x - r) - m;  box+0x98 = center.x + r + m;   (r = +0x58, m = +0x88)
box+0xa0 = (center.y - r) - m;  box+0xa8 = center.y + r + m;
box+0xb0 = (center.z - r*3.0) - m*10.0;  box+0xb8 = center.z + r*3.0 + m*10.0;
```
Only min / max matter for `0x140766200`.

`141219980(box, const float* p)` (extend by one point):
```
if (!box.valid) { box.min.setFrom(p); box.max.setFrom(p); box.valid = 1; }   // 14121c630
else { Vec3 q; q.setFrom(p); box.min = minV(box.min, q); box.max = maxV(box.max, q); }
```
(no `141219af0` here.)

`14121bf20 minV(a, b)` per component, z then y then x (assembly):
`r.c = (b.c > a.c) ? a.c : b.c` (COMISD/JBE: ties and NaN take `b`); result built with
`14121c510` (so stored as float again; the values are already floats, exact).
`14121c090 maxV(a, b)`: `r.c = (a.c > b.c) ? a.c : b.c` (ties and NaN take `b`).

`14121a9f0 union(this, out, other)` (other passed by value, destroyed at the end):
```
if (!this.valid)       out = other;                       // copy 0xe8
else if (!other.valid) out = this;
else { Vec3 lo = minV(this.min, other.min), hi = maxV(this.max, other.max);
       141219610(out, lo, hi); }                          // valid = 1, derived fields recomputed
```

### 1.2 Rect2 (0x40 bytes; 2D box): ctor `141238b90`, reset `141238d00` (valid = 0, points 0),
union `141239830` (same shape as `14121a9f0`, componentwise `141239eb0` min / `141239fb0` max),
`141238bf0(r, &min, &max)` construct valid, `141239060` derived fields. Points at +0x08 (min),
+0x18 (max), 16 bytes each (two doubles, getters `14123a2e0` = first, `14123a300` = second).

### 1.3 View item (0x87c0; ctor `1411f07d0`, QObject based, vtable `142ce9c98`)

| Off | Meaning |
|---|---|
| 0x10 | model wrapper `m` (holder, `140cbc920` sets, `140c1aef0` reads `*p`) |
| 0x18 | `LLOBJ_DATA* obj` (null = unbound) |
| 0x20 | int (0 in `1411f0a50`) |
| 0x28 | QList 2D geometry curve items (0x1c0, ctor `1412671d0`; `1411f3560(view, i)`) |
| 0x30 | QList 2D mesh items (0x298, ctor `14124bb30`; `1411f3620(view, i)`) |
| 0x38 | QList 3D geometry rep, one entry (0x530, ctor `14126dff0`, vtable `142d36708`; `1411f36d0`) |
| 0x40 | QList 3D mesh rep, one entry (0x5d8, ctor `14122b8d0`, vtable `142d00068`; `1411f3770`) |
| 0x48 | QList 3D items per entry of the object's geometry list `obj+0x3998` (0x1b0, ctor `14126c540`; `1411f3810(view, i)`) |
| 0x50..0x88 | other display lists (cleared on bind) |
| 0x98 | Rect2: 2D box |
| 0xd8 | **Box3: 3D box (the cached box)** |
| 0x1c0 | Rect2, 0x200 Box3: "all parts" boxes (`1411f5c90` / `1411f5f10`, not read by Save) |
| 0x2e8 | QVector<bool> (per mesh node; display only) |
| 0x338 | pointer (contact display, `1411fdd30`) |

The ctor (`1411f07d0`) constructs the lists, both boxes (empty), stores `m` (`140cbc920`) and calls
`1411f0a50` (obj = 0, +0x20 = 0, initialise the display lists).

Rep getters create the entry on first use:
```
GeoRep*  1411f36d0(view) { if (view+0x38 empty) append(new(0x530) GeoRep  /*14126dff0*/); return at(0); }
MeshRep* 1411f3770(view) { if (view+0x40 empty) append(new(0x5d8) MeshRep /*14122b8d0*/); return at(0); }
Item*    1411f3810(view, i) { while (size(view+0x48) <= i) append(new(0x1b0) Item(1411f6270(view)) /*14126c540*/); return at(i); }
```

### 1.4 Surface rep (common base `141233520`, vtable `142d04628`; GeoRep and MeshRep derive)

| Off | Meaning |
|---|---|
| 0x08 | int nodes per face (3 / 4) |
| 0x0c | int number of faces |
| 0x10 | int number of nodes |
| 0x18 | `int*` faces (4 ints per face) |
| 0x20 | `double*` node coordinates (3 per node) |
| 0x28 | owned copy of the faces (GeoRep only) |
| 0xf9 | u8 **patch mode**: 1 (ctor `141233720`) = patch set path; 0 = sub-object path |
| 0x100 | patch set (kind 1, used for the box) |
| 0x1f8 | patch set (kind 2, display only) |
| 0x2f0 | sub-object (0x240, ctor `141254bb0`) used when +0xf9 == 0 |
| 0x2f8 | Box3: the rep's cached box |
| 0x4a8 | GeoRep: `OBJ_GEO3D*` source |
| 0x510 | MeshRep: `LLOBJ_DATA*` source |

Patch set (0xf8, init `1412822b0`): +0x00 `double* coords`, +0x08 `float* coords` (null here),
+0x10 `int* faces`, +0x18 per-face normals (24 bytes each), +0x28 patch array (200 bytes per
patch, preceded by a count), +0x30 int nodes, +0x34 int faces, +0x38 int face stride (4),
+0x44 kind, +0x60 per-face patch id, +0xc4 float feature angle 30.0 (`1429f9168`), +0xc8 float
cos/threshold, +0xdc / +0xe0 int nodes per face, +0xe8 int patch count. Patch (200 bytes):
+0x00 int face count, +0x08 `int*` face numbers (1-based), +0xa8 nodes per face, +0xb0 float
QVector (6 floats per face corner: normal x,y,z, position x,y,z), +0xc0 4 or 7.

### 1.5 Display settings item (0x48; `1411ee5b0(set, m)`)

```
set+0x40 = m;  1411ee610(set):
    set[0] = 0;
    1411ee670(set, &true):  set+4 = 0;
                            for i in 0..0x2d: set[8+i] = 0;
                            set[8] = set[9] = set[10] = set[11] = 1;         // flags 0,1,2,3
                            set[0x10] = DAT_143d781e4; set[0x11] = DAT_143d781e5;  // flags 8, 9
    1411f0750(set): set[0x3c] = set[0x3d] = 0;
    *(float*)(set+0x38) = 0.5f;                                   // DAT_1421ebc38
```
`1411eea40(set, &i)`: `(i < 0 || i > 0x2d) ? 0 : set[8 + i]` (46 flags).
Defaults are constants except flags 8 / 9, which come from the global display-preferences object
`0x143d781e0` (bss; built by the CRT initializer `141ca0b50` -> `1402d5520` -> `1402d5570`:
`+0 = 0; +4 = +5 = +6 = 1; ...`), so 1 in a fresh GUI; the preferences dialog `140d16ae0`
may change them (not traced, not used by the box). No QSettings is read for flags 0..3 / 0x2b.
Flags used by the box: 1 (visible: refresh does nothing for the view when 0), 2 (mesh), 3
(geometry), 0x2b (geometry-list items, default 0).

## 2. Creating and binding the views

### 2.1 `0x140c80280 viewerSync(m, obj, bool flags)` (callers: every object loader, see spec_B)
```
if (!obj) return;
int n = 140c7c220(m);            // 0 when no engine, else 1408301c0(eng) = DEF_PRE 1401d6df0 last_object_number
140c82930(m, &n);                // views list = n items
View* v = 140c82d50(m, &obj->no /*obj+8*/);
if (!v) return;
1411f17f0(v, obj, true);         // bind
Settings* w = 1411f1a10(v);
if (w && flags) { bool t = true; 1411ee800(w, obj, &t); }
140c8e640(m);                    // refresh
```

### 2.2 `0x140c82930 resizeViews(m, const int* n)`
```
if (size(m+0x18) == *n) return;
while (*n < size(m+0x18)) 140c82b70(m);                      // remove the last view
while (size(m+0x18) < *n) append(m+0x18, new(0x87c0) View(m)); // 1411f07d0
140c82a20(m);                                                // settings list follows
```
`0x140c82a20`: `while (size(m+0x20) < size(m+0x18)) append(m+0x20, new(0x48) Settings(m))`
(`1411ee5b0`); never shrinks, so settings (and their flags) survive view deletion.

### 2.3 `0x140c82d50 view(m, const int* no)`
```
if (*no < 1) return nullptr;
while (size(m+0x18) < *no) append(m+0x18, new(0x87c0) View(m));
int c = size(m+0x18); 140c82930(m, &c);    // no-op (same size): settings are NOT grown here
return at(m+0x18, *no - 1);
```

### 2.4 `0x1411f1a10 settings(view)` and `0x140c832d0`, `0x140c83080`
```
Settings* 1411f1a10(View* v) {
    SfStepData* m = 1411f1980(v);              // *(v+0x10)
    if (!m) return nullptr;
    int i = 1411f1ab0(v);                      // v->obj ? v->obj->no (obj+8) : 0
    Settings* s = 140c832d0(m, &i);
    if (!s) { i = 140c83080(m, v); s = 140c832d0(m, &i); }
    return s;
}
Settings* 140c832d0(m, const int* i) {         // 1-based, existing entries only
    if (0 < *i && *i <= size(m+0x20)) { /* dead grow loop */ return at(m+0x20, *i - 1); }
    return nullptr;
}
int 140c83080(m, View* v)                       // 1-based position of v in m+0x18, 0 when v is null
                                                // or the list is empty (decompile partly garbled; only
                                                // a fallback when the object number has no settings)
```

### 2.5 `0x1411f17f0 bind(view, obj, bool flag)`
```
if (!obj) { view->obj = 0; 1411f0b50(view, 1); 1411f4e80(view); return; }
1411f0b50(view, 0);              // obj = 0 and delete every display item: 1411f10b0 deletes +0x38 and +0x48,
                                 // 1411f1570 +0x40, 1411f0c30 +0x28, 1411f0e70 +0x30, ... (fresh reps per bind)
view->obj = obj;
if (!140065800(obj))  1411f2050(view);    // 3D object (obj+0x39c0 != 2)
else                  1411f1d80(view);    // 2D object
int mode = (m && 1411f... m->vt? 141201910(m) != 0) ? 141201910(m) : 141696820();   // colour mode (display only)
1411f28f0(view, mode);           // colours: 1411f2430 / 1411f2970 / 1411f2b60 (display only, no box)
if (view+0x338) 1411fdd30(view, flag);    // contact display (no box)
1411fc900(view);                 // every byte of QVector view+0x2e8 = 1 (display)
```
UI-only and droppable for the converter: `1411f28f0`, `1411fdd30`, `1411fc900`, `1411f4e80`.

### 2.6 `0x1411f2050 bind3D(view)` (the bind-time box; superseded by the refresh box, §3)
```
1412196f0(view+0xd8);                                         // reset
if (140834e20(obj)) {                                         // obj+0x3990 (3D geometry present)
    GeoRep* g = 1411f36d0(view);
    int z = 0; OBJ_GEO3D* s = 14086c7c0(obj, &z);             // obj+0x3c00[0]
    g->patchMode /*+0xf9*/ = 1411f6210(view);                 // !(m+0x258), 1 when m is null
    14126e130(g, s);                                          // build, computes g+0x2f8 (§4.1)
    view+0xd8 = union(view+0xd8, g+0x2f8);
}
if (obj+0x87c != 0 && 1408340f0(obj)) {                       // has_mesh && coordinates plausible
    141201e90(view+0x2e8, obj+0x880);                         // QVector<bool>.resize(nodes)  (display)
    MeshRep* r = 1411f3770(view);
    r->patchMode = 1411f6210(view);
    14122bb60(r, obj);                                        // build, computes r+0x2f8 (§4.2)
    14122d9e0(r, view+0x2e8);                                 // node flag pointer (display)
    view+0xd8 = union(view+0xd8, r+0x2f8);
}
clear(view+0x48);
int n = 14086c580(obj);                                       // geometry list count (obj+0x3998)
for (i = 0; i < n; ++i) 14126c760(1411f3810(view, i), view, 14086c0e0(obj, i), i, 0.01);
1411f3990(view); 14126c010(1411f3ad0(view)); 1411f3ee0(view); // s3328 / s3458 surfaces (display)
1411f5f10(view);                                              // "all parts" box view+0x200
```
`1411f6210(view)`: `b = 1; if (m = *(view+0x10)) if (m+0x258) b = 0; return b`. `m+0x258` is
0 from the ctor (`140c7a870` -> `140c91200`) and set to 1 only by `140c911e0` from `140d44320`
(a separate editor's model, callers `140d42xxx..140d460f0`); on an operation model it stays 0, so
**patch mode = 1** on the Save path.

### 2.7 `0x1411ee800 setDefaultFlags(set, obj, const bool* reset)`
```
bool mesh = false, geo = false;
if (!140065800(obj)) {                                // 3D object
    if (!140835070(obj)) {                            // obj+0xc != 0 (not rigid)
        if (140834c80(obj)) mesh = true;              // obj+0x87c != 0
        else if (140834e20(obj)) geo = true;          // obj+0x3990
    } else {                                          // obj+0xc == 0
        if (140834e20(obj)) geo = true;
        else if (140834c80(obj)) mesh = true;
    }
    1411ee920(set, &mesh, &geo, reset);
} else {                                              // 2D object
    if (140834c80(obj)) mesh = true;
    if (140834db0(obj)) geo = true;                   // 2D curves present
    1411ee990(set, &mesh, &geo, reset);
}
1411ee920 == 1411ee990:  1411ee670(set, reset);  set[9] = 1; set[10] = mesh; set[11] = geo; set[0] = 1;
```
So after it: flags 0,1 = 1; flag 2 = mesh; flag 3 = geometry; 8,9 = preferences; all others 0.

## 3. Refresh: `0x140c8e640` (and `0x140c8efe0`, identical box part)

```
if (140c7c0e0(m)) {                                   // engine+0x1c == 2 (2D model)
    141238d00(m+0x108);                               // model 2D rect
    foreach View* v in m+0x18:
        if (v && 1411f1b80(v) /*obj && obj+0xc != 7*/ && (s = 1411f1a10(v)) && s.flag1) {
            1411f5250(v);  m+0x108 = union(m+0x108, v+0x98);
            if (m+0x10 && *(m+0x10)+0x202 && (140c7b370(m) == 1 || == 3))   // axisymmetric: add the mirror
                ... add rect (0,0)-(1,1) logic to m+0x108 (model rect only)
        }
} else if (140c7c0a0(m)) {                            // engine+0x1c == 3 (3D model)
    141238d00(m+0x108); 1412196f0(m+0x148);           // model boxes
    foreach View* v in m+0x18:
        if (obj = v->obj && 140834fa0(obj) /*+0xc != 7*/) {
            if (!(s = 1411f1a10(v))) qDebug(UNK_1428fe258);          // no settings: box left as bound
            else if (s.flag1) {
                1411f5540(v);                                       // recompute v+0xd8
                m+0x148 = union(m+0x148, v+0xd8);
                if (Item* it = 1411f3810(v, 0)) { it->vt[9](it); m+0x108 = union(m+0x108, it rect); }
            }
        }
}
```
Note `1411f3810(v, 0)` *creates* item 0 of `v+0x48` when the list is empty (side effect: from then
on `1411f1c40` (size(v+0x48) > 0) is true, but flag 0x2b is 0 so the box is unaffected).
`0x140c8efe0` (caller `140d118e0`) is the same without that item step.

### 3.1 `0x1411f5540 recompute3D(view)`
```
1412196f0(view+0xd8);
Settings* s = 1411f1a10(view);
if (!s) return;
if (s.flag[3] && 1411f1c00(view)) {             // obj && 140834d50(obj) (object_has_geometry)
    GeoRep* g = 1411f36d0(view); 141235bd0(g);  // recompute g+0x2f8 from its buffers
    view+0xd8 = union(view+0xd8, 141235b60(g) /*copy of g+0x2f8*/);
}
if (s.flag[0x2b] && 1411f1c40(view)) {          // size(view+0x48) > 0
    1411f5a90(view);                            // vt+0x48 on each item (refresh)
    view+0xd8 = union(view+0xd8, 1411f58e0(view, tmp, 0));  // union of 14126d3e0(item) boxes
}
if (s.flag[2] && 1411f1bc0(view)) {             // obj && 140834c80(obj) (has_mesh)
    MeshRep* r = 1411f3770(view); 141235bd0(r); // creates an empty rep if none was built
    view+0xd8 = union(view+0xd8, 141235b60(r));
}
```
`140834d50` (= DEF_PRE `140160cc0`): `owner = obj+0x3a98; if (owner && owner+0x1c == 2)
return 140834db0(obj) (2D curves); return obj+0x3990`.

### 3.2 `0x1411f5210` / `0x1411f5040`
```
void 1411f5210(View* v) { if (!140065800(v->obj)) 1411f5540(v); else 1411f5250(v); }
Box3* 1411f5040(View* v, Box3* out, bool recompute) {
    Box3 b;                                    // empty
    if (!v->obj) { *out = b; return out; }
    if (recompute) 1411f5210(v);
    if (!140065800(v->obj)) b = v+0xd8;        // 3D
    else b = 1412399d0(v+0x98);                // 2D: rect -> box
    *out = b; return out;
}
```
`0x140766200` passes `recompute = false` four times (the Save path never recomputes).

## 4. Rep build and rep box

### 4.1 Geometry rep: `14126e130(g, OBJ_GEO3D* s)`
```
g+0x4a8 = s;  14126e260(g) /*empty*/;
if (s) {
    int nNodes = 1400c34d0(s);   // size(s+0x10) / 3          = DEF_PRE 1400034d0 node_count
    int nFaces = 1400c3500(s);   // size(s+0x18) / 4          = DEF_PRE 140003500 elem_count
    int type   = 140929bd0(s);   // = DEF_PRE 1402fabc0 (not ported): 0 if nFaces == 0; else 3 when
                                 //   f[0] == abs(f[3]) and (nFaces < 2 or f[4] == abs(f[7])), else 4
    int* faces = 1401b26a0(s);   // &(s+0x18)[0]   (DEF_PRE 1402fe9d0)
    double* xyz = 1401b2680(s);  // &(s+0x10)[0]   (DEF_PRE 14026a3c0)
    14126e1e0(g, &type, &nFaces, &nNodes, faces, xyz):
        141233840(g, type, nFaces, nNodes, faces, xyz, true);
        141235bd0(g);                          // box
        14126e780(g);                          // per-face centres / normals buffer (display)
}
```

### 4.2 Mesh rep: `14122bb60(r, obj)`
```
r+0x510 = obj;  bool f = 0; 141230490(r, &f) /* r+0x548 = 0 */;
if (obj) {
    double* xyz = 140854af0(obj+0x8a0);          // = DEF_PRE 1401808d0 node_set_coords (ported)
    14122bc00(r, &obj+0x89c, &obj+0xbf0, &obj+0x880, *(int**)(obj+0xbf8), xyz):
        141233840(r, *(obj+0x89c) /*nodes per face: 3 tet, 4 brick (DEF_PRE 140165500)*/,
                     *(obj+0xbf0) /*boundary polygons*/, *(obj+0x880) /*nodes*/,
                     *(obj+0xbf8) /*polygons, 4 ints each*/, xyz, false);
        141235bd0(r);
        r+0x528 = xyz; r+0x530 = nodes; r+0x534 = polys; r+0x538 = nodesPerFace; r+0x540 = faces;
}
```

### 4.3 `141233840(rep, type, nFaces, nNodes, faces, xyz, bool copy)`
```
if (rep+0xf9 == 0) 141233cd0(rep, ...);      // sub-object: 141233dc0(rep, 1) creates 0x240 object,
                                             // 1412553a0(sub, type, nFaces, nNodes, faces, xyz, copy ? 2 : 1, 0)
else               141233950(rep, ...);
141235d20(rep);  141236010(rep);  return 1;  // display state
```
`141233950` (patch mode):
```
int* f = faces; int t = type;
if (copy) {                                  // geometry rep
    rep+0x28 = vector<int>(nFaces*4, 0); f = data(rep+0x28);
    for i < nFaces: f[4i..4i+2] = faces[4i..4i+2]; f[4i+3] = abs(faces[4i+3]);
    t = (f == nullptr) ? 3 : (f[0] == f[3] ? 3 : 4);   // only the first face decides
}
141234730(rep, type, nFaces, nNodes, f, xyz);  // stores +0x08..+0x20, vt+0x20 (resize flag vectors)
141233c90(rep);                                // clear both patch sets
set1 = rep+0x100: coords = xyz; faces = f;
141282560(set1, nNodes, nFaces, t, /*kind*/1, 0);
141282be0(set1);                               // per-patch vertex buffers (the box source)
set2 = rep+0x1f8: coords = xyz; faces = f; 141282560(set2, nNodes, nFaces, t, 2, 0);  // no buffers
```
Mesh rep (`copy == false`): `f` is the engine array `obj+0xbf8` itself (see §6, side effect).

`141282560(set, nNodes, nFaces, nodesPerFace, kind, p6)`: returns 0 without building when both
coordinate pointers are null, faces is null, `nNodes < 3` or `nFaces < 1` (patch count stays 0
-> empty box). Otherwise: threshold `set+0xc8 = (float)cos?(set+0xc4 * pi/180)` (`14528d490`,
30 degrees), face normals `141284970` (makes every negative 4th face entry positive **in place**),
node->face incidence `1412865a0` (uses `abs` of the first nodesPerFace entries), flood fill of
faces into patches (`141284bc0` first unassigned face, `141284c30` recursive with depth 1000 and an
explicit stack beyond; kind 1 stops at edges whose normal test fails, kind 2 = connected
components), `1412851d0` builds `set+0x28` (patch i gets all faces with id i+1: every face is in
exactly one patch), `1412833c0` per-patch centre / mean normal. Positions are not changed.

`141282be0(set)`: for p = 1..count: `1412813a0(patch p, &faceCount, faceList, set->faces,
&set->nodesPerFace /*+0xdc*/, &set->stride /*+0x38 = 4*/, &set->nNodes /*+0x30*/, set->coords,
set->faceNormals)`:
```
buf(patch+0xb0) = QVector<float>(faceCount * npf * 6, 0.0f);
patch+0xa8 = npf;  if (npf == 3) patch+0xc0 = 4; else if (npf == 4) patch+0xc0 = 7; else return;
                                         // (other npf: buffer stays zeros)
nodeNormal = accumulate face normals per node, normalise (display only)
k = 0;
for each face F in the patch (1-based), for j < npf:
    n = faces[(F-1)*stride + j] - 1;      // already made non-negative by 141284970
    buf[k+0..2] = (float)nodeNormal[n].xyz;
    buf[k+3] = (float)coords[3n]; buf[k+4] = (float)coords[3n+1]; buf[k+5] = (float)coords[3n+2];
    k += 6;
```

### 4.4 Rep box `141235bd0(rep)`
```
if (rep+0xf9 == 0) {                                     // sub-object path (not on the Save path)
    if (Sub* s = 141233dc0(rep, false))  rep+0x2f8 = 141257ed0(s, tmp, true);
        // 141257be0(s): over s+0x170 vertices of 10 floats at s+0x1c0, position = floats 7,8,9:
        //   vertex 0: max(+0xe0) = min(+0xc8) = (double)p; else if (min <= v) { if (max < v) max = v; } else min = v;
        //   also centroid (+0x100) and +0xf8 = length(max-min)/2.0
        // 141257ed0: Box3 from min = s+0xc8, max = s+0xe0 (141219610)
} else
    rep+0x2f8 = 141287090(rep+0x100, tmp);
```
`141287090(set, out)`:
```
if (set+0xe8 == 0) { out = Box3(); return; }            // empty
Box3 acc;                                               // empty
for p = 1..set+0xe8:
    141283140(set, p, &x)  (bounds-checked patch fields, qDebug UNK_142d4ce18 when p out of range)
    Box3 b = 1412820d0(patch p);
    acc = union(acc, b);
out = acc;
```
`1412820d0(patch, out)`:
```
Box3 b; 1412196f0(&b);
int n = size(patch+0xb0); float* v = data(patch+0xb0);
for (i = 0; i < n / 6; ++i) 141219980(&b, v + 6*i + 3);   // the position triple
141219af0(&b);                                            // derived fields (valid unchanged)
out = b;
```
Net effect: rep box = componentwise min / max over `(float)coord` of every node referenced by any
boundary face; floats compare exactly as the doubles they widen to.

## 5. 2D path (brief)

`1411f1d80 bind2D(view)`: reset `view+0x98`; if `140834db0(obj)` (2D curves, DEF_PRE `140160d20`):
for each curve i < `14086c4b0(obj)` (= DEF_PRE `140198310`): `c = 14086c460(obj, &i)`,
item = `1411f3560(view)`, `141267410(item, c, 0.01)`, union of the item rect `1412510a0` (item+0xb0)
into `view+0x98`; if has_mesh and `1408340f0(obj)`: mesh item `1411f3620(view, 0)`,
`14124be10(item, obj)`, union. Then `1411f5c90` ("all parts" rect +0x1c0).
`1411f5250 recompute2D(view)`: reset `view+0x98`; `s = settings`; if `s.flag3 && 1411f1c00`:
for each curve item: `item->vt[9]()` then union of its rect; if `s.flag2 && 1411f1bc0 && (it =
1411f3620(view, 0))`: `it->vt[9]()`, union.
`1412399d0(rect, out)`: `Box3 b; min = Vec3(rect.min.first, rect.min.second, 0.0)` (`14123a610`:
x = `14123a2e0`, y = `14123a300`, z = 0, stored as floats), `max` likewise from `rect+0x18`;
`141219dd0(b, min, max)` -> valid = 1 even for an empty rect. The rect builders (`141267410`,
`14124be10`) were not traced: the 2D curves' point rule is open.

## 6. Side effects on the engine (matter for a faithful save)

1. `141284970` (mesh rep, `copy == false`) rewrites `obj+0xbf8` polygons: every negative 4th
   entry `p[4i+3]` becomes `-p[4i+3]`, in the engine array, at every bind of a 3D object with a
   plausible mesh. The geometry rep works on a copy (no change to OBJ_GEO3D).
2. `1408340f0` (= DEF_PRE `140160060`, not ported, not in the ledger): `ok = true; dim =
   *(int*)(owner+0x15f8); x = node_set_coords(obj+0x8a0); for (i = 0; i < obj+0x880 && i <= 9;
   ++i) for (j = 0; j < dim; ++j) { d = x[i*dim + j]; if (1e9 < d || d < -1e9) { ok = false; break; } }`
   (the break leaves only the inner loop); if `!ok`: `char tmp[64]; strncpy(tmp, name /*obj+0x828*/,
   0x3f - strlen("*Corrupt* ")); strcpy(name, "*Corrupt* "); strcat(name, tmp)` (string at
   `0x1427a8098`, 10 chars; tmp is not terminated when the name has >= 53 chars: read past the
   copied bytes, as in the binary); returns `ok`. Called from `1411f2050` and `1411f1d80` at every
   bind, so a corrupt object gets the prefix once per bind (repeatedly).

## 7. When the box changes between File > Open and File > Save

* Writers of `view+0xd8`: ctor (empty), `1411f2050` (bind3D), `1411f5540` (recompute). Readers
  that recompute first: `1411f5040(.., true)` from `1402cb600`, `1404f3a10`, `1404f6cc0`,
  `1404fb900` (interactive tools; not on the Save path).
* Bind (`1411f17f0`) callers: `140c80280` viewerSync (every loader in spec_A/B/C: `14005b030`,
  `14005b2f0`, `14005bd30`, `140c7c950`, `140c7ce40`, `140c7cd90`, also `140762f30` / `140763e50`
  at Save after positioning), `140c7ff80` rebind-all (material KEY read `140c7cf80`, restart load
  `140057230`, ...; binds the i-th view to the i-th engine object `1408302a0(eng, i)`, flags only
  when its `reset` argument is true, then refresh), `140c7f230` appendDispObj (bind + flags, **no
  refresh**: callers `14019dcf0`, `1401b3ac0`, `14039e5d0`, `1415ce500`, `1415d06a0`,
  `14170f6b0`, `141710020`, `141732ba0`), and UI paths (`1402a7160`, `1402bf540`, ...).
* Refresh (`140c8e640`) callers: `140c80280`, `140c7ff80`, `14038e2b0`, `140c8a320..140c8a3e0`,
  `140c8cd50`, `140cfd750`, `140cfe9c0`.
* So on the operation load path every object load ends with bind + flags + refresh of the whole
  model; the state at Save is the §0 formula evaluated on the reps of each view's last bind, with
  the flags of the object's last `1411ee800`. Exceptions where the bind box (geometry U mesh, no
  flag filter) survives: an object of type 7, a view without a settings item, a model whose
  `engine+0x1c` is neither 2 nor 3, or a last bind through `140c7f230`.
* Engine coordinates changed after the last bind (e.g. positioning at Save) are not seen until
  the next bind; `140762f30` / `140763e50` call viewerSync themselves (see their spec).

## 8. Engine calls

| GUI | DEF_PRE twin | Port status | Use |
|---|---|---|---|
| 140834d50 | 140160cc0 | verified (`defdbio/gendb/p4_02b/object.cpp` object_has_geometry) | flag 3 test |
| 140834c80 | 140160bf0 | verified (`gendb/frame.cpp` has_mesh) | flag 2 test |
| 140834e20 | 140160d90 (sig not unique; same body) | verified (`geo_kw`) | `obj+0x3990` |
| 140834db0 | 140160d20 | verified (geometry_has_edges) | 2D curves |
| 140834fa0 | 140160f10 | ported | type != 7 |
| 140835070 | 140160fe0 | **not in ledger** (trivial: `obj+0xc == 0`) | rigid test |
| 1408340f0 | 140160060 | **not in ledger, not ported** | mesh plausibility + rename |
| 14086c7c0 | 140198620 (sig not unique) | verified | `obj+0x3c00[i]` |
| 14086c580 | 1401983e0 | ported | geometry list count |
| 14086c0e0 | 140197f40 | verified | geometry i |
| 14086c540 / 14086c4b0 / 14086c460 | 1401983a0 / 140198310 / 1401982c0 | verified | 2D curves |
| 140854af0 | 1401808d0 | ported | node coordinates |
| 1400c34d0 / 1400c3500 | 1400034d0 / 140003500 | ported (OBJ_GEO3D node_count / elem_count) | sizes |
| 140929bd0 | 1402fabc0 | **not in ledger, not ported** | face node count 3/4 |
| 1401b2680 / 1401b26a0 | 14026a3c0 / 1402fe9d0 | ported / not-needed | data pointers |
| 140065800 (140065840) | 140158d00 | verified | `obj+0x39c0 == 2` |
| 140065560 / 14035b400 | none unique (`engine+0x1c == 2 / 3`) | inline | model dimension |
| 1408301c0 | 1401d6df0 | ported (`gui_twins/step_utils.cpp`) | last object number |
| 1408302a0 | not checked | - | i-th object (rebind-all) |
| 14528d490, 1417f4b4c, 14093xxxx, 140944xxx | CRT `cos`-like / `abs` / vector math | - | display math (normals) |

GUI-only code to port for the box (no DEF_PRE twin): Box3 / Vec3 helpers (`1412195a0`,
`1412196f0`, `141219610`, `141219dd0`, `141219af0`, `141219980`, `14121a9f0`, `14121bf20`,
`14121c090`, `14121c630`, `14121c6a0`), `1412820d0`, `141287090`, `141235bd0`, `1412813a0`
(position part), `141282560` (only: which faces exist and the early returns; every face lands in a
patch), `141284970` (in-place abs side effect), `141233950`, `14126e130`, `14122bb60`,
`1411f2050`, `1411f5540`, `1411ee800`/`1411ee670`, `1411f5040`, plus the Qt import
`QVector3D` (float storage, `x()` returns double).

## 9. Open questions

1. `14121c200` (radius) and `141239060` (2D derived fields) not traced; not read on the Save path.
2. The sub-object path (`rep+0xf9 == 0`, `1412553a0`) is only reachable for a model whose
   `+0x258` was set by `140d44320`; its vertex set (all nodes or face nodes) was not checked.
3. `140c83080` decompile is partly garbled (Java-style iterator with removed blocks); only the
   fallback when an object number has no settings item uses it.
4. 2D: the point rule of the curve / mesh items (`141267410`, `14124be10`, item vt+0x48) is open.
5. Callers of `140c7f230` (bind without refresh) were not checked against the operation-load path.
6. The 2D refresh's axisymmetric extra rectangle (`14123a140` with 0 / 1.0, `1404ec860`) only
   changes the model rect `m+0x108`, not the views; details not written out.
