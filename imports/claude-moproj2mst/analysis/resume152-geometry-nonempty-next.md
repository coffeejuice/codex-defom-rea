# CP152: first one-value leaf has a closed dimension prefix and an allocator-owner boundary

The smallest source-ready integrated cut is the first actual normal receiver's **dimension-setter prefix**, then an intentional guard before nonempty resize/temporary allocation. Full one-value allocation is not yet admitted. Native bytes now close the normal allocation/publication order and a material unwind distinction: even old count0 allocates a temporary, and no local cleanup handler frees it if the later value-buffer allocation throws. Existing all-zero variable sizing/general recipient completion remains unchanged.

Only this report was written. CP151 Viewer/test remain frozen at `3ad0117fdc715f4dbb8f0495d5d09213987519e4209a330ea07a1c1e0905abc2` / `6f2bb9273d9d02a847acca6ed49ad5dd9ea19b730a09bf93a8498ebd5228fcfa`. Investigation used bounded immutable PE/DUMPBIN plus retained source/captures; no source/build/test, GUI/original execution, REA call/import or mutable REA database access occurred.

## Exact first positive-size profile

Use the existing initial high same-frame source/primary/six-BC/node-n1 prefix. The canonical registry's ordinal0 is actual StateVarNode id1, normal vtable `1427eac80`, slot+10 -> `1409e49d0`. Admit only D[1]=1, all other ordinary registered widths0, n1, this receiver cur2=1, unique actual empty values body with first/last/end null, bound actual node+10 slot null, canonical receiver/body/order and source/destination/BC ownership. The real empty reset clears old values capacity; recheck that zero-capacity state after mutable lookup rather than infer it from size0 alone. Post widths and six nonnegative BC widths retain their existing independent treatment.

After dim0=1/dim1=1 with cur2=1, wanted value count is exactly1. A positive D width with cur2=0 still wants0 and is not this nonempty branch. All arithmetic is small/exact; do not broaden signed wrapping products or source row bounds.

## Native order, allocations and publications

Original EXE was rehashed as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Half-open immutable byte receipts:

| Range | Meaning | SHA-256 |
| --- | --- | --- |
| `[1409e49d0,1409e4a3c)` | Actual normal virtual sizing leaf | `e217c2e3f540250a19f472a3cc73c76e99609827c94f0fe27e2ce8f01cec4b06` |
| `[1409e4270,1409e4296)` | Save previous dim0, then set current dim0 | `d9d3099c52f2d0678ad580b4b99c6e6e8f41fc871792ff76ba69c93d131cec71` |
| `[1409e42d0,1409e42f6)` | Save previous dim1, then set current dim1 | `f38e42db7cf97d2cb1359096619cc8213a7f373e421f82ef65ba4e7228a14a5c` |
| `[1409e46c0,1409e494c)` | Resize including temporary, publication, normal free | `29f8e0e8332005c0155c0cd835ac67c4f2e85ef4bd9322e3f61cfd048cc0dbaa` |
| `[1408325c0,14083264e)` | Value-vector resize dispatcher | `46539c67177ed79e9a9f2fef1b88ce258ecd7196071b89cf6d9313f6ea457991` |
| `[140832700,140832788)` | Insert/detach wrapper | `20f214d2bf534a720eddc7d33973beee9dc52a74319d9672aab1691757887aba` |
| `[140832790,140832b20)` | Body capacity/allocate/fill/publication | `74b4506c8d610202e662a120cf63f2cca43f507cf83fc7eb74de18c602486c81` |
| `[14146eabc,14146eb55)` | Throwing operator-new outer loop | `b3387d73bf1a5d85ccc99fb9b0fc0d51ce05d655a112500f893bf200afe4a24d` |
| `[1401b2730,1401b2755)` | Unique body skips detach allocation | `31cc3619f7c97a30011bc5a51454475eedffd7b2282cb35914568fb942c14af7` |

