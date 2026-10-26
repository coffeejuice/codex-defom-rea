# CP142: actual SUB disposer, release callers and local unwind

The containing SUB destructor and its Project-list release caller are present in the original PE and were already recovered for Report in CP81. I independently revalidated them. **The complete SUB destructor does not read/delete the raw manager at+20 or optimizer at+28. There is no direct relative deletion order between these two slots in that body.** Newly decoded constructor unwind actions also do not delete previously published raw owners. This corrects the broad “containing destructor missing” statement in the CP141/142 Type3 frontier; it does not prove a whole-program leak or close accepted public Close.

Only this report was written. No source/build/test, GUI/app control, original execution or mutable REA access. Bounded DUMPBIN14.44.35228.0, section-aware PE/.pdata/xdata reads and validated direct-call candidates were used against retained `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, original v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Actual normal destructor order

SUB constructor1400b5470 publishes a separate model pointer at+0, manager at+20 and optimizer at+28; the SUB itself has no constructor vtable store. Normal SUB destructor **1400b58d0..1400b59de** has this complete order:

1. Read guarded+48 through140c1aef0. If live, read again, invoke its deleting virtual vt+18 with EDX=1 at1400b592c, then assign null through140cbc920 at1400b594a.
2. Call1400b5bb0(SUB) at1400b5954. It reads guarded+30; if live, invokes that object's deleting virtual vt+18 at1400b5c03 and assigns null at1400b5c21. A null initial guard returns without either effect. This is the previously recovered Report slot helper.
3. Read raw+0; if nonnull, call its deleting virtual vt+18 with EDX=1 at1400b598a.
4. Tear down guard+48 through140cbc900 at1400b59a6, embedded+40 through1400bc6b0 at1400b59b5, QString+38 via IAT141e059e0 at1400b59c4, then guard+30 through140cbc900 at1400b59d4. Return1400b59dd.

There is no SUB+20/+28 load, type-conditioned deletion or manager/optimizer destructor call in this complete instruction span. Nested virtual calls have their own object graphs; this report does not establish that no other component can release one of those objects. In particular, do not turn the omission into a native guarantee that both allocations remain live forever, and do not add host smart-pointer deletion order on this evidence alone.

The nonvirtual scalar deleting wrapper **1400ac330..1400ac366** calls1400b58d0 at1400ac342, tests saved flags&1, conditionally calls scalar allocation release1414dfc68 at1400ac357, then returns the original receiver token. This is a concrete SUB disposer, rather than either owner's own deleting virtual.

## Project/Open release and accepted-Close boundary

Project sub-list cleanup **1400a80e0..1400a818a** loads count through1400a7360, iterates ascending indices through1400a74f0, and invokes1400ac330(sub,1) at1400a8160 for each nonnull pointer. It then calls14020db90 on Project+58. Project destructor14008a3c0 calls this list cleanup at14008a403. Its scalar deleting wrapper1400453e0 calls the destructor at1400453f2 and scalar release conditional on flags&1.

The actual root-slot release helper **140044bf0..140044c70** tests root+19c0, calls1400982c0(Project) at140044c14, rereads the same slot, deletes through1400453e0(Project,1) at140044c46 when nonnull, then stores root+19c0=null at140044c60. Its predecessor1400982c0 calls1400aab30, whose complete body only calls1412f1190 on Project+60 and returns1. That context-reset callee was not expanded here; do not erase it or claim full Project/owner teardown closure from the later destructor alone.

The Open failure route is concrete: main OpenProject14001ca30 calls14008a9b0 at14001d0f6, saves AL, performs vt+548 at14001d112, then tests saved AL. False reaches140044bf0 at14001d126. This is failed-Open disposal of the root's Project slot, not a normal successful Open replacement or accepted Close occurrence.

The root destruction route is also concrete:140044040 calls1400440c0 at140044066, then140cda520.1400440c0 calls140044bf0 at1400440ce and140045000 at1400440d8. It does not by itself bind which live root is reached by FileCloseAll. Accepted main Close1400299a0/manager-clear ordering still lacks a same-instance edge into this root/Project release route. Main-window QObject destruction must not be equated with single-project accepted Close without that edge.

A separate selected-subproject removal1400a70b0 invokes1400ac330 at1400a71fe. Retained `evidence/gui_mo/s1/s1-01/dc/act1.c` shows additional DOM removal, list take,1400bbd40 and1400a7320 work before deletion and current-sub selection afterward. It is a distinct action and not permission to reuse its order for FileCloseAll.

An executable-section raw E8 candidate scan was bounded to these targets, then the callers above were inspected:1400b58d0 <-1400ac342;1400ac330 <-1400a71fe,1400a8160;1400a80e0 <-14008a403;140044bf0 <-14001d126,1400440ce;1400440c0 <-140044066. The scan does not exclude indirect callers or dynamically formed addresses.

## Constructor exception actions: newly closed local behavior

For1400b5470, .pdata covers1400b5470..1400b566c and points to unwind14313ccd0. Flags2 select handler14146e20c and FuncInfo14214afc0 (magic19930522,maxState7, no try blocks). Unwind map14313cc98 contains:

| State | Next | Action | Actual local effect |
| --- | --- | --- | --- |
|0|-1|141827260|teardown embedded guard SUB+30|
|1|0|141827280|destroy QString SUB+38|
|2|1|1418272a0|teardown embedded SUB+40|
|3|2|1418272c0|teardown embedded guard SUB+48|
|4|3|1418272e0|scalar release allocation in constructor frame+28 (flow model under construction)|
|5|3|141827300|scalar release allocation in constructor frame+38 (manager under construction)|
|6|3|141827320|scalar release allocation in constructor frame+48 (optimizer under construction)|

All three allocation actions call1414dfc68 directly; they do not read the SUB's published owner fields. The constructor frame addresses above are stack locals, **not** SUB+28/+38/+48. The nested constructor's own unwind can run first; these actions are allocation release, not substitute object destructors.

IP map14313cd18 confirms state5 begins1400b558c, covers manager ctor call1400b55a1 and ends1400b55c0 before publication1400b55cd. State6 begins1400b55fc, covers optimizer ctor call1400b5609 and ends1400b5628 before publication1400b5635. Each returns to state3. Therefore an optimizer-constructor exception frees that in-flight allocation plus the completed embedded members; this local map does **not** destroy the already published manager+20 (nor earlier raw model+0). Allocation-null is a separate ordinary branch storing null; it must not be conflated with thrown construction. A failing later guard assignment also has state3, not synthetic completed-owner destruction.

This closes the previously missing local EH metadata, but not every nested member's destructor internals, upstream SUB-allocation unwind or process/global ownership. Preserve these distinctions when reconstructing observable failure behavior; adding conventional RAII cleanup could change the original local contract.

## Smallest safe next action

No manager/optimizer owner-field integration or accepted-Close release is justified yet. The next useful bounded source investigation is the root release predecessor1412f1190 and the actual lifetime/registration edges of manager1400d1180 and optimizer140917ff0, seeking any independent release route for the published objects. Separately bind accepted main Close's nonempty child/root to140044bf0 for the same actual Project/SUB. Do not keep searching for a nonexistent direct+20/+28 order inside the now-complete1400b58d0 body.

For an eventual private destructor-prefix test, use an actual SUB with independently known-null+48/+30, guard before raw model+0 deletion and keep raw+20/+28 opaque/untouched; this only qualifies if those actual field producers/identity and the private entry are assigned and verified. It cannot establish owner cleanup, constructor-failure leak behavior or public Close. Existing detached optimizer scalar projection and public Type3/ShapeOptim/Close guards remain appropriate.

## End-exclusive byte receipts

| Range | SHA-256 |
| --- | --- |
|1400b5470..1400b566c constructor|`cc0dd5062b9b3f9e8ffef32bda76f376675110c6af0c3e043cfa4f805304c9fb`|
|1400b58d0..1400b59de ordinary destructor|`2b35e06273c333eb3c9ee445daf5e8d27c2e54245c22236345e3bc2474beb3de`|
|1400ac330..1400ac366 deleting wrapper|`ed4d13f5c1eb45d0762a74fdc00fdd3cd65df6fb8e5fdc9c846023c2afeef5ab`|
|1400b5bb0..1400b5c2b Report helper|`2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9`|
|1400a80e0..1400a818a list cleanup|`17d446ec2ab4fca352f9353858a184ae63a07c6d1f1e60df0c8931b89c1d9fa4`|
|14008a3c0..14008a466 Project destructor|`d33ef435aea6da822f0aa985431d82cba18f8a18374e724d0d76030e986614f9`|
|1400453e0..140045416 Project deleting wrapper|`8b82e0d93a439f626d57dcb2a27fb558d5d3dbc994b15b441de8c13c24db49ed`|
|140044bf0..140044c70 root-slot release|`5a6af1df347276b2f42521f51aac2bddd65d6496d60a745d5c922f86170cf939`|
|14001d0f6..14001d12b Open failure caller prefix|`70a142b2001a6ffc413afab3ca0da8b3faac922721155cdc07db24eb5ff084ec`|
|1400982c0..1400982da release predecessor wrapper|`666ce31f618ded7b656be8121d509077e80e0d55b7bb375acb82259d8f2d2fd8`|
|1400aab30..1400aab4e context-reset wrapper|`424890e2bc5706437a6f6089667a077a85d8e34919c4d215220601d54ab82334`|
|140044040..14004407b root destructor|`fe7145d827bd0ed75c1e9114b7f2e1b953af60cd7341bfdcff69f6ff3975985c`|
|1400440c0..1400440e2 root release caller|`6e8b592d15fc29c331add9c6aa2a33f72a524ddc6b3a92e697aa5c0d8720bb3d`|
|14313ccd0..14313cce0 constructor unwind|`d5b3ead0f1eee7c92b0894b3e4bd415a528a0ebaa1a00360074ebeda0df22f3e`|
|14214afc0..14214afe8 FuncInfo|`f1bdc1dda5c802e42f4368a17dbdaa9947d9c0a97e3bb197e52148003ecc5842`|
|14313cc98..14313ccd0 unwind actions|`5085b4bb28ad129322b1ebe390ff9d5817d705d28f8a9aea6bf14bb46103da14`|
|14313cd18..14313cd78 IP state map|`eb466499386099ee4057c1e047b88d2e4dec576f01e7a43245b1faa6cb386958`|
|141827260..141827338 action funclets including inter-function padding|`617ac9900ea9611a43ea2c0201476f74d938f0f538d91a62b213b520cefe8ce4`|

CP81 `analysis/resume81-report-close-pe.md` (SHA-256 `e8c3528ef18fdf80e5b85f2d1abc7a63a5ce5a041049a8847f8fe75b09bd82fd`) is the retained prior basis for the destructor/caller discovery, independently revalidated here. CP141/142 Type3 reports should narrow their missing-witness statement to published-owner release and accepted-Close admission, rather than missing SUB destructor body. No production source was changed to apply that interpretation.
