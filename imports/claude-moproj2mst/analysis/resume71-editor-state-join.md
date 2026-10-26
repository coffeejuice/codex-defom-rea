# Join editor entry to current application owners and widget history

The smallest native editor value producer is QWidgetPrivate's ordered copy of
CURRENT QApplication palette and font into the actual selected widget. That
owner edge is source-qualified, but it does not qualify the current widget-count
history. Startup174 and editor760 are separately captured phases, not a retained
same-process trace of586 intervening constructions. A safe public admission must
keep that history dependency visible; simply binding palette/font would otherwise
allow the existing tail to increment stale174 to175.

Only this report was written. No source/build/tests, GUI, original execution,
mutable REA or raw capture. Token usage is unavailable. Root retains source/build
ownership. CP70/71 palette reports and current runtime/tail source were read.

## Smallest actual producer-to-editor edge

CP70/71 source establishes this ordered chain on the same selected receiver:

1. QWidget65070d70 allocates QWidgetPrivate and calls6505e780.
2. Private+170 output is passed at6505e8d4 to QPalette6504abb0, which obtains
   current QApplication palette65016f70 and copies shared palette data.
3. Private+180 output is passed at6505e8de to QFont651f3560, which obtains
   current QApplication font6501b330 and installs the actual FontData; destination
   resolve mask becomes0 at651f35c6.
4. Later6506e4d0 consumes those private values. The parentless palette/brush
   selection and the font/device tail belong to this widget, not MainView or an
   application-global replacement widget.

Actual editor004 identity is linked through MainWindow0xafdce0, Project0x3797390,
Operation0x37bdbe0 and editor0x383e780. The retained observation order is:

| Record | Boundary / observed state |
|---|---|
| 1 | MainWindow constructor returned |
| 2 | Selected native CLI Open entered on same MainWindow |
| 3/4 | Same Project read entered/returned success |
| 5 | Native Open returned same Project |
| 6 | postOpen entered same Project |
| 7 | editor constructor entered with that Operation, postOpen=true |
| 8 | Same editor QWidget entered, parentnull and flags0 |
| 9 | Its QWidgetPrivate entered |
| 10 | Same private returned with palette/font; first referenced globals state1 is current760/max761 |

Records1..9 do NOT contain a widget-count snapshot. The first count observation
is after the private ctor, not at Open entry. Therefore this capture cannot assign
which part of the prior count increase belongs to MainWindow startup, Project
reading, postOpen, operation/model selection, or the selected editor's private
constructor. It is not a complete Open interval count trace.

The private return has FontData0x4748690/mask0/field38=168/3c0 and palette data
0x3e3edb0/word8=0. Subsequent current-group0 Window-role10 brush is actual style1
BrushData0x4618db0/alpha255. These are same-native-call evidence, not a proof that
the candidate's startup aliases still name the current getters' shared owners.
No style24-null provenance is established by these records.

## What the174 ->760 notation means

The first SplitView capture004 (PID30208) observes current/max173 ->174 before
placeholder creation. Its process and receiver differ from editor004 (PID27216),
which observes current760 ->761 and max761 unchanged. The candidate deliberately
models that early SplitView prefix on its single Session WidgetRuntime, then
stops before complete View_Default/Primary setup and alias publication.

Thus586 is the arithmetic difference between the candidate's known early count
and the original editor-entry count. It is a history gap, not a trace of586
constructors, not proof that Open alone creates586 widgets, and not permission to
append586 anonymous WidgetState owners. The maximum differs too. Losing the
high-water history cannot be repaired by knowing a net current delta.

Native constructor6506e7ab..6506e7c9 increments current global659c9948 with uint32
wrap and raises maximum659c994c only when signed current exceeds signed maximum.
Native QWidget destructor6506eff2 decrements current only. Its decrement follows
several teardown dependencies, including posted-event removal/native-platform
work, and precedes allWidgets removal and Destroy event. Adapter invalidation is
not that destructor effect. Current candidate source implements constructor
increments in WidgetConstructorTail, but has no matching native destructor
counter commit. QObject's lock-add counter has a different owner and purpose.

Current760/max761 in a normal positive-range history is compatible with widgets
having been destroyed after a previous high count. It does not establish which
objects or when. Even assuming these are the only writes, counts alone cannot
recover allocation identities, constructor/destructor order, callbacks, registry
membership or posted queues. No same-process interval proof was found here.

## Candidate state and the missing dependency

ConversionSession creates one runtime, binds MainView/manager startup values and
qualifies the first pending SplitView tail with current/max173. The tail advances
once to174. The Session comment retains View_Default/Primary construction before
main-window view-manager aliases may be committed. currentViewManagerOwner still
refuses that missing alias boundary. These unrecovered startup component lifetimes
are earlier than native Open/editor entry and may contribute to the count gap;
the retained captures do not enumerate them.

beginEditor300Factory correctly binds the selected Project/Operation/GuiModel/
Remesh factory widget to this same runtime. It supplies no post-Open private-value
record and expects PrivatePaletteUnavailable. The private observer can bind an
existing actual same-runtime default palette/font owner, but rejects replacement
of a different current shared default owner. The tail observer merges unknown or
equal global counts; it correctly rejects changing known174 into760.

