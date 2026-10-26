# CP102 — Deferred DOE LinkTo source review

## Result

No blocking source or test defect found in the frozen deferred `LinkTo` increment. The implementation follows the qualified native queue and resolution order within the existing concrete mode-0 header-owner boundary. This is a static review only; no build or runtime test was performed here.

## Verified behavior

- `appendBoundedHeaderVariable` queues only nonempty `LinkTo`, pairing a borrowed concrete `HeaderVariableRecord*` with the copied target name. It selects the reader's last record, including when a duplicate name was found, and queues after the optional ID/dimension setters but before Description processing.
- `readInputTable` owns one temporary queue per direct `DoeVariables` container. It calls `reindexExistingVariables` before resolving requests, and finishes resolution before advancing to later top-level siblings. A later-row refusal unwinds before the current container's queue tail; a later-sibling refusal occurs after that tail.
- Resolution scans the coherent typed owner list in encounter order and compares exact name content. It assigns the first matching target pointer. A missing target skips assignment, preserving the previous link; absent/empty `LinkTo` queues nothing. Queue state cannot leak into a later container.
- `HeaderVariableRecord` is the concrete `Study`-owned allocation. Retained native destructor evidence says it does not follow/delete `+0x40`, consistent with candidate raw nonowning `Var::link` storage. The current admission checks preserve owner/list identity and reject foreign, mixed payload, wrong-mode and unsupported numeric profiles.

The focused test covers forward/backward/self/cyclic links, duplicate-name reader-last source, repeated requests and missing-target retention, empty/absent retention, per-container lifetime, row-vs-sibling partial effects, owner/profile guards, and the actual lazy Study path followed by the existing pre-XML Save refusal. The tests preserve the public Save boundary.

## Snapshot hashes

- `src/doe/variable_record.cpp`: `0A802CBBE0B46A80F01A3955CB48C7A346F20F2D9F1519E6D41DA2E6D0A78813`
- `include/m2m/doe_variable_record.hpp`: `3D65444A19FEF6C05A32753E1D990EB673395D3ED04B9B86DBB9D20258DA45DC`
- `src/doe/doe_study.cpp`: `C6EAA2581E23D4BCE83301190667D038F4BF2045DCA35BC7BC0EC7C0250E83EB`
- `tests/doe/test_variable_deferred_link.cpp`: `C7250C0D37EE6FF4AB06F0E925DEA5C0873894D5420164B62DC4ECE4D85919BA`

The source explicitly retains its reconstruction boundary: queue allocation/QString exception timing is not Qt fault-parity, and no public Save/output/refresh behavior is claimed.
