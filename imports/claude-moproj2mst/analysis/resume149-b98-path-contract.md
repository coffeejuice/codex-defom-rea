# CP149 B98 path contract: no source correction supported

## Decision

Do not change the candidate Diestress path writer or the folder comparator. The B98 raw `Gear_blank.MST` difference is fully explained by the different project roots used for the retained original save and disposable candidate save. Both writers derive the absolute `KFREAD` key path from the project-owned subproject DB path and replace `.DB` with `.KEY`. A source change to emit the historical Oracle-B root would be wrong for a moved project and would contradict the original writer's path contract.

The remaining qualification is deliberately narrow: fresh raw same-root original/candidate equality has not been observed. That requires an original Save on a disposable copy at the same exact project path later used for the candidate, preserving both raw files. Current GUI release is still unverified, so that original runtime comparison is blocked. This is not a source defect to patch.

## Byte and root evidence

The independent CP149 raw audit (`analysis/resume149-b98-gear-raw-diff.md`, SHA-256 `ead343390c8619fef5c3e6622fbe03aa8270cea6194b58ebfd4548961ab48215`) compared immutable retained files:

- Original `Gear_blank.MST`: 2,756 bytes, SHA-256 `e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1`.
- CP136 raw candidate: 2,722 bytes, SHA-256 `e5845ff130256e919f4193ccc73b2196f00f1de3b74c8beb0ab6ba3f9c4aae12`.
- Both contain KFREAD at byte offset 1325; its path value begins at byte 1338.
- Original value (80 bytes): `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`.
- Candidate value (46 bytes): `C:/PROBLEM/b98r/r136-527631d9/p/Gear_blank.KEY`.

The 34-byte file-length difference equals the path-value length difference. Substituting only the candidate path value in a separate diagnostic vector produces the exact retained original bytes and SHA-256. Raw candidate and original remain different; the historical comparator correctly reports one differing MST, no missing/extra paths and no adjustment.

Retained Oracle-B setup scripts build and open case 98 under `C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b\98_B_DS3F`; `run.py` opens the project from that case directory and saves the MST there. The process CWD being `_tools` is not the path source. The candidate acceptance run used its copied project under `C:\PROBLEM\b98r\r136-527631d9\p`. Both paths therefore reflect their opened project location.

## Original and candidate writer trace

The retained original source contract (`analysis/resume116-b98-path-source.md`, SHA-256 `fe2bb6de606cb2b5c6b083b2b1a562b9ce65e0131756242dbae68bafb32edad1`) binds original Diestress `writeMst` at `0x1402b5410`. B98's Subproject is `Type="0"` with `DbName="Gear_blank.DB"`. The writer resolves the active subproject's full DB path through `0x1400b9e80`; the Type-0 branch reaches `0x1400b9df0` then `0x1400a4f60`, which applies `.DB` to the full opened-project path. The writer replaces `.DB` with `.KEY` and passes that string to `0x1401344a0`, which emits KFREAD marker, count 1 and path. It does not substitute an Oracle-B root.

Current candidate sources implement the same data flow:

- `src/ops/diestress.cpp` (SHA-256 `9dd8a7164209b3bb2019e7ffdce1d677064bc6bad3c41bf970c70da570d2e3f1`) uses `opSubProjectDb`, which resolves the active operation subproject and calls `subProjectDbPath`; `writeMst` then applies case-insensitive `.DB` → `.KEY` and emits it with `emitKfreadOne`.
- `src/model/paths.cpp` (SHA-256 `b033d8c11b31bd98fedf7d77d3c1bd0a71b85dff3a00aea408cddb507a441168`) returns the full `Project::path` with `.DB` for the normal non-Type-4 branch. The candidate has no B98-specific root.
- `src/mst/emit_a.cpp` (SHA-256 `a115ad345dc5f65e00f1f356693e562ac91a0c170f778ea1c0195cc3f25061ed`) provides the common KFREAD emission path; the Diestress writer has its source-qualified dedicated emitter.

This is the correct fresh-project behavior: if the project is opened from another directory, the generated absolute key path follows that directory. Rewriting it to the old historical root, making it relative, or adding a case-specific path substitution would regress moved projects and would not follow the original source chain.

## Smallest next action and limits

No implementation change is warranted. Preserve the raw one-file DIFF plus the existing separately named path-qualified comparison. If the acceptance objective requires a fresh raw parity witness, the smallest valid evidence action is a matched pair on one disposable short project path: restore the identical pre-save project tree at that exact path, capture original Save output, restore the identical pre-save tree again at the same path, capture candidate Save output, then compare raw bytes and retain both. Do not run that workflow until a supported app-control release operation and verifiable receipt are available; current `analysis/resume146-gui-release-capability.md` says release is unverified. No path normalization or production change should stand in for the missing paired witness.

CP147 remains the latest full validation (source hash `be223aec2c01736c4a5994ad4c85e06a1128edc1d84584a61446a9cfc3e88363`): historical 93 SAME / 5 DIFF. This report does not clear the raw B98 DIFF or claim fresh original parity.

No source, fixture, build, GUI, original executable, or mutable REA state was changed or run for this audit.
