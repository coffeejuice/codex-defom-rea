# CP143: indirect feature release and published SUB owners

The root-release predecessor is the already recovered **29-feature provider release sequence** on the same Project's inline context. It does not establish release of SUB+20 (`SfMoDoeManager`) or SUB+28 (`DefOptModel`). Their pointer fields are untouched by the directly inspected SUB destruction path; ultimate allocation lifetime remains unresolved across provider/global callbacks. No native order for deleting those two published allocations is justified.

The conditional original accepted-Close chain into SUB destruction is statically closed in CP110/116 and revalidated here. CP142's remaining-edge wording was too broad: the unclosed public integration dependency is the **actual candidate/live main→manager→Mo→Project/SUB occurrence and predecessor state**, not an unlocated original scalar-deleting target. This report narrows that claim without treating static dispatch as a live accepted Close or candidate ownership producer.

Only this report was written. No source/build/test/GUI/app-control/original execution or REA/database access. Bounded offline DUMPBIN14.44.35228.0 and section-aware immutable PE/.pdata/RTTI/import/reference reads used retained original DEF_GUI_MO_64.exe v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Same-Project context operation

For nonnull Mo+19c0=P,140044bf0 reads that exact P and calls1400982c0(P) at140044c14.1400982c0 calls1400aab30;1400aab30 computes C=P+60 and calls1412f1190. Only after that returns does root release reread Mo+19c0 and call the Project deleting wrapper1400453e0, then null Mo+19c0. Feature operations may have callbacks, so the reread and actual owner identity must remain part of any future cursor; a saved copy cannot silently replace a changed current Project.

The complete1412f1190..1412f134e body calls only1412f0da0, exactly29 times, with fixed module-symbol pointers. I checked their actual pointer identities and decoded names in the retained PE. They include2D/3D GUI, simulation, machining, preform, cogging, rolling, extrusion and furnace feature names. They are not SUB field offsets or manager/optimizer pointer inputs. CP116's canonical-key pointer/map distinction remains controlling.

For each name,1412f0da0 resolves the selected actual context byte through1412e76c0; absent/zero skips further work. Nonzero calls1412e5780, with a negative result also skipping. Otherwise1412f0e2d invokes map subscript1412f0ab0 on C+1458, reading the returned node's raw payload at1412f0e32. For a nonnull payload,1412f0e46 erases the map entry through1412f0bb0 **before**1412f0e56 invokes that saved payload's deleting virtual0 with EDX=1. Later external/provider operations can fail before the selected feature byte is cleared; a successful final byte clear is at1412f100b. The whole function has substantial provider/global work, so its indirect graph is not reduced to the explicit map erase/delete edge.

Those map payloads arise from the separate feature-provider constructor1412ee6a0 and0x3ea0-byte allocation in registration1412f08d0, as qualified in CP116. DOE manager+20 instead arises from0xb8 allocation/1400d1180; optimizer+28 from0x50 allocation/140917ff0. No map insertion or provider-construction edge here aliases either published SUB allocation. This is distinct construction provenance; it is not a whole-program proof that no provider callback can ever consult or release a global object.

After feature release, Project destruction clears its SUB list before destroying this same inline context C. Context/map destruction releases ordinary node storage, sentinel and proxy, without automatically deleting raw provider payloads. The29-feature release phase must not be replaced by an owning-map clear or promoted into a blanket process reset.

## Conditional accepted Close reaches this SUB destructor

Native main Close1400299a0 performs fresh busy/current-main, child can-close/modal/nested-Save and accepted callback/refresh effects before140029a76 calls ModelManager clear140c78300. Its child encounter loop calls each nonnull child's deleting virtual **vt+18 at140c784f2**, passing EDX=1. Only after those calls return does it clear the list/reset current.

For an actual `SfMoModel` child M, constructor/destructor vtable142112878 has slot+18=**140045350**. I independently decoded its RTTI complete-object locator1430d5508 as `.?AVSfMoModel@@` and read the actual slot.140045350 calls140044040 at140045362;140044040 calls1400440c0 at140044066;1400440c0 calls140044bf0 at1400440ce. This is the missing indirect target of the earlier CP81 bounded direct-E8 scan, already incorporated in CP110/116.

For the Project actually reread from M+19c0 after feature cleanup, the sequence is1400453e0 ->14008a3c0 ->1400a80e0. The ascending Project+58 list loop reads each SUB and calls1400ac330(sub,1) at1400a8160 ->1400b58d0. Thus a SUB selected from that same P is destroyed after P's feature cleanup. This conditional static chain does not prove all callbacks preserve the pointers, all main guards pass or candidate Session storage implements native main/Qt construction. Existing busy/modal/current-main/feature/diagram/OPI/Report/public Close guards remain real prerequisites.

## What is and is not released for DOE/optimizer

The two published types are independently identified: manager constructor1400d1180 sets vtable1421671b8 (RTTI `.?AVSfMoDoeManager@@`); optimizer140917ff0 sets1427bceb8 (RTTI `.?AVDefOptModel@@`). Their scalar deleting entries are virtual0,1400d7f60 and140918fc0, respectively. These are distinct from SfMoModel's vt+18 and from each other. A pointer's nonnull type tag does not admit either deleting entry on some different receiver.

