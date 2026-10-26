# CP252: original named-display creation with a main model but no step

The original dimension getter's default of 3 does **not** establish successful named-model creation without a main step. On a named-map miss, with successful allocations/initialization and a stable live main wrapper whose step remains null, the original constructs a new dimension-3 model, performs name-dependent role/metadata work and sets its display backlink, then unconditionally reads the main step's `+0x744`. The verified load is at `0x140cfa4be`; a null main step makes its effective address `0x744`. Title/control copying, named-holder publication and normal return occur later and are not reached on that ordinary faulting path.

This is a conditional static control-flow result, not an observed original crash, process exit, exception-disposition or reachable fresh-project occurrence. A production change that merely substitutes default dimension 3 and returns a usable named model would be wrong. No safe bounded production no-step behavior change is ready from the getter alone or this trace; retain the current converter's explicit adapter refusal and identify its lack of native failure-prefix parity.

Only this report is written. No production/test source, build, shared workflow state, original execution, GUI control or mutable REA database was changed. I applied the reverse-engineer-anything skill using retained decompiles and read-only PE/DUMPBIN inspection; no REA session or target process was opened.

## Evidence identity and inspection boundary

Original: `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2 as qualified by the retained converter evidence; independently recomputed SHA-256:

`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

Read retained `evidence/gui_mo/s1/mdt/L4/vs/a.c`, `b.c`, `c.c`, `g.c`, the shared constructor in `evidence/gui_mo/s1/mdt/L0/A/b0.c`, current `source/apps/moproj2mst/src/engine/scene.cpp`, model getters, and CP250's revised contract/review. Cross-checked complete unwind regions for the named creator and selected helpers directly against the immutable PE with installed Windows DUMPBIN, banner version **14.44.35228.0** (executable directory **14.44.35207**). The read-only command form was `/disasm:bytes /range:<start>,<inclusive end> <original PE>`; output remained in tool receipts, with compact byte hashes below. No generated assembly file or additional receipt file was written.

Unwind ranges are convenient retained code bounds, not proof that every C++ exception funclet or callee behavior is covered. The trace below identifies explicit ordinary code paths and qualifies prior calls as returning normally where needed.

## Input cases must remain distinct

1. **Name already present with a valid holder:** `0x140cfa127` calls the map presence helper; `jne 0x140cfa5e8` bypasses allocation/main-model queries. `0x140cfa5ff` obtains the holder and `0x140cfa60e` gets its guarded model. Then the by-value name QString is destroyed and the model value returns at `0x140cfa643`. A holder with an expired/null model may therefore return null without touching the main model; a null holder pointer from a separate allocation-failure state is not the same supported case.
2. **Name absent, live main wrapper, null step:** this is the discrepancy investigated here. The dimension getter can return 3 because its wrapper receiver is valid; the later step-number source load remains unchecked.
3. **Name absent, no main wrapper at all:** default 3 is not a null-model guarantee. `0x140c7fee0` calls `0x140c7acd0(receiver)`; that getter adds `0xe0` and dereferences the guard storage. A null wrapper would attempt a low-address guard read rather than safely reaching a completed nullable-step branch. Do not merge this case with a valid wrapper containing a null guard.
4. **Name absent, allocation or constructor/import exception:** normal prefix reasoning below does not establish allocator handler outcomes or constructor unwinding. Explicit null allocation branches exist, but they do not supply a safe named-model result.

The raw PE literals at `0x1429f8118` / `0x1429f8160` were checked as ASCII `ROIs` / `Coupons`, respectively.

## Verified ordinary path on a named-map miss

| Original address/range | Actual operation and no-step consequence |
|---|---|
| `0x140cfa137–14c` | Request `0x278` bytes via `0x14146eabc`, store result and test for null. This allocation happens **before** main-model resolution and dimension getter. |
| `0x140cfa14e–16a` | On nonnull allocation, call `0x140cf94f0(display)` to obtain the main wrapper; call `0x140c7fee0(main)`; pass returned integer to `0x140c7a6b0(new allocation, dimension)`. |
| `0x140c7fee0–ff1f` | Set local result to 3; get current step via `0x140c7acd0`; only if nonnull call `0x140100980(step)` to read its `+0x1c`. Valid main wrapper plus null step returns 3. |
| `0x140cfa19c–1f4` | Construct/compare the `ROIs` name and branch. These temporary QString calls are after the new-model constructor. |
| `0x140cfa1fa–211` | ROI branch gets the **new** model's step and calls `0x140831650(step,1)`, which directly writes `step+0x3400`. |
| `0x140cfa211–2fd` | ROI branch copies model name, constructs/assigns icon and translated tooltip through the named metadata functions, then joins the common backlink path. |
| `0x140cfa302–366` | Non-ROI branch compares `Coupons`. A different name skips role/name/icon/tooltip updates and joins the common path. |
| `0x140cfa36c–383`, then through `0x140cfa482` | Coupons branch writes new-step role 2 and applies its name/icon/tooltip metadata. |
| `0x140cfa487–499` | Call `0x140c7ade0(newModel, display)`. Retained decompile shows direct write to new wrapper `+0x10`; the model has not yet been inserted into the named map. |
| `0x140cfa499–4ae` | Re-fetch the display's **current** main wrapper; get its **current** step; save that source-step result in RBX. It is not the earlier dimension-getter result cached in another form. |
| `0x140cfa4b1–4bb` | Get the new model's destination step into R11. |
| **`0x140cfa4be`** | **`mov eax,dword ptr [rbx+0x744]`**. There is no source-null test between the main-step getter and this load. With RBX=0, effective source address is `0x744`. |
| `0x140cfa4c4` | Only after that load succeeds, store EAX into destination `R11+0x744`. On the stable-null-source fault, even this destination step-number store is unreached. |

