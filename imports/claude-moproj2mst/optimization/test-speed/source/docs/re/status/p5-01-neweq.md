# p5-01: NEWEQ.MSG (main 14001ce7b / 14001d1bf)

From the listing (deform-rea-work/out/p501/neweq_asm.txt):

* Message unit (14001ceae..14001cebd): DAT_14077a7bc = |p| (NEG + CMOVS), DAT_14077a7c0 = 0;
  p == 6 returns at once; a unit of 0 becomes 6 (14001d024). 14001d1bf repeats the |p| and 0 -> 6
  rule without the 7c0 write. Single owner: the named views def::cmd::ctc3d::dat_14077a7bc /
  dat_14077a7c0 in p67-7's statics image (<defcmd/ctc3d/common.hpp>, `@data` tags there); the
  start-up/exit here and every engine reader use those names (no __has_include guard: def_pre
  links defcmd). Unset (0), fio would write the engine's unit records to a stray fort.0, which
  is what branches without this store (claude/p67-gate at 4dccf9b) show in 55 GENCTC/OBJPOS
  e2e cases.
* Order in main 14001c9f0: 140224d80 (program info), 1401b8e10 (environment), **14001ce7b(-1)**,
  the licence stubs (not ported), the -m parse, the session.
* 14001ce7b(-1): unit = |-1| = 1 (the store at 0x14077a7bc); not 6, so for a negative argument
  (branch 0x14001d030): `OPEN(UNIT=1, FILE='NEWEQ.MSG', STATUS='UNKNOWN')` (1404b5054, name
  str 140bad0a0, 9 chars, status 7 chars) then `CLOSE(1, STATUS='DELETE')` (1404b53a4, "DELETE"
  6 chars): an old NEWEQ.MSG is removed. Then (0x14001ced6) `OPEN(1, 'NEWEQ.MSG', 'UNKNOWN')`
  again and `REWIND(1)` (1404b5a10): an empty file in the working directory, formatted
  sequential (no other OPEN specifiers are set).
* Contents: the records Fortran routines write to unit 1 (`WRITE(1, ...)`); the shell itself
  writes none. In the port the Fortran runtime defrt/ftn/fio.hpp (p67-7) appends unit-1 records
  to this file (CR LF record ends), with the file closed between statements.
* 14001d1bf(-1) at the end of a menu session: `CLOSE(1, STATUS='DELETE')`. So the file is gone
  after every menu session (E / Y, the 101-selection limit, EOF) and stays after `exit(0)`
  (DEF_MULTI.INI, -m utilities) or an abnormal end.

Check: `harness/shell/shell_cases.py` compares NEWEQ.MSG presence and bytes of every case, on top
of defcmp, and that no stray fort.* file differs (`--tier2` adds the tier-2 corpus cases). Result on claude/p5-01-follow: SHELL 12/12,
MENU9 3/3, FILE_INPUT 5/5 without the DEF_CVT3 launcher, tier 2: every case that runs to the end
matches (absent after menu sessions, 0 bytes after DEF_MULTI.INI and -m).