The manager constructor stores its incoming SUB backlink into manager+90 at1400d1210. This is a raw backlink store, not a QObject-parent constructor argument or evidence that SUB later owns a smart pointer with automatic deletion. Its base initialization140919b80 ->14091dfc0 and the optimizer's base14091dfc0 do not supply an observed QObject parent/destruction edge. The underlying base shared-data/storage members have separate teardown; do not relabel them as ownership of the entire manager/optimizer allocation.

CP142's full SUB destructor1400b58d0..1400b59de reads+48/+30 guards and raw model+0, then tears down embedded fields; it never reads+20/+28. Its scalar wrapper later frees the SUB allocation itself. The b5470 constructor's decoded local unwind frees only the allocation whose constructor is in flight, then embedded members; it does not free earlier published raw owners. Those precise local facts remain valid. Pointer bits at+20/+28 are not explicitly cleared by this path; once the SUB allocation is freed, their old values are opaque history, not live authority.

A raw executable E8 scan found no direct caller of optimizer deleting wrapper140918fc0; that is expected for a virtual0 target and does not prove it is never called. Five direct optimizer-getter1400b5ab0 candidate sites were located:140090337,1400916f6,14009a0f8,14013d798,14013ed31, within retained load/save/ShapeOptim methods. Bounded consumer inspection showed borrowed optimizer use, not a new SUB disposer. The manager getter has42 raw candidates; this lane did not claim an exhaustive typed dataflow/indirect-call audit of them. An unclassified callback/global-registration route is still an exact lifetime dependency.

Therefore the bounded answer is: **neither direct SUB destruction nor its immediate feature-context predecessor proves deletion of the published DOE/optimizer allocations; permanent retention or a global leak is not established either.** The explicit per-feature deletion concerns its map payload, not automatically those SUB slots. Choosing a C++ unique_ptr destruction order would still invent behavior.

## Next evidence action and integration limit

Prioritize a typed lifetime audit from manager1400d1180/+90 backlink and optimizer140917ff0 into their virtual0 targets, including registration containers/global callbacks which retain these exact allocated addresses. Use the five optimizer-getter caller families and manager's actual stored collection relations; inspect slot0 calls with typed receiver provenance rather than scanning for a destructor name alone. Any assertion that a published pointer remains allocated through Close requires that closure or an authorized same-instance lifetime witness, not just absence of an E8 call.

No new private owner helper or public Close callsite is proposed. Existing detached optimizer values and feature runtime frames remain separate. Actual cleanup integration still needs source-qualified Project feature construction/registration, actual P/SUB identity across callback-sensitive releases and native main/Mo admission; the isolated map/CRT tests do not supply those witnesses.

## Immutable receipts

All ranges below are half-open; full functions use their .pdata end rather than a following padding byte.

| Range | SHA-256 |
| --- | --- |
|1412f1190..1412f134e, 29-key release|`95532f69394a3f61e1eeb5cd30ff19a2cdc261bd535e67af06dc5fbbf2aff574`|
|1412f0da0..1412f105f individual release|`4510480e79a2108f866cc8d297de121bc75ad4ee42dc9045cf3bfa7a22e3c167`|
|1400299a0..140029d04 normal Close|`8a3490e8c9007cd78fa2c3d876f2e071ee2e8bb113292ae70f944cb4a3a30d9a`|
|140c78300..140c7854b manager clear|`b2c665e2182ec5050431091d4859aafd7d86e1ca1ca2e4e7d14a36c5bf7b1487`|
|142112878..142112898 SfMoModel first4 slots|`f6af86c5ca83d2fb064f8bf8ed7ae47f45a6a09e5dfe551f7ad176db3e8b8c17`|
|140045350..140045386 Mo deleting wrapper|`07e6b393cb4f7bf1ffd09ff57d42a6b11eb855dd60291cd18b5f7fc8a9d13a78`|
|1400d1180..1400d153d manager constructor|`cbe324597947bb38fd61601a17264686c87ca4c49b2d553d673fa35264953974`|
|140919b80..140919ca6 manager base constructor|`9974da9d4a5b103137f90d1eb3285a8ed7109b8dfcc757926bf4a3ccdd1542dc`|
|14091dfc0..14091e02d shared base constructor|`6603cffba958d3657e80854c8cb1cddfd21fc725ecdd375873a05551feddc4a3`|
|1421671b8..1421671e0 SfMoDoeManager first5 slots|`07091c41df90048ea9f9713b8812bea3979e366d875d87a5b2947bb9050884ed`|
|1427bceb8..1427bcee0 DefOptModel first5 slots|`7d3005d91aca538ff926e3e8d423a967149e0884fee10c82fe605a76eaa17b73`|

Retained controlling reports: CP110 SHA `818298bbcb5f73fe11d1c98fd2c9754ea5d79ef6f99d61394ee8137e7a57aa0c`; CP116 feature lifetime SHA `8a24844a96af8d6897c96c4720635a3a646081548b376e7890520d8486013871`, with canonical-key/unwind/ASM metadata `resume116-feature-lifetime-evidence.json` SHA `215c68d512628362459b92755baf4c71c664a78683acc55e16d605867f768a94`; CP117 cleanup-order SHA `a0bcdbdfde77a93268044ebd08fb80cfbc90ac0f4e310bdfedb23699606b6a10`; CP142 local destructor/EH receipt SHA `711f8a8940a89713c23f5825b192a24a2bbf53d5093529823ef6ef7eeb765533`.
