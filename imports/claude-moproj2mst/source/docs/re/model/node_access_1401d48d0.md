# Node-set accessors and the "layout sync" FUN_1401d48d0

Packet p67-3. Port: `port/defmodel/include/defmodel/node_access.hpp`,
`port/defmodel/src/node_access/node_access.cpp`; test `port/defmodel/tests/test_node_access.cpp`.
Sources: `deform-rea-work/out/p673/extra.c` (decompiles), `out/p673/e/acc.c`, `out/p673/e/acc_L.txt`
(listings of 1401808d0, 1401809c0, 1401d2b70, 1401d48d0).

## 1. Accessors on the node set (Cls_14017ef20, LLOBJ_DATA+0x8a0)

* `FUN_14017f020(set)`: returns `set+0x130 == 0 ? 0 : set+0x130 + 0x15f8` **[fact]**. The owner at
  `set+0x130` is the model `DB_STEP_DATA` (`+0x15f8` is its 0x4f-int component table `u15f8`, the
  `dims` argument of every allocation) **[inferred]** from the table offset and the 1401d48d0 call.
* `FUN_1401808d0(set)` (coordinates): `dims = 14017f020(set)`; `14017ffe0(set, *(int*)(set+0x12c),
  dims, 0)`; if `set+0x130 != 0`: `int k = 0; 1401d48d0(owner, &k)`; returns `*(double**)(set+8)`
  **[fact]** (listing 1401808d0..140180938).
* `FUN_1401809c0(set)` (URZ in GENBCC.md): the same with key 2 (`R9D = 2` at 1401809d3, `k = 2` at
  140180a00) and returns `*(void**)(set+0x18)` **[fact]**: the data slot of state variable id 2
  (14017f2f0 binds id 2 to `set+0x18`, p1-04 `Cls_14017ef20::kRegs`) **[fact]**.
* `FUN_14017ffe0(set, n, dims, key)`: already ported as `bc::node_set_alloc` (node_arrays.cpp).
  For `n > 0`: key 0 / -1 / -2 and `set+8 == 0` → `set+8 = calloc(n*dims[0], 8)` (140257150);
  key -1 / -2 → six int arrays `set+0xf0..0x118 = calloc(n*dims[0x12+k], 4)` (140256dc0) and
  `set+0x12c = n`; then always `140329e60(set, n, dims, key)` **[fact]**. 140329e60 calls slot 2 of
  every state variable, which resizes only the variable whose id is `key` (or all for -1)
  **[fact]** (p1-09 `StateVarBase::vslot2_140301690`). No variable of the set has id 0
  (ids 1..6, 8..0x11, 0x18..0x23) **[fact]**, so key 0 touches only the coordinates.

## 2. FUN_1401d48d0(model, const int* which), 3392 bytes

Two arguments **[fact]** (prologue stores RCX and RDX only; Ghidra's `param_3`/`param_4` are
artefacts). Body:

1. `1401d48e1`: stores -1 at `[RSP+0x20]`, the fifth outgoing stack slot; the first four calls of
   1401d4400 seem to pass it, but 1401d4400 takes three arguments **[fact]**, so it is unused.
2. `if (FUN_1401d2b70(model)) return;` (1401d48f1..1401d48fd jump to the epilogue 1401d5608)
   **[fact]**.
3. Otherwise a chain of `if (*which == code || *which == -1)` tests, each calling
   `FUN_1401d4400(model, &id, &flag)` ("ReadSingleAArrayFromDB": re-read one object array from
   the database) or, for code 0x11 / 0x3c, `FUN_1401cf930` / `FUN_1401cfc30` (rebuild node /
   element names); the last seven only when `FUN_1401cc180(model)` (step version `+0x1b64`)
   is >= 11000 (`CMP ..,0x2af8`) **[fact]**. Code → (id, flag), in order:
   0→(0xb,1) 1→(0xc,1) 2→(0xd,1) 3→(0xf,1) 4→(0x10,1) 5→(0x11,1) 6→(0x12,1) 8→(0x14,1)
   9→(0x15,1) 10→(0x16,1) {0xb,0xe,0x10}→(0x17,1) 0xc→(0x37,1) 0xd→(0x1a,1) 0xf→(0x19,1)
   0x11→1401cf930 0x18→(0x1b,1) 0x19→(0x13,1) 0x1a→(0x1d,1) 0x1b→(0x22,1) 0x1c→(0x26,1)
   0x1d→(0x2e,1) 0x26→(0x1d,0) 0x27→(0x1e,0) 0x28→(0x1f,0) 0x29→(0x20,0) 0x2a→(0x21,0)
   0x2b→(0x22,0) 0x2c→(0x23,0) 0x2d→(0x26,0) 0x2e→(0x1c,0) 0x30→(0x25,0) 0x31→(0x29,0)
   0x32→(0x2b,0) 0x33→(0x2a,0) 0x34→(0x2d,0) 0x35→(0x2e,0) 0x38→(0x2c,0) 0x39→(0x31,0)
   0x3a→(0x32,0) 0x3c→1401cfc30; version >= 11000: 0x22→(0x3a,1) 0x23→(0x3d,1)
   0x36→(0x2f,0) 0x3b→(0x38,0) 0x37→(0x39,0) 0x3f→(0x3b,0) 0x3e→(0x3c,0).
   flag 1 = node arrays, 0 = element arrays **[inferred]** (the flag-1 codes are node-set ids).

`FUN_1401d2b70(model)`: if the byte `model+0x1b58` is 0 it calls `FUN_1400015f0(model)`
(`model+0x1c == 3`, pure) and discards the result; it returns 1 on both paths (`MOV AL,0x1` at
1401d2b96 and 1401d2b9a) **[fact]**.

## 3. Effect with `*which == 0` (the drop before moving the coordinates)

None. 1401d2b70 always returns 1, so 1401d48d0 returns before reading `*which`; no memory is
written and nothing is called except 1401d2b70 / 1400015f0, which only read **[fact]**. The
whole visible effect of 1401808d0 is the allocate-if-missing of `set+8` (n × dims[0] zeroed
doubles) and the slot-2 call 140329e60 with key 0, which changes no variable **[fact]**.
The dead dispatch would, for 0, re-read DB array 0xb (node coordinates) **[inferred]**.

## 4. Port status

| address | C++ | status |
|---|---|---|
| 14017f020 | `def::model::fun_14017f020` | ported |
| 14017ffe0 | `bc::node_set_alloc` (alias `fun_14017ffe0`) | ported earlier (kw-g2bc) |
| 1401808d0 | `def::model::fun_1401808d0` | ported |
| 1401809c0 | `def::model::fun_1401809c0` | ported |
| 1401d48d0 | `def::model::fun_1401d48d0` | ported (dead branch kept) |
| 1401d2b70 | `names_up_to_date` (step_names) | ported earlier |
| 1401d4400, 1401cf930, 1401cfc30 | `node_access/pending.cpp` | DEF_NOT_PORTED, unreachable |
