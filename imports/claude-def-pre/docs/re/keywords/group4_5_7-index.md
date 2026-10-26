# Keyword groups 4, 5, 7: index

Specs for every keyword of registry groups 4 (material, 42), 5 (phase transformation, 10) and
7 (dimension, 1) of `DEF_PRE_64.exe` v11.0.2, recovered from the code. Shared machinery
(line helpers, DYNARY tables, header writers, material and phase-pair objects, KEY writer order)
is in [_common_g457.md](_common_g457.md); each spec refers to it.

**Registry columns:** handler2 is the KEY **write** handler; handler3 is a data **accessor**
used by the edit menus; handler1 is a no-op. The **read** code is the group parser's `switch`
(cases below), not a registry column.

**Status:** all 53 keywords have a spec; none is unreachable in batch mode (KEY reading reaches
all of them through `FUN_1401b6520`). Five are read but never written by KFWRIT: LTHEAT, GRNMDL
(value discarded), TMPSOL, TMPLIQ (stored only) and DIM (routing switch).

## Verification

* **Read, whole corpus:** [tools/kwcheck457.py](tools/kwcheck457.py) replays the three group
  parsers. Every group 4/5 occurrence in the 942 corpus KEY files (`DEFORM_corpus/key`,
  `/materials`) consumes exactly its lines (column "corpus"); the per-spec workers ran their own
  independent simulators with the same result.
* **Write, original program:** 28 oracle cases ([tools/oracle_cases457.py](tools/oracle_cases457.py))
  make the original DEF_PRE read a corpus KEY and write it back with KFWRIT: the 8 library
  material KEYs (IN718_6um recrystallization, JIS-S45C phases, AISI-1020 machining,
  AISI4340-JohnsonCook) and 20 example KEYs covering 87 keyword/header variants
  ([tools/kw457_variants.json](tools/kw457_variants.json)). Every write grammar in the specs
  reproduces those outputs byte for byte, CR LF and three-digit exponents included. Results are in
  the shared oracle cache (`%USERPROFILE%\.deform-oracle`).
* **Header variants present in the corpus:** [tools/kwvariants457.py](tools/kwvariants457.py).

## Keywords

