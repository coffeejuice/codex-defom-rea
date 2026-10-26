# CP66 zero node-sync flag with old actual step version

The next source-qualified branch is an existing actual N owner with u1b58=0
and signed step_version<6000 on the public 3D geometry path. Native's gate still
returns true and synchronization fast-returns. This can reuse the ported getter
without entering its unqualified GUI synchronization counterpart. Preserve all
coordinate, canonical-registry, owner-identity and CP64 refusal prerequisites.

This is an admission expansion with test-fixture consequences: step_version
defaults0 in the source model, so existing zero-flag negative fixtures currently
represent this newly proven fast case. Their intended unresolved cases must
explicitly use version6000 or above after implementation.

Owner: `gradient_state`. Only this report was written. No source/build/test,
GUI or mutable REA action was performed. Bounded immutable disassembly was read
directly without writing additional sidecars, as this lane owns this report only.
Token measurement is unavailable.

## Reviewed frozen boundary

CP65 viewer SHA-256:
`0b66b8535c36a749b030322496c1123f9ba3c05387839bfe0155340f93b972a9`.
Viewer-test SHA-256:
`61b69630755fadef3e56b8a133eb838cb6907da409e978d6e11776d3befa483c`.

Current `qualifiedGeometryReadiness` accepts preallocated coordinates and exact
canonical28 registry, N owner null or actual step with nonzero u1b58, and E owner
null or actual step. All other positive-count states are Unqualified and refuse
before rectangle arithmetic. Opaque/different owners are compared as identities
and never dereferenced as actual models. These bounds remain useful and should
not be generalized merely because one additional gate result is recovered.

The public `viewerRefresh` reaches `refresh`, whose geometry-item iteration is
in its `dim==3` branch. `modelDim` reads the actual step+1c. Actual identity comes
from the view's guarded owner model and its existing step, not a pointer attached
to the mesh by default. No real loaded optional-mesh owner writer is demonstrated.

## Newly closed native gate dependencies

