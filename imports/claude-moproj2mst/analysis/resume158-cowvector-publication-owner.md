# CP158: CowVector publication requires synchronous release ownership

The first body/slot publication remains guarded at **0x140832adc**, before store **0x140832ae9**. The missing contract is a retained per-body/per-block release owner that can finish the source free routine synchronously, including its HeapFree-zero errno continuation, during normal calls and automatic destruction. A successful B8 allocation/fill cannot supply that contract.

This pass closes one previously explicit static gap: **a throwing shared clone frees its newly allocated body32 through a native EH cleanup action**. It does not roll back the earlier old-body reference decrement, publish the new handle, or run the incomplete clone's payload destructor. This is a source-ready lifecycle rule for the eventual bridge, not authorization to publish B through today's host-free CowVector.

Only this report was written. No source/build/test, original execution, GUI/app control, mutable REA/session, allocation experiment or deployed executable was touched. The reverse-engineer-anything workflow used bounded DUMPBIN 14.44.35228.0 and the existing section-aware offline PE reader. All claims below are static observations or expressly identified deductions; no unwind/HeapFree outcome was observed at runtime. Original DEF_GUI_MO_64 **v11.0.2**, `evidence/original-binaries/DEF_GUI_MO_64.exe`, independently rehashed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses are preferred image VAs, requiring relocation for a live run.

## Publication and release are one ownership obligation

The exact unique-empty first-scalar profile does not allocate a new body32. Its existing body receives B at first+8 (`832ae9`), B+8 at last+0x10 (`832afa`), and B+8 at end+0x18 (`832b14`). After returning from resize, `9e485b` obtains actual mutable value_data; `9e486f` publishes that pointer through actual u30. For remap=false, T is then passed to source free at `9e4935`. Only **after that helper returns** does `9e493a` set changed=true and return the leaf result. Publishing a live pointer therefore commits both B's later lifecycle and the immediately required T free.

The source routes are distinct but converge:

| Allocation/storage | Native acquisition | Native release |
| --- | --- | --- |
| First scalar T | request0 -> normalized HeapAlloc1, CP152/157 | `9e4935 -> 14146d898 -> 1414dfc68 -> 141471b7c` |
| First scalar B | request8 at `140832a1d`, CP157 | old-payload grow/free; body clear `140b6050c`; body dtor `1401375bc`; all via `14146d898` |
| Detached body32 | request0x20 at `1401b2784 -> 14146eabc` | completed body's deleting dtor `140137587 -> 1414dfc68`; constructor-throw cleanup `1419f8cfd -> 1414dfc68` |
| Detached payload | positive signed count*8 at `1401b28dc -> 14146c25c` | completed body payload release via `1401375bc`; no separate payload exists if that allocation throws |
| Existing candidate empty body | already host-new allocated, current CowVector ctor | must retain host-body provenance independently of future source-role B |

`14146d898` and `1414dfc68` are exact tail jumps to the same source free routine. Nonnull free rereads current native module heap slot `144700810`, passes flags0 and the block to HeapFree at `141471b92`. It does not carry a heap handle inside the body. A reconstruction needs canonical module identity and generation plus retained block/body provenance; that bookkeeping is not a claim that the original body contains those records. A module slot must not be silently rebound while any such blocks survive.

HeapFree nonzero returns without the errno branch. HeapFree zero calls errno provider `141474134` at `471b9c`, retains its returned pointer, reads GetLastError at `471ba4`, maps at `471bac -> 1414740ec`, writes errno at `471bb1`, and then returns. The provider must preserve the same thread's failed HeapFree LastError until that read. The routine returns no reliable freed/not-freed Boolean contract to these callers. **Raw zero plus a completed errno tail still permits caller continuation.** T can remain unreleased while the leaf returns changed=true; payload clear can null fields after its failed release; deleting dtor can then attempt body-storage release after payload release failed. A failed block therefore needs an opaque unresolved history independent of later body destruction, without automatic retry or claiming success.

CP154 already proves StateVar clear -> detach -> body-clear/free -> null body fields -> null bound slot, and last-body release -> payload dtor -> flag1 body-storage free. Those synchronous callers cannot return Pending. A partial free/error owner cannot be substituted into them. Current source still uses `std::free` for payload and `delete` for bodies; source SHA-256s below confirm no lifecycle hook was added here.

