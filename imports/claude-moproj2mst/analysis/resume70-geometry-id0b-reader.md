# CP70 cached id0x0b reader completes which0 synchronization

A narrowly qualified **nonzero cached marker** can complete the high-version
coordinate getter after CP69's real COW/table prefix. The reader returns false
without owner/body mutations, but its outer which0 caller ignores that result
and returns normally. Thus reader-false is not mesh-readiness false: coordinates
remain the existing N+8 pointer, then connectivity decides Skip or Populated.
The cache-zero body remains unqualified and guarded.

Owner: `gradient_state`. Only this report was written. No source/build/test,
GUI or mutable REA action was performed. Bounded direct reads and a static
disassembly path walk were used; no original machine code was executed and no
new sidecar was written. Token measurement is unavailable.

## Frozen source and full normal-function evidence

CP69 viewer SHA-256:
`704d581801ea31ddedf0e2b6151973384d720634feea595b7956356304d6a65d`;
viewer-test SHA-256:
`89a9f5cb11af68f2ee6be11c6b6fb403a05841539870a4de666030a3b5250e1d`.
Current high-version readiness performs real key0 dispatch and actual table
selection, then returns HighVersionReaderPending before index/cache reads.

Original target SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Target/tool paths and dumpbin SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`
are the CP69-qualified retained PE and VS tool. Commands used absolute target
path with `/disasm:bytes /range:START,(END-1)`.

| Read | Bounds, end exclusive | Native-byte SHA-256 | Stdout SHA-256 |
|---|---|---|---|
| Complete outer sync | `14082dca0..14082e9e0` | `f985e7ddbd4218a68c500775f28e6f8e231ff0b43e96f557679927a023b10454` | `d6f77fd7051cd4d3535e5b0d189fcc72868ae1a38e66aafb70f0092f5ebd17b5` |
| Complete single reader | `14082d7d0..14082dc8b` | `a3bc02528ce5ce18e576c5f938d82fc8b081616504437a8ca980001ce74dd6e4` | `de5cdda35ab04b415ea6d094f9d2915f7b33d6deb5511bca70586c233502cd1f` |
| Companion DB pointer gate | `140828083..1408280aa` | `4d8807237793a6aac3af8b9721183a84da7f311dbc2c46899aea8297522132cc` | `531d0cbd5fdf61a917e2bc72e30ea730fe3710a4d6ca27f0c63b5ac104b4298d` |
| DB-load cache commit | `14082839f..1408283bd` | `b664fcaccde93acb8b06e79506b5757d998785dc504130068153e4b817a291cc` | `8b17acb990c1fac7a76e2fde3c7e4db7ab1a8dafa79b80a4bbcda57bdd372823` |
| Miss diagnostic prefix | `14082d887..14082d8db` | `9b1ba7d5eed1e50b2a565fbb9b912f7a7deba92456a183b082e852ecd9c890d1` | `520135963fe3d977d2ce67b4dd6e851e0a9f3d7aca8050fac0a4d514bc0f3c72` |

The first two cover PE unwind intervals through normal RET; remaining rows are
explicit bounded slices. Companion DB-load helper's unwind bounds are
`140828060..1408283d5`; only the listed gate/commit slices are relied on for
mutation claims. No exception-funclet coverage or full miss-body recovery is
claimed. Exploratory misaligned/partial windows were replaced by aligned full
function reads for the final reader/outer analysis.

## Post-selection index, MAR and cache order

Let S be the actual model step, M the independent optional mesh and N=M+8.
The reader receives id0x0b and flag1 from outer sync which0. CP69 qualifies actual
COW before synchronization and the real process-global3D table selection. After
selection native performs:

1. `82d809`: read signed global `143DBDAE8`, mapped to g_mar_index[3].
2. `d818`: read S.u1b90[that index]. `d81f` subtracts1 in32-bit arithmetic;
   `d822` sign-extends the wrapped result.
3. `d82d` reads S+1b88 (MAR base); `d834` calculates base+offset*4 with LEA.
   The calculated pointer is stored on the stack. **No MAR pointee is read on
   the cached route.** Even null MAR can reach the subsequent cached return.
4. `d845..d85a`: require signed id>=0 and id<global `143DBDDB0`, mapped to
   g_a_index[128]. Out-of-range sets al=0 and returns via `82dc83`.
5. In range, `d876` reads signed32 S.u26c4[id]. Nonzero at `d880` sets al=0 and
   returns via `82dc83`; zero continues to `d887`.

The fixed source MAR address table has140 slots (u1b90[0x8c]); the cache table
has280 slots. For a source-safe smallest cached admission, require the actual
post-selection g_mar_index[3] in0..139 and id0x0b in the actual signed range
(`g_a_index[128]>11`), then actual S.u26c4[11]!=0. This admits any nonzero cache
marker, including negative values. It need not require every unrelated table
cell canonical, but must not dereference an out-of-range global-derived header
index. Current-dimension3 preserves altered tables, so dimension alone does not
establish these bounds.

For cached normal return, the MAR start/base loads and modular LEA have no
surviving state effect. Do not invent a nonnull MAR/A-body requirement or allocate
storage: native never dereferences those bodies before cached return. If modeled
explicitly, use defined integer address arithmetic for the wrapped32-bit
subtract/64-bit LEA; constructing an out-of-bounds/null-based C++ pointer is not
needed to preserve an unused address calculation. Invalid table indices stay
pending after the real CP69 prefix; they are not normal cached success.

The out-of-range-id branch also returns false normally, but that is a separate
optional admission. The smallest proposed next slice is in-range id0x0b with a
nonzero cache marker. Do not silently merge cache-zero with that case.

## Outer which0 normal continuation is now closed

The full outer body has580 decoded instructions,45 reader call sites and no
non-stack direct MOV destination. Starting after the known gate-false branch,
which remains the caller's stack-local0. The first id0x0b/flag1 reader at
`14082dd0b` is the only reader call reached for which0; its false return is not
tested. All later field/name branches test nonzero keys or wildcard-1 and skip.

The outer function still reads actual step version via `140825420` at
`14082e7ba`. Below11000 it jumps to the common epilogue; at/above11000 it traverses
additional nonzero-key/wildcard branches, all skipped for which0. It returns at
`14082e9df` without any further model/array mutation. Name rebuild helpers
`140828cd0`/`140828fd0` are not reached for which0.

A constrained static path walk checked actual decoded branches, requiring each
which comparison to be preceded by the load of the original which pointer.
It allowed only the observed MOV/LEA/ADD, CMP and JE/JNE/JGE/JMP instructions
and the selected reader/version calls. For versions6000 and10999 it reached RET
after257 instructions/81 which comparisons; for11000 and INT32_MAX it reached
RET after298/95. Both paths called only the first reader and the version getter.
The reader cached-normal-return hypothesis is independently established above;
this was symbolic control-flow analysis, not target execution or runtime parity.

Coordinate getter `140854af0` then returns the existing N+8. It does not translate
the reader's al=0 into a null coordinate pointer. Therefore preallocated N+8
still passes coordinate readiness and reaches connectivity. Connectivity null
gives the established edge-only Skip; populated connectivity can reach the
second coordinate/connectivity round and the recovered builder.

## Cache miss: exact unresolved owner/body edges

Cache-zero first calls helpers at `14082d895`/`d8ab` with process-global
diagnostic object `143DBE000`, then calls `140828060(S,&id)` at `d8c0` and another
diagnostic helper at `d8d6` with global `143DCDAE0`. These global object effects
are not recovered by an integer cache check.

`140828060` reads S+3390, the companion DB pointer. Null branches to its exit;
nonnull enters DB work. After a callee at `14082839a` (`140b38d57`) returns, its
tail stores1 to S.u26c4[id] at `8283b2`. This cache commit precedes the later
reader object-copy loop. No rollback/result qualification is shown here; do not
invent a transactional cache update or assume null DB suppresses subsequent
reader work.

The reader loops to S.u1378 object count and obtains each destination through
`1408302a0(S,ordinal)` at `82d9e6`. It skips null/kind7 objects and a further
object predicate, then passes that returned object plus S's MAR/A bases,
address/dimension tables and id to copy/allocation helpers `1408421d0` and
`1408450f0`. This establishes a model-object destination edge, not a license to
substitute the optional geometry M or its arrays. Exact destination buffer
mutations in those callees remain unqualified. A null companion DB still does
not prove this loop or global diagnostic effects harmless.

The shared counterpart reader remains DEF_NOT_PORTED in
`src/node_access/pending.cpp`. Cache-zero must retain a guard before these
diagnostic/DB/body operations, preserving already committed COW/table effects.

## Smallest next integration and tests

After the existing real COW/table prefix, check the actual header index/range
and nonzero cache marker above. Cached return completes the first coordinate
getter without dispatch replay; proceed to the actual connectivity getter.
Never call the full ported coordinate getter again to finish that first prefix,
which would double its COW traversal. Populated's second native getter must
still traverse once; its cached synchronization fold is justified only while
the same actual owner/module/header/cache contract remains valid. Prefer one
local coordinate-prefix routine used by both rounds if needed to make that
qualification explicit, preserving count/count/width before the second round.
Keep cache-zero/invalid-header pending with public unsupported and raw effects.

Required focused cases before source admission:

- Actual N, high version6000/11000/INT32_MAX, cache1 and-1, E owner null/actual:
  cached populated -> two actual N dispatches and Ymax1; cached connectivity-null
  -> one and known Ymax0. Shared list detaches once; second remains unique.
  Actual flags/cache/owner/body pointers and values stay unchanged after table
  selection. Null MAR/A bodies remain null and must not cause a fabricated guard.
- Current global dimension0/2 must select real3D tables before cache evaluation.
  Current3 with valid altered MAR index/header values and id limit must preserve
  the actual tables. Current3 with invalid MAR index remains pending after one
  dispatch; no unsafe header read or table repair is permitted.
- Cache0 stays pending after one dispatch/table effects even with a null DB or
  zero objects; external diagnostic/reader behavior is still unresolved. Keep
  the independent unknown N/E owner, registry and missing-coordinate guards.
- CP69 lifecycle test's cache1+initial dimension0/2 combinations would deliberately
  change from unsupported to known results; its initial3 negative sentinel
  MAR index remains pending. Update repeated-fresh-invocation expectations
  accordingly, preserving table no-refill, raw diagnostics and source identities.
- Test cached -> miss -> cached transitions without stale rectangle reuse, and
  versions below/above11000 so outer which0 tail cannot introduce extra calls.
  Keep all existing lower-version/nonzero-byte fast and malformed builder cases.

Root source target remains `m2m_mdt_viewer_box`, CTest `m2m.mdt.viewer_box`.
No source/test was authored or executed here. This qualifies conditional internal
cached behavior, not real-project optional-mesh owner provenance/runtime parity.
