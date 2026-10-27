# CP447 CopyMirror matrix chain and enabled-builder prefix

This investigation closes the eight requested static matrix/helper contracts and identifies a bounded enabled-builder prefix. It does not close the gate-true lifecycle: the original builder passes two locally uninitialized triples into the allocation helper's bounds calculation. No original application, GUI, C++ edit, executable build or test was run.

## Evidence and attribution

The imported `DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. After CP446 freeze release, Ghidra 12.1.4 processed a new disposable `scratch/cp447-ghidra/project/guimo` copied from the CP445 disposable project, using `-process DEF_GUI_MO_64.exe -noanalysis -readOnly`. The CP445 and original retained projects were not opened by these runs. Logs under `scratch/cp447-ghidra/` record read-only processing and discarded changes. The underlying historical filesystem snapshots were not acquired as transaction-consistent Ghidra backups; this qualification remains attached.

| Export | SHA-256 |
| --- | --- |
| `analysis/cp447-mirror-matrix-helpers.c` | `fc83a6bcd26c1b5f4e5dd9d183b6b9f891db209187ae76f1b348f0c9eed30739` |
| `analysis/cp447-mirror-vector-dependencies.c` | `6b8351832abbd614d8ed3eb20ef126e37f850f0169d3b4b0a9291e8837299a33` |
| `analysis/cp447-mirror-vector-length.c` | `84a9ff2d7088353ea837accc2cc3a21a97fa9d6ac7d7318cc623e8bdceac8245` |
| `analysis/cp447-mirror-builder-range.asm` | `0b3d3fee577a97a67058b584667ac5f1049bc6b6d36eeb56666b2e6e81001e3a` |

`analysis/cp447-byte-verification.json` binds all 160 assembly instructions in `[140e23a00,140e23d80)` byte-for-byte to that imported PE and records scalar constant bytes. The first `FuncAsm.java` output, `cp447-mirror-builder.asm`, contained only the function header because the retained function body had no decoded instructions; it is superseded by `AsmRange.java` output. DecompList exported exactly the eight requested helpers; follow-ups exported dot product, normalization, and vector length. Normalization's final `141471bbc` dependency was not decompiled, so its exceptional floating-point behavior is unresolved.

These are static contracts and instruction bindings. They do not establish a reachable fresh original Open occurrence or original runtime output parity.

## Matrix convention and helper contracts

Use flat storage `M[4*r+c]` at byte offset `0x20*r+8*c`. `140945d20` writes the identity; `140945e70(dst,src)` copies all 16 doubles. `140944d10(a,b,out)` adds triples; `140944f20(a,b,out)` subtracts triples. `141247e90(record)` reads the mode byte at `+0xf8`, not the active byte.

`1409464c0(M,p)` first copies `(p.x,p.y,p.z,1)` to a temporary, computes `q[c] = sum(k=0..3, M[4*k+c]*p4[k])`, then stores only q[0..2] back to p. This is a homogeneous row-vector point transform; translation is at indices 12,13,14. It does not divide by q[3].

`140946040(A,B)` computes `T[4*r+c] = sum(k=0..3, A[4*k+c]*B[4*r+k])`, then copies T into A. In this storage convention the result is `B*A`, not `A*B`. Therefore the builder's copy-parent, multiply-by-reflection, copy-back sequence produces `R*P`, and a row vector is transformed as `p*R*P`. Preserve the explicit four-term accumulation order.

For `n=param_4`, `p=param_5`, `d=n.x*p.x+n.y*p.y+n.z*p.z`, `140946ca0` writes:

```
R = [ 1-2*nx*nx   -2*nx*ny     -2*nx*nz     0
      -2*nx*ny     1-2*ny*ny   -2*ny*nz     0
      -2*nx*nz    -2*ny*nz     1-2*nz*nz   0
       2*d*nx      2*d*ny       2*d*nz      1 ]
