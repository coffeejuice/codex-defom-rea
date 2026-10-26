# Checkpoint 53 connector acceptance audit

## Status

The connector correction is implemented and source-regression checked, but original
project parity is still unestablished. `analysis/takeover-state.json` records the
correction as `implemented_and_regression_checked`, with 16 native admission-probe
cases, nine P03 regression cases, and `whole_project_original_parity: false`.
The 16-case probe calls original `0x140074f50` with synthetic type and owner-index
callbacks. It characterizes that admission function's decisions; it does not call
the original type predicate, initialize real operation classes, parse a project,
or execute GUI Open/Save/Close.

The current `tests/p03/test_connector_contract.cpp` has nine passing cases in the
Checkpoint 51 test receipt (`resume51-combined-full-tests.json`, log
`build/test-runs/1792869167228861600/m2m.p03.connector_contract.log`). They cover
the four-class 16-cell matrix, occupancy/order behavior, a Convert bridge, Cycle
and DoeOutput combined connectors, the retained Cogging-to-DoeOutput edge,
Cogging's 2D/3D distinction, and type-admitted self-connection. The log reports
`0 of 9 cases failed`. The combined Checkpoint 51 build/test receipt has no
connector-specific gaps; it is converter-source evidence, not native project
acceptance.

## Exact source-backed reachable branch

The strongest retained real-project connector input is the complete Oracle-B
project `120_B_DOE/COG_P1_B1.moproj`, SHA-256
`60ADDC2722E85F92A088FA07F77E8A966928AB5B934F161DF87DCF6A417ED30E`.
`analysis/resume48-connector-real-edge-search.md` records that Sub_Project index
1 requests item index 2 (operation id 3, `SfOprCogging`) to item index 5
(operation id 6, `SfOprDoeOutput`). `classinit.cpp` sets Cogging to `0x1a`
(`Control3d`) and DoeOutput to `0x20097` (`Control2d3d`).

That edge reaches the shared parser boundary in `src/model/subproject.cpp`:
`parseSubProject` collects each DiagramItem's requested target, resolves the two
items through the registry-backed class mapping, and at lines 370–372 applies
`connectorAccepts(target input, source output)` before testing `!to->prev` and
calling `connectItems`. `connectorAccepts` implements equal-type admission and
the combined `Control2d3d` compatibility. The parser tries each new source output
once; this is the requested-edge load branch, not the general interactive
reconnect rule. The P03 test reproduces this edge with the real registry and XML
parser, but its two-item synthetic document does not prove that native Open
selects and accepts the same edge.

## Remaining checks and captures

1. **Bind a fresh original/candidate integration pair to the exact edge.** Make
   separate pristine disposable copies of `120_B_DOE`; do not compare against the
   retained post-capture folder as though it were a fresh original run. Capture
   original and candidate Open results and the per-subproject graph immediately
   after Open. For Sub_Project 1, record the requested edge and actual `next` /
   `prev` relation for operation ids 3 and 6. Preserve executable/source hashes,
   fixture-copy manifests, and ordered action/status receipts. This is the needed
   original comparison for the corrected DoeOutput input path.

2. **Capture lifecycle consequences separately.** For each same run, retain Save
   return/status, complete output-tree paths and hashes, DOE side-file effects,
   Close result, and post-Close/reset state. The retained historical comparator
   records `B:120_B_DOE` as `DIFF` with seven missing entries in Checkpoint 51;
   it is not a fresh original-session acceptance result and does not localize a
   connector mismatch. The active DoeOutput loading/Save implementation has
   separately qualified unsupported paths (`analysis/resume46-doe-output-next.md`),
   so report the first refusal and partial effects by stage rather than treating
   an expected downstream Save refusal as connector evidence.

3. **Pin the entire registered flag dispatch table if broader source assurance is
   required.** The implementation review accounts for 28 registered keys and
   says 23 receive `classInit` table values while five receive constructor flags,
   but explicitly does not claim independent verification of every original
   constant. Existing P15/P17/P12 tests cover subsets. A focused real-registry
   expectation table should assert every registered key's exact evidence-derived
   flags (including constructor-owned entries) and special connector type pairs.
   Keep unregistered `SfOprGroup` and `SfOprCopyMirror` out of this table: P02
   documents their intentionally absent registry entries.

4. **Keep the Cycle claims bounded.** Complete project case
   `51_MO2_Operation_Cycles2` is not an edge fixture: its Cycle item has no
   connection request (`analysis/resume48-cycle-connector-binding.md`). The
   current parser regression tests the Cycle combined flags with synthetic
   bridge edges. If native Cycle connector integration is an acceptance
   requirement, use a disposable modified copy with explicit incoming and
   outgoing Cycle edges and compare original/candidate graph state. Label it a
   synthetic modified-project probe; do not report it as unchanged case-51
   parity because adding the Cycle edge may change group and writer behavior.

5. **Do not elevate the native 16-cell probe to GUI acceptance.** The retained
   probe's `original_gui_executed: false` and qualifications say its callbacks
   are controlled. A separate native capture must establish original class
   lookup/flag initialization and actual parsing if those internals are part of
   the claim. The P03 test and current receipt already cover source parser
   behavior; repeating the mocked probe cannot close this gap.

## Evidence limits

The connector implementation review derives the four representative classes'
connector types from retained original initialization source and PE literals; the
Cycle and DoeOutput constants are supported by `evidence/gui_mo/s1/s1-07/dc/init1.c`
and match the corrected table. The original-probe receipt covers only the
admission-function branch conditions. The checkpoint51 full CTest receipt has
160 passing targets/cases as reported there, but its own qualifications say
CTest and historical comparators are not fresh original-session acceptance.
Nothing in this audit claims complete GUI connector parity, full DoeOutput Save
support, general reconnect/edit parity, or unknown-class lookup parity.

Read-only source/receipt audit. No source, build, GUI, project fixture, or REA
database was modified.
