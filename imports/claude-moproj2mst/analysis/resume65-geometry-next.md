# CP65 actual connectivity-owner fast return

The smallest remaining owner admission is an **existing E owner equal to the
actual view model's step**, while preserving all CP62/63 coordinate prerequisites
and the CP64 Unqualified refusal. Its native connectivity synchronization gate
returns true for every value of the actual owner byte it reads. Node-owner
qualification must remain independent; a zero node synchronization flag is not
made safe by a qualified element owner.

Owner: `gradient_state`. Writes are this report plus three unique bounded
immutable native captures. No source/build/test, GUI or mutable REA action was
performed. Source implementation requires a separate ownership assignment.
Token measurement is unavailable.

## Current boundary and native identity

Reviewed CP64 frozen viewer SHA-256
`2744965662cd8ef6b5832f136e08c358852c87fbb549fa5b3a1152d42bac712a`
and viewer-test SHA-256
`de0aa597961a4a69a1d98bd13cb31c973645d162dda25c8f992fae56775d1490`.
`qualifiedGeometryReadiness` currently rejects every nonnull E+f0 before the
coordinate getter. That rejection now propagates through rectKnown=false,
without a raw populated fallback. The smallest positive change is to relax only
that identity check for the actual live step.

Original target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Tool: retained VS dumpbin, SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
All commands used `/disasm:bytes /range:START,(END-1)` on that absolute PE.

| Artifact under `analysis/` | Native interval, end exclusive | Native-byte SHA-256 | Capture SHA-256 |
|---|---|---|---|
| `resume65-geometry-mesh-connectivity-wrapper.asm` | `140894330..14089434f` | `08cfdf3525682bb1a74762ece90cd589bc7edb3c9792cd1976956b3d3c9ddaaa` | `7ff2f76abf6ef1d5dbb4a09b860021414c0f760a27ea0449202fa3a952635523` |
| `resume65-geometry-connectivity-owner-getter.asm` | `140857ed0..140857f14` | `c5a04f092955d268f10058ba56606b29dc561422d0bbe4740eaa6b910bd6ad99` | `13e21714a4ba7e73de754d6f1fbf07bc83be9316679c5fa7b39d7e1b2db5932a` |
| `resume65-geometry-element-owner-gate.asm` | `14082bf60..14082bf7d` | `15eccf1890ddbaf7bf16025925077d71628e8beed77c14fe85f36d66a3f9c257` | `ab6606b8f9dd70e02d87ef027869997f12768648db7e714906b77ded6d49709d` |

The wrapper/getter captures cover their PE unwind regions through normal RET.
The short gate is a leaf without an unwind record and is captured through its
`rep ret`; no exception-funclet coverage is asserted. The synchronization prefix
and exact exit are already retained in CP63's
`resume63-geometry-element-owner-sync-prefix.asm` and
`resume63-geometry-element-sync-fast-exit.asm`, with their hashes in
`resume63-geometry-owner.md`. No entire sync function coverage is needed or
claimed for this proven fast route.

## Exact effect and ownership contract

M is the actual optional cross-section mesh and E=M+148. The wrapper adds148
at `14089433e` and calls `140857ed0` at `4345`. The getter checks E+f0:

1. Null skips synchronization and returns E+8.
2. Nonnull sets local which=1 at `140857ee8`, passes that owner's identity and
   the local address to `14082ebf0` at `7f01`.
3. The sync prefix calls gate `14082bf60` at `14082ec1a`. The gate reads
   owner+1b59 at `bf6a`; both zero (`bf75`) and nonzero (`bf79`) set al=1.
4. Sync zeroes its return value at `ec26` and jumps to the stack-restore/RET at
   `14082efc4`. The getter ignores that result and returns the current E+8 at
   `140857f0b`. There is no reached DB rebuild, array replacement, state-variable
   loop, allocation, refcount effect or E-owner write on this route.

An opaque nonnull owner is still unsafe to dereference at owner+1b59. Require an
actual live allocation identity first. Resolve `view.ownerModel.get()` and its
nonnull `step.get()` through the existing model guard. Compare E+f0 to that exact
step as raw pointer identity, without casting an arbitrary owner and reading it.
Never infer the model from cross-section ownership or set E's owner as a default.

