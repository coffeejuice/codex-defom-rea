# CP156: caller arguments are closed; the missing owner is the Boolean constructor frame

`130ad1` is unconditional once operation-wrapper setup `130830` reaches its argument suffix normally. A null model payload does not skip the notifier. The argument constructors do not dispatch callbacks. This removes the caller-argument uncertainty left by CP153, but does not establish that an actual candidate/native Boolean constructor has reached that suffix.

This receipt adds immutable-PE evidence and candidate-owner reconciliation only. No production source, tests, build, GUI/app control, original execution or mutable REA was used. Unless prefixed otherwise, short addresses below are original EXE RVAs under image base `140000000`.

## Exact branch and action sequence

In `140130830..140130ae0`:

1. `130857` calls `cbc920(&E[+38], incoming OPI)`. After normal return, initialize local `S=[rsp+60]=null` at `13085c`.
2. Resolve that OPI guard at `130871`. The **only conditional branch in this function**, `130879`, skips model retrieval if the resolved OPI is null. Otherwise resolve again and call `05f770(OPI)` at `13088f`, saving its actual returned SfStepData/GuiModel pointer as S at `130894`.
3. Both paths merge at `130899`. Call `377b20(E,S)` at `1308a6`; it binds **E+28**, a different guarded field from E+38. This must return before argument construction. Null S remains a valid argument value at this stage.
4. `1308bf` invokes `025080` with the type literal `1421a8848="SfStepData *"` and **address of local S**, not S treated as a generic-argument record. Its result is copied as a 16-byte argument. Nine subsequent QGenericArgument constructions use null type/data pointers. The first copied argument is passed at R8 (`rsp+120`); the next is R9 (`rsp+130`), followed by the remaining stack arguments.
5. `130ac3` selects `1421a8858="2signalChangeActiveStepData()"`; `130aca` loads the actual global notifier address `143d76700`. `130ad1` directly calls `2cf3c0`. Its normal return site is **130ad6**, followed only by this function's epilogue.

`025080` merely forwards to IAT `141e04168`, then returns its destination. The immutable QtCore export resolves to **670017f0**: store data pointer at argument+0, store type pointer at argument+8, return destination; RET at `670017fa`. The exact leaf ends at `670017fb`; adjacent bytes/functions are not part of this proof. Thus all ten argument constructions/copies have no event dispatch or allocation in these selected bodies. This does not erase failure or callback dependencies of the preceding guard/model calls.

`05f770` forwards to `05f820`, whose class lookup, virtual class-model query and possible lazy model construction are represented by `engine::opModel` in `src/engine/loader.cpp:168`. A class/model pointer being available later in editor fill does not prove that this earlier native call returned. Do not pre-evaluate it or manufacture S from OPI identity. A reached notifier gate/reset itself need not inspect S.

## Which constructor invocation owns the call?

The selected factory `1411e0510` first distinguishes literal **OprPreview** (`142cd26e8`, alternate 0x58 allocation/constructor `11e3970`) from literal **Object** (`142cd26f4`). The Object comparison succeeds before branch `11e0654`; only a non-null **0x50 allocation** from `11e065b` enters Boolean constructor `11e2480` at `11e0677`. The allocation-null path does not call this constructor. A source occurrence must retain this branch and allocation lifetime, not identify every class-editor fill as that native call.

Inside that constructor, `11e24a5` invokes `130740`. Its earlier call `130763 -> 377950` must return; `377950` calls QWidget construction at `37798a` and resumes at **377990** before installing its base tables/guard fields. Back in `130740`, initialize E+38 at `130791`, then call setup `130830` at `1307a1`. Only after setup returns does `1307af` call `130d70`. Boolean table publication at `11e24b7`, setupUi at `11e2529` and fill at `11e2533` are later. Neither a pending posted event nor an unsuppressed notifier can authorize jumping across those earlier return barriers.

## Candidate owner: present values, absent constructor epoch

The represented lifecycle owner is the registry-owned **BooleanClass instance**, with its `BooleanObjectEditor editor_` member in `src/ops/boolean.cpp`. Its actual `objectTreeFill` hook calls `g_l5Slots.booleanEditorFill(editor_,p,op)`; that slot maps to the **later** `1411e25b0` selection fill. The editor record in `include/m2m/mst/loader_l5.hpp:37` has shown/text values only. It is overwritten by selection fill and reused on replacement. It does not retain a 0x50 Object allocation, a constructor invocation generation, either E guard, or earlier QWidget/wrapper normal-return stages.

