# A bounded startup count prefix: four real widgets after SplitView

Retained evidence closes a small part of the count gap: after the first actual
SplitView reaches174, placeholder0, handle0, placeholder1 and handle1 each have
a distinct QWidget construction boundary, ending View_Default at178. This is a
source-qualified observed prefix with four named owners, not permission to set
the runtime count directly to178. The later route to editor760 remains untraced.

Only this report was written. No source/build/tests, original execution, GUI,
mutable REA or new raw captures. Token usage is unavailable. Root retains source
assignment and integration ownership.

## Exact retained count prefix

Original splitter probe002 (PID300) tracks placeholder identity by the two exact
application return addresses from QWidget65070d70: dab952 and dab9e0. It tracks
each handle at QSplitterHandle65402b00 with the SAME splitter parent, in observed
order. QWidgetPrivate init6506e4d0 observations read actual global current/max
before and after each tracked call. Its selected endpoint is View_Default return,
not Primary insertion.

| Actual role / pointer in probe002 | Before init record / current=max | After init record / current=max |
|---|---|---|
| SplitView0x39339c0 | 2 /173 | 17 /174 |
| placeholder0 0x39624b0 | 18 /174 | 19 /175 |
| handle0 0x4e0ebf0 | 34 /175 | 49 /176 |
| placeholder1 0x3962480 | 50 /176 | 51 /177 |
| handle1 0x4e0eb00 | 66 /177 | 81 /178 |

Globals state3 is173, state10=174, state13=175, state22=176, state25=177 and
state33=178. Final record100 explicitly retains all four distinct placeholder/
handle owners and current/max178, plus the actual grid item/parent state.
Capture002 reports100 observations,43 states, zero failed reads/callback errors,
no truncations and no truncated messages. It has no complete event trace.

Independent splitter004 (PID30208) confirms the surrounding endpoints in ONE
process: record38 before_placeholders has globals state7 current/max174;
record39 View_Default return has globals state18 current/max178. It traces only
the selected first SplitView tail in detail, so this final178 observation must
not be mistaken for full callback coverage of all four intervening lifetimes.
Probe002 supplies the individual roles/count intervals.

Together these establish four specific QWidget constructor intervals and their
net+1 results. They do not prove that every possible nested callback lifetime is
absent: a balanced untraced create/destroy pair would not be excluded by endpoint
counts. Recover callback/population effects separately before claiming complete
execution parity. No public candidate state was advanced by reading these data.

## Source order and why there are four owners

Actual constructor chain after QSplitter/QFrame/QWidget returns:

1. SfSplitView dab810 installs derived classes and initializes its name member,
   then enters dab900. The original base parent's identity is MainView, not
   manager; later grid insertion reparents the splitter to manager.
2. dab915..dab94c allocates0x28 and calls QWidget(parentnull,flags0) for
   placeholder0. Its own QWidget tail increments count before returning to the
   application's derived class stores atdab952..dab96d.
3. dab99d calls QSplitter::addWidget on that exact placeholder. This reparents it,
   creates its logical record and calls the splitter's real createHandle virtual.
   The resulting distinct handle0 has its own QWidget base constructor, then
   derived QSplitterHandle fields and QCursor/setCursor/event effects.
4. Only after the FIRST addWidget returns, dab9a3..dab9da creates distinct
   placeholder1; after its base returns, the second addWidget similarly creates
   distinct handle1.
5. After both additions return, setCollapsible(0,true), setCollapsible(1,true)
   and HideSecondary execute. Five application notifier registrations follow
   derived initialization, then name SplitView_Main, grid add/reparent, manager
   layout-name View_Default and View_Default return.

Two logical splitter records are not two QWidget instances: each record contains
widget+18 and separately owned handle+20, and their QObject child-list ordering
can differ from logical record order due lower/reparent operations. QGridLayout
and its layout items are QObject/storage owners, not more QWidget counter events.
Do not count host allocation stores, scene objects, registry nodes or image data
as QWidget constructors.

## Smallest bounded implementation opportunity

The first new count-changing owner is parentless placeholder0 atdab94c. A future
increment can retain its actual pending allocation/component identity and execute
its QWidget base prefix in the SAME runtime. It must start only after the actual
SplitView QWidget/QFrame/QSplitter and derived pre-dab900 effects have returned.
Current candidate remains earlier at its shared tail/post dependency; the native
endpoint observation is not a substitute for those missing effects.

The first placeholder's source-ordered private-palette/font producer and common
parentless base primitives resemble the existing editor base, but placeholder is
not an Editor300 Project/Operation/GuiModel receiver. Do not manufacture a staged
editor to obtain its constructor. Factor/reuse only source-identical primitives
with an actual placeholder component owner, leaving application wrapper effects
attached to their correct native type.

At its OWN WidgetCounts point, commit only174->175 with native signed maximum
comparison, exactly once, preserving the count if later Create/post remains
pending. Do not set175 at allocation, private construction, parent binding or
from an observed QWidget-return endpoint. Keep derived class install/addWidget
pending until this base actually returns. Parentless private palette/font/DC/
thread/event inputs must be qualified for this receiver/phase; existing startup
font or display values cannot be justified solely by numeric equality.

