# CP114: native feature-map constructor and retained unknown state

The first Project+60 constructor dependency is now bounded through its normal map/sentinel initialization and its proxy unwind action. This establishes a conditional native owner prefix; it does **not** establish actual candidate heap identities, allocation-handler outcomes, full feature contents or Open registration. A full zero-initialized context would be wrong: the inspected constructor helpers leave the two PST lookup bytes untouched. No detached owner/helper/test was added because that would presently substitute invented heap/default state for the missing actual producer.

Original retained DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All new work is offline PE text/.pdata/.xdata/literal inspection through DUMPBIN14.44.35228.0 and Python reads. No original/native target, GUI, live provider, REA import/mutation, build or test executable was run. Only uniquely named analysis artifacts were written.

## Exact map producer

Let C be the actual inline feature context at Project+0x60, and M=C+0x1458. Project constructor `140089917` calls `1412f10d0(C)`; its first call at12f10e4 is `1412f1060(M)`. The bounded map constructor does this in source order:

1. At12f1076 request **8 bytes** through `14146eabc`. On its normal nonzero return P, write `[P]=M` at12f1085, then publish M+0=P at12f108c. This allocation is a separately owned proxy/backpointer, not the sentinel and not the entire context.
2. At12f1092 call `1412edac0`. That helper requests **0x30 bytes** through the same allocator. Its nonzero normal return H receives null qword links at H+0,+8,+10, byte H+28=1 and byte H+29=0 before helper return.
3. Publish M+30=H at12f1097. Set H+29=1 at12f109b; set H+8=H, H+0=H, H+10=H in that exact order; finally store qword M+38=0 at12f10b6. Return M at12f10be.

Successful state therefore includes proxy P with backpointer M, M+0=P, M+30=H, M+38=0; H links self, H+28=1, H+29=1. Other map/node bytes are not zeroed by these instructions. In absolute context offsets: proxy C+1458; head C+1488; size C+1490; independent signed cache C+1498 is written only later by the outer context constructor. A generic empty STL map is not evidence of these pointer identities, byte values, publish order or ownership effects.

## Allocation and failure are distinct effects

`14146eabc` normally returns **only a nonzero allocation**: call147489c(size), test RAX, return if nonzero; otherwise call1481ce8(size), retry allocation if its result is nonzero, or enter lazy bad-allocation exception construction/throw through globals `0x1446ffb20`/`0x1446ffb38` and146eb58. The intervening allocator/new-handler/global/throw behavior is not ported or supplied by a qualified candidate producer here. Caller-side null tests do not authorize inventing a successful null-allocation profile. In particular, sentinel helper12edac0's raw null path would proceed to a write through address8 before return, rather than return a valid empty owner. It must not become a safe null-head constructor.

The map constructor has source EH cleanup absent from a mere happy-path field list. .pdata identifies unwind RVA342c2cc, flags2 and C++ handler14146e20c; FuncInfo magic19930522 at RVA2db6df0 has one unwind state. Its IP map is:

| IP | State |
| --- | ---: |
|1412f1060| -1 |
|1412f108f|0|
|1412f10be| -1 |

Unwind state0 maps to action141b9c670 (to-state-1). That funclet retrieves the actual map argument and calls12f5e30; the leaf loads `[M]` (proxy P) and tail-jumps to14dfc68. No null store to M+0 occurs in those bodies. Thus a sentinel-allocation throw after proxy publication can release the proxy through the native cleanup action while leaving the failed-construction slot value in place; it does not initialize the later outer context fields. Exact allocator/free/new-handler/throw semantics, including external/custom handler state, remain dependencies. Do not add rollback of unrelated bytes, replay proxy allocation or claim a successfully constructed map after this failure.

## Post-map context fields, with untouched bytes retained

Only once the map constructor returns does12f10d0 perform the following conditional normal-return initialization. EDX is set0 and RCX=C before12e6fb0; the three bounded reset leaves preserve both, so subsequent12e6ff0/12e7020 use the same C and DL0.

| Context offsets | Source-established value/effect |
| --- | --- |
|+1498 DWORD|signed-1, after map construction|
|+3c DWORD,+40 DWORD,+44 DWORD|0 at their respective source positions|
|+00..+0f|0 from the first and even/odd reset leaves|
|+12..+30|0 from the same leaves|
|+34..+37|DWORD0|
|+38..+3a|0|
|+48,+c48,+1048,+848,+448|calls146de38 with the literal at142146f86, whose first byte isNUL; these are empty-string initialization calls, not whole-buffer zeroing|
|+1450 qword|0 at the final outer-constructor store|
|**+10,+11**|**untouched by these constructor/reset bodies**|
|+31..+33,+3b, string-buffer tails, +1448 fallback and other unlisted bytes|not established by the inspected writes|

The lookup retained in CP112 maps **`2D_DEF_GUI_PST2` to C+10** and **`3D_DEF_GUI_PST3` to C+11**. Exact literal bytes and hashes are in the new metadata receipt. These two bytes may have prior allocation/earlier-producer values; no zero/default-false inference follows from the three reset calls. This is deliberately narrower than a whole-program claim that no other earlier or later producer can write them. Any isolated model or eventual owner must retain known/unknown provenance per field, not eagerly zero the whole0x14a0-byte context. Unknown-key lookup also returns C+1448 with availability=false; that fallback byte is not initialized here and must not be read merely to manufacture a false result.