The canonical notifier owner is already obtainable through the Project's bound FlowReceiverWitnessRegistry, as CP153 established. That resolves state ownership but supplies no constructor-stage admission. `ConversionSession::beginEditor300Factory` retains an actual **Forming300** factory/base attempt and guards its Qt effects; it cannot stand in for this Boolean Object constructor. The current Session Boolean Apply/trace owners concern other calls and likewise cannot certify `130ad1`.

There is an existing nonowning **GuiModelGuard** primitive (`include/m2m/engine.hpp`; `src/engine/gui_model.cpp`) suitable for a future actual owner cell such as E+28/W+48. This narrows implementation machinery, but does not create either native owner or prove cbc920 returned. Do not add such a cell to MainViewState or equate W+48 with E+28/E+38.

## Smallest next cut and witness

The notifier mutation contract is source-closed **after an actually reached callee entry with return site 130ad6**: read canonical N+141 once; true preserves N+90 and returns; false resets N+90 before the existing key-construction frontier. It does not require non-null S or successful MainWindow delivery. CP152's receiver invocation and all E/H/Apply claims remain separate.

For production integration, first retain an Object-constructor frame on the actual Boolean lifecycle owner, distinct on each replacement, with the source-qualified allocation branch and consumed normal-return stages through `1308a6`. It must reach this argument suffix from those stages; a qualification string or caller-set reached flag is insufficient. This is the exact missing candidate action. Do not insert reset into ordinary `booleanEditorFill`, borrow the Forming factory, or add an isolated notification helper.

If an original occurrence is needed to bind that frame, the smallest targeted observation is `2cf3c0` **filtered by caller return 130ad6**, linked to its outer `130830(E,OPI)` and selected Object allocation/constructor generation, with N+141 read and N+90 before/after the reached branch. Recording the raw nullable S/type/data record confirms this caller's payload without inventing it. A same-E QWidget normal-return receipt at `377990` supplies the earlier constructor barrier; a later fill or constructor return alone must not be used to move the reset before an unobserved failure. No such new runtime receipt was captured here. E/H posted-notify and first-Open Apply/add require their own occurrence evidence and are not admitted by this cut.

## Reproduction identities

Original EXE SHA256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Original QtCore SHA256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Read-only DUMPBIN version: `14.44.35228.0`. Selected byte ranges end exclusive:

| Range | SHA256 |
| --- | --- |
| EXE 140130830..140130ae0 | 608b09364e69d463491cc8d205ea0022bf5c9b9cde168949fb01a24658e3188c |
| EXE 1401308ab..140130ad6, argument suffix through call | c3ab7fd4458ecafb1bea2a0566376bbd0e55a9bf4ce2977adfc3d73bad750e7c |
| EXE 140025080..1400250b2 | 84afcbe4515c24193732264e38a666409c8be07224991f2381bdd915e38d144c |
| QtCore 670017f0..670017fb, exact constructor leaf | 65d3a94c25b7067d6dd42c5ab3e7694104797881953021e8a64e5de2435c60f2 |
| EXE 140377b20..140377b46 | f2fee0ba1f33bc2f03414472c4e9cad29bf4ec3d3571cff991f19fe7c481cb35 |
| EXE 14005f820..14005f8ef | 392a28835eb687cae0ecb10f434790f0bc1ad1f4b6eefc747db127e4bf3a4d37 |
| EXE 1411e0510..1411e0736 | 3aa90a30675dee654f5ad165796a5a63fe229f8e7f018c4a03767279d5375747 |
| EXE 140130740..1401307bf | d68d74f9f103807ce55628c9e9b2fb7d783aace91da5a3fd2aa786a76fb874aa |
| EXE 140377950..1403779f4 | 77984c579a935d969ea7eaa77aa23fcf9faedc9abf36903c23a0a36a23e86073 |
| EXE 1411e2480..1411e2544 | c936cee30197093aef78c4c440a0864e37ebd983a41e02ae2226e87d75753b90 |
