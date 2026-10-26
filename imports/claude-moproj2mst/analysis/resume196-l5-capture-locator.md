# CP196 L5 original capture locator

## Scope

Read-only bounded inspection prompted by `analysis/resume196-l5-corpus-manifest.md`. Searched file names under the retained `evidence/gui_mo/oracle_b` case directory and `evidence/gui_mo/s1/corpus_b` case directory for `69_B_CNV`, `85_B_CNVF`, `M23_INPUT.KEY`, and `TEMP_STEP.KEY`; reviewed the relevant oracle-B and corpus-B case indexes. No broad filesystem scan, mutable REA database access, source/test/build/GUI operation, or content inspection of mesh payloads was performed. Hashes below were computed only for individually named payload files in other retained cases.

## Exact target cases

- `69_B_CNV`: neither `evidence/gui_mo/oracle_b/69_B_CNV` nor `evidence/gui_mo/s1/corpus_b/69_B_CNV` contains a file named `M23_INPUT.KEY` or `*TEMP_STEP.KEY`. The retained oracle-B and corpus-B case rows describe the native Convert2Dto3D case saved through GUI Open + Save; the oracle MST reference is not the literal payload or an ordered file-effect log.
- `85_B_CNVF`: neither `evidence/gui_mo/oracle_b/85_B_CNVF` nor `evidence/gui_mo/s1/corpus_b/85_B_CNVF` contains a file named `M23_INPUT.KEY` or `*TEMP_STEP.KEY`. The retained row describes Convert2Dto3D substituted for the first operation and GUI Open + Save. The surviving `SimCtrl.KEY` does not supply the missing output bytes or effect ordering.

Therefore this search found no exact-case M23 bytes, TEMP_STEP output, or occurrence-correlated ordered Save effect trace for either target.

## Nearby retained payloads (not substitutes)

The bounded file-name listing found payloads in other case directories:

| Case/path | File | Size | SHA-256 | Qualification |
|---|---|---:|---|---|
| `evidence/gui_mo/oracle_b/114_B_CNV_CLICK/Operations/Task00004` | `M23_INPUT.KEY` | 838 | `7a016951ce5d72f381c6e94002571f21858d33d3d8ee43b041c6dbd368b1c2ad` | Different case; not shown to be either target input. |
| same | `SfOprConvert2Dto3D_TEMP_STEP.KEY` | 1,480,644 | `70c1e741daafd05be4ac6cd01af2230fd62dbea4adf85800fe24567fb043ebc7` | Different case; payload content not inspected and no target-case correlation established. |
| `evidence/gui_mo/oracle_b/118_B_CNV_AX2S/Operations/Task00004` | `M23_INPUT.KEY` | 838 | `3d4841ba02807fea8a37b03d1a5595258a73ac0b24e79c524a2eb5a07bfb81ac` | Different case; no corresponding TEMP_STEP file in that task directory. |
| `evidence/gui_mo/oracle_b/129_B_DBSTEP_CNV/Operations/Task00002` | `M23_INPUT.KEY` | 2,362 | `d76ed9828132246ae795ac47fbe6513f2398c7fc3e441bb7c9b3d655240c2a89` | Explicitly synthetic: `analysis/wave2-db129-diagnosis.json` says `mkdbstep2.py` changed operation 2 to Convert2Dto3D while retaining a 3D database. |
| same | `SfOprConvert2Dto3D_TEMP_STEP.KEY` | 8,818 | `0099abeffd157cff96dee677f40798769bc1a7970be6320f61b41fb62e2d9ba3d` | Same synthetic case; cannot qualify native public-case behavior or target-case effect ordering. |

These files prove only that nearby retained captures contain such named payloads. They do not establish byte identity with either 69/85 inputs, nor an ordered original GUI Save trace for those exact inputs. The separate DB129 captures are historical candidate-versus-training comparisons (`original_executed: false`), not original application observations.

## Next action

Keep the CP196 step-owned interval guarded. If a qualifying witness is still required, acquire or locate an original capture tied to one exact target input that preserves the literal M23 bytes and occurrence-ordered Save effects (including TEMP_STEP creation/write evidence). Otherwise record that the retained imported corpus cannot supply it; do not infer the missing effects from MST references or substitute cases.
