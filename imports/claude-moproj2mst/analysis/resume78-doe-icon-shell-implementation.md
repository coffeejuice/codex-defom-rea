# Checkpoint 78: isolated QIcon shell constructor prefix

Implemented the approved new-file slice qualified by `resume78-doe-qicon-ownership.md`: retain one concrete eight-byte private-pointer payload initialized to null, QSize(-1,-1), and the fixed DOE addFile request. The helper returns an explicit AddFile boundary. It executes no QFileInfo, plugin, engine, graphic lookup/transfer, receiver dispatch or public Type2 continuation.

Only the three new source/test files below and this report changed. No shared Session, existing DOE source, public parser/writer, C ABI or CMake file changed. No build, executable test/CTest, GUI/original app or mutable REA operation occurred. Root owns grouped validation/archive.

## API and ownership

Public new header `include/m2m/doe_icon_shell_prefix.hpp` defines:

```cpp
class DoeIconShellArena;
const DoeIconShellPrefix& appendDoeIconShellPrefix(DoeIconShellArena& arena);
```

The arena owns prefix records and concrete `DoeIconShellValue` payloads. That payload contains exactly one private-data pointer slot, with a target-x64 static size assertion of8 bytes. Prefix metadata is separate from payload storage. Read-only observers expose shell, optional default size/request, adapter stage, optional NextBoundary::AddFile and guardMessage. They borrow the arena's lifetime; there is no escaped shared Session/native owner or native liveness claim. The arena cannot be copied/moved and exposes no individual reset/erase operation.

The four stages are AwaitingAllocation, ShellConstructed, SizeConstructed and AddFileBoundary. They are explicit adapter checkpoints, not native object fields. The only admitted next boundary is AddFile; there is no loaded-image, addFile-success, graphic-transfer or delivered-receiver status in this API.

## Order and retained partial state

Before allocating the represented shell, the helper publishes an adapter metadata record into the caller's arena. Failure to grow that metadata collection therefore occurs before any represented shell existed. The normal admitted allocation constructs the eight-byte payload with privateData=null, reflecting QtGui `0x65014130`'s single qword store. That constructor assignment cannot throw.

The helper then records QSize width/height -1/-1, matching QtCore `0x6700ae00`, and prepares the exact resource request:

```text
resource = :/SfTool/icon/tool/iconDoe_24_24.png
size = (-1, -1)
mode = 0
state = 1
next = AddFile
```

The resource filename is not a24x24 decoded-size claim. Request preparation does not allocate QIconPrivate or an engine and cannot make the shell private pointer nonnull. A host exception during resource/request preparation leaves the earlier concrete shell and stage in the caller's arena; a later caller guard likewise does not destroy them. Ordinary arena destruction reclaims adapter storage only, not reconstructed native leak cleanup or Close.

The helper represents only admitted nonnull allocation/construction. It does not expose synthetic allocation-failure profiles, normal false returns or successful null icons. A host allocation exception can leave an incomplete AwaitingAllocation record with no payload. CP78's native constructor-unwind raw-deallocation distinction remains evidence, not a new injected constructor failure: this reconstructed field-store constructor has no throwing work. No caller should infer original allocator/null/fault parity from the host exception path.

## Guard and integration limit

At the normal boundary, guardMessage is:

```text
DOE QIcon shell: addFile QFileInfo/plugin/engine ownership remains unsupported
```

The API returns the prefix so a caller can inspect it, then stop at that boundary. A source consumer must retain the caller-owned arena across the guard. No native addFile is invoked or fabricated; its actual QFileInfo/suffix/plugin/global-counter/private/engine effects remain absent. The actual receiver's backend/map/graphic-item/old-QIcon ownership and delete-before-store continuation remain separate prerequisites.

This arena is explicitly caller-owned and isolated. It is not bound into ConversionSession and must not be presented as the canonical actual receiver allocation owner until shared source integration qualifies that context. No production caller invokes it, and no SetOprHasDoe population/backend/icon guard or DoeTableRefresh/public Type2 guard was released.

## Focused tests and static preflight

`tests/doe/test_icon_shell_prefix.cpp` contains five Testkit cases:

- Exact eight-byte/null payload and signed default size; stage and explicit AddFile boundary.
- Exact fixed resource/mode/state, with private data still null after request preparation.
- Separate allocations for repeated prefixes; prefix/shell observers remain stable as arena metadata grows.
- A caller-thrown unsupported guard retains the preceding allocation and request in its actual arena.
- No arena ownership back edge; dropping an explicit shared adapter owner expires its weak observer. This tests adapter lifetime only, not native icon/resource destruction.

These are authored checks, not executed results. The helper has no Session/notifier/FileEffects/Operation/graphic input or global interaction, and static inspection found no engine/resource lookup or receiver call. No generic callback interface was introduced to claim those effects.

Static preflight checked all quoted includes against actual target/local roots, Testkit main and supported active macros, five case declarations and absence of C assert/REQUIRE_EQ. `git diff --check` reported no diagnostics for the assigned paths; a direct whitespace scan also covered every new file and reported no trailing whitespace. The existing CMake CONFIGURE_DEPENDS glob discovers the new source/test without a shared edit.

Root target: `m2m_doe_icon_shell_prefix`; CTest: `m2m.doe.icon_shell_prefix`. No worker compilation or executable checks ran.

## Frozen sources

Paths are under `source/apps/moproj2mst/`.

| New file | SHA-256 |
| --- | --- |
| include/m2m/doe_icon_shell_prefix.hpp | 945C2860CB2FDA201E7BADD0AA9676A23DB32953D4FC4FD250ED68332A7EFDCE |
| src/doe/icon_shell_prefix.cpp | CB6820DDB8BFF6916798E4002D81AF926575C6B70118240059BFD9192C322B80 |
| tests/doe/test_icon_shell_prefix.cpp | FB9E0DF5DEAEEE11EF7943581373823D0A2320F0654A8F473832070CBAB04CD5 |

All three source files are held unchanged for root's source-stable grouped build/test/archive. The controlling evidence report `analysis/resume78-doe-qicon-ownership.md` hashes to `D8113389CD35B1E8C7F25CFB22462C8177F1BC9EC69B0400DC75484E8B3DFE7D` and binds the original GUI/Qt DLL hashes and bounded capture receipts.

The next state dependency is actual addFile QFileInfo/plugin/engine ownership for this nonempty resource, alongside actual receiver/population/backend admission before any public delivery. The shell constructor increment does not prove full icon behavior, fresh B121 parity or Type2 Save completion.
