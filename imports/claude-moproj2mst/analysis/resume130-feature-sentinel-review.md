# CP130 independent review: isolated feature sentinel

## Result

No actionable mismatch found in the reviewed sentinel slice. The frozen implementation follows the retained source order for the 48-byte allocation and the map-constructor stores, and its reports preserve the boundary at the isolated `M` return. This is a static review; it does not establish runtime execution or original-process parity.

## Source and artifact binding

The reviewed source files match the CP130 release hashes:

| File | SHA-256 |
| --- | --- |
| `src/model/feature_crt_runtime.hpp` | `053ee825f92374cd7a61b88d83381b2ebefb3fcc2bdcb5aca657c998b8b72903` |
| `src/model/feature_crt_runtime.cpp` | `cc9c9a72ca0302148615ba51f384a51a291f40bedabda218b6a51dfe9f43e3d7` |
| `tests/model/test_feature_crt_runtime.cpp` | `c26ca7daa2e91770dcc6526e4bb0be885b5c9f55ebbceca08f6c96a971f441ac` |
| `analysis/resume129-feature-map-next.md` | `6dbaf98431d7a4c5e81549b149dc31dfeaf5d3121872ea805368af9caf1a8ac9` |
| `analysis/resume130-feature-sentinel.md` | `42a047df675841bf0734a5e7b2feeb53e14a50cfb4b397318f9667319e3e4aa7` |

The native source qualification binds original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. It identifies the helper stores at `12edae3`, `12edaf3`, `12edaff`, `12edb02`, `12edb06`, followed by outer publication at `12f1097`, nil update at `12f109b`, three reload/self-link pairs in `+8,+0,+10` order, count store at `12f10b6`, and return `M` at `12f10be/12f10c6`.

## Review findings

- `SentinelHeader` is compile-time constrained to 48 bytes with link offsets `+0/+8/+0x10`, byte offsets `+0x28/+0x29`, and an explicit empty placement constructor. Only fields reached by qualified stores are read. The opaque `+0x18..+0x27` payload and `+0x2a..+0x2f` suffix remain untouched and unknown.
- `resumeSentinel()` rereads the candidate heap slot and issues `HeapAlloc(heap, 0, 48)`. A null heap stops before allocation; a null allocation records the consumed attempt and parks at `SentinelAllocationFailurePending` before the qualified new-mode read. That branch does not publish head/count, replay, roll back the proxy, claim a native throw, or free memory.
- On success, the implementation records the five helper stores in source order, helper return, `M.head` publication, nil transition, each actual `M.head` reload and self-link in `+8,+0,+10` order, count zero, then isolated constructor return. The 20-entry fixed ledger matches that successful sequence. The tests check the ledger and resulting known fields, preserve the prior 8-byte proxy report, and check repeated calls do not replay the operations.
- `Storage::headAllocation` retains the successful block even before publication. The runtime’s retained call owns `M`, `A`, and `H`; destructors are host-metadata-only and add no `HeapFree`, `HeapDestroy`, or automatic native cleanup.
- The dynamic `candidateMapConstructorReturned` flag is distinct from the static `nativeProjectContextConstructed`, `projectBindingAdmitted`, unwind/cleanup, and public-close flags, which remain false. The report stops at the enclosing feature context boundary.

The declared sentinel-allocation failure projection is correctly separate from the actual frame. Its tests cannot be mistaken for evidence of a kernel allocation failure. The actual positive path is also clearly a candidate-process allocation, not an original runtime capture.

## Limits

The owner reports no build or test execution for CP130; this review also ran no build, tests, GUI, original target, or mutable REA action. The nine test cases and their Release-active checks were inspected statically. No public feature admission or Close qualification follows from this isolated map-constructor slice.
