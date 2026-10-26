# Checkpoint 78: QIcon shell, addFile and graphic ownership

The original matched SetOprHasDoe receiver allocates an eight-byte QIcon value object, initializes its private pointer to null, constructs QSize(-1,-1) and calls addFile with the DOE resource. The shell/default-size prefix is now closed by immutable Qt DLL code. addFile is a separate QFileInfo/plugin/engine ownership boundary; it cannot be implemented by storing a resource filename and declaring an icon loaded. The later graphic setter consumes the raw icon pointer only when it finds the graphic item.

New bounded EH evidence also closes an important distinction: the receiver frees raw allocation if its QIcon constructor unwinds, but does not register completed-icon destruction/deletion around addFile or backend delivery. Preserve that original ownership behavior rather than applying automatic cleanup on every later guard.

This lane changed only uniquely prefixed static evidence and this report. No candidate source, build, tests, original execution, GUI or mutable REA changed. Root must review/release the proposed source successor before it starts.

## Actual imported values and constructor prefix

The retained GUI IAT entries were resolved from immutable PE import descriptors:

| GUI import slot | Actual import | Preferred-image target |
| --- | --- | --- |
| 0x141e0b688 | QtGui4_64.dll QIcon default constructor | 0x65014130 |
| 0x141e0b690 | QtGui4_64.dll QIcon::addFile(QString const&,QSize const&,Mode,State) | 0x650a2590 |
| 0x141e0b6a0 | QtGui4_64.dll QIcon destructor | 0x650a1430 |
| 0x141e059a8 | QtCore4_64.dll QSize default constructor | 0x6700ae00 |

Normal receiver path `0x140086ca0` still begins with actual backend+0x18 and first matching logical operation1. Only the nonnull/matched true route reaches:

1. `0x140086d0d`: allocate8 bytes; nonnull calls the QIcon constructor at `0x140086d24`.
2. QIcon leaf `0x65014130–6501413a`: store qword0 to `[this]`, return this. There is no vtable, QObject parent, private engine allocation, resource lookup or global counter in this default constructor. Its eight-byte value is a private-data pointer slot, distinct from any future QIconPrivate.
3. `0x140086d53`: QSize constructor. Leaf `0x6700ae00–6700ae10` writes height=-1 at +4, width=-1 at +0 and returns this.
4. Construct QString from exact nonempty `:/SfTool/icon/tool/iconDoe_24_24.png`, then call QIcon::addFile at `0x140086d95` with Mode0/State1. Size(-1,-1) is an unspecified-size request, not proof of24x24 decoded pixels. The filename contains24_24 but does not override the actual default size argument.

The default-constructor and QSize captures use bounded leaf spans that contain padding/neighbor bytes after RET. Claims use only those exact leaf instructions through the first RET, not the neighboring function.

The receiver's allocation-null branch supplies null this to the later addFile use. Do not invent a successful null-icon fallback or ordinary false return; the candidate must guard the unsupported fault branch. Allocator exception/failure contracts are not closed merely by its null test.

## addFile is an engine owner, not a path assignment

New complete selected unwind-region disassembly for `0x650a2590–650a27d9` establishes:

- Empty QString returns at `0x650a25cc`; this is not the fixed nonempty DOE request.
- With d=null (the newly constructed shell), it constructs QFileInfo(path) at `0x650a25e4`, obtains suffix at `0x650a25f5`, and tests that suffix.
- Nonempty suffix enters two alternative QFactoryLoader lookup routes (`0x650a1e80` and `0x650a1d10`, followed by imported instance(QString)). Nonnull plugin interface/create results can allocate a QIconPrivate wrapper and install the returned engine. Actual plugin/resource registration/result was not observed; do not choose a fallback route by assumption.
- When no private object was supplied, fallback allocates32 bytes at `0x650a2737`, constructs QIconPrivate via `0x650a0550`, stores it into the QIcon shell at `0x650a2753`, then allocates a16-byte default engine and constructs it via `0x650a2470` before storing its pointer into private+0.
- At `0x650a279c–650a27b9`, the selected private engine receives virtual+0x28 with the original path, QSize, mode and state. addFile returns void. This call is not a checked image-load-success Boolean.
- An already-nonnull d instead calls detach at `0x650a2797`; it is outside the newly constructed DOE shell's initial branch and must not be imported into that branch by copying an existing icon.

