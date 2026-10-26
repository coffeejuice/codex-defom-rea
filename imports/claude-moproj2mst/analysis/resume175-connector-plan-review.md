# CP175 connector independent plan review

## Decision

**Source-ready from the accepted frozen CP174 predecessor; no blocking plan defect found.** The proposed cut `[0x1311de,0x1311e6)` is exactly one second QList atomic increment. The current factory/header/test digests match the released CP174 receipts below. The inspected build receipt records stable source-tree digest `23ae8e2b14258004bf5229bf8e83d940dd56b6c526358f004e2368ddba9ea1ec`; the parent supplies full-suite/freeze acceptance. This review does not independently establish an original-comparison result or rerun acceptance gates.

## Independent byte and symbol check

Directly read shipped `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Raw offset `0x1305de` contains `f083056af81b0001`, eight bytes with SHA-256 `42761f1f0be20d86fa8ecaaa4a6cfad56f59f6c3f58031e1535c02381ab7fcb9`. The instruction is `lock add dword ptr [rip+0x1bf86a],1`: next RIP RVA `0x1311e6` plus displacement gives RVA `0x2f0a50`. Direct PE export-table parsing identifies that address as `?shared_null@QListData@@2UData@1@A`, ordinal 4511. The next four raw bytes are `48897b78`, the excluded private `+0x78` store.

The source CP174 store assigns `qualified->list` to the second weak binding after first-list identity qualification. It advances to `BeforeSecondQListIncrement` without incrementing or setting the second marker. Thus CP175 must consume the same root QList object, not manufacture another state record. Static bytes identify the selected instruction/global and do not establish a live absolute count, release behavior, exceptional paths or full QObject completion.

## Required admission and effect

Retain the existing exact store/root owner pairing and retained Report/private identity checks. Admission must be a dedicated exact `BeforeSecondQListIncrement` branch. Reuse the retained-prefix qualifier for first QList/QString, sizes, address points, completion/backlinks; explicitly validate the committed root-qualified QVector and all four zero observations. Both first and second QList weak bindings must lock to the canonical root list with pointee **and control-block** identity; matching raw pointers alone admits wrong ownership. Empty and expired second bindings both refuse at this cut. Require the first QList, QString and QVector markers true and the second QList marker false before any mutation.

After all checks, `WidgetRuntimeState::commitQListSharedNullReference()` is an existing `noexcept` unsigned increment, suitable for one modulo-2^32 event. Set the second marker true and advance to the newly added `BeforePrivateZero78`; stop at `0x1311e6`. Do not observe or initialize private `+0x78`. Both bindings, all other root deltas and observation baselines/qualification, zeros, counts/maps, identities and completion/backlinks stay fixed. A supplied projection changes through the delta; its baseline itself must not be rewritten.

One wording clarification for implementation: acceptance item 3's “false first/second predecessor marker state” means **false first marker or prematurely true second marker**. A false second marker is the valid CP174 predecessor, not a refusal condition. The detailed owner-bound contract already states the correct polarity.

## Focused validation hazards

Start isolated CP175 positives by rewinding a completed new-stop fixture to `BeforeSecondQListIncrement`, resetting only the second marker to false and retaining both same-root QList bindings. Earlier CP174-binding rewind fixtures must instead clear the second binding and marker; earlier increment fixtures must reset their own markers as well. Leaving a committed second marker after rewinding would test fixture corruption rather than the intended earlier transition.

Migrate ordinary constructor stop expectations to `0x1311e6`, but preserve intentional refusal expectations at earlier cuts. Existing CP174 tests currently expect the second marker false after reaching their terminal stop; once CP175 follows through, those expectations need precise adjustment or deliberate CP174 prebinding reconstruction. Existing CP173 tests must allow the later QList increment in total-history assertions while still proving exactly one QVector increment.

Cover second-binding empty/expired/foreign/aliasing/wrong-control-block faults explicitly, alongside first binding/root/QString/QVector and malformed reachable QVector qualification, zeros, sizes/address points/identity, phase, markers and completion/backlinks. Snapshot before refusal and check all three deltas, both QList weak ownership identities, observations, cursor, fields, counts/maps and completion. In a replaced-private test, snapshot through the replacement and never use a reference to the destroyed original.

Use independent nonzero-history and `0xffffffff -> 0` cases, with and without a supplied qualified QList observation. Check exactly one event, projection wrap where known, unchanged observation metadata and unknown projection when absent. Actual `ConversionSession` pairing plus same-key and different-key reentry must prove no replay and no new allocation/cache/completion. The current QList module/RVA fields are compile-time constants; there is no optional mutable QList qualifier to fabricate for a negative test.

## SHA-256 receipts

| File | SHA-256 |
|---|---|
| `analysis/resume175-connector-next.md` | `8dbf332bc5b03808aea1f3670b9e797a72a2fd99e254361b35bb3304c083df51` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `7703139852f2eec98d7da3485ffa0ea2df4b27047876ea3d56c9a9697110a7bb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d9cf855f0c06db319657b682b3442def787ffbedbaf39caf9354892355994a0` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `e21a97c15fefc135b0e498272fd4b3e9beb8c476d019bd33943e97552eff1cc0` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |

Only this report was written. No source/test edit, build, executed tests, GUI control, live REA, deployment or shared recovery-state change occurred. Focused, source-stable regression and original-comparison gates remain implementation acceptance work.
