# CP203 converter crash-stack probe review

## Disposition

The manifest defect reported in the first review is repaired in the current
source. Converter-side source review passes for the requested ownership,
first-exception capture, false disposition, bounded cleanup, absence checks,
fixture integrity gate, and false guard. This is not authorization to change
the guard or launch the original; the separately requested harmless native
exception-handler preflight remains outstanding.

Current reviewed runner SHA-256:
`381a456c649c104c1fffaa25e2f9c472951188121181200272478e0b60270ec2`.
Current plan SHA-256:
`45f8c3df8174f0ca95f4e79985e88170f1fb49260d55f9b7120aeef99c49666f`.
The initially reviewed runner was
`admin/original_crash_stack_probe.mjs`, SHA-256
`cef324dd22b04698854e9d2fc72147df41cc559c117a2b48a1254de1646927d1`;
that version had the blocker below. The first plan hash was
`3b43a516531cf538629d10af3d2f077fc5fe1cb70d11c1c1efad1cd5df7b2eb5`.
No original process was launched and no GUI input was sent.

## Findings

1. **Resolved — manifest digest property mismatch (old runner lines 165,
   297–298).** `manifest()` returns `contentDigest`, not `digest`. The current
   source stores and compares `contentDigest` and requires
   `fixtureComparison.unchanged === true` in `captureComplete`. A failed or
   missing after-manifest therefore cannot qualify as complete.

2. **Guard and target ownership are otherwise appropriately fail-closed in the
   inspected source.** Windows, the exact opt-in argument, and the literal
   `ORIGINAL_RUN_REVIEWED === false` check all precede output-directory
   creation, Frida initialization, helper creation, and target launch. The
   launcher is pinned to the executable/project/helper hashes and expected
   B98 source/copy manifest. The checked helper creates one suspended child,
   holds its process/thread handles, and on EOF or explicit terminate performs
   handle-owned cleanup; the runner asks for termination on timeout/error,
   awaits helper exit/drain, detaches, then independently checks Windows and
   same-device Frida absence. A helper watchdog bounds child lifetime.

3. **The exception path matches the requested observation policy.** The
   injected source registers only `Process.setExceptionHandler`, selects only
   the first access violation, records exception/RIP/register/backtrace data,
   and returns `false` so normal exception handling continues. The runner
   requires a PID-matched handler-ready message before its one resume command.
   No `Interceptor` or GUI-input path is present. These are source findings;
   they do not establish installed Frida behavior on the original.

## Re-review checks

`node --check imports/claude-moproj2mst/admin/original_crash_stack_probe.mjs`
passed. Running the runner with the exact opt-in argument produced
`PRELAUNCH_FAILURE` with `BLOCKED before any target launch`; this verifies the
false guard and did not create a helper, output directory, process, or Frida
session. The source diff since the first review changes the manifest property
references and requires the unchanged condition for completion. Any further
source change invalidates this review. The review does not authorize changing
the false guard or executing the original.
