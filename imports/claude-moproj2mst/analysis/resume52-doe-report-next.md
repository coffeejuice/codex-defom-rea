# Report QObject: exact missing owners and next isolated step

Keep the active Report guard. The two font-value helpers do not qualify QObject
private initialization, TLS selection, thread references or constructor identity.
One shared symbol now has partial ownership support in current source, which
must be reused rather than duplicated. The smallest independent implementation
ready next is the private+0x20 bit transformation described below.

This report is the only write. No source edit, build, original execution, GUI
control or mutable REA operation was performed. A new source increment is held
until the parent releases its checkpoint50 freeze.

## Correct the historical capture-plan RVAs before using it

The immutable QtCore PE has preferred image base `0x67000000`, SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
Subtracting that base from the assembly addresses gives:

| Native point | Preferred VA | Correct module RVA |
| --- | --- | --- |
| Public QObject constructor | `671313d0` | `1313d0` |
| Private initializer | `67131160` | `131160` |
| First QList shared-null increment | `67131190` | `131190` |
| QString shared-null increment | `671311ad` | `1311ad` |
| QVector shared-null increment | `671311d2` | `1311d2` |
| Second QList shared-null increment | `671311de` | `1311de` |
| Current-thread-data getter | `6702aca0` | **`2aca0`** |
| TLS initializer | `67029600` | **`29600`** |
| TlsGetValue call | `6702acbc` | **`2acbc`** |
| Thread-data ref helper | `67027630` | `27630` |
| Actual public constructor call to ref helper | `67131456` | `131456` |
| QObject counter increment | `67131488` | `131488` |

Historical `wave3-doe-report-qobject-capture-plan.json` incorrectly names
`0012aca0`, `00129600` and `0012acbc` for the thread getter, TLS initializer and
TlsGetValue call. Those are each 0x100000 too high. Its private-initializer
sub-offset wording also needs the full explicit RVAs in this table. Preserve
that file as historical evidence; use the corrected module-relative points in
any new script and verify the instruction bytes against the loaded module.

For the executable caller, the actual call is `141403229` (EXE RVA `1403229`),
after EDX is zeroed at `141403222` and RCX reloaded at `141403224`. Capturing at
the actual call gives the initialized parent argument and intended object
identity. Do not mistake the setup instruction for the call itself. Runtime
addresses must still use the freshly recorded loaded module bases.

## Exact shared-null fields and order

Private initializer `67131160(private,0x40804)` installs these pointers and
increments the referenced 32-bit atomic words in this exact order:

| Private field | QtCore shared symbol RVA | Increment instruction | Effect |
| --- | --- | --- | --- |
| +0x18 | QListData `2f0a50` | `131190` | +1 |
| +0x30 | QString `2f0b30` | `1311ad` | +1 |
| +0x68 | QVectorData `2f0bb8` | `1311d2` | +1 |
| +0x70 | SAME QListData `2f0a50` | `1311de` | +1 more |

The two QList fields must share one owner and total +2, not two independent
copies of a presumed initial count. QString and QVector each add +1. Absolute
physical counts are not the file's initial words or host shared_ptr counts.
Where qualified symbolic deltas are used, retain explicit unknown bases and
associate every effect with the same native symbol/runtime and constructor frame.

The initializer also writes the qualified null fields and masked private+0x20
word; it does not zero the whole 0x88 allocation. Public construction later
publishes object+8, writes private+8 back to the SAME Report identity, selects
thread data, stores it to private+0x40, retains it, then increments QObject count.

## Partial current-source progress: reuse the QVector symbol

Current `WidgetRuntimeState` now owns `gradientSharedNull`, qualified by exact
QtCore SHA-256 and symbol RVA `2f0bb8` in `observeGradientInputs`. That is the
**same QVectorData::shared_null symbol** used by QObject private+0x68. Earlier
statements that no shared-null owner exists are superseded for this one identity.

The present interface is still gradient-specific:

- `gradientSharedNull(const WidgetState&)` requires a live actual widget's private
  input binding and that receiver's explicit gradient shared-null qualification.
- The owner is `BrushGradientStopDataState`, with private `retainValue` and
  `releaseValue` operations accessible to `BrushGradientStopsCall`.
- Its `modeledValueReferences` is a size_t count of modeled gradient vector-value
  references. Its physical atomic count is explicitly unknown; the immutable
  file's initial ref1 is not imported as current state.

