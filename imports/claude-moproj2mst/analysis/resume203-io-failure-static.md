# CP203 converter RIO failure-order audit

## Result

The narrow failure prefix remains source-bound in current converter code, and
the CP164 candidate-only failure conjunction is already present. This audit
found no additional source-ready candidate behavior that advances original
RIO parity. Keep enabled remesh write-open failure guarded at the public
boundary. The missing acceptance evidence is a single original Save run that
binds failed remesh open, handler selection, filter/termination path, and
external child outcome in temporal order.

## Current source and retained candidate evidence

In `source/apps/moproj2mst/src/core/convert.cpp`, the native full-Save path
records the MST attempt/return, invokes `saveRemeshAtProjectSave` using the
session remesh owner and logical current directory, and only afterward maps
an incomplete remesh outcome to `M2M_E_UNSUPPORTED`. The remesh helper's
enabled failed-open route is not treated as an ordinary ignored write failure.
The result matches the previously retained CP164/CP171 implementation
qualification: project/MST false returns do not bypass the remesh tail, while
the terminal native outcome is unresolved.

`source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp` already
contains the CP164 conjunction regression. It proves a pre-truncation remesh
write denial, creates and retains an MST sentinel under write denial, scripts
the project-output post-close error, checks prior Boolean BWZ/INI effects, and
expects the candidate unsupported boundary. Its observed event sequence is
project-output open → aboutToClose → close returned → error query. It does not
observe input Open or Save-current in that event vector, and its assertions
are explicitly candidate adapter/device results. This is an existing bounded
candidate test, not an original GUI Save result or native terminal acceptance.

Current CP201 validation records 361 registered tests with zero failures,
while its historical full-Save comparator remains 93 SAME / 5 DIFF and is not
fresh-session raw-byte acceptance. This audit did not rerun validation.

## Static original failure prefix and missing witness

Retained static evidence for original DEF_GUI_MO_64 v11.0.2
(SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`)
supports the conditional prefix: active operation Save; `.moproj` write;
operation property-save visits; MST and following study output; then local
remesh handling. A returned false from project/MST does not skip the later
stages. If enabled remesh `fopen` returns null, the first `fprintf` reaches
the invalid-parameter dispatcher. The retained startup contract statically
shows an encoded-null handler initialization before main and a dispatcher
that decodes the handler slot. If the slot is still null and the default
handler/filter take their ordinary returning path, the handler requests
`TerminateProcess` with `0xC0000417`. This is a conditional static inference,
not an observed original runtime outcome.

The exact missing same-run witness must bind: original image/module identity;
successful Open and enabled remesh state; Save-time CWD and resolved primary
path; pre-attempt write denial; actual null `fopen`, first `fprintf`, and
module `errno=22`; decoded handler-slot value and selected branch; filter and
debugger behavior; whether `TerminateProcess(0xC0000417)` is reached/returns;
external child status; and before/after raw outputs including the earlier
project/property/MST effects. Instrumentation's possible effect on filter
behavior must be recorded. A matching status alone, static handler setup, or
candidate child endpoint cannot fill this gap.

## Decision

No source or test change is justified by this pass. The CP164 conjunction is
already the narrow source-ready candidate test; another pair/triple denial or
candidate endpoint exercise would duplicate candidate precedence without
establishing original behavior. Preserve the unsupported public guard and
defer terminal-outcome implementation until the same-run original witness is
available and qualified.

This was a read-only converter audit. No source/test edit, build, test run,
original process, GUI control, or mutable REA action was performed. This
report is the only file written by the audit.
