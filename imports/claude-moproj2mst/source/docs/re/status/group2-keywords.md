# Status: group 2 (object) keyword specs

Thread branch `claude/project-thread-j4c7t9`, worktree `claude-deform-rea-grp2`, Ghidra copy
`deform-rea-work/ghidra_g2`, decompiles in `deform-rea-work/out/g2/`.

* [x] parser switch map (`docs/re/keywords/group2-parser-map.md`)
* [x] shared reader helpers (`docs/re/keywords/group2-helpers.md`)
* [x] writer order (`docs/re/keywords/group2-writer-map.md`)
* [x] per-keyword specs (137 of 137, index in docs/re/keywords/group2-index.md)
* [x] corpus grammar checks (per spec; every corpus occurrence parses)

## Batches

* J (GEO23 GEOSEC MSHSEC CRDSYS POLYTP DIEPNT DIEEDG CSPMTX): done. GEO23/GEOSEC/MSHSEC/CRDSYS
  are parsed but never written in batch: their only writer `FUN_1402a9c80` is reached only from
  the uncalled exporter `FUN_1401c6920`. POLYTP/DIEPNT/DIEEDG are 3D only (2D: nothing consumed).
  DIEEDG writer bug: an edge set of unknown type is written as a `DIEPNT` line with count 0.
* G (EC* edge/face BCs, 12): done; corpus scan of all KEY files parses (0 mismatches).
* F (WINPRS WINTMP WINATM WINRSE): done; only WINTMP in corpus (18 records parse). Several
  round-trip breaks documented in WINPRS.md (gap windows, table variable codes, mode 2).
* A (OBJNAM OBJTYP AVGSTR LMTSTR ZSTR PENVOL REFTMP MOVCTL MOVANG TMPLMT): done; corpus OK.
  OBJTYP code 6 leaves the type unchanged. MOVANG is never written.
* H (MG* 12, OPSTOP, DYNARY): done; MGSIZM is never written. DYNARY has no corpus use.
* E (21 remesh/control/scalar keywords incl. OBJPHS): done; corpus OK except PRSTRE/OBJPHS (unused).
* K (STROKE SPDLMT ANGMOV CNTRAX ANGMVY ANGMO2 CNTRA2 FORCE): done; corpus OK. obj+0x22d0..0x22e0 = stroke vector X/Y/Z.
* I (GRAIN GRAIN2 SIZESH TXTODF VOTAGE DATOM VOLFS TIMERC ERECVL NRECVL YLDS ZEFI ZMFI CURRNT NWEAR): done; corpus OK. GRAIN does not round-trip a uniform nonzero array with W > 1.
* D (group2-statevar-writers.md + NDTMP NDHEAT NDFLUX STRAIN STNCMP DENSTY DAMAGE STRESS USRNOD USRELM CUSVAR VOLFC DVOLF TICF): done; corpus OK except CUSVAR (unused) and integration-point layouts (no corpus use).
* B (DIEGEO RZ DRZ URZ FRZ PRZ DRMESH ELMCON MTLGRP MATAXI SYMSUF WPAXIS TPLOGY RSEDEF FLDGEO FLWNET): done; corpus OK (FLDGEO/FLWNET unused). In 3D curve DIEGEO is written twice.
* C (BCCFNC BCCDEF BCCDFN BCCANG BCCTMP BCCTFN BCCCRB BCCRHT CRBFLX RHTFLX HDNOBJ HDNEST LOCTMP LOCATM LOCDEF): done; corpus OK. BCCANG never written; RHTFLX written in any dimension.
