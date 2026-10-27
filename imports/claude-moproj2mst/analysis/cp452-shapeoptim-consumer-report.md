# CP452 ShapeOptim consumer: private projection is source-closed on prepared values

The CP451 `Type3OptimizerDefOptView` can support a private, in-place, normal-path projection of the optimizer-present prefix of original `0x14013d740`. This does not justify attaching the prepared state to `SubProject` or relaxing public Type3 admission. The useful advance over CP149 is an original-PE-bound consumer prefix, the exact fallback append arguments, and the distinction between fields reset and fields retained across calls.

`cp452-shapeoptim-consumer-byte-verification.json` binds five bounded assembly exports to retained original `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`: 510 addressed instructions, 2,586 compared bytes, zero mismatches. It includes artifact and raw-range hashes, source hashes, range ends, and method. The prefix ends at `0x14013de2c`, immediately before property traversal. All four helper captures end at RET. `cp452-shapeoptim-consumer-verify.py` reproduces the PE section mapping and comparisons without executing the original. Dumpbin tool banner is 14.44.35228.0.

## Scalar and derived state, in order

The getter targets and native owner offsets are from CP450; the destinations and ordering are confirmed by the new consumer prefix.

| Call | CP451 prepared value | ShapeOptim destination |
| --- | --- | --- |
| vt+138 | value38 | int +cc |
| direct 14091e0b0 | value0c | int +d8 |
| direct 14091e0f0 | value10 | int +d4 |
| vt+128 | value34 | int +d0, then byte +e8 = (+d0 > 0) |
| vt+118 | value30 | int +c0, then derived int +c4 |
| vt+28 / vt+30(index) | ordered numConRows | row loop below |
| vt+c0 / vt+d8(QString-out,index) | ordered regionRows.value08 | append each name to +f0 |
| vt+148 | value40 | double +e0 |
| vt+158 | value48 | byte +e9 |

The +c4 computation uses signed division/remainder, as confirmed by `cdq/idiv` in the native prefix. Let x = +c0. If x >= 10000, use x % 100. Otherwise, if x % 1021 < 3, use that remainder; otherwise, if x is 1055, 1056 or 1057, use zero; otherwise use x % 10. This is not positive-modulo arithmetic: negative x can satisfy the first remainder comparison and produce a negative +c4. +c8 is not written by this prefix.

## Row tag, ordered overwrites, and append values

The loop captures row count once, increments a signed index from zero, passes that index in EDX to vt+30, and immediately dereferences the returned pointer. It dispatches on `row+4` (`Type3NumConRowValues::value04`), not classifier result `row+0`. CP450's physical node layout supersedes older CP131 offset prose. Row +10 and formatted QString +20 are never consumed here.

Before the row loop, only bytes +98, +99, +9a and +9b are reset to zero. The other checkbox bytes, scalar limits/IDs and append lists retain incoming state unless written by a row. For each row, in input order:

| value04 tag | Writes |
| --- | --- |
| 0x3f2 | +98=1; +74=value0c; +9d=(+74>0); +80=max(value08,1) |
| 0x3f4 | +99=1; otherwise same shared +74/+9d/+80 stores |
| 0x438 | +9a=1; otherwise same shared +74/+9d/+80 stores |
| 0x439 | +9b=1; +78=value0c; +9e=(existing +74>0); +84=max(value08,1) |
| other | append value04 int to +a0; append value18 double to +a8; +70=value0c; +9c=(+70>0); +7c=max(value08,1) |

The 0x439 test of +74 is assembly-confirmed at `0x14013dc43`; substituting +78 would change the original behavior. Thus a preceding special row or seeded +74 affects +9e. Duplicate special tags overwrite their shared scalar fields; flags accumulate after the four-byte reset. Fallback rows append in order, with later fallback rows overwriting +70/+9c/+7c.

The fallback correction is concrete: `0x14013dcaf..dce4` passes `(OB+a0,row+4)` to `140025100`, then `(OB+a8,row+18)` to `140140810`. The decompile loses the second call argument, and CP149's phrase 'string-like value' is incorrect. Both append helpers check Qt sharing, then append a native list slot; direct node constructors `140025580` and `140140bd0` select constant normal branches that pass copy lengths four and eight respectively to `14146f100`. Their bypassed branches also show int and movsd element loads. The prepared projection therefore uses ordered int and double values. This closes value types/call arguments, not Qt allocation/detach/unwind parity or a fresh audit of the general byte-copy routine.

