# CP149 — Report SubProject slot to accepted-Close bridge

## Finding

The native owner is more concrete than a generic “Project owns a document” statement: the Report document is stored in the active SubProject record's `+0x30` field, while the Project owns SubProject records in `P+0x58` (`QList<SUB*>`). However, retained evidence does not connect that Project/SubProject instance to the accepted-Close destructor path. No source-ready release transition is established. Keep candidate Report Open/Save and Close behavior guarded.

## Bound owner and publication path

The retained `evidence/gui_mo/s1/s1-01/spec.md` §8.1 documents the `0x50`-byte SUB layout: Project pointer at `SUB+0x10`, type/id at `+0x18/+0x1c`, and a QPointer field at `+0x30`; it identifies `P+0x58` as the Project's SubProject pointer list. It also distinguishes the Open constructor `0x1400b5470` (one owned model per SubProject) from the GUI-created `0x1400b5680` path. This source qualification establishes the object graph but not its teardown.

For Report specifically, retained `L7/d3.c` gives `0x1400b5470` initializing `SUB+0x30` via `0x140cbc920(slot,null)`. The lazy accessor `0x1400b5ad0` reads that field through `0x140c1aef0`; on null it allocates `0x70`, obtains the main-window pointer, constructs `0x140643100(raw,mainWindow,mode,mode!=2,0)`, publishes the result to the same field through `0x140cbc920`, then reads/returns the field. `L7/V/v3.c` qualifies `0x140c1aef0` as a direct dereference. Report Open and Save callers resolve the active SUB and obtain the document through this accessor; `FUN_1412aeb30` then passes it to `0x140645030` for `.DS` Save.

These are field identity and publication facts. The constructor's QObject parent argument and QPointer layout label do not tell which owner destroys the document first or how the field is cleared when native close destroys the Project/SubProject.

## Accepted-Close bridge query

Retained accepted-Close evidence establishes manager gates/clear order only: `0x140029a76` reaches manager clear `0x140c78300` after its predecessor checks. The empty-list branch clears the manager list/current slot; the nonempty branch invokes child deleting virtuals. The evidence does not show this manager operation deleting the active Project, traversing `P+0x58`, or touching a `SUB+0x30` Report document slot. The main-window QObject child list is a third possible lifetime path, but the constructor parent's presence alone does not show the same document pointer being deleted there.

A bounded search of retained L7/Close evidence finds the SUB constructor and Report accessor, but no native SUB deleting destructor body or accepted-Close caller that releases an element of `P+0x58`. `0x1400b5f40`, although numerically near the accessors, is qualified in the retained general model evidence as a SUB-model/DB accessor: it reads `0x1400b5a70` and calls `0x140085800`; it is not evidence of SUB destruction. Likewise the retained `FUN_14013ab00` is unrelated object cleanup (`+0xb8/+0xb0`, counters and methods) and cannot be treated as SUB teardown. This rules out two nearby-address false leads without claiming exhaustive PE-wide absence.

Therefore the bridge currently stops at two independently established endpoints:

- Open/Save: Project's active SUB → `SUB+0x30` → Report document instance.
- Accepted Close: manager clear/current reset in the bounded native branch.

There is no retained occurrence linking those endpoints to one Project/SUB/document pointer, nor a QPointer clear/destructor order against main-window QObject teardown.

## Smallest missing witness and decision

The smallest static evidence increment is a native destruction-path capture that starts at the accepted-Close caller, identifies the active Project's `P+0x58` release/clear, and follows the concrete SUB deleting destructor through its `+0x30` clear to the Report document's deletion or last-reference decrement; capture the ordering relative to main-window QObject child destruction. It must also resolve `0x140cbc920`'s concrete QPointer setter/reset behavior for that field. If those static references do not bind the same dynamic instance/ordering, the missing witness is an original same-instance Report Open→Save→accepted-Close trace with addresses for Project, SUB, document, and main window.

No smaller helper can safely be integrated now: setting or clearing candidate `SubProject+0x30`, using a `shared_ptr`, or relying on QObject parent deletion would choose an owner/release order not yet established. Do not confuse CP143's later viewer callback with this first document edge: `0x1412af0c0` follows `0x1412aeb30` and does not itself access the Report document slot in its captured body.

## Evidence scope

- `evidence/gui_mo/s1/s1-01/spec.md` §8.1 and `P+0x58` entries: SUB layout and Project list ownership.
- `evidence/gui_mo/s1/mdt/L7/d3.c`: constructor `0x1400b5470` and accessor `0x1400b5ad0`.
- `evidence/gui_mo/s1/mdt/L7/d2.c`: active-SubProject-to-document accessor path and Report document methods.
- `evidence/gui_mo/s1/mdt/L7/s0.c`: Report Save's document callback and later viewer callback order.
- `analysis/resume74-close-next-owner.md` and `analysis/resume80-report-close-release.md`: accepted-Close and slot-release evidence boundaries.
- `evidence/gui_mo/s1/s1-01/dc/mst0.c`: `0x1400b5f40` is a model/DB accessor, not a destructor.

Read-only retained evidence review. No source edits, build, GUI, original execution, or mutable REA operation.