Target: retained `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Tool: retained VS dumpbin, SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Commands use `/disasm:bytes /range:START,(END-1)` on the absolute retained PE.

| Direct read | Bounds, end exclusive | Native-byte SHA-256 | Tool stdout SHA-256 |
|---|---|---|---|
| Owner-is3D helper | `14035b400..14035b42c` | `36189ef4f1de6d9633f8a3f1dfa2b81cdf976e175a141b28db4f186b9f22f4a4` | `b3581bc96adae2e65a4f048ec5c355be5ef02e3a087558593733b22c924d8eac` |
| Owner step-version getter | `140825420..140825431` | `0b55588ba6865add0412d11de3790062a236c71843a95c3a99ad3808cecdd331` | `62503e5a1b3d688f7754364be57bb272a1796df13663ca3aecf3380ac87a1311` |

The first covers its PE unwind region through RET. The second is a leaf without
an unwind record; the initial exploratory48-byte read was narrowed to its17-byte
body through RET, excluding padding and the following function. No exception-
funclet coverage is claimed. Its complete bytes are:
`48894c2408488b4424088b80641b0000c3`.

`14035b400` compares `[owner+1c]` with3 at `35b40e`, returns1 on equality and0
otherwise, and has no calls or state writes outside its stack. `140825420` reads
signed32 owner+1b64 into eax at `82542a` and returns. The source model exposes
that exact layout as `DB_STEP_DATA::step_version`; its static offset assertion
is1b64 and source initializer assigns0.

Together with retained complete gate
`resume63-geometry-owner-up-to-date-leaves.asm`, this establishes:

1. `14082bf1e`: read owner+1b58; nonzero returns true at `bf53`.
2. Zero calls owner-is3D at `bf2e`. Non-3D returns true via `bf4f`.
3. 3D calls version getter at `bf3f`.
4. `bf44` compares eax with0x1770 (6000); signed JL at `bf49` returns true via
   `bf4f` when version<6000. Otherwise `bf4b` returns false.

Thus the gate is `flag!=0 || dimension!=3 || signedVersion<6000`. The next
increment should select only the actual-owner public3D profile and the newly
proved signedVersion<6000 route; it need not add a new 2D geometry-item path.
Nonzero flags retain their previous acceptance independent of version.

Node sync `14082dca0` takes the proven fast jump at `dccd` to `e9d8` for a true
gate; the retained CP63 prefix/exit captures show only stack restore and RET.
No GUI synchronization mutation is reached by the proposed old-version case.

## COW timing and unresolved effects

Coordinate getter `140854af0` first obtains dimensions, calls `140854200` at
`854b19`, then invokes model synchronization at `854b46`. The allocator's key0
dispatch `140b3e8e0` obtains mutable COW slots before any sync gate decision.
For the canonical nonzero-ID profile, it does not resize values, but a shared
node-variable collection detaches once. This effect precedes the native
u1b58/dimension/version reads.

On the newly qualified fast branch, invoke the existing real getter so this
ordering/effect is preserved. The predicate must still finish its coordinate
getter before reading connectivity; populated true readiness still gets a second
coordinate/connectivity round after count/count/width.

Actual owner/zero flag/version>=6000 remains unresolved. Original would already
have reached COW dispatch before entering that synchronization body. Current
adapter preflight refuses with no detach; retain that bounded unsupported
behavior rather than claim it is the native failure order. Preserving a committed
COW prefix on that unresolved route would require a separate staged/resumable
contract; do not call the ported getter there because its unconditional shared
counterpart would incorrectly continue past the GUI gate-false branch.

E's actual-owner byte gate remains independently true for0/nonzero. It neither
rescues an unknown N owner nor suppresses the N version comparison. Missing
coordinates still cannot be allocated through nullable/incompatible dimensions,
and noncanonical variable methods remain outside the proved key0-skipping set.

## Smallest implementation and focused tests

Own viewer.cpp, test_viewer_box.cpp and an implementation report only. Retain
actual N identity verification before reading model state. Broaden its flag guard
to allow the actual-owner/preallocated/canonical public3D branch when signed
step_version<6000, without writing either field. Preserve CP65 E identity rules,
all raw-fallback refusals and existing rectangle builder guards. The existing
port's always-true sync fold is usable only after the selected native gate result
is independently proved; no shared model helper edit is needed.

Required Release-active cases:

- Actual N owner/u1b58=0, versions0 and5999, E owner null or actual, populated
  and null connectivity: two or one actual N dispatches, actual dimensions,
  populated Ymax1 or edge-only Ymax0. Shared list first-detach/second-stable and
  unchanged arrays/owner/flag/version are required.
- Signed threshold: versions-1 and INT32_MIN are below6000 natively; version6000,
  6001 and INT32_MAX remain Unqualified with no adapter dispatch/detach. Do not
  introduce an unsigned comparison or an invented nonnegative-version guard.
- Nonzero u1b58 at version6000 or INT32_MAX remains qualified; native returns
  before version-dependent rejection. No actual step version is synthesized.
- Existing zero-flag negative cases in
  `geometry_actual_owner_getter_admission_keeps_unqualified_profiles_guarded`
  (mode0 and mode6) must explicitly set version6000; their current constructor
  default0 would be accepted by this new contract. Preserve their refusal
  semantics using actual gate-false data and check version remains unchanged.
- In `geometry_populated_unqualified_refusal_recovers_after_source_restoration`,
  mode0 must also use version6000 for its zero-flag refused phase. Restore the
  fixture's prior version or restore nonzero flag explicitly, with known ->
  refused -> known observed results and no stale rectangle reuse.
- Keep CP65 N-owner-null/u1b58=0 cases valid independently of model version;
  keep opaque/foreign owners, missing coordinates, bad registry, zero-count
  early skips and malformed builder cases. Add an old-version actual owner plus
  missing coordinates/id0 as negatives to prevent this admission bypassing them.

Root target remains `m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`. Hooks must
forward the actual dispatcher and end before cleanup; COW leases own list
storage only, and fixture owner fields restore before mesh destruction. No test
was authored or executed here. This is a conditional internal fidelity slice,
not loaded-project owner provenance or original runtime parity evidence.
