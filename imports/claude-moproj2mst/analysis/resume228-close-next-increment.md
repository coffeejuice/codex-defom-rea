# R228: Close/destructor next increment

## Decision

No nonduplicate Close/destructor source increment is admissible from the retained evidence. Keep the public normal-Close path guarded and do not add process-global reset or generic destructor cleanup. The smallest missing witness is one same-owner original Open → accepted Close occurrence, with the nested Save outcome and owner generations carried through destructor entry/return. This conclusion reconciles the RCLOSE queue, CP214 ownership audit, CP160/169 document-slot evidence, and current candidate source; no original runtime, GUI, build, tests, or mutable REA state was used.

## Source-backed boundaries

The existing normal Close guard contract places process-type/can-close and, for successful Open under the qualified fresh Windows style, the default-Yes nested Save before post-answer callbacks and detach. A nested Save is not discharged by an earlier explicit Save: the result used by the guard is the nested call's integer return, remapped to Cancel only for the specified Yes result. Post-answer main-window virtual and graph-removal notification precede detach. Therefore a detached cleanup helper cannot substitute for public Close.

The current source has numerous already-separated Close prefixes/effect helpers, but these do not create the missing owner. In particular, `ReportDocumentSlot` is a diagnostic initial null/non-null token. Its Close projection only permits the null return or refuses opaque nonnull deletion; it is not a live SubProject `+0x30` guard with constructor, parent, rebind, and death tracking. The native destructor path reads the guard twice; the second read selects the deleting virtual target, then clears the guard. Destructor payloads are current fields and QObject child/parent destruction order is conditional on accepted ownership.

Process-global reset evidence remains narrow. Individually qualified stores may be modeled at their actual source boundary; there is no basis for blanket reset of engine globals, notifier maps, graph registries, caches, Fortran units, or pending events. Session/adapter destruction is not evidence of native Project Close reset.

## Why source cannot advance

The closest apparent small addition would be another Close cleanup or destructor prefix. It would duplicate a source-level prefix without resolving whether the concrete owner is live after nested Save callbacks, whether the second guard read names that owner, or whether a QObject parent already destroyed it. The static facts establish conditional deletion order, not an accepted occurrence. CP214 likewise finds no same-owner delivered/accepted Close and leaves partial-open failure cleanup uncorrelated with allocation state.

Nested Save effect ordering is a hard barrier: modal answer, nested Save return, callback effects, possible owner mutation/death, then fresh Close owner reads. The current public guard appropriately refuses where those effects/results are unavailable. Relaxing it would invent accepted Close effects. A synthetic test of the candidate's own guessed order would not add original evidence.

## Smallest missing witness

One natural successful Open followed by accepted default Close, recording identities and generations for main window, outer model manager, Mo child, Project/SubProject and document; nested Save answer/return; post-answer callbacks; both live document-slot reads; deleting virtual and destructor entry/return; guard clear; current payload fields; and QObject parent acceptance/child destruction order. Capture only the reached process-global stores, rather than inferring a global reset. This is the minimum witness that could admit a same-owner lifecycle increment. Failure cleanup remains a separate per-return matrix unless that same occurrence naturally reaches a relevant failure edge.

Until then, retain the public Close guard and diagnostic slot behavior. This report is a read-only checkpoint, not a behavioral claim about a reconstructed executable.
