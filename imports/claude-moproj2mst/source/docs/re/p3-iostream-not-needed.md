# P3 MSVC STL iostream rows: not-needed

Census follow-up for packets C04 and C06 of [census-2026-10-21.md](census-2026-10-21.md) (69 P3 rows
reached only through vtable slots) and the 49 P3 rows the census lists as "ported around: MSVC STL
iostream" under "Unreachable in batch". Result: all **118** rows are `not-needed`
([status/c04-c06.tsv](status/c04-c06.tsv)). No row is reachable through a vtable of a model class,
and none is left todo.

## Argument

1. Every row is MSVC 2008 STL iostream code (`ostream` inserters, `sentry`, `num_put<char>`,
   `numpunct<char>`, `codecvt<char,char,int>`, `ios_base` / `basic_ios` / `basic_ostream` /
   `basic_ofstream` / `basic_filebuf`, the locale facet registry). The exception is the small
   `FILE*` wrapper class that the same callers use alongside it (1401b1880 / 1401b18b0 / 1401b18f0 /
   1401b1940).
2. Every reference to a row was listed (Ghidra reference manager, code and data). A code reference
   comes either from another row or from one of the external functions in the frontier table below.
   A data reference is a slot of one of the RTTI vtables V1-V5 below (the `.pdata` entries are
   ignored). No row sits in the vtable of a non-STL class.
3. The functions that write (install) those vtables are themselves rows, or STL static
   initialisers already `not-needed` in p1-01, or library functions with no code references. So the
   only objects in DEF_PRE whose vtables reach a row are made by the frontier functions or by the
   `std::cin` / `std::cout` static initialisers.
4. Every frontier function is ported with C stdio (`fprintf`/`fputs`/`fsopen`/`fclose`, or the
   p9-03 `FILE*` emulation of `ostream << double`). The menu functions that use `std::cin` /
   `std::cout` are ported with the host C++ library and call no row. So no ported batch path creates
   an object whose vtable reaches a row.
5. The census slot chains (`KEY reader: 1401b6520 > [v]...`, `140280760 > [v]14015af80`,
   `14039a200 > [v]14015b740`, `menu: 140007d30 > [v]14015b440`, `1400078b0 > [v]1403321a0 > ...`)
   come from the over-approximation "a `call [reg+8k]` reaches slot k of every live vtable". Those
   call sites dispatch on model objects (`_DEF_VERTEX`, `SdctlGList`, `_DEF_LOOP`...) or on the
   stream of a frontier function. By point 3, no iostream object exists at those call sites in
   batch code.

Method: headless Ghidra (`deform-rea-work/scripts/Callers_c04.java`, project copy
`ghidra_c04c06`). The script lists the references to each row and to each vtable, and maps each data
reference to its RTTI vtable using `out/c04c06/vtables.txt`. Raw output is in
`deform-rea-work/out/c04c06/` (`callers*.txt`, `frefs.txt`, `rowinfo2.json`, `p3.c` decompiles).

## Vtables reaching rows and the functions that install them

