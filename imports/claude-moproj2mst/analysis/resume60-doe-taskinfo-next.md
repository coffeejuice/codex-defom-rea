# CP60 first active DOE import dependency and TaskInfo boundary

The next useful evidence action is the actual DBLIST helper1406554a0, not more TaskInfo value helpers. Retained source evidence reveals that earlier notes reversed DBLIST and ReportList helper labels. Preserve those historical artifacts; use the corrected mapping below for further work. The constructor-history projection does not advance either import branch or the native Report constructor.

Only this report was written. No source/shared files, builds/tests, GUI, original execution or mutable REA were used or changed. Original static evidence is qualified to GUI executable SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc; retained decompilation does not establish runtime state or close indirect calls.

## Correct the retained branch labels

analysis/wave2-doe-part-state-leaves-receipt.json explicitly binds1424e3cb0 to DBLIST and1424e3cb8 to ReportList. In function140650e60 from analysis/wave2-doe-session-valid-existing-input-decompile.json, body lines210-231 contain:

```text
cStack_308 = __8QString__QEBA_NPEBD_Z(tag, &UNK_1424e3cb0);
if (cStack_308 == 0) {
    // Not DBLIST: compare ReportList next.
    cStack_2d8 = __8QString__QEBA_NPEBD_Z(tag, &UNK_1424e3cb8);
    if (cStack_2d8 != 0 && FUN_1402cbec0(global143d766b8) != 0)
        FUN_140655df0(partInnerModel, copiedNode);
} else if (param_6 == 0 && FUN_1402cbec0(global143d766b8) != 0) {
    // DBLIST branch includes this additional mode condition.
    copyQString(param_3);
    FUN_1406554a0(partInnerModel, copiedNode, copiedQString);
}
```

Here the imported __8QString member is equality, so the zero branch means the DBLIST comparison did not match. The distinct helper arities and the extra param6==0 condition are material, not naming differences.

| Encountered direct child | Original helper | Additional condition | Arguments beyond receiver |
| --- | --- | --- | --- |
| DBLIST |1406554a0 |param6==0 and shared global gate |copied DOM node plus copied QString param3 |
| ReportList |140655df0 |shared global gate |copied DOM node |

Gate1402cbec0 is already captured: global143d766b8 dword+1c must be0. This is separate from SessionNotificationState.suppressed; no equivalence or candidate owner for that global field is established here. DBLIST's copied QString is the caller's param3. Its path/filename meaning is still unqualified; do not substitute SessionDocument.filename merely because the types match.

The earlier mapping in resume53-doe-dblist-seam.md and the derived labels in resume56-dblist-callee.md are superseded. Their raw disassembly of655df0 remains useful, but applies to the ReportList branch. The resume58-60 invoke/RTTI/vtable work tracing SfTaskReportGenerate and+218/+240 therefore qualifies that ReportList continuation. Its findings do not establish DBLIST behavior, and B127 DBLIST child shape cannot justify skipping the different6554a0 routine. No existing artifact is rewritten by this correction.

## Already implemented order versus first reachable missing effect

session_records.cpp visits SessionRoot child elements in XML order. DBLIST and ReportList remain one shared candidate refusal message, but the native branches are different and should stay independently guarded until closed. The CP59 active DoeOutput regression records the first encountered DBLIST/ReportList refusal after the retained2013-byte.DS read and three preceding simulated sample-file writes. The earlier B127 probe in resume53 identifies the exact first child as DBLIST, with a retained part/root, zero Session records and no current selection. This is a candidate observation, not original runtime acceptance.

Therefore the first missing import effect on that retained active path is the part-level DBLIST effect of1406554a0. Its immediate native receiver is the part inner model param1 of650e60 and its direct child DOM node. SessionPart is a candidate abstraction for that scope, but no destination field or ownership transfer is yet proved. A SessionRecord, selected SessionTask, TaskInfo or FlowReceiverWitnessRegistry cannot stand in for it. Actual gate state and copied third-argument context are also required.

Other fixture shapes can reach Session first. Existing Session import already preserves metadata/Image/UserDefinedStepList before retained allocation; factory return before append/current index; append before named-model clear; repeated SfTask children at encounter. Task Opr/Obj readers already preserve per-list clears, converted values and encounter order. Populated OutputList is separately guarded. After all imports return, entity0 selection precedes ChangeStepRange; ordinal0 fallback follows that notification; only selected currentSession resets to-1 before Open/Chapter/Count notifications. Do not move these effects ahead of the earlier DBLIST refusal or rebuild them using receiver history.

