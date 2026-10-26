# CP63 geometry owner admission after getter/COW recovery

The next positive slice can admit an **already bound actual model owner on the
native synchronization fast path**, with coordinates already allocated. Missing
coordinates remain outside this slice. A new native comparison also disproves
using the port's unconditional node-sync gate as evidence for arbitrary owners.

Owner: `gradient_state`. Writes are limited to this report and five uniquely
named bounded immutable asm captures. No source/build/test/GUI/mutable REA action
was performed. Token measurement is unavailable.

## Current qualified boundary

Read the CP53/56/60/62 geometry qualifications, especially
`resume56-geometry3d-node-owner.md`, `resume60-geometry3d-next.md` and
`resume62-geometry-owner.md`. CP62 frozen viewer SHA-256 remains
`ec88fb2967d3fcc05998df2eaaf11b8b58a2a4701771250138f1e88699efa1ea`;
viewer-test SHA-256 remains
`766a12aad9982e55c782ec6e7f4b484c0542c08ae076d1f5140fcd980b0a042b`.
I did not execute validation or infer success from a source freeze.

Let M be the actual optional cross-section mesh; N=M+8 and E=M+148. CP62 admits
native getter rounds for preallocated N+8, both model owners null, exact
canonical28 registry IDs/types, and positive node/element counts. Connectivity
E+8 null gives a known skip after one coordinate dispatch; populated arrays give
two getter rounds and the recovered packed rectangle builder.

`OwnerlessGeometryReadiness::Unqualified` currently falls through to the legacy
raw builder. Thus populated arrays with nonnull owners or an unqualified registry
can still produce a raw rectangle without the native getter effects. This is an
existing admission gap, not proof of those native branches. Null-connectivity
fixtures for such profiles remain refused. A future guard change must explicitly
distinguish adapter refusal from the original predicate's known-false skip.

## New immutable native evidence

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Tool: VS dumpbin, SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Commands were `/disasm:bytes /range:START,(END-1)` on the retained absolute PE.
The final captures end at instruction boundaries; initial truncated prefix
outputs were replaced before this report was frozen.

| Artifact under `analysis/` | Native interval, end exclusive | Native-byte SHA-256 | Capture SHA-256 |
|---|---|---|---|
| `resume63-geometry-node-owner-sync-prefix.asm` | `14082dca0..14082dcd2` | `f6f8009d4356b5a011ad2c04365ad487bee6bc24b3221348afcab3c8e114109e` | `ac35549d00f26ebab047a784408ae491b4c2be03cc3faa4f27bea365c561c342` |
| `resume63-geometry-element-owner-sync-prefix.asm` | `14082ebf0..14082ec2d` | `a679ca1f65f4f1745bb296f338e05aef0e0ed24c63c517ee2a20c62343c628b6` | `dee050ef193fb174aef4ab866083e9af6993848dd8f7ca0c1e15ceaff74d58ba` |
| `resume63-geometry-owner-up-to-date-leaves.asm` | `14082bf10..14082bf7d` | `f7b93556d18ccaf99eff5cb6c83529006c778694078804b653038941a6f18a5e` | `353c8eeabc1cfe0c04c2ecbc87339875672dc9a81c69d6a19136d2c3aa314c65` |
| `resume63-geometry-node-sync-fast-exit.asm` | `14082e9d8..14082e9e0` | `2506308a4aa42cc63a6a2931e57bfb8ecbc4bff2eb363aa7b36d008e198de602` | `c4b58af8b646fef78c00c49587a16060712f106e4fac7a1c1491daa5e238b608` |
| `resume63-geometry-element-sync-fast-exit.asm` | `14082efc4..14082efcc` | `6fc1483307faa61c68d97a0be3fa91d5301a5975cbef51dbc3d4d9edf27ed002` | `f67623273c4bc6d4d2407e767f63387010006d474ee1781c7e4b4036283b6c33` |

These are explicit prefixes/exits and two complete short gates, not complete
sync-function coverage. PE unwind regions are node sync `14082dca0..14082e9e0`
and element sync `14082ebf0..14082efcc`. Node gate's unwind interval is
`14082bf10..14082bf5a`; element gate is a leaf through `rep ret` at `bf7b` and
has no separate unwind record. The combined gate capture includes six bytes of
INT3 padding between the functions. No exception-funclet coverage is claimed.

## Exact owner-dependent behavior

The already retained coordinate getter `140854af0` obtains dimensions through
`140853240`: N+130 null yields null, otherwise owner+15f8. It calls
`140854200(N,n,dims,0)`, then, when N+130 is nonnull, calls owner sync
`14082dca0(owner,&which0)` before returning N+8.

At `14082dcc1`, node sync invokes `14082bf10`. This gate reads owner byte
`+1b58` at `bf1e`. Nonzero jumps directly to `al=1` at `bf53`, then returns.
Node sync takes its fast jump at `dccd` to `e9d8`, which only restores the stack
and returns. No DB rebuild or replacement of N's coordinate pointer is reached.

