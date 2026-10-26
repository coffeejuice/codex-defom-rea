# Independent connector implementation review

The parser admission patch matches the frozen four-class contract. Its first integration exposed two inherited missing registry flag initializers, Cycle and DoeOutput, that the four-class tests did not cover. Both flag entries are present in the coordinator source at this review's final registry check. This closes the identified flag-initialization gap for all 28 registered keys; successful whole-project original comparison remains pending.

## Actionable findings from the initial review

1. **Cycle flag initialization was required before shipping the new gate.** `CycleClass` originally inherited `OpClass::dimFlags = 0`, and the original classInit table omitted SfOprCycle. The new type helper consequently treated Cycle as Control2d and rejected Forming3D->Cycle and Cycle->Forming3D. Original `FUN_14134d2e0` in [init1.c](../evidence/gui_mo/s1/s1-07/dc/init1.c) starts at line 1926 and calls `0x1405fd8a0(info, 0x301b)` at line 1991: bits 1+2 select Control2d3d. The coordinator has added the source flag entry during this review.

2. **DoeOutput has the same dependency.** `DoeOutputClass` in [noop.cpp](../source/apps/moproj2mst/src/ops/noop.cpp) originally inherited zero flags and lacked a table entry. Original `FUN_140e2df30` starts at init1.c line 2112 and stores `0x20097` at line 2172, also Control2d3d. The class is registered and can appear as a parsed DiagramItem in an ordinary Sub_Project; a DOE study is not required to reach connector parsing. The coordinator has added the source flag entry during this review.

Both findings are resolved in the final inspected source: classInit stores Cycle 0x301b and DoeOutput 0x20097. The new cycle_and_doe_output_have_original_combined_connectors case uses the real registry and parser for each bridge with Forming2D and Forming, checks incoming and outgoing links, and verifies a later compatible incoming edge cannot steal the input. This directly exercises both restored initializers. Existing manually linked group-order tests bypassed registry initialization, which explains why the omissions were previously invisible.

## Implementation review

- `connectorType` correctly gives flag 0x10000000 precedence over ordinary dimension bits. Forming2D, Forming, Report and Convert2Dto3D resolve to the types established before editing.
- `connectorAccepts` implements exact equality and combined/single compatibility in either direction. QString equality is case-sensitive here, matching the original fixed strings.
- Parser admission evaluates compatibility before `!to->prev`. The source loop tries each newly created output once, so input occupancy is the relevant mutable condition. Earlier incoming edges do not occupy a later source output. The generic clearing/linking helper remains unchanged.
- `makeRegistry` calls classInit before inserting each object; constructor-provided flags survive when there is no table row. Production convert calls makeRegistry before Open. The test helper also uses the real registry before parsing.
- Existing cellless-item placement, item refresh and view-height code remain present. The patch does not overlay the older L5S file.
- The null-info helper branch remains explicitly labelled as constructor fallback; it does not prove unknown-class Open success. The inherited parser can allocate unknown names independently of original lookup behavior, which remains an existing scope-qualified unknown.

No branch-precedence defect was found in the new helper itself. No additional default-zero initialized registered key remained after the two coordinator additions. This check is source initialization coverage, not independent validation of all 28 original flag constants.

## Bounded registry audit

Exactly 28 insertClass calls are present in makeRegistry. The initial table supplied 21 registered classes; five factory constructors supplied their own flags; Cycle and DoeOutput were the only omissions. The coordinator's current table supplies 23 registered classes and the same five constructors supply the remaining flags. SfOprGroup and SfOprCopyMirror are not registered and are outside this denominator. Their original registration/lookup behavior remains separate backlog work; this report does not endorse complete original registry closure.

| Registered key | Current flags | Initialization |
|---|---|---|
| SfOprForming | 0x1a | classInit table |
| SfOprForming2D | 0x19 | classInit table |
| SfOprHeatTransfer | 0x1a | classInit table |
| SfOprHeatTransfer2D | 0x19 | classInit table |
| SfOprMultiBlow3D | 0x2a | constructor: ops/multiblow.cpp |
| SfOprMultiBlow2D | 0x29 | constructor: ops/multiblow.cpp |
| SfOprFormingExpress | 0x16 | classInit table |
| SfOprFormingExpress2D | 0x15 | classInit table |
| SfOprDistortion3D | 0x1a | classInit table |
| SfOprDistortion2D | 0x19 | classInit table |
| SfOprHeatTransferExpress | 0x16 | classInit table |
| SfOprHeatTransferExpress2D | 0x15 | classInit table |
| SfOprHTFurnace | 0x1a | classInit table |
| SfOprHeatTreat | 0x2a | classInit table |
| SfOprHeatTreat2D | 0x29 | classInit table |
| SfOprCogging | 0x1a | classInit table |
| SfOprPre3D | 0x5a | classInit table |
| SfOprPre2D | 0x59 | classInit table |
| SfOprReport | 0x9b | classInit table |
| SfOprShapeOptim | 0x40097 | classInit table |
| SfOprDoeOutput | 0x20097 | classInit table |
| SfOprConvert2Dto3D | 0x1100009d | constructor: ops/convert23.cpp |
| SfOprBoolean2D | 0x1000099 | constructor: ops/boolean.cpp |
| SfOprBoolean3D | 0x100009a | constructor: ops/boolean.cpp |
| SfOprCycle | 0x301b | classInit table |
| SfOprDiestress | 0xdf | classInit table |
| SfOprDiestressBM2D | 0x4000009d | classInit table |
| SfOprDiestressBM3D | 0x4000009e | classInit table |