## New original PE witness: clone construction failure cleanup

The complete detach procedure `[1401b2760,1401b27d4)` decrements old refs at `277a`, requests body32 at `2784`, stores the returned raw body at frame+0x28, and calls clone ctor `1401b2890` at `27a3`. New-handle publication is only at `27cc`.

The ref helper `[14006bdd0,14006be0c)` explicitly subtracts1 from the old body's 32-bit refs and returns whether the result is0. Detach ignores this return. There is no inverse increment in its normal body or cleanup action.

The PE exception-directory record points to unwind info **143174b08**: version1, flags2 (unwind handler), one unwind-code slot. The aligned handler RVA is **0146e20c** and its language-specific FuncInfo RVA **021ec2f0**. The latter contains FH3 magic **19930522**, maxState1, one-entry unwind map **1432cc560**, no catch/try entries, and three IP-to-state pairs at **143174b18**:

| IP start | State |
| --- | --- |
| `1401b2760` | -1 |
| `1401b278e` | 0 |
| `1401b27c2` | -1 |

The unwind-map entry is `{toState=-1, actionRVA=019f8cf0}`. Action **[1419f8cf0,1419f8d08)** receives the established frame in RDX, loads its frame+0x28 raw body pointer at `8cf9`, calls **1414dfc68** at `8cfd`, restores registers and returns. It does not call body dtor `1401375a0`, free the old payload, publish the handle, or repair old refs.

Clone ctor **[1401b2890,1401b297d)** has unwind flags0 at **1434212c8**. It initializes refs, obtains a signed32 source count, and for count>0 calls payload-new at `28dc` before the first pointer store `28f0`; on success it publishes first/last/end and copies. For count<=0 it explicitly nulls the three pointers. The copy loop is a typed double load/store loop with no throwing call.

Under ordinary FH3 propagation, these bytes establish the bounded failure contract: body32 allocation throw occurs before state0, leaves the old handle/ref decrement in place, and has no new body to release; payload allocation throw while in state0 runs exactly the raw-body-storage free action, then propagates. It does **not** invoke a destructor that would read uninitialized first/last/end. This differs from adding a general payload/body RAII guard or restoring old refs for a stronger exception guarantee. This is static exception-routing evidence, not a captured throw, and does not close the throwing-new/FLS/errno backend itself.

The EH cleanup's body free can itself yield HeapFree0 and must finish the same synchronous errno tail before the native unwinder continues. A candidate placement-construction bridge therefore needs distinct storage ownership and construction-complete state, with body32 cleanup on ctor failure routed through that same release owner.

## Smallest useful next implementation and original witness

No further actual publication slice is ready through current APIs. The smallest coordinated implementation is **a complete synchronous source-free role plus per-body dispatch**, preserving the 8-byte handle/0x20 body/0x38 StateVar layouts. Prepare all provenance backing before source/refcount effects. Keep the current host body host-backed while its first B is source-backed; route every consuming method (clear, destruction, assignment displacement, grow/push_back, shared detach/clone) from the actual body's retained provenance. Source body32 must use separate storage acquisition, placement construction, the now-qualified ctor-failure storage release, and explicit completed-body destruction/storage release. A missing owner must never fall back to host free. Unbound containers retain host behavior. This is one integrated lifecycle change, not a Viewer-only continuation.

The new clone cleanup rule can be implemented/tested as part of that bridge with declared order/failure tests, without forced OOM or invalid handles. It does not justify another candidate temp0/B8 experiment, and isolated metadata preparation adds no publication progress. Backend FLS/PTD/new-handler/error ownership remains a separate dependency; a retained-initial mapper or host errno is insufficient.

For a **small precise original success witness**, capture one fresh admitted high-reader invocation through first scalar publication and T release, then the same body's natural clear/destruction. Bind process/module SHA/base/thread/frame generation; actual receiver/body/u30/refs1; allocation heap-slot reads and T/B returned ranges; ordered first/last/end and slot stores; T HeapFree arguments/raw result/helper return/changed=true; and later same-B HeapFree arguments/raw result before actual pointer nulls and slot null. Observe host-independent original body-storage release separately if the body dies. This is substantially smaller than general shared growth/clone validation and directly checks the first published buffer's release routing. No original pointer or heap handle becomes candidate authority. Positive success observations qualify only that route; the immutable EH/free-zero contracts still need candidate representation and representative original failure validation when available.