| Keyword | id | grp | Read | Write | Accessor | Corpus |
|---|---|---|---|---|---|---|
| [MTNAME](MTNAME.md) | 92 | 4 | case 0x0 of FUN_140384a50 | inline FUN_1403cdca0 | – | 740 in 440 files |
| [FRAE2H](FRAE2H.md) | 93 | 4 | case 0x1 of FUN_140384a50 | FUN_1403cf3f0 | – | 740 in 440 files |
| [FSTRES](FSTRES.md) | 94 | 4 | case 0x2 of FUN_140384a50 | FUN_1403d0c90 | FUN_1403d1c50 | 740 in 440 files |
| [YOUNG](YOUNG.md) | 95 | 4 | case 0x3 of FUN_140384a50 | FUN_1403cf050 | FUN_1403cf0f0 | 740 in 440 files |
| [POISON](POISON.md) | 96 | 4 | case 0x4 of FUN_140384a50 | FUN_1403cf180 | FUN_1403cf220 | 740 in 440 files |
| [EXPAND](EXPAND.md) | 97 | 4 | case 0x5 of FUN_140384a50 | FUN_1403cf2b0 | FUN_1403cf360 | 740 in 440 files |
| [THRCND](THRCND.md) | 98 | 4 | case 0x6 of FUN_140384a50 | FUN_1403cf490 | FUN_1403cf530 | 740 in 440 files |
| [HEATCP](HEATCP.md) | 99 | 4 | case 0x7 of FUN_140384a50 | FUN_1403cf5c0 | FUN_1403cf670 | 740 in 440 files |
| [EMSVTY](EMSVTY.md) | 100 | 4 | case 0x8 of FUN_140384a50 | FUN_1403cf700 | FUN_1403cf7a0 | 740 in 440 files |
| [LTHEAT](LTHEAT.md) | 101 | 4 | case 0x9 of FUN_140384a50 | not written | – | 7 in 2 files |
| [TMPSOL](TMPSOL.md) | 102 | 4 | case 0xa of FUN_140384a50 | not written | – | 7 in 2 files |
| [TMPLIQ](TMPLIQ.md) | 103 | 4 | case 0xb of FUN_140384a50 | not written | – | 7 in 2 files |
| [FPERV](FPERV.md) | 104 | 4 | case 0xc of FUN_140384a50 | FUN_1403cf830 | FUN_1403cfbf0 | 740 in 440 files |
| [FRCMOD](FRCMOD.md) | 105 | 4 | case 0xd of FUN_140384a50 | FUN_1403d3820 | – | 739 in 439 files |
| [CREEP](CREEP.md) | 106 | 4 | case 0xe of FUN_140384a50 | FUN_1403d22a0 | FUN_1403d2a00 | 733 in 438 files |
| [DIFCOE](DIFCOE.md) | 107 | 4 | case 0xf of FUN_140384a50 | FUN_1403cfca0 | FUN_1403cff00 | 740 in 440 files |
| [GRNDAT](GRNDAT.md) | 108 | 4 | case 0x10 of FUN_140384a50 | FUN_1403d3a90 | – | 6 in 6 files |
| [GRNMDL](GRNMDL.md) | 109 | 4 | case 0x11 of FUN_140384a50 | not written | – | 3 in 1 files |
| [BURGRS](BURGRS.md) | 110 | 4 | case 0x12 of FUN_140384a50 | FUN_1403d2c20 | FUN_1403d2f20 | 676 in 398 files |
| [ALPHA](ALPHA.md) | 111 | 4 | case 0x13 of FUN_140384a50 | FUN_1403d2fb0 | FUN_1403d31f0 | 676 in 398 files |
| [NDISFM](NDISFM.md) | 112 | 4 | case 0x14 of FUN_140384a50 | FUN_1403d3280 | FUN_1403d34c0 | 676 in 398 files |
| [RECVRY](RECVRY.md) | 113 | 4 | case 0x15 of FUN_140384a50 | FUN_1403d3550 | FUN_1403d3790 | 585 in 338 files |
| [HDNPHA](HDNPHA.md) | 114 | 4 | case 0x16 of FUN_140384a50 | FUN_1403cffe0 | FUN_1403d01f0 | 740 in 440 files |
| [ELRST](ELRST.md) | 115 | 4 | case 0x17 of FUN_140384a50 | FUN_1403d0280 | FUN_1403d0320 | 740 in 440 files |
| [MSTMTR](MSTMTR.md) | 116 | 4 | case 0x18 of FUN_140384a50 | inline FUN_1403cdca0 | – | 740 in 440 files |
| [HDNTIM](HDNTIM.md) | 117 | 4 | case 0x19 of FUN_140384a50 | FUN_1403d03b0 | FUN_1403d0480 | 43 in 39 files |
| [JOMINY](JOMINY.md) | 118 | 4 | case 0x1a of FUN_140384a50 | FUN_1403d0510 | FUN_1403d05f0 | 285 in 163 files |
| [UTSDAT](UTSDAT.md) | 119 | 4 | case 0x1b of FUN_140384a50 | FUN_1403d0680 | FUN_1403d0720 | 740 in 440 files |
| [HDNRUL](HDNRUL.md) | 120 | 4 | case 0x1c of FUN_140384a50 | inline FUN_1403cdca0 | – | 740 in 440 files |
| [PMEAB](PMEAB.md) | 121 | 4 | case 0x1d of FUN_140384a50 | FUN_1403d07b0 | FUN_1403d0850 | 740 in 440 files |
| [PMITT](PMITT.md) | 122 | 4 | case 0x1e of FUN_140384a50 | FUN_1403d08e0 | FUN_1403d0980 | 740 in 440 files |
| [ANISO](ANISO.md) | 123 | 4 | case 0x1f of FUN_140384a50 | FUN_1403d0a10 | FUN_1403d0c00 | none |
| [MATDEN](MATDEN.md) | 124 | 4 | case 0x20 of FUN_140384a50 | inline FUN_1403cdca0 (3D only) | – | 242 in 174 files |
| [MASDEN](MASDEN.md) | 125 | 4 | case 0x21 of FUN_140384a50 | FUN_1403d1eb0 | FUN_1403d1f50 | 172 in 100 files |
| [COARSE](COARSE.md) | 126 | 4 | case 0x22 of FUN_140384a50 | FUN_1403d1fe0 | FUN_1403d20e0 | 158 in 92 files |
| [SIZEMD](SIZEMD.md) | 127 | 4 | case 0x23 of FUN_140384a50 | inline FUN_1403cdca0 | – | 158 in 92 files |
| [TXTURE](TXTURE.md) | 128 | 4 | case 0x24 of FUN_140384a50 | inline FUN_1403cdca0 | – | 151 in 87 files |
| [DIFBND](DIFBND.md) | 129 | 4 | case 0x25 of FUN_140384a50 | FUN_1403d2170 | FUN_1403d2210 | 151 in 87 files |
| [SINTER](SINTER.md) | 130 | 4 | case 0x26 of FUN_140384a50 | FUN_1403d45f0 | – | 10 in 10 files |
| [GBENGY](GBENGY.md) | 131 | 4 | case 0x27 of FUN_140384a50 | FUN_1403d4400 | – | 151 in 87 files |
| [GBMOBI](GBMOBI.md) | 132 | 4 | case 0x28 of FUN_140384a50 | FUN_1403d44a0 | – | 151 in 87 files |
| [NUCSIZ](NUCSIZ.md) | 133 | 4 | case 0x29 of FUN_140384a50 | FUN_1403d4550 | – | 151 in 87 files |
| [PHASTF](PHASTF.md) | 134 | 5 | case 0 of FUN_140383120 | inline FUN_1403cbf00 | – | 168 in 24 files |
| [TTTD](TTTD.md) | 135 | 5 | case 1 of FUN_140383120 | FUN_1403ccd00 | FUN_1403cdb20 | 168 in 24 files |
| [TTTN](TTTN.md) | 136 | 5 | case 2 of FUN_140383120 | inline FUN_1403cbf00 (fixed text) | – | 168 in 24 files |
| [PHASLH](PHASLH.md) | 137 | 5 | case 3 of FUN_140383120 | FUN_1403cc4a0 | FUN_1403cc550 | 168 in 24 files |
| [PHASVL](PHASVL.md) | 138 | 5 | case 4 of FUN_140383120 | FUN_1403cc5e0 | FUN_1403cc690 | 168 in 24 files |
| [TRNSFP](TRNSFP.md) | 139 | 5 | case 5 of FUN_140383120 | FUN_1403cc720 | FUN_1403cc970 | 168 in 24 files |
| [PHEQVF](PHEQVF.md) | 140 | 5 | case 6 of FUN_140383120 | FUN_1403cca00 | FUN_1403ccad0 | 8 in 8 files |
| [COARSN](COARSN.md) | 141 | 5 | case 7 of FUN_140383120 | FUN_1403ccb60 | – | none |
| [IPSNRG](IPSNRG.md) | 142 | 5 | case 8 of FUN_140383120 | FUN_1403ccc50 | – | 160 in 22 files |
| [VARSEL](VARSEL.md) | 143 | 5 | case 9 of FUN_140383120 | inline FUN_1403cbf00 | – | 34 in 4 files |
| [DIM](DIM.md) | 288 | 7 | FUN_14037e530 | not written | – | none |

