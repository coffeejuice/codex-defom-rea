# CP132 independent review: geometry node-skip prefix

## Finding: pre-skip MAR table access is not admitted

The zero-`+0x87c` callee boundary and same-owner check are otherwise well bounded, but the frozen candidate can reach `NodeSkipLoopCommitted` without accounting for a native access that precedes every caller skip. Treat this as an actionable qualification gap before claiming the complete caller decision prefix.

In retained `[14082d7d0,14082dc8a)` caller code, loop ordinal `i` is kept as signed 32-bit at `[rsp+54h]`. After `step_object_at(step, i+1)`, native instructions at `14082d9f0..da17` execute in this order:

1. `movsxd rcx,dword ptr [143dbdb48]`: sign-extend actual `g_mar_index[27]`.
2. `mov eax,dword ptr [step + rcx*4 + 0x1b90]`: read the selected step's MAR table entry.
3. `imul eax,dword ptr [rsp+54h]`: take the low 32-bit product.
4. `movsxd rcx,eax`; `lea rax,[base + rcx*4]`: sign-extend and form the per-object address without dereferencing it.

Only after this sequence does the caller test null/type-7 at `da1c..da2f`, `+0x3610` at `da31..da42`, and state at `da47..da4e`. The node-only call is at `dbe9`; its `+0x87c==0` callee exit prevents allocation/copy, but it does not retroactively skip the earlier step-table read.

The candidate loop at `viewer.cpp` (CP132 frozen SHA-256 `32d5335cf545d99b87264fc5beaa4d136c98e94fbaf92230088f59e016d96e0b`) computes neither the selected `g_mar_index[27]` table read nor a corresponding admission check before its null/type/flag branches. Its comment correctly avoids using signed C++ multiplication or out-of-range pointer arithmetic; the native 32-bit IMUL/LEA result is otherwise dead on this state-false/node-only path. But without constraining the actual selector to the fixed `DB_STEP_DATA::u1b90[0x8c]` extent, the candidate may continue for a selector where the original performs an out-of-bounds read and faults before any caller skip.

## Minimum safe correction

Before entering the loop, require actual `g_mar_index[27]` to be within `[0, 0x8c)` for this selected 3D table profile, and confirm the table profile is the one established by the already reached `select_index_tables` call. This makes the native stride read in-bounds for every loop iteration. The selected stride value may be read/recorded as a bounded input if the implementation needs an explicit receipt. The candidate need not form the dead per-object address to reach the zero-node return.

If a later implementation consumes that address, it must not use signed C++ `i * stride` or pointer addition. Reproduce the native arithmetic with defined operations: multiply the `uint32_t` representations modulo 2^32, reinterpret/sign-extend that 32-bit result, then add the scaled offset to a `uintptr_t` (or otherwise prove a bounded object extent before pointer formation). Never dereference the address in this node-skip slice.

## Other checked conditions

- The single initial `withStep` callback acquires the real current step before installing the barrier and retains the borrow synchronously through the query, arrays timer, object loop and node-timer Stop. No pointer is captured for later reuse; parked refreshes are blocked before new reads/effects.
- Ordered region state derives from the actual `g_a_index[27/32/35]` comparisons and corresponding `step->u15f8[0x1a/1b/1c]` signed-positive predicates. Caller skip order is null, type 7, byte `+0x3610`, then assignment-state/flag.
- For an eligible main object, `object->owner == nativeStep` binds the native `+0x3a98` owner to the same borrowed step before calling the existing helper. `obj_read_node_arrays` selects tables from that owner before checking `+0x87c`; its zero branch returns before allocation or copy. `+0x87c` is distinct from row count at `+0x880`.
- The implementation's refusal on state=true, non-node-only flag, foreign owner, or nonzero `+0x87c` occurs before entering an unqualified branch; query/version/COW/arrays-timer effects stay committed and no node timer Stop is fabricated.
- The exact `nodeTimer.count == 2` guard is conservative: its two native entry checkpoints are produced by `start("Start")` and the subsequent `ReadSingleAArrayFromDB` checkpoint in this admitted route. Stop appends the third checkpoint. Tests must leave an unrelated/prepopulated timer count unsupported rather than resetting it.
- No assignment allocation/copy, element-assignment path, optional RGEOM mesh write, reader return, barrier retirement or later resume authority is admitted.

## Source qualifications and scope

The review used `analysis/resume132-geometry-next.md` (the CP132 native evidence map), `analysis/resume132-geometry-node-skip.md` (implementation receipt SHA-256 `04d2e7681ffbb454ea4b671f490f703767846e656450ce57166f56c6d4348247`), retained caller ASM `analysis/wave3-editor-mds-gui-coordinate-lazy-read-14082d7d0.asm`, current `viewer.cpp`, `obj_read_node_arrays` and `StepTimer` source. Original binary identity in the controlling report is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

No source, tests, build, GUI, original target or mutable REA state was changed or executed. This report records a review blocker and asks for a bounded selector admission before final review; it makes no runtime/parity claim.

## Re-review after frozen repair

The source owner repaired the pre-skip selector access and released these hashes:

| File | SHA-256 |
| --- | --- |
| `src/engine/viewer.cpp` | `3ea1b91dcf933def29aca5ab253971b8aee3a4670c2dfef57a77732ca6c6e6fa` |
| `tests/mdt/test_viewer_box.cpp` | `fdc51a26328c016d0e26d5e982094b2aa5961a15d7b7466be7cc762c3f823b51` |
| `analysis/resume132-geometry-node-skip.md` | `94285966702f73dceb2bd2b9a7f5940bef307d6e252dd33a07b52fcf30d5da0e` |

The revised loop now loads `g_mar_index[27]` after each object lookup, rejects indexes outside `[0,0x8c)`, and reads `nativeStep->u1b90[index]` before null/type/`+0x3610` skips. This closes the material gap for the admitted in-bounds table profile. Extreme in-range stride values remain safe because the candidate deliberately omits the dead per-object product/address on the node-only path. The updated focused case covers invalid selectors `-1`, `0x8c`, `INT_MIN`, `INT_MAX`, plus in-range boundary selectors `0` and `0x8b` with extreme stride words.

No further source issue was found in the repaired slice. The actual borrowed step still scopes the full synchronous path; the owner check binds object `+0x3a98` to that step; the `+0x87c` zero path calls the existing helper after owner-table selection and before allocation/copy; caller-state and skips remain ordered; and the node timer Stop is reached only after all objects take a supported skip/zero-return path. Refusals retain earlier effects and do not clear the barrier or replay on refresh. The pending reader return and later geometry work remain guarded.

Final status: the original finding is closed by the revised frozen source. This remains a static review; root owns compile/runtime validation. No build, test, GUI, original execution or mutable REA action occurred in either review pass.
