# CP67 high-version node synchronization remains guarded

Retain the public actual-N-owner/u1b58=0/version>=6000 guard. After the already
reached coordinate COW dispatch, native enters a real single-array reader. That
reader selects process-global index tables before checking its cache flag, and
the shared source reader counterpart is explicitly unported. A preallocated
coordinate pointer or a nonzero cache marker alone does not qualify bypassing
this prefix. No complete new public admission follows from this audit.

Owner: `gradient_state`. Only this report was written. No source/build/test,
GUI or mutable REA action was performed. Bounded immutable PE reads were retained
as exact intervals/hashes below; no additional sidecars were written. Token
measurement is unavailable.

## Frozen source and native identity

CP66 viewer SHA-256:
`18fc6c1913ec6142bb00e0f7278a3f4ff954ee01f853ce662a593d646d20042b`;
viewer-test SHA-256:
`00d470e743c8333089b7e4c89e4d47307580840f5c96cbccc023fd7616ea2223`.
The source currently qualifies actual N identity, preallocated coordinates and
exact canonical registry, then admits nonzero u1b58 or the public3D signed
version<6000 route. CP65 E identity rules and CP64 no-raw-fallback refusal remain.

Native target: retained `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
VS dumpbin SHA-256:
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Final commands used `/disasm:bytes /range:START,(END-1)` with the absolute PE.

| Direct bounded read | Bounds, end exclusive | Native-byte SHA-256 | Stdout SHA-256 |
|---|---|---|---|
| Which0 first reader call | `14082dcd2..14082dd10` | `bde763fafaefc85477c3099a6cc05f0eb316d61cf632ea6c22b309ba76f95168` | `d9c29545ddd86da7b6ae89442060778752bfa47ae023f19b0492a94407f83ba1` |
| Single-array reader prefix | `14082d7d0..14082d887` | `204969faca2691e3cd2b8523d5772c08cee0d6ab93c5df3b48958b36c3d8ef92` | `51cd17dba78a70eed01fc3b1be6de681fda4286a4b696d21f9f3069a4bd34cb5` |
| Actual owner dimension | `140100980..14010098e` | `aa0064cab9b75339e0e750a487979be868dc766f587bcdb05e4df1648bcdc9ec` | `ba0c37df80453ceff0d755a1cf01e558bba2ae5cf2f83088a94773579a95bec5` |
| Dimension selector | `140820fe0..140821004` | `522b4d7dd9edcaf69c71de7ba37b2cd901e8e5122ab4a85b48291b54acaafa4e` | `c43b18bf78b3fa326110167623379acadea328dd1c8b6e5f47d014b109461fc1` |
| Select3D | `140820fb0..140820fd8` | `42f06cfc3d400ad1d0ffa35524f1eccf1e184404aa3cd9ce2ea7dfad09539c4a` | `1ba3b31ca482d5e76754dfc3e3d732a6148d3e94c9311aa12067d43f9d2f71ed` |
| Reader normal return | `14082dc83..14082dc8b` | `5e79c119452b79849262cc6675d975f0c23be5de8a55df36a89c01c087b424eb` | `e6bff017d0000d0879c4fae87599022b11e09c0ac64d163e8e8c509810dbd9e0` |

The selectors cover their PE unwind regions through RET; dimension getter is a
leaf through RET without a separate unwind record. Other rows are explicit
prefix/exit slices. The full reader unwind region is `14082d7d0..14082dc8b`
(1211 bytes); node synchronization is `14082dca0..14082e9e0`. No complete reader,
whole high-version sync route or exception-funclet coverage is claimed. Initial
exploratory ranges containing partial instructions/padding are not the final
qualified windows listed above.

## Reached order and separate owners

Let M be the optional geometry mesh, N=M+8 and S the actual step bound in N+130.
Native coordinate getter `140854af0` obtains S+15f8 dimensions and calls
`140854200` at `854b19` before synchronization at `854b46`. Its canonical key0
collection dispatch `140b3e8e0` performs mutable lookup/COW detach before the
model gate, without resizing nonzero-ID canonical values. Thus shared-list
detach is an already reached native prefix, not something that waits for the
high-version DB reader to succeed.

With S+1b58 zero, S+1c==3 and signed S+1b64>=6000, `14082bf10` returns false.
The coordinate getter passes a stack-local which0 to `14082dca0`. The first
matching sync branch writes stack-local flag1 at `14082dcec`, id0x0b at `dcf1`,
then calls `14082d7d0(S,&id,&flag)` at `dd0b`. This proves the first reached
reader dependency; later selector branches were not fully followed after that
unresolved call and are not claimed completed here.

Reader prefix:

1. `14082d7ee` calls `140100980(S)`, which returns S+1c. This is model dimension,
   not a DB handle or ownership transfer.
2. `d7fc` calls `140820fe0(&dimension)`, selecting2D only for2 and3D otherwise.
   On this actual public3D route it calls `140820fb0`.
3. `820fb4` reads process-global dimension `143DBDAD8`. If already3, the selector
   returns without filling. Otherwise it calls `140820320` then `140820a60`,
   and only after both returns writes global dimension3 at `820fc9`.
4. `82d809` reads global `143DBDAE8`, indexes S+1b90, subtracts1 and calculates
   an address from S+1b88 into a stack local. This prefix has not yet dereferenced
   the calculated MAR pointer; missing MAR storage is not independently proved
   harmless for the later reader route.
5. `d845..d85a` checks id>=0 and id<the signed global at `143DBDDB0`; failure
   sets al=0 and returns via `14082dc83`.
6. For an in-range id, `d876` checks S+26c4[id]. Nonzero likewise sets al=0 and
   returns. For this first call id=0x0b. Zero continues beyond the qualified
   prefix into reader work, not a proved no-op.

The three ownership levels are distinct: the geometry object owns M; N owns its
COW handle/storage and borrows S as model identity; the dimension/MAR/A index
tables are process-global state. S owns/borrows its DB regions/address tables.
Do not replace the process-global state with a per-mesh default, or substitute
M's local dimension flags for the actual model dimension. Later writes into
model object arrays are not yet qualified to target this independent optional M.
The shared source comment describing reads into objects is a guide to investigate,
not proof that the returned N pointer can be predicted after native reader work.

## Source gap and smallest recovered prefix

`source/port/defmodel/src/node_access/pending.cpp` implements counterpart
`fun_1401d4400` as DEF_NOT_PORTED. Its surrounding comments call the route
unreachable because the DEF_PRE-address counterpart gate is always true. CP63/66
native evidence proves that assumption cannot justify this GUI high-version
route. Calling current `fun_1401808d0` here would silently use its unconditional
sync return and omit the reader; retain the public guard instead.

The source already has `select_index_tables`, `select_index_tables_3d`,
`g_index_tables_dim`, g_mar_index and g_a_index in the step module. They are
counterpart state, not automatically qualified GUI table contents. The native
3D initializer bodies and exact source/native table mapping need comparison
before claiming this prefix is recovered through those functions. The selector
can mutate globals even when S+26c4[0x0b] says cached. Therefore neither a cached
coordinate array nor that cache marker alone admits a direct fast return.

The smallest independently established prefix ends at the call to the dimension
selector: real coordinate collection dispatch/detach, actual gate false, local
which0 -> reader id0x0b/flag1, actual dimension read. It can be documented or
modeled in an isolated suspended call, but publishing a successful viewer result
would require crossing more dependencies. A future stateful prefix must retain
actual N/S and global module ownership and avoid replaying the dispatch on resume.
There is no existing geometry-call cursor here; do not add a partial side effect
to a stateless retry and label it complete recovery.

The next bounded investigation is the reader's cached early-return profile:
qualify actual3D table selection/initializers and global index reads, prove the
id0x0b cache branch and remaining outer which0 continuation, then decide whether
one complete high-version no-read case can be admitted. Until those proofs,
retaining the guard is the smallest faithful implementation decision.

## Required tests before any prefix/admission

- A suspended high-version prefix with shared canonical collection must detach
  once before reaching its sync/reader boundary. Resume must preserve actual
  owners and cursor, reject replaced/expired model/module state, and not repeat
  dispatch. An already unique collection retains its body. Array/value identities
  remain unchanged until a separately qualified reader mutation occurs.
- If selecting3D is recovered, initial global dimension2 must call the two actual
  initializers in order and commit dimension3 only afterward; initial3 skips
  fills and preserves the actual existing tables. Tests must own/reset only their
  module state and not erase unrelated process history during production code.
- Cache-marker-nonzero reader tests must prove the global selector prefix still
  precedes its return, with id0x0b/flag1 and actual model regions. Test invalid-id
  bounds separately if that helper is generalized. Do not synthesize MAR storage
  to turn unqualified pointer arithmetic into a valid reader.
- Cache-marker-zero must stay pending before unqualified diagnostic/DB/object
  writes. Recover exact source/destination owner edges before testing copies or
  coordinate replacement. Do not use another object's main FE arrays as the
  optional geometry mesh.
- Current public versions6000/6001/INT32_MAX with zero flag must retain unsupported
  and the present no-dispatch/no-detach adapter boundary until integration is
  assigned. This differs from native's reached COW prefix and must remain stated.
  CP66 signed-below6000 and nonzero-byte fast cases, CP65 E-owner byte cases,
  unknown owners, bad registry, missing coordinates and CP64 refusal stay intact.

The existing focused root target is `m2m_mdt_viewer_box`, CTest
`m2m.mdt.viewer_box`; an isolated suspended helper would need its own assigned
source/test lane and root CMake coordination. No source or test was authored or
executed here, and no loaded-project owner provenance/runtime parity is claimed.