## TaskInfo is still behind an independent earlier constructor

Native task-factory initialization still commits factory/data and retains first Report allocation before refusing the first Report QObject(parent=null), before TaskInfo. The new constructor-history projection creates no Task and no registered complete population. Existing preference, first/default-font, second-font-assignment and private+20 bit helpers are isolated values only.

The prerequisite order remains: Report QObject private allocation/shared-null effects and backlinks, actual TLS-selected QThreadData+0 ref, QObject counter; then TaskInfo wrapper/private initialization, first current-font value with local mask0, icon/colors/flag, second current-font value with its own mask, and wrapper+8 publication after success. Correct QtCore RVAs from resume52 remain2aca0/29600/2acbc for current-thread getter/TLS initializer/TlsGetValue call. No static branch-label correction supplies runtime shared-null counts, TLS values or actual current-font observations. Keep the Report guard unchanged.

## One minimal next action

Perform one bounded immutable-original static capture that verifies the650e60 DBLIST/ReportList branch bytes and disassembles the actual1406554a0 entry through its PE unwind-bounded region. Use unique resume61 names and preserve the executable hash; no original process or mutable analysis DB is needed. The controlling question is: what first effect does6554a0 perform, on which owner, using its copied QString, before its first unresolved call?

The receipt should distinguish the additional DBLIST mode condition from the gate, identify the first state store/allocation/dispatch and its exact receiver/argument provenance, and stop at the earliest unresolved callee. If it is not effect-free for the retained one-DBNAME shape, retain every qualified prefix effect; no no-op DBLIST implementation is authorized by ReportGenerate's unrelated OutputList shape check. Obtain the full logical extent only if unwind metadata shows chaining/splitting; an unwind region alone is not proof of a complete procedure.

Proposed ownership: evidence-only lane owns analysis/resume61-doe-dblist-real-handler.md plus uniquely prefixed immutable asm/receipt files. Existing session_records/model/Session interfaces and public guards remain root-owned. Once the first6554a0 effect and gate/context owner are closed, propose the smallest corresponding isolated helper and focused test; there is not enough evidence in this audit to assign a production state field now.

## Receipt

UTC: 2026-10-24T21:08:20.339710+00:00. No executable verification was run.

| Retained evidence/current source | SHA-256 |
| --- | --- |
| `analysis/wave2-doe-part-state-leaves-receipt.json` | `ea2cbdabb05ea6fc90ef3e82d023a729d4629df1bce2f3531ed87f3ae67cc793` |
| `analysis/wave2-doe-session-valid-existing-input-decompile.json` | `e1e3598d484e64e286ab36a5fa11d11aa598a6d25efcd316a671d2e38a743c54` |
| `analysis/resume56-dblist-gate.asm` | `7013415e9d21da05a44baa721f4c5d49070aa5faaa494bdaa6b48badcdf7c036` |
| `analysis/resume56-dblist-handler.asm` | `0c87637089cd61388f050296f700aed4df8e1416176737bf78d2eb0842af3b7a` |
| `analysis/resume53-doe-dblist-seam.md` | `30379170dd260cf74b641bb6a5d00ef4a2a2dac65f547045558a10d0909d7f07` |
| `analysis/resume60-dblist-signal-next.md` | `e74cb8ae603d5ae04ed2f96a0aba30867b068b267acc8896eef9dcca3a44bb2c` |
| `analysis/resume59-doeoutput-active-regression.md` | `490c061038af2b6b2a07380f93ce29b52d97dac065049adf271609c55983ea99` |
| `analysis/resume59-doe-projection-implementation.md` | `d69e83ca09b8cc0ede177368dcd87ba2ff992b5a467840b6e34350f0f2e00105` |
| `analysis/resume50-doe-taskinfo-binding.md` | `576859764a6c123181424f1e33a34103bb393350d8a068b06d9342339c0de839` |
| `analysis/resume52-doe-report-next.md` | `d5c98bbe0a6799780c33356917c528587059d464703786f0bffa064b7a4133b9` |
| `source/apps/moproj2mst/src/doe/session_records.cpp` | `8e8410f7fb4596bc62e752b02969208e51c9cfcfab402ba72727885bb3f50794` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `88a0a355496a69d515d59affb01e4543b8ac7206cd1764a8302e1d6130830d7d` |
