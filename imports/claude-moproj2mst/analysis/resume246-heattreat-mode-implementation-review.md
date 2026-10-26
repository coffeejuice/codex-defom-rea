# CP246 HeatTreat Mode implementation review — PASS

The three test-only HOLD findings are resolved. The production migration previously passed semantic review; repaired tests and focused receipts now support release of this bounded leaf. No outstanding defects found.

## Repair verification

1. The L6 Mode table now binds each property name to its HTPropsBlock flag and native offset. For both bool values, each setter starts with distinct native sentinels and verifies the selected block/word plus unchanged other words. Each getter explicitly sets the selected property flag and seeds a distinct native sentinel before dispatch, then verifies Bool output, overwritten selected word and unchanged other words. A no-op Diffusion/PhaseTransformation getter or a command that writes all three words will fail. The extra temporary assignment of !enabled before the final enabled assignment is redundant but harmless.

2. The detached-step interval now invokes all three getters with known true property values and checks returned Bool values. All three saved native sentinels and the existing STEPDEF sentinel remain checked, both before and after step restoration. Normal and exceptional restoration are retained. Existing STEPDEF dispatch checks remain intact.

3. Viewer Mode snapshots now use control-relative offsets from `&model.step->sim_ctrl`, matching SC+0x108/0x110/0x10c. All three direct parked-command refusal assertions remain, followed by the corrected word checks and existing whole-step/state/effect comparisons.

## Production conclusion retained

Deformation writes trans[1], Diffusion trans[3], PhaseTransformation trans[2], each via one simCtl call and explicit enabled ? 1 : 0. simCtl retains geometry-first nullable resolution. Missing step is a guarded no-op. The property helper receives a copied bool before opModel; setter defaults → property store → native attempt → refresh and getter native attempt → Bool construction remain. Private discriminator dispatch is correct. STEPDEF stays unchanged. Callback template/setScWord/control alias removal leaves no remaining consumer. No identity/storage/C ABI or cache1 gate changes are introduced.

## Focused repair receipt

`analysis/cp246-heattreat-mode-repair4-build.json` records configure/build exit 0 for both expected targets, source_stable_during_build true, and matching before/after fingerprint `3c0d9491216beeb02df264b0c303e8b84f37e8ab906bc2c00030203750e6705d`. Recorded executable SHA-256 values are:

- m2m_mdt_l6_slots.exe: c8af35b5938dc93bef6e5f9df4ab829d7d1ac3e03eca10cc55c52092d261b2f9
- m2m_mdt_viewer_box.exe: f22eb247f8671f007d749a475463efea3dd3049ea4ca7155b884c4c2fb422015

The inspected LastTest.log records m2m.mdt.l6_slots with 0/12 failures and m2m.mdt.viewer_box with 0/57 failures, both Test Passed, corroborating the implementation report's repair 2/2 result. Earlier fingerprints/hashes in that report describe the pre-repair run; the repair4 receipt above is the applicable repaired build.

These are reconstructed fixture results, not a full source-stable regression/freeze, original execution, public Open or Qt reentrancy proof. Historical slot provenance and remaining ownership/alias limitations stay as qualified in the approved design. Concurrent reaction-rate viewer work is outside this Mode review.

This report supersedes the earlier test-only HOLD after checking exactly its three required corrections. Only this review report was written; no source/test/build, shared recovery state, original process, GUI or REA changes were made.
