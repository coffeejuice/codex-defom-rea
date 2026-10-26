# CP102: distinct load-time Boolean object-creation bound

A distinct native branch creates an object independently of the UpdateData/Apply timer question: Boolean postLoad1411df020 calls addObject when the OPI slot-record count is zero. Its normal call precedes propsLoad and postLoadViews in the same base-load invocation. The candidate already implements this branch. This narrows the audit and adds a required creation-site discriminator; it does not prove that DB133 selects the branch or resolve its missing-Cutter callback timing.

Only this unique report was written. No REA database access, original execution, GUI/control, source edit, build or tests. Immutable PE reads and DUMPBIN14.44.35228.0 were used in memory; no new raw artifact/database. Original EXE SHA remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Native proof at distinct sites

The PE's Boolean constructor1411de8d0 installs primary vtable142ccb3a8 at11de8f6/8fd. Direct table reads establish:

| Slot | Native target |
| --- | --- |
|+58|1405ef430 base load|
|+b8|1411def50 Boolean prologue|
|+78|1411df000 class-data leaf|
|+1b8|1405f6520 load-data decision|
|+c0|1411df020 Boolean postLoad|

Complete bounded decode of1411df020..11df188 shows current OPI/BooleanProps/data guards, then061220 at11df0cb returns the **OPI slot-record count**. Compare11df0dd and branch11df0e2 choose:

- Zero count: reload that OPI into RCX at11df0e4; call057ba0 at11df0e9, continuation11df0ee; pass slot0/kind5 to058aa0 at11df0fb; jump to return tail11df181.
- Nonzero count: branch11df102, resolve the stored workpiece's object through057710 at11df10b; the later kind7/slot scan can select a kind5 workpiece. It does not execute this zero-count add site.

The count is not GuiModel::objectCount, not `cutter==0`, and not the number of defined/non-kind7 objects. The selected zero-count body does not explicitly assign BooleanData workpiece/cutter values after the add; do not invent such normalization. The add result is not checked before the subsequent set-kind call. This is a creation **request** with subsequent source order, not an invented success/failure recovery contract.

Complete base-load decode1405ef430..5ef4ed provides the enclosing order after its current-OPI/model guards and model reset:

`+b8 at5ef485 -> +78 at5ef498 -> +1b8 at5ef4a8 -> +c0 at5ef4bb -> propsLoad054350 at5ef4c6 -> postLoadViews0547b0 at5ef4da -> viewport-name054250 at5ef4e4 -> return`.

Therefore, on this actual Boolean load invocation's admitted zero-slot branch and normal returns, its addObject call returns before that invocation's propsLoad/postLoadViews. No1000ms readiness or UpdateData delivery decides this branch. Binding the load invocation to a particular fresh Open still requires that actual selected owner/input/source occurrence; base load may also be invoked during later activation. Earlier load-data/helper or addObject descendants can have effects and are not declared callback-free by this body-local proof.

## Reconcile the creation selectors

| Native addObject continuation | Origin | Owner in RCX |
| --- | --- | --- |
|1411df0ee|Boolean postLoad zero-slot fallback|Actual current OPI|
|1411e295a|Boolean Apply Workpiece Add New branch|Editor+38 resolved OPI|
|1411e2bcb|Boolean Apply Cutter Add New branch|Editor+38 resolved OPI|

CP98's corrected selector remains: RCX is OPI, not GuiModel. The two Apply sites were intentionally a narrow mutation witness. A future trace claiming **all Boolean creation** must additionally cover11df0ee and preserve nested invocations; it cannot classify this load-time mutation as a timer callback merely because it occurred during Open. Conversely, observing a generic057ba0 hit without its continuation does not distinguish these causes.

The initial regular creation bound does not identify an actual earlier/later successful object return where nested helpers reenter. Actual first-execution claims still need complete ordered coverage, correct CP82 enclosing Open boundary and owner generations.1c94 remains loader return, not whole Open completion.

## Actionable independent seam

Current `l5_slots.cpp:booleanPostLoad` already uses `c.current`, guarded properties/data, `opi->objs.size()`, addObject and setSlotKind in this order. `loader.cpp:baseLoad` preserves the caller ordering. No new production mutation is justified merely by rediscovering it.

A bounded next regression can explicitly select an actual synthetic Boolean operation/class/model with **zero OPI slot records**, invoke its postLoad slot directly without editor Fill/Apply or event dispatch, and observe one add/set-kind prefix: a resulting slot0/object kind5 on the normal supported backend; no added BooleanData normalization; no manufactured second object merely because cutter is0. A second fixture with a nonzero slot-record count distinguishes this branch from zero engine-object count. Keep unknown/null model or backend failure guarded; do not force success to make a count assertion. Existing `test_l5_loader.cpp:boolean_load_and_workpiece_pick` covers loaded nonzero/kind7 workpiece selection, but an explicit zero-slot focused case was not found in that inspected test body. No test ran here.

For runtime acceptance of the missing-Cutter phase, retain CP98/101's actual D callback ->current impl/editor ->Apply/OPI witness, while adding the third continuation above to the passive mutation census. Record actual slot count11df0cb/zero branch if attributing the load-time fallback; do not infer it from an XML omission before generic loading completes. This is the smallest distinct next evidence/test action; no GUI/original run was attempted.

Source inspection hashes: `src/engine/l5_slots.cpp` `bf24d2de7d6119eb16da3766f185035529ea7a50247bb4ea8cabe02d0d5bc4b2`; `src/engine/loader.cpp` `746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c` under `source/apps/moproj2mst/`. New PE unwind bounds checked: Boolean ctor11de8d0..11de921, postLoad11df020..11df188, base load5ef430..5ef4ed. These selected bodies/table reads establish the conditional bound, not a fresh runtime occurrence or complete transitive callgraph. Public first-Open/PostPending guards remain.
