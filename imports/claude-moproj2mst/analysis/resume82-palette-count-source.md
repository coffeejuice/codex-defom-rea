# CP82 palette count producer and private temporary-release witness

The native source establishes an initial atomic word of 1 for newly allocated
palette data, and the retained splitter003 probe establishes later current words
on the actual shared application-palette data. Neither establishes the current
word at placeholder0's earlier private temporary deref. CP81 must retain
`PaletteRefUnavailable` there. The smallest missing input is one qualified read
of that SAME data's first dword immediately before the imported deref, already
including the getter's native retain.

Only this report was written. CP81 source stayed frozen. No source, build, test,
GUI, original execution or mutable REA operation was performed. The investigation
used retained captures and bounded DUMPBIN views of imported original DLLs. No
reconstructed executable or deployment was inspected or changed.

## Native owner and ordered mutation

The atomic belongs to QPalette DATA, at data+0. It does not belong to the
application wrapper, widget, host shared_ptr or QWidget instance counter.

The selected QWidgetPrivate call at preferred VA `6505e8d4` invokes QPalette's
default constructor `6504abb0`. Its call at `6504abbe` obtains a temporary through
`QApplication::palette` (`65016f70`). The known-present application slot is
QtGui RVA `9c9690`. The getter copies its source through `6504a8b0`, whose
`6504a8c6` native ref is the temporary's retain. The subsequent constructor
sequence is:

| Preferred VA | Operation on the same palette data |
|---|---|
| `6504abc3` | Load local temporary data from `[rsp+20]` into RCX |
| `6504abc8` | Load getter output data into RDX |
| `6504abcb` | Store data pointer into destination; no destination retain yet |
| `6504abce` | Imported QBasicAtomicInt::deref on temporary data |
| `6504abd4..abd6` | Test returned nonzero flag and skip destruction if true |
| `6504abd8..ac08` | Zero branch destroys 60 brushes at data+8 and frees data |
| `6504ac10` | Write entire destination word+8 to zero |
| `6504ac17` | Retain destination data; returned bool ignored |
| `6504ac25` | Return; QWidgetPrivate's font construction follows |

The QtCore leaves `67001aa0..1aa8` and `67001ac0..1ac8` are LOCK ADD dword
`+1`/`-1`, SETNE AL, RET. Thus the current word is raw uint32 bits. There is no
positive-count admission rule or immortal/sentinel exemption in these leaves.
The temporary release returns false only when its updated word is zero; CP80's
destruction-pending guard must remain for that branch. Destination ref's boolean
cannot become a refusal merely because a raw wrap produces zero.

## Allocation producer is exact, but earlier than the needed state

`QPalette(GlobalColor)` at `6504bf80` requests `0x1f0` bytes at `6504bf99..bf9e`.
On a nonnull allocation it calls `65049990` at `6504bfb0`, stores that data in
the output at `6504bfb9`, zeros the output word at `6504bfbc`, constructs a
QColor at `6504bfca`, and invokes palette initialization `6504b7c0` at
`6504bfd7`. The allocation-null branch is distinct; it proves no initialized
data object.

The data constructor's `65049996` sets EDX=1; `6504999e` calls IAT
`65697078`, the QAtomicInt(int) constructor, with RCX still the new data. Its
resolved QtCore leaf `67001b40..1b46` is `mov dword ptr [rcx],edx; mov rax,rcx;
ret`. This directly proves data[0]=1 at allocation initialization. The data
constructor then constructs 60 brushes (size 8, data+8), assigns the serial at
data+1e8 from QtGui global `659bd0fc`, increments that serial global, and zeros
data+1ec.

This is a possible start for a complete native mutation ledger. It is not a
current-count producer for the already shared `app_pal` data observed during
SplitView construction. Palette initialization, caching, copies, assignments,
temporaries and releases between this allocation and the selected private copy
are not all closed by the retained evidence. Seeding CP81 with 1 would discard
those intervening mutations.

## Retained splitter003 does read the real app_pal slot

This qualification is stronger than the earlier splitter002 snapshots. In
`build/original-splitter-probe-003/probe.js`, `cacheState()` reads the actual
loaded QtGui module plus RVA `9c9690`, then calls
`value(p,'palette',false)`. Application-values state 6 records wrapper
`0x3d53700`, data `0x46cedb0`, word8 `13893616` (`0xd3fff0`), application
`0x12fe470`. That is actual slot/wrapper/data evidence in this process; it is
not an inference from a widget or the class-palette cache.

The final `false` matters: `value()` reads data[0] only when its `refs` argument
is true. Neither this application slot snapshot nor ordinary widget snapshots
read the atomic. The source and capture are therefore sufficient to qualify the
present slot/data identity, but not its native reference word at that point.
Captured addresses must not be compared with aliases in another process.

The same probe DOES directly read `dataRefs` later, during parent-context
palette lookup/resolve and widget palette returns. For placeholder0, state 13
already records widget `0x4022630`, private `0x54e51e0`, its private palette
`0x54e5350`, data `0x46cedb0`, word8=0 and parent=null. Parent-enter ordinal 28
then reparents that already constructed widget to SplitView `0x3ff4000`.
The selected later observations are:

