# Checkpoint 62: keep the DBLIST warning prefix out of runtime dispatch

## Decision

Do not add a candidate warning helper or test yet. The static evidence fully
binds two constant strings and the imported UI call, but it does not fully
qualify the conditional input that should trigger the warning, the list of
missing database names, or the owner/context passed to the dialog. A helper
that accepts a guessed list and emits a plausible message would convert those
unknowns into candidate behavior and falsely suggest that the DBLIST path is
recovered.

The safe next artifact is still an evidence-only boundary. Do not dispatch
`QMessageBox::warning`, add a GUI/session substitute, attach DBLIST state to
`SessionPart`, or enable the public B127 Type 2 path.

## What is actually qualified

The static capture `analysis/resume60-dblist-reportlist-6554a0.asm` is tied to
`DEF_GUI_MO_64.exe` SHA-256
`FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. The
corrected tag assignment recorded in `analysis/resume60-dblist-slot-next.md`
is DBLIST -> `FUN_1406554a0(param_1, node, param_3)` and ReportList ->
`FUN_140655df0(param_1, node)`. Retain that mapping regardless of the older
capture filenames or earlier reversed prose.

At `0x1406558cc`, IAT `0x141e0bf40` resolves through the original PE import
table to the `QtGui4_64.dll` `QMessageBox::warning` overload. The warning title
literal is `Error in reading` (VA `0x1424e3f38`, raw offset `0x24e2738`,
17 bytes including NUL; SHA-256
`4bcc645adfac273583213a153e3526c1e0c58a1b6788c250a0eae048b06bc602`). The
warning text prefix is `The following Database files referred to in the
Session file could not be found:\n\n` (VA `0x1424e3ee0`, raw offset
`0x24e26e0`, 83 bytes including NUL; SHA-256
`fc3188ef54fc8c010cec117d559432d296b501a953b0a445e7b55a459393673c`). The
call setup has a QWidget pointer, two QString locals, button flags, and zero
as the fifth argument. The title local and prefix construction are visible in
the bounded handler assembly.

This is sufficient to preserve the literals and identify the API boundary. It
does not establish a pure helper's complete input/output contract or justify
calling a modal UI API from a converter path.

## Unresolved input, condition, and owner

The warning path is conditional on local state and the result of
`FUN_1402cb7a0(0x143d766b8)`; processing is bypassed when that result equals 5.
The meaning of those state values and their relation to a missing DBNAME are
not established. The handler iterates a collection, compares QString values,
uses a local buffer sized `0x400`, and calls helper functions before creating
the warning arguments. The exact collection element source, DBNAME path
resolution rule, missing-item test, buffer data type/capacity behavior,
message concatenation, and separator policy remain unresolved.

Although the warning call receives a QWidget pointer, its concrete identity and
ownership lifetime are not established from this capture. The source `param_1`
is the part-level receiver for `FUN_1406554a0`, but it is not shown to be the
QWidget or a DBLIST storage owner. The candidate `SessionPart` remains only a
possible structural place to retain the raw XML node; no native field mapping
has been proven. The QMessageBox return value and its effect on handler state
are also unqualified.

These missing contracts are material: without them a regression cannot
discriminate the no-warning path, the warning path, and which referenced
database names should be included. A test for the two fixed literals alone
would only test duplicated constants; it would not validate conditional
behavior or a meaningful DBLIST integration contract.

## Bounded next evidence

Before proposing a helper, close the following static dependencies against the
same original PE identity, using short assembly/import/string views rather than
repeating a broad or timed-out decompile:

1. Trace the direct DBLIST child's `DBNAME` value through the loop and establish
   exactly what comparison determines a missing database entry.
2. Identify the helpers that produce the collection, the `0x400` buffer, and
   the second QString argument. Record exact bounds and truncation/error paths.
3. Bind the QWidget getter and caller `param_3` to their actual meanings and
   trace the condition under which the warning call occurs. Keep the original
   global `+0x1c` gate distinct from the handler's later state check.
4. Determine whether the database-reference result is stored anywhere after
   the dialog call. Do not infer an owner from the QObject pointer or the
   part-level receiver.

Until that work establishes a pure data contract, keep the isolated DBLIST
reader as a raw direct-child extraction seam and preserve the public lifecycle
refusal. The original 16-case connector probe and candidate project tests do
not qualify this `.DS` warning path.

Read-only evidence audit. No source, test, CMake, build, GUI, runtime, fixture,
or mutable REA database was changed or executed.
