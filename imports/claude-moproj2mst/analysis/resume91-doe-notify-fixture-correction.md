# CP91 notifier fixture correction

The failed notifier test used a fresh mode0 header-only DoeVariable while asserting unsupported and no variables. CP91 intentionally supports exactly that profile. Its scalar/B121 test name and purpose require a Keyword path whose native derived manager appends keyword/operations and then reaches SetOprHasDoe delivery. This was a stale fixture, not a production defect.

Only `source/apps/moproj2mst/tests/doe/test_variable_notify_binding.cpp` changed. In `set_opr_has_doe_session_hook_leaves_public_b121_variable_guard`, the row now contains a TLOC/unit0/double Keyword with parameter-1, nominal0.1 and KwOperation id1. Existing unsupported, vars.empty and no-notifier-attempt assertions remain. The current public reader still refuses that matched payload before append, while native scalar delivery remains independently guarded; the test does not claim delivery or scalar parsing completion. Source qualification is CP50's retained0x14090ee20/0x1400d25a0 evidence, not similarity to a header row.

Static quoted include resolution, Testkit main/supported assertions and whitespace checks passed. No C assert or REQUIRE_EQ exists. No compiler/build/CTest, production edit, GUI or mutable database action was performed. Root owns rerun target `m2m_doe_variable_notify_binding` / CTest `m2m.doe.variable_notify_binding`, then the full source-stable launcher.

Corrected test SHA-256: `B9EDD61573836D17E856670A21EB1E87ABC5F58012ECA37C595DF419E4F21F82`. Source released and held unchanged through root validation/archive.

The initial failed full-run receipt `build/logs/resume91-full-run.stdout.json` remains untouched, SHA-256 `6529FF98077354170A52ADF45FBD1AFA7D38E53738FE887D241631D19C811EF7`. It is retained as failed-run evidence; this correction does not rewrite it or claim the rerun has passed.
