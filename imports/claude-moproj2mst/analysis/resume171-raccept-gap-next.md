# CP171 read-only acceptance gap audit

## Result

The highest-yield logged-gap blocker that can be resolved from retained evidence without GUI is the optional PACK header check in `m2m.mdt.mdt_write`. It is already proven against an exact retained installation fixture: CP124's focused run bound `C:\Program Files\SFTC\DEFORM\v11.0.2\PACK\pack.dat` (11 bytes, SHA-256 `f82967bf8117671aad02e8621549adbb6caf1e5265c55818ec806d9d6b250c10`) and passed all six cases, including `header_text_from_pack_dat` (`analysis/resume124-pack-focused-check.md`). CP170's full receipt still logs `M2M_INSTALL_FIXTURE unset` (`analysis/resume170-compilefix-tests.json`), so the evidence exists but has not been carried into the current validation environment. This closes only that optional fixture-coverage blocker when the verified path/hash is rebound; it does not by itself clear the suite's CP170 logged-gap classification or amend its frozen receipt.

The remaining five logged-gap suites require inputs not established as retained and qualified here (the exact `87_B_HTFF` Oracle-B state/output, independent deflate vectors, Props-L2 class/source ownership, missing P07 pairs, and pristine pre-GUI-edit Save inputs). Reusing unrelated cases or synthesizing oracles would not close them. The CP170 diagnostic `m2m.s.save_folder` still logs the same five historical DIFFs. B98's sole MST delta has already been explained by its differing embedded project root; raw equality still needs a matched-root original/candidate Save and therefore is GUI-gated. The three DOE DIFFs represent different action-created states, and A55 still lacks its process-bound event/mode witness.

## One concrete independent next action

Prepare a narrow, read-only requalification receipt for the existing CP124 PACK result against CP170's current test source and frozen binary: verify the currently installed fixture's bytes/hash against the retained CP124 binding, inspect the CP170 `mdt_write` test's environment-variable contract, then record the exact focused invocation and output needed for root to rerun only that test with `M2M_INSTALL_FIXTURE` set. Do not run or rebuild it in this audit. The result should say that CP124 demonstrated the assertion on its source snapshot, while CP170 has not yet consumed that evidence; only a CP170-bound focused run can close today's logged event. Keep active-cogging refusal separate from the optional fixture event.

This is independent of the pending CP171 implementation work and of all live REA, GUI, original-executable and mutable-state gates. The CP170 baseline remains 361 registered tests, 354 clean passes, six logged-gap suites, one diagnostic-difference suite, no failures/skips/incomplete outputs or fixture changes, and historical 93 SAME / five DIFF (`analysis/wave3-checkpoint170-validation.json`).

## Qualification and limits

`m2m.s.save_folder` is diagnostic and its green status does not mean its five cases match. Historical rows remain `A:RS_55_Multiple_Operations_Example1`, `B:120_B_DOE`, `B:121_B_DOE_VAR`, `B:122_B_DOE_GEO`, and `B:98_B_DS3F`; retain the raw classification. Fresh original Save/Close is not established by the retained CLI audit (`analysis/resume170-headless-oracle-next.md`), and app-control release remains unverified. No source, build, fixture, executable, GUI, live REA session or shared state was changed or run for this audit.

Evidence read: `analysis/wave3-checkpoint170-validation.json`, `analysis/resume170-compilefix-tests.json`, `analysis/resume170-root-integration.md`, `analysis/resume124-acceptance-queue.md`, `analysis/resume124-pack-focused-check.md`, `analysis/resume149-logged-gaps-next.md`, `analysis/resume149-b98-path-contract.md`, `analysis/resume170-headless-oracle-next.md`.