QIconPrivate constructor `0x650a0550` initializes engine pointer0, atomic ref1, obtains a value from an actual global atomic counter at `0x659be118`, initializes +0x10=0 and +0x14=2, stores the returned counter value at +0x0c and zeroes +0x18. This includes a real shared counter effect that a path-only record omits. Fallback engine ctor `0x650a2470` installs vtable0x656b16d8 and shared container/ref state. No source-closed plugin inventory, selected engine instance or decoded resource/cache payload is available from these constructor facts alone.

The first unclosed addFile subcall for the fixed nonempty/new-shell route is QFileInfo(QString) construction at `0x650a25e4`; suffix/plugin/private-engine work follows. The smallest isolated shell successor should stop at addFile entry, preserving its input request and not executing a guessed QFileInfo/engine.

## Graphic lookup and transfer after addFile returns

Retained original application captures remain controlling for these downstream steps:

- After addFile returns and its temporary QString is destroyed, the receiver re-reads model+0x18 and calls actual backend virtual+0x238(logicalItem,icon,1) at `0x140086dd2`. Backend identity cannot be cached through unqualified callbacks and assumed unchanged.
- Known vtable candidates SfDiagEditor/SfDiagCellEditor map this to `0x140181c80`. That setter first calls actual backend virtual+0x1d8. Candidate target `0x140171190` uses implementation at backend+0x28, map+0x20, keyed by the same logical item. A missing map entry returns null without default insertion in this accessor.
- Missing logical item in the earlier receiver returns **before** allocation. Missing graphic item here returns **after** icon construction/addFile. No receiver-side icon delete follows this normal return. It is an unconsumed/possibly leaked allocation, not a successful ownership transfer or an allocation-free no-op.
- A graphic item with selector1 calls `0x14016dbe0`, which follows graphic+0x68 to implementation primary-icon slot+0x118. If the old icon is nonnull it calls `0x14016e8d0(flag1)` before storing the new raw pointer at `0x14016dc55`. The helper invokes QIcon destructor then operator delete for flag1. There is no equality fast path; do not silently add one for old==new.
- New normal QIcon destructor body at `0x650a1430` checks d; null returns. Nonnull decrements private ref through actual QBasicAtomicInt.deref. Only final-ref paths release engine/private storage, with additional ownership behavior distinguished by private+0x14 (1 versus2). A counter decrement is not a blanket immediate engine destruction.

Actual backend type/map/graphic implementation/old icon owners are still required. A registration/commit descriptor, FlowItem pointer or Operation.hasDoe flag cannot stand in for them. No direct Operation.hasDoe store or redraw call appears in this identified receiver chain.

## Failure and unwind facts

The new raw EH receipt decodes the original receiver's MSVC FuncInfo magic19930522, unwind map and IP-to-state map. Selected funclets were then disassembled:

| Original receiver instruction range | EH state | Qualified cleanup |
| --- | --- | --- |
| before0x140086d17 | -1 | No local icon-allocation cleanup entry yet. |
| 0x140086d17 until0x140086d44 | 0 | `0x141817880` frees raw `[frame+0x48]` allocation through0x1414dfc68 if construction unwinds. |
| 0x140086d44 until0x140086d7d | -1 | Completed heap QIcon is not an automatic C++ local object. |
| 0x140086d7d until0x140086d9c | 1 | `0x1418178a0` destroys the resource QString at frame+0x58; no QIcon destruction/delete action. This includes addFile. |
| from0x140086d9c through backend call | -1 | No receiver-local QIcon cleanup action around later delivery. |

This is static MSVC cleanup evidence, not an executed exception experiment or a claim about every process fault. It corrects a too-broad policy of retaining raw allocation even if its constructor throws: **construction-failure raw cleanup is present**, while completed-icon automatic cleanup for later exceptions is absent in this caller.

addFile's own EH map has two local object cleanup states (QFileInfo and suffix QString) and five raw allocation construction-failure cleanup actions. Its disassembled funclets release those temporary/construction allocations, not an enclosing QIcon or a generally installed engine/private object. After an installed private state or plugin action has occurred, a later exception must not be rewritten as an empty d or automatic success. The exact descendant plugin/engine failures remain guarded; no general rollback contract is established.

The known backend setter and primary pointer setter have unwind metadata flags0 and no C++ cleanup handlers in their selected entries. If an old-icon destructor/delete does not return, the new-slot store remains unreached. Do not reset old/new pointers or reclaim the new icon through an invented catch. Normal old-icon deletion-before-store is closed; arbitrary destructor-fault recovery is not.