```

The labels normal/point are geometric interpretations of the formula; the concrete ABI operands are the two triples. The PE constants are `1421249e0=1.0`, `142101520=2.0`, `142680a30=-2.0`, `1421ad1d0=0.0`, and `1421d3d30=0x8000000000000000` (the sign bit, represented as -0.0). Do not move normalization before matrix construction: this helper directly uses the supplied n without checking or normalizing its length. The separate, later `141247ba0` normalizes its first triple in place, stores it at +c0..+d0, and XORs the dot-product bits with the sign bit for +d8. Arithmetic negation and sign-bit XOR are not interchangeable evidence for NaN payloads or signed zero.

## Exact enabled-builder order and first unsupported boundary

1. Resolve actual View Settings (`1411f1a10`). Require flag 1 and either flag 2 or flag 3. A false gate returns before allocation. Missing Settings are not evidence for a false gate.
2. Address the View owner at +0x348 and call `141249420` (the result is unused by the exported body). Initialize a local matrix to identity, then overwrite it with `140946ca0(n,p)`.
3. If the input parent index is zero, choose mode zero. Otherwise directly address `owner+(parent-1)*0x108`; the original does not validate this index against the allocated extent. Compute `t=p+n`, transform t through the parent's +0x40 double matrix, transform p in place through that matrix, and overwrite n with `t-p`. Choose mode `parent.mode==0`. Copy the parent matrix, call `140946040(temp,R)`, then copy temp back into R. An unknown/uninitialized parent matrix is an earlier unsupported boundary for this branch; do not synthesize identity or sanitize the native parent index without marking the boundary.
4. Enter `141249730(owner,R,mode,boundsA,boundsB)`. The mutation order in this helper is established by the separately retained `analysis/cp445-mirror-plane-producers.c:1–78` (SHA-256 `a6e2062dfe53102ab015758d83b36d8010e3cc8f5b21d0e55f059b61b9c6465d`) and `analysis/cp445-mirror-plane-dependencies.c:1–29` (SHA-256 `597320168d9b00297c4885f76127ef76a572aa5a78a6e835f637ce14398d47c8`), not by the builder assembly alone. At extent exactly 128 it returns zero before inactive-slot search, even if an inactive slot exists. Otherwise `141249680` selects the first inactive slot below extent or increments the extent and selects the old extent. The successful in-range slot receives all 16 doubles at +0x40 and their 16 individual float conversions at +0x00, then mode at +0xf8. `141247ed0(record,1)` sets active at +0xf9; enabling does not clear existing links.
5. **First unresolved consumption for a known matrix/valid selected slot:** `141249e40` reads boundsA. Builder instructions `140e23cc8..140e23cf3` pass `RSP+0x170` as argument four and `RSP+0xc8` as argument five. Neither triple has a local store or an address passed to an earlier call in this function. The two `14121c480` objects are at RSP+0x188 and +0x1b8, distinct from these triples; retained `V/v1.c` and `V/v2.c` show their constructor writes within those objects. The triples must remain unknown. Do not substitute zeroes, current object extents, parsed input points, or presumed bounding boxes.
6. Beyond that unresolved point the native helper transforms both bounds, orders transformed endpoints, extends them with the original bounds, and initializes/extends owner bounds at +0x8438/+0x8450. It then returns slot+1. The builder looks up that returned slot, applies `141247ba0(n,p)`, and joins parent/new slot through `141249f80`. These operations are after the boundary and are not authorized by the proposed prefix alone.

The caller `140e23490` (`evidence/gui_mo/s1/mdt/L5/d1.c:959` onward) passes the parsed first integer as parent index, with parsed floats 3..5 converted to n and floats 0..2 to p. Only after the native `void` builder returns does it write +0x104 and the six original parsed coordinate values into fixed row-index records, then enable each record. A second pass applies parsed active flags. **The native caller would continue if the builder returned normally**; no original refusal at this point is established. A reconstructed guard must propagate an explicit unsupported result across that caller boundary after retaining the proven helper prefix mutations. Such a guarded reconstruction does not perform the caller's fixed-row writes, later rows, or final flags, and cannot be claimed as the original's completed post-load state.

## Smallest implementation leaf and ownership

The useful source-closed leaf is the actual View's enabled-builder prefix for parent index zero, or for a separately proven initialized parent matrix: reproduce reflection construction, native slot selection, matrix double/float stores, mode and active=1, then explicitly refuse immediately before the first unknown bounds read. This advances proven native state while preserving the gate-true/public admission guard and the unsupported result. It requires no normalization claim because `141247ba0` lies after the boundary. An entirely pure matrix helper leaf is also source-closed, but by itself does not advance View-owned lifecycle state.

Assign one source owner exclusively to `source/apps/moproj2mst/src/engine/viewer.cpp`, `source/apps/moproj2mst/src/engine/mirror_plane_owner.hpp`, and the focused mirror-owner regression file chosen by root. Constructor-unknown matrix/bounds fields should have explicit unknown representations in snapshots; do not zero-initialize native-unknown fields. Keep operation registry/public admission untouched. If a new helper header or test target is introduced, resolve includes/Testkit macros and give its CMake file to that same owner or root.

Meaningful checks for that prefix include: gate false unchanged; parent-zero reflection and float conversion from a non-unit n; reused inactive slot without extent growth; new slot extent growth; extent 128 refusing allocation even with an inactive slot; mode=0; enabled records retaining links; prefix allocation/matrix/active writes retained on the unknown-bounds refusal; and caller fixed-row writes/final flags not occurring after that refusal. Parent composition should be added only with a proven initialized parent-matrix seam and a noncommuting transform example that distinguishes R*P from P*R. These tests establish the implemented static prefix, not original runtime parity.

### Minimal CP446 implementation proposal

Limit the first landed increment to gate true with parsed parent index zero. Keep nonzero parents guarded until a parent record's matrix is known; the CP446 constructor intentionally does not initialize any native matrix. In `importMirrorEntry`, perform the existing actual Settings reads first, construct the local identity/reflection from converted `plane.v[3..5]` and `plane.v[0..2]`, then enter the native allocation prefix. At extent 128, stop before mutation at the still-unimplemented capacity-return/linking branch; do not report successful native completion. Otherwise scan only records `[0, allocatedSlotCount)` for the first inactive slot. If none is found, select the old extent and increment it. This selected slot, not the input row index, is the first affected record.

Add optional `array<double,16>` and optional `array<float,16>` matrix observations to `MirrorPlaneRecord` and its snapshot. A fresh constructor leaves both absent. On the selected record, assign all double elements first (+40..+b8), then convert and assign all float elements (+00..+3c), then set mode zero, then active true without clearing links. Stop with explicit unsupported status before transforming bounds. Leave owner bounds unknown; leave c0/e0/a and all links unchanged. The high-water extent and matrix/mode/active writes survive this stop. No fixed row-index caller write or second-pass flag application runs after it. Retain the existing source-qualified guard for positive serialization and public operation admission.

The two triples are not incoming stack parameters. If entry stack pointer is S, the prologue makes RSP=S-0x228. BoundsA occupies `[S-0xb8,S-0xa1]` and boundsB `[S-0x160,S-0x149]`; both are inside this new frame. Saved/incoming argument slots are at post-prologue RSP+0x230 onward. Their physical bytes can inherit earlier stack contents, but they are not an address alias for the parsed arrays in the actual caller's own frame. The caller passes its named normal/point locals; no observed argument points into these future lower-frame triples. No write to either range appears before their use, and none of the earlier explicit callee destinations overlaps them. This establishes an unknown-input boundary, not a deterministic zero/box input.

Keep one owner on `viewer.cpp` plus `mirror_plane_owner.hpp`. Extend `test_l5_loader.cpp` or `test_copymirror_property_writer_prefix.cpp` only under that same ownership, or use a new focused test file with root-owned CMake integration. Tests can exercise append by repeated gate-true parent-zero calls that stop on bounds, and reuse by interposing an existing gate-false import that disables the selected record. Confirm the returned unsupported status as well as retained actual owner identity, extent, matrices, mode, active and unchanged c0/e0/a/links. A non-unit n=(2,0,0), p=(3,0,0) gives R[0]=-7 and R[12]=24; this distinguishes the native unnormalized matrix from a normalized reflection. Use nontrivial finite values to check individual float conversions. Test false gate and nonzero unknown-parent refusal retain their established order, and capacity remains an explicit unsupported branch with no new allocation.

This prefix is safe to claim only as a static reconstruction prefix terminated by the port's existing unsupported mechanism. Native code does not throw at the unknown-bounds point. The port's stop exposes a defined observation boundary before the native nondeterministic read; it does not reproduce subsequent native failure/output behavior or establish a successful native conversion.