## Registration remains the next lifetime edge

Open0aa960 traverses actual diagram items, resolves feature-name producers and queries this same C through12ec7b0; a false selected byte can call12f08d0(true). Its negative cache1498 test runs12eee10 before empty/null feature guards; the earlier CP112 capture shows allocation/construction/global-provider/query/delete in that producer. Registration may write the selected flag, insert an actual owned provider object into C+1458 or delete a failed object. Its actual results/ownership are not implied by a freshly empty map or source feature-name vocabulary. The retained Boolean fixture is not a same-owner feature/provider observation.

Accepted Close then12f1190's29-name sequence calls12f0da0 on **this same C**, and only selected nonzero actual flags can enter provider/map removal/deleting-virtual effects. A new independently zeroed Close context would skip real owned entries; a per-Session blanket reset would erase the distinction between map/context cleanup, process-remesh state and main/application lifetime. CP110's preceding main/busy/modal/refresh/manager/diagram admission guards still apply; no normal-Close callsite was added to the public converter.

The smallest implementation contract now available is a **conditional staged constructor prefix**, provided root first supplies an actual Project-linked context identity and source-qualified allocation return/ownership receipts for the8-byte proxy and48-byte sentinel. It must preserve first/second allocation order, publish proxy before second allocation, retain unknown bytes, and stop on unknown allocator/handler/unwind/free outcomes. It cannot yet produce full construction success, real registration or accepted cleanup. A pure fabricated-success allocator plus zero-filled fields would conceal these unresolved edges, so no such helper was created here.

Concrete next offline action: bound allocator backend147489c, handler1481ce8, free14dfc68 and the map constructor's relevant C++ exception-handler dispatch before implementing allocation/failure transitions. Separately identify actual writers to C+10/+11 and the actual operation feature-name producer05ef960/0aa960 path for the selected supported project. A source-bound external result interface can record real provider results later; no always-available answer or licensing substitute is justified. Focused future tests should include proxy-success→sentinel-failure cleanup with no outer stores, untouched PST bytes containing distinct nonzero sentinels, unknown allocation refusing without publication, successful proxy/head identity/self-links/byte order, negative cache predecessor before empty feature name, and same-owner registration→Close release with no reallocation/defaulting.

## Compact evidence receipt

`resume114-feature-owner-state-evidence.json` SHA-256 `f8e3ddabf5195e25fe935fc9d6ed8019e42c4c643e2c62230b13e290ddc575e9` binds binary metadata, EH/IP map, literals and actual leaf RET bounds. It also binds the first partial capture: the script retained12f1060 successfully, then stopped because12e6fb0 has no unwind entry; no empty/missing receipt is treated as complete. The reset leaves were recaptured with explicit64-byte candidate spans. Interpret only through their RETs at12e6fe8/12e701a/12e7059; padding and the next-function fragment are not part of the claimed leaf. Proxy cleanup leaf ends at12f5e38; later captured next-function bytes are likewise excluded.

| Raw capture under analysis/ | SHA-256 |
| --- | --- |
|resume114-feature-constructor-1412f1060.asm|`cb7994cfa20f3bc8465016648e826ecdf9f0d8818e33b07f8620c88cb56376fc`|
|resume114-feature-flags-1412e6fb0.asm|`198375e4e1a1a89249a0e96cb29a54a86661e88c264dd1aea6e5986192ecb262`|
|resume114-feature-flags-1412e6ff0.asm|`4d1a4a310102a697c44fda7a47431af41f8023344a80598fffeba614fb27f577`|
|resume114-feature-flags-1412e7020.asm|`57ad77652ec2ac0305084bb87e63fe53df32f0a67563a389a3e48bc70dd2cbfe`|
|resume114-feature-map-sentinel-1412edac0.asm|`08e448ba9345cf8a15f02465c0667b3518c520542de75516b576956290c764ef`|
|resume114-feature-allocation-14146eabc.asm|`e11fe4ba26a2e4235b073c015111df92320f33b842ae2799237ab439a9134510`|
|resume114-feature-allocation-141b9c670.asm|`116904a1c0167d936ffb102643d733cf4f6eb81a1b9225ab79d12a2a9565eaf0`|
|resume114-feature-proxy-unwind-1412f5e30.asm|`4d0544f4631a2278c1768540856bce692ba32e259260a223dea92eaf8c7e646e`|

Four separate compact `resume114-...-static.json` receipts retain exact commands, binary/tool hashes and bound classifications for flags/map-sentinel/allocation/proxy-unwind captures. The existing12f10d0 constructor capture remains immutable (`38e87349cb26103cb4f4d593fa0ac441ff4b5706283645e6cfdf00255cbe6e62`). No shared files or production/test source were modified; this is a bounded evidence increment with explicit owner/allocator/registration blockers, not a constructor/Close acceptance result.
