# CP130: retained codec observation and the missing refresh occurrence

Retained original runtime evidence does observe a null C-string codec, but only during a different CLI startup/Open/editor occurrence. It does not bind the DOE Save dispatcher key constructor or its returned QString data. No production increment after CP125's flag clear is source-closed. Keep the refresh key/receiver guard. The useful next action is one narrowly correlated natural key-construction observation, reusing an already retained raw codec-reading probe fragment; another static import/global scan or isolated Null helper will not resolve this boundary.

Only this new report was written. No source/test/build, original execution, process enumeration/read, GUI/control, live Frida/REA session, mutable analysis database or retained probe execution occurred. Retained scripts/captures were read as files and their hashes verified.

## New retained evidence found

`analysis/wave3-original-editor-probe-002-receipt.json` binds original PID30116, native CLI startup Open and the original GUI/QtCore hashes. QtCore was loaded at0x54570000 with SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; original GUI hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its `build/original-editor-probe-002/capture-evidence.json` hashes to `f575f96ac894da56b776e2bf8388aaff6f5aa30ed65e8906c133b6b891f96dce`, exactly matching the frozen raw-files receipt.

The capture's structuredContent.normalized_result.messages records cStrings.pointer="0x0", vtable=null on actual TID28252 at four stages:

| Message index | Payload kind | Sequence |
| --- | --- | --- |
|36 | editor_qwidget_constructor_return |18 |
|38 | setup_entry |20 |
|40 | name_setter_return |22 |
|53 | size_grip_setter_return |35 |

The hash-verified retained `build/original-editor-probe-002/probe.js` lines191-196 implement `language()`: read current QtCore module base+RVA0x2f3e10 directly, then, only if nonnull, read the pointed object's vtable. For that module base the C-string pointer field is0x54863e10. Locale and translation codec fields are read separately at RVAs2f4438 and2f4440. Thus this is genuine raw C-string state evidence, not an inference from Windows ACP or locale codec class.

`analysis/wave3-editor-probe002-prefix-reconciliation.json` independently retains the observed null CStrings/translation fields and explicitly limits them to those CLI checkpoints. `wave3-close-splitter-probe-reconciliation.json` also has null C-string observations at startup stages in another process; it is not this Save occurrence. `wave3-editor-observed-locale-codec-identity.json` identifies QWindowsLocalCodec from a locale-codec vtable and cannot substitute for C-string state. The older build_L4 test_codec executable/host codec-boundary tests likewise provide no original Save call frame or data owner and were not executed here.

The Probe002 receipt says CLI startup Open precedes main show/event loop, partial observations do not prove Open/Save/Close completion, and the owned process was terminated with absence verified. No event in these four observations is0x1400d369d,0x1402cf41f, the actual key constructor return, a DOE table receiver or a Save normal return. The capture does not cover every later codec write/event in a future process. A historical null pointer cannot be promoted to a live canonical process field for a new Session/Save.

## Exact unresolved edge, without another static loop

CP126 remains the controlling static contract (`analysis/resume126-doe-refresh-key.md`, SHA `BE38149AC49D3CD8C2B3F7AFABFFB08E79BA70FDE3909CEC8E79660AAD5C2024`). After actual notifier flag90=false, the dispatcher constructs a QString at source slot rsp+0x98 at0x1402cf41f using GUI IAT0x141e04480. Its input is the distinct24-byte `2signalDoeTableRefresh()` literal at0x142166b90. Constructor QtCore+0x174bd0 calls fromAscii_helper+0x74120; the codec comparison is the **instruction at RVA0x74126**, reading the **data field at RVA0x2f3e10**. Do not confuse that code address with the global pointer address.

A qualified null read selects fresh80-byte allocation/ref1 on normal constructor return for this key. Nonnull selects the actual decoder and requires its returned owner; fixed ASCII content does not supply that owner. Unknown process/codec remains guarded. CP82's isolated36-byte icon-resource/104-byte owner cannot represent this distinct key occurrence, and the current actual Session has no bound production process-codec context.