**Explicit guard:** retain CP157 `BeforeBodyPublication` at `140832adc`. Do not execute first-store `140832ae9`, slot-store `1409e486f`, T nonnull free, leaf-completion, later receiver, Stop or retirement until the complete synchronous free/error owner and all actual body/payload consumers are installed. No leak-on-dtor escape, process-long model retention, host malloc relabeling, destructor early-return or post hoc parked-failure promotion closes this guard. No original parity or completed lifecycle is claimed.

## Immutable byte receipts

Half-open VA ranges; hashes are PE bytes, not DUMPBIN text. First disassembly windows ending inside instructions were excluded and aligned ranges recaptured. PE section mapping used existing `tools/doe_static_pe.py`; exception tables were decoded directly from immutable bytes.

| Range | SHA-256 |
| --- | --- |
| `14006bdd0..14006be0c` | `0fc86410dcd4ee1c2054639e0f514678b99730e36902eef86aac5a9d6c19d182` |
| `1401b2760..1401b27d4` | `1d3c56085667f49d6d5f8de7bc9f6253223513fdf79ba16dca228173bafb723b` |
| `1401b2890..1401b297d` | `6f1cdbaf97acd4999d20f733ac5e4fe3719b5218420d3fe72a0c1ad7a34835b1` |
| `1419f8cf0..1419f8d08` | `d454b775089241fcc956458e1325093839de798d2d05d532e5118b6bd2426fc7` |
| `143174b08..143174b18` | `69ae6142e5cc1bb4608007b31a1ef3c1aba644714d37ba7f008d0241556902ff` |
| `143174b18..143174b30` | `2c5869ada581f19af554f2e4d69ac3716a7a78695760960308c5e963c5209fc8` |
| `1421ec2f0..1421ec318` | `0e53b52f423106794153d858379aed80646c4ff01a45d46d00426769cc52bcf0` |
| `1432cc560..1432cc568` | `3454176c5ec37409cdcb5f19b12556676a0324f60cb94b52304ad8d359dd9086` |
| `1434212c8..1434212d0` | `136991d2f5bc00abebfc21efaa20262eec8617bc5c87ff6b409959dccad91329` |
| `140832adc..140832b20` | `0fc7d8dfabee1d880920433caa391778e72c2b818c1632d6503380bfed4ecf3f` |
| `1409e4853..1409e4872` | `48a382357a294a987921f7b70ce006a60e231f9331c3f88c61679328aa34c5b8` |
| `1409e4926..1409e494c` | `dbaf1342a8c69bf01bd94ee6bade33f84cad2ba2e8a82267275bd922f47f2579` |
| `14146d898..14146d89d` | `9703a21979bc6a7105e0e0e651cf542f49d922cecd9c129074a78e6e26e633c5` |
| `1414dfc68..1414dfc6d` | `6648cd6b98e65e1617c9f4cab20900bf9382741e32b04fc6be2d054aef811bde` |
| `141471b7c..141471bb9` | `f9cc22f367e3f55d00ae8f63f4891c223244b13da199bfd53fb0fef7c52dd696` |

Source qualifications read and rehashed: CP154 allocator bridge `395f37b14e227c4cf5f2b8a4dbbe0efe48745a4bd0fe2ff7b3c9131fc9e96c19`; CP157 next-after-temp0 `314184eafca55b3407258627bea93f61a573517a2a9bd6d95783ceb543466b08`; CP157 unpublished implementation `08bc5e30e529f6b6e014706bc5b20fe0ff4cbe1c73c1cb1e15ffcaf827d39bf8`. Current CowVector `6928fd1bfeb9001e0ad31e560b527a8f26f8621c92c4f3345b1ecf764ab0a983`; StateVar CPP `6bd7f9cf4ff638994311f888f573a079ec0558a7ce725df81329aa0719ff535d`. No speed/token improvement is claimed.
