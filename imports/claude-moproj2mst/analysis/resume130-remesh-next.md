# CP130 remesh handler: source effect and smallest next probe

## Source-closed effect

The retained original PE closes the remesh failure prefix, conditionally on
reaching enabled Save and receiving a null `fopen("wt")` result. The first
`fprintf` consumes that null `FILE*`; its CRT branch writes `EINVAL` (`0x16`,
decimal 22) and calls the converter module's invalid-parameter dispatcher.
Startup initializes the module handler slot to encoded null. If that value
is still decoded-null at dispatch, the default branch clears the module dword
`0x1450540e8`, captures a local context, chooses a source-defined unwind or
fallback path, and invokes the process filter. Every ordinarily returning
filter path reaches `TerminateProcess(GetCurrentProcess(), 0xC0000417)`.

These are instruction-level facts and a qualified source prediction. They do
not show that the slot remained null in an actual run, which context/unwind
branch ran, whether the filter returned, or that the OS observed the expected
status. The context operation is a virtual projection for exception
reporting; it is not a live stack unwind and does not execute Save-scope
destructors.

## Evidence and later receipts

The original is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The controlling sources are `analysis/resume103-remesh-terminal-outcome-architecture.md`,
`analysis/resume109-remesh-handler-prelude.md`,
`analysis/resume110-remesh-handler-model-implementation.md`, and
`analysis/resume116-remesh-invalid-parameter-occurrence.md`. Their PE ranges
qualify the unchecked writer, first invalid-parameter call, startup handler
assignment, default-handler prefix, and termination argument.

The later CP108/109 worker and CP111 isolated-CLI evidence records candidate
child termination using a fixed candidate endpoint. That endpoint omits the
native CRT context/filter prelude. These receipts prove the candidate
supervisor's observation path and candidate output effects; they do not prove
that the original reached the native boundary or exited with that status.
Retained B133-before has no remesh sidecars, and no original run receipt
records an enabled-remesh Save. The candidate overlay is therefore not an
original historical occurrence.

## Smallest useful next probe

When the original GUI action gate is available, use one fresh, owned original
process and one disposable B133-derived directory. Seed the valid primary
and fallback remesh files so Open enables remesh; retain a parent-owned
read-sharing handle that permits Open's read but denies Save's write-open.
Verify before launch that this exact primary is the writer's relative path
under the project directory and is not rewritten earlier. Invoke one Save,
then let the process exit naturally while the external parent records:

- original image hash, process identity, and loaded module base;
- full before/after project manifests and raw bytes for the two remesh files
  and earlier Save outputs;
- the denial handle's open mode and lifetime;
- child OS exit status, timeout/kill state, and whether the child was stopped
  by the supervisor.

The strongest low-perturbation positive result is a natural child exit with
`0xC0000417`, the primary still byte-identical under the pre-truncation
denial, and no later output changes, all bound to the same source image and
fixture. That would establish an original process outcome for this controlled
case, but without in-process tracing it would still not prove the actual
decoded handler slot, captured context, selected unwind branch, or filter
result. A debugger-based trace can expose those values but changes
`IsDebuggerPresent`; it must be reported as an instrumented branch and cannot
stand in for the uninstrumented process outcome. Do not combine the two
claims.

This is one focused experiment, not a request to synthesize a handler or
force termination. If the original process does not reach the expected
status, retain its complete outcome and do not retrofit the source prediction
to match. Keep candidate public refusal and the candidate fixed endpoint
qualification unchanged until the actual original result is observed.

No GUI, original executable, source/build/test, or mutable REA action occurred
in this read-only audit.
