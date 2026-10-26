# After selected texture opacity: font tail and raw0x4a owner prefix

The selected null-image path needs no CP68 widget HWND/style branch: CP67's clear
bit 11 return reaches 6506e75b and the existing shared QWidget constructor tail.
The immediate data-readiness guard is that tail's actual font/device context.
With its separately qualified matching-font/Create profile, the first unimplemented
native effect is allocation/construction of the heap raw0x4a event at 6506e81a,
followed by the actual thread postEvent call. Do not mark QWidget returned there.

Owner: editor300. This report is the only write. Existing source/bounded captures
were read and native PE bytes hashed; one bounded QEvent-constructor DUMPBIN
window was inspected without writing another artifact. No source/build/CTest,
original execution, GUI or mutable REA. CP68 source freeze is respected. Token
usage is unavailable; no test or completion result is claimed.

## Caller and actual tail receiver

Original QtGui SHA-256 remains
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`;
QtCore SHA-256 remains
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
Read CP66/67 caller qualifications, wave3-closure-next-6506e4d0.asm,
wave3-closure-next-651f2e30.asm, wave3-closure-next-6507f9d0.asm,
wave3-closure-next-67131160.asm, wave3-closure-next-6711bfd0.asm and
wave3-closure-next-6711ae90.asm. Relevant retained contracts are
wave3-close-widget-font-device-successor-contract.json and
wave3-close-widget-constructor-tail-api-proposal.json.

After 650979cf returns, 6506e75b uses the SAME actual QWidget in RSI and private in
RDI. It passes QWidget+10 as QPaintDevice, private+180 as current QFont and a
caller-local temporary to 651f2e30. That constructor copies the current font's
resolve mask, calls paint-device virtual metric8, and compares FontData+38 with
that metric and +3c with 0. Matching shares the SAME font data; mismatch allocates
0x60 and copies unrepresented font/engine state, still guarded. Actual base
QWidget devType 1 skips its printer-device branch. The caller then assigns the
returned font to the same private+180 at 6506e77b and destroys its temporary at
6506e786. This is not an application-global replacement font or a second widget.

The next native order is:

| Caller point | Effect/owner |
|---|---|
| 6506e796/e7a6 | Set actual QWidget raw 34/raw 35 true |
| 6506e7ab..e7c9 | Increment current QWidget count and update signed 32-bit maximum |
| 6506e7cf..e7f6 | Compatibility version/attribute test; alternative enters native creation |
| 6506e805 | Construct STACK QEvent raw 15 at caller rsp+58 |
| 6506e814 | Synchronous sendEvent to SAME QWidget |
| 6506e81a..e84c | Allocate 0x18 and construct a DISTINCT HEAP QEvent raw0x4a |
| 6506e857 | postEvent(SAME QWidget, heap event), priority 0 via thunk |
| 6506e85d | Only after post returns, clear private+c8 |
| 6506e869 | Only then destroy original stack Create 15 event |
| 6506e86f onward | Finish init epilogue; subsequent QWidget/application wrappers remain separate |

Current EditorBase already invokes WidgetConstructorTail on the SAME receiver/
runtime, after actual image finish and changed opaque-bit/native helper return.
That tail retains its stack Create and dispatch cursor. It never sets completed
true: PostPending currently returns a precise raw0x4a queue/dispatcher dependency.
Thus further matching tail inputs can reach PostPending, not QWidgetReturned or
application wrappers. The selected brush is no longer read or copied in this
suffix; its image/opacity receipts remain historical and must not replay.

## Immediate actual values: font readiness is not inferred

The current default editor fixture reaches FontDevice first. The owner/value
requirements are already represented by WidgetConstructorTailObservation:

| Requirement | Actual owner / constraint |
|---|---|
| Current font | SAME widget font/private+180; expectedFontAlias resolves to that runtime FontData owner, mask preserved |
| Metric target | SAME base QWidget's QWidget+10 paint-device subobject, known QWidget virtual implementation |
| Thread choice | Actual QObject::thread(qApp) QThread object versus QThread::currentThread object |
| DC | Actual QtGui cached display HDC at module RVA 9cae30, not widget HWND or cleanup-counter owner |
| Metric | Actual extra DPI-Y override/parent chain and display GetDeviceCaps index 90 result |
| Match | FontData+38 bitwise equals metric and +3c is0; otherwise retain data-copy guard |
| Later counters | Actual shared current/maximum widget counts and compatibility state at this phase |

Native DC acquisition/lookup precedes the override/parent tests, so a known DPI
or matching FontData field alone cannot skip missing thread/DC qualification.
WidgetThreadState affinity/QThreadData equality is not by itself QThread-object
identity; the existing font successor contract explicitly preserves that distinction.
Known-null cache reaches GetDC allocation, not a default DC alias. Do not seed
current widget counts from a font value, image ref count or earlier startup role.

The retained probe004 constructor-tail reconciliation explicitly covers the FIRST
SplitView startup tail, not editor-time population. It records matching 168/0,
actual cached DC and current/application QThread, counters 173->174 and that
role's event-filter population. Those are valuable role-qualified observations;
they are not generic post-Open editor values. The editor's isolated tailInputs
fixture similarly supplies explicit test values, not original runtime evidence.
Public Session still lacks its post-Open private/default bridge before this tail.

No new owner is needed to call the existing matching-font tail: it must consume
these actual current receiver/runtime values. A ready/succeeded boolean or
application font clone would not qualify them. Foreign-thread DC, mismatching
font copy and compatibility native-window creation retain their existing guards.

## First unimplemented effect: unposted heap event

A small integrated NEXT CODE cut is possible without claiming post completion:
retain the normal successful raw0x4a heap-allocation/constructor prefix after the
actual stack Create dispatch returns, then guard at the post call. This needs a
separate unposted heap-event owner, not a stack event inserted into a queue or an
observation pretending an event is already posted.

Caller 6506e81a requests 0x18. On nonnull return, it invokes QtCore QEvent::QEvent
at 6711d550 with type 0x4a, then stores QtGui class/vtable 6569e350 at 6506e849.
Bounded direct inspection establishes EXACT constructor stores:

- 6711d557: private-data pointer+8=0;
- 6711d55f: word type+10=argument;
- 6711d563: Core QEvent class store;
- 6711d56b: flags word+12 AND fffc (clear posted bit 0 and spontaneous bit 1);
- 6711d56f: OR 0004 (accepted bit 2 true), then RET 6711d577.

Upper 13 flag bits are PRESERVED, not initialized to 0. A new state must model
known mask 0007/value 0004 or equivalent separate bits; do not invent whole-word
flags 4. The caller's QtGui class override then supplies the existing qualified
PlainQtGuiQEvent deleting-class identity. Actual event allocation identity belongs
to this call frame; a vector ordinal is not a native observation alias.

The first bounded prefix contract is:

1. Allocate/create one separately owned unposted raw0x4a value only after current
   Create 15 dispatch completes; retain normal allocation/class/type/private-data/
   known-flag effects once. Allocation failure/exception/null continuation stays
   guarded; do not redesign it as a safe no-post success.
2. Keep this heap value caller-owned until the retained post-call owner accepts
   responsibility. Its posted bit is still false, receiver posted count is not
   incremented and no queue record or dispatcher wake is fabricated.
3. Retain the existing stack Create 15 alive, keep private+c8's later store pending,
   and keep ConstructorTail completed=false at a new post-call dependency.
4. Host abandonment is not queue insertion or native destructor success. Tests
   need independent stack/heap lifetimes and no duplicate allocation/construction
   on retry. Do not use the existing already-posted queue-observation constructor
   to manufacture this prefix.

A separate source-derived receiver prerequisite can also be implemented narrowly:
QObjectPrivate constructor 67131213 writes postedEvents dword +24 = 0 on the actual
NEW object. Initialize that same WidgetState postedEventCount at its recovered
QObjectPrivate construction stage, rather than globally defaulting all adapters
or forcing 0 at PostPending. Existing live/adapted objects can have nonzero counts.
Any later real posts must mutate that field. Zero receiver count does NOT mean
an empty thread queue or absent events for other receivers.

Suggested ownership for this cut, if root selects it: widget_events.hpp/.cpp and
its focused test for the new heap-prefix state/shared tail; editor_base.cpp and
its focused test only for the source-derived new-object posted-count initialization.
Root must assign shared files exclusively and owns target wiring/build. This
read-only report implements none of them.

## Actual post/queue cut remains distinct

QtGui IAT 65696bf0 resolves to two-argument QCoreApplication::postEvent at 6711bfd0.
That thunk sets R8D = 0 then jumps 6711ae90, proving priority 0. For the normal live
receiver/thread branch, the callee uses:

- Actual receiver QObjectPrivate+40 QThreadData owner;
- That thread's mutex+48 and post-event list+30, stabilizing affinity under lock;
- Receiver postedEvents+24 for the conditional application compression path;
- Ordered list insertion 6711acd0, THEN event.posted bit OR 1 at 6711afef, THEN
  receiver count++ ataff9, THEN thread canWait byte+50=false ataffc;
- Unlock BEFORE optional actual thread dispatcher+20 virtual wake at+98;
- Normal return only after wake returns or actual dispatcher is qualified null.

A zero posted count can close this receiver's compression branch under the
normal no-earlier-post profile, but does not identify the rest of the list,
backing/capacity, mutex state or dispatcher. Existing WidgetPostEventQueue supports
qualified observations/removal, not constructor posting/insertion. Its current
records require actual WidgetState receivers; earlier captured queue populations
also contain unrelated/opaque QObject identities, which cannot be modeled as
fake widgets. Do not seed empty/current queues or reuse first-SplitView startup
records at editor time without the phase/owner join.

A future posting cursor must use the SAME actual WidgetThreadState queue and
WidgetState count, distinguish observed record aliases from generated heap-event
identity, and preserve insertion->posted/count/canWait->unlock->wake effects.
A missing dispatcher/wake guard AFTER insertion must retain committed queue
ownership/effects, not roll back or report a successful constructor. Null-thread/
receiver deletion, compression, shared/growing backing and dispatcher branches
need separate source qualifications. A Win32 dispatcher wake's hidden HWND is
another owner, distinct from CP68's QWidget HWND; neither may substitute for it.

Only actual post return permits private+c8 clearing, stack Create destruction
and completed tail. Event posting/wake is not event DELIVERY, timer execution,
modelUpdated or later editor fill. Public mode300/wrapper/Save cannot be widened
merely because allocation or queue insertion is represented.

## Proposed focused checks and receipts

Use existing explicit tail inputs to reach the selected matching-font/Create
branch without inventing public aliases. Verify completed image/opacity effects
never replay while font/Create/post prefix waits; actual font owner/mask and
separate DPI/DC/thread requirements remain intact. For new-object posted count,
verify initialization before thread/copy tail, retry stability, and no reset of
already adapted receivers. For heap prefix, verify 0x18 request/type 0x4a/class/data
and low 3 known bits, unknown upper bits preserved, stack event remains alive,
no posted flag/count/queue/wake effects and once-only lifetime on retry/abandonment.
No such tests are authored/executed here.

Native byte SHA-256, ranges end-exclusive, independently hashed from pinned PEs:

| DLL / interval | SHA-256 |
|---|---|
| QtGui6506e75b..6506e81a | 190bd5528bee76a1ae7a7fd666963afbaaaa87c719dbe70bd3d5d173e71fd7af |
| QtGui6506e81a..6506e851 | 1d7fe198290126999bea0a5e7b45b08276940de129abefef8f76447488425271 |
| QtGui6506e851..6506e86f | f68e2f636acef461639a1d340d02ffefdff7d4763f8c97d19b546d749b55af90 |
| QtCore6711d550..6711d578 | 3eca021ab2f985bb21228a50ef30714b2fd8647f3d0d21f8dd5c822cf7006805 |
| QtCore67131213..67131216 | 67d275f9916f45bc8e582261789f48a202afbedb716b4a556652b025015e9357 |
| QtCore6711bfd0..6711bfd8 | 5ff5c34d6cb2c15a01b83babd507992c6034f2556fd37de55691d68231f9dbaf |
| QtCore6711ae90..6711b039 | 3e9d9e08c0592464f8f1abc87e14914a11053529346439d712e19d6e356b854c |

No source files changed and no new raw artifact was needed. The bounded QEvent
window extended into padding/next destructor; only the exact ctor through its
RET above is claimed. Public PrivatePaletteUnavailable/not_ported6504abb0,
constructor PostPending and application wrappers remain guarded. The chosen
heap-prefix cut is internal state recovery, not fresh Open-Save-Close completion.