A pending Report is a QObject, not that WidgetState receiver. Do not fabricate a
widget/gradient binding or create a second shared-null owner to obtain access.
Root-owned integration must expose a common symbol identity/ref-effect seam
while preserving the existing gradient owner's identity and effects. Merely
obtaining another diagnostic shared_ptr does not perform QObject's +1.

No corresponding actual-runtime owner/seam for QListData RVA2f0a50 or QString
RVA2f0b30 is established by the current reviewed source. The vector improvement
therefore does not close the entire private initializer.

## Exact TLS/QThreadData fields still missing

For the source-qualified existing-index/non-null fast path:

1. `67029600` checks QtCore dword RVA `2f08e8`; if it is not `ffffffff`, it returns
   without the additional TLS-index allocation/lock path.
2. `6702aca0` reads that index and calls imported TlsGetValue at `6702acbc`.
   A nonnull return branches directly to its return at `6702ae42`.
3. Public QObject stores that exact pointer into private+0x40 at `6713144e`.
4. At `67131456`, RCX is the selected QThreadData pointer. Helper `67027630`
   increments **dword at QThreadData+0** with LOCK ADD1, then returns.
5. Only afterward, public QObject increments QtCore dword RVA `2f4344` at
   `67131488`.

Required missing state is therefore the actual qualified TLS index, its actual
nonnull result, one QThreadData identity bound to the existing thread owner, and
its dword+0 ref observation/delta. The same-call private+0x40 and helper RCX must
match. The parentless/non-null-TLS fast path does not require inventing values for
QThreadData+8, an OS thread ID field, adoption records or unrelated thread fields.

Current `WidgetThreadState` models notify-depth and posted-event queue state,
not TLS or QThreadData. Neither notify-depth nor an OS thread ID identifies that
pointer. A missing index or null TLS result follows additional allocation/adoption
branches and remains guarded. The existing QObject counter seam cannot certify
those earlier effects.

## Smallest useful future capture

When original execution is available to its single owner, restrict the capture
to one actual first-Report constructor frame and use the corrected points above:

1. Record actual executable/QtCore module identities and bases, thread/call
   ordinal, Report RCX and null parent at EXE RVA1403229. Verify the true DOE
   factory call path; startup/editor evidence is not a substitute.
2. Track the 0x88 private allocation and the four exact shared-symbol increments,
   with before/after dwords and symbol identities. Verify +2/+1/+1 attribution;
   retain ambiguity from concurrent/nested activity rather than normalize it.
3. Read index RVA2f08e8, actual TlsGetValue result, and the selected pointer.
   Stop qualification on a missing-index/null-result branch rather than assume
   the existing fast path.
4. Bind private+0x40, helper RCX and QThreadData+0 before/after +1 to that same
   call. Record the later QObject count and object/private backlinks separately.

No target calls, Qt event pumping, memory writes or GUI action are justified by
this plan. Root controls any eventual process/capture and its cleanup. This report
does not launch a process or establish a usable app-control release lifecycle.
Another static disassembly cannot supply actual TLS values or runtime counts.

## Dependency-ready isolated helper

The private initializer's local word transformation is source-closed and needs
none of the missing global owners:

```cpp
std::uint32_t isolatedQObjectPrivateFlags(std::uint32_t previousWord);
// Result: (previousWord & 0xfffffcf0u) | 0x60u
```

Source `67131224..6713122f` loads private+0x20, ANDs `fffffcf0`, ORs60 and stores
the result. This means known-result mask `0x36f`, value `0x60`; every bit outside
`0x36f` retains its old value. The helper should accept an explicit supplied word,
return the transformed value, and neither read a native private pointer nor
manufacture an unknown old word as zero. Native integration still requires that
old word to be available at the actual initializer stage.

Focused tests should establish cleared bits0..3/8..9, forced bits5/6 and preserved
bit4/7/high bits independently. Useful source-derived examples are0->60,
ffffffff->fffffcf0 and80000010->80000070. Test a supplied high-bit pattern and
confirm the input remains unchanged. Do not interpret this pure helper as
initialization of a complete QObject private object or permission to replay the
shared reference effects.

Use existing DOE-owned helper/test files after the checkpoint freeze releases;
leave all active Report/TaskInfo/public guards untouched. That is the next
independent implementation candidate, not an enabled active-study path.

## Verification and evidence

The following offline receipt records exact opcode/base checks, current source
bindings and retained evidence hashes. No runtime acceptance is claimed.

<!-- Offline verification and evidence table appended below. -->

Verification UTC: 2026-10-24T18:56:09.418535+00:00.