| id | class (RTTI) | vtable | slot rows | functions writing the vtable (ctor/dtor) | how those are reached |
|---|---|---|---|---|---|
| V1 | `std::num_put<char,ostreambuf_iterator>` | 14051f568 | slots 0-8: 14015bed0, 14015bdf0, 14015bab0, 14015b740, 14015b640, 14015b540, 14015b440, 14015b340, 14015af80 | ctor 14015af20, dtor 14015bf10 (both rows) | 14015af20 only from 14015ad70 < 14015a850 (`use_facet<num_put>`) < 140159340 (`ostream << int`) and 1401595b0 (`ostream << double`, p9-03) |
| V2 | `std::numpunct<char>` | 14051f700 | slots 0-5: 14015df30, 14015de50, 14015de60, 14015de70, 14015deb0, 14015def0 | ctor 14015ddf0, dtor 14015df70 (rows) | 14015ddf0 < 14015dc70 < 14015db40 < V1 slots 14015af80 / 14015c080 / 14015ceb0 only |
| V3 | `std::codecvt<char,char,int>`, `std::codecvt_base` | 140525338, 140525380 | 1401b58a0, 1401b5730, 1401b5740, 1401b5750, 1401b57a0, 1401b57e0, 1401b5820, 1401b5850; 1401b5760 | ctor 1401b5660 / 1401b56c0, dtors 1401b58e0 / 1401b5700 (rows) | 1401b5660 < 1401b54e0 < 1401b5240 < 1401b3b10 (filebuf open, < 1401b1f30 < 1401b1bd0) or < `basic_filebuf::imbue` 1401b3460 (V6 slot) |
| V4 | `std::ios_base`, `std::basic_ios<char>` | 14051f2e0, 14051f2d0 | 140159820 (ios_base slot 0), 140159d70 (basic_ios slot 0) | rows 1401b1da0, 1401b1dd0, 1401597c0, 1401597f0; library 140477cc0 / 140477c6c / 140477d14 (`basic_istream` ctor/dtor, no code references); 14050489c / 1405048ec (atexit teardown of `std::cin` / `std::cout`, registered by 1405040c4 / 14050419c, `not-needed` in p1-01) | 1401b1da0 only from 1401b1bd0, 1401b1df0, 1401b3850 (rows under 1401b1bd0) |
| V5 | `SfOStream`, `std::basic_ofstream<char>`, `std::basic_ostream<char>` | 140524fb8, 140524ff8, 140525010 | 1401b1ce0, 1401b3640, 1401b35b0 (slot 0 each) | rows 1401b1bd0 / 1401b1c80 (SfOStream), 1401b1df0 / 1401b1ed0 (ofstream), 1401b3850 / 1401b1ea0 (ostream); 1405048ec (`std::cout` teardown, p1-01) | 1401b1bd0 (SfOStream ctor) is called only by 140378e00 at 14037e15c (ARMCTL, case 0x15 code 14037e0be-14037e1f4), 14013c970 at 14013c9f3 and 140148f70 at 14014b96e |
| V6 | `std::basic_filebuf<char>` (not a row itself; its `imbue` slot 1401b3460 reaches rows 1401b4030, 1401b5240) | 140525078 | - | row 1401b3ac0 (ctor, only from 1401b1df0 < 1401b1bd0); 140504078 / 140504150 (`std::cin` / `std::cout` filebufs, p1-01 `not-needed`) | see V5 and the `std::cin` / `std::cout` note |

`std::cin` (14063a030) and `std::cout` (14063a100) are built by the p1-01 `not-needed` static
initialisers. Their only code users are menu functions: 140007a80, 140008030, 140008220,
1400150f0 (p10-01, verified, `forms.cpp` / `menu3_sim_controls.cpp`); 140008a00, 140009100
(p10-02); 140007d30, 14000af30 (verified, `menu_shell.cpp`); 140008600, 140009c50, 140009e90
(`menu7.cpp`); 1400096a0 (menu, no status row yet). All of them read with `istream::getline`
(140007460, `not-needed`, ported as host `std::cin.getline`). None of them calls any row.

## Frontier: external callers of the rows

| function | role | calls rows | port status (branch, file) |
|---|---|---|---|
| 140378e00 case 0x15 (ARMCTL) | writes DEF_ARM_CTL.DAT through `SfOStream` | 1401b1880, 1401b18f0, 1401b1940, 1401b18b0, 1401b1bd0, 140159340, 140007a50, 140002830, 140158be0 | ported with `fprintf`/`fflush`/`fclose`: `cmd_armctl` in port/defcmd/src/p8_01/commands.cpp (claude/p8-01 1c5b1d22) |
| 14013c970 | CNVT3D DEF_CNV23.DAT writer | same set as ARMCTL | verified, p9-01 (port/defcmd/src/cnvt3d/log.cpp, claude/p9-01 04ece833) |
| 140148f70 | CNVT3D sweep NBCD.OUT writer | same set + 1401595b0 | verified, p9-03 (port/defcmd/src/cnvt3d_sweep, claude/p9-03 bd227fd9) |
| 1401595b0 | `ostream << double` | 140002840, 140002960, 140002b80, 140007350, 140007610, 14015a2e0, 14015a310, 14015a850 | ported, p9-03 `fun_1401595b0` in port/defcmd/src/cnvt3d_sweep/ostream.cpp (C stdio emulation with `snprintf`); only caller 140148f70 |
| 14013c340 | CNVT3D "[i/n]" status log | FILE wrapper 1401b1880 / 18b0 / 18f0 / 1940 only | verified, p9-03 |
| 14013c6b0 | CNVT3D CONV_STATUS.DAT log | FILE wrapper only | verified, p9-01 |
| 14013d7e0 | DEF_INT_3DTET.DAT writer | FILE wrapper only | ported, p9-03 |
| 1401b3460 | `basic_filebuf::imbue` (V6 slot) | 1401b4030, 1401b5240 | STL; filebuf objects only from 1401b3ac0 (row) and the p1-01 `not-needed` `std::cin`/`std::cout` filebufs |

