# Next 3D geometry-mesh increment: preserve a proven false readiness result

The old nested-owner ambiguity is partly resolved by checkpoint56 evidence.
The smallest new implementation task is to distinguish the original
**coordinate-present, ownerless-connectivity-null** readiness failure from an
unrecovered/malformed rectangle builder. On that proven false predicate, keep
the edge-derived geometry rectangle known and skip the mesh contribution.
Current `refreshGeometryItem` instead marks the item unknown and refuses its
scene box. Do not attach a fictitious model owner to make the getters pass.

Owner/reviewer: `gradient_state`. Exclusive output: this report. No source or
shared-file edits, builds, original execution, GUI control or mutable REA.
Token measurement unavailable. This is a source-qualified next task, not a new
real-project populated-mesh acceptance result.

## Reconcile the earlier audit before implementation

Read these qualifications together:

- `analysis/resume53-geometry3d-ownership.md`, SHA-256
  `ae81da87d1d1d675f7a0b8ab5809aba997b1b0098ef61c75c7e2c71692b64a8d`,
  separates cross-section allocation ownership from the nested node owner.
- `analysis/resume56-geometry3d-node-owner.md`, SHA-256
  `207599d1a9ee3bdcda086c69e4fa3f193fb6e311aa4a7b3af38b36c49a0004b1`,
  closes the null-owner/preallocated-coordinate getter profile and retains the
  missing-array fault boundary.
- `analysis/resume56-geometry3d-increment.md`, SHA-256
  `a1196800787155ad2f813a698921466ad9d63483576b54d5fd30a58f81af8910`,
  describes the **already implemented** populated rectangle branch. Do not
  assign another worker to recreate that four-node builder.

Reviewed `source/apps/moproj2mst/src/engine/viewer.cpp`, SHA-256
`9a701255523f07025f6771759561f41f71d5328bde835249690dc14cb2e0013a`,
and `tests/mdt/test_viewer_box.cpp`, SHA-256
`6976a8fab354237a6a951295804d25639aee99b6a2846eb4e2cae8ed6e593725`.
No existing source/test was changed.

## The two owner levels and exact getter fields

`GEO2D_AND_MESH2D+28` owns a separately allocated `OBJ_MESH2D` M. Its
constructor/reset/copy/delete lifetime is in
`source/port/defmodel/src/geo/geo2d_and_mesh2d.cpp` and
`src/obj_mesh2d/obj_mesh2d.cpp`. The nested sets are inline subobjects:

| State | Nested offset | Absolute from M |
|---|---|---|
| Node set N | M+8 | M+8 |
| Coordinate pointer | N+8 | M+10 |
| Node count | N+12c | M+134 |
| Node model owner | N+130 | M+138 |
| Node dimension flag | N+138 | M+140 |
| Element set E | M+148 | M+148 |
| Connectivity pointer | E+8 | M+150 |
| Element count / width | E+e8 / E+ec | M+230 / M+234 |
| Element owner | E+f0 | M+238 |
| Element dimension flag | E+f8 | M+240 |

The model owner `N+130` supplies `DB_STEP_DATA+15f8` dimensions. The known
LLOBJ owner-binding setter applies to `LLOBJ_DATA+8a0`; it does not prove a
writer for this independent cross-section mesh. The mesh constructor starts
N's model owner null; local `M+15c0` flags and nested dimension flags become2
through reset. Copy allocates/copies arrays through those local flags without
inventing a model owner. E's owner is separately null on construction.

Thus allocation ownership by the cross-section, local array dimensions and
model ownership by N+130 are three distinct facts.

## Qualified native order and new bounded inspection

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
`evidence/deform-rea-work/out/mdtV/item3.c` retains predicate `1408ee940`.
I independently read its entire PE unwind region through normal RET with
dumpbin `/disasm:bytes /range:0x1408ee940,0x1408ee9f4`. Native bytes
`1408ee940..1408ee9f5` exclusive have SHA-256
`4dd2b51200609553f5a7c0e27fa4f6883b72a29b014abd781ebf3cbb1c8d1855`.
Tool: retained VS dumpbin SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Unwind bounds do not claim exception-funclet coverage. No new asm sidecar was
written because this task owns only this report.

The native predicate executes:

1. Nonnull G+28 mesh check.
2. Node count `1400c3570` >=1, then element count `1400c3590` >=1.
3. Coordinate getter `140514f00` at `1408ee9b6`; null returns false at
   `1408ee9c8` without invoking the connectivity getter.
4. Connectivity getter `140894330` at `1408ee9d5`; null returns false at
   `1408ee9e7`. Both nonnull return1 at `1408ee9eb`.

`analysis/wave2-geometry-14126d0c0-disassembly.txt`, SHA-256
`1013440dcfcf35effd8c6c692335d5cd0ddb5067a30e95f380cee480fce645f9`,
shows a false predicate at `14126d266..d26d` jumps past the mesh builder to
`14126d3c7`. Edge sampling/rectangle reset already occurred. Width, the second
coordinate/connectivity getter calls and `14124bec0` are reached only after a
true predicate. This is a skipped contribution, not an original unsupported
status and not permission to return a zero rectangle over the edge rectangle.