Static checks passed: QtCore PE preferred base and full DLL hash; exact selected flag-transform/thread-ref opcodes; historical plan RVAs differ from the source-derived points; mask invariants for all32 one-bit input patterns; current guard retained; current shared vector symbol has exact RVA/module check; thread class still lacks TLS/QThreadData fields. Current source is reviewed working source, not an assertion that every concurrent checkpoint50 edit has completed validation.

| Opcode selection | RVA | Exact bytes | Byte SHA-256 |
| --- | --- | --- | --- |
| `0x67131224` (14 bytes) | `0x131224` | `8b432025f0fcffff83c860894320` | `841574798a97e10cba0dcfe2aa6864bfb1735a61a94c13d4ba6c13b21ef6fb45` |
| `0x67027630` (5 bytes) | `0x27630` | `f0830101c3` | `f341ffb307a86945ccc4bb2416c97327358a809b723666abc0d6633ba8e1be25` |

| Retained evidence | SHA-256 |
| --- | --- |
| `analysis/wave2-doe-task-qobject-private-initializer.asm` | `beb76f3a7a4b089aa59dc17336c9ae41fb1498fa9fe92e440b3538eb6ff4aa07` |
| `analysis/wave2-doe-task-base-public-qobject-constructor.asm` | `945960cbfd88a502ae6797cc14b7c37d5d4094818883a2ca8dfea2735462f207` |
| `analysis/wave2-doe-task-qobject-current-thread.asm` | `bb4f6c61aa4879b3a4b7b9a7463f690e2f7a78fadf08dd0e7b60c39c45b29151` |
| `analysis/wave2-doe-task-qobject-thread-ref.asm` | `cfbc1216143573a6aa3c18952a408ec06d5cad139236ee060496f85b1a439e43` |
| `analysis/wave2-doe-task-qobject-thread-tls-initializer.asm` | `c06368be2e4a56cb27af21608f344fa868c383eca125e3ffd1838cd1373014b3` |
| `analysis/wave2-doe-task-public-qobject-ownership-contract.json` | `81e05b0f2aee8014da834c2a3db46df6eea09a6a01e6744498e4ee3d9afd9a32` |
| `analysis/wave3-doe-report-qobject-prerequisite-contract.json` | `d132044c91c93b2b6b8017acb11f05451e4ef3b4896f83f158aa055d379a5998` |
| `analysis/wave3-doe-report-qobject-capture-plan.json` | `feb2f1a14fa2216e21aee8a9775bb8bca20cb73ff9811597f585a2aa41b706db` |
| `analysis/resume50-editor-gradient-binding.md` | `0a35ae4f9c84d4fae0ad8180ff97b339cd3586469dd68af36b24479b460ab5fe` |
| `analysis/resume50-gradient-helper-review.md` | `4d4a82408b078a59d1b58e435559b96a0877ad1142a477169fce43b07cd31a47` |
| `analysis/resume50-doe-taskinfo-binding.md` | `576859764a6c123181424f1e33a34103bb393350d8a068b06d9342339c0de839` |
| `analysis/resume51-doe-second-font-helper.json` | `bfc44b5eaca2daf5257a9e1b67a3607a12da310851fdb0e98e8821e2951b778c` |

| Current source reviewed | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d59535b1771173dfdbdd2bef5ae6637867336544e3487a4d4b683dcab6274ae` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `058761d59a0cd6d67c9ddbcad6fa59ad608c1046fbf523de2ee5def9dc9a3fcd` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `88a0a355496a69d515d59affb01e4543b8ac7206cd1764a8302e1d6130830d7d` |
| `source/apps/moproj2mst/include/m2m/engine/widget_events.hpp` | `e75a9f7631dab2202a894310e94adfbda0906232074a6b57c8ab1531cd55b4ec` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `cf76337e9f390a321a02031e4c625368dde95315c7acfe62a01c7248a4d8ce3c` |
| `source/apps/moproj2mst/src/engine/view_manager.cpp` | `d520ad1fb30bed0ed10f6e579d4d021466b488ff282b5b333863496458004976` |
| `source/apps/moproj2mst/include/m2m/engine/brush_gradient_stops.hpp` | `adc6f78674c0706586a8d9dd442f6c42a41ca8adc4f4eb5e0dff3b3e3524c8f2` |
| `source/apps/moproj2mst/src/engine/brush_gradient_stops.cpp` | `f469297380f2bafb762313f9c419b3587ad4593043aae135c6c9ccfb465e1958` |

Token measurement is unavailable. No speed or token-saving claim is made.