1. Key-1 passes the actual id/key gate. Leaf `9e4a07` calls actual dim0 setter: write prev0 from old cur0, then cur0=1. It reloads actual id/D[id] and `9e4a26` calls dim1 setter: prev1=old cur1, cur1=1. Neither changes cur2/prev2, values or data slot. `9e4a32` then calls resize(false).
2. Resize compares wanted1 with old actual count0. Its dim/current/previous reads precede `9e47b4` value_data. Native `value_data ->1b2700 ->1b2730` skips detach when the actual body refcount<=1; the supported profile requires exactly1. Old data is null and count0.
3. At `9e47cb..47e4`, old count0 is sign-extended/multiplied by8 and passed as **request0** through `14146c25c ->14146eabc ->14147489c`. This call is not optimized away. Retained `resume119-feature-allocator-14147489c.asm` shows malloc at `14748b4..48bc` normalizes zero to allocation length1, reads source CRT private-heap slot `144700810`, and calls HeapAlloc(heap,flags0,1) at `14748f1`. The wrapper preserves original request0 for handler calls. The successful one-byte temporary has unknown contents and is retained only in the resize stack frame at `rsp+48`/`rsp+68`.
4. `9e4816` calls memcpy with count0. Native copy guards at `146f173` and backward `146f314` return on r8==0 before any byte read/write; null old data is therefore not read. Backward-zero slice `[14146f314,14146f31d)` SHA-256 `b5a0808278f82a241f8981350e3d617b7033e14735008830a0799e0423f6a401`.
5. `9e483b` resizes the actual handle at receiver+28 to1 with fill0.0. Empty unique capacity0 goes through `8325c0 ->832700 ->832790` and capacity test `8327d2 ->8329b9`. Body capacity becomes oldsize0 + max(oldsize0,insert1)=1; `832a1d` calls the same operator-new with **request8**. This is a second allocation, not reuse of the one-byte temporary.
6. Successful new storage receives one0.0 at `832a94..a98`; both old copy ranges have length0. `832ad7` frees old first=null, whose free wrapper returns before reading heap. Body first/last/end publication is ordered at `832ae9`, `832afa`, `832b14`, producing an owned one-double vector.
7. After vector resize returns, `9e485b` reloads actual value_data; `9e486f` publishes its pointer through actual nonnull u30. remap=false skips the remap loop. Only then `9e4935` frees the temporary via `14146d898 ->1414dfc68 ->141471b7c`; only after that free helper returns does `9e493a` mark changed=true and the normal virtual leaf return. Caller leaf-completion ordinal must not advance earlier.

## Allocation owner, failure and unwind facts

Retained `analysis/resume119-feature-context-proxy-allocator.md` qualifies source CRT heap producer `HeapCreate(0,4096,0)` into slot144700810 and the successful allocation/free owner. It also qualifies malloc new-mode144700858, encoded new-handler1447006e0/DecodePointer, CRT errno and outer native bad-allocation state. Neither host malloc/new success nor an on-disk global value supplies that module/process runtime-owner binding.

On HeapAlloc null, malloc may call its source new-handler according to new-mode, set source errno12, and return null; outer operator-new performs another handler occurrence/retry or native bad-allocation throw. A nonzero malloc new-mode can cause two distinct handler reads/calls for one failed sequence. Original request size is0 for temporary and8 for data; normalized HeapAlloc length1 does not turn handler size0 into1. Dynamic heap/handler/errno state must be read at its actual occurrence, not frozen from a prior source call.

The exact scopes have unwind flags0 (no EH/UH/chain handler):

| Scope / unwind info | Raw aligned info bytes | SHA-256 |
| --- | --- | --- |
| resize9e46c0 /1432c7fe4 | `0111020011f20a30` | `c2233e11bc7b172409e1a9c70bca0fc18dfa86078f12fa8a63e01ef506a153b1` |
| vector resize8325c0 /143401288 | `0114020014321030` | `d9c225e7652d826832fca58e820e65ef930b080f931ef180020c3f6ccc4595c2` |
| insert832700 /14334a0e8 | `0118010018620000` | `af2a87103e8b8bb6f500326f38ca0e3aece5bc4ebd078ac2458aec920f193f80` |
| body insert832790 /14315e880 | `011b02001b011100` | `abfefc230d4675431ead38d84da04d231b0430b37c26de25b38a6c2f3e7bd58c` |

Consequences for this empty-capacity profile:
- Temporary allocation failure occurs after dimension writes but before any temporary pointer is returned/stored; values/body/slot remain empty/null, and the normal leaf has not returned.
- Data allocation failure occurs after the one-byte temporary succeeded, before value-body pointer publication or bound-slot publication. These scopes provide stack unwind only; they do not free the retained raw temporary on that throw. Do not add a RAII cleanup and call it native unwind. Earlier receiver/caller mutations also remain. This is a local cleanup fact, not a claim about every enclosing exception handler or eventual module heap teardown.
- Temporary normal free is after value-body and bound-slot publication. Native nonnull HeapFree can fail, then CRT errno/GetLastError mapping occurs before the free helper returns; ignoring its result does not prove the block was freed. Leaf completion requires either that exact continuation or an explicit pending guard, not an unconditional “free succeeded” receipt.

## Candidate boundary and smallest integrated action

