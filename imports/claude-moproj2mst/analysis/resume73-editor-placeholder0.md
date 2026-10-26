# First placeholder QWidget base: exact entry and count point

The minimal next placeholder entry is a distinct SplitView-owned pending
placeholder0 component at application dab94c, with parentnull/flags0, using the
same WidgetRuntime. Its first source-qualified prefix can stop before the current
private palette copy. It must not be represented by StagedEditor300, and no
174->175 count effect occurs at allocation or private-prefix entry. The count
change belongs to its later6506e7b5 QWidget-tail store.

This report is the only write. No source, build, tests, original execution, GUI,
mutable REA or new raw artifact. Existing bounded captures and source were read;
artifact hashes are below. Token usage is unavailable.

## Dependencies before the call is reached

SfSplitView dab810 first calls QSplitter65403210 atdab839. QSplitter first calls
QFrame653b2470 with its private object, orientation and actual MainView parent;
QFrame calls the supplied-private QWidget constructor65070f00. The first known
SplitView current/max174 is already committed in that QWidget tail, but that
number alone does not mean the base call returned.

The remaining predecessor order is:

- Complete the existing shared QWidget font/attributes/count/compatibility/Create/
  post continuation, later private+c8 clear and original stack Create destruction.
- Return through QFrame: install QFrame classes and execute its style/frame helper
  with subelement53. An observed QRect result does not substitute for that call.
- Return through QSplitter: install its primary/secondary classes; set private+218
  orientation; call setSizePolicy6506db20, then raw attribute0x44=false. Size policy
  can invoke geometry/event effects before return.
- Return from QSplitter at654032be. ONLY THEN dab840..dab85b installs application
  SfSplitView classes142a5a638/142a5a600; dab868 constructs its QString+28; dab874
  calls derived initializer dab900.

Current ensureDefaultSplitView ends with the retained WidgetConstructorTail
report and always returns completed=false. SplitView stages have no QFrame,
QSplitter or dab900/placeholder continuation yet; optional orientation/name are
not committed. No current source branch can legitimately jump straight from
WidgetPostPending or count174 to placeholder allocation. Root owns any predecessor
source/integration assignment.

## Exact allocation and ABI at dab94c

The first initializer prefix is:

| Application address | Exact effect |
|---|---|
| dab915/dab91a | Request0x28 application object allocation; retain returned storage |
| dab924/dab92a | Test returned storage; normal nonnull branch continues to QWidget construction |
| dab936..dab93d | Pass integer0 to flag-value construction helper546c70, output stack rsp+30 |
| dab942 | Pass returned flag-value address in R8 |
| dab945 | EDX=0, the actual QWidget parent argument |
| dab947/dab94c | RCX=the allocated placeholder; call IAT141e0ba80 |
| dab952..dab96d | Only after QWidget returns, install application classes1420fbde8 and1420fbdb0 at object+0/+10 |
| dab990..dab99d | Pass same placeholder to actual SplitView QSplitter::addWidget through IAT141e05aa0 |

The pinned IAT resolves141e0ba80 to
`QWidget::QWidget(QWidget*, QFlags<Qt::WindowType>)` at65070d70, NOT QFrame,
QSplitter, SfWidget or Editor300.141e05aa0 resolves to QSplitter::addWidget.
The Qt ABI receives the flag value by an address and65070de8 loads its dword;
it is not an arbitrary R8 integer flag parameter. Probe002 independently observes
this call's intended parentnull and flags0. This audit does not claim the unseen
546c70 helper body from its name alone.

The null-allocation branch does not establish successful construction and must
remain outside the normal nonnull implementation cut; it later passes null to
addWidget. No allocation-failure behavior, exception unwind or safe normal-return
replacement is inferred here.

## The same actual placeholder through QWidget

QWidget65070d70 performs, on the same RCX destination:

1. Allocate QWidgetPrivate0x1e0 at65070d94..99; call6505e780 with fixed version
   0x40804 on nonnull storage. Source private constructors/member writes precede
   palette6504abb0 atprivate+170, font651f3560 at+180 and default locale.
2. After private constructor return, call QObject through IAT65696a30 with that
   private and parentnull at65070dc2. Bind the actual current thread and perform
   QObject's separate count effect; no QObject parent/child membership occurs.
3. Construct QPaintDevice subobject atplaceholder+10 (65070dcd), then install
   QWidget classes656ab898/656ab860 at65070dda/de4.
4. Read the actual flags dword, pass parentnull and flags address to
   QWidgetPrivate::init6506e4d0 at65070dfa.
5. Only after init returns does65070e00 return SAME destination; ret65070e10.

Private current palette/font copy cannot occur before their producer owner is
known. Locale/application presence/version, actual current QThreadData, allWidgets
presence/registration, QWidgetData, initial attributes, parentless geometry/self
focus, adjusted flags, layout direction, current palette brush selection/opacity
and common constructor tail all retain their original order and dependencies.
Parentnull means no early parent/focus splice to the SplitView; that reparenting
is later in addWidget. A component's host storage lease is not native QObject
parent ownership.

The native QWidget body is source-identical to the parentless base already
recovered for Editor300, but its APPLICATION CALLER IS DIFFERENT. EditorBase's
private constructor takes StagedEditor300*, checks widget.nativeIdentity equals
that factory, and writes self-focus using factory_. Reusing it by constructing a
fake Project/Operation/model/editor would substitute the receiver and add wrong
application semantics. Any reuse must extract only the common constructor
primitive and parameterize it by actual component/WidgetState identity.

## Counter174 ->175 and what it does not authorize

Probe002 identifies actual placeholder0x39624b0 using the exact dab952 return
address. Record18 enters QWidgetPrivate init with parentnull/flags0, globals
state10 current/max174. Record19 returns from that SAME init, still parentnull,
globals state13 current/max175. ThreadData is0x3690c70; extra is absent. Raw
extended words change[0,0,0] to[12,4096,0]. No later SplitView parent assignment
is smuggled into this base interval.

Source count load6506e7ab, increment6506e7b2 and store6506e7b5 are after font
assignment/temporary destruction and raw34/35. Signed maximum update follows at
6506e7bc..c9. This placeholder's own174->175 must happen there exactly once.
It is before compatibility/Create/post and can remain committed when a later
post guard is reached. Neither QWidget-return observation nor parent assignment
is a substitute counter event. The QObject construction count remains distinct.

The successful original endpoint proves branch reachability and net+1, not that
all constructor dependencies are currently reconstructed. Probe002 has no full
constructor event trace, does not supply the selected parentless brush's alpha/
style as editor004 does, and does not directly capture every font-tail input for
this placeholder. Probe004's detailed QThread/DC/font context is its selected
SplitView, not a blanket future receiver observation. Missing actual private/
thread/DC/brush/event context must remain precise guards. No760 value belongs
anywhere in this entry.

After QWidget returns, application class install and addWidget follow. That
first addWidget can create distinct handle0, including its QWidget base and later
QCursor/setCursor/raw0xb7 behavior; placeholder1 is allocated only after all that
first addition returns. A pending count175 cannot authorize handle0 allocation,
record insertion, placeholder1, notifier registration, grid membership or completed
View_Default by itself.

## Minimal candidate owner and source entry

Suggested smallest bounded implementation, subject to root ownership assignment:

- Add one pending placeholder component owned by the actual SplitView derived
  initializer, carrying a unique WidgetState/nativeIdentity and actual component
  control block. Bind it to the existing runtime with bindWidgetComponent. It has
  its own lifetime/cursor; it borrows neither editor widget nor SplitView widget.
- Enter it only from a new source cursor that has genuinely returned through the
  predecessor bases and dab868 QString construction. No public ready boolean or
  observation of a future count should activate this cursor.
