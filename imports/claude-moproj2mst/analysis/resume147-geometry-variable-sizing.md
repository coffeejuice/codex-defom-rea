# CP147: actual zero-size variable sizing is closed; stop before equal-width copy

The source-ready next cut is the actual ordered28-receiver sizing loop for **empty values and ordinary registered D[id]==0**, immediately after CP146's six BC attempts/node n1. Execute the existing real mutable lookup and each actual virtual slot2, with per-entry owner/profile checks and partial progress; then remain parked before equal-width memcpy. The zero-size native leaf is now verified directly, including its constructor/vtable origin. Positive/nonempty resizing and its failure/unwind paths stay guarded.

Only this report was written. No source/build/test execution, GUI/original launch, new REA call or mutable database access occurred. Evidence is bounded immutable PE/DUMPBIN plus ported sources. The graph lane's REA state was not accessed.

## Native registry and receiver identity

Original EXE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Node ctor `140853140` calls init `140853280`, which calls registration `140853510`. A bounded static extraction of that body found24 calls to normal ctor `140b3efd0` and4 to post ctor `140b3f060`. Every id/slot pair and its order exactly matches candidate `Cls_14017ef20::kRegs`:

```text
normal ids: 01 02 03 04 05 06 08 09 0a 0b 0c 0d 0e 0f 10
            18 19 1b 1c 1a 1d 22 23 11
slots:      010,018,...,0c8, in that same order
post ids:   1e 1f 20 21; slots 0d0 0d8 0e0 0e8
```

Normal ctor installs vtable `1427eac80`; its +10 slot at `1427eac90` points to `1409e49d0`. Post ctor first calls the normal ctor, then installs `1427eacb0`; its +10 slot at `1427eacc0` points to `140b3f0e0`. These are the actual receivers of dispatcher `140b3e8e0`, not inferred methods from similarly named classes.

| Half-open byte range | Meaning | SHA-256 |
| --- | --- | --- |
| `[140853510,1408541f5)` | Registration/order | `aecbe0879c3dece31b36f7266176ab1503e3a296e224ef46458561257f059c17` |
| `[140b3efd0,140b3f028)` | Normal ctor/vtable | `1b330c4cfca4b6e8fdb6cb15caef3107f67ffb3e29c9dcce6420e9444cb1ec16` |
| `[140b3f060,140b3f09e)` | Post ctor/vtable | `127204e16b3a3d1a0d82e92c4f071e296119175d086918878b9c18f2032eecf4` |
| `[1427eac90,1427eac98)` | Normal virtual slot2 | `6faf07550dd40affcd31cecbe01ab1f4f5e9ad757eaf714c02c15891f47f9ed2` |
| `[1427eacc0,1427eacc8)` | Post virtual slot2 | `fddb3978da2365d264c006a432d03cfbb938fc2a8e753bc3b51bc5fa86bc2312` |
| `[1409e49d0,1409e4a3c)` | Normal sizing leaf | `e217c2e3f540250a19f472a3cc73c76e99609827c94f0fe27e2ce8f01cec4b06` |
| `[140b3f0e0,140b3f145)` | Post sizing leaf | `bddde05d0b080be81b784389b4fedf92abe03bc9c5ce14c284759262c526228d` |
| `[1409e46c0,1409e494c)` | Resize/equal-count exit | `29f8e0e8332005c0155c0cd835ac67c4f2e85ef4bd9322e3f61cfd048cc0dbaa` |

## Exact leaf effects and no-allocation proof

Normal leaf key-1 passes its own-id/-1 test at `9e49f1..49fc`, calls dim0 setter `1409e4270` at `9e4a07`, reloads its actual id/D[id], calls dim1 setter `1409e42d0` at `9e4a26`, then resize(false) at `9e4a32`. Dim0 setter saves prior cur0 to prev0 before writing1; dim1 setter saves prior cur1 to prev1 before writing0. Neither changes dim2/prev2 or the data slot.

Resize multiplies actual current dimensions in low32 arithmetic and compares actual values count at `9e4722`. With n1 and D[id]0, wanted count is0 regardless of the unchanged dim2. An existing empty vector count0 takes `9e4726 -> 9e493f`, returning the initial false changed flag. No temporary/vector allocation, data copy, slot publication or cleanup branch executes. Actual count getter chain `9e4360 -> b60460 -> 86f010` only reads its body start/end and subtracts/divides by8; it does not detach or allocate.

