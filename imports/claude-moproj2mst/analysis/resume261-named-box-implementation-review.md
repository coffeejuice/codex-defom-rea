# CP261 named-holder scene-box implementation independent review

## Verdict: PASS

The source and test review remains sound. `sceneBox` visits canonical named holders after main, secondary and third; it checks each flag before obtaining the owned model, skips null holders, and queries cached boxes through `viewerSceneBox`/`boxOf`/`boxUnion`. The loop does not mutate or retain holder collections, refresh named models, deduplicate aliases, add geometry admission, or alter radius/camera logic.

The caller test checks sorted key order after out-of-order creation, hidden and shown named box contributions, multiple-box window bounds, stale-cache use after direct geometry mutation, false-flag and shown null holders, invalid boxes preserving main, and the all-hidden case. These are literal window assertions; the report does not claim radius, origin, refresh, or whole-fit parity. Ownership and pointer lifetimes are appropriate for the tested canonical holders.

## Validation review

The implementation report records a successful focused `m2m_mdt_viewer_box` configure/build and CTest 1/1. The canonical build receipt has both configure and build exit codes 0, targets only `m2m_mdt_viewer_box`, and matching before/after source-tree hashes with `source_stable_during_build: true`. The focused CTest log confirms test 244 passed. Current source/test hashes match `cp261-named-box-hashes.json`.

The report transparently qualifies the initial Tee receipt-lock issue: the first wrapper could not write its receipt because Tee held that same path open, while configure/build returned 0; the initial console JSONL is not represented as a completed receipt. A subsequent recovery invocation produced the canonical successful wrapper receipt with a no-op build against the same unchanged source tree. The validation evidence supports the successful focused result without hiding the receipt-write problem. No full suite or freeze is claimed.

Original behavior remains qualified to retained static evidence and CP260's reviewed provenance. No fresh original execution, natural named occurrence, named refresh parity, independent radius/store gate, or full camera parity is claimed.