- On first nonnull normal allocation, retain the same pending owner and flag/parent
  call context. Execute only source-closed private prefix. Stop at the current
  per-placeholder PrivatePalette dependency if application-owner admission is
  absent. This increment has NO QWidget-count, class-return or addWidget effect.
- A later separately assigned generic parentless constructor continuation can use
  that SAME WidgetState/runtime and existing shared tail, committing count only
  when it reaches the actual count point. Any extraction from editor_base must
  preserve its existing actual-editor API/guards and current partial effects.
- After native QWidgetReturned, install only this placeholder's two application
  classes, then retain the addWidget continuation before any second allocation.
  Keep source private/default inputs per actual receiver and observed stage.

Possible conceptual interface is a private
`SplitViewState::resumeFirstPlaceholderConstruction()` on its derived initializer
cursor, with a retained generic `ParentlessWidgetConstruction` child call. The
names are proposals, not existing APIs. Current WidgetState friends/constructor
are deliberately restricted, so a separately assigned patch must add only the
actual placeholder/common constructor owners as friends. It should not expose a
public arbitrary WidgetState factory or relax matchesComponentOwner.

The isolated owner/private-prefix can be tested independently while public
predecessor gates stay closed. Such a test is conditional primitive recovery,
not proof that public Session can yet reach dab94c. Meaningful checks include
same raw/control-block/runtime identity, no parent membership at base entry,
foreign/replaced owner rejection, once-only storage/private prefix, guard-before-
palette/font/thread/count/registry, observer-only no resume, and no application
class/addWidget/placeholder1 effect before QWidget return. Later counter tests
must cover retained174->175 across Create/post retry, not observed count seeding.
No tests were authored or executed by this audit.

## Retained evidence identity

Original application SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
QtGui SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Preferred application/Qt bases140000000/65000000; runtime module bases differ.

| Artifact | SHA-256 |
|---|---|
| analysis/wave3-closure-next-140dab810.asm | 9d4a251a8f46c895c54000b6f987490478680aa8bc27fd7dad398f8c8d038b70 |
| analysis/wave3-closure-next-140dab900.asm | 59c4651c251ba5a9912d53cba7ac927f4db59ea947d0aee92932ab9259a6412f |
| analysis/wave3-closure-next-65070d70.asm | 79542748f0ce31086b76e420ab7024ddd133b5efdfc4f5f13d706e1ab95cc6d4 |
| analysis/wave3-closure-next-6505e780.asm | 76ea1255e2b25c691ec26bf3198d1ad53e991632b268a9d5c77e0b44c706bd81 |
| analysis/wave3-closure-next-65403210.asm | 0cf8bfe97747465623b2f0ed787e1457d33015ce5c313c9aeed09dc3149ce5ba |
| analysis/wave3-closure-next-653b2470.asm | 56141dd36166d2fba54bb54a7ba3c6b6b6774d20c0d4de81716b612537d1b9aa |
| analysis/wave3-closure-next-close-default-splitter-derived-initialize-static.json | a08ec792a2878bdf4e8fc3f3405e03ecd5cafebf54b99342e4a5da92f75d6023 |
| build/original-splitter-probe-002/capture-evidence.json | bdb95dbe5840180a1b1bb178faafe997accbff835f66242e1a3c4618d8d47fca |
| source/apps/moproj2mst/src/engine/editor_base.cpp | 20063efa3b650fec2dce5274894c10d5448ad8566b26bd4dad956512e06f3817 |
| source/apps/moproj2mst/src/engine/view_manager.cpp | dc6ad8356e061bf7e9aebaa1d32ef2caa8a0d4d41da89720c023a4b825899a4c |
| source/apps/moproj2mst/include/m2m/engine/view_manager.hpp | c0c146eddb8cba22f30b573a9109c97494046305dd81fdb03c2e6164433d9c53 |

Source hashes identify the read snapshot only. Public editor/private-palette,
SplitView predecessor and wrapper/Save guards are unchanged.
