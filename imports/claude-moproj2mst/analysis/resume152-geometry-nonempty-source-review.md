# CP152 independent source review: first nonempty geometry dimensions prefix

## Result

No blocking source defect found in the frozen CP152 prefix. It commits only the native ordered dimension setters for the narrowly admitted first scalar receiver, leaves whole-leaf completion at zero, and parks before resize/allocation/copy/Stop. The nonempty branch does not fall through into the zero-width loop when admission or the late recheck fails.

## Source findings

`geometryMainSizingOwnerMatches(..., scalarPrefix=true)` binds this path to one main recipient, the initial high producer frame and actual owner, the published primary coordinate and six BC arrays, registry size/order, and ordinary widths exactly id1=1 with all other ordinary widths zero. `sizeGeometryMainFirstScalarPrefix` then qualifies the actual first receiver as id1 with cur0=0, cur2=1 and an empty unique values body whose first/last/end are null. It snapshots receiver identities and dimensions, performs the real mutable lookup at ordinal0, then revalidates current ownership, dimensions, binding and body identity before dereferencing the receiver again.

The successful prefix calls `set_dim0(1)` before `set_dim1(step.u15f8[variable->get_id()])`, matching the retained native setter order and preserving previous values. It sets `ScalarDimensionsCommitted` without advancing `mainVariablesCompleted`. The caller copies coordinates only for `VariablesSizedCommitted`; this stage cannot satisfy that predicate. The branch is selected specifically when `D[1] == 1`, so a refused/late-invalid prefix returns parked rather than entering `sizeGeometryMainZeroVariables`. The separate all-zero branch remains selected for `D[1] == 0` and its 28-recipient virtual loop is unchanged.

The updated width test now distinguishes D1=+1 (one lookup, ordered dim writes) from D1=-1 (no lookup). The new positive-prefix test checks previous/current results, unchanged cur2/prev2 and other receivers, empty values and null bound slot, retained primary/BC writes, no later recipient, and no replay after D repair. Five adversarial lookup mutations check refusal before setters while retaining the real lookup and earlier effects. These are candidate-only checks; they do not establish fresh original parity or allocator behavior.

## Qualification and hashes

Static review only; no build, tests, GUI, original execution, or mutable REA access was performed. The remaining explicit boundary is the unresolved native `resize(false)`/temporary operator-new request0 and subsequent data allocation request8, which the implementation correctly does not cross.

- `source/apps/moproj2mst/src/engine/viewer.cpp`: `8777AD9D05F6F357F34F82E18B1FA366D7CECF057F603A89B6858052D5C09669`
- `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp`: `0407258D80D8924A2BECC6E2A79D4943C049E830D22D036413C6FBB8D5E8EFF5`
- `analysis/resume152-geometry-nonempty-prefix-implementation.md`: `F87542D36F6A425896F586EFC7CBC54D7FC093AF00EBB27C8FFF89E90DD06B36`
