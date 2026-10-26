# TaskInfo initialization and the next bounded segment

TaskInfo cannot yet execute in the native factory. The current factory retains
one Report allocation and stops before its public parentless QObject constructor
completes. The isolated preference helper does not advance that constructor,
initialize TaskInfo, publish a PIMPL, or register a prototype.

This pass qualifies the native TaskInfo field order and identifies an independent
value helper that can be implemented without changing that guard. No source was
edited and no build, executable test, original application, GUI session or mutable
REA operation was run.

## Source identity and qualifications

The immutable target is original `DEF_GUI_MO_64.exe` v11.0.2, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The selected QFont/QIcon code is from retained `QtGui4_64.dll`, SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
The assembly is offline immutable evidence, not a successful original runtime
observation. PE import-directory names were read directly to identify the calls.
Source-level class/field names below describe recovered responsibilities unless
an imported symbol supplies the name. Evidence hashes appear at the end.

The retained public QObject contract distinguishes Task's public constructor
`671313d0` from the protected constructor `6712e630`. Its shared empty containers,
TLS/QThreadData owner and reference increments remain prerequisites. The existing
guard must stay before TaskInfo even if an isolated field helper passes tests.

## Native order and field availability

At `141403229`, the Task base invokes public `QObject(parent=null)`. Only after
return does it install the Task vtable at `14140323c`, address Task+0x20, and call
TaskInfo wrapper constructor `14140e090` at `141403248`.

The wrapper first writes vtable `142e94038`, allocates 0x80 bytes, and calls private
constructor `14140e910` on the nonnull allocation. Wrapper+0x08 is written only at
`14140e0f6`, after that constructor returns. A suspended private constructor must
not expose its pending allocation as a committed wrapper PIMPL. The explicit null
allocation branch does not establish actual OOM or exception behavior.

| Private offset | Native operation and order | Availability after that operation |
| --- | --- | --- |
| +0x00, +0x08, +0x10, +0x18, +0x20 | Five ordered default QString constructors at `14140e92a`, `e93d`, `e950`, `e963`, `e976` | Null QString values, not five invented nonnull empty strings |
| +0x30 | Default QFont constructor at `14140e989` | First current application FontData identity; destination resolve mask becomes 0 |
| +0x40 | Default QIcon at `14140e99c` | Null icon private pointer; retained QtGui leaf `65014130` writes that pointer |
| +0x48 | QColor constructor at `14140e9c8` | RGBA arguments (255,255,255,255) |
| +0x58 | QColor constructor at `14140e9f3` | RGBA arguments (255,210,138,255) |
| +0x68 | QColor constructor at `14140ea1e` | RGBA arguments (235,169,62,255) |
| +0x7c | Byte store at `14140ea2c` | True; semantic field name remains unassigned |
| +0x30 again | Explicit QApplication::font at `14140ea36`; QFont assignment at `14140ea5c`; destroy temporary at `14140ea68` | Second current application font, including its own resolve mask, replaces the first font value |
| +0x40 again | Temporary default QIcon, assignment at `14140ea99`, destruction at `14140eaa5` | Null icon value remains, with the actual assignment/lifetime sequence retained |
| +0x28 | No write in `14140e910` | Unavailable here; later factory-backlink setter `14140e350` writes this field during registration |

Do not zero +0x28, gaps, or untouched tail bytes. Do not call the private object
complete before its second font read and icon assignment finish. QString/font/icon
value projections also do not prove native shared-data refcounts or unwind parity.

The PE imports identify `141e0b610` as QFont's default constructor, `141e0b960` as
the no-argument QApplication::font, `141e078f0` as QFont copy assignment,
`141e0bf28` as QFont destruction, `141e0b688` as QIcon default construction,
`141e08688` as QIcon copy assignment, `141e0b6a0` as QIcon destruction,
`141e0b9c0` as QColor(int,int,int,int), and `141e044c0` as QString default
construction. The offsets in the table follow the call receiver setup, not the
names of neighboring imports.

## First subsequent TaskInfo writes and read

Report metadata routine `1407b8b50` performs these writes on the committed actual
TaskInfo private object through its wrapper. They are not constructor defaults:

1. Private+0x00 receives `tr("General", null comment)` through `14140e1a0`.
2. Private+0x10 receives raw `SfTaskReport` through `14140e240`.
3. Private+0x18 receives `tr("Report", null comment)` through `14140e290`.
4. Private+0x20 receives a separate ordered `tr("Report", null comment)` through
   `14140e2e0`.

