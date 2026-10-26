# CP164: smallest source-ready geometry dependency cut

## Recommendation

Take the bounded 3D geometry-list mesh rectangle dependency at `0x14126d0c0` → `0x1408ee940` → `0x140514f00` / `0x140894330` → `0x14124bec0`. It is the smallest geometry cut supported by retained native evidence and an existing guarded converter implementation. Keep it independent of Boolean and RIO behavior. Do not expand this cut into general mesh lifecycle or infer a Cogging Save edge.

## Exact proof and current source boundary

The retained native analysis identifies `0x14126d0c0` as the bound geometry-list item refresh. For a positive geometry mesh, readiness `0x1408ee940` calls the coordinate getter `0x140514f00`, then connectivity getter `0x140894330`; two nonnull results permit builder `0x14124bec0`, whose rectangle is unioned into the item rectangle. The scene selector `0x140c8e0f0` can use the resulting scene rectangle only under its recovered view/display conditions and only when ordered radius exceeds `1e-6`; otherwise it falls back to the existing model box.

The converter already represents these boundaries in `source/apps/moproj2mst/src/engine/viewer.cpp`. It accepts only qualified readiness profiles and validates the known coordinate/connectivity layouts before deriving the rectangle. The remaining source-ready step is therefore a narrow evidence closure for a *reachable original populated geometry-list mesh*: identify a native writer/owner for the node set consumed by the readiness predicate and retain the getter/builder result contract for that same ownership profile. This would let the existing converter guard admit a demonstrated profile without broadening its accepted states.

## Guard and missing witness

Guard on the unresolved claim: positive synthetic counts and a statically described branch do not establish that an original Open/Save project populates this optional geometry-list mesh, establishes its node-set owner, and reaches both getters and the rectangle builder. Preserve the explicit unknown/refusal for unqualified positive-count meshes. Also keep the cache-refresh-before-Cogging-Save relationship unclaimed: the retained writer excerpt uses cached scene state, while its incoming refresh ordering is not proven.

The decisive missing witness is source-bound evidence for the owning writer and a retained native/REA observation (or complete qualified call-path proof) showing one real geometry-list mesh instance with its owner and getter outputs reaching `0x14124bec0`. Until then this cut remains a dependency investigation, not a claimed reachable feature or parity result.

## Evidence identities

All paths are relative to this converter checkout. SHA-256 values bind the reviewed source/evidence files:

- `analysis/wave2-geometry-contract.json` — `c6d07a8d9ad1f1c2199a6112578b0490a5a0eb26ed5a5a7bd929220d422b95f4`
- `analysis/wave2-geometry-static-evidence.json` — `b6ef96c73a4707d9bc6f02c49a5288def8f9738ad276576af905519bdd8d13db`
- `analysis/wave2-geometry-contract.md` — `16bc95ebf19284d5d7eb0ddcf53587aa9de1469f9e8ae8905f96a86e3227f3b5`
- `analysis/resume81-cogging-mesh-pe.md` — `188ea345dfccd3309cae2ac2434838a4c3c5e5095d02cd13ff0eafbfbefcf958`
- `analysis/resume53-geometry3d-ownership.md` — `ae81da87d1d1d675f7a0b8ab5809aba997b1b0098ef61c75c7e2c71692b64a8d`
- `source/apps/moproj2mst/src/engine/viewer.cpp` — `3cd7ba2978cf3823c8500b289ac27c04b1548f545a3e0b00bd3ed3a4bf8bbea4`

Qualification: the static contract combines retained Ghidra/REA decompilation with offline PE/disassembly checks; it does not claim execution of the whole scene selector or demonstrate a real populated geometry-list mesh reaching the builder. No source/test/build, GUI, live REA, Boolean, or RIO work was performed for this report.
