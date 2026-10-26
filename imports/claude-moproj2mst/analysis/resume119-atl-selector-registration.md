# CP119: the actual ATL registration inventory excludes selectors44/45

The selected main constructor does register ATL events, but the recovered routine requests **selectors15d,160,15e,15f**, not44 or45. The44/45 initialization/cleanup dispatcher branches recovered in CP117 are not established eligible callbacks by this fixed registration inventory. This narrows the hypothesis: actual COM activation, source identities and delivery of the four registered callbacks remain unknown; the mere existence of44/45 branches is not a default event schedule or a reason to replay initialization during Close.

Only this report was written. Bounded retained/local static PE/source, DUMPBIN, RTTI, GUID and rel32 candidate scans; no source/interface/plan change, build/test, GUI, original launch, REA import/retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed. Function/site addresses below are RVAs relative to140000000; globals/tables use full VAs.

## Actual constructor registration and failure order

Base constructorc98b00 calls **ca6910 atc991bf**, after earlier QMainWindow/base/member/setup work and afterc99530's W=M publication/M+50=null initialization. It stores the returned HRESULT atc991c4, reloads the original M and returns it atc991d8 without testing that HRESULT. The derived10490 setup and its later G factory follow only after this base return. The constructor does not throw or replace M merely because this particular routine returns a negative HRESULT; other exceptions/effects remain qualified.

ca6910's initial provider chain is now explicit:

1.ca6945 callscbc6d0 with CLSID at14290d948, null outer, context17h. cbc708 invokes IAT141e0c848, independently resolved as **ole32!CoCreateInstance**. CLSID bytes decode as `82c5ab54-c92c-4d52-aac5-27e25e22604c`; requested IID at14290d958 is `00000000-0000-0000-c000-000000000046`.
2.Negative activation HRESULT branches atca6953 to cleanup/return, skipping registrations. On successca6976 callscbfd10, which invokes QueryInterface with IID14290db58 `cb3bf65e-0816-482a-bb11-64af1e837812` into a second local interface holder. Negative result branches atca6984 to cleanup/return.
3.On this successful path, the local interface's virtual+68 receives the exact UTF16 literal **“AtlCude3D”** at142909048. The first event source is this temporary local interface, not an assumed M+1f8 field.
4.Later source-provider virtual+48 fills/uses M+200's interface holder; virtual+50 similarly supplies M+208. Their implementations, output identities and callback behavior are not present in the retained executable bodies. Do not equate these COM source holders with main+480's Qt stack or a Session widget.

The decoded routine has only the two negative-result branches above; it stores each subsequent add-handler HRESULT but does not test it before requesting the later registrations. Thus the table below is an ordered **attempt inventory**, not proof every Advise succeeded or that any event fired. Partial registration is not rolled back merely because a later attempt returns failure.

| Callsite | Source holder | Event index/count | Stored incoming selector | Dispatcher target |
| --- | --- | --- | --- | --- |
|ca69e7 ->cbf800|temporary local interface|0 / 1|15d|ca6ba0(M,int)|
|ca6a6b ->cbfa70|M+200|0 / 1|160|ca6eb0(M)|
|ca6aef ->cbfbc0|M+208|0 / 2|15e|ca6e10(M,int)|
|ca6b39 ->cbfbc0|same M+208|1 / 2|15f|ca6e70(M,int)|

Each call supplies the actual proxy address **M+28** as this, primary M as receiver, the selector inr8d, the source interface inr9, then event index/count on the stack. The dispatcher adds1, as CP117 established, so these incoming selectors choose table indices15e,161,15f,160 respectively. Their argument adaptation is established by retained v6.c and the PE branch table. Callback bodies and their transitive GUI effects were not newly closed here.

## Where selectors are stored and how an event selects one

The ATL proxy constructorcbc7a0 initializes its list/cursor fields+8,+10,+18 to null. RTTI identifies its node type at TypeDescriptor143959840 as `ATL::__ComEventingImpl<SfMainWindowBase>::__ComEventingNode`; node table14290dca0's virtual0 is **cc11c0**.

The three add-handler helperscbf800/cbfa70/cbfbc0 obtain/create nodes throughcc0390/cc0850/cc0bf0. Their node constructorcc0f90 sets node+8 to receiver M, stores event count at+18, allocates the integer mapping at+10 and fills its bytesFF, yielding initial entries-1. For the first helper,cbf895 tests mapping[eventIndex]==-1; cbf8b1 writes the supplied selector into that entry, then updates active count and retains receiver M atnode+8. The corresponding two helper bodies have the same selector-map role for their source-interface variants.

cc11c0 reads **node+10[index]** and returns that stored integer; event index0 or1 is not itself the large01e910 selector. This makes the four registration constants above the relevant mapping values. Constructor initialization to-1 does not make selector44/45 available.

After map setup,cbf91f callscbf940 with the source, node+30's sink interface, event IID and cookie destinationnode+20. cbf940 performs the source QueryInterface/connection-point lookup/Advise pattern; its virtual+28 call supplies that sink and cookie destination. **Node+30 is the registered sink interface, not the connection point**. The latter is acquired in temporary holders insidecbf940. First event-interface IID at14290db78 is `8fe3a216-e235-49a6-9136-f9d81fdadef5`; the variant getters return IID constants14290db88/14290db98. Actual external sink/source firing and payloads remain unobserved.

## Lifetime and bounded absence claim

