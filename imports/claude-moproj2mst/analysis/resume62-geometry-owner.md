# Next geometry coordinate slice: native getters on the populated ownerless path

The next implementable slice is the **already supported populated, canonical,
ownerless mesh's getter effects**. CP60 preserves the coordinate getter's COW
effect on the connectivity-null skip, but populated connectivity bypasses that
helper and enters the raw rectangle builder without either native getter round.
The original calls coordinate/connectivity once for readiness and again before
the builder. Recover those effects on the same mesh without inventing a model
owner or admitting absent-coordinate allocation.

Owner: `gradient_state`. Exclusive writes: this report and three bounded immutable
asm captures listed below. No source/shared edits, build/test execution, GUI,
original execution or mutable REA. Token measurement unavailable.

## Current boundary and evidence reconciliation

Read `analysis/resume53-geometry3d-ownership.md`,
`analysis/resume56-geometry3d-node-owner.md`,
`analysis/resume56-geometry3d-increment.md`,
`analysis/resume60-geometry3d-next.md`, and
`analysis/resume60-geometry3d-skip-implementation.md` with their qualifications.
The optional cross-section mesh is owned at `GEO2D_AND_MESH2D+28`; its inline
node set's model owner at mesh+138 is a different relationship.

Current viewer SHA-256 is
`8fc663a1a0c2a33fc70552dbebad916f1d6d7954430ca702d90395418decbdf3`;
viewer test SHA-256 is
`e4e2a61689f157a8b905217753745716117066c67eede1d3486e9ebc6722c941`.
`analysis/resume60-integrated-full-tests.json` records `m2m.mdt.viewer_box`
passed without logged gaps/differences/truncated output. I read that receipt;
I did not rerun tests. Its success does not validate the currently omitted
getter effects on the populated branch.

`ownerlessGeometryConnectivitySkip` explicitly returns false at its first
condition when connectivity is nonnull. `geometryMeshRect` then reads raw node
and element arrays and computes the already recovered float rectangle. The
existing COW-detach test exercises null connectivity only. A shared node-variable
list on the populated path therefore retains its old handle today, although the
native coordinate getter would detach it.

## Bounded new immutable evidence

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Tool: VS dumpbin at the path already identified in CP60, verified SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Commands use `/disasm:bytes /range:START,(END-1)` on the absolute retained PE.

| Artifact under `analysis/` | Native interval, end exclusive | Native-byte SHA-256 | Capture text SHA-256 |
|---|---|---|---|
| `resume62-geometry-ready-caller-14126d261.asm` | `14126d261..14126d30a` | `f3e5d6e5a44a8018f708f89af928d3e2b3adf262aa0bd77c5c372efe3015ff06` | `0709a730b7dac0b019e0eb9f29569166c9b6f69d2fa4a6c2bd1e59a9e46b223b` |
| `resume62-geometry-node-mutable-slot-14086e340.asm` | `14086e340..14086e373` | `12a7994916323f3165bd6df1065f4987dabb5a83b20121c2b516c13a5d6f60c5` | `954053500350f13acd4c47bdce5ff30517cd94d3b3cacdf752a6c267650d414f` |
| `resume62-geometry-node-detach-1408f2550.asm` | `1408f2550..1408f2575` | `91035ee8aa546eab50aacb0b6f1fd2100778af750f1dd11dbc5ab4370536ef27` | `58a33d94a02e324e6605f67e946be360629f48d7c0ca319dd8db6421218a675f` |

The first is an explicit caller slice, not a whole function. Its false-branch
destination lies outside the slice and is qualified by the already retained
complete `wave2-geometry-14126d0c0-disassembly.txt`. The two short helpers are
PE unwind regions through their RETs; that is not exception-funclet coverage.

## Exact reached order and ownership

Let M be the actual optional mesh, N=M+8 and E=M+148. Positive node/element
counts precede readiness. The predicate `1408ee940` calls
`140514f00(M)` -> `140854af0(N)` then `140894330(M)` -> `140857ed0(E)`.
The former returns N+8 after allocation-on-access dispatch; the latter returns
E+8, bypassing synchronization when E+f0 is null.

When readiness is true, the caller slice shows:

1. `14126d266`: invoke readiness, branch past mesh work on false at `d26d`.
2. Create/select the existing display-mesh child (`14126dc20`) and resolve M.
3. Node count at `14126d29b`, element count at `d2a9`, width at `d2b7`.
4. **Second coordinate getter** at `14126d2c5`.
5. **Second connectivity getter** at `14126d2d4`.
6. Pass the returned arrays and count/width to builder `14124bec0` at `d305`.