Even a completed key still precedes actual QMap detach/lookup/default insertion, native local QList reference/snapshot and ordered guarded registrations. CP126 identifies the possible SfMoDoeTable receiver, actual child timer stop and normal slot/dispatcher return gap. Current Session scalar state and table diagnostic receipts provide none of those later effects. Do not call PAR writers, insert an empty key or manufacture no-live-target delivery from the old codec observation.

## Smallest retained-probe-based observation contract

The retained `language()` fragment is a bounded raw-read mechanism that can be reused in a separately coordinated observation, not a completed source owner. The smallest useful new evidence stops after key construction, before map/list/Qt receiver work; it need not capture a full GUI workflow or full refresh.

1. Bind one naturally reached Type2 Save after XML normal return to a process/module generation, TID, original notifier identity and exact dispatcher occurrence. At the0x1402cf41f callsite retain actual source key pointer/25 bounded bytes including NUL, destination QString slot, call frame and source site. Correlate on this actual thread/call; do not select every unrelated QString constructor.
2. At that constructor's naturally reached QtCore+0x74126, read module-base+0x2f3e10 directly, using the existing helper's module/hash discipline. Do not invoke a codec getter, set a codec, seed Null, pump events or call the decoder manually. Preserve the reached branch and raw read stage. A before/after asynchronous sample from an unrelated running thread does not bind the comparison.
3. For null, pair it with natural normal return to dispatcher0x1402cf425 and the same destination slot's data pointer. Retain initialized data fields/ref count, size/allocation24, payload pointer and bounded UTF-16 key/NUL. Preserve unknown flags/padding. Allocation exception or absent return is an incomplete occurrence, not a published key. A nonnull result needs the naturally used decoder/returned data owner; otherwise stop there.
4. End this bounded witness before2cf454 map lookup; retain cleanup/process-lifetime outcome through the observation framework. It supplies key construction evidence only, never map/list population or callback success. The earlier flag store and XML effects remain even if key construction fails.

This report neither requests nor starts such a runtime session. The existing oracle_b `_tools/cwd.py` has a ReadProcessMemory helper for PEB current-directory reads, but no source-PC/TID/call correlation; simply repurposing it to sample the global would not satisfy the witness. The existing `probe.py` is GUI automation and was not run. Any future observation must follow root's existing ownership/control/cleanup workflow and preserve uncertain/interrupted outcomes.

## Decision and exact next dependency

No source files or isolated helpers are proposed. The exact new input needed is an invocation-bound C-string codec read plus this key's normal constructor/data publication (or failure), or a reconstructed canonical process producer with closed initialization and intervening writers on the same context. The recovered old null observations can qualify their own historical editor profiles only. Once that input exists, root can assign an actual process/Session binding and reuse the existing canonical QString data/ref mechanics for this distinct key. The later map/list/receiver/timer return remains independently guarded.

If no such new witness/producer is available under current constraints, mark this refresh-key branch evidence-blocked and choose another independent actual-owner reader/output branch. Repeating the PE zero-fill, language getter, import binding or Qt handler inventory cannot establish a new invocation's state.

Inspection SHA-256 anchors: Probe002 receipt `D7EB93FADACF6323686B69BA2806D6918D517E6BECB79913FBC0CED904765868`; retained probe.js `55E7FE8D67C7B15485A7ABE5FF9A1A519CFB8A7E3D644BC9C41365DFA9D9361F`; Probe002 reconciliation `C48F2DAED6509D571C9895FC6F2D53DADCEBA5036B12C1611939DA3F1533616A`; earlier codec observation contract `53AB3E0BF3D16791046F6032E737A1944F311D8A9A84DE47C90F766733609706`. These are retained file identities, not a new runtime execution or public completion claim.
