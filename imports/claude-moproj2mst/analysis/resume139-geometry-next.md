# CP139: admit the element metadata value independently of packed XY storage

The smallest source-closed successor is a narrow populated-builder admission: keep actual node storage qualified as packed XY (`N.u138 == 2`), but admit actual element metadata `E.u0f8 == 3` as well as `2`. The native builder and its rectangle finalizer consume width/counts/flat arrays; this field does not select their layout. This advances the existing private Viewer contribution for an already-owned, allocated, valid profile. It does not establish that a public file loader creates that profile, complete the native display representation, or release an editor/import guard.

This lane wrote only this report. No source change, build/test execution, original launch, GUI control, or mutable REA operation occurred. The source proposal remains the exclusive pair `src/engine/viewer.cpp` and `tests/mdt/test_viewer_box.cpp`, with a new implementation receipt, if root assigns it.

## Verified artifact and bounded method

Original immutable `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Offline DUMPBIN 14.44.35228.0 bounded ranges and direct PE exception/vtable reads were used. No shared analysis database was opened. All ranges below are half-open; hashes cover original PE bytes, not formatted disassembly.

| Range / purpose | SHA-256 |
| --- | --- |
| `[14124bec0,14124c14b)` full builder | `2ffda9258e62250966b1df188f1e175addde74745b04c0f584eb716a3acc447e` |
| `[14124c1e0,14124c291)` builder initializer | `4ae1d6a1612e7e041adbf168806f245086d3ca4ffe4f7007a42e4f532581ecff` |
| `[14124c100,14124c14b)` previously missing builder tail | `27c1d2a947992d0e48983b217ea92c3f6f3d242c6bd83355a5183ee43827cd1d` |
| `[140ab4b90,140ab4baf)` width wrapper | `c8d3383b5503736932f049963d5aea5e3086de8e402f847326e4a36efcba1d26` |
| `[140859750,140859761)` width leaf | `aee7b1c90235817bf1d4d31e3a5706f3870ba24c21f7759e2045a7aa7c30b22b` |
| `[14124c3d0,14124c462)` actual virtual rectangle update | `d6bf56997e9743aa344e83bad8090d70ac92d5294620c0d65c6a32a9f119e5f5` |
| `[142d11f90,142d11f98)` actual vtable slot +48 | `15583117b3d57b9cf43eec6f76924039a61d3139d643416d22bc6d8d41f70b17` |

The retained `resume51-mesh-rect-builder-14124bec0.asm` ends at `14124c100` before the function tail. It is not a complete-body receipt. The exception directory gives `[14124bec0,14124c14b)`; the missing tail and actual virtual target were read separately here.

## Exact owner, layout and order

The unchanged readiness predicate and CP137 reader qualifications establish the actual optional geometry mesh, node/connectivity owners, live current step and completed getter effects. For actual element owner, current `viewer.cpp:620` qualifies the existing native `14082bf60`/`14082ebf0` fast sync; a foreign owner remains unqualified. The getter `[140857ed0,140857f14)` tests only owner `E+f0`, optionally syncs, then returns `E+8`. Its retained receipt is `resume65-geometry-connectivity-owner-getter.asm` (PE hash `c5a04f092955d268f10058ba56606b29dc561422d0bbe4740eaa6b910bd6ad99`). It does not read `E+f8`.

Caller `14126d0c0` reads node count at `14126d29b`, element count at `d2a9`, width at `d2b7`, then performs the second coordinate getter at `d2c5` and connectivity getter at `d2d4`. At `d305` it passes only the representation, width, element count, node count, connectivity pointer and coordinate pointer to `14124bec0`. Width wrapper `140ab4b90` adds mesh+148, calls `140859750`; that leaf reads exactly `E+ec` and returns. No `E+f8` value is passed to the builder.

The initializer stores width/counts at representation+188/+18c/+190, allocates elementCount*width integer entries at +198 and 3*nodeCount floats at +1a0. The builder converts each connectivity value from one-based to zero-based, and constructs each three-float vertex from a constant first component and exactly two input doubles at `2*i+j`. Its input stride is packed XY, independent of element metadata.

The missing tail calls actual representation virtual +48 at `14124c129`, then stores the original coordinate pointer at representation+218 (`c139`). Constructor `14124bb30` installs primary vtable `142d11f48` at `bb64..bb6b`; slot bytes at `142d11f90` are `d0c3244101000000`, resolving +48 to `14124c3d0`. That function resets rectangle+0b0, loops nodeCount, takes float pair starting at vector index `3*i+1`, extends the rectangle, and finalizes it. It reads no element metadata or source geometry owner. Its range is exception-qualified `[14124c3d0,14124c462)`.

The ported element class separately represents allocation and metadata: `source/port/defmodel/src/llobj/cls_140182880.cpp:101` allocates `n*m` integer connectivity, assigns n/m, and does not use u0f8. Its constructor default is3, its setter writes the field directly, and `fun_140183c20` uses it for a separate topology calculation. This source qualification supports the proposed already-allocated profile; it is not a newly observed native loader producer.

## Minimal implementation and preserved partial effects

In current `geometryMeshRect` (`viewer.cpp:810`), replace only the E metadata rejection with acceptance of2 or3. Keep N.u138==2, positive width, checked connectivity product, pointer/range/finite guards and all owner qualifications. Do not admit N.u138==3: whether its actual allocation has packed XY is a separate missing storage witness. Do not generalize arbitrary element metadata, zero width, malformed topology, NaNs or infinities.

Also bound `nodeCount <= INT32_MAX / 3`, not merely the current `/2`, before reading connectivity/coordinate arrays. Native `14124c270` computes 3*nodeCount in 32-bit arithmetic for its float array, and finalizer `c41c` uses the same triple stride. This preserves the supported no-wrap native allocation profile; do not perform signed overflowing C++ arithmetic or emulate a huge allocation.

Both guards stay after the existing count/count/width snapshot and completed second getter. Edge sampling already occurs before readiness. On a later layout/count/data refusal, retain COW, reader/timer and edge prefix effects, set the item unknown, and do not union a partially computed mesh rectangle. On success, union the same double-to-float rectangle with the previously sampled edges. No extra getter, timer, frame retirement, query, cache mutation, registry reset or borrowed owner is introduced.

The full native representation's connectivity/vector allocations and retained coordinate pointer are not reconstructed by the rectangle projection. This proposal does not claim otherwise. The recovered virtual target establishes why the existing rectangle projection is independent of E.u0f8; it does not justify implementing unrelated display state.

## Focused acceptance contract

- Compare identical allocated packed-XY meshes with E.u0f8=2 and3, first ownerless/cached, then actual same-step high-version/cache0/null-companion. Both should contribute the same populated mesh bounds and edge union. Assert metadata, coordinate/connectivity content, version/cache, ownership and current step are unchanged.
- In the high/cache0 case assert two real COW dispatches, second after first Stop, two uncapped arrays checkpoints, final node count3, and no third getter. Retain capacity/no-op and first-/second-round failure distinctions already covered by CP137.
- Keep N.u138=3 and unsupported E metadata0/-1 unknown after completed getter effects. Existing CP137 later-layout refusal mutates N.u138, so its outcome stays guarded.
- For E flag3 retain invalid width, out-of-range connectivity, nonfinite input and float overflow refusal. Check edges remain only a prefix and the scene cannot consume it as a known completed item.
- Use a small allocated fixture whose declared node count is temporarily `INT32_MAX/3+1` (also <=INT32_MAX/2) to prove the triple-product guard refuses before coordinate traversal; restore the count before fixture destruction. Exercise an ownerless/cached case to isolate arithmetic and a qualified actual-null case if the fixture supports both getter rounds without traversing those declared node entries.

No new helper, shared header, CMake entry or public ABI is required. Root can assign the same Viewer/test pair; there is no current source ownership from this read-only lane. Runtime/build results remain root-owned and are not claimed here.