The p9-03 ostream emulation also covers 140159050 (`ios_base::setf`), 140159090 (`precision`) and
14015a260 (`num_put::put(double)`, the real caller of the V1 slot 14015bab0 and its helpers).

## Rows

Columns: census group (C04, C06, or ported-around from "Unreachable in batch"), Ghidra name,
vtable slot or role, callers inside the row set, and the vtables (V1-V6) and frontier functions it
traces back to.

| row | census | name | class / vtable slot | callers in the set | traces back to |
|---|---|---|---|---|---|
| `1400011e0` | ported-around | FUN_1400011e0 | shared locale/iostream helper (also under num_put<char>) | 14015a850 14015db40 1401b5240 | V1, V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1400012d0` | ported-around | facet_Register | shared locale/iostream helper (also under num_put<char>) | 14015a850 14015db40 1401b5240 | V1, V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1400012e0` | C04 | FUN_1400012e0 | helper of codecvt<char,char,int>, codecvt_base, num_put<char>, numpunct<char> | 14015bf10 14015df70 1401b5700 | V1, V2, V3 |
| `1400012f0` | ported-around | FUN_1400012f0 | shared locale/iostream helper (also under num_put<char>) | 14015af20 14015ddf0 1401b56c0 | V1, V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `140001310` | ported-around | locale | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b4660 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1400013d0` | ported-around | FUN_1400013d0 | shared locale/iostream helper (also under num_put<char>) | 14015a850 14015db40 1401b5240 | V1, V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `140001540` | ported-around | FUN_140001540 | shared locale/iostream helper (also under num_put<char>) | 140159340 14015af80 14015b340 14015b440 14015b540 14015b640 14015b740 14015bab0 14015c080 14015ceb0 | V1; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140001550` | C04 | FUN_140001550 | helper of num_put<char> | 14015af80 14015c080 14015ceb0 | V1 |
| `140001560` | C04 | FUN_140001560 | helper of num_put<char> | 14015af80 14015c080 14015ceb0 | V1 |
| `140002830` | ported-around | FUN_140002830 | basic_ostream / basic_filebuf / locale code (no own vtable) | external only | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140002840` | ported-around | FUN_140002840 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140002960` | ported-around | FUN_140002960 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140002b80` | ported-around | FUN_140002b80 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140003200` | C04 | FUN_140003200 | helper of num_put<char> | 14015da30 | V1 |
| `1400033d0` | C04 | FUN_1400033d0 | helper of num_put<char> | 14015d620 | V1 |
| `1400060b0` | C04 | FUN_1400060b0 | helper of num_put<char> | 14015af80 | V1 |
| `140007350` | ported-around | sentry | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140007610` | ported-around | ~sentry | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140007a50` | ported-around | FUN_140007a50 | basic_ostream / basic_filebuf / locale code (no own vtable) | external only | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140158be0` | ported-around | FUN_140158be0 | shared locale/iostream helper (also under SfOStream) | 1401b1ce0 | V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140159340` | ported-around | FUN_140159340 | basic_ostream / basic_filebuf / locale code (no own vtable) | external only | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401597c0` | ported-around | ~basic_ios<> | shared locale/iostream helper (also under SfOStream, basic_ios<char>, basic_ofstream<char>, basic_ostream<char>) | 140158be0 140159d70 1401b36d0 1401b3700 | V4, V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401597f0` | ported-around | FID_conflict:~codecvt_base | shared locale/iostream helper (also under SfOStream, basic_ios<char>, basic_ofstream<char>, basic_ostream<char>, ios_base) | 1401597c0 140159820 | V4, V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `140159820` | C04 | FUN_140159820 | ios_base slot 0 | vtable | V4 |
| `140159d70` | C04 | FUN_140159d70 | basic_ios<char> slot 0 | vtable | V4 |
| `14015a1f0` | ported-around | FUN_14015a1f0 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015a2e0` | ported-around | FUN_14015a2e0 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015a310` | ported-around | FUN_14015a310 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015a850` | ported-around | FUN_14015a850 | basic_ostream / basic_filebuf / locale code (no own vtable) | 140159340 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015ad70` | ported-around | FUN_14015ad70 | basic_ostream / basic_filebuf / locale code (no own vtable) | 14015a850 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015af20` | ported-around | FID_conflict:collate<unsigned_short> | basic_ostream / basic_filebuf / locale code (no own vtable) | 14015ad70 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015af80` | C04 | FUN_14015af80 | num_put<char> slot 8 | vtable | V1 |
| `14015b340` | C04 | do_put | num_put<char> slot 7 | vtable | V1 |
| `14015b440` | C04 | do_put | num_put<char> slot 6 | vtable | V1 |
| `14015b540` | C04 | do_put | num_put<char> slot 5 | vtable | V1 |
| `14015b640` | C04 | do_put | num_put<char> slot 4 | vtable | V1 |
| `14015b740` | C04 | FUN_14015b740 | num_put<char> slot 3 | vtable | V1 |
| `14015baa0` | C04 | FUN_14015baa0 | helper of num_put<char> | 14015b740 14015bab0 | V1 |
| `14015bab0` | C04 | FUN_14015bab0 | num_put<char> slot 2 | vtable | V1 |
| `14015bdf0` | C04 | do_put | num_put<char> slot 1 | vtable | V1 |
| `14015bed0` | C04 | FUN_14015bed0 | num_put<char> slot 0 | vtable | V1 |
| `14015bf10` | C04 | FUN_14015bf10 | helper of num_put<char> | 14015bed0 | V1 |
| `14015bf40` | C04 | Ffmt | helper of num_put<char> | 14015b740 14015bab0 | V1 |
| `14015c080` | C04 | FUN_14015c080 | helper of num_put<char> | 14015b740 14015bab0 | V1 |
| `14015cd50` | C04 | _Ifmt | helper of num_put<char> | 14015b340 14015b440 14015b540 14015b640 | V1 |
| `14015ceb0` | C04 | FUN_14015ceb0 | helper of num_put<char> | 14015b340 14015b440 14015b540 14015b640 14015bdf0 | V1 |
| `14015d470` | C04 | _Put | helper of num_put<char> | 14015af80 | V1 |
| `14015d510` | C04 | _Rep | helper of num_put<char> | 14015af80 14015c080 14015ceb0 14015d830 | V1 |
| `14015d5a0` | C04 | FUN_14015d5a0 | helper of num_put<char> | 14015af80 | V1 |
| `14015d5e0` | C04 | FUN_14015d5e0 | helper of num_put<char> | 14015af80 | V1 |
| `14015d620` | C04 | insert | helper of num_put<char> | 14015c080 | V1 |
| `14015d780` | C04 | _Putc | helper of num_put<char> | 14015c080 14015ceb0 14015d830 | V1 |
| `14015d830` | C04 | _Putgrouped | helper of num_put<char> | 14015c080 14015ceb0 | V1 |
| `14015da30` | C04 | operator= | helper of num_put<char> | 14015d470 14015d510 14015d780 | V1 |
| `14015daa0` | C04 | FUN_14015daa0 | helper of num_put<char> | 14015d470 14015d510 14015d780 | V1 |
| `14015dab0` | C04 | FUN_14015dab0 | helper of num_put<char> | 14015d470 14015d510 14015d780 | V1 |
| `14015dac0` | C04 | FUN_14015dac0 | helper of num_put<char> | 14015c080 | V1 |
| `14015dae0` | C06 | FUN_14015dae0 | helper of num_put<char> | 14015c080 14015ceb0 | V1 |
| `14015db00` | C06 | grouping | helper of num_put<char> | 14015c080 14015ceb0 | V1 |
| `14015db40` | C06 | FUN_14015db40 | helper of num_put<char> | 14015af80 14015c080 14015ceb0 | V1 |
| `14015dc50` | C06 | _Maklocchr<char> | helper of num_put<char> | 14015c080 14015d780 14015e070 | V1 |
| `14015dc70` | C06 | FUN_14015dc70 | helper of num_put<char> | 14015db40 | V1 |
| `14015ddf0` | C06 | numpunct<char> | helper of num_put<char> | 14015dc70 | V1 |
| `14015de50` | C06 | FUN_14015de50 | numpunct<char> slot 1 | vtable | V2 |
| `14015de60` | C06 | FUN_14015de60 | numpunct<char> slot 2 | vtable | V2 |
| `14015de70` | C06 | do_grouping | numpunct<char> slot 3 | vtable | V2 |
| `14015deb0` | C06 | FUN_14015deb0 | numpunct<char> slot 4 | vtable | V2 |
| `14015def0` | C06 | FUN_14015def0 | numpunct<char> slot 5 | vtable | V2 |
| `14015df30` | C06 | FUN_14015df30 | numpunct<char> slot 0 | vtable | V2 |
| `14015df70` | C06 | FUN_14015df70 | helper of numpunct<char> | 14015df30 | V2 |
| `14015dfb0` | C06 | FUN_14015dfb0 | helper of numpunct<char> | 14015df70 | V2 |
| `14015e010` | ported-around | Init | basic_ostream / basic_filebuf / locale code (no own vtable) | 14015af20 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015e040` | ported-around | _Getcvt | shared locale/iostream helper (also under num_put<char>) | 14015e010 14015e070 | V1; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401595b0 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `14015e070` | C06 | _Init | helper of num_put<char> | 14015ddf0 | V1 |
| `14015e2f0` | C06 | _Getlconv | helper of num_put<char> | 14015e070 | V1 |
| `14015e310` | C06 | FUN_14015e310 | helper of num_put<char> | 14015e070 | V1 |
| `14015e320` | C06 | FUN_14015e320 | helper of num_put<char> | 14015e070 | V1 |
| `14015e950` | C06 | FUN_14015e950 | helper of num_put<char> | 14015e070 | V1 |
| `1401b1880` | ported-around | FUN_1401b1880 | FILE* wrapper class (no vtable) | external only | ext callers 14013c340 (p9-03), 14013c6b0 (p9-01), 14013c970 (p9-01), 14013d7e0 (p9-03), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b18b0` | ported-around | FUN_1401b18b0 | FILE* wrapper class (no vtable) | external only | ext callers 14013c340 (p9-03), 14013c6b0 (p9-01), 14013c970 (p9-01), 14013d7e0 (p9-03), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b18f0` | ported-around | FUN_1401b18f0 | FILE* wrapper class (no vtable) | external only | ext callers 14013c340 (p9-03), 14013c6b0 (p9-01), 14013c970 (p9-01), 14013d7e0 (p9-03), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1940` | ported-around | FUN_1401b1940 | FILE* wrapper class (no vtable) | external only | ext callers 14013c340 (p9-03), 14013c6b0 (p9-01), 14013c970 (p9-01), 14013d7e0 (p9-03), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1bd0` | ported-around | FUN_1401b1bd0 | basic_ostream / basic_filebuf / locale code (no own vtable) | external only | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1c80` | ported-around | FUN_1401b1c80 | shared locale/iostream helper (also under SfOStream) | 140158be0 | V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1ce0` | C06 | FUN_1401b1ce0 | SfOStream slot 0 | vtable | V5 |
| `1401b1da0` | ported-around | basic_ios<> | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1bd0 1401b1df0 1401b3850 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1dd0` | ported-around | FUN_1401b1dd0 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1da0 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1df0` | ported-around | FUN_1401b1df0 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1bd0 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1ea0` | ported-around | ~basic_ostream<> | shared locale/iostream helper (also under SfOStream, basic_ofstream<char>, basic_ostream<char>) | 1401b1ed0 1401b36d0 | V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1ed0` | ported-around | FUN_1401b1ed0 | shared locale/iostream helper (also under SfOStream, basic_ofstream<char>) | 1401b1c80 1401b3700 | V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1f30` | ported-around | FUN_1401b1f30 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1bd0 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b1fa0` | ported-around | FUN_1401b1fa0 | shared locale/iostream helper (also under SfOStream) | 1401b1c80 | V5; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b35b0` | C06 | FID_conflict:scalar_deleting_destructor' | basic_ostream<char> slot 0 | vtable | V5 |
| `1401b3640` | C06 | FUN_1401b3640 | basic_ofstream<char> slot 0 | vtable | V5 |
| `1401b36d0` | C06 | FID_conflict:vbase_destructor' | helper of basic_ostream<char> | 1401b35b0 | V5 |
| `1401b3700` | C06 | FUN_1401b3700 | helper of basic_ofstream<char> | 1401b3640 | V5 |
| `1401b3850` | ported-around | basic_ostream<char,std::char_traits<char>_> | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1df0 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b3ac0` | ported-around | basic_filebuf<char,std::char_traits<char>_> | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1df0 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b3b10` | ported-around | FUN_1401b3b10 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b1f30 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b4030` | ported-around | Initcvt | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b3b10 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1401b4080` | ported-around | always_noconv | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b4030 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1401b4660` | ported-around | FUN_1401b4660 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b3b10 | ext callers 14013c970 (p9-01), 140148f70 (p9-03), 140378e00 (p8-01 ARMCTL) |
| `1401b5240` | ported-around | FUN_1401b5240 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b3b10 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1401b54e0` | ported-around | FUN_1401b54e0 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b5240 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1401b5660` | ported-around | FID_conflict:codecvt<wchar_t,char,int> | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b54e0 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1401b56c0` | ported-around | FID_conflict:ctype_base | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b5660 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
| `1401b5700` | C06 | FID_conflict:~codecvt_base | helper of codecvt<char,char,int>, codecvt_base | 1401b5760 1401b58e0 | V3 |
| `1401b5730` | C06 | FUN_1401b5730 | codecvt<char,char,int> slot 1; codecvt_base slot 1 | vtable | V3 |
| `1401b5740` | C06 | FUN_1401b5740 | codecvt<char,char,int> slot 2; codecvt_base slot 2 | vtable | V3 |
| `1401b5750` | C06 | FUN_1401b5750 | codecvt<char,char,int> slot 3; codecvt_base slot 3 | vtable | V3 |
| `1401b5760` | C06 | FUN_1401b5760 | codecvt_base slot 0 | vtable | V3 |
| `1401b57a0` | C06 | FUN_1401b57a0 | codecvt<char,char,int> slot 4 | vtable | V3 |
| `1401b57e0` | C06 | FUN_1401b57e0 | codecvt<char,char,int> slot 5 | vtable | V3 |
| `1401b5820` | C06 | FUN_1401b5820 | codecvt<char,char,int> slot 6 | vtable | V3 |
| `1401b5850` | C06 | do_length | codecvt<char,char,int> slot 7 | vtable | V3 |
| `1401b58a0` | C06 | FUN_1401b58a0 | codecvt<char,char,int> slot 0 | vtable | V3 |
| `1401b58e0` | C06 | FID_conflict:~messages<wchar_t> | helper of codecvt<char,char,int> | 1401b58a0 | V3 |
| `1401b5910` | ported-around | FUN_1401b5910 | basic_ostream / basic_filebuf / locale code (no own vtable) | 1401b5660 | V6; ext callers 14013c970 (p9-01), 140148f70 (p9-03), 1401b3460 (V6), 140378e00 (p8-01 ARMCTL) |
