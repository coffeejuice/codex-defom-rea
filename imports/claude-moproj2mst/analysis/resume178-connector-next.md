# CP178 connector: Qt private-initializer version decision

## Decision

The next bounded cut on the selected, non-null Report QObject constructor path is QtCore RVA **`[0x1311f1,0x1311f9)`**, end exclusive. `cmp edx,0x40804` followed by `je +0x12` takes the equal branch to **RVA `0x13120b`**. The preceding public QObject constructor explicitly passes `EDX=0x40804` to this private initializer, and the retained initializer region contains no write to EDX or intervening call before this compare. This is a source-ready branch decision for that qualified caller path. Stop at `0x13120b`, before the first private `+0x08` store. It does not establish QObject completion or any subsequent backlink, thread, counter, TaskInfo, task or cache effect.

The inspected CP177 source ends at `BeforeQtVersionCheck` / `0x1311f1`, with no retained version argument or branch result. The parent reports CP177 source-stable focused and full suite acceptance; `analysis/resume177-dual-build.json` records successful configure, repeated focused tests and build steps for source-tree SHA-256 `2f0494e74879e0055c37fcbaac03771a974018fe55b3cf5a3ff71c840e624487`. This static pass did not run those gates or an original comparison.

## Exact bytes and EDX provenance

Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; preferred image base is `0x67000000`. PE `.text` RVA `0x1000` maps to raw offset `0x400`, so raw offset is RVA minus `0xc00` for these instructions.

| Preferred VA / RVA | PE bytes | Meaning on selected path |
|---|---|---|
| `0x6713140f` / `0x13140f` | `ba 04 08 04 00` | Public QObject constructor sets `EDX=0x40804` after successful `0x88`-byte private allocation. |
| `0x67131414` / `0x131414` | `48 8b c8` | Moves allocated private pointer to `RCX`; EDX stays unchanged. |
| `0x67131417` / `0x131417` | `e8 44 fd ff ff` | Calls private initializer at RVA `0x131160`. |
| `0x671311f1` / `0x1311f1` | `81 fa 04 08 04 00` | Compares incoming EDX with `0x40804`; equality sets ZF. |
| `0x671311f7` / `0x1311f7` | `74 12` | `je` takes the branch to preferred VA `0x6713120b` / RVA `0x13120b`. |
| `0x671311f9..0x6713120a` / `0x1311f9..0x13120a` | `41 b8 04 08 04 00 48 8d 0d 3a be 10 00 e8 b5 bf ee ff` | Mismatch fallthrough, skipped by the selected equal branch. Do not implement or describe its call's effects from this cut. |
| `0x6713120b` / `0x13120b` | `48 89 7b 08` | Next private `+0x08` store; outside CP178. |

The selected eight-byte cut is `81fa040804007412` at PE raw offset `0x1305f1`, SHA-256 `0255c14fa893d0c8f2b08512b47e3d4672cde7559f99731ab3f7f983cfad95aa`. The thirteen caller bytes at raw `0x13080f` are `ba04080400488bc8e844fdffff`, SHA-256 `403861a57d687e136473a558d18ad834f70c04b368f904f3107d19f3a117579a`. The skipped mismatch sequence has SHA-256 `75c3134a6aaad6da6a2364f2b94afb68816cacaf6c35c7260ddeefe4481346d6` over RVA `[0x1311f9,0x13120b)`. These are PE-byte checks, not runtime observations.

The retained private-initializer disassembly from RVA `0x131160` through the comparison uses `RBX`, `RDI`, `RAX` and `RCX` and memory, with no EDX write or call. EDX is volatile across calls generally; the absence of a call in this specific interval closes the value chain. The public constructor's allocation-null branch does not enter this initializer. This establishes the selected path's equality only. Other callers and a mismatch path require separate qualification.

## Owner-bound source contract

Start from the accepted CP177 pending Report/private frame at `BeforeQtVersionCheck` / `0x1311f1`. Retain the constructor argument as a **qualified** `0x40804` value with provenance to public QObject RVA `0x13140f` and its call RVA `0x131417`, scoped to this QtCore digest and successful `0x88`-byte allocation. A small optional value plus a fixed caller-RVA qualifier on `QObjectPrefix` is sufficient; no general register emulator is needed. Do not derive the value from the comparison constant alone or silently assume it for a foreign caller.

Before changing phase, require the same pending Report ordinal and allocated private pointee; `0xc0` task and `0x88` private allocation sizes; task/private address points and written flags; the exact CP177 phase; qualified caller/version evidence; `private40`, `private48`, `private50`, `private58`, `private78`, `private80` all observed as zero; first and second QList bindings to the same root record with their committed markers; QString and QVector bindings to the qualified root records with committed markers; root identity by pointee and control block; and no QObject completion or backlink. Preserve the existing accepted symbolic reference deltas and observations. Reject malformed identity, provenance, phase or predecessor state before mutation.

On admission, record that the equality branch at RVA `0x1311f7` was taken, and advance once to a phase such as `BeforePrivateZero08` at RVA `0x13120b`. Phase alone can encode the decision if its name and qualified argument remain unambiguous. Do not mutate any private field or global reference count at this boundary. The skipped compatibility sequence is absent on this selected path, but no claim is made about its semantics, other callers or runtime occurrence.

## Short focused acceptance

1. From accepted CP177 state, capture the pending/private/root identities, six zero observations, four bindings and markers, three symbolic root deltas, maps/counts and completion/backlink flags. One continuation with the qualified caller argument `0x40804` must advance to `0x13120b` and record the taken equality branch. All captured data remains unchanged; private `+0x08` remains unknown.
2. Same-key and different-key reentry at the new stop must refuse without replaying the decision, changing state, or completing QObject. An actual `ConversionSession` owner pairing should pass the positive case.
3. Missing/foreign caller provenance, wrong argument such as `0x40803`, wrong module digest, bad owner/root/binding/marker, wrong sizes or address points, missing/nonzero predecessor zero, wrong phase, or premature backlink/completion must each refuse before mutation. The wrong-version test must **not** execute the unmodeled mismatch fallthrough.
4. Run focused `doe_task_factory` and root-owner tests, then the established source-stable checkpoint and original-comparison gates. This note ran no tests, build, original process, GUI or live REA operation.

## Source qualifications and SHA-256 receipts

The disassemblies are exception-directory unwind regions, not proven complete logical functions. Preferred VAs are display coordinates; the digest and RVAs are the portable qualification. The proof is static for the selected non-null private-allocation path and cannot assert a live occurrence, cleanup behavior, or generic Qt caller behavior. Source hashes below identify the inspected CP177 snapshot, not a CP178 freeze. Paths are relative to this converter checkout.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `analysis/resume177-connector-next.md` | `e8ac9bd94385960a97ae28d9554c2dd59a783333b4827174c3d88212be13ea42` |
| `analysis/resume177-dual-build.json` | `7659fed35d5927e25c0f5580f3fd9d3e2c1fe7e7fcd2dd8883580663fac39464` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `6b5a4c207fc96d863a6463de06c536bda7cef9c934f304f3cc5ae178545d6bcb` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `71e8cb455a5e808144e19e1b1f2ced12783080722d9fcab93b33e8bdd2c21b90` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `df030fe660d7ff8f5268e47201118795c43fe8695b7c201c8c2d8be32b8862af` |

This pass changed only this note. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
