# CP223: retained Boolean witness audit

## Result

The bounded retained-capture audit found no admissible Boolean constructor timing tuple and no Boolean Apply/add occurrence nested in a complete Open. This is an evidence-coverage result only; it does not establish that either path never ran. No implementation increment or public-guard change is supported.

## Files and bounds checked

- `analysis/resume124-boolean-evidence-index.md`: retained inventory of 12 primary probe/status streams plus 27 additional message-bearing side files (39 streams, 1,490 raw message entries). It records no exact-site records for the Boolean Apply/add chain and cautions that streams are separate/overlapping, not a merged timeline.
- `analysis/resume196-bool-witness-audit.md`: reconciles CP124/CP164/CP171 and explicitly finds no same-invocation constructor E chain or Apply/add in a complete first Open.
- `analysis/resume164-boolean-next.md` and `analysis/resume214-boolean-timing.md`: controlling constructor, postLoad, and Apply distinction and required witness fields.
- `build/original-editor-probe-004/capture-evidence.json`, its pinned receipt `analysis/wave3-original-editor-probe-004-receipt.json`, and `build/original-editor-probe-004/probe.js`: inspected through the existing pinned checker, not by dumping full message streams.
- `admin/check_boolean_open_witness.py`: read the relevant checker logic and invoked `python admin/check_boolean_open_witness.py --require-witness` from the converter root.

The checker returned `valid_preflight_witness_unobserved`, exit status 1. Its three input hashes matched their pins, `coverage_issues` was empty, but the selected editor was Forming (`constructor_rva: 0xeb26f0`), the observed endpoint was only `selected_editor_first_grid_published`, whole-Open coverage was incomplete, and Boolean Apply/add timing was unobserved. `same_owner_frame_extraction` found zero `boolean_witness_frame` packets and lists all four Apply-chain roles (open, update_data, apply, add) missing. The checker is specifically an Apply/Open structural preflight and cannot establish constructor timing.

## Tuple coverage

| Required evidence | Retained result |
|---|---|
| Object factory branch and actual 0x50 allocation generation E, joined to `11e2480(E, incoming OPI)` | Missing from the retained qualified Boolean capture; Editor004 selects Forming `eb26f0`. |
| Same-E QWidget `377990` normal return and wrapper setup normal returns (`130830`, E+38 OPI resolution, conditional model retrieval if taken, E+28 bind) | No joined Boolean E invocation/normal-return tuple retained. Forming QWidget observations do not qualify Boolean. |
| Notifier `130ad1 -> 2cf3c0`, caller continuation `130ad6`, notifier identity and reached suppression/N+90 state, tied to the same E generation | Missing. No captured Boolean constructor frame establishes this branch. |
| Complete selected Open completion with nested same-owner current UpdateData -> `11e2870` Apply -> E+38 OPI resolution -> add normal return at `11e295a`/`11e2bcb`, with returned object/number | Missing. Editor004 stops at first-grid publication, not the selected whole-Open boundary; no Boolean witness frames exist. |
| Distinct `postLoad` add origin `11df0ee` | Kept separate by the reports/checker; it cannot be used as Apply evidence. No new `postLoad` claim is made. |

## Next action

No further static call-site decoding or candidate-only tests are justified by this audit. Keep the public constructor/Open guard. The next evidence step remains a fresh, naturally reached original Boolean capture meeting the CP164 constructor tuple and, separately, a complete Open plus Apply/add chain if first-Open Apply timing is required. The subtree's recovery instruction blocks acquiring another GUI-control session until a supported release operation and verifiable receipt are established; that precondition is not met here. No original process, GUI, debugger or mutable REA session was started, and no source/build/shared state was changed.

Only this analysis file was written.
