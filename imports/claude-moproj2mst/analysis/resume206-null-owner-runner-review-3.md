# CP206 first-AV core focused static review — HOLD

Reviewed `admin/original_null_owner_probe.mjs` at SHA-256 `026a55888f6d711184b77ab785bb7c341c0dd3be6bd82855d415325d5c799c2b` against the CP206 hook spec, prior runner reviews, and qualified CP205 capture. No original process, Frida session, probe, or GUI interaction was run.

## Release blocker: retained hook events are not reconciled with core counters

The injected first-AV core copies `sequence`, per-site `counts`, and `overflow` into `hookSnapshot` before backtrace work (lines 172–178). The host retains that core (line 320) and selects its snapshot as `counters` (lines 396–397). Yet `hookCoverage.counts` is derived solely from retained `scriptMessages` (line 398), and `hookEvidenceQualified` never requires equality between those counts and `counters.counts` (lines 400–408).

Consequently, if one or more pre-AV hook messages are missing from the retained host stream while at least one record remains per site, the host can still set `captureComplete=true`. In particular, an injected site count of 16 can appear as 15 retained records, bypassing the host's cap check when the snapshot overflow is zero. The missing record could be the producer store, post-store confirmation, consumer load, or call operand needed for the causal comparison. The runner's classification string remains indeterminate, but `CAPTURE_COMPLETE` would incorrectly certify complete raw hook evidence.

Require the core snapshot to have all four expected numeric site counts; compare each against the retained hook count, and require the relevant totals and overflow state to be within bounds. Treat any mismatch as `INCOMPLETE`, retaining both sources and the mismatch in the report. This check should precede any future reviewed run.

## Checks that passed statically

- The compact `first-av-core` send occurs before `Thread.backtrace` and the expanded `first-access-violation` send. It records PID, exact creation-filetime text, RIP, RCX, fault address, guarded module/memory detail, a copied hook snapshot, and the `return false` disposition. The handler still returns `false`. A missing core or missing expanded AV prevents `captureComplete`; the expanded receipt still requires an accurate backtrace and registers.
- The host receives and retains `first-av-core` independently of the 96-message bounded array. Its qualification checks the PID, exact creation-filetime text, AV type, essential scalar fields, hook snapshot presence, and disposition. The fallback parser for the older separate counter message is syntactically well-formed, though the new core is the expected source.
- Original v11.0.2 image, helper, project, source/fixture manifest, command, cwd, four hook RVAs/bytes, and creation identity pins remain as reviewed. The false launch guard and exact opt-in argument remain. The single resume follows handler-ready validation; natural-exit wait, fallback cleanup, process-absence checks, detach check, fixture comparison, and exclusive output retention remain in place.

This HOLD concerns qualification of retained evidence, not a claim that Frida actually loses messages. The four hooks also cannot identify a later writer or prove object lifetime; a later reviewer must pair the selected fault, consumer, and same-frame producer before making a causal claim.
