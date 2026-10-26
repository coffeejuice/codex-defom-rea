# Status: keyword specs, groups 1 and 3

Branch `claude/project-thread-qj8op8` (from `claude/project-thread-xp4r5r`), started 2026-10-21.

Done:
* 81/81 specs in `docs/re/keywords/` (70 group 1, 11 group 3), index `group1_3-index.md`,
  shared mechanics `_common.md`.
* Grammar checked on the corpus with `docs/re/keywords/tools/check_group13_grammar.py`: every KEY-file
  occurrence passes; results in `tools/group13_corpus_check.tsv`.

Method: headless Ghidra decompiles in my own project copy (`deform-rea-work/ghidra_kw13`, output in
`deform-rea-work/out/kw13`), disassembly for argument order where the decompiler was unclear, PE
constants read directly from the image. No DEFORM binary modified, no debugger, licence code untouched.

Open items: see the end of `docs/re/keywords/group1_3-index.md`.
