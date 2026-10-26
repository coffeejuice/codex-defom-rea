# Keyword groups 1 and 3: spec index

All 81 keywords of registry group 1 (simulation control, 70) and group 3 (inter-object, 11) have a spec.
All are reachable in batch mode: the KEY interpreter `FUN_1401b6520` (KFREAD, menu 2, `DEF_MULTI.INI`,
`DEF_PRE.INI`) reads them, and the KEY writer `FUN_1401c5810` (KFWRIT, menu 8) writes them.
Two are read-only by design: `INIGES` (read and discarded) and `DSMAX_NSTEP` (converted into DSMAX).
Shared mechanics (line helpers, the table object, error codes, writer order, pair list): [_common.md](_common.md).

Corpus check: `python -I tools/check_group13_grammar.py docs/re/keyword_registry.tsv <corpus>/key` simulates
each keyword's read grammar on every corpus file and confirms it stops exactly at the next keyword,
comment or EOF. Every occurrence in a real KEY file passes; the only misses are 14 `TMAX` lines inside
GUI project files (`.d2mo`) whose next token is `KWD_FOR_MST_END`. Full table: [tools/group13_corpus_check.tsv](tools/group13_corpus_check.tsv).

| Keyword | Id | Group | Read | Written | Corpus files | Notes |
|---|---:|---|---|---|---:|---|
| [TITLE](TITLE.md) | 22 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [CURSIM](CURSIM.md) | 23 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 507 |  |
| [OPRNAM](OPRNAM.md) | 24 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 127 |  |
| [SIMNAM](SIMNAM.md) | 25 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 507 |  |
| [STYPE](STYPE.md) | 26 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [SMODE](SMODE.md) | 27 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 |  |
| [GEOTYP](GEOTYP.md) | 28 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 356 | 2D only |
| [MESHNO](MESHNO.md) | 29 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 359 |  |
| [UNIT](UNIT.md) | 30 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 | default not traced |
| [TNOW](TNOW.md) | 31 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 427 |  |
| [TRANS](TRANS.md) | 32 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 487 |  |
| [HTMTHD](HTMTHD.md) | 33 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [STNOUT](STNOUT.md) | 34 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 544 |  |
| [ELMNOD](ELMNOD.md) | 35 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 533 |  |
| [NSTART](NSTART.md) | 36 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 343 |  |
| [STPINC](STPINC.md) | 37 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 485 |  |
| [PDIE](PDIE.md) | 38 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 486 |  |
| [OSCTRL](OSCTRL.md) | 39 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 305 | 2D only |
| [NSTEP](NSTEP.md) | 40 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 493 |  |
| [EMAX](EMAX.md) | 41 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [TMAX](TMAX.md) | 42 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 494 |  |
| [SMAX](SMAX.md) | 43 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 |  |
| [VMIN](VMIN.md) | 44 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [LMAX](LMAX.md) | 45 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 |  |
| [STPDEF](STPDEF.md) | 46 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [DEMAX](DEMAX.md) | 47 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 323 |  |
| [DSMAX](DSMAX.md) | 48 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 157 |  |
| [DSMAX_NSTEP](DSMAX_NSTEP.md) | 49 | 1 | `FUN_14039aa90` | – | 0 | never written |
| [DTMAX](DTMAX.md) | 50 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 337 |  |
| [DTTYPE](DTTYPE.md) | 51 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 443 |  |
| [SLDERR](SLDERR.md) | 52 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [DTPMAX](DTPMAX.md) | 53 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 |  |
| [DVMAX](DVMAX.md) | 54 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [MDSOBJ](MDSOBJ.md) | 55 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 494 |  |
| [CNTERR](CNTERR.md) | 56 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 305 | 2D only |
| [VFACTR](VFACTR.md) | 57 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 357 | 3D list form |
| [DTSUB](DTSUB.md) | 58 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 414 |  |
| [DPLEN](DPLEN.md) | 59 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 177 | 3D only |
| [ODMAX](ODMAX.md) | 60 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 168 | 3D only |
| [CVGERR](CVGERR.md) | 61 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 |  |
| [INIGES](INIGES.md) | 62 | 1 | `FUN_14039aa90` | – | 0 | never written |
| [ITRMTH](ITRMTH.md) | 63 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [ITRMXD](ITRMXD.md) | 64 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [DEFBWD](DEFBWD.md) | 65 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [SOLMTT](SOLMTT.md) | 66 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 485 |  |
| [ITRMXT](ITRMXT.md) | 67 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [TMPBWD](TMPBWD.md) | 68 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [LATENT](LATENT.md) | 69 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [SOLMTD](SOLMTD.md) | 70 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 485 | S4 message reads ITRMXD bytes |
| [GEOERR](GEOERR.md) | 71 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [PENINF](PENINF.md) | 72 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [UNTE2H](UNTE2H.md) | 73 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [TINTGF](TINTGF.md) | 74 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [ENVTMP](ENVTMP.md) | 75 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 484 |  |
| [FRNNAM](FRNNAM.md) | 76 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 0 | written with ENVTMP 10 |
| [BLZMAN](BLZMAN.md) | 77 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [CNVCOF](CNVCOF.md) | 78 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [ENVATM](ENVATM.md) | 79 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [ACVCOF](ACVCOF.md) | 80 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [ENVMPR](ENVMPR.md) | 81 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [ENVMPT](ENVMPT.md) | 82 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [EHRATE](EHRATE.md) | 83 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 482 |  |
| [TIMEID](TIMEID.md) | 84 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 0 |  |
| [ERECID](ERECID.md) | 85 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 0 |  |
| [NRECID](NRECID.md) | 86 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 0 |  |
| [USRDEF](USRDEF.md) | 87 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 56 |  |
| [UENAME](UENAME.md) | 88 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 442 |  |
| [UNNAME](UNNAME.md) | 89 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 442 |  |
| [CVNAME](CVNAME.md) | 90 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 0 |  |
| [FRQNCY](FRQNCY.md) | 91 | 1 | `FUN_14039aa90` | `FUN_1403d4990` | 91 |  |
| [CNTACT](CNTACT.md) | 271 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 470 |  |
| [FRCFAC](FRCFAC.md) | 272 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 376 |  |
| [IHTCOF](IHTCOF.md) | 273 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 378 |  |
| [SEPRES](SEPRES.md) | 274 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 374 |  |
| [INTRST](INTRST.md) | 275 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 374 |  |
| [FRCROT](FRCROT.md) | 276 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 247 | written in 2D only |
| [SEPDEN](SEPDEN.md) | 277 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 247 | 2D only |
| [FRCWIN](FRCWIN.md) | 278 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 6 |  |
| [CNTMTH](CNTMTH.md) | 279 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 127 | written in 3D only |
| [WMODEL](WMODEL.md) | 280 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 16 |  |
| [RGDCNT](RGDCNT.md) | 281 | 3 | `FUN_140388fb0` | `FUN_1403c8560` | 0 |  |

## Open items

* **UNIT default**: the constructor chain traced does not set `S+0xfc`; find its first writer
  (`FUN_1401f40b0` callers) before relying on the unit-dependent defaults.
* **DSMAX_NSTEP**: meaning of the object field `+0x22e0` (owned by the group 2 object specs).
* **Mirror copy**: `FUN_1401f9d50` / `FUN_1401f9e10` (friction and IHTCOF copy) were not decompiled in
  detail; the port should copy the whole friction/IHTCOF blocks.
* **FRCWIN writer** forms for window shapes 3 and 11–13 are only summarised; they cannot come from a KEY
  file (the reader rejects those shapes) but can come from a DB.
* `FUN_1401d0fc0` (called by GEOTYP, STNOUT, ELMNOD, UENAME, UNNAME) belongs to the model skeleton.
* No corpus file uses CVNAME, DSMAX_NSTEP, ERECID, FRNNAM, INIGES, NRECID, RGDCNT or TIMEID; their
  grammars come from code only.