There is an additional dependency if only palette/font admission is added:
WidgetConstructorTail::WidgetCounts checks merely that current/max optionals have
values, then increments. It has no current-stage history token. Consequently, a
new editor on the already seeded runtime could consume historical174 unless the
integration proves continuity or explicitly retains a guard for that interval.
An owner match alone does not prove time-of-use state. This audit does not alter
the code or claim the public path currently reaches that later stage.

## Candidate admission contract, without a count reset

A bounded future editor-entry API can be defined, but cannot certify the missing
public state from these observations alone. Suggested conceptual contract:

`admitEditorPrivateInputs(actualSelectedWidget, currentApplicationOwnerGrant)`

The grant would be runtime-owned provenance, not an arbitrary ready boolean or
an external native pointer. Required scope and ordering:

- Same live canonical selected widget/control block/runtime; the existing factory
  still verifies actual Project, Operation, GuiModel and Remesh identities.
- Current application palette owner/revision at the6505e8d4 getter, already interned
  in this runtime; copy its source-defined group/resolve state to this receiver.
- Current application font owner/revision at the later6505e8de getter, already
  interned in this runtime; copy actual shared data and set destination mask0.
- No native construction/resume on mere admission; earlier palette copy survives
  a later font dependency. No independent palette/font clone or startup fallback.
- Counter continuity is a separate prerequisite for the later WidgetCounts read.
  A grant cannot assign760, adjust by586, reset maximum or certify omitted widget
  lifetimes. If known values belong only to the earlier incomplete phase, retain
  them as historical receipts and keep the new current-stage count unavailable
  for consumption until continuity/current-stage qualification is established.

This names a possible input join, not a newly approved baseline shortcut.
The existing observePrivateConstructionInputs(actualWidget,o) already expresses
most value/identity checks. New code is justified only for the producer's current
application-owner revision and lifecycle qualification, not a duplicate observer
that accepts the same unexplained aliases. A standalone explicitly admitted
editor profile can test composition; it does not derive a continuous public Open.

A source-derived lifecycle API could later centralize exact once-only QWidget
constructor/decrement commits on actual WidgetState identities. It must hook at
6506e7b5 and6506eff2 with their real preceding effects/cursors; maximum comparison
belongs only to constructor commits, and adapter owner release is not a decrement.
That primitive would help recover the missing history but would not supply its
missing population. A scalar delta alone also loses maximum/order information.
No destructor shortcut or such API is implemented/proposed for immediate release.

## Smallest next evidence step

Keep public PrivatePaletteUnavailable until the actual current QApplication
palette/font producer is joined to the selected runtime. Independently trace the
first unrecovered View_Default/Primary/placeholder component after the frozen
SplitView prefix and its actual QWidget lifetimes. This is earlier than assuming
a count at postOpen. Existing font-tail/event/queue gates must remain intact.

If future runtime observation becomes authorized, one same-process bounded
interval must record current/max at early SplitView endpoint, MainWindow return,
Open entry/return, postOpen entry and selected editor private entry/before its own
increment, together with count-changing constructor/destructor identity/order.
A count-only checkpoint could localize the gap but cannot alone recover required
lifetimes/callback effects. This is a dependency description, not a capture launch
or request for GUI control.

For an eventual owner-join implementation, meaningful focused checks should cover
same live receiver/font/palette revision, rejection of startup/foreign grants,
ordered palette-before-font pending continuation and no counter effect from
admission. A count-history gap must remain a guard even when the historical174
is present; exact editor qualification must allow only its own single increment,
without resetting previous receipts. No tests were authored/executed here.

## Bounded evidence identities

Original QtGui4_64.dll SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Preferred native base65000000; captures resolve actual module-relative addresses.

| Artifact | SHA-256 |
|---|---|
| analysis/resume70-editor-open-palette-owner.md | 8e6f950da2a64cb65d67f396e179cb5cdbf6305637f7361a3026db60e058e22e |
| analysis/resume71-editor-palette-entry.md | 60d0d5dee42592f5045627232b095ae1ef7659edad1886f6dbc9bcb69f943361 |
| build/original-editor-probe-004/capture-evidence.json | e4d0a277c4bbb074fa1e038ce3870e64ebb87ec884665cb1bb0ddd0221f99ced |
| build/original-editor-probe-004/probe.js | 0251ee49c0378af50e0778c8a584a5243402b10f26ab64fc315466bf10c68203 |
| build/original-splitter-probe-004/probe.js | 582b6291dc9abc631048732631bac058f8697d69049bb28a2a4c496e73f394b1 |
| analysis/wave3-close-widget-registry-teardown-contract.json | 2b3c8db2c0d16286370fc17a4a8da58caa607c48d2e08ad61162d9d0b0219623 |
| analysis/wave3-closure-next-6506e890.asm | e4b9157fe2da784ea0d74dc198d9b467cdbf301471c332b4ebe7f98ac8094c17 |

The report is the only write. No public/editor completion, validation pass or
continuous startup-to-editor state reconstruction is claimed.
