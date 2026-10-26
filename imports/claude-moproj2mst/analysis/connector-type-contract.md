# Fixed connector parser contract

The projected type helper plus an unoccupied target input matches the original admission rule for the parser’s fresh-source loop. This contract was frozen before the coordinator’s connector code change. It covers four source-qualified representative classes and seven fixed parser fixtures.

## Original identity and qualification

Copied original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No original process or original function was executed by this reviewer. Static evidence is retained decompilation; PE literals were read directly at their section-mapped file offsets. All source hashes and raw string bytes are in the [JSON contract](connector-type-contract.json).

## Constructor and class flags

Constructor `0x14015ef80`, retained in [82760.c](../evidence/gui_mo/L5d/82760.c) from line 264, reads info slot +0x58. Default is Control3d; bit 2 clear selects Control2d; bits 1 and 2 select Control2d3d. Bit 0x10000000 selects asymmetric input2D/output3D.

| Class | Original initializer | Flags | Input | Output |
|---|---|---|---|---|
| SfOprForming2D | 0x140c043e0 | 0x19 | Control2d | Control2d |
| SfOprForming | 0x140c05bc0 | 0x1a | Control3d | Control3d |
| SfOprReport | 0x1412ad930 | 0x9b | Control2d3d | Control2d3d |
| SfOprConvert2Dto3D | 0x1401c24d0 | 0x1100009d | Control2d | Control3d |

The class key strings and flag arguments are independently established by init1.c and PE string locations. The five constructor strings at 0x1421d2738/2748/2758/2768/2778 resolve respectively to Control3d, Control2d, Control2d3d, Control2d, Control3d. Compatibility function 0x140161470 uses the same three names at its own literal locations; it accepts equality or a combined connector paired with either single dimension.

## Fixed fresh-source/fresh-target acceptance matrix

Rows supply the source output; columns supply the target input. These expectations were transcribed from original branches and fixed before editing C++.

| Source \ Target | Forming2D | Forming | Report | Convert2Dto3D |
|---|---:|---:|---:|---:|
| SfOprForming2D | accept | reject | accept | accept |
| SfOprForming | reject | accept | accept | reject |
| SfOprReport | accept | accept | accept | accept |
| SfOprConvert2Dto3D | reject | accept | accept | reject |

An occupied target input rejects in every cell when the source output is fresh. The type predicate is evaluated first in original admission 0x140074f50. An incoming edge on a later source does not occupy that source’s outgoing connector. The parser creates all items first, then attempts source indices once in ascending order (sub0.c lines 925–948); the both-occupied index comparison is therefore outside this slice.

Seven fixed fixtures in JSON cover first-wins, rejected-first compatibility, chained occupancy, asymmetric conversion, combined types and two self-loop admission cases. Fixtures specify requested links, attempt outcomes, final next/prev arrays and cells. They establish parser expectations only; self-loop traversal and successful full conversion remain separate questions.

## No-info fallback and probe limits

The constructor’s null-info branch uses Control3d. However, reg1.c FUN_14006ff90 returns null when registry lookup fails and invokes the constructor only with valid info. Thus an unknown class is not established to open as a default 3D item. Treat helper no-class-info fallback fixtures as synthetic constructor cases until the original registry failure path is traced.

The retained [original admission probe](connector-admission-original.json) executes 241 exact original bytes for 16 controlled cases. It mocks the type predicate and owner-index callbacks, so it supports admission branch ordering and link-state rules; it cannot validate actual type strings, flags, GUI parsing or Save. This review supplies independent static type evidence and does not extend that probe into a whole-application claim.

Keep current item placement, view refresh and view-height recovery. Keep the generic connector mutation helper unchanged for other callers. Run real parser tests against the fixed matrix and fixtures, then freeze the candidate before fresh original project comparisons.