Candidate `source/port/defmodel/src/layout/state_var_base.cpp:137` uses raw malloc1 for old count0, existing CowVector.resize, publishes the slot and frees temporary. This preserves the broad successful order and does not add RAII on vector throw, but it does not by itself qualify source private-heap/new-handler/errno ownership or exact failure continuation. Its CowVector data is currently host-allocated and freed by that port's own allocator. Attaching a private HeapAlloc pointer to it without a compatible allocator/deallocator owner would be invalid.

`source/apps/moproj2mst/src/model/feature_crt_runtime.hpp` offers explicit isolated `prepareOwnedTestProcess` ownership with no default conversion hook. Its source-role heap/handler records are not bound to the current GuiModel/StateVar allocator, and it is not a generic vector allocation interface. A standalone declared heap success or test-process runtime must not be passed off as this active conversion receiver's allocator. A full value-allocation cut therefore still needs an explicit actual allocator mapping/lease compatible with the live values body, actual normalized0 temporary and8-byte returns, and represented failure/free continuations. No original live heap/handler witness was captured here.

The **source-ready Viewer-only prefix** avoids that allocation dependency:

- Admit the exact ordinal0/id1/D1=1/cur2=1/empty-capacity profile after real BC/n1 publication, under the same current frame/source/destination/BC/registry owner guards. Keep all other ordinary widths0. Require no prior started positive recipient with a completed zero-width receipt whose shared D facts would be invalidated; this is the first eligible main recipient in the active reader. Zero/caller skips before it remain allowed.
- Capture the actual body/ordered receivers/dimensions as in the zero-size dispatcher. Perform actual mutable lookup0 through the existing forwarding seam and recheck current frame/actual slot/receiver/type/value-body capacity and owner facts after it.
- Execute **existing real typed methods** `set_dim0(1)` then `set_dim1(actual D[actual id])` in the native order. Do not directly assign fields or call the unrestricted virtual slot, because that would enter unresolved allocation. This explicitly implements a partial ordinary-leaf prefix, not a completed virtual dispatch. It uses the verified normal vtable/type to select that partial body.
- Record a distinct dimensions-committed/allocation-pending stage; keep completed whole-leaf count0. Stop before calling resize/value_data/temporary operator-new0. Preserve previous/current dimensions, all BC/node/source effects and the exact installed barrier. Do not process receiver1, copy main coordinates, Stop, retire, issue second getter or later resume. The all-widths0 loop continues to use actual full virtual calls unchanged.

This is a small actual native state advance and a reviewable guard before the first allocator occurrence. It does not produce a nonempty value or release the public scene/editor guard. If the next checkpoint requires actual value allocation rather than this prefix, the allocator-owner/failure inputs above are the precise blocker.

## Minimal tests and subsequent witness

Proposed files after assignment: Viewer CPP, existing `test_geometry_a_producer.cpp`, new receipt; no shared header/CMake or allocator source change for the dimensions-only cut. Target `m2m_mdt_geometry_a_producer`, exact CTest `m2m.mdt.geometry_a_producer`.

- Use actual first fresh main recipient with D1=1/other ordinary widths0, cur2=1, source extent3, real primary/BC owners. Seed old cur1 to7 while dim0=0/values empty. One actual lookup0 occurs; require prev0=old0,cur0=1,prev1=7,cur1=1,cur2/prev2 unchanged, values body/capacity/count/slot unchanged, other27 receivers untouched, arrays1/node timer2/unknown scene and no parked replay after D repair. This observes actual setters and distinguishes the new partial stage from BC-only or full value allocation.
- Late lookup changes owner/body/binding/D1 or cur2; recheck before setters and retain actual lookup effects without dimension/heap/copy/Stop claims. Negative D1, non-first positive ordinary id, nonempty/shared/misbound values or cur2!=1 keep their earlier guard. Do not allocate large values or force process OOM.
- Update the existing D1=+1 nonzero-width preflight expectation deliberately: that exact profile now reaches one lookup/two setters then remains unsupported; D1=-1 still performs no leaf. Audit BC-only fixtures using D1=1 for stale “no dimensions changed” assertions. All-zero general0..4/8 recipients remain normal and unchanged.

For a later actual one-value continuation, require same live receiver/frame/body and allocator owner, trace both normalized temporary request0/HeapAlloc1 and data request8 returns, fill/body/slot publication order, null-old-free and actual temporary-free result. Declared-failure tests must separately model first allocation failure, second failure with retained temp, and post-publication free failure with no invented unwind or full leaf completion. Existing native handler/errno/exception records must be bound to their actual source occurrence. These facts cannot be inferred from a standalone successful allocation or host std::bad_alloc catch.
