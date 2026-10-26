# CP212 one-run original release and restoration

Independent technical review at `analysis/resume210-outer-frida-review.md`
released exactly one fixed owned-original trial. The disabled runner SHA-256 was
`9e584f00ad1b6eeef6a384e069cf6d3e075060a6de062220488d5fcf829cbe58`.
For the trial, root changed only three assignments in
`admin/original_outer_loader_frida_probe.mjs`: `ORIGINAL_RUN_REVIEWED` and
`HOOK_BYTES_REVIEWED` from false to true, and the single unconditional
`result.captureComplete = false` preparation value to true before its tested
prerequisite conjunction. `node --check` passed. The released SHA-256 was
`cb038b92021f0599e83322a362b9e2402c8357cb95554d266889e7a2cad03707`.
Reapplying exactly those three substitutions in memory to the restored
disabled file reproduces that released hash.

The only original command was
`node admin/original_outer_loader_frida_probe.mjs --execute-reviewed-probe`.
It returned exit 0, printed `CAPTURE_COMPLETE`, and retained raw receipts at
`analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z/`.
The runner was immediately restored to its three false preparation values;
its current SHA-256 again equals the reviewed disabled hash above. No GUI
control API was used. The original executable belongs to the v11.0.2
installation, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The summary retains `releaseBlocked` text from the disabled preparation
template. It is stale metadata after the recorded independent PASS and exact
guard release, not a live runtime check. Independent raw-capture review must
still qualify the actual hook, fault-path, file and cleanup evidence before a
behavioral claim is integrated. There is no authorization for an automatic
second original run from this one-trial release.
