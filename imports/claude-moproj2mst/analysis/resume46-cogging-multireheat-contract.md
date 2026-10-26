# Cogging multi-reheat serialization contract for focused test

## Source binding and evidence limits

The serialized path is `datafiles::writeMdtBytes` → `writeMdt` → `masterWrite` in `source/apps/moproj2mst/src/datafiles/mdt_write.cpp` (current source around lines 404–470). `masterWrite` owns the ordering through `NoReheats`; `reheatWrite` (annotated `@src 0x14076cb50`) owns the one-reheat block. `writeMdt` appends Process Data only after `masterWrite` returns. The public test seam is declared in `source/apps/moproj2mst/src/datafiles/readers.hpp`.

The retained source qualification for queue C10 in `analysis/closure-audit.json` cites original static source at `source/apps/moproj2mst/src/datafiles/mdt_write.cpp` lines 422 and 590 and identifies original addresses `0x14076ac03`, `0x14076ac72`, and `0x1407617e0`. The reverse-engineering spec `source/docs/re/moproj2mst/mdt/spec.md`, section 3, ties the native call sequence together: `0x14076a8c0` emits defaults and Simulation Data; the `0x14076ac03..0x14076ac72` loop writes REHEAT records; `0x1407617e0` appends Process Data. It records the pointer as cumulative, with the second-loop destination index `n - 1 + i(i + 1)/2`, so for `n >= 2` the original writes the second record under “Reheat 1” and then reads beyond the reheat array. The C10 entry explicitly classifies this as non-reproducible/undefined and says not to claim reproducible output.

The retained verified disassembly file `analysis/wave3-cogging-verified-1407617e0.asm` is the Process Data writer body and its call into `0x14076a8c0`; it does not itself contain the `0x14076ac03..0x14076ac72` loop body. Thus the pointer-induction conclusion is bound here through the retained spec's analysis of that address range, not represented as a fresh direct disassembly inspection. The relevant native result remains static evidence; no runtime capture is recorded. The current port deliberately guards `nReheats >= 2` and throws `M2M_E_UNSUPPORTED` after writing `NoReheats` to its local `Out`. It also rejects a positive count with too few `reheats` elements before writing `SIMLST`. Neither port behavior should be described as an observed original multi-reheat result.

## Exact assertions for one focused C++ test

Add a focused test beside `tests/mdt/test_cogging_save.cpp` (or a dedicated `test_cogging_reheat_serialization.cpp`) using only `data::CogModel`, `CogGlobals`, and `datafiles::writeMdtBytes`; this does not require an engine model or application capture. Construct deliberately distinguishable data:

- `nReheats = 1`, `reheats.size() = 1`, `nPass = 1`, `passes.size() = 1`.
- Mark reheat heat slots 0 and 2 active with distinct `nstep` values, leaving slot 1 inactive. Mark pass bite heat active, pass heat active, and one bite deformation record present. Give the bite, pass, and three reheat heat blocks unique `nstep` or `UserKey` markers.
- Set pass `noBite = 1`, `autoBite = 0`; ensure the bite vector contains one bite. This avoids invalid vector access and selects the non-auto labels.

Assert using exact byte subsequence/line positions (CRLF included as emitted by the writer), not unordered containment:

1. The `Simulation Data` banner precedes `SIMLST`; `SIMLST` precedes its generated labels; all labels precede `NoReheats 1`; that line precedes the `Reheat 1` banner; the full reheat record precedes `Process Data`.
2. The `SIMLST` count equals four for the selected active records: `RH01-HI`, `RH01-HO`, `RH01-PS01-BH001`, `RH01-PS01-BD001`, and `RH01-PH01-Fix` actually totals **five**; assert `SIMLST 5` and the exact list in this order with counters 1–5. (The bite-heat entry is present because bite heat is active; BD is unconditional; pass heat follows bite records.)
3. In `Reheat 1`, assert `NoPass 1`, then the three `Heat Transfer` records in slot order 0,1,2 including the inactive middle slot, then `Pass 1`; within the pass assert its pass fields, then its own HEAT block and `Bite 1`/bite heat and deformation fields in the exact order encoded by `passWrite`/`biteWrite`. Use unique UserKey/nstep markers to distinguish reheat slots from pass and bite blocks, and assert no `Reheat 2` banner.
4. Separately within the same test function (or a second test case in the same focused file), set `nReheats = 2` with two valid default-constructed elements and assert `writeMdtBytes` throws `Error` with code `M2M_E_UNSUPPORTED` and the current candidate diagnostic prefix `NoReheats 2`. Do not assert native-equivalent output, a particular native crash, or exact partial bytes: the API returns no string after the throw, and the original pointer walk is out of bounds.
5. A malformed ownership guard case can assert `nReheats = 2`, `reheats.size() = 1` fails first with `M2M_E_INPUT` and `reheat count does not match its reheats`; this verifies guard order versus the unsupported branch. It is a port contract check, not original parity evidence.

Field writer references for these assertions: `reheatSimCount`, `reheatSimLines`, `passSimLines`, `simLine`, `reheatWrite`, `passWrite`, and `biteWrite` in `mdt_write.cpp`. The source determines that active reheat heats are considered in slots 0,1,2; pass simulations are processed in pass order, bite order (bite heat only when active, then each bite deformation unconditionally), then pass heat when active. The REHEAT text block separately emits all three heat records regardless of `active`, then each pass in index order. Avoid asserting that inactive heat records are omitted from the REHEAT block; only SIMLST filters them.

## Acceptance boundary

This test can pin the current deterministic one-reheat writer's bytes and the port's two-reheat refusal. It cannot validate the original's undefined multi-reheat bytes or turn that refusal into a recovered original behavior. Original runtime capture remains a separate evidence item and must retain crashes/variation as observations rather than tune this test to one output. No sources/tests/builds were changed for this contract note.