Translations use the actual `SfTaskReport` metaobject and the shipped two-string
QMetaObject::tr overload. No English fallback or merged repeated translation call
is justified. Private+0x08 remains its null-string constructor value in this
selected metadata routine. Private+0x28 still does not become an invented null.

In the selected preference path, the first read is `1414091d6` calling Task name
getter `1414037b0`, which addresses Task+0x20 and calls `14140e5d0`. That getter
copies private+0x10 using imported QString copy construction. The settings key
therefore starts with raw `SfTaskReport`, not a translated display string. The
first suffix is `/exeFileName`; native key conversion and settings owner remain
separate prerequisites. The isolated accessor added for candidate48 cannot supply
those missing lifecycle/backend prerequisites by itself.

## One immediately implementable segment

Implement only an explicitly isolated **TaskInfo default-font value helper** in
the DOE-owned task factory files. A suitable interface is conceptually:

```cpp
engine::WidgetFontValue isolatedTaskInfoDefaultFontValue(
    const engine::WidgetFontValue& firstCurrentApplicationFont);
```

The helper copies the exact existing `WidgetFontDataState` shared owner and sets
only the destination `resolveMask` to 0. It must not allocate/intern another font
data owner, mutate the supplied font, query settings, construct a TaskInfo wrapper,
publish wrapper+8, mark `qobjectBaseCompleted`, or invoke the native factory. It is
the value projection of the first default-QFont call, not a reconstruction of its
atomic refcount/temp-destruction effects or the whole private constructor.

The source for this segment is the retained default QFont body `651f3560`: it calls
QApplication::font at `651f3580`, copies its FontData pointer at `651f3589`, performs
the native reference/temporary-lifetime work, and writes destination mask 0 at
`651f35c6`. A null FontData pointer is explicitly tested in that body. An already
supplied value with a null data pointer may therefore be copied with mask 0;
an **unavailable observation** must not be manufactured into such a value.

Ownership is precise: the input value belongs to the actual application's shared
`WidgetRuntimeState`; the result holds the same shared FontData owner. For an
isolated test only, construct an explicit runtime observation fixture and obtain
its interned `WidgetFontValue`. An alias integer alone is not ownership and must
not create a second FontData object. This helper needs no new root-owned runtime
API or source file.

The existing runtime `applicationFont()` returns its stored default-font value,
and `observeStartupValues()` accepts its startup observation once. That API does
not itself prove the font is still current at either later TaskInfo call site.
Consequently the native factory must not call this helper until the same owner's
current first-read value is qualified at that source position. The second read
is independent and must later preserve its own mask; this helper must not be
reused to force that second value's mask to zero.

The focused test belongs in `test_doe_task_factory.cpp` and must be explicitly
isolated. Give the fixture font a nonzero mask, verify returned FontData pointer
and shared-control-block identity match the input, destination mask is 0, and the
input mask/data are unchanged. Verify a second independently supplied application
font keeps its distinct identity; the helper cannot return a cached earlier font.
Also distinguish an explicitly supplied null-data value from an unavailable
optional observation. Preserve all existing native factory tests and guard text.
Root owns compilation and execution. Passing these checks establishes only this
value rule.

## Native integration dependency and future frame

After the shared QObject prerequisite closes, the actual pending Report allocation
must retain an in-place constructor frame: wrapper-vtable stage, stable pending
0x80 PIMPL allocation, five initialized string values, first font, later fields,
second font/icon stages, then wrapper publication. Reuse the existing
`TaskAllocationStore`/`PendingTaskPrototype` identity; do not replace it with the
bounded parsed `SessionTask`, whose DOE lists are unrelated Report defaults.

A later integration test should stop on unavailable first font after the five
string constructors, retaining the pending private allocation and leaving wrapper
publication unavailable. A stop on the second font must retain the first font,
icon, colors and flag already established, again without publishing the wrapper.
Once-only resume must not replay allocation/field effects. Those tests require the
shared QObject/private/Qt ownership seam and stage-aware frame, so they are not
claimed as immediately executable by the isolated helper proposal.

No source changes, build or tests were performed in this static work unit. The
source/evidence verification receipt below is an offline consistency check,
not original runtime acceptance or lifecycle completion.

## Evidence and verification

<!-- Evidence SHA-256 table appended by the bounded verification script. -->

Verification UTC: 2026-10-24T17:55:51.645859+00:00.

