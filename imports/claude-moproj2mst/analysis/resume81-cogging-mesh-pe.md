# CP81 Cogging / geometry-list native dispatch trace

## Result

The original image confirms two separate native paths: Cogging Save consumes a
cached scene box; the 3D viewer refresh can build a bound geometry-list item's
mesh rectangle into its model rectangle/box. The bounded evidence does **not**
establish the call edge that refreshes that box immediately before Cogging Save.
The geometry contribution is therefore a possible cached-state dependency, not
yet an action-equivalent Cogging Save gap.

## Immutable input and tool

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Verified local DUMPBIN: `C:\Program Files (x86)\Microsoft Visual
Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`,
SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Read-only bounded commands used `/disasm:bytes /range:START,END` over the PE:

- `0x14126d0c0..0x14126d220`
- `0x140766200..0x140766460`
- `0x1411f5540..0x1411f5680`

## Qualified native chain

The geometry-list item's native refresh is `0x14126d0c0`. Its prologue calls
`0x14126cae0` to obtain the viewer view and checks the result. The retained
continuation in `analysis/wave2-geometry-14126d0c0-disassembly.txt` and
`analysis/resume53-geometry3d-ownership.md` resolves the bound item through
the geometry list, obtains the optional `GEO2D_AND_MESH2D + 0x28` mesh, and
calls readiness `0x1408ee940`. For positive node and element counts, readiness
calls coordinate getter `0x140514f00`, then connectivity getter `0x140894330`;
only two nonnull returns proceed to builder `0x14124bec0`. That rectangle is
unioned into the item rectangle. A bound item's rectangle may then enter 3D
scene-box computation.

Viewer recomputation entry `0x1411f5540` resets the cached box, obtains each
view's settings, and checks display flag 1 and geometry/mesh mode flags before
selecting the rectangle branch. For a geometry-list rectangle it reaches the
item refresh/list path; the retained native body of list-box aggregator
`0x1411f58e0` is separately documented in `source/docs/re/moproj2mst/mdt/V/spec_V.md`.
The current source's guard at the populated mesh boundary remains qualified by
`analysis/resume53-geometry3d-ownership.md`: the optional mesh is cross-section
owned, but no writer to its node-set owner field has been established. Positive
synthetic arrays do not prove an original project reaches those getters.

The Cogging writer entry `0x140766200` reads its operation type and branches on
the selected object index. In its object-1 path, the relevant view/scene calls
are:

1. `0x1405f8d90` then `0x14005f770` obtain the intermediate project/model
objects.
2. `0x140c7ae80` is called with the operation's object index; its result is
   checked for null.
3. `0x140c82d50` is called with that view and a pointer to the object-index
   local. Its returned scene is checked for null.
4. At `0x14076630b..0x140766315`, the writer passes that scene, a local box
   address, and `r8d=0` to `0x1411f5040`.
5. The returned box is read for center/radius and used for Cogging geometry.

The zero third argument is consistent with the recovered no-recompute route:
Cogging reads the viewer's cached scene state. This bounded writer excerpt
contains no direct call to `0x1411f5540`, `0x14126d0c0`, or `0x14124bec0`.

## Smallest missing edge

The missing contract is whether the supported original Cogging Save action has
already caused `0x1411f5540` (or equivalent item refresh/recompute) for the
same viewer/model before `0x140766200` asks `0x1411f5040` for the cached box.
Static callee bodies alone do not establish this ordering or freshness. A
complete static resolution needs incoming xrefs/callers for `0x1411f5540`,
`0x14126d0c0`, and `0x140766200` through the native Save caller/event path,
including indirect Qt callback edges. DUMPBIN's byte-range output does not
provide that complete xref graph. The smallest decisive comparison after a
verified serialized xref facility exists is to follow the original Save event
caller to both refresh and Cogging writer, recording whether refresh precedes
the writer and whether both refer to the same viewer object.

Until then, retain the narrow statement: the mesh rectangle can contribute to
the cached 3D scene box, and the Cogging writer uses that cached box with
recompute disabled. Do not assert that Cogging Save itself reaches the mesh
builder, or that current refusal of an unknown geometry rectangle is a
Cogging-output mismatch. No source, GUI, build, test, or mutable analysis
database was touched.
