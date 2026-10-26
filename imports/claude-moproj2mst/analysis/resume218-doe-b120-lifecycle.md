# CP218 B120 Add Study lifecycle boundary

## Result

The retained B120 before/after pair does not define an implementation-ready
public converter increment. It records an interactive Add Study followed by
Save, while the candidate public conversion path opens the project and
unconditionally selects subproject 0. CP215's reader-I/O identity check is a
correct guard on an already-owned Type2 study's lazy InputTable read; it does
not create, select, generate samples for, or save a newly added study.

The smallest unresolved evidence unit is one same-occurrence original trace
from the actual Add Study command through its resulting SUB/Study identity and
selection, any sample/design generation call and result, and the subsequent
File > Save dispatch. Until those links are joined, the 100 empty sample rows
retained in B120 cannot be assigned to a particular generation routine or
order, and no helper can be source-closed without inventing the public trigger
and state transitions.

## Source and retained-state comparison

The existing bounded B120 audit (`analysis/resume203-b120-doe-state-static.md`)
reproduces the retained project-file hashes and establishes these facts:

* `_before` has one Type0 subproject, no `SfDoeStudy`, and no `DoeStudies` files.
* The post-GUI project contains the original Type0 plus Type2 `Id=1`,
  `Active=5`, display name `DOE Study 1`, and `SfDoeStudy name="DoeStudy1"`.
  Its seven retained Control files are outputs from that historical
  Add-Study-plus-Save sequence, not an isolated ordinary Open–Save result.
* The retained empty sample vectors establish saved state only. They do not
  identify whether Add Study generated them, whether a Save-time routine did,
  or whether a later operation changed the design.

In current candidate source, `src/core/convert.cpp` accepts flags and paths
through the public conversion entry point but has no selected-subproject or
Add-Study argument. After `openProject`, it calls `activateSubProject(p, 0,
ctx)` (around line 249). Thus passing the post-GUI B120 tree to the public path
selects Type0 even though the stored Type2 record says `Active=5`. Setting
`M2M_FLAG_WRITE_SAVE` enables the existing full Save path; it does not replay
the Add Study UI transition or change the selected index. `M2M_FLAG_DOE_TEMP`
is explicitly refused and is unrelated to an Add Study command.

`src/model/subproject.cpp::makeSubProject` and the DOE Study constructor
construct an owned Study when parsing an existing Type2 record. `parseSubProject`
then uses `readLazyStudy` on the Type2 `SfDoeStudy` path. The static lifecycle
order in `analysis/wave2-doe-lifecycle.md` separates that constructor, lazy
activation, sample wrappers, parameter parsing, selected DOE operation load,
and Save. These are existing-record/Open behaviors; they are not evidence of
the GUI's Add Study call sequence.

## CP215 boundary

CP215 adds `InputTableSource::readerIo` to bind the bytes read by
`readLazyStudy` to the same `StudyIo` later supplied to the contextual
notifier. The check rejects a null/mismatching identity before entering the
notifier prefix, after the constructor prefix and before notifier state is
changed. Its covered occurrence is the synchronous lazy-read route for an
existing Type2 Study. As `analysis/resume215-doe-io-review.md` records, the
source/context are not retained and no key, receiver, callback, or Study is
created by this check.

That boundary is orthogonal to B120 creation. It neither proves the natural
Add Study owner/selection nor upgrades the B120 files into a fresh original
Save witness. Keep the CP215 identity check and the public active-study Save
guard as they are.

## Exact missing witness and next evidence

Capture one fresh original occurrence starting from the no-study B120
before-state. The required linked observations are: the user action/command
and receiver; the constructor/manager/SUB/Study identities and assigned
subproject id/name; the resulting project Active value and selected/current
operation or DOE-output binding; every sample-generation request/branch and
actual result (including whether empty rows are generated or loaded); then
the same process/thread's File > Save entry and ordered DOE-save calls.
Record the same occurrence's return/failure edges and output paths so the
persisted files can be attributed to the action. A later Save or another
project/session cannot fill a missing link.

After this trace, decide whether the faithful converter needs (a) an explicit
public selected-study/Add-Study operation, (b) only a narrow Save-time helper,
or (c) no creation support within the existing m2m ABI. The current evidence
does not choose among these. Do not synthesize a Type2 record, set
`Active=5`, run a sampler, or expand the ABI from the retained after-state
alone.

## Qualification

This is a read-only source/evidence comparison. It does not claim fresh
original runtime parity, a complete absence search, or that Add Study is
outside the agreed Open-to-Save scope. It makes no statement about the
per-file historical output differences handled in the separate B120/B121/B122
file-output audit. No source, fixture, build, GUI, original process, or shared
state was changed.