Static checks passed: ordered PIMPL call sites; private constructor call before wrapper publication; default QFont data copy before mask-zero store; candidate48's three frozen helper/test files remain unchanged. No executable test was run. Token measurements are unavailable.

| Evidence | SHA-256 |
| --- | --- |
| `analysis/wave2-doe-task-base-public-qobject-call.asm` | `9ab0fc19b72c28c16af2c61d9e303e7962ba015b9da6ec4294607662d18d6428` |
| `analysis/wave2-doe-task-prototype-taskinfo-ctor.asm` | `7f395fce24a655724492b720cefd36664223c47a84f99b208456e446d40a1ada` |
| `analysis/wave2-doe-task-prototype-taskinfo-private-constructor.asm` | `172e1daaf0e6d57fde2153eb64694da2486bc4500d2d840b5f064000b83d980a` |
| `analysis/wave2-doe-task-prototype-qt-default-font.asm` | `1e816c4d5df5edc117db15ae602bc7ad75fbe283c4353614eb2ef356786c81f9` |
| `analysis/wave2-doe-task-prototype-qt-empty-icon.asm` | `9cc3ced9bb77a9e0f8ce91d1b1b70bd8866987324854edbf03117e46dbd00752` |
| `analysis/wave2-doe-task-prototype-report-metadata.asm` | `26b7f5bbf82f3da49fd905c7e8e905778a7eb32a7c2b2577b2f619d74af8e5b7` |
| `analysis/wave2-doe-task-prototype-report-translation.asm` | `0a4741c57908cdc4a50c7e5a5e46ec341d291f292a756103abf3aedba34bd58b` |
| `analysis/wave2-doe-task-prototype-info-title-setter.asm` | `2c53e4daf814c6a45027774619d40fc84853c4508f59d64d00c3a8ae3dd4de84` |
| `analysis/wave2-doe-task-prototype-info-name-setter.asm` | `dde7308f92f6c3b281fc0f52b212fdbe7032bee83cb97a905716537522220d85` |
| `analysis/wave2-doe-task-prototype-info-text2-setter.asm` | `85f530052c0f57e7a5033fe021f894c28640376ae8afb8f235356899ebee3ef6` |
| `analysis/wave2-doe-task-prototype-info-text3-setter.asm` | `3c73afff513f3a42db190eee1b5b683f466579efde00bb8dedf3c9af099c7438` |
| `analysis/wave2-doe-task-prototype-taskinfo-name.asm` | `85f0d0060bbd7e230aa0ee179e8bce5e5a085d70385d19c42c85e04466422c78` |
| `analysis/wave2-doe-task-prototype-taskinfo-factory-owner.asm` | `530714aff780f6262bce07e9bed95e6468d9e28fa6b56ef6a2c214c027f2f275` |
| `analysis/resume46-doe-preference-entry.asm` | `b1b2dc50fb8c2e1b73cde9dbdbab5b78467062af608d3c439b4977a632de42fa` |
| `analysis/resume46-doe-preferences.asm` | `7cf0d134d7f7cdfe8c22700b6986c0df8312008cc8a000ac4c0c1047a8b333fc` |
| `analysis/wave2-doe-task-factory-prefix-api-proposal.json` | `d0be839928cc62bffe213dba3c6263772f57c3b7f4904c987e454aa008e1d6e4` |
| `analysis/wave2-doe-task-public-qobject-ownership-contract.json` | `81e05b0f2aee8014da834c2a3db46df6eea09a6a01e6744498e4ee3d9afd9a32` |
| `analysis/wave3-doe-report-qobject-prerequisite-contract.json` | `d132044c91c93b2b6b8017acb11f05451e4ef3b4896f83f158aa055d379a5998` |
| `analysis/resume48-doe-preference-helper.json` | `6e2264c4b15750eb1da1cd59c858069eb9065f772982638dcb4a244f3c7d48ca` |

| Current source reviewed | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d59535b1771173dfdbdd2bef5ae6637867336544e3487a4d4b683dcab6274ae` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `4579d41d3e28b6c1546b810720dde5fd6260343c8a7c3312c2a73d926f72abd3` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `5580c19c093ecbd9115499b8a3c9d2d7cbf9d3a596bb2f167f5e9e1d88f169a5` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `a3c7cff4a57a51b84c8cf4dd17999f8790c20e9448aada9eae70bd8456f4175a` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `69e4b6f1e88644502abb047824db88bab7d511d7dc76c3f3943986f1d0ea66b7` |