## Findings a port must reproduce (details in the specs)

* Errors 2/3 print a message and the interpreter **keeps reading**; it does not resynchronise.
* Read/write asymmetries in the original: generic tables of header t 5 are read but written
  with no header line; FSTRES type 0 aborts the KEY write with 3; ANISO table variable 4 aborts it
  with 3; GRNDAT type 4 is always written as t 5 and negative types are dropped; TTTD types 10-13
  with one value write a line the reader never consumes; FSTRES 13/15 and CREEP 7/8 write
  `%15.7E`; PMEAB forces 2-D tables to variable 20; HEATCP writes var 8 as t 3; DIFBND keeps
  ncomp 2; TTTN's name is never written (fixed "Transformation from a -> b" text).
* Several DYNARYs are not re-initialised on read (BURGRS, ALPHA, NDISFM, RECVRY, TRNSFP, DIFBND,
  FRCMOD tables, TTTD 10-13): earlier values leak into later keywords for the same object.
* `%i` is base-detecting (leading 0 = octal); LTHEAT reads its real value with `%i`.
* No unit conversion happens in any group 4/5/7 read or write path.

## Not in the corpus (verified from code only)

ANISO, COARSN; FSTRES types 0, 5, 7-10, 12-16, negative; FRCMOD 11, 12, 17; CREEP 1-5, 7, 8;
GRNDAT 2, 4, 5, negative; TTTD 4, 7-13, negative; FPERV tables.

## Physical models (inferred, see FSTRES.md, GRNDAT.md, TTTD.md)

The binary names few models. AISI4340-JohnsonCook and AISI-1020(Machining) both use FSTRES
type 2 (3-D table in strain, strain rate, temperature); the 4340 table fits Johnson-Cook exactly,
the 1020 table fits an Oxley-type law only approximately. IN718_6um uses GRNDAT type 1 (nine
recrystallization / grain-growth DYNARYs). JIS-S45C uses TTTD types 1 (TTT table), 5 and 6.