## Smallest ready isolated source slice

Propose a retained typed **shell constructor/request boundary**, ending before addFile executes:

- Own one concrete eight-byte private-pointer slot allocated on the admitted nonnull path. Initialize its slot to null in the proven default constructor step.
- Represent default QSize as signed width/height -1/-1 and the fixed resource/mode0/state1 request. Keep payload storage and diagnostic phase metadata separate, so sizeof the represented shell remains8 on this x64 target.
- Publish the concrete allocation to a caller-owned arena before any later potentially throwing request preparation. A guard at AddFile preserves this allocation. Do not put it in a temporary unique_ptr that is destroyed merely because later source continuation refuses.
- Record no addFile return, engine type, loaded image, graphic transfer, receiver dispatch or native lifetime-completion status. The next boundary is explicitly AddFile/FIleInfo-engine ownership.
- Arena destruction releases adapter storage only, not a reconstructed original leak cleanup/Close. A constructor-failure path needs its distinct raw-release semantics if implemented; no artificial allocator-null success is admitted.

This may be tested as a source-qualified isolated value/allocation prefix. It must not be called from public SetOprHasDoe delivery yet: actual receiver population/backend/item ownership remains an independent prerequisite, and addFile/resource/graphic effects remain unimplemented. A resource descriptor alone does not advance the active DOE path.

Proposed **exclusive new-file** write set, pending root release:

- `source/apps/moproj2mst/include/m2m/doe_icon_shell_prefix.hpp`
- `source/apps/moproj2mst/src/doe/icon_shell_prefix.cpp`
- `source/apps/moproj2mst/tests/doe/test_icon_shell_prefix.cpp`
- `analysis/resume78-doe-icon-shell-implementation.md`

Suggested API is a caller-owned `DoeIconShellArena` holding concrete allocations plus `appendDoeIconShellPrefix(arena)` returning an observation of the retained shell, request and `NextBoundary::AddFile`. No Session field, old icon or generic Qt callback is introduced by this isolated slice. Future canonical Session integration must separately bind that arena at the real allocation site; the new API must not label a supplied diagnostic arena as already native/session-owned.

Focused tests: dnull/sizeof8 and size(-1,-1); exact resource/mode/state; two calls retain two distinct payload allocations; prefix/request guard does not drop an already-created shell; no engine/graphic/Operation/notifier/FileEffects mutation; adapter arena release is explicitly separate from native cleanup; unsupported use cannot claim addFile returned or delivered a receiver. Testkit/static preflight/root grouped validation apply. No source/test implementation is included in this report.

## Evidence receipts

Binary hashes: original GUI `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`; QtCore4_64.dll `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

New evidence comprises15 bounded dumpbin selections totaling27,165 output bytes, plus compact import/EH value receipts. These are immutable static-file observations; selected unwind regions are not asserted to close every descendant. Existing CP51 graphic/registration captures remain unchanged.

| New compact receipt | SHA-256 |
| --- | --- |
| analysis/resume78-doe-qicon-static.json | D92809C9C52C0D3A64F06929C4CCCF18DB3EFAD4616C74EA320EDFB396BFD5FE |
| analysis/resume78-doe-qicon-owner-static.json | 1251DF3C7E804F9669668FFA99BDD93D0AF679642FC66741263079F24DF5918E |
| analysis/resume78-doe-icon-unwind-static.json | 07DCE58901CE5639250323F123D42D8FF43CEB340B09FDD115EF0A9AFFA0E9F5 |
| analysis/resume78-doe-icon-size-static.json | FB1A871A0A88E51D9766B4D65ACA90F278C25B20980AA6458C9590F1D0C1E91A |
| analysis/resume78-doe-icon-owner-static.json | 7C75DEA2DCA4CF5E0ED1F77CEB21D3C5367C9CCE3306F015E4F0BD8B41421463 |
| analysis/resume78-doe-icon-unwind-states.json | 1FEC951EDA70A27DD563154BA5D5272B3B7218C2680D112C773433A754D6633B |

No runtime B121 receiver delivery, plugin registration/result, resource decode, graphic ownership or full public Type2/DoeTableRefresh completion is claimed. The next active unmatched dependency is still the actual receiver/backend/population integration; the proposed shell prefix is an independent bounded constructor increment only.