The source field `Cls_140182880::u0f0` has historical type LLOBJ_DATA*, but the
recovered `llobj_ctor::set_owner` already stores DB_STEP_DATA identity there via
reinterpret_cast for the main object element set. This is an address/type-layout
fact, not a recovered writer for the optional cross-section mesh. The new branch
must only recognize an existing matching pointer; it must not call that setter
or alter E's local dimension flag to manufacture admission.

`fun_140220460` already returns E+8 directly in the shared port, whose counterpart
comment folds the always-true gate. For this selected actual-owner profile, the
native omitted read is of ordinary nonvolatile model storage and has no surviving
state effect. That justifies the existing fold only after live identity is
qualified. It does not justify opaque pointers, concurrent destruction, a
memory-mapped/volatile model, or claiming the native gate was never called/read.

## Smallest implementation successor

Own only viewer.cpp, test_viewer_box.cpp and an implementation report. Replace
the blanket E-owner-nonnull preflight rejection with:

- E owner null: retain the prior route, without needing a new model lookup.
- E owner nonnull: require the actual guarded view model and live actual step,
  and E-owner equality to that step. Reject all other identities before native
  getters or model dereferences. No u1b59 value predicate is required.

Keep N+8 preallocation and canonical registry proof before getters. Retain N
owner null, or exact actual step with actual u1b58 nonzero. It is safe to resolve
the actual step once when either owner is nonnull; no ownership is transferred.
Do not couple E's admitted byte truthiness to N's separate synchronization gate.
Keep coordinate then connectivity readiness, and count/count/width then second
getter order on populated results. Null returned connectivity remains known
edge-only Skip; unknown owners/registry remain Unqualified and refused by CP64.

Missing coordinates, actual N owner/zero u1b58, other/opaque N or E identities,
null/custom/id0/noncanonical registry and malformed packed builder profiles
remain guarded. No shared model source or CMake change is needed.

## Exact test prerequisites

- New actual-E-owner matrix: explicit fixture E+f0 equals its actual step;
  preallocated quad, canonical N registry, N owner null. For u1b59 values0,1 and
  0x80, populated connectivity yields unchanged Ymax1 and exactly two real N key0
  dispatches with null dims. Set actual model.u1b58=0 here to prove N-owner-null
  does not accidentally acquire the separate node-owner flag requirement.
- Same three values with connectivity null yield one real N dispatch and known
  edge-only Ymax0. With a shared list lease, first coordinate dispatch detaches
  once; populated's second retains the new unique body. Arrays/owners/flag bytes
  remain unchanged. These hook counts observe N dispatch, not a new instrumented
  count of the pure E getter; E order is qualified statically.
- Add actual E owner plus actual N owner/u1b58 nonzero: actual step dimensions,
  both getter rounds, unchanged values and source identities. With N u1b58=0,
  require Unqualified refusal and no N dispatch even though E gate is safe.
- A different valid model's E owner and an opaque E owner remain unsupported for
  populated and null connectivity without dispatch/detach. Existing opaque-E
  negative matrix remains a valid negative case; it must not become successful
  merely because E is nonnull. Add missing-coordinates and bad-registry cases
  combined with actual E owner to check no unsafe route is newly admitted.
- Transition E actual -> other/opaque -> actual with populated arrays: known
  result, unsupported, recovered known result, preserving source arrays. Keep
  zero-count early skips, CP64 restored owner/registry tests, CP62 ownerless and
  CP63 actual-N-owner cases intact.

Use Release-active CHECK/REQUIRE and existing Testkit entry point. Fixture owner
changes must restore before mesh destruction; leases own list storage only and
hooks end before cleanup. Root target `m2m_mdt_viewer_box`, CTest
`m2m.mdt.viewer_box`. No tests were authored or executed in this audit. No retained
real-project writer/loaded optional-mesh record proves actual E-owner binding;
this is a conditional internal admission contract, not runtime parity evidence.