## Tests and evidence limits

The five initial inspected tests cover the frozen four-class 16-cell acceptance matrix, first accepted incoming edge by parse order, rejected first edge leaving a target free, a target with an outgoing edge retaining its independent input, and a 2D->Convert->3D chain. The regression-before log records two failures out of five cases. The fixed-v2 log records four CTest targets passing (connector_contract, flowops, order, subproject), which is distinct from the number of cases inside connector_contract. No build or test was run by this reviewer. Those retained logs precede the final two-case addition; the coordinator must bind the final candidate to its own final regression receipt.

The two missing initializers are the critical coverage gap. The frozen contract also includes equal-type and Convert self-loop admission and combined-type occupied-target fixtures; those are not present in the initially inspected test file. They are useful follow-up parser checks, but do not replace the required Cycle/DoeOutput registry coverage. Self-loop tests should inspect admission/link state without invoking downstream graph traversal unless that traversal's contract is separately recovered.

The retained original-function probe executes only 0x140074f50 with mocked type and owner-index callbacks. It supports the admission branching conditions, not original type predicate execution or GUI parsing. Original PE string resolution and retained constructor/initializer source independently establish the four frozen representative class types and the two newly found flags. Full original GUI Open->Save->Close has not been run for this patch.

## Review source identity

Final source review refreshed 2026-10-23T10:43:32.955398+00:00. Initial bounded registry check time: 2026-10-23T10:42:57.684132+00:00. Source may continue changing under the coordinator; these hashes define the files observed at report generation.

| Path | SHA-256 |
|---|---|
| [source/apps/moproj2mst/src/model/subproject.cpp](../source/apps/moproj2mst/src/model/subproject.cpp) | `278479b54f1f60f4b8a2de5e2c0d7110fa0ae0bf5b4d8050c9ac53150cca4c8e` |
| [source/apps/moproj2mst/tests/p03/test_connector_contract.cpp](../source/apps/moproj2mst/tests/p03/test_connector_contract.cpp) | `554ee9e002dc8e8dd849b212390b766555bc93e6edfcad5daf917d93cdc49713` |
| [source/apps/moproj2mst/src/model/classinit.cpp](../source/apps/moproj2mst/src/model/classinit.cpp) | `fff60fa7bcf99be293ba0cb36f1c0d6b8a5810b057feaaf7a3cadad7e6b01bdc` |
| [source/apps/moproj2mst/src/model/registry_rescue.cpp](../source/apps/moproj2mst/src/model/registry_rescue.cpp) | `fb341cb7becf1322c4b253f0c76dad0414551535a6a405970455d6a101524f9d` |
| [source/apps/moproj2mst/src/ops/cycle.cpp](../source/apps/moproj2mst/src/ops/cycle.cpp) | `a570e09a51a4bc12f50494a333e1d5c1c694012a9fcfe1a50959b58d7d1edc17` |
| [source/apps/moproj2mst/src/ops/noop.cpp](../source/apps/moproj2mst/src/ops/noop.cpp) | `8850a5704e2f153aaeefd21e586cc64a16047f87e875970b07b73718dcbff544` |
| [source/apps/moproj2mst/include/m2m/mst/writer.hpp](../source/apps/moproj2mst/include/m2m/mst/writer.hpp) | `54f7ad3e5cb4732d602c66cda4e4d3e6e1b7f9f8e8b585fa4196f437642e1fcc` |
| [evidence/gui_mo/s1/s1-07/dc/init1.c](../evidence/gui_mo/s1/s1-07/dc/init1.c) | `a56e46ef917e0e2cb84b4357ad4f82099d1ac4a80b5ca2600ee8a330f506a85a` |
| [analysis/connector-type-contract.json](../analysis/connector-type-contract.json) | `2a4ed136383127806ce1a16d5dbdb70db13ac5aa5dd2b7adf6d3463da3510cf9` |
| [analysis/connector-admission-original.json](../analysis/connector-admission-original.json) | `6507135bd3276b2e0b47cc9acbb67df9b0dccdd6b6e05e5566474d152f55933e` |
| [build/logs/connector-regression-before-test.log](../build/logs/connector-regression-before-test.log) | `9990ec77056f5c9c8fcf69ed520b2706b1bf1718e573ea64a68020315a6660d1` |
| [build/logs/connector-admission-fixed-v2-test.log](../build/logs/connector-admission-fixed-v2-test.log) | `296b80a1b113e83ea7dbad0942b1b7fae11879c9ffb7b9f2f4ffc4414b670ae7` |
| [evidence/gui_mo/L5d/82760.c](../evidence/gui_mo/L5d/82760.c) | `7793419ce9f621f6dc9c9d8f8e54226010a36947035c5059a9c100ec438c30f3` |
| [evidence/gui_mo/L5d/conn2.c](../evidence/gui_mo/L5d/conn2.c) | `60675d30acca3334133e0dbf18c57736e83f655231f29c6704d2a148eb5448b2` |
| [evidence/gui_mo/L5d/74f50.c](../evidence/gui_mo/L5d/74f50.c) | `bf280b718bed6c6e7c6eb5f2e129da578480792d3bd849e469cbbbc6984b4b58` |
| [evidence/gui_mo/s1/s1-01/dc/sub0.c](../evidence/gui_mo/s1/s1-01/dc/sub0.c) | `a6ad05e4c5ffddf6911c4f0211af451197aac04c5bff67d76dc2f7ad71026529` |

The frozen connector contract separately retains the pre-correction classinit.cpp and subproject.cpp identities. This report writes no source and launches no original process.
