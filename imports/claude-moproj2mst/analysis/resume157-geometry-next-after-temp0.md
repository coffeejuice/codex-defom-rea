# CP157: request8 can advance to an unpublished one-double buffer

The next conditional source-ready cut is a **fresh same-invocation unpublished value-buffer prefix**: finish temp0's actual-nonnull success tail, execute the zero-length old-value copy, allocate a distinct8-byte block B from the same current candidate module-role heap, initialize its one double to+0.0, and take the old-buffer-null free return. Stop at native `0x140832adc`, before the first actual CowVector body publication at`0x140832ae9`. T and B remain separately retained pending blocks; body first/last/end and the bound StateVar slot stay null, whole-leaf count0, public barrier installed.

This prefix needs new explicit occurrence/storage code; current APIs do not provide it. It does **not** need an original same-run pointer witness to execute candidate-owned success operations established by immutable bytes. It does need that witness before claiming original runtime correspondence or parity. Actual body/slot publication remains blocked by compatible synchronous payload/body deallocation and failure ownership. Allocating/filling an unpublished B does not solve that lifecycle boundary.

Only this report was written while CP156 owns the source/build files. No source edits, configure/build/test, original/GUI execution or mutable REA occurred. Read CP155 implementation/source review, CP152/153 native receipts and CP154 lifecycle qualification. Source review SHA-256 `da409cf0a6a98e2a9c91e361c18371ba831731b00e3dfb828786d4ee6dfd62bb`. The retained original DEF_GUI_MO_64 v11.0.2 was rehashed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Normal-success edges closed by the PE

CP155 ends immediately after HeapAlloc at`0x1414748f1`, with candidate raw T recorded and all malloc/new-return/copy traits false. For an actual nonnull T the remaining allocator tail has no new-mode/handler/errno/global read:

- `0x1414748f7` saves RAX in RSI;48fa tests RAX;48fd takes the nonnull branch to492b.492b returns RSI in RAX,492e jumps to4942, and4942..4951 restores registers/returns. It skips the new-mode read at48ff and all failure work.
- Outer new at`0x14146eadb` tests the returned pointer; nonnull falls through eae0..eae5 to restore/return. Its handler loop and bad-allocation globals are bypassed. `0x14146c25c` is only a tail jump to that outer wrapper.
- Native resize `0x1409e47e9..47f3` retains T in stack locals.4800 reloads the actual count,4805..4808 forms count*8, and4816 calls copy. Under the exact unique/null-body profile count0 remains zero. CP152's native memcpy zero guards return before any old-data read or T byte write. A port must implement that zero-count branch without passing null old storage to a host copying routine or doing null-pointer subtraction in C++.

Native vector resize `0x1408325c0` sees wanted1>=size0 at25de and takes2617; it reloads size, computes insert count1, obtains mutable end via32650 and calls insert832700 at2643. Insert's count check2718 is nonzero, position offset is0, and detach2730 skips cloning on actual refs1. Body832790 sees capacity0<insert1 at27d2 and goes to29b9. Growth is oldsize0+max(oldsize0,insert1)=1; multiplication produces exact8 and `0x140832a1d` calls the same new wrapper. No body32 allocation is reached in this unique-body profile. Do not generalize it to shared/nonempty/reserved-capacity bodies.

The request8 malloc/HeapAlloc path rereads the current heap slot for this **second** allocation. Actual nonnull B can take the same closed allocator success tail. Then:

| Native site | Exact empty-body effect |
| --- | --- |
| `0x140832a22..a2c` | Retain B in insertion stack locals |
| `0x140832a4a ->0x1401b2980` | Copy [oldfirst,pos)=[null,null), returns B without reading/writing an element |
| `0x140832a94..a98` | Read fill0.0 and store one double at B |
| `0x140832a63..a85` | Decrement remaining1 to0 and advance local destination to B+8 |
| `0x140832ab7 ->0x1401b2980` | Copy [pos,oldlast)=[null,null), returns B+8 without element access |
| `0x140832ad7 ->0x14146d898 ->0x1414dfc68 ->0x141471b7c` | Old-first is null; source free returns at471bb8 before reading any heap slot or invoking HeapFree |
| **`0x140832adc`** | **Recommended guard before the body pointer stores** |
| `0x140832ae9 / afa / b14` | Later first/last/end publication; excluded |

New bounded disassembly independently closes the vector copy's zero range: `0x1401b2994` compares first with last,2999 jumps directly to29cb,29cb returns the destination and29d0 returns. No load at29a5 or store29a9 executes. The ordinary StateVar empty-copy and the vector's two empty ranges are separate occurrences; neither permits reading an uninitialized T/B byte.

