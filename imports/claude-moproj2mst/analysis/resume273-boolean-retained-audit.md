# CP273 retained Boolean checker audit

## Outcome

The bounded offline checker was run once against its pinned retained input using `python admin/check_boolean_open_witness.py --self-test --require-witness`. It exited 1 as expected for a structurally valid preflight with no admitted witness. All 12 checker self-tests passed. No input errors were reported.

| Input | Identity qualification | Coverage result |
|---|---|---|
| `build/original-editor-probe-004/capture-evidence.json` | SHA-256 `e4d0a277c4bbb074fa1e038ce3870e64ebb87ec884665cb1bb0ddd0221f99ced`, matches checker pin. Root receipt `analysis/wave3-original-editor-probe-004-receipt.json` SHA-256 `63f392d69d9def0699f963a3722b9f4e0b257c0192ad53f4097a3ea2f5250439`; probe `build/original-editor-probe-004/probe.js` SHA-256 `0251ee49c0378af50e0778c8a584a5243402b10f26ab64fc315466bf10c68203`; both match pins. Module identity authority is the pinned root receipt. | 73 observations; editor kind `Forming`, constructor RVA `0xeb26f0`; selected whole-Open coverage incomplete. CLI loader return observed, CLI post-open and startup completion unobserved; GUI completion is a different profile. Boolean Apply and Apply-origin creation within Open are unobserved. Zero same-owner frame packets; missing roles: Open, UpdateData, Apply, add. `witness_admitted=false`; no first/absence claim. |

The checker only accepts this pinned Editor004 input; it does not enumerate or admit arbitrary retained captures. This input is positively identified as Forming, so it cannot fill either CP164 Boolean timing cut. The result is a coverage miss, not evidence that either event never occurs. The static module profile names DEF_GUI_MO_64 v11.0.2 with SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; this identifies the expected target, not a runtime Boolean occurrence.

## Next action

No retained input checked here joins the Boolean same-E constructor/notifier chain or a complete-Open nested Apply/add chain. Keep both timing claims unqualified. A future timing claim requires a qualifying Boolean runtime record with consistent process, thread, owner, invocation and allocation lifetimes, matched normal returns, and complete Open coverage for an Open-placement claim. The resume instructions prohibit acquiring another GUI session until a supported release operation and verifiable receipt/status exist; do not initiate that capture before then.

This audit read retained local evidence only. It did not access the original executable, GUI, mutable REA, source, build, or shared plan. Only this report was written.
