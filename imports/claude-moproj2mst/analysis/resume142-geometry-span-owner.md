# CP142: equal-width geometry copy needs allocation provenance, not nominal A length

The destination is the actual main object's node-coordinate allocation, and the source is the actual current step's A allocation. The equal-width branch is not ready to admit from raw nonnull pointers and header lengths. A smallest source-backed producer prefix exists in the nonnull-DB loader: query the actual high-version entry, allocate A when absent, then handle an empty selected region. That prefix must preserve its version/cache mutations and exact frame lifetime before any copy is released. Do not allocate or seed A in CP141's null-companion path: native null return performs neither action.

This report is the only write. No source/build/test execution, GUI/original launch, shared analysis database or mutable REA operation occurred.

## Exact native copy and bounded evidence

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Offline DUMPBIN was decoded from function starts; only relevant blocks/calls were returned. Half-open byte receipts:

| Range | Meaning | SHA-256 |
| --- | --- | --- |
| `[1408422c7,140842354)` | Equal-width LLCRZ source address/count/copy | `96746cc457c5b8225807caa92a6614ab8e83d48820b580a99e44d087afb149a3` |
| `[140828060,1408283d5)` | Actual single-A loader | `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796` |
| `[1408281fb,140828275)` | Fresh A publication and empty-region cache commit | `286c53802c1db4aacf4fa6489fa9656c89577c20e3a037b246b0142f79b804f3` |
| `[1409c83b0,1409c8408)` | Zero/null or calloc(count,8) allocator and failure diagnostic | `2ca64c1da768dd96662135394af3e2d8621a13a257c78ab42d2ebb046ea00ef4` |
| `[14082bf10,14082bf5a)` | Actual GUI names/up-to-date predicate | `c6e2177bc1357b7f5fe80b828e89967a50695d4d953c557bfa3aeea90eb06ace` |

For exact selected region11, local s remains0. Native `8422e1` compares D[0] with actual stored coordinate width. Equal proceeds through `8422ed..842307`, computing `(object+3984 - 1) * D[0]` with low32 arithmetic, sign-extending it and forming the A pointer at `842312`. `84232b..842337` computes `object+880 * D[0]` in low32 arithmetic, sign-extends and multiplies by8 for the copy byte count. Destination is main coordinate pointer +8a8. The CP141 mismatch jump at `8422e3` intentionally stops before these operations.

## Actual owners and missing span proof

`DB_STEP_DATA` owns A at +1b78: `step_free_contents` frees/nulls it (`source/port/defmodel/src/step/db_step_data.cpp:186`). A strong current-step borrow keeps the step alive during a synchronous call; it does not establish an arbitrary raw pointer's allocation extent or origin. The logical header length +1b6c and calloc-capacity field +1b74 are distinct.

The ported writers are not one uniform capacity protocol. `w3_arrays.cpp:179` replaces A with realloc(header A length), while `:390` sets +1b74 and allocates a different compact layout. `w4_fold.cpp:36`'s reserve function frees/reallocates when requested capacity exceeds +1b74. `w2_objarrays.cpp:483` allocates maximum-region-length+2 when A is absent, without setting +1b74. The last behavior is independently verified in the original GUI: `828215` loads +2b24, `82821b` adds2, `82821e` calls the allocator, `82822e` publishes +1b78; there is no +1b74 update. Therefore neither nominal length nor a generic nonnull-pointer/capacity observation certifies every allocation producer. An arbitrary reserve call at this getter would add a native effect absent from the null-companion branch.

Main destination storage belongs to the actual object's `Cls_14017ef20` at +8a0. `bc::alloc_node_arrays` frees/reallocates through `node_set_alloc` using actual object count +880 and D (`bc/node_arrays.cpp:69`). `Cls_14017ef20::alloc` reallocates coordinate bytes for n*D[0] and records n (`llobj/cls_14017ef20.cpp:91`); its destructor/free function frees +8. These establish real producers, not a universal capacity accessor. A preexisting +8a8 pointer alone does not prove that its current allocation matches newly changed object count or D[0].

The minimal eventual copy proof is a current-generation receipt from those actual producers: same live step/object, exact source/destination pointer, allocation element extents and relevant counts/widths, still matching after the last optional-node COW callback. For positive d=D[0], n=main node count, first=object+3984, require first>=1, n>=0, defined nonnegative 32-bit offset/count products, `(first-1)*d + n*d` within the proved A extent, and n*d within the proved destination extent. Actual separate owning allocations must preclude overlap. Do not use wrapped or null pointer arithmetic, a fake lease, or newly seeded numeric data to satisfy these conditions.

## Smallest next producer action

The original loader's null-companion check returns before its directory query and all allocation. The actual GUI predicate `14082bf10` returns false specifically for u1b58=0/dimension3/version>=6000; the DEF_PRE port's constant up-to-date fold cannot substitute for this branch.

For a real nonnull DB owner, the loader queries sizes/version at `82814e`, applies that predicate at `828167`, validates selected region/header lengths/cache, then reaches fresh allocation. If +1b78 is null and `u2b24+2` is a bounded positive nonwrapping count, the successful actual allocation gives an exact source extent that can be retained in the same live frame. For selected region length +2b28[id]==0, `828265` sets cache[id]=1 and returns at `828270`, without file-array decoding. That allocation/cache prefix is source-closed under its real prerequisites; allocation failure diagnostic and nonempty file read remain separate guarded branches.

Current Viewer already pins actual Project DB ownership and a live directory receipt for its admitted low-record path, but a high-record query/allocation/cache prefix and exact high-frame retirement are not implemented. Existing low-only retirement cannot be reused by relaxing its version predicate. A successful high empty-region prefix changes the second getter: cache1 means its second COW takes the cached reader return, rather than repeating CP137's cache0 null loader. A future cut must preserve that one-reader/two-getter order and partial allocation/cache effects, and guard before any unproved equal-width assignment. Fresh destination allocation must likewise be qualified at its real callsite, or remain unresolved.

## Concrete test boundary

- Preserve CP141 equal-width refusal with null A, existing destination coordinates and both optional widths0/4. Copying is still guarded; do not turn those tests into seeded span admissions.
- A future producer test should use the actual owned DB handle/directory entry/header preconditions, query the high-version entry and exercise null A -> actual bounded calloc -> empty-region cache1 inside one installed frame. Check exact pointer extent, zero initialization, version/cache order, same current step and one reader/two getters. It must not resume or reacquire a parked frame.
- A later copy test needs real producer receipts for both allocations; use nontrivial first-node offset and multiple rows to expose offset/length mistakes. Reject short source/destination spans, stale/replaced pointers, changed counts/widths/generation, selector aliases and overlap before copying, while retaining prior reader/producer effects.

No equal-width copy source edit is recommended until that actual owner chain is bound. The useful next implementation is the ordered high-record producer prefix under the existing DB owner, not an isolated span observer or a metadata-only seed. `viewer.cpp` remained at CP141 SHA `c0d43362c4959a56062f4f7c6150e1ee29c9502d8663e7b57733acf52f3bf1ab` throughout this lane.
