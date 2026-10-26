# CP107 final ABI and acceptance audit

## Frozen source/build and ABI

CP107 `analysis/resume107-v2-core-grouped-build.json` records a stable
3,424-file source tree, SHA-256
`4ecb601b6924fe244240bd4ea632365ec0d62693e0c2d97dbef91590e04ea309`.
The verified CP107 source archive is
`build/wave3-checkpoint107-verified/source.zip`, SHA-256
`f8cba072111da1932409f2dd463292e22d81f7f98761bd69fdbceb09ea61ada0`.
Candidate `moproj2mst.dll` SHA-256 is
`0abe53b2ae218773b8ef8386122afee89f529d85ff59887188f83ee318b17b9a`.

Against frozen CP106 DLL
`build/wave3-checkpoint106-verified/moproj2mst.dll` (SHA-256
`d8235c3663d094143b0c8ab097871583ac035a644f021807ad737f2ec34c48ce`),
independent PE export-directory parsing confirms identical names, hints,
ordinals, and RVAs:

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

## Full-run binding and terminal-control scope

Full-run receipt `build/test-runs/1792907874115848400/receipt.json` records
290 tests: 283 ordinary passes, 6 with logged gaps, 1 with logged differences,
and zero skipped, failed, or incomplete-output tests. Source before/after
hashes both match CP107. Candidate DLL before/after hash matches the verified
artifact. Fixture manifests match at SHA-256
`ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7`, with an
empty `fixture_changes` list.

The private core bridge test and `m2m.mdt.terminal_harness_remesh_v2` both
pass. The latter covers the RemeshV2 wire codec, selected decoder, sticky
failure classification, bounded preparation lease, read-sharing/write-denial
control, malformed records, and synthetic boundary/stall protocol cases. Its
explicit v2 child role does **not** call conversion, `RemeshTerminalForwarder`,
or the remesh writer. Those controls must not be presented as an observed
remesh failure or candidate process-terminal result.

The CP107 archive has an optional `ConversionCallServices::remeshTerminal`
pointer and a `convert.cpp` callsite that forwards live `SaveCallState` when a
private caller supplies it. The only concrete `RemeshTerminalCallService`
implementation in the archived source is the test `Spy`. Public conversion
entrypoints and the product C ABI pass null; the v2 harness does not supply a
service. Thus CP107 includes private forwarding plumbing and synthetic
protocol controls, but **no actual conversion worker/product route** that
selects RemeshV2, invokes the native writer's admitted failed-open dispatch,
or observes a terminal child status. Fresh-default handler prelude and
end-to-end worker execution remain unverified.

## Historical 93/5 and raw differences

CP107's `m2m.s.save_folder` sweep retains 98 cases: 93 matches and five DIFF
cases, with no refusals. The fresh-resave corpus has 48/49 identical with one
file difference; Oracle B has 45/49 identical (three older-window view
profiles) and 29 file differences. The five case-level records are:

- `A:RS_55_Multiple_Operations_Example1`: one differing
  `operations/task00002/simctrl.key`.
- `B:120_B_DOE`: `cog_p1_b1.moproj` differs; seven DOE Control files are
  missing.
- `B:121_B_DOE_VAR`: the same project difference and seven DOE Control files
  are missing, plus `temp.kwd` (eight missing).
- `B:122_B_DOE_GEO`: the same project difference and ten missing files: the
  seven DOE Control files plus lower, nominal, and upper geometry `.geo`
  outputs. The log abbreviates the last two names; full paths remain in
  `analysis/resume48-diff-b122.md`.
- `B:98_B_DS3F`: one differing `gear_blank.mst`.

The DOE after-states include GUI-created study/variable/geometry state, not a
simple Open→Save baseline. B98's comparator difference is associated with an
old-workspace `KFREAD` path. A55's sweep identifies the file path but does not
retain candidate bytes for a fresh byte-level diagnosis.

The six logged-gap categories remain explicit: optional PACK header input,
unavailable Save L6 oracle, five `props_l2` class-registration cases, P07
corpus omissions, and `.moproj` inputs lacking a pre-GUI-edit source, plus the
associated logged events. A zero skipped count does not convert these gaps
into ordinary coverage.

No source edits, build, GUI, or original-runtime execution were performed for
this audit. The only executions were the three focused public ABI tests.
