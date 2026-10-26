# Binding the empty-gradient value to the actual editor brush

The first implementable hook is runtime observation/interning, before editor
dispatch: extend the qualified private-constructor observation so the existing
`WidgetBrushDataState` can reference its actual embedded gradient source value,
and let the same `WidgetRuntimeState` own one qualified QtCore shared-null symbol.
Only then should editor style15/17 invoke the isolated stop-vector helper.

Owner: `editor300`. This report is the only write. No source/build/GUI/original
execution/mutable REA was used. Existing helper/editor source hashes remained
`509e0a226325ee99a856fe1ca9eeaf13b81a984d7a79554f7fcfd67aa693db86`
and `7ee9165d16dd0351024bead83e219c9a4c9fb1590a1bdbc955033a39c190f2bd`.
Token measurement is unavailable.

## Native identities and reached reads

Retained QtGui identity is SHA-256
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Read `resume48-gradient-state-contract.md`, `resume48-editor-next.json`,
`resume49-editor-next-contract.md`, and the complete captured normal opacity
region `wave3-editor-parentless-opacity-6511c090.asm` with their qualifications.
Probe004 observed style1 only; it provides no production gradient binding.

| Value | Required same-owner link |
|---|---|
| Actual editor component | `StagedEditor300::widgetState()`, live native identity equals factory |
| Runtime/thread | existing `EditorBaseConstruction` runtime and current-thread owner checks |
| Current palette | actual widget palette and resolved current group, Window role10 |
| Brush data `B` | runtime-interned owner from that palette slot; already weakly retained after alpha |
| Embedded gradient | native `B+70`, not an independent editor gradient object |
| Source stop owner | pointer read at gradient+8, therefore `B+78` |
| Source count | signed32 at stop owner+8; exactly0 admits the implemented helper profile |
| Shared-null symbol | QtGui IAT `65696b40` resolves QtCore `QVectorData::shared_null`, RVA2f0bb8 |

The source may be that same shared-null owner. Do not create an ordinary alias
owner and a second symbol owner for one native identity. Distinct brushes may
also share a non-symbol source owner; equal content does not establish aliasing.

Unknown source pointer is not a known empty vector. A qualified null pointer
must not become shared-null: the native getter dereferences it. Negative and
positive counts remain the helper's explicit nonzero guard. Zero source count
does not consume source capacity, flags, payload or a source-value ref increment.

## Runtime observation hook

`WidgetRuntimeState::observePrivateConstructionInputs` already validates the
actual live widget/component/runtime and a stable nonempty qualification. Its
palette/brush alias maps and slot checks provide the appropriate identity path.
The new binding should be an extension of that path or an equally qualified
method, not a new `EditorBaseContext` success boolean.

The shared write set needs:

1. Runtime-local stop-data aliases and unknown-to-known/equal observations;
   one source-value object on each actual brush's embedded gradient; and one
   shared-null symbol owner for the runtime. A tagged symbol link should unify
   source references with that owner without inventing an observed numeric alias.
2. An explicit brush-alias-to-source-owner link. Reject absent brush aliases,
   conflicting links, duplicate aliases, foreign control blocks/runtime owners,
   expired widgets and mismatched qualifications before committing new links.
   Do not assume the existing broad observation method is globally atomic;
   validate this new batch before changing already-bound gradient owners.
3. Shared-null profile qualification for its actual module/symbol and reached
   header. The isolated factory supplies a source-qualified header profile; it
   is not evidence that an arbitrary live address currently has those fields.
   Preserve unknown physical atomic references. Never derive them from host
   `shared_ptr::use_count()` or the immutable file's initial1.

This runtime hook can be tested without admitting the editor branch. Useful
cases are two palettes/brushes sharing one source owner, source equal to the
symbol owner, unknown source/count, conflicting late observation with no partial
link change, foreign-runtime rejection, and widget death before observation.
No existing solid-brush or startup receipt may seed a missing gradient link.

## Editor continuation after binding

The subsequent editor-owned hook is the existing `BrushStyleDispatch` stage,
for style15/17 only. The actual brush alpha was already consumed once. Allocate
one retained `BrushGradientStopsCall` using that brush's bound source and the
same runtime's symbol owner; preserve it across missing count/nonzero/profile
guards. Do not perform a fresh palette lookup or restart the QObject/group/alpha
prefix during a retry.

Once the getter returns, read its actual returned vector payload in order,
compare each QColor byte5 with255, and explicitly destroy the returned value
before resolving widget opacity. For the implemented zero-source profile these
are the reconstructed default black/white stops, not an observed `opaque=true`
answer. Missing or nonzero source state must not execute that scan.

The required normal effect order is:

`BrushAlphaConsumed -> source count -> local vector/default allocations/appends
-> returned-value retain -> local-value release/destruction -> ordered stop-alpha
scan -> returned-value release/destruction -> PaletteOpacityResolved -> optional
opaque bit17 store -> native-opacity helper -> shared QWidget tail`.

Retain a cursor after returned-value destruction so later raw120/native-helper
guards cannot reconstruct or release it twice. Equal opacity skips the native
helper. Runtime replacement, native-widget death, style16, style24, unknown
source state, nonzero source, native-window paths and public6504abb0 retain their
separate guards. Helper success establishes no QWidget wrapper/setup completion.

## Additional immutable QtCore qualifications for helper review

Read-only PE inspection used `evidence/original-binaries/QtCore4_64.dll`, SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, and dumpbin
`C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`,
tool SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
The exports name grow at RVA7c5a0 and shared_null at RVA2f0bb8.

`/disasm:bytes /range:0x6707c5a0,0x6707c5d9` contains normal returns and all
branches of the grow helper. Native byte SHA-256 is
`dad43aa3b6ee8c5314929fc50526cf419a0ab918a2587d878270b8986bf8dc1c`.
Its reached `r9=true` branch computes `n+abs(n)/2`; only positive n1 and n2
are used here, producing capacities1 and3. No qAllocMore branch is reached.

The 16 file-backed bytes at shared_null RVA2f0bb8 are
`01000000000000000000000001000000`, SHA-256
`288b1793833bb59673891be54d485696d3f7d8ff9a9168758ef9d00b1bca8985`:
initial ref1, capacity0, count0, flags1. These are immutable file initializers,
not a live atomic-count capture.

The first append requests capacity1. QtGui `6511a8c2..a8c5` compares that with
the local shared-null capacity0 and jumps to allocation on inequality BEFORE
the ref-count test at `6511a8c7..a8d4`. Thus allocation does not need a current
absolute shared-null ref count. The helper tracks only its introduced value
ref and releases it; assuming that release cannot free static shared-null belongs
to the qualified ordinary Qt symbol-owner profile, not an arbitrary malformed
reference counter. The reviewer has been given these exact locations/hashes.

No new native execution was performed, no current gradient runtime input was
observed, and no allocation-failure/exception-unwind parity follows from this
binding contract. The next concrete implementation is the shared runtime
observation hook above, followed by an independent editor binding increment.