Post leaf accepts only its actual positive id. Key-1 branches at `b3f105` directly to `b3f140` return: no dimensions, values or slot changes, and no D[id] read. Post D widths therefore need not be0. All six BC widths D[0x12..0x17] are independent of the24 ordinary registered ids, so positive BC buffers can coexist with this no-variable-allocation profile.

For positive/nonempty sizes, dimensions have already changed before resize enters its allocation branch. Prior receivers also remain changed. The port shows a temporary old-data allocation, vector resize and later slot publication; exact original exceptional cleanup is not fully qualified here. Do not enter that branch or claim rollback/failure atomicity. No real OOM test is justified.

## Smallest integrated implementation

Use only the original high callback at MainBcAttemptsCommitted, with its exact installed frame/current borrowed step/actual DB and directory identities, recorded source and primary coordinate extents, all six recorded BC attempts/pointers/counts, node n1 and untouched seventh pointer. No arbitrary pointer-length receipt or parked reacquisition is allowed. Require the actual bound registry to remain28 canonical receivers in the native order, unique registry/value bodies, empty values and null data slots. Preflight all24 ordinary D[id] widths as0 before the first sizing leaf.

Implement the bounded dispatcher loop rather than call the unrestricted whole setter and hope to validate afterward. Snapshot actual count28 as native does; for each ordinal use existing `def::model::ext::fun_14019a860(mainNodes,index)` (the real mutable/detaching lookup), recheck current frame/owner/source/destination/BC/registry/receiver/width facts after that callback-sensitive seam, then invoke actual `variable->vslot2_140301690(1, step.u15f8, -1)`. Do not manually write dimensions or replace post dispatch with a fabricated result. Record completed ordinal only after its actual leaf returns.

The initial unique registry means no lookup detach on the admitted normal path. If a lookup callback changes identity/width/sharing, preserve its actual lookup effects and all earlier completed leaves, then park before the current unqualified leaf. Do not restore earlier dimensions or restart the registry. After28 actual calls, record VariablesSizedCommitted and stop before memcpy, node Stop, frame retirement or second getter. The exact installed barrier remains active even though all ordinary dimensions and node count are now1.

This needs only Viewer, existing `test_geometry_a_producer.cpp` and a receipt. No shared header/CMake/helper is required. Existing low/null/zero-main paths and preexisting span/equal-width copy guards remain unchanged.

## Tests and prior cohort maintenance

- Use the actual CP145/146 object and both allocation producers. Set ordinary registered widths0, retain mixed positive/zero BC widths, and snapshot actual ordered receiver pointers, cur/prev dimensions, value-body identities and slot bindings. Require24 normal dim0=1/dim1=0 with correct previous values and unchanged dim2; four post receivers must be byte-equivalent in those fields. Values/body identities and all slots remain empty/null. Registry identity/order and all source/primary/BC pointers stay unchanged. One optional COW/arrays checkpoint, node timer2 and unknown scene remain.
- Observe the28 actual mutable lookups in native order during the n1 phase. Capture the actual ordinal/receiver rather than count the earlier reset's lookups. No main variable heap allocation is expected on this profile. Normal all-zero BC and mixed positive BC profiles should both work.
- An ordinary positive/negative width, nonempty/shared/misbound registry or replaced owner/span must retain BC/node-count effects but perform no sizing leaves when rejected at preflight. Do not run an enormous resize to test this guard.
- A declared late lookup callback changes the current ordinary width or receiver/owner identity after several leaves. Require completed normal dimensions to persist, the current and later leaves untouched, no allocation/copy/Stop, and a sticky frame after raw repair. An alias-induced actual lookup detach may be retained as a partial lookup effect; it must not grant leaf/owner permission.
- Repeated refresh/mutator probes cannot rerun any leaf. Preserve CP146's BC-only cohorts with an explicit next sizing guard (e.g. ordinary D[1]=1 before snapshots), or update their normal expectations deliberately. Keep the CP145 negative-BC/coordinate-only cohorts and all earlier guarded/completed paths unchanged.

The zero-size sizing cut is source-ready. Equal-width copy and any positive/nonempty variable resize remain separate source assignments.
