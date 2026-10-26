# CP179 connector: private `+0x08` zero after the Qt version branch

## Decision and predecessor

The smallest next native effect after CP178's qualified taken branch is QtCore RVA **`[0x13120b,0x13120f)`**, end exclusive: `mov qword ptr [rbx+8],rdi`. On the selected public QObject constructor path, `RBX` is the retained `0x88`-byte QObjectPrivate allocation and `RDI=0`. This instruction records an eight-byte zero at **private** offset `+0x08..+0x0f`. Stop at RVA `0x13120f`, before the distinct private `+0x10` store. This does not write the task's `+0x08` private-pointer backlink, which occurs later in the public constructor after this initializer returns.

The inspected CP178 source stops at `BeforePrivateZero08` / RVA `0x13120b` after checking the qualified `0x40804` argument, caller RVAs and equal branch. `analysis/resume178-dual-build.json` records successful source-stable configure, repeated focused tests and build for source-tree SHA-256 `1c53de8b41e9c8299d3a6c6f76c09696080dc950a2ced0fe2310338ed1e2e47c`; the parent reports CP178 full-suite acceptance. This read-only pass did not rerun those gates or establish original-comparison acceptance.

## Exact PE bytes and register provenance

Shipped `QtCore4_64.dll` SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, preferred base `0x67000000`. Its PE `.text` maps RVA `0x1000` to raw offset `0x400`, so the cut begins at raw offset `0x13060b`. The four raw bytes are **`48 89 7b 08`**, SHA-256 **`0ff59178935f4e42602274d216887da3888a7055509d5c9caff6c34bf579ed7e`**.

| Preferred VA / RVA | PE bytes | Selected role |
|---|---|---|
| `0x67131178` / `0x131178` | `48 8b d9` | Retains incoming private allocation pointer in `RBX`. |
| `0x671311b5` / `0x1311b5` | `33 ff` | `xor edi,edi` clears full `RDI` to zero. |
| `0x671311f1` / `0x1311f1` | `81 fa 04 08 04 00` | CP178 compares qualified incoming `EDX=0x40804`. |
| `0x671311f7` / `0x1311f7` | `74 12` | Equal branch jumps directly to `0x6713120b`. |
| `0x6713120b` / `0x13120b` | `48 89 7b 08` | **This cut:** eight zero bytes at private `+0x08`. |
| `0x6713120f` / `0x13120f` | `48 89 7b 10` | Next private `+0x10` store; outside CP179. |

The retained initializer instructions between `RBX=private`, `RDI=0` and `0x13120b` contain no write to RBX or RDI and no call on the qualified equal branch. The non-equal fallthrough at RVA `[0x1311f9,0x13120b)` contains a call, but CP178 explicitly excludes it. The write fits within the `0x88` allocation. The separate public constructor's later RVA `0x131425` stores the returned private pointer at task `+0x08`; it must remain unavailable at this stop.

## Source-ready owner-bound cut

Add `std::optional<std::uint64_t> private08` to the same `SparseQObjectPrivate` pointee, initially disengaged. Unknown bytes must not be represented as zero before this store. Begin only from CP178's accepted `BeforePrivateZero08` phase / RVA `0x13120b`, which certifies the qualified public caller's `0x40804` argument and equal branch to this target.

Before mutation, require the retained pending Report ordinal and exact allocated private pointee; native task/private sizes `0xc0`/`0x88`; written task/private address points `0x23cac8`/`0x1a9d10`; this exact phase and qualified version provenance (QtCore digest, argument `0x40804`, load RVA `0x13140f`, call RVA `0x131417`); all six earlier private zero observations at `+0x40/+0x48/+0x50/+0x58/+0x78/+0x80`; and `private08` still disengaged. Require the factory's live root to match its allocation store by control block and pointee. The first/second QList bindings must match the qualified root QList record at RVA `0x2f0a50` with both committed markers; QString at `0x2f0b30` and QVector at `0x2f0bb8` must match their root records and committed markers. QObject/base remains incomplete and task/private backlinks unavailable. Refuse malformed owner, identity, phase, version provenance, predecessor observation, binding or marker before mutation; do not infer absolute shared-null counts.

On admission, set only `private08 = std::uint64_t{0}` and advance once to a phase such as `BeforePrivateZero10` / RVA `0x13120f`. Preserve all other sparse fields, allocated-private and root identities, bindings/markers, three symbolic reference deltas and observations, task/prototype/cache counts and maps, and completion/backlink flags. No host register emulator is needed for this qualified normal path: the retained bytes close `RBX` and `RDI` through this instruction. Do not bundle the adjacent `+0x10` store or claim full QObject construction.

## Short focused acceptance

1. From accepted CP178 state, snapshot owner/private/root identities, six predecessor zeros, `private08` unknown, four bindings/markers, three root deltas, maps/counts and completion/backlink flags. One continuation changes only `private08` to zero and phase to `BeforePrivateZero10` / `0x13120f`; private `+0x10` remains unknown. Include an actual `ConversionSession` pairing.
2. Same-key and different-key reentry at the new stop each refuse without replaying the store, changing reference counts or maps, or completing QObject.
3. Refusal cases: missing, expired, foreign or aliasing-foreign root; wrong/expired QList, QString or QVector binding or marker; wrong module/RVA version provenance or argument; bad task/private identity, size or address point; wrong phase; missing/nonzero earlier zero; already observed `private08` (zero or nonzero); premature backlink or completion. Each refusal leaves the full retained frame and root counters unchanged.
4. Run focused `doe_task_factory` and root-owner tests, followed by the established source-stable checkpoint and original-comparison gates. This note ran no test, build, original process, GUI or live REA operation.

## Qualifications and SHA-256 receipts

The retained QtCore dump is bounded by an exception-directory unwind region, so this is a static proof for the selected non-null private-allocation, equal-version path, not a claim about all callers, a live occurrence, cleanup or a completed logical QObject constructor. Preferred VAs are display coordinates; use the module digest and RVAs for source qualification. Source hashes identify the inspected CP178 snapshot, not a new candidate freeze. Paths below are relative to this converter checkout.

| File | SHA-256 |
|---|---|
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `analysis/resume178-connector-next.md` | `e1ef3c91a288c89a45dcca54c9e5164464fdba0566f546ec82a9c36841d834d5` |
| `analysis/resume178-connector-implementation.md` | `10899a404e2d2a261df5bf80368ce9ed38e264336ea7154fbd7ab61081c80310` |
| `analysis/resume178-connector-source-review.md` | `6619c5a7632afa0316fa9025841aca0df47288bf0736f196859b298d771676bf` |
| `analysis/resume178-dual-build.json` | `0ebe8d3fe6133e78618bba0eda11202a7fe5d2543821ac882b6338f7d221ab3a` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `d125ec4c0c45f6d3ec7da7ae31f97971f27b9b335762020f37aa58a3574945aa` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `4ffa7e3e5b4f147cd4abb1643c4f8ab0f9e89c0d65b1da3d63314cf00b6204dc` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `91919dc54f34e6d1566eb0703004444b2fcb29324d53e027772a2c9e9559fc0f` |

This pass changed only this report. It made no source, tests, shared plan/state, build, deployment, GUI-control, original-process or live REA change. No GUI-control session was acquired.