Whole retained .text E8/E9-rel32 candidate scans found these direct add-helper references:cbf800 onlyca69e7; cbfa70 onlyca6a6b; cbfbc0 onlyca6aef/ca6b39. Each was decoded inside realca6910, rather than treated as a raw-byte proof. The corresponding remove helperscbfd50/cbff90/cc00e0 have four decoded direct callsites,ca73c3/ca742d/ca747e/ca74ab, all in **ca72b0**. ca72b0 is called by regular base destructorc991e0 atc99259, after global main clearing andc99890 cleanup. Base eventing destructioncbc800 also traverses/releases retained nodes; its physical COM cleanup is not projected as harmless.

ca6910 itself has decoded direct callersc991bf (constructor) and024409 (dispatcher incoming selector15c). ca72b0 likewise hasc99259 (destructor) and0245b4 (dispatcher selector161). These dispatcher alternatives are not evidence of actual later event registration/invocation. They reinforce that an address alone does not identify constructor/destructor occurrence.

Within this recovered fixed registration path, **neither44 ->c99530 nor45 ->c99890 is registered**. They remain valid dispatcher branches, but no source in this inventory makes them a later default event. The earlier CP117 “registration/selector caller unknown” is therefore narrowed: the constructor registration source is now closed and selects different methods. No new44/45 witness should be demanded merely because the broad dispatcher exposes those methods.

This is not a whole-program claim that an external caller, indirect registration, alias write or other module can never alter the map/invoke the proxy. Raw rel32 scans are incomplete for those mechanisms. No such additional registration was established, so none is added to the selected profile. Relevant retained binary inventory contains the original GUI and Qt DLLs, without an identified implementation of the activated COM class; no COM server behavior is invented.

## Actual owner bridge and remaining witness

The same M owns the proxy receiver and later constructs G; G+10/registered child receivers name M. W is the separate mutable AppState.main value used by fresh Close busy/dialog/accepted lookups. ca6910 does not justify merging those edges or replayingc99530 to refresh them. A base-phase Advise call is also not proof of no synchronous callback: the COM source may execute before registration returns. Base+300's constant false is phase-specific, while the later derived slot still needs actual+480/index/+270 inputs.

The smallest actual producer continuation now has a precise branch: **after real QMainWindow/base predecessors and an actual negative CoCreateInstance/QI result**, this attempt installs no new hooks andc98b00 still returns the same M; derived setup/G publication can continue in source order. The selected first constructor occurrence starts with the freshly null proxy list; this statement is not a reset of registrations retained by a later repeat occurrence. It does not permit fabricating an activation failure to avoid events. On the successful provider branch, the one M/proxy must retain the actual local/M+200/M+208 sources, successful selector mappings and Advise cookies through their correct unregistration/destruction occurrences.

Current Session owns neither that native main/proxy graph nor those COM sources/cookies; it retains a candidate manager with Session backlink and no live W producer. Implementing the bridge therefore still requires the real shared application/main construction/backend work identified in CP116/117. No detached helper is proposed in this report-only lane.

The remaining selected-lifecycle witness is the actual COM activation/QI/source/Advise outcomes and **which registered15d..160 callbacks, with what payload and order, are delivered during construction/Open/Save/modal intervals**. Missing44/45 registration is not a license to assume this other event set empty. Qt startup/due-event and fresh FileCloseAll prerequisites, including CP110's corrected Save-failure/nested-Save behavior, remain attached. Static evidence alone supplies no accepted Close run.

Focused future validation should assert the four ordered mapping attempts and partial-failure behavior on the actual constructor owner; negative activation skips all hooks but does not fail M construction; M/W identity remains separate; and known registration does not imply any delivered callback. Use a source-qualified actual backend/witness before admitting event-free or populated schedules. No test was run here.

## Byte receipt

DUMPBIN14.44.35228.0; section-aware PE/.pdata mapping; end-exclusive spans:

| Span | SHA-256 |
| --- | --- |
|140ca6910..140ca6b94 registration routine|`48f0911635dc491e470a7824c1c34e4ff54cb4371763be3226a602791e99e021`|
|140cbf800..140cbf940 first add/map/Advise helper|`a920199f94003edc1a2d53e9392003db0f9d7b14f142b922ff6f98dd5ea2b1c4`|
|140cc0390..140cc0727 first node lookup/create helper|`676ac456b0c4bdbd16cc5b9b928fc135c3227e0ef5a785a29ea53b93dd2a74ed`|
|140cc0f90..140cc10b6 node constructor|`cf44c72c4d3407bc774ce2250693751e59f2513dfd094fc9a9647094d4022421`|
|140cc11c0..140cc11db mapped-selector getter|`f37db39033be85606e7c313a75de20dc2fea45ff863d53a8aa638988e550a2ea`|
|140cbc7a0..140cbc7f3 proxy constructor|`259abcf84e0f91d8568996daea260f5f88387fa3d800ab9e76f042f1f7fc2178`|
|140cbc6d0..140cbc713 activation wrapper|`c0b7b4789b820171e193b402fb353af8cc348bff9bea8bc2cb2e939bf260f4e1`|

Retained source: evidence/gui_mo/s1/moproj_vt/v6.c (`cef772daeb877e3dc2b0a76c1c98de8261e337a7e3501e9640636f99aa9ce9aa`); resume117-close-canonical-main-construction.md (`ffd128c9f46352f1d1b89337634e75ccbe0a4aa7d01bd420c19726f2ad374aa9`); CP107/110/112/114/116 source qualifications. No runtime Evidence ID, original occurrence or tested-candidate freeze is claimed.