This refutes an inference from the nullable getter to whole-function safety. Under ordinary Windows low-address protection, the stable-null-source load predicts an access violation. It is not a normal converter-style `M2M_E_UNSUPPORTED` return. The analysis does not assume that an actual application's current main wrapper/step cannot change between the earlier getter and this relookup: the code explicitly reads current state again after construction and, for the two special names, Qt metadata work.

## Constructor and role prefix

`0x140c7a6b0` first constructs a parentless QObject, installs the model vtable, initializes its list/step-guard/geometry/name/icon/tooltip members, then calls `0x140c7a870(newModel, supplied dimension)` and finally assigns its DB pointer null through `0x140c7aa80`.

Within `0x140c7a870`, verified code requests `0x3408` bytes at `0x140c7a8aa`; on nonnull allocation calls `0x140821bf0(storage, dimension)` at `0x140c7a8c5`, and assigns the resulting step through the wrapper at `newModel+0xe0` (`0x140c7a913`). Subsequent initialization helper calls and `newModel+0x25c=0` precede return. This constructor receives dimension 3 on the valid-main/null-step dimension path. It creates a separate new model/step; it does not, in the inspected direct argument flow, assign a step to the old main model.

These are source-qualified calls and stores. Their full constructor/callee failure behavior, allocator/new-handler policy and arbitrary imported Qt/reentrant effects were not exhaustively rederived here. The report's later-fault prediction therefore explicitly assumes this prefix returns normally, produces the needed new model/step, and the re-fetched current main step is still null. It does not claim all invocations reach the same PC.

The role setter `0x140831650` is a direct store of the supplied integer at new-step `+0x3400`; no null check. ROI and Coupons role initialization can therefore fail earlier if the new model lacks its own step. If the outer `0x278` allocation returns null, the constructor/dimension getter are skipped: ROI/Coupons soon call the new-model step getter on a null wrapper; an ordinary other name reaches the unchecked new-model backlink setter. Neither branch is a proved safe null return. A throwing allocation/constructor can instead leave through exception machinery before any later instruction, and its exact cleanup is not established by this ordinary-path audit.

Name/icon/tooltip setters assign new-wrapper fields `+0x260`, `+0x268`, `+0x270` using Qt assignment/destruction calls. They are distinct effects before the main-step-number load for those names. The model's display backlink is set before that load in every normal miss branch. These effects are to the new unpublished allocation; their occurrence must not be mislabeled successful publication or externally visible completed named-model creation.

## What lies after the null source load

Only if the step-number copy completes does the function proceed:

1. Re-fetch current main and its step again at `0x140cfa4d3–4e0`; compute source `+0x340`.
2. Obtain the new destination step, compute destination `+0x340`, and call `strcpy` twin `0x14146fec0` at `0x140cfa501`.
3. Query new-model control at `0x140cfa50b`; if nonnull query main control at `0x140cfa52d`; if nonnull re-query main/control and copy GEOTYP (`+0x100`) at `0x140cfa551–557`.
4. Allocate the `0x10` named holder at `0x140cfa562`; construct its guarded model/flag through `0x140d12250` when allocation is nonnull.
5. Obtain/insert the map slot via `0x140c75050` at `0x140cfa5d6`, then publish the holder pointer at `0x140cfa5e3`.
6. Destroy the by-value name QString at `0x140cfa62d` and return the new model at `0x140cfa643`.

The nullable control getter does not protect the earlier unchecked step-number/title operations. Stable null main step at `0x140cfa4be` means no title copy, control query/copy, holder allocation/map insertion, normal name destruction epilogue or normal return is reached **along this ordinary path**. Do not infer that no unwind cleanup/exception handler runs: the original has unwind metadata, and this audit did not qualify all language handlers, SEH disposition, caller handlers, runtime policy or process termination. No leak, rollback, accepted Close or fixed exit code is claimed.

## Current converter discrepancy and implementation readiness

Current `source/apps/moproj2mst/src/engine/scene.cpp` (SHA-256 at inspection `afd5b1cf049ca59a6c4f88d75853afba39d108f81a3634ce878d880c0d665275`) returns an existing named holder first, then explicitly rejects `!b || !b->step` **before** allocating the new model. CP251/CP250's read relocation uses `b->dimension()` only after that guard. This preserves the converter's prior live-step behavior and adapter refusal; it does not reproduce the original miss-branch allocation-before-dimension-getter prefix or the stable-null-main-step fault after new-model setup.

