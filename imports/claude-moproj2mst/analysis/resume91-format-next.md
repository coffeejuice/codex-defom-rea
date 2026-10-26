# CP91: one-value reader inventory after B_WEIGHT BOM coverage

## Result

No additional public one-value numeric sidecar was found in the retained reachable Extra3D Open caller. The one-value file readers are now accounted for: `SW2SP.DAT` and the integer value in `STRETCH.DAT` use `FUN_1410da690` with count 1; `B_WEIGHT.DAT` uses `FUN_1410da510` with count 1. The candidate has source-qualified integer routing for the first two and the bounded recognized-BOM/ASCII decoder for the one-double B_WEIGHT path.

The original `FUN_140ec8110` call inventory in `analysis/wave2-save-diffs-300-support-140ec8110.c` is:

| Original source line | Reader and count | Literal binding | Candidate path |
|---:|---|---|---|
| 46 | `FUN_1410da510`, 2 doubles | `0x142b14e60` → `NBC.DAT` | `readDoubles(NBC.DAT, array<double,2>)` |
| 65 | `FUN_1410da690`, 1 int | `0x142b14e78` → `SW2SP.DAT` | `readInts(SW2SP.DAT, array<int,1>)` |
| 79 | `FUN_1410da690`, 1 int | `0x142b14e90` → `STRETCH.DAT` | `readInts(STRETCH.DAT, array<int,1>)` |
| 94 | `FUN_1410da510`, 3 doubles | `0x142b14ea0` → `STRETCH.DAT` | `readDoubles(STRETCH.DAT, array<double,3>)` |
| 112 | `FUN_1410da510`, 1 double | `0x142b14eb8` → `B_WEIGHT.DAT` | `readDoubles(B_WEIGHT.DAT, array<double,1>)` |
| 126 | `FUN_1410da510`, 0 doubles | `0x142b14ed0` → `STRAIN_DST.DAT` | `readDoubles(STRAIN_DST.DAT, empty span)` |

String addresses are independently mapped by `analysis/wave2-save-diffs-300-extra3d-keywords.json`. The retained sidecar reader decompilation in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` binds these calls to QFile ReadOnly and default QTextStream. B_WEIGHT's BOM inputs are therefore a complete one-value source slice; the CP90 implementation deliberately applies them only to that exact path/count.

## Remaining candidates are different scopes

`NBC.DAT` requests two values and the later `STRETCH.DAT` double read requests three. Their default QTextStream instances use the same BOM decoder, but extending the candidate's bounded BOM decode to those spans would admit a different multi-extraction path. Before doing so, review/test the exact caller-visible behavior for empty/trailing rows, extraction failure, status continuation, cursor/pushback, and decoded delimiters. A BOM alone does not establish that multi-value `readReal` behavior matches the original. These are the smallest next numeric-reader candidates, but they are not a new one-value contract.

`STRAIN_DST.DAT` passes count zero. The original and candidate open the file and do not consume bytes, so its contents do not exercise codec or numeric parsing on this path.

`SPRING.INI` is a separate count-first path (`FUN_1410dab40` and `FUN_1410da690`) where the parsed integer controls allocation/iteration. The current candidate intentionally keeps populated spring handling unsupported. Do not route it through the B_WEIGHT BOM or SW2SP integer allowances without a separate allocation-safety and original count-reader contract.

## Recommendation and evidence boundary

Do not add another generic codec route in CP91. The next implementation would be multi-value NBC/STRETCH, or the separately controlled spring count path; neither is a one-value follow-on. Continue only after selecting that scope and completing the corresponding original reader and candidate state/side-effect review. A live original-process comparison is not needed to establish the listed static caller counts, but is the next strong validation for any new multi-value behavior or unknown codec conversion path.

No source or test changes were made; no build, GUI, original-process session, or mutable REA operation was performed.
