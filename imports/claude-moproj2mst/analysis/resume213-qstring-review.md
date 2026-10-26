# CP213 independent QString review: PASS (bounded reader only)

The exact copied QtCore artifact supports the proposed layout. The final frozen reader passes this bounded static/actual-code mock review. This is not original-process runtime validation and does not by itself release an original-process run. No original process, GUI, build, source, deployment or shared recovery state was changed. Only this report was written. Applied the reverse-engineer-anything skill's distinction between copied-PE facts, synthetic validation and runtime evidence; no REA session was acquired.

## Artifact and layout: PASS

Independently parsed the copied PE section table and read getter bytes directly from `evidence/original-binaries/QtCore4_64.dll`. SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, file size 3,250,688 bytes, machine AMD64 `0x8664`, timestamp `0x517180cf`, preferred image base `0x67000000`, SizeOfImage `0x31f000`. Independently hashed installed `C:\Program Files\SFTC\DEFORM\v11.0.2\QtCore4_64.dll`: same digest and length; installed FileVersion `4.8.4.0`, ProductVersion absent.

| Getter | RVA | PE file offset | Exact bytes |
|---|---|---|---|
| length / size | 0xfec0 | 0xf2c0 | 488b018b4008c3 |
| capacity | 0x2e90 | 0x2290 | 488b018b4004c3 |
| unicode / constData | 0x47940 | 0x46d40 | 488b01488b4010c3 |

Export aliases agree with the retained export table. These entry-through-RET bytes prove the QString object's Data pointer at +0, capacity at Data+4, length at Data+8, and character pointer at Data+0x10. They resolve the earlier reversed +4/+8 prose. The destructor and fromRawData/setUnicode bounded assembly support the ref field and external-buffer allowance; fromRawData's Data+0x1a zero write does not establish a terminator after the external content. Neither header structure nor readable memory proves heap liveness.

## Synthetic evidence: PASS with limits

Reran `python analysis/resume213-qstring-layout-fixture.py`: 17/17 pass, matching its retained receipt and fixture SHA `d3957d0b70bf9a81cc1a0d052bdd80b334026d09b522dfa849fc7c3fd15bdf38`. The cases meaningfully distinguish length and capacity, enforce the positive-ref trial policy and 128-unit cap, preserve embedded zero/surrogates, allow external unterminated data, and reject unreadable/partial/changing state. Header and object mutation callbacks fire at the intended rereads. These are actual Python assertions under the invoked normal interpreter; `python -O` must not be used.

The 17 cases do not cover negative capacity separately, null Data-header pointer, unreadable/partial object or header, cap boundary 128, multiple readable ranges, mapping/protection races, unchanged-header content mutation, or ABA. The model never claimed those cases were covered. It is not evidence that Frida's exact API or an original live object works.

Reran `node admin/test_encoded_open_frida_fixture.mjs`: PASS for the actual generated agent and host mock. Added ephemeral in-memory fixture probes for ref 0, ref -1, capacity -1 and capacity 0 with length 34: all rejected. No fixture file was edited. The current stock mock covers six reader failures but not all 17 Python cases or module identity ambiguity/partial metadata reads. Its pointer addition uses unbounded BigInt, so it cannot validate native 64-bit wrapping semantics.

## Final frozen generated reader: PASS

Reviewed frozen include SHA `c3713e3f6cd53c39370ed69cb17e587825ef1bde8ec66472532d1f9a0c1e6122` and generated runner SHA `57d7877d0ebf0065350b22a87d2931c4b745be75175fd03b2a1651916a21feba`. Both hashes were independently checked against the orchestrator's freeze. Subsequent edits need focused rereview.

The final reader qualifies the installed DLL's host-side SHA, unique loaded module name/base, exact path, image size, DOS/PE signatures, AMD64 machine, PE32+ magic, timestamp and SizeOfImage, then verifies all three getter byte spans at loaded-base-relative RVAs. This is a bounded loaded-image qualification, not proof that every loaded byte matches disk. Loaded base is not confused with preferred base. Metadata reads execute before hooks are installed; a read error blocks readiness.

Object and 26-byte header are read into exact-length copied buffers; all fields derive from those copies, without converting 64-bit pointers through JS Number. The reader rejects null/unreadable/partial/wrapped ranges, nonpositive refcount, invalid capacity/length, odd character pointers, and lengths above 128. It deliberately adds eight-byte object/header alignment as a conservative policy. Every crossed readable range is checked before the exact guarded copy. A race can still invalidate a map precheck; caught read failures remain failed witnesses.

Little-endian content bytes produce authoritative units and bounded display text without NUL scanning or target-side Qt calls. The reader records raw header/object bytes, flags, storage kind and UTF-16 hex. Empty strings skip the content read but still undergo header/pointer policy and rereads. External buffers need no inline relationship or terminator. Header/object rereads must match; failed witnesses return `units:null,text:null` and remove candidate content hex. Refcount and harmless padding changes conservatively fail. Matching rereads cannot detect ABA or all content-only mutations and do not establish heap liveness or an atomic snapshot.

Host witnesses compare every expected unit at outer entry/return and acceptance entry/return; acceptance retains raw RCX/RDX and requires RDX equal the outer object. No NativeFunction/SystemFunction or getter invocation exists in the reader. No full Open claim follows merely from a reader witness.

Reran the frozen actual-code mock: PASS. Its reader failure matrix now includes capacity 33/-1 and ref 0/-1 in addition to negative/129 length, null/unreadable/partial content and header race. Also ran seven ephemeral in-memory extensions against the exact generated agent: null header, odd data, partial header, partial object all fail with null units/text; empty cap/length zero, 128-unit content, and embedded-zero plus paired/unpaired surrogates succeed with exact units. These modifications existed only in the test process; no runner or fixture file changed. The stock mock pointer addition uses unbounded BigInt, so native-pointer wrapping and real Frida API behavior still require native fixture qualification.

The first review's material findings (unique loaded-module/metadata identity, exact object/header returned lengths, copied-byte field parsing, capacity/ref/alignment policies) are resolved. Single-range conservative coverage rejection is also resolved by the final range iterator. Earlier failure-output/storage diagnostics were repaired.

Two diagnostic qualifications remain nonblocking: over-cap witnesses are explicitly incomplete (`ok:false`, `rawReadStatus:failed`, `truncated:true`) but their bounded error text is the generic invalid/excessive length/refcount/capacity message rather than a dedicated `length-cap` status; map/read outcomes are represented by success/failure and bounded error text rather than separate fields for every stage. These do not turn an incomplete snapshot into success. A failed reread's `recheckStatus` may remain `not_reached` if the read throws, with failure retained in `error`.

Keep the original execution guard until the orchestrator's separate runner, native ownership/endpoint and harmless native fixture gates are satisfied. This PASS is confined to the independently verified layout and frozen bounded-reader implementation. No original or GUI session was acquired, and no release of GUI control is claimed.
