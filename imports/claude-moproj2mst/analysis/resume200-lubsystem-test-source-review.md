# CP200 system-lubricant test source review

## Verdict: PASS

The prior blocker is resolved. The replay-derived BCC transition expectation has been removed. The remaining BCC assertion checks only that object 1 is unchanged. This is independently justified by the qualified `bccdef_lubricant` contract in `source/docs/re/moproj2mst/mdt/L3/spec_save.md` §3 and `source/docs/re/commands/IMPLUB.md` §3: the helper is invoked on the found object for each default pair's master ID, while the admitted literal rebuilt list has masters 2 and 3. Object 1 occurs only as a slave, so it is not a helper target. The assertion does not calculate an expected transformed array or duplicate the helper's loop.

## Verified

- The revised rebuild plan, PASS plan review, and updated implementation report were read.
- The test loads the copied RS39 fixture's sub-project 2 / operation 3, requires `SfOprForming`, System enabled, lubricant ID 1, a manager-resolved readable lubricant KEY, and a present 3D step. It checks the loaded 3-slot/3-object mapping, IDs, kinds and `f08` candidate rule before admitting the literal four-record in-memory expectation.
- The recorded write is checked at its actual operation path, with no deletes, stable model/step identity, and byte equality to the retained 631-byte original KEY. The retained expected file is separately checked for exactly two `CNTACT` markers. The assertions keep serialized block count distinct from in-memory list count and do not claim fresh parity.
- The BCC assertion now checks only `objectOneAfter == bccBefore[0]`; no replay-derived transition remains for target objects 2 and 3.
- The CP198 frozen source hash remains `780eeec08f58c8a5afc92e512ae01944e5cb9c8e7c19da0b4b8fe071871349cb`. The current source is an implementation change; this review does not alter it.

## Scope and evidence limits

Only this review note was written. No source/test/build/GUI/REA database/parent/DMO/Claude/original state was changed. No build or test was run, so loaded-field admission and runtime behavior remain unverified.


## Follow-up: object-count admission fix

The compile fix replaces the undeclared objCount(*op) references with op->objs.size(). This preserves the precondition: the checked-in objCount implementation returns static_cast<int>(op.objs.size()) (source/apps/moproj2mst/src/model/objects.cpp), and the gate compares the value to exactly 3 in both the initial admission and loop guard. It does not change the intended three-operation-object requirement or weaken any slot/object mapping checks. The PASS verdict remains unchanged. No source, build, or test action was performed during this re-review.


## Follow-up: RS39 sub-project activation

The test now calls activateSubProject(p, 0, ctx), matching the fixture structure: the retained XML has exactly one Sub_Project element, with Active=2. That Active value selects an operation-chain entry; it is not the parsed sub-project vector index. The test then explicitly selects operation ID 3 and checks its SfOprForming class and System lubricant fields. This correction preserves the intended real-operation precondition and avoids indexing a nonexistent sub-project. PASS remains unchanged. No source, build, or test action was performed during this re-review. Parsed XML Sub_Project element count observed: 1.