For zero `+1b58`, the native gate calls `14035b400`. If that result is nonzero
it also calls `140825420`; a signed result >=6000 makes the gate return false
at `bf4b`, reaching the unresolved sync body. Other routes return true. The two
called helpers were not qualified in this lane, so no additional zero-byte
case is admitted.

This differs materially from `src/node_access/node_access.cpp` and
`src/step_names/name_arrays.cpp`: their DEF_PRE-address counterpart
`fun_1401d48d0` relies on `names_up_to_date` (`1401d2b70`), whose implementation
and comments say always true. The GUI target above has a real false route.
Do not transfer the unconditional counterpart claim to GUI owner-backed getters
or globally change the shared helper from this report.

Connectivity getter `140857ed0` calls `14082ebf0(owner,&which1)` when E+f0 is
nonnull. At `ec1a` that function calls `14082bf60`, which reads owner+1b59,
but returns `al=1` on both branch outcomes (`bf75` and `bf79`). The fast route
zeros its result at `ec26` then returns via `efc4`. A valid actual owner is still
necessary: the byte read occurs before either branch, so an arbitrary opaque
nonnull address cannot be made safe by the constant return value.

## Admission decision

| Existing state | Decision |
|---|---|
| N+8 absent, N+130 null | Keep unsupported before getter: `14085424d` reads null dims[0] before any clean coordinate-null predicate result. |
| N+8 absent, owner merely nonnull | Keep unsupported: requires actual owner identity, dimensions, allocation/product/packing and sync qualification. |
| N+8 present, N owner null, canonical registry, E owner null | CP62 qualified; retain its actual getter/COW behavior. |
| N+8 present, N owner is actual view's step, owner+1b58 nonzero, canonical registry, E owner null | Next safe positive slice: actual dimensions and proven sync fast return. |
| N owner is actual step but +1b58 zero | Keep getter guard until GUI false-route dependencies are recovered. |
| N/E owner opaque or another unqualified allocation | Keep getter guard before any owner byte/dimension dereference. |
| Noncanonical/null/custom/id0 registry | Keep registry guard in this next owner slice; preallocation alone does not qualify state-variable virtual calls. |

The registry restriction is sufficient rather than necessary: the retained
Node/Post key0 bodies skip for any nonzero ID, and an empty registry has no
dispatch slots. A separate method-proven registry generalization could therefore
be investigated, but should not be combined with owner admission or remove the
custom/null/id0 safeguards in this task.

## Smallest implementation task and tests

Own viewer.cpp/test_viewer_box.cpp only. Reuse actual
`view.ownerModel.get()` (already a guarded GuiModel identity) and its nonnull
`step.get()`; do not derive an owner from cross-section ownership or write an
owner into N. Compare raw N+130 against that actual step before dereferencing it.
Admit only the nonzero step.u1b58 fast route, existing N+8 and canonical registry;
leave E+f0 null for this first owner increment. The step dimensions come from
the actual owner+15f8; they are borrowed, not replaced by local mesh flags.
Keep readiness coordinate/connectivity and count/count/width/second-getter order.
Both getter rounds may call the existing port only because the selected native
branch independently proves its sync returns immediately.

Preserve the CP62 ownerless route, missing-coordinate guards and all builder
checks. For profiles outside both proved getter routes, explicitly retain the
unqualified status. Prefer refusing their populated raw fallback before any
native effects rather than presenting its rectangle as qualified readiness;
record that admission tightening separately if included. It is an adapter
unsupported result, never an invented original false predicate.

Focused Release-active regression prerequisites:

- Explicit fixture binds N+130 to its actual GuiModel.step with u1b58 nonzero,
  E owner null, preallocated quad arrays and canonical registry. Forwarding hook
  observes two n4/key0 dispatches with **actual step.u15f8**, not null dims;
  shared-list first-detach/second-stable behavior and bounds remain unchanged.
- Same actual owner/profile with null connectivity observes one coordinate
  dispatch, the actual dimensions pointer and edge-only bounds. No second round.
- Unique-list repeated refresh retains body/arrays/owner and sees two calls each
  time. Test a nonzero flag value other than1 to preserve native byte truthiness.
- Owner+1b58 zero must not invoke the new getter route. If admission tightening
  is selected, populated/null-connectivity versions both refuse without dispatch;
  otherwise explicitly document the retained populated raw fallback limitation.
- Opaque N owner and E owner are compared only, never dereferenced. Include both
  populated and null-connectivity arrays and require no getter dispatch.
- Missing coordinates remain refused with null and actual owners; no test should
  execute native null-dims allocation to manufacture a clean skip.
- Keep null/custom/id0/noncanonical registry cases, malformed builder cases and
  populated-to-skip-to-populated coverage. Restore fixture owner fields before
  mesh destruction; hooks and COW leases own no variable pointees.

Exact root validation target remains `m2m_mdt_viewer_box`; CTest
`m2m.mdt.viewer_box`. No new test was authored or executed here. No retained real
project yet proves this optional mesh has such an owner binding; synthetic tests
would establish the internal conditional contract, not loaded-project parity.