The next slice concerns getter/array state on the existing rectangle path; it
does not claim complete display-child construction or ownership from that call.

The coordinate getter's key0 dispatch reaches `140b3e8e0`, which obtains each
registered variable through `14086e340`. New capture `14086e340` calls detach
helper `1408f2550` before fetching its mutable slot. The latter compares the
handle body's ref count to1 and calls `1408f27f0` only for unsigned count>1.
Therefore a shared canonical collection must detach even though none of its
variable resize methods matches key0. The later second getter still traverses
the now-unique collection, preserving its current body.

Port equivalents are already available: `fun_140220440`,
`fun_140220460`, `fun_1401808d0`,
`Cls_140329c70::set_all`, and `CowList::mutable_at`. The source raw-layout
rectangle bypass is not a substitute for these reached collection effects.
No coordinate allocation occurs when N+8 already exists; canonical Node/Post
methods have nonzero IDs and skip dimension reads at key0. Both nested owners
stay null and no DB/model synchronization is introduced on this profile.

## Smallest implementation task

Own only `viewer.cpp` and focused viewer tests for the same qualified ownerless
preallocated profile as CP60, extended to nonnull connectivity. Reuse the exact
canonical28-ID/method-type proof. Invoke readiness coordinate then connectivity
getters, preserving the current known-null skip. On the qualified populated
route, reproduce the second count/width/coordinate/connectivity acquisition
order and feed its borrowed arrays to the recovered builder. Keep the same M
identity across both rounds; do not create an LLOBJ node set or attach a model
owner to obtain dimensions.

This is an internal fidelity increment, not a larger data-layout admission.
Retain positive-count, known packed-array/layout, width, product/index and finite
value guards for the currently recovered rectangle builder. Do not silently
authorize arbitrary owner-backed synchronization, custom/id0 variables or an
unqualified raw-layout fallback as original getter readiness. Any additional
refusal change outside the supported profile should be recorded separately.

**Missing coordinates with null N owner stay unsupported.** The retained
`140854200` body dereferences dims[0] at `14085424d` when coordinates need
allocation. N owner null supplies a null dims pointer. The later calloc helper's
zero-size return does not rescue that earlier dereference. The existing missing
coordinate guard must remain before invoking the getter; do not invent a null
return/zero buffer or use M's local flags as the getter's model dimensions.

## Exact proposed focused checks

- **Populated canonical/shared list:** start from the existing four-node quad,
  both arrays present, both owners null. Hold a COW list-storage lease. Refresh
  must detach the node-variable collection once and produce the unchanged
  populated scene rectangle. Variable pointer/ID/current/previous/value bodies,
  coordinates and connectivity remain unchanged; no new arrays or owner writes.
- **Two coordinate dispatches:** use the existing
  `def::model::llobj::ext::g_hook` seam solely around refresh, filtering wrapper
  address140329e60 and this actual N. Forward each call to its real default body
  with the hook temporarily disabled and restored by RAII. Record exactly two
  calls with n4, dims=null, key0 for the populated path. First before/after list
  body changes; second before/after body stays identical. This records the real
  ported dispatch rather than replacing it with a supplied success result.
- **Preserve CP60 skip:** identical shared-list profile with null connectivity
  records one coordinate dispatch, detaches once and returns edge-only bounds.
  No second getter round or populated mesh extent is fabricated.
- **Unique list/repeated refresh:** an already unique canonical populated list
  keeps its body while still recording two coordinate dispatches per refresh.
  Repeat without reallocating/replacing source arrays or changing their owners.
- **Guard matrix:** preserve CP60's missing-connectivity refusals for absent
  coordinates, null/custom/id0 registry entries and unknown owner profiles, plus
  the existing malformed builder refusals. On a populated but unqualified
  registry/owner, do not admit the new unsafe getter call; the existing raw
  fallback is not evidence of original readiness, and any tightening of that
  fallback is a separately recorded change. Preserve populated->skip->populated
  rectangle coverage and Release-active CHECK/REQUIRE assertions. Keep hook scope
  shorter than fixture destruction.

The hook and COW lease own no StateVar pointees; restore hook/temporary registry
changes before existing mesh destructors run. Root owns test authoring/build and
required regressions for the next implementation; no tests were edited/run here.
The remaining real-project optional-mesh population/original comparison gate is
unchanged. This evidence establishes reachable source state effects, not a new
runtime acceptance case.