The subsequent source cut is reparenting placeholder0 and real handle0 creation,
not placeholder1 allocation. QSplitterHandle's post-base cursor construction,
extra allocation, global cache/platform prefix and raw0xb7 dispatch retain their
existing guards. A constructor-base count may already be176 when one of those
later effects blocks; that is a retained partial effect, not a completed logical
record or second-placeholder admission. Full four-count composition therefore
requires more than a scalar counter helper.

Meaningful focused cases for a separately assigned implementation would verify
actual placeholder/handle identities, source count point, once-only increment
across late Create/post guards, no next owner/derived class effects before prior
return, and no duplicate allocation/reparenting on retry. Existing count-wrap/
signed-max semantics should be reused rather than duplicated. No tests were
authored or executed here.

## Primary insertion is not another monotonic increment

Immediately after d35a80 returns, MainView caller0e9c55 invokes mainwindow
virtual+790, resolved to c9bc00. That function ONLY retrieves guarded window+138;
it does not allocate the Primary graphics view. The separate ca5fd0 producer
allocates/configures a graphics view only when its cache is null and explicitly
called. Its QWidget/QGraphicsView internal children/viewport lifetimes cannot be
assigned a guessed count here.

Caller0e9c7e then asks manager d36170 for Primary insertion. Source path
 d34ad0 ->dabf00 ->dabbd0 obtains the actual current index0 child. Same incoming
child returns. A DIFFERENT nonnull old child is hidden, then destroyed through
virtual+18 with deletion flag1. Nonnull requested replacement is inserted;
null requested replacement still removes the old child. dabf00 shows a nonnull
requested replacement only after dabbd0 returns.

Do not model this as a simple178->177 decrement. QSplitter::childEvent raw47
at6540508a onward finds a logical record by matching record+18 to the removed
widget, removes that record, then deletes its nonnull handle+20 through virtual
+18 at65405168, frees record storage and recalculates. Thus actual placeholder
removal may entail a separate handle lifetime and further callback ordering.
The QWidget decrement6506eff2 itself follows earlier teardown dependencies.

Probe002 explicitly stops at View_Default return before this request; probe004's
final snapshot is at the same boundary. They do not qualify the then-current
requested+138 value, removal notifications or complete destructor effects. A
current getter/record/handle/teardown profile must precede any numeric mutation.
Neither a null Primary assumption nor an unconditional two-widget deletion is
supported as a captured execution result.

## Remaining gap and next dependency

After the observed partial prefix the arithmetic difference to editor pre-count
760 is582, but this is still cross-profile reconciliation, not582 missing
constructors. The selected editor capture belongs to another process and starts
count reads only after its own private ctor. Primary/view replacement, remaining
MainWindow/startup UI, Project Open, postOpen and other widget lifetimes remain
unsegmented. The later max761/current760 also preserves destruction/high-water
history not encoded by a net count.

The practical next step is the actual placeholder0 owner/base prefix after the
already retained SplitView post/QFrame/QSplitter gates, then its first addWidget/
handle continuation. For the later Primary route, first qualify actual guarded
+138 and child/record/handle identities. Public palette/profile admission must
retain a current-stage count-continuity guard: neither178 nor760 may replace
historical runtime174 merely because these observations now exist.

The bounded prefix is newly identified evidence, not newly executed candidate
behavior. Existing public private-palette, post, wrapper and Save/Close gates
remain intact.

## Evidence identities

Original DEF_GUI_MO_64.exe SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Original QtGui4_64.dll SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Application/Qt addresses use preferred140000000/65000000 images.

| Artifact | SHA-256 |
|---|---|
| build/original-splitter-probe-002/capture-evidence.json | bdb95dbe5840180a1b1bb178faafe997accbff835f66242e1a3c4618d8d47fca |
| build/original-splitter-probe-004/capture-evidence.json | 184e45ee34732e4436057df2b99741ce1bed713440f9cc0bfd7e7f9a7ce4df9d |
| build/original-splitter-probe-002/probe.js | 086b383d3589107f8a5d0f57a31533693122d2b0d3067f145624f6ae7dc5e1b2 |
| analysis/wave3-closure-next-140dab900.asm | 59c4651c251ba5a9912d53cba7ac927f4db59ea947d0aee92932ab9259a6412f |
| analysis/wave3-closure-next-140dabbd0.asm | b6c7f3dc395c36d88e176677019f22c08c300676c843bf541243e28e59835816 |
| analysis/wave3-closure-next-140c9bc00.asm | e7581b3ab6b45155c378360bfd1265f385eb751136a0ae3cc2aad8e3f3a149d5 |
| analysis/wave3-closure-next-65404fb1.asm | b05273e4e20982836c707398058d72aa183e9eabe2765952d37cab17ba2e0673 |
| analysis/wave3-close-default-splitter-owner-cursor-correction.json | 87ca0d8d8f828ff3ae871c18fa313c6c66178a57ea7b2721e6b238d8dc41a813 |
| analysis/wave3-close-splitter-handle-cursor-contract.json | 460062ba7ace14287494c7ef8dfde37a9ac5c22b035a11f9f655cf9900bc0318 |
| analysis/wave3-close-splitter-local-effects-contract.json | 861ec51c1edfb0d86babf55bbb82b6af501be8631b5dc7a91cdbf3c2e2adce3c |

Hashes identify retained artifacts. No source or validation result was produced
by this read-only lane.