Old-null free is a **source free-return** with zero heap reads/calls, not a successful HeapFree. It does not require the missing nonnull-free errno provider. Do not collapse it into T's later nonnull free or report T/B released. It is reasonable to include this closed null branch in the same unpublished-prefix cut; a smaller raw-B-only cut may stop at the second HeapAlloc return, but would leave the known fill/null-free prefix unnecessarily pending.

## Required current owner and occurrence guards

Use a fresh explicitly selected extension mode prepared before the initial query, not a resume API on an already parked CP155 frame. CP155's old Temp0Report remains historical/raw-only; new successful-tail/value-prefix receipts must state their own effects. Do not flip its static false traits or promote an old raw result merely by inspecting it later. Unbound and existing temp-only modes retain their present behavior.

The extension stays in the same current original high borrow and sealed real model/step/object/table/ordinal0 StateVar receiver occurrence. Re-establish the installed frame/control block, DbHandle/reader/directory, A/source extent, primary/six-BC/node-n1, canonical registry/order/type/id, u30 binding and null slot, post-setter dimensions/current+previous observations, exactly one started recipient and whole-leaf count0. Actual value body must still be the same body, refs1, first/last/end allnull, D1=1/all other ordinary widths0 and cur2=1. Another object's empty buffer or a former body address supplies no authority.

For the fresh extension mode, preserve relevant native getter order before T's allocation. If invoking the existing typed value_data accessor, do so at its native pre-T point only after proving refs1, then confirm the same body and returnednull. Do not retroactively call it after the existing parked raw T to claim the earlier getter ran. No broad resize or unrestricted virtual call is admitted. Count0/capacity0 must be established from actual null body facts without undefined host null-span arithmetic.

After actual nonnull T, complete the typed successful return tail and zero-count copy while retaining that same private block/role. Before request8, recheck current frame/body/slot and canonical module lease. The second allocation reads the current candidate role slot and must use the same live owner generation, flags0, requested8/kernel8. It is not another temp call, another module owner or a reset of CP155's one-shot budget. No new generic allocator interface accepting arbitrary size/heap/pointer is needed.

Prepare all additional report/block-retention backing before HeapCreate/T. Extend the one bounded experiment to at most one T(length1) and one B(length8), retained separately under the same module role. No registry growth or host allocations may occur between these source effects. Retain private pointers for executing the local typed fill; reports expose only opaque tokens and the observed initialized double bits0 after the actual write. T's byte remains unknown and unread.

Run the original successful allocation/fill/copy/null-free prefix as an actual partial typed operation associated with the live body, rather than manufacturing a completed vector receipt. At the guard, the pending call owns B as an **unpublished insertion-local buffer**. It is not body-owned payload yet. Neither B nor B+8 enters first/last/end/u30, and the body has no source-heap deallocator registration. Model/frame teardown therefore continues using its unchanged empty host-owned CowVector, while both pending blocks remain process retained. No early free, owner destruction, later receiver/copy/Stop/retirement or replay follows.

## Failure and synchronous-free boundary

Actual Tnull stays at CP155's raw return, before new-mode. Actual Bnull stops at its own raw return before new-mode, retaining successful T and all previous dimension/BC/source effects. It does not call host std::bad_alloc, collapse the two failures, run an invented RAII cleanup or claim native throwing-new returnednull. The CP152/153 flags0 unwind evidence still establishes no local T cleanup if the original second allocation eventually throws; the pending guard itself is not that throw.

The first unclosed **nonnull normal free** in this path is T at`0x1409e4935`, after successful body/slot publication. Source `0x141471b7c` reads current heap144700810, calls HeapFree at471b92 and, if raw0, must finish errno-provider471b9c, preserved LastError, mapper and errno store before returning. That missing FLS/PTD/error owner cannot be replaced by host errno or by the retained-initial-table mapper alone. A raw0 result does not itself prevent native caller continuation after its errno tail returns, and an ignored return is not proof the block was freed.

Publishing B even as a “success-only” optimization would also expose it to later real StateVar clear/CowVector growth/clone/destruction. Current `cow.hpp` frees through host std::free and deletes bodies with host delete. Automatic destructors cannot park halfway through the failed-free path. Consequently **do not cross body store832ae9** until the actual body/payload provenance and complete synchronous free/error backend exist for every consuming path (including separate clone body32). Retaining G indefinitely, intercepting only Viewer cleanup, adding a destructor early-return/leak, or using host malloc B while labeling it source-heap storage does not close this obligation.

## Remaining original witness, acceptance and source scope

