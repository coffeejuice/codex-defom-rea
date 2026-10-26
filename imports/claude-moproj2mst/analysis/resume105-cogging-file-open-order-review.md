# CP105 Cogging open-failure test: operation order review

## Result

The candidate test's primary open-failure and final-MST checks are aligned with
the recovered Save order. Its Boolean BWZ/INI presence assertions are valid
observations of the whole Save, but do **not** establish continuation after the
Cogging open failure. The fixture puts the Boolean operation before Cogging.
The current test/report wording that calls those outputs “later Boolean”
outputs should be corrected before interpreting the test result.

## Fixture order

The input copied by
`source/apps/moproj2mst/tests/s/test_save_cogging_open_failure.cpp` is
`evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3_before/COG_P1_B1.moproj`. Its
`Operation_List` order, read directly from the retained XML, is:

| Position | ID | Type | Name |
|---:|---:|---|---|
| 1 | 1 | `SfOprHeatTransfer` | `HeatTransfer` |
| 2 | 2 | `SfOprBoolean3D` | `EXIT` |
| 3 | 3 | `SfOprCogging` | `CogABC` |
| 4 | 4 | `SfOprHeatTransfer` | `HeatTransfer (3)` |
| 5 | 5 | `SfOprHeatTransfer` | `ENTER` |

The native outer Save function `0x1400980d0` calls the property-save loop
`0x1400982e0`, then the main MST writer `0x1400a20d0`. Inside
`0x1400982e0`, the loop obtains the operation count, iterates indices from zero
upward, resolves that indexed operation, and invokes its property-save virtual
slot `+0xb8`; it ignores the individual return value and advances. Thus the
Boolean side-file callback for position 2 precedes Cogging's position-3
`fopen("wt")` attempt. The later position-4/5 operations are heat-transfer
operations; the current assertion does not identify any side file from them.

The candidate reconstruction's `convert.cpp` also walks `p.operations` in
vector order and emits Boolean side files inside that loop. Its assertions
`read(Boolean.BWZ)` contains `WZD_BOOLEAN_CUTTER_OBJECT_NO` and
`read(DEF_MACHDIST_CTRL2.INI)` contains `WZD_MACHDIST_OPERATION_BOOLEAN`
therefore confirm that the earlier Boolean operation ran during this Save.
They are not a post-Cogging witness. The retained static native proof does not
make that candidate observation an original runtime execution.

## What the test does establish

The `aboutToClose` action runs after candidate project output has opened and
closed, while the converter call is still in progress. Holding an existing
Cogging file open for read with `FILE_SHARE_READ` denies the later CRT write
open while allowing the earlier project/input reads. The test requires the
previous Cogging bytes to remain unchanged. This is consistent with original
static evidence: native Cogging writer `0x1407617e0` calls `fopen("wt")`; its
null-file branch bypasses serialization and returns 1, and the native
property loop ignores that result.

The test's `native-output.MST` end-marker assertion is the meaningful
post-property-loop continuation witness: native Save places `0x1400a20d0`
after `0x1400982e0`, and the candidate conversion similarly reaches its MST
writer after iterating operation properties. `M2M_OK` remains a candidate
adapter assertion only; it does not assign a status to the native GUI Save.

## Minimal correction

Keep the BWZ/INI assertions if desired as whole-Save controls, but describe
them as output from the earlier Boolean operation. In particular, change
“later Boolean BWZ/INI” in the test comment and
`analysis/resume105-cogging-open-failure-test.md`; the test's own explanatory
comment currently says only that the callback runs after Open, and its
assertions need no logical change. Keep the MST assertion as the
post-property-loop check. A test specifically requiring a later operation
property output would need a fixture with a suitable writer after Cogging in
its operation order and source-qualified expected effects; the current fixture
does not provide one.

Evidence is the retained original Save decompile
`evidence/gui_mo/s1/mdt/save_d0.c`, source qualification
`analysis/save-semantics.md` and `analysis/resume105-failure-close-matrix.md`,
the copied fixture XML above, and candidate sources
`source/apps/moproj2mst/tests/s/test_save_cogging_open_failure.cpp` and
`source/apps/moproj2mst/src/core/convert.cpp`. No source or test was changed,
and no build, GUI, original execution, or mutable REA database was used.
