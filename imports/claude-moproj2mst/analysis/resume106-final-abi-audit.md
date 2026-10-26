# CP106 final ABI and historical acceptance audit

## Frozen source/build binding

CP106 build `analysis/resume106-remesh-seam-grouped-build.json` records a
source-stable 3,421-file tree, SHA-256
`7356d8030a31b574c9d5806449ffa1539ccb5216e2a7f0522887e344a341c99a`.
Its verified source archive is
`build/wave3-checkpoint106-verified/source.zip`, SHA-256
`7d96bf1d552ffff6a6876fe20073f75c2934bd10d5dddfff19239698808bfc16`.
The archived source fingerprint matches the build source hash. Candidate DLL
SHA-256 is
`d8235c3663d094143b0c8ab097871583ac035a644f021807ad737f2ec34c48ce`.

The frozen CP105 comparison DLL is
`build/wave3-checkpoint105-verified/moproj2mst.dll`, SHA-256
`0a321757927724bd22992a713741898f31603db1361fc2c1f198d2d04cc418ae`.
Independent PE export-directory parsing finds all six names, hints, ordinals,
and RVAs identical:

```text
ordinal hint RVA      name
      1    0 00003360 m2m_api_version
      2    1 00003370 m2m_convert_utf8
      3    2 000038D0 m2m_convert_w
      4    3 00003C50 m2m_set_codepage
      5    4 00003C80 m2m_set_deform_dir_w
      6    5 00003DF0 m2m_set_gui_unit
```

I independently ran `m2m.g0.abi`, `m2m.g0.abi_open_failure`, and
`m2m.g0.abi_original_settings`; all three passed (CTest exit 0).

## Full-run binding and isolated remesh seam

CP106 run `build/test-runs/1792907293801283500/receipt.json` records 288
tests: 281 ordinary passes, six with logged gaps, one with logged differences,
and zero skipped, failed, or incomplete-output tests. Source hashes before and
after match CP106. The candidate DLL before/after hash matches the frozen
artifact. Fixture manifests are unchanged: both snapshots hash to
`ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`, and
`fixture_changes` is empty.

The seven-case `m2m.mdt.simcontrol300_remesh_terminal_contract` target passed.
This is writer-contract coverage, not terminal execution. The **CP106 archive**
contains the private remesh writer/service seam and its contract test, but its
`core/convert.cpp` calls only public `saveRemeshAtProjectSave`, which delegates
with a null service. Its archived `ConversionCallServices` has no remesh
service field; there is no `RemeshTerminalCallService` bridge or terminal
worker consumer. Therefore CP106 does not activate the admitted service path,
expose a terminal result through public convert/C ABI, or test an owned child
exit. The newer live-tree core bridge is a later source state and is not part
of this CP106 archive/build claim.

## Historical 93/5 and raw differences

CP106's `m2m.s.save_folder` sweep repeats the 98-case result: 93 matches and
five case-level DIFFs, with no refusals. The fresh-resave corpus has 48/49
identical and one file difference; Oracle B has 45/49 identical (three
older-window view profiles) and 29 file differences. Raw case records:

- `A:RS_55_Multiple_Operations_Example1`: one differing
  `operations/task00002/simctrl.key`.
- `B:120_B_DOE`: `cog_p1_b1.moproj` differs; seven DOE Control outputs are
  missing.
- `B:121_B_DOE_VAR`: the same project difference and seven DOE Control files
  are missing, plus `temp.kwd` (eight missing).
- `B:122_B_DOE_GEO`: the same project difference and ten missing files: the
  seven DOE Control files plus lower, nominal, and upper geometry `.geo`
  outputs. The run log elides the final two paths; the retained full inventory
  is `analysis/resume48-diff-b122.md`.
- `B:98_B_DS3F`: one differing `gear_blank.mst`.

The DOE after-states include GUI-created study/variable/geometry state; these
are not simple Open→Save mismatches. B98's comparator difference is associated
with the legacy `KFREAD` workspace path, and A55's current sweep does not
retain candidate bytes for a new byte-level diagnosis.

The six logged-gap categories remain separate from the five DIFF cases:
optional PACK header input, missing Save L6 oracle, five class-registration
cases in `props_l2`, P07 corpus omissions, and `.moproj` inputs absent before
GUI editing, plus the corresponding recorded gap events. Zero top-level CTest
skips does not turn those gaps into ordinary coverage.

No source changes, build, GUI, or original execution were performed for this
audit. The ABI tests were run against the already-frozen candidate only.
