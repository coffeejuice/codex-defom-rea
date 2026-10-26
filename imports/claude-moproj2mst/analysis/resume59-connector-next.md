# Checkpoint 59: next connector-adjacent dispatch boundary

## Finding: the retained connector edge is not selected by the public conversion path

The candidate-only Checkpoint 54 test reads the full `120_B_DOE` MOPROJ and
explicitly calls `activateSubProject(p, 1, ctx)` to assert the requested Cogging
operation 3 -> DoeOutput operation 6 edge. That verifies parser integration for
the actual project XML, but it does not cover public active-operation dispatch.

The retained MOPROJ has two sub-projects. Sub_Project 0 has `Active="-1"`; its
five-item flow includes Cogging operation 3 but requests no connection for that
item. Sub_Project 1 has `Type="2"`, `Active="5"`; its six-item flow contains
the tested Cogging -> DoeOutput edge at item indices 2 -> 5. In source,
`initProject` starts `Project::activeSub` at 0; `convert` calls
`activateSubProject(p, 0, ctx)`; and `activateSubProject` stores the selected
index in `p.activeSub`. The C ABI (`m2m_convert_w` / `m2m_convert_utf8`) forwards
the project, output, and flags to `convert`, with no sub-project-selection
argument. Therefore this retained edge is not reached through the present
public conversion call on the unmodified fixture. A C ABI run on this input
cannot be claimed as a full-project connector acceptance run for operation 3 ->
6.

The next smallest candidate-only dispatch check is internal and explicit: after
the full-project parser check succeeds, activate Sub_Project 1 and call
`engine::openActiveOperation(p, ctx, fx)`. The XML `Active="5"` selects its
DoeOutput item, so the call tests the step immediately after connector
admission: active-item resolution and class `loadOperation` dispatch. It must
be labelled an internal active-sub-project check, not public API behavior.

## Source-supported first guard for that check

The exact retained DOE document is
`DoeStudies/DoeStudy1/Control/COG_P1_B1.DS`, 2,013 bytes, SHA-256
`97e4aaa28aa545dd08360e98191fba3b2cfb6c39098e10aeb3b45d02c33e563d`. Its first
Session contains an `<Image>00000000</Image>` child. It has no CR bytes, so the
candidate's current `readSessionDocument` text-mode CR refusal is not the first
known stop for this file.

The `openActiveOperation` chain resolves `flow.activeItem`, the item operation
id, and its registered class, establishes `OpClass::current`, obtains/resets the
engine model, and calls `loadOperation`. DoeOutput's override resets its local
state, resolves the preceding operation/model, resets and shows the output
model, then calls `loadClassData`. On the Type 2 branch `loadClassData` checks
owned Study and configured I/O, invokes `readDoeDocument`, and only later calls
`finishDoeOutputClassData`. `prepareCurrentOutputSession` derives the DOE
control `.DS` path, resets the retained output document, opens/reads the file,
and enters `readSessionRecords`. `importSession` processes the first Image
through `qUncompress` and then throws the explicit
`M2M_E_UNSUPPORTED` refusal because QImage PNG decoding is not recovered.
That precedes task allocation, Report QObject/task-factory work, class-data
completion, and DoeOutput's later “selected session/global task application”
guard. This order is source-supported by `ops/noop.cpp`, `doe/session.cpp`, and
`doe/session_records.cpp`, and the Image field is present in the bound fixture.

Thus the exact expected first candidate refusal for this fixture is:

```text
code: M2M_E_UNSUPPORTED
message: DOE session Image qUncompress complete; QImage PNG decoding remains unsupported
```

The warning callback is bound by the DOE document preparation path; retain any
recorded Qt warning independently of the refusal message. Do not assert that
DoeOutput class-data completion, global-task application, Save, or Close was
reached.

## Proposed acceptance contract

Use a separate disposable complete-project copy under configured converter
scratch, preserving the retained source and the Checkpoint 54 fixture. The
candidate-only check should:

1. Assert the input project hash and all-tree size/SHA manifest before the run;
   configure `Project` with the copied sibling DB path, `makeRegistry`, recording
   DOE I/O, and `Ctx` with no session.
2. Assert `openProject(p, file, ctx)` accepts the MOPROJ and sets `p.openedOk`;
   assert the document contains two sub-projects.
3. Explicitly activate index 1. Assert it is Type 2, marked loaded, has
   `flow.activeItem == 5`, maps that item to operation 6 `SfOprDoeOutput`, and
   preserves operation 3 -> 6 in `next` / `prev`.
4. Invoke `engine::openActiveOperation(p, ctx, fx)` and capture the thrown
   `Error`. Assert the exact unsupported code/message above. Also assert the
   recording DOE I/O observed a read of the copied `DoeStudy1/Control/COG_P1_B1.DS`
   with 2,013 bytes and the fixture SHA-256, and preserve its warning list.
5. Record the initial/final `fx` writes and deletes and the complete input-tree
   manifest. Assert the input tree is unchanged. Do not expect a returned
   `Operation*`: the qualified stop is an exception during that operation's
   load. Do not conflate the in-memory reset/read prefix with a rollback.

These assertions establish the candidate sequence from a complete retained
project, through real registry/parser admission, into active DoeOutput dispatch,
and up to the first evidence-bound unsupported Session payload. They do not
validate the remainder of DoeOutput class loading or operation acceptance.

## Original/API evidence limits

`analysis/connector-admission-original.json` remains an execution of
`0x140074f50` with mocked type and owner-index callbacks; it does not establish
the original's active-sub-project selection or DoeOutput dispatch. The retained
Oracle-B MOPROJ and `.DS` are input evidence, not a fresh original execution.
Static original DoeOutput initialization and dispatch decompiles support the
flag and function ordering, but no fresh original/candidate pair has recorded
the 3 -> 6 graph and active operation in equivalent state. The public
conversion API's current unconditional index-0 activation further means the
internal index-1 check cannot substitute for that pair.

To claim public API behavior for this cross-sub-project edge, first recover and
evidence the original active-sub-project selection transition and its public
input/control surface. Do not add an ABI selector or rewrite the fixture to put
the edge in Sub_Project 0 under a parity label. The distinct public Open/Save
and full-project connector acceptance remains pending.

Read-only source and fixture audit. No source, build, project, GUI, or REA
database was modified.