`GuiModel::dimension()` returning 0 without a step and `dimensionOr(3)` returning 3 are separate scalar API contracts. Changing this call to `dimensionOr(3)` while removing the explicit guard is insufficient: the subsequent source `b->step->u744` and title reads remain invalid. Adding guards/default values to those later reads, skipping them or returning a newly created named model would invent successful behavior not supported by the original trace. Moving a converter exception after some initialization would also require an explicit faithful failure-prefix/retention contract; ordinary `unique_ptr` exception cleanup must not be asserted to match original access-violation handling.

**No bounded production success/failure-parity patch is ready.** Keep the current refusal qualified as adapter behavior, retain the live-step leaf, and track this discrepancy rather than quietly “fixing” no-step creation. An isolated diagnostic model of the static prefix could record attempted effects, but it would be a new diagnostic scope, not original runtime evidence or permission to broaden the public converter.

## Smallest missing witness

For an actual-original behavior claim, acquire one owned, disposable **named-map-miss** occurrence with a live main wrapper and absent main step, binding:

- Original EXE/Qt image identity, display/main wrapper identities, name bytes and proof the map key is absent.
- The original producer/path that makes this state reachable; a test forcibly nulling a pointer is an intervention and must be labeled as such.
- Outer allocation result, dimension getter input/result, constructor/new-step result, and any selected role/metadata/backlink effects.
- Current main wrapper/step at the second lookup feeding RBX; if it remains null, the `0x140cfa4be` access address and exception record/first-chance context.
- Whether any relevant handler continues/throws/unwinds/terminates, actual cleanup/retention/publication state and final caller/process outcome.

A non-special fresh name minimizes role/icon/translation branches for the first fault witness; the ROI/Coupons prefix needs separate coverage if its partial effects are claimed. An already-existing key does not witness the miss branch. A standalone getter probe returning 3, a candidate exception, the same exit code, or a different main-step lifetime does not close these conditions. This investigation did not acquire that witness or request a GUI session.

## Compact reproducibility receipts

Native byte ranges are half-open and mapped through PE file-backed sections; hashes below were independently recomputed in this read-only audit:

| Range | Meaning | SHA-256 |
|---|---|---|
| `140cfa0e0..140cfa644` | Named creator unwind region | `30768786294b9d68ea4ab4fe2d2bc6a61b7170fe8d1527a519cdfd415f61f65a` |
| `140cfa14e..140cfa177` | Main/dimension/new-constructor calls | `5144c0aca2418f3bd362a3d411dc13027420a2508150de2d627166139befee9e` |
| `140c7fee0..140c7ff1f` | Default-3 dimension getter | `e8585aca967e35e147769c8f868d8817892a2e3a509676abc3444b1e62bcca0b` |
| `140c7a6b0..140c7a795` | New wrapper constructor region | `9a6cb8e26bead8ac6925d5236ac72bee65b82b86c4e96c07785bde0f15f05937` |
| `140c7a870..140c7a9f6` | Step allocation/initialization region | `715d6fb55b48322c22e88d4a12556924bb9a6c3b182c93dc1ccffcf3c01823bd` |
| `140cfa487..140cfa4cb` | Backlink/relookup/unchecked step-number load+store | `7909f384ac008e536001bfb2545b88b1a2afd54d671b169b2e2dfbb378c528e2` |
| `140cfa4be..140cfa4c4` | Six-byte source load at the predicted fault PC | `d77ecc417f507366d726b4112443cbee41a476f5935a22ce11dae6e00c6cccae` |
| `140cfa4cb..140cfa55d` | Later title/control operations | `90ca34788f608bb7776437519cb20a51884c7f69accb1d19ef0b176c2dba021c` |
| `140831650..140831669` | Direct role setter through RET | `943c8db1db08ef94fce7ede3a5d661ff209f027392540aada647848a644086ad` |
| `140c1aef0..140c1aefe` | Guard-storage pointer load through RET | `5bc1bfe5b72daf957f388c00813c416d4a2b8f719b9667d675ec3db8b1150732` |

Retained decompile SHA-256: `L4/vs/a.c` `4010f1fef9e8f49f4684dfd914bb3394d3b104552d8b7c4d5e15a752ffbc1fe6`; `b.c` `effbfab1305f015761fb522d9ae31d614adb00f39444768c0ecb04188f1f1683`; `c.c` `2764b5b73b935a5ec7c831fbf4a8567c20c11f43f3f49fb59eab016b242eec41`; `g.c` `128d0542263879945da3c039822b26a137a881868767c5655f3ff6d2dc6e2a96`.

The first role-setter DUMPBIN request ended inside its final store; it was discarded for boundary claims and re-read through RET at `0x140831668`. No decoding output was treated as a runtime event. The report is the only filesystem write; no original/source artifact or analysis database was modified.
