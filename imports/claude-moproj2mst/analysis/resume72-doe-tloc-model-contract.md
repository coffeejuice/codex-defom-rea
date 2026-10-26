# CP72 one-variable Type 2 TLOC model contract

## Target and evidence boundary

Use the retained B121 post-action file `evidence/gui_mo/oracle_b/121_B_DOE_VAR/DoeStudies/DoeStudy1/Control/DoeInputTable.XML` as the one-record model target (707 bytes, SHA-256 `18ec0c482730c82ade12e91771ca67e53e4604573032624b99f3f4829a86b069`). This is an immutable historical after-state fixture, not a fresh original Open–Save result. Do not feed the B121 `_before` project to a test that expects a study or variable to be synthesized.

The current parser at `src/doe/doe_study.cpp:375-386` accepts empty `DoeVariables` and empty `Operations`, but refuses immediately when either contains child elements. The writer at `:324-329` similarly refuses any nonempty `Study::vars`; `saveInputTable` also refuses a variable before operation-index finalization. The current `doe::Var` is primarily geometry-shaped (`name`, index/number/type, geometry files/items, operation membership, link), with no scalar keyword payload. The test target must call lower-level parser/model/writer seams and must not enable public Type 2 activation or change `subproject.cpp` guards.

## Minimal model extension for this fixture

Avoid overloading geometry fields. Add a typed optional scalar-keyword payload to `Var`, sufficient for this one observed record:

```text
Var
  name = "Transfer Time"
  variable_id = 1                  // XML varId
  dimension = 3
  sample_count = 3                 // XML numSamples; do not equate to PAR row count
  scalar_keyword = {
    name = "TLOC"
    parameter_type = "double"
    unit_type = 0
    parameter = -1
    nominal_value = 0.1
    nominal_data_text = "TLOC     \t0.1"
    keyword_operation_ids = [1]
  }
  operation_binding_ids = [1]      // top-level Operations -> Operation -> DoeOprVariable
```

Preserve the distinctions: `varId` identifies the variable; the nested `KwOperations` and the top-level `Operations` are two serialized relationships, not one interchangeable list; and `DEF_DOE.INI`'s `OPR_NO=5` is a different operation numbering layer than the input table's id `1`. B121's `DEF_DOE.PAR`/`Samples_LHS.par` contain 100 one-value sample rows even though the XML `numSamples` attribute is 3, so the test must assert both independently and must not infer a relation the evidence does not qualify.

Prefer storing the parsed typed nominal value and retaining/recreating the `NominalData` text through a qualified conversion helper. For this bounded case, require the text to preserve the observed `TLOC` keyword name, tab separator and `0.1` value. Do not introduce generalized scalar formats, unit conversions, linked variables or constraints from this one record.

## Focused test sequence

1. **Read fixture:** call the lower-level `readInputTable` with the exact retained bytes in a direct `Study` test. Assert the loaded study-header fields and exactly one variable with the fields above, plus both operation mappings. Assert it returns successful child-reader status. Do not invoke `readLazyStudy`, project activation, Save driver, or original app.
2. **Model query:** assert `count()==1`, `varAt(0)` identity, reindexing behavior, and `varsForOperation(1)` contains the same variable exactly once; operation `2` returns none. These checks guard the connection from parsed XML to the existing `Model` dispatch used by downstream emitters.
3. **Serialize to recorded effects:** call `writeInputTable` with a recording `FileEffects` sink and a short synthetic path. Assert exactly one write to the requested `DoeInputTable.XML` target and no unrelated files/effects. Parse the emitted XML and assert one scalar variable, both mappings, and all numeric/string attributes. Once XML ordering/formatting is source-qualified, additionally compare the resulting bytes to the retained B121 XML as a candidate-only frozen-format regression; keep the raw fixture and candidate bytes distinct.
4. **Nominal side effect boundary:** separately test the B121 root `temp.KWD` representation (`TLOC     \t0.1\n`, 13 bytes; SHA-256 `15eefcf3997f7c89507af9d42db2e38a37e38f4f20d7f3b0467bd992455dc404`) through the existing keyword effect seam only if its owner/order is source-qualified. Do not append this file as part of the input-table writer test by assumption.
5. **Keep current refusal boundary:** no test in this contract should claim end-to-end public DOE support. Public conversion still selects active subproject index 0 and the parent Type 2 reader requires owned study and I/O wiring. The model contract can be merged independently while those lifecycle guards remain explicit.

## Retained neighboring data

- B120 supplies the minimal empty Type 2 `SfDoeStudy` plus header-only table. It is a useful prior case for the empty reader but has no scalar `Var`.
- B122 supplies a geometry-specific `Workpiece-Geometry` record and three GEO payloads. It requires a distinct geometry/file-owner model and is excluded from the TLOC model test.
- B121's sample metadata has `numSamples=3`, while mode-0 PAR tables have 100 rows. Keep model assertions for XML metadata and sample-manager behavior separate.

The precise schema above is supported by the retained B121 bytes, `analysis/wave2-doe-evidence.json`, and source locations in `src/doe/doe_study.cpp`, `src/doe/activation.cpp`, `include/m2m/doe_study.hpp`, and `include/m2m/mst/doe_model.hpp`. This is a candidate source/test design only. No source/build/test/GUI action or mutable REA operation was performed; no original parity claim follows.
