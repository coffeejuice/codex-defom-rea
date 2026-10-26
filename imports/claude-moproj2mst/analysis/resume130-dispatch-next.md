# CP130 operation dispatch / Save gap audit

## Decision

No disjoint source-ready public Save path was found after CP127. The ordinary Type2 Study Save has already advanced through owned Constraint XML, then the CP125 notifier suppression/`flag90` prefix. The next instruction is process-dependent key construction and actual receiver dispatch. Report and ShapeOptim Save have independent document/optimizer owner gaps; active DoeOutput Open can stop earlier at the DBLIST/ReportList document branch. No alternative operation class lets the Save pipeline safely jump around these ordered boundaries.

## Disjoint branch check

The current dispatch inventory is bounded as follows:

- The regular class `vt+0x68` Save and property `vt+0x60` dispatch are implemented for the source-closed families. Step-4 property writers (Cogging, Boolean, L6 and Express) are already present; Boolean's step-4 side-file call remains distinct from its later MST call. There is no new supported generic dispatcher arm to add.
- Type2 Study Save's empty-variable Constraint XML serializer is already integrated at `src/doe/type2_xml_prefix.cpp` and runs after Operations against the concrete Study-owned records. It preserves immediate file effects and stops later at DoeTableRefresh.
- The actual DoeTableRefresh notifier's unsuppressed `flag90=false` write is already integrated in `src/doe/table_owner_state.cpp`. CP126 establishes that key creation follows it and that the invocation-time codec/data owner is not represented.
- Report `vt+0x68` is not a detached report-writing opportunity: it needs the actual `SP+0x30` report document and viewer-model sequence. ShapeOptim's `vt+0x80` Save likewise needs its Type3 optimizer/objective owner and ROI model. `analysis/resume88-operation-dispatch-inventory.md` and `analysis/resume122-doe-report-owner-frontier.md` keep these independent guards.
- Active DoeOutput Save (`vt+0x68`) is downstream of a successful active Open. On the retained B120 case, active DoeOutput Open reaches the direct DBLIST/ReportList guard first. A DoeOutput Save implementation cannot be made reachable by adding a post-refresh writer or assuming that Open returned; the DBLIST global owner and selected Session/task application are still separate prerequisites (`analysis/resume123-doe-output-next.md`).

The immediate public Save blocker is therefore not a missing file writer or static method mapping. It is the transition from the already modeled notifier scalar prefix into a specific live Qt receiver occurrence.

## Smallest decisive refresh witness

The smallest useful witness is one same-process, same-Session project Save trace that stays continuous from the current native dispatcher call to its return. It must bind, in order:

1. **Notifier and key conversion:** actual dispatcher arguments at wrapper `0x1400d369d` (global notifier `0x143d76700`, literal key `2signalDoeTableRefresh()`), suppression byte `+0x141`, and post-reset `flag90 +0x90`; at QtCore constructor path `0x67174bd0`/`0x67074120`, capture the invocation-time `codecForCStrings` value at `0x67074126`, its owning process/module generation, and whether the completed QString was published to the source slot. This resolves the distinction between the conditional 80-byte fresh/null branch and a nonnull actual decoder/data owner. A process-global zero initializer, unrelated key construction, or explicit test Null does not substitute.
2. **Actual dispatcher collection:** at map lookup `0x1402d03f0` (called from `0x1402cf454`), record whether the real key is present or inserted, the actual QMap data/ref owner, the returned ordered receiver list and its local snapshot/COW state. The native initial shared-null table or fixed table-constructor registration is not the Save-time collection.
3. **Receiver and normal return:** bind the actual live guarded `SfMoDoeTable` generation published at main-window `+0x458`, its nonnull `QTimer` at `+0xb0`, slot entry `0x1400f4e10`, the nested refresh body `0x1400f47e0`, and normal slot, dispatcher, and wrapper (`0x1400d36a2`) returns. A constructor signal connection proves a potential producer; it does not prove this registered receiver was live or invoked in the selected Save.

A trace that stops after step 1 makes key construction conditionally source-ready but still cannot advance public Save. Steps 2–3 are the owner/callback dependency that controls the actual branch.

The evidence request is specific and bounded: collect the above at one actual normal Save occurrence, including relevant identity/generation values and whether the callback returned. No GUI/original-process observation was performed or requested from this lane.

## No-bypass rule and next ownership

Do not add a new fixed-key QString helper, seed a Null codec profile, infer a present-empty map value, synthesize the table registration from static constructor code, mark the timer stopped, or call PAR/sample writers before normal dispatcher return. Each would skip an unresolved state owner or reorder native partial effects.

There is no CP130 source/test ownership to assign. The existing tests cover the CP125 scalar prefix and partial effects. When the witness is available, root must first own a canonical process/Session/notifier/map dispatch context; a subsequent DOE lane can own the narrow dispatch/callback implementation and a focused test on disjoint files. If work is needed before that, continue separate evidence for the exact DBLIST owner or Type3 ShapeOptim owner, not a source bypass.

## Scope

Read-only comparison of current Save/operation dispatch source, the CP127 inventory, CP124/125 implementations, CP126 refresh-key evidence, and the existing Report/ShapeOptim/DoeOutput boundary reports. No source edit, build, GUI, original execution, or mutable REA action was performed.
