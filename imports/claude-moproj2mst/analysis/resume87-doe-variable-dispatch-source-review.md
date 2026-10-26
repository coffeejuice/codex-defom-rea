# CP87 independent review: DOE variable-child dispatch

## Result

The current source implements the proposed exact direct-child dispatch. In DoeVariables, only an immediate element whose tag is exactly DoeVariable reaches the existing unsupported payload boundary. Other element kinds/tags and raw children are ignored without recursion. After the entire list contains no exact row, CP85 reindex runs at that child encounter; CP84 ID finalization remains at the outer reader tail. No source correction is indicated in the reviewed snapshot.

## Order and partial effects

- Nonmatching-only lists proceed to reindex; a normally completed outer reader subsequently performs its one ID pass. Repeated no-row lists reindex at each encounter, but do not cause extra outer ID passes.
- A direct exact DoeVariable after ignored siblings throws at that encounter. No reindex for that list and no outer ID pass occur; prior top-level effects remain. The test confirms an earlier DoeRoiType assignment survives while existing Var index/number and ownership stay unchanged.
- If a later Constraints payload throws, earlier region names and empty-list reindex remain, but outer ID repair does not occur. The earlier Constraints guard remains before later DoeVariables and therefore prevents its reindex.
- A null encountered by the CP85 reindex still throws at its inner dereference after earlier index stores, and before outer ID repair.
- The direct scan does not parse/allocate variables, set geometry/scalar prefix owners, resolve links, alter operations/sample state, or notify the Session. Existing exact-row payload guard remains.
- Actual ConversionSession / active Type2 / ownedDoeStudy loading verifies owner identity and sample continuation order; notification attempts remain empty. Vars are empty in this fixture. The separate existing-record fixture retains the Save variable-dimension reconciliation guard.
- Same-owner Save with no Vars reaches immediate XML recording and the refresh boundary, but remains blocked by missing main-window receiver; old DOM is not replaced and no replayable project write is created. Serialized DoeVariables is empty and ignored metadata/nested non-direct row is absent. This is partial-effect evidence, not completed Save.

The source/test snapshot was read-only; no build or test run occurred. Retain native QDom ownership/fault parity and variable-row construction as explicit limitations.