The complete nullable-dimension leaf was independently checked through its
`rep ret`, extending the truncated checkpoint56 window:
`140853240..14085326c` exclusive, byte SHA-256
`6c5ed0cb9e3e2f2d4b87b33b0a703f09e40480fbe6b619b656f6a4a0596d1d1b`.
N+130 null returns null dimensions; nonnull returns owner+15f8.

Existing qualified captures show the rest:

- `wave3-editor-mds-coordinate-storage-140854af0.asm`: obtain dims, call
  `140854200(N,n,dims,key0)`, skip model sync if N+130 null, return N+8.
- `wave3-editor-mds-gui-storage-twins-complete-140854200.asm`: with positive n,
  N+8 nonnull skips direct `dims[0]` allocation; key0 skips BC-array allocations,
  then always dispatches the registered variable collection via `140b3e8e0`.
- `wave3-editor-mds-node-variable-vtables.json` and slot2 bodies
  `...-1409e49d0.asm` / `...-140b3f0e0.asm`: the canonical28 node variables
  have no id0. Their qualified methods skip resize/dimension reads for key0.
  The collection can still detach on mutable element lookup when shared.
- `resume56-geometry-connectivity-getter-140857ed0.asm`: E+f0 null skips model
  sync and directly returns E+8; null connectivity produces the predicate's
  known false result.

An existing coordinate array is therefore sufficient only with the qualified
key0-skipping registered-variable profile. Do not erase that profile or its
COW lookup effects when translating a raw pointer check into native readiness.

## Smallest implementation task

Own only `viewer.cpp` and its focused `test_viewer_box.cpp` tests for a narrow
predicate outcome before rectangle building. Distinguish at least:
`SkipContribution`, `EvaluateRecoveredBuilder`, and `Unrecovered`. A false
native readiness predicate must not use the malformed-builder unknown result.

Admit the new skip only for this explicit profile: positive counts; N+130 null;
preallocated N+8; canonical known key0-skipping variable registry; E+f0 null;
connectivity getter returns null. Use the existing recovered coordinate and
connectivity getter paths in source order, preserving the reached variable
collection lookup/detach; do not replace that prefix with a bare
`if (!elements->u008)` that silently drops native getter effects. Preflight the
current registry/type/IDs without mutating it, or require an equivalent existing
source-qualified canonical registry invariant. Unknown/custom/null/id0 variable
entries remain unrecovered before any unsafe null-dimension call.

On known skip, retain the already computed edge rectangle and `rectKnown=true`.
No builder width/stride/index/coordinate-value validation is reached by this
branch. Keep the current successful populated builder and its malformed cases
as they are, with their existing qualifications. Do not broaden to arbitrary
owner-backed synchronization, custom state variables, dimension packing,
allocation failure or a real loaded project without evidence.

In particular, positive counts with **absent coordinates and null N owner**
remain guarded: `140854244..4250` dereferences null `dims[0]` before a clean
coordinate-null predicate result is established. Missing arrays cannot all be
collapsed into a benign skip. Do not execute that faulting original/port getter
in-process just to test the boundary.

## Exact focused tests

1. Construct the existing 3D geometry-line fixture with a fresh optional
   `OBJ_MESH2D`; set positive counts and allocate coordinates but keep
   connectivity null, canonical registry and both owners null. After refresh,
   `viewerSceneBox` succeeds with the edge-only bounds x[-2,2], y0,z0.
   Assert original node/element owner pointers and array pointers are unchanged;
   no new coordinates/connectivity are allocated.
2. Same known false predicate, but coordinate values include NaN, width0 and
   nested dimension flags3. The edge-only result remains known because those
   builder-only value/width/flag reads are not reached. Use valid allocated
   coordinate storage and restore any altered fields for owned fixture cleanup.
3. Positive counts with both arrays absent and N owner null still produce the
   existing unsupported boundary, without invoking a null-dimension allocator.
   This distinguishes the coordinate fault profile from the new connectivity
   false profile. Preserve the existing malformed-coordinate test.
4. Keep coordinates nonnull/connectivity null but supply an unqualified registry,
   null variable entry, id0 entry or unknown virtual method type: reject rather
   than claiming a safe key0 getter. Restore registry ownership after each test;
   do not double-delete shared StateVar pointers.
5. Hold a COW handle lease to the canonical StateVar pointer collection, then
   exercise the proven skip. Verify the original getter's required detach and
   unchanged variable values/ids, source coordinates and both owner pointers.
   The diagnostic handle lease must not own/delete variable pointees twice.
6. Transition the same item from valid populated mesh to null connectivity and
   refresh: the earlier mesh's y-extent must disappear while edge bounds stay
   known. Restore the saved connectivity pointer before destructor cleanup.
   Then reconnect the valid array and verify the existing populated rectangle
   result returns. No stale mesh rectangle may be unioned on the false branch.
7. Preserve existing zero-count skip, populated two-coordinate rectangle,
   non-2D populated-data refusal, invalid connectivity and nonfinite-coordinate
   builder refusal tests. The relaxed known-skip case must not relax the true
   predicate's existing builder checks.

Root owns the targeted `m2m_mdt_viewer_box` build/run and required regressions.
These tests are proposed, not authored or executed in this audit. The existing
synthetic four-node fixture remains unit coverage; a retained real 3D optional
cross-section mesh with qualifying pointer/count state is still absent from
the cited acceptance evidence. Code reachability is established; real-project
population and original/candidate project parity remain unproven.
