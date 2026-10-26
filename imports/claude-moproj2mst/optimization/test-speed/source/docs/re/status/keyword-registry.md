# Status: keyword registry (thread kd6glp)

Branch `claude/project-thread-kd6glp` (from `claude/project-thread-xp4r5r`), worktree
`claude-deform-rea-kwreg`, Ghidra copy `deform-rea-work/ghidra_kwreg`. Decompiles in
`deform-rea-work/out/kwreg_*.c`. Per-function status: [`keyword-registry.tsv`](keyword-registry.tsv)
(ledger statuses; `ported` = in `defmodel` with tier-1 tests).

- [x] table read from the binary by `port/defmodel/scripts/gen_keyword_registry.py` (PE file, read only)
- [x] constexpr table `port/defmodel/include/defmodel/keyword_registry_table.hpp`, regenerated
      `docs/re/keyword_registry.tsv` (adds `table_index`)
- [x] lookup `FUN_1402f76c0` and the 8 other accessors that do not call handlers
- [x] unknown-keyword branch of both readers (warning text, SRFNRG / leading-digit exceptions)
- [x] tier-1 tests: `ctest -L defmodel` (every name, id, group, flag, handler slot, table position;
      unknown and edge cases; committed table == binary when DEFORM is installed)
- [ ] handler dispatch `FUN_1402f79c0` / `FUN_1402f7a70` and the three default handlers (Phases 2 and 10)

## What the binary does

* Table at `0x140544400` in `.rdata`, 304 records of 0x30 bytes, followed directly by the string
  "ERROR: KEYWORD NOT EXISTS!". Record: `+0` int id, `+8` name, `+0x10` group, `+0x14` flag,
  `+0x18/+0x20/+0x28` handler slots.
* The table is **not in id order** (position 21 is CRDSYS, id 282; the last record is DIM, id 288).
  Ids are a permutation of 0..303. All searches scan in table order.
* Name lookup (`FUN_1402f78d0`) is a plain `strcmp`: case-sensitive, no trimming, no prefix match.
  The reader feeds it the `fscanf("%39s")` token unchanged. Unknown → id `0x131` (305), group 0.
* `FUN_1402f7790` checks `0 < i < 304` and returns the id of the record at **position** `i`,
  so position 0 (PROBID) is rejected. Nothing calls it.
* `FUN_1402f7720` (517 call sites, mostly the KEY writer) `strcpy`s the name into one 40-byte
  static buffer and returns it; the port keeps that buffer.
* Unknown keyword in `FUN_1401b6520`: `fgets` the rest of the line (1024 bytes), then unless the
  name is exactly `SRFNRG`, `fprintf(stdout, "WARNING: Keyword %s not found. Skip %s\n", name, rest)`.
  The rest still has its `'\n'`, so the warning is followed by a blank line. If that `fgets` hits
  EOF the buffer keeps the previous line's text (the reader port must keep that).
  `FUN_1403987e0`, the other reader, prints the same warning but is silent for names starting with
  `1` or `2` and has no SRFNRG exception.
* Groups: 1 simulation control (70), 2 object (137), 3 inter-object (11), 4 material (42),
  5 phase transformation (10), 6 command (33), 7 dimension (1, DIM).
* Flag `+0x14`: values 0 (199), 7 (44), 5 (29), 8 (27), 6 (3), 15 (BCCDEF), -1 (LOCTMP). Its only
  reader is `FUN_1402f77e0`, which nothing calls, so its meaning was not recovered and does not
  affect batch output.
* Handler slot 1 is `FUN_1402f7590` everywhere. Slot 2: 155 KEY write handlers besides the default.
  Slot 3: 49 label handlers besides the default, and **ZSTR has a null slot 3** (the earlier
  extract showed `0x100000000`, and plan §3's "50" counted it).
* No unit or dimension table is referenced from the registry records; units are handled inside
  the per-keyword read/write handlers.

## Corrections to earlier data

* `docs/re/keyword_registry.tsv` ZSTR `handler3` was `0x100000000`; it is `0x0`.
  `docs/re/batch_scope.py` now skips a null slot. The ledger never had a row for the bogus value.
* Every other cell of the earlier TSV matched the binary when compared by id.
