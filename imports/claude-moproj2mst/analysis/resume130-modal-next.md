# CP130: retained modal activation frontier

## Result

The retained original-run index does contain actual **No** answers for some Close-question dialogs: `evidence/gui_mo/oracle_b/_tools/rs_log.tsv` records `close / dialog / 1 / Question / answered No` for `129_B_DBSTEP_CNV` (line 100) and `133_B_DBSTEP_BOOL3` (line 114). The following rows report saved artifacts and successful exit. This is stronger than a screenshot and qualifies those captured Close workflows as returning No.

It does not close the default-Yes modal frontier under investigation. Those rows name the Close question; they do not identify the must-be-last Save warning, nor record the input method/time, events delivered before the answer, callback receiver/model state, `done(int)`, `exec` return, or Save-writer reachability for that warning. The `133_B_DBSTEP_BOOL3/dialog_1.png` image depicts the separate current-operation Close question with Yes focused; it does not contradict the log's later No answer, and it is not a warning activation trace. Treat focus as an initial state and the log as a post-action answer summary.

## What can be source-cut

For those logged Close runs only, the original question was answered No and the logged action continued to save/exit. The captured evidence does not justify translating this into a default answer: No is a user-selected response in the log, while retained static analysis qualifies a Yes initial default for the fresh style/focus path. Nor does the summary establish the event schedule preceding No. It cannot be reused as evidence that a sole-Yes Save warning auto-returns Yes, or as a modal schedule for another occurrence.

The static must-be-last path remains conditional and as documented in `analysis/resume116-default-dialog-result-producer.md`, `analysis/resume120-default-dialog-result.md`, and `analysis/resume121-default-dialog-done-bridge.md`: delivered activation of the sole Yes button maps to `0x4000`; absent activation, there is no closed result producer. The nested Qt loop may dispatch callbacks before the answer, but the retained captures do not establish whether or which callbacks ran in the logged Close occurrences or in the warning.

## Smallest missing witness

A source cut for default-Yes warning handling still needs one original occurrence of the must-be-last Save warning from modal entry through return: capture the actual delivered inputs and callbacks in order (including an explicit empty-delivery observation), relevant receiver/model state before and after each callback, the activating action, clicked button, `done(int)`, `exec` result and mapped standard button, Save return, and writer reachability. Keep the outer Close question and its nested Save warning as separately identified intervals if claiming the composed Close route. Existing `rs_log.tsv` Close No summaries do not substitute for either interval.

No GUI was used; no source, build, test, or mutable REA state was changed. This is a retained-evidence report only.
