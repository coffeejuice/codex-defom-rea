# Empty-gradient helper review

**Accept the isolated helper's normal successful-allocation, exactly-zero-source
branch. No concrete implementation defect was found in shared-null reference
deltas, allocation order, copied fields or returned-value lifetime.** This is a
static review acceptance, not runtime parity, test execution, shared-widget
integration, or public mode300 admission.

Reviewer: `gradient_state`. Exclusive write: this report. No source changes,
builds, original execution, GUI control, or mutable REA. Read-only retained PE
inspection supplemented the existing source qualifications. Token measurement
unavailable.

## Reviewed freeze and evidence

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/engine/brush_gradient_stops.hpp` | `d940515ec26683c66031beaa028d793fc2cb8fc49490dc25be0784512ae81491` |
| `source/apps/moproj2mst/src/engine/brush_gradient_stops.cpp` | `509e0a226325ee99a856fe1ca9eeaf13b81a984d7a79554f7fcfd67aa693db86` |
| `source/apps/moproj2mst/tests/mdt/test_brush_gradient_stops.cpp` | `ac9daca8526e7f5451d13bae0a579064a66ba4c90f319ac8ad8909e59a3c9eb9` |

Controlling evidence qualifications were read in
`analysis/resume48-gradient-state-contract.md`,
`analysis/resume48-editor-next.json`,
`analysis/resume49-editor-next-contract.md`, and the retained
`analysis/wave3-editor-parentless-opacity-6511c090.asm`.

Original QtGui4_64.dll SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Previously qualified helper native regions were independently rehashed:

- `6511a860..6511aa26` exclusive: byte SHA-256
  `ea1074ee442a45f3b34c812f9a221f3b888ca73fa24e3198a8e9c5b4a8ce14ed`.
- `6511ae60..6511af78` exclusive: byte SHA-256
  `eebca89f2b89371d13dd139af7c9ea65bb331caa5517980efae9e41a4b850624`.

Independent QtCore4_64.dll inspection identified:

- Full DLL SHA-256
  `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`,
  preferred base `67000000`.
- PE export `QVectorData::grow(int,int,int,bool)` at `6707c5a0`.
- PE export `QVectorData::shared_null` at `672f0bb8` (RVA `2f0bb8`). Its
  file-backed 16-byte header is
  `01000000000000000000000001000000` (ref1, capacity0, count0, flags1),
  byte SHA-256
  `288b1793833bb59673891be54d485696d3f7d8ff9a9168758ef9d00b1bca8985`.
  The file's initial ref1 is **not** a captured current runtime atomic count.
- PE exports `QBasicAtomicInt::ref` at `67001aa0` and `deref` at `67001ac0`.
- `grow` bounded native bytes `6707c5a0..6707c5da` exclusive, including normal
  branch RETs, SHA-256
  `dad43aa3b6ee8c5314929fc50526cf419a0ab918a2587d878270b8986bf8dc1c`:

```text
40534883ec20418bd8448bd24584c974108bc2992bc2d1f84103c24883c4205bc32bcb440fafd38bd1418bcae8cf04fbff99f7fb4883c4205bc3
```

The disassembly was read with the same dumpbin identified in the earlier
contract, using `/disasm:bytes /range:0x6707c5a0,0x6707c60f`. Only the complete
grow function through `6707c5d9` is interpreted; adjacent code is excluded.
The true branch `6707c5b1..c5c0` computes `n + trunc(n/2)`; n1 gives1 and
n2 gives3. This read does not execute the original DLL.

## Shared-null count and forced allocations

The source count check in helper lines94–100 agrees with `6511b3b4`: exactly
zero selects default construction. It does not require the source's flags,
capacity, payload or external-reference count. Negative and positive counts stay
behind the distinct nonzero-source guard. In particular, an observed source
count0 is not treated as an observed empty payload.

The local vector takes a separate reference to the actual shared-null symbol at
`6511b3be..b3cd`. Helper lines108–110 model that added reference, without
inventing its current absolute atomic count. The default append still must
allocate capacity1 regardless of whether the local vector's ref-count comparison
in `6511ae87` reports1: if it is1, the subsequent required-count1 versus
capacity0 comparison takes the same growth path. In `6511a860`, old capacity0
differs from new capacity1 (`6511a8c2..a8c5`), forcing allocation before any
ref-count==1 fast-path could bypass it. Thus unknown absolute shared-null
reference count does not make this allocation decision unknown.

The native valid-symbol profile retains the shared-null symbol's existing
baseline ownership; the added local reference is released after the new
allocation replaces it. Helper count0->1->0 is a delta ledger, not an assertion
that the physical native atomic count becomes0. Keeping the symbol alive is
consistent with that qualified profile. The runtime adapter must still bind the
actual shared-null symbol/stage and header profile, as its header comment
requires. A fabricated symbol, raw corruption or a freed sentinel is not admitted
by this review.

The second append needs count2 while capacity1, so it grows to3. Again capacity
changes, so `6511a860` allocates a new owner rather than modifying the old one in
place. The first constructed allocation has one modeled local value ref and
zero external refs; dropping that ref frees it after its first stop was copied.
There is no need to seed a physical atomic count from a host shared_ptr registry.

## Allocation and release order

| Native normal-path event | Helper implementation |
|---|---|
| Ref shared-null before either append (`6511b3cd`) | lines108–110 |
| Allocate `16+24*1 =40` bytes; assign count0/ref1/cap1/flags1 (`6511a8db..a938`) | lines115–120, first constructed owner/ref |
| Drop local shared-null ref and replace pointer (`6511a9da..aa17`) | lines121–122 |
| Append position0, black QColor14 (`6511b45f`, append helper) | lines123–124 |
| Allocate `16+24*3 =88` bytes, preserving prior flags bit1 | lines126–131; known shared-null/default bit1 is0 |
| Copy existing stop before releasing old owner (`6511a97e..a99d`) | lines132–133 |
| Drop/free first allocation before writing new white stop (`6511a9da..aa17`, then append) | lines134–138 |
| Retain returned value before dropping getter-local value (`6511b47e..b4af`) | lines142–146 |
| Caller drops/frees returned vector (`6511c278..c28f`, all-opaque path) | explicit `destroyReturnedValue`, lines152–159 |

The default's bit0 is1, so the returned-value shareability test does not detach
again. The returned owner has one modeled value ref after local destruction,
then becomes dead only after explicit caller destruction. Diagnostic shared_ptr
leases in `allocatedData()` and `returnedData()` correctly do not prolong native
vector lifetime. Destructor cleanup is explicitly qualified as host cleanup,
with allocation-failure/exception parity unclaimed.

The two default records match the retained native QColor construction: positions
`0000000000000000` and `3ff0000000000000`, full QColor alpha words, black then
white, and no copied native pair padding. The source gradient's original data
pointer, zero count, flags, capacity and external refs are not replaced.

## Validation and integration limits

The authored tests cover source/count guards, exactly-zero versus negative,
unknown untouched source fields, header mismatch, capacities1/3, meaningful
record bytes/order, single returned-value destruction, distinct results for two
calls, source equal to shared-null, owner/control-block rejection, and diagnostic
leases. I did not run them or a build under this review assignment. The current
tests mostly inspect final records and effect counts; a full ordered effect-list
assertion would make future accidental reordering easier to catch. That is a
coverage improvement, not a defect in the reviewed code's current order.

This remains a value helper. Runtime symbol/alias interning and live
WidgetBrushDataState binding are not supplied here. The editor must retain its
actual returned vector value through the alpha scan, destroy it before changing
opacity, and preserve its native-opacity/retry cursor. Nonempty gradient,
radial, texture, allocation failure and public300 guards remain separate. Passing
helper tests does not establish those integrations or full lifecycle parity.
