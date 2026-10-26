# CP83 independent source review: populated DOE Operations reader

## Assessment

The source change is narrowly limited to admitting populated `<Operations>` in `readInputTable`. It sets the existing child-reader status to true and does not read operation IDs/names, write `Var::ops`, reindex variables, or mutate the `Study`. This matches the retained normal path of original `0x140912a20`, which traverses child nodes, performs no payload action, and returns its initially true status.

One implementation-comment mismatch is the only review note: the candidate branch now sets `status = true` but contains no child-node traversal, while its comment says it “traverses every DOM child.” The original does walk firstChild/nextSibling and creates temporary element wrappers. With the candidate XML document already built by `xml::setContent`, skipping that invisible no-op loop preserves ordinary return and model-state behavior; it does not reproduce the internal traversal or its allocation/exception behavior. Either perform a bounded no-op pass over child nodes or describe the candidate as normal-return equivalent and leave those internal-fault paths outside the claim.

## Native and candidate behavior

The retained native evidence cited by `resume83-doe-next-independent-branch.md` is completed original `0x140912a20` within GUI PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; selected function bytes hash `a7456391b8e777c2586fef701520dde892c610f37fa3f01a730d91aff83d29a8`. Its local result starts true, loops through child nodes and returns that result. No mapping or manager callback is applied. The candidate preserves `status=true` for any `Operations` element, including unknown/nested children; malformed XML still returns false in `xml::setContent` before child dispatch.

`test_operations_reader.cpp` checks a populated element with raw text/comment, an operation-like mapping and unknown nested data while a test Study already contains a variable, region, constraint and sample state. It asserts true return and unchanged identities/fields, then verifies malformed XML is false and leaves the variable index unchanged. The assertions distinguish acceptance of traversal from mapping support.

## Guards and order retained

- **Variable branch:** nonempty `DoeVariables` still refuses at `0x14090ee20` before a later Operations sibling is reached. Its check remains in the earlier document-order branch; the test confirms that combined case refuses.
- **Regions and constraints:** populated `RegionsOfInterest` still clears `Study::regions` before refusal; populated `Constraints` still sets `Study::constraints=0` before refusal. Operations preceding either element only leaves status true; the test checks both partial clear orders.
- **Type/lazy-load boundary:** `parseSubProject` still requires the actual Type2 owned Study alias and configured Project StudyIo before `readLazyStudy`; Type3 `SfDoeStudy` still refuses. The active-study test explicitly selects a Type2 owner in a recording Session fixture. It is not a new public SUB-selection rule.
- **Lazy-load continuation:** for the fixture with empty `DoeVariables` and populated `Operations`, `readLazyStudy` continues after InputTable into LHS, FF, USR and then current `DEF_DOE.PAR`. The focused test checks reads and output attempts in that order, current mode/table loaded into the same owned Study, and no variable creation or XML output side effect from reading.
- **Save/output boundary:** the Save test uses the same owner and confirms the immediate InputTable XML attempt succeeds and reaches the existing `DoeTableRefresh` owner guard (`main-window +458` unavailable). The previous Project DOM Sub_Project remains, no final Project write is planned, and the test does not claim Save success. Type3 `.OPT`, DoeOutput/Report construction, variable parsing and notification delivery are not exercised or enabled by this change.

No edits to `subproject.cpp`, public writer, conversion/session, DoeOutput/Report, headers or public ABI are present in this source lane. Type/outputs conclusions beyond the guarded Type2 Save test are based on unchanged source branches, not new dynamic coverage.

## Disposition and limits

The populated Operations status branch is supported and safely isolated. Resolve the comment-versus-no-op-traversal distinction before presenting this as literal traversal parity; it is not a manager-mutation or public-admission defect. Root owns the build/test run; none was run during this read-only review. No GUI or mutable REA access occurred.

| Reviewed file | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `4108B7363F7CE5119E8C2C4497B98B928D7346383E41316AB60B9BB07B0ECEEA` |
| `source/apps/moproj2mst/tests/doe/test_operations_reader.cpp` | `05066ECF7FD6979B7325B1E3AB0C42BF154F29E295DF551FF5B6AF0196AD6A99` |

This review is the only written file.