| Ordinal / stage | Value wrapper | Data / current word |
|---|---|---|
| 37, palette_lookup_return | temporary `0x12fd748` | `0x46cedb0` / 166 |
| 39, widget_palette_return | parent palette `0x5154bc0` | same / 166 |
| 40, palette_resolve_enter | placeholder `0x54e5350` and temporary `0x12fd748` | same / 166 |
| 41, palette_resolve_return | output `0x12fd738` | same / 167 |
| 53, widget_palette_return | placeholder `0x54e5350` | same / 165 |

These are current atomic reads, not values reconstructed from host lease counts.
They also show that a read taken in another temporary context is not freely
portable: resolve's output has already added one native value at ordinal 41.
The initial SplitView context has analogous later values 165, 166 and 164;
successive placeholder/handle contexts reach 170. Choosing any one of those
words for CP81's earlier release would change its source point.

Probe003 hooks parent `6506dbd0`, palette propagation `65068080`, lookup
`65020be0` at caller `650644cb`, resolve `6504a2f0`, and selected widget-palette
returns within those contexts. It does not hook the private constructor's
`6504abb0`/`6504abce`, the default getter `65016f70`, or every atomic mutation.
The interval from private palette construction through the rest of QWidget,
QFrame, style and reparenting has not been reduced to a complete palette-data
ref ledger. Subtracting a guessed number from 166 or 165 is therefore unsound.
Other retained editor snapshots inspected for this question contain palette
data/word identity, not the missing at-edge palette atomic read.

## Smallest admissible continuation input

The next boundary is a source-qualified current-word observation, followed by
the existing isolated primitive's one-time release, not a fresh default count:

1. Identify the actual same runtime, live placeholder receiver, current
   QWidgetPrivate/default-QPalette frame, its app_pal wrapper and interned data.
2. Observe data[0] at `6504abc3` or immediately before `6504abce`, and join the
   temporary/getter output/destination pointer to that SAME data. At `6504abce`
   the pointer store is already committed, matching CP81's partial-slot cursor.
3. Admit the raw word only at the ledger's actual current mutation sequence.
   It already includes getter copy `6504a8c6`'s +1. For one CP81 getter token
   with no other modeled mutation this is sequence 1; if other retained getter
   frames share the data, it must be their real current sequence. Never replay
   their retains to make an observed count agree.
4. Release the SAME held getter token once. Unknown current bits remain held
   with no mutation. A committed zero consumes that token once and stops at the
   shared data-destruction guard. Nonzero permits word0 then a destination
   retain only in a separately authorized integrated continuation.

Any observation admission should carry pinned module hash/RVA, actual receiver
and frame provenance, runtime/data control-block identity, current sequence and
the observed raw word. A naked user-provided alias/count, a nonzero branch
boolean, app slot presence, `use_count()`, font count or widget count is not a
substitute. Same sequence/equal word can be idempotent; conflicting/stale
sequence, different data/runtime or released owner must refuse without mutation.
Host copies of the retained read continue to share the one getter token.

A complete ledger from actual allocation plus every intervening palette ref
mutation could supply the same prerequisite, but is broader than this one
missing at-edge witness. This report neither authorizes nor starts a new GUI or
native capture. CP81's public/private and count/font gates stay intact.

## Evidence receipt

DUMPBIN executable: Visual Studio BuildTools MSVC 14.44.35207 Hostx64/x64,
banner 14.44.35228.0, SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Imported originals are under `evidence/original-binaries/`:

| Artifact or exact half-open byte range | SHA-256 |
|---|---|
| QtGui4_64.dll (preferred base 65000000) | `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` |
| QtCore4_64.dll (preferred base 67000000) | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| QtGui `65049990..650499f4` (100 bytes) | `5715acdc185a8d9e24743dddc95b8a10f0f41228cca490303e0b3616d1ac1670` |
| QtGui `6504bf80..6504bfea` (106 bytes) | `3cf5e6195328c734ca31032f6140742b4fac53cc20232dc66c1c225ed912907a` |
| QtCore `67001b40..67001b46` (6 bytes) | `06dcc77a1c9c4313b8353858305ec3c04ecb0d4aebc8e5e372c592c54e4d456c` |
| QtGui default ctor `6504abb0..6504ac26` | `704806e2a08c3327f0cb7f8d0ca7c496af0c69dbd897d456e48713cf1fa6a399` |
| QtGui copy ctor `6504a8b0..6504a8fd` | `a0c82e2e8037dc55d2034c9b82ad3ae911f3c4af2e740438ecfbe4e9970cc03b` |
| QtCore ref `67001aa0..67001aa8` | `7b57a50acb800ca3e7d326b624b14349c569ea06f18e594273c33a92fe1256de` |
| QtCore deref `67001ac0..67001ac8` | `06457cdd529a46d5cd423e2ccd7a761b5abfd4d97a001e30895a55157b8065b6` |
| splitter003 probe.js (27464 bytes) | `8a8d54f8c530c76443d14a9c21fadeb63f95d076fc96536d90dbc0c6c709626d` |
| splitter003 capture-evidence.json (349527 bytes) | `ae7f9f9eadfcb91b22a0c2149d573234618eb2401ea0c461cebcb86b9a971f14` |

The retained capture contains 254 messages, 165 observations and 56 states;
`messages_truncated=false`. Its recorded running status is historical capture
data, not a claim that the old process is currently running. No full ledger or
capability object was printed for this investigation.