No examined geometry receipt binds original actual heap reads/T/B returns/free to one fresh reference invocation. For original validation still need one process/module/base/thread/frame generation containing real G/step/current O/normal id1 receiver/body/u30/dimensions, request0/HeapAlloc1→T, then request8/HeapAlloc8→B, one+0.0 write, body first/last/end and slot stores, and T's actual free result/continuation and leaf return. Retain their order and distinguish original heap slot144700810 from this candidate's reconstructed role. Original pointers/handles remain evidence, never candidate write authority. A successful candidate prefix closes none of that original correspondence.

Source readiness and parity are separate: immutable successful branches close this conditional unpublished candidate cut; they do not close actual original runtime inputs, failure handlers/errno/exception state, general vector lifecycle or heap termination. A fresh witness would not by itself make the existing host-free body safe for B.

After CP156's source/build freeze releases the files, the natural bounded write set is the existing ModuleHeapRole header/CPP, app geometry binding header/CPP, Viewer and their focused tests plus an implementation receipt. No CowVector/StateVar layout/free hook is needed for an unpublished B. Add a preparation-time extension mode and its own immutable success-tail/value-stage receipt; preserve temp-only public report traits and sticky barriers. Do not mutate these files while CP156 owns them.

Focused actual fresh-process tests should require both real nonnull T/B, distinct owned tokens, one current module owner and two ordered slot reads/HeapAlloc calls (length1 then8), initialized B bits+0, source old-null-free with no HeapFree/extra heap read, unchanged actual body/slot/other receivers/primary+BC/leaf count/timers, no completed vector/leaf, and retention of both blocks after model death. Existing temp-only and all-zero geometry cases remain regressions. Declared first/second-null models must retain exact partial effects and separate origin; stale frame/body/slot/lease/width and replay attempts refuse before another allocation. Do not force OOM or invalid handles. Tests at6000/11000 remain separate hidden processes under the fixed module experiment budget.

## Bounded immutable receipts

Used DUMPBIN14.44.35228.0 from the existing MSVC14.44.35207 path, `/disasm:bytes /range:...`, plus an in-memory PE section parser/SHA-256. Only aligned semantic ranges support claims; unrelated trailing fragments were excluded. No mutable analysis database was opened.

| Half-open PE range | SHA-256 |
| --- | --- |
| `14146c25c..14146c261` | `84360f6b4a938ea38f2adc09117208a30d650b385365fbe2cf29723a5ae3cb82` |
| `1414748f7..1414748ff` | `bb630d7d5e07d0a47e852d4a03f92e5ae38787577380e7bf53bd4c34419c13f2` |
| `14147492b..141474930` | `9d1778b889a5d098cf98a9ae27218723086115252050d4bd5f31a0bde85eaa99` |
| `141474942..141474952` | `356ae26d0236b5717b1e578950bd0e0d02bac37dd124c103ead3dbb8f96c8470` |
| `14146eadb..14146eae6` | `2bb60b5db076915afc059f24a1d2a8a63b69367a8c20b1d4f7238b6749ce888e` |
| `1409e47e9..1409e481b` | `660523566cac51a89185168933e182403defd54b1be3661c962e7d1e3bddac40` |
| `1408325c0..14083264e` | `46539c67177ed79e9a9f2fef1b88ce258ecd7196071b89cf6d9313f6ea457991` |
| `140832700..140832788` | `20f214d2bf534a720eddc7d33973beee9dc52a74319d9672aab1691757887aba` |
| `140832790..140832b20` | `74b4506c8d610202e662a120cf63f2cca43f507cf83fc7eb74de18c602486c81` |
| `1401b2980..1401b29d1` | `cb8a0b09fbf581921ca0ac79751d8ce2427a7a3f1873a8d4bd8c7843366a12f2` |
| `141471b7c..141471bb9` | `f9cc22f367e3f55d00ae8f63f4891c223244b13da199bfd53fb0fef7c52dd696` |

Current read-only source hashes still match CP155: ModuleHeapRole header/CPP `7c2c481bef7db0f6af862f74ed4a8943efeee9836b26520a220b1f237635bee3` / `3858445806d83f13d0e6eb22d6167105b9d1ee9b4d84a054a319479c66eb102d`; app binding CPP `d628c7114d8c166a13b71c10c659f66b84c395d9c4b4cb24d33ead04663f75a5`; Viewer `bec69d481702ad198f9a9b9ac7843d10347d9c6787c695b99775db5e72563cd0`; CowVector `6928fd1bfeb9001e0ad31e560b527a8f26f8621c92c4f3345b1ecf764ab0a983`. No executable/deployment or efficiency improvement is claimed.
