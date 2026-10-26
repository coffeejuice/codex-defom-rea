# p5-01: command dispatcher interface (FUN_140378e00)

Branch `claude/p5-01`. Header `port/defcmd/include/defcmd/dispatch.hpp`, sources
`port/defcmd/src/dispatch/{dispatch,hooks,key_io}.cpp`, test `port/defcmd/tests/test_dispatch.cpp`.

## Entry

```cpp
namespace def::cmd {
// @src 140378e00  group 6 parser; m = 1401d81b0(model) of the KEY loop, f after the keyword name
int dispatch(model::DB_STEP_DATA* m, std::FILE* f, int id);
class ModelKeyHooks : public key::KeyReaderHooks;   // model side of 1401b6520 (groups 1-7)
}
```

`ModelKeyHooks(m)` gives `def::key::read_key_stream` the binary's routing: groups 1-6 on
`keyword_target_model(m)` (1401d81b0; group 6 = `dispatch`), group 7 (DIM) on `m`,
`has_sub_model` = `companion_model(m) != nullptr` (1401d8120), dimensions through vtable slot 0.
Whoever ports the KEY file loader 1401b5f00 (p8-07) constructs it and calls `read_key_stream`.

## Case bodies of other packets

Every case is `int cmd_<name>(def::model::DB_STEP_DATA* m, std::FILE* f)`: it starts on the rest
of the keyword line, reads its own arguments and returns **what the dispatcher returns** for the
keyword (0, 2, or another code such as WRTOBJ's 1). The dispatcher passes the value through
unchanged, so a case that ends with `break` in the binary must itself return
`status == 0 ? 0 : 2` (14037e1fd).

`hooks.cpp` binds each hook to the owner's function when the header exists (`__has_include`),
otherwise to a body that stops with `DEF_NOT_PORTED(<handler address>)`:

| Header | Functions | Ids (case) | Owner |
|---|---|---|---|
| `defcmd/contact_cmds.hpp` | `cmd_inictc`, `cmd_genctc` | 5, 6 | p67-6 |
| `defcmd/cmd_objpos.hpp` | `cmd_objpos` | 7 | p67-1 |
| `defcmd/cmd_p67_4.hpp` | `cmd_wrtobj`, `cmd_svscal`, `cmd_cpyobj`, `cmd_doescl` | 9, 12, 13, 14 | p67-4 |
| `defcmd/p67_5.hpp` | `cmd_tcycle`, `cmd_genaxs`, `cmd_gensub`, `cmd_genbcc` (take `DB_STEP_DATA&`; adapted) | 18, 19, 20, 295 (0x11f+8) | p67-5 |
| `defcmd/cmd_p8_01.hpp` | `cmd_execmd`, `cmd_genint`, `cmd_implub`, `cmd_impeqp`, `cmd_armctl`, `cmd_morp`, `cmd_gfread` | 11, 15, 16, 17, 21, 297, 298 | p8-01 |
| `defcmd/cmd_p8_05.hpp` | `cmd_brdext` | 292 | p8-05 |
| `defcmd/cmd_p8_07.hpp` | `cmd_defamg` | 293 | p8-07 |
| `defcmd/cmd_p8_08.hpp` | `cmd_remesh`, `cmd_defint`, `cmd_deffrz` | 291, 294, 296 | p8-08 |
| `defcmd/cmd_p9_01.hpp` | `cmd_cnvt3d` | 287 (0x11f+0) | p9-01 |

A header must declare all functions of its row (the check is per header). The p8 readers
(p8-02/03/04/09 `cmd_p8_0x.hpp`) are not dispatcher cases; menu 2 file input binds them separately.

Callees of the cases ported here, same default rule:

| Hook | Binary | Default |
|---|---|---|
| `read_keyword_file(m, name, &reset, &flags)` | 1401b5f00 | `defcmd/key_file.hpp` `def::cmd::read_keyword_file` (p8-07) |
| `db_read_step(m, db, step, a, b)` | 1401cc2a0 DBREAD core | DEF_NOT_PORTED (Phase 2, no owner yet) |
| `gendb_write(m, name, R8, R9, [rsp+20])` | 1401bb550 | `defdbio/gendb/gendb.hpp` `write_db` (p4-01) |
| `object_surface_update(obj, flag)` | 1402059a0 | returns for an object without surface (140160d90), else DEF_NOT_PORTED |
| `write_object_short(m, f, obj, 0)` | 1402a1df0 | `defkey/kw_g2geo.hpp` `def::key::g2geo::write_object_short` (p5-shortkey); no caller of 1401c5810 in DEF_PRE_64 sets flag bit 31 |

Ported here: PROBID (0), RUNTYP (1), DBREAD (2) frame, KFREAD (3) frame, GENDB (4) frame, TLOC (8,
1401a99a0), KFWRIT (10, 1401c5810/1401c5af0/1401c5d20), DSTROKE (154), ACTOUT (286), the
post-read step 1401d7f40 (1401d6250, 1401d63c0), DB step lookup 1401e3b60/1401e3d40, the default
(return 2) for every id without a case. Tests may replace hooks through `dispatch_hooks()` and
restore them with `reset_dispatch_hooks()`.