The current shared `TaskBlock60::f98[6]` covers only +98 through +9d. A private projection state must represent +9e as well; do not index that existing six-element array at six. Its `listA8` stores uint64_t values; a private value projection may use doubles, or explicitly preserve their bits if mapping into that representation. This report does not establish that public TaskBlock60 layout changes are required now.

## Region names and retained-state effects

CP450 binds vt+c0 to owner+20 list count and vt+d8 to a signed-index getter of region node QString+8. Invalid indices produce exact `Selected Object`. The new consumer confirms the index in R8D, caller QString output pointer in RDX, and pass of that output to `140004b90(OB+f0,QString-out)` before destruction. It retains input ordering and duplicates; no clear or deduplication occurs in this prefix. The same absence of clear applies to +a0 and +a8. Repeating the prefix against a nonempty seeded state appends again. A helper that constructs a fresh result while unconditionally resetting all fields would represent only an additional selected profile, not this in-place contract.

## Side effects and failure prefix

The original returns without this projection or property tail when the current item's project or active SUB is absent. A null optimizer skips the whole optimizer prefix and proceeds to property traversal; it does not reset the four flags. An optimizer-present normal path writes scalars, four flag resets, rows, region-name appends, and finally +e0/+e9 in the order above. There is no transaction or rollback in the captured prefix. A failure in a row getter/append or region getter/append occurs after earlier writes/appends; no success/error value is returned by this void consumer. Native row access is not checked for null after the getter. CP450 documents that valid index access can detach Qt storage and allocation failure has no new safe null handling before consumer dereference. Do not invent a safe native failure return, or claim a private std::vector exception prefix matches Qt.

The remainder of retained `evidence/gui_mo/s1/mdt/L7/d0.c:1131-1240` dynamically casts the operation's props, traverses property nodes, logs names with qDebug, updates StopCritGroup MaxSims/MinObjFuncVal from +cc/+e0, and finally invokes nonnull +b0's vt+58. These are ordered after the prefix even when the optimizer is null, provided project and SUB exist. This tail is retained decompile evidence and current reconstruction, not newly instruction-by-instruction bound here. A private value helper should end at the +e9 store and leave all props/log/weak-pointer work outside its advertised scope. No GEO I/O occurs in this selected prefix.

## Smallest next implementation and tests

Implement only a private seeded projection state and a helper taking `const Type3OptimizerDefOptView&` plus that state, applying the optimizer-present normal prefix above. Keep it unreferenced by public Type3 Open/Save. An optional-null wrapper may preserve state for no optimizer, but project/SUB/property behavior is outside the helper. State includes exactly the destinations above, seven bytes +98..+9e, and ordered int/double/name lists. Use counts representable as signed int32 and valid stable prepared vectors. It should not read row value00, value20 or fabricate native row +10.

Focused meaningful tests: unequal classifier/tag values; all four special branches and fallback int/double appends; shared-field last-write ordering; the 0x439 +74 dependency with preceding and seeded values; +08 clamp below/at/above one; negative and threshold +c4 arithmetic; untouched +c8 and absent-row values; reset only four flags; names and duplicate names preserve order; two calls append twice. Test the private contract, not native error parity.

## Exact remaining integration gates

1. Actual owner publication through SUB+28, aliases, teardown/disposal and reopen/failure lifetime must be reconciled with the already recovered concrete DefOptModel constructor/getters. CP149's old statement that no constructor/vtable/getter bodies are retained is stale after later checkpoints, especially CP450. The prepared view has no claim to that ownership edge and cannot alias Type2's separate SUB+20 DOE manager.
2. Native .OPT file open/codec, streaming failure/partial mutation and all admitted records must be connected to the owner. A prepared-value parser/view alone does not establish production file behavior.
3. Existing `ShapeOptimClass::loadClassData` Type3 refusal at report.cpp remains. The helper does not authorize removing it or subproject parser admission guards.
4. `shapeoptim_load.cpp` loadData performs study-folder attempt before the `listF0_` nonempty GEO refusal. Per-name paths, 1-based ROI insertion, object name/kind and geometry reader `1408f3840` remain unreproduced. Do not wire projected names around that guard.
5. saveAfterFiles first copies property state and gathers nonempty ROI IDs, then refuses the Type3 optimizer update. The optimizer setter/update and .OPT Save behavior remain separate. saveOperationFiles computes/commits study-folder effect before refusing per nonempty ROI object GEO writer `1408f4760` (mesh-kind selection also matters). Keep both guards, including empty/nonempty distinctions and earlier visible effects.

Read-only recovery except these new `analysis/cp452-shapeoptim-consumer*` evidence files. No source/test/build/plan/state edits, executable builds, original execution, GUI input, app-control acquisition, or mutable shared REA/Ghidra project operations were performed.
