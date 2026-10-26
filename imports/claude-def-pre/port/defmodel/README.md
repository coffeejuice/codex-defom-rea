# defmodel

Keyword model library of the DEF_PRE_64 batch port (`docs/port-plan-def-pre-batch.md` §4).
Holds the keyword registry and the keyword-model class layouts (`include/defmodel/layout/`,
`docs/re/model-layout.md`). Conventions: `docs/port-conventions.md`.

* `include/defmodel/keyword_registry.hpp` — lookups (`lookup_keyword` = `FUN_1402f76c0`, and the
  other accessors at `0x1402f7680`–`0x1402f7950`), all `constexpr` except `keyword_name` and the
  unknown-keyword warning helpers.
* `include/defmodel/keyword_registry_table.hpp` — the 304 records, **generated**; do not edit.
* `scripts/gen_keyword_registry.py` — reads the table from `DEF_PRE_64.exe` (path from `--exe`,
  `$DEFORM_PRE_EXE`, or the v11.0.2 install) and rewrites the header and
  `docs/re/keyword_registry.tsv`. `--check` only compares.

Built by the port superbuild (`port/build.cmd`); tests: `ctest --test-dir port/build -L defmodel`
(or `-L registry`). Status: `docs/re/status/keyword-registry.md`.
