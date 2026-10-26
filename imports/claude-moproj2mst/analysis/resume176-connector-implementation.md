# CP176 connector implementation receipt

Implemented the bounded normal non-null constructor cut QtCore RVA `[0x1311e6,0x1311ea)`: one retained QObjectPrivate `+0x78` observation becomes `std::uint64_t{0}`, then the cursor stops at `BeforePrivateZero80` / `0x1311ea`. The new optional is disengaged on fresh allocation. No private `+0x80` value is represented or inferred.

The continuation validates the retained Report ordinal, allocation-time private pointee, sizes/address points and written flags, exact phase, live root ownership with control-block and pointee identity, all four container bindings and committed markers, and four earlier observed zero fields. It refuses an already observed `private78`, incomplete predecessor, replaced private allocation, malformed binding/owner, or premature completion/backlink before mutating the field or cursor. Both QList bindings must resolve to the same qualified root. The QVector immutable RVA is checked directly; its optional qualification remains an independent observation, accepting absent and refusing a conflicting value as earlier cuts do. No absolute count is required.

Only `private78` and the phase change on admission. There is no reference increment, new binding, allocation, map/cache update, backlink or completion. Existing tests were migrated to the new final phase/RVA and sparse reconstructed predecessors now clear the later `private78` observation. Added CP176 cases cover fresh unknown/type, a 49-case no-mutation refusal matrix, four unpaired/aliasing root owners at the retained predecessor, and actual ConversionSession ownership with both sparse and supplied observed counter history, single continuation and same/different-key reentry. Refusal snapshots preserve prior fields, markers, bindings/control blocks, private identity, roots/deltas/observations, allocation sizes/address points, completion/backlinks, counts and maps; the CP176 matrix adds preservation of `private78`.

## Source and PE qualification

Read `analysis/resume176-connector-next.md`, bounded retained initializer disassembly, and the static capture qualification. Verified selected original converter SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Parsed the local PE section mapping: `.text` RVA `0x1000`, raw `0x400`; target raw `0x1305e6` contains `48 89 7b 78`, SHA-256 `89e81f6e18a602e8dadf2420153a73d497e90b06d5f96b5c12209fc6ea2be71d`. Retained disassembly establishes earlier `RBX=private` at RVA `0x131178` and full `RDI=0` at `0x1311b5`; the selected store stops before the separate `+0x80` instruction at `0x1311ea`. This is static normal-path evidence from an unwind-region capture; it does not prove all exceptional paths, live occurrence, cleanup or QObject completion.

Static text checks confirmed migrated prior test stop strings contain no `RVA 0x1311e6`, only four deliberate CP176 predecessor `BeforePrivateZero78` uses remain, the new field has `optional<uint64_t>` compile-time assertion, and the new refusal matrix enumerates 49 cases. These are static checks, not compilation or a test pass.

## SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume176-connector-next.md` | `48b3b73a64383c9d1e97f9791c55249116b02264a7117acb969d9eb086a16721` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-static.json` | `9a5a9ae6622a56330ccbbe6dd949d2dc02753d55d7ed59cd5559d834f80b760e` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `285cad32db28596de87cf453f45af35e17ddb0edb1593c486518842947870622` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `576c994815adabc6932ddfaefd465d7e322b9c94a92d777a61f790295e0b37fa` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `576c6012b4180de84c6bc40bd3a0f21ba451d63916845fa29fb302cd4d828bf0` |

## Limits and release

No build, focused/full test execution, candidate freeze, deployment, GUI, original process or live/mutable REA operation was performed. No shared recovery plans/state were edited. Build/regression/original-comparison gates remain with the parent. No GUI-control session was acquired. The three exclusively owned source/test files and this receipt are released to the parent for review and further work.
