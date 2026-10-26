# R233: Close/failure queue after CP228–CP231

## Finding

The newer evidence does not admit an independently executable Close or failure-cleanup increment. CP228 moved loader unit initialization into a model-owned command; CP229 moved Editor300 frequency control; CP230–231 moved furnace post-load/prologue work into model-owned commands. Their validation records explicitly leave accepted Close guarded, and none records an original Open/Save/Close occurrence, Close owner identities, destructor entry, or a loader failure unwind. These checkpoints add no new Close-specific observation to the CP214 ownership audit or CP228 Close note.

Accordingly, retain the current public guard. Do not infer Close acceptance from adapter cleanup, successful explicit Save, passing candidate tests, or the unrelated new owner commands. Do not broaden process-global reset: the newer records contain no source-produced reset observation.

## Exact missing same-owner witness

The smallest decisive evidence remains one fresh original successful Open followed by accepted normal Close, carrying concrete identities and allocation generations for main window, model manager, Mo child, Project/SubProject, and any lazy document. It must record the nested Save's answer and integer return, post-answer callback delivery/effects, the two current Close slot reads, the selected deleting virtual and destructor entry/return, guard clearing, and QObject parent/child ownership and destruction order. This is needed to establish that the object reaching destruction is the same live owner produced by Open/used by Save after callbacks.

For failure ordering, the missing witness is separate: each relevant reached loader/Open failure return after allocation must be correlated with the live manager child and Project state at return/unwind, plus prior durable file/process effects. CP230/231 did not reach or qualify those failure edges. A synthetic exception test can validate candidate consequences only and cannot fill the original occurrence gap.

## Queue disposition

No source or test increment is recommended from the newer evidence. Keep nested Save, public guard, same-owner destructor, partial-open cleanup, and global reset behind their existing evidence gates. This is a read-only queue reconciliation; no source, build, shared state, GUI, or REA state was changed.
