# Checkpoint 48: indexed project with a real DoeOutput connector edge

## Qualifying complete project

The broader retained Oracle-B capture corpus contains a complete project whose requested flow edge actually touches the corrected DoeOutput type:

`C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b\120_B_DOE\COG_P1_B1.moproj`

Its inspected SHA-256 is `60ADDC2722E85F92A088FA07F77E8A966928AB5B934F161DF87DCF6A417ED30E`. The parent directory contains the complete project capture, including `.DB`, `.MST`, object/material/equipment/task folders, and DOE study artifacts. Use only a fresh disposable copy for any future comparison; the retained corpus itself must remain unchanged.

The XML contains two Sub_Project blocks. In sub-project index 1, the requested edge is:

| Diagram item index | Operation id | Class | Connector request |
|---:|---:|---|---|
| 2 | 3 | `SfOprCogging` | target item index 5 |
| 5 | 6 | `SfOprDoeOutput` | receives edge from index 2 |

So the exact edge is **operation id 3 `SfOprCogging` → operation id 6 `SfOprDoeOutput`**. The retained class initialization review gives Cogging flags `0x1a` (3D output `Control3d`) and DoeOutput flags `0x20097` (combined `Control2d3d` input/output). This requested edge exercises the combined DoeOutput input compatibility in a complete project. It is a stronger real project fixture than base case 51, whose Cycle item has no requested connections.

## Cycle and DoeOutput corpus search result

The indexed base corpus case table identifies only two Cycle examples (cases 48 and 51), both `SfOprCycle:1;SfOprForming2D:2`; direct inspection of the installed SI case 51 shows its only requested diagram edge is Forming2D→Forming2D, while Cycle has no connection request. The base corpus case 52 contains one DoeOutput operation in its class inventory, but source analysis establishes it has no DiagramItem in the flow chart, so it cannot test its connector flags.

Searching the broader captured Oracle-B project directory found 11 project snapshots with an actual edge incident to Cycle or DoeOutput; they are DOE-case and before/diff variants, not 11 independent test cases. The qualifying representative above is the canonical `120_B_DOE` snapshot. The scanner parsed each `.moproj` Sub_Project/DiagramItem/Connections/ItemIndex relation and retained only in-range target indices where either endpoint class was Cycle or DoeOutput. No installed 2D Examples or 2D LABS project had such an edge.

## Validation boundary and cautions

- The 16-case original native probe still only tests `0x140074f50` with mocked type and owner-index callbacks. It did not open this DOE project or observe its flags.
- A fresh original-versus-candidate comparison of `120_B_DOE` would validate the connector edge only if project Open reaches the second Sub_Project and reports operation id 3→6 as accepted. Capture the per-subproject graph and flags immediately after Open, then Save status, output tree/bytes, DOE side files, and Close/reset effects separately.
- The converter's existing DoeOutput save paths remain explicitly qualified/unsupported for active DOE/session data (`analysis/resume46-doe-output-next.md`). Therefore this project is a valid Open/connector integration input, but may refuse at downstream DOE Save. Do not treat a Save refusal as evidence against connector admission; record the stage and exact partial filesystem effects.
- The Oracle-B project is an already saved follow-up capture, so make a new disposable copy and pair it with a fresh original session. Do not use the stored post-capture files as the fresh original run.

## Evidence consulted

- `source/docs/re/moproj2mst/runner/cases.tsv`: base cases 48, 51 and 52 and their operation inventories.
- `source/docs/re/moproj2mst/runner/cases_b.tsv`: follow-up project/capture indexing and provenance context.
- `C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b\120_B_DOE\COG_P1_B1.moproj`: two Sub_Project blocks and the exact operation-index edge above; SHA-256 recorded above.
- `source/apps/moproj2mst/src/model/classinit.cpp` and `analysis/connector-implementation-review.md`: DoeOutput/Cogging flags.
- `analysis/connector-admission-original.json`: mocked-probe scope.
- `analysis/resume46-doe-output-next.md`: current DoeOutput loading/Save limitations.

Read-only file/index search only. No project was copied or changed; no build, application, GUI control, or REA database was used.
