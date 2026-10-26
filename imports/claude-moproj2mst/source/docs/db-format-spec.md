# DEFORM .DB file format (v11.0.2 reader)

Status: working specification, recovered by static analysis of the database module
(`datarm23.f`, rev 1.81) linked into `DEF_PRE_64.exe` of DEFORM v11.0.2, for the purpose of
reading one's own simulation results in other software. Licence-checking code was not
analysed. The two example databases were used only to test the rules below after they had
been read from the code.

Notation: words and records are 1-based, as in the original Fortran. "int" is a 32-bit
two's-complement integer, "real" an IEEE-754 double. Evidence is given as function entry
addresses in `DEF_PRE_64.exe` (image base 0x140000000).
**[fact]** = read directly in code, **[tested]** = also confirmed on the example files,
**[inferred]** = interpretation not yet proven.

The C++ implementation of everything in this document is in [`deformdb/`](../deformdb).

---

## 1. Container

| Item | Rule | Evidence |
|---|---|---|
| Access | Fortran unformatted direct access, record length 512 bytes | `FUN_140407e81` opens with `DIRECT`, `UNFORMATTED`, `RECL=0x200` **[fact]** |
| Record capacity | 128 int, 128 char*4, or 64 real words per record | constants at `0x140638020` = 128, 128, 64 **[fact]** |
| Multi-record items | An item of n words occupies consecutive records starting at its first record; the next item always starts on a new record | block reader `FUN_14044b62a` (int), `FUN_14044c2c2` (real), `FUN_14044bc76` (char) **[fact]** |
| Byte order | Big-endian. The reader always swaps on this little-endian build: ints with a 4-byte reverse (`FUN_14044461c`), reals with an 8-byte reverse (`FUN_1404449af`) | `FUN_1403b41d9`: both branches set the swap flag to 2 **[fact][tested]** |
| Reals | Reals are read as `2n` int words and then 8-byte swapped | `FUN_1403b6679` **[fact]** |
| Text inside real arrays | 8 characters per real word; because the writer swapped the whole word, the text is recovered by reversing the 8 bytes | **[inferred]** from the swap rule |
| Size limit | Record numbers are int32, so a file can address 2^31 × 512 B = 1 TiB | **[fact]** |

## 2. Record 1: signature

20 characters at the start of record 1 (`FUN_1403b1e45`, `FUN_1403b1ff5`, `FUN_1403b26e5`) **[fact]**:

| Signature | Family code | Meaning |
|---|---|---|
| `SFTC-DEFORM-2D-DB450`, `SFTC-DEFORM-2D-DB720` | 2 | legacy 2D database |
| `SFTC-DEFORM-3D-DB230` | 3 | legacy 3D database |
| `SFTC-DEFORM-23-DB100`, `SFTC-DEF-2D-DB100-00`, `SFTC-DEF-3D-DB100-00` | 23 | combined 2D/3D database (v10 and later). Each step carries its own dimension and version |

## 3. Record 2: database header

`FUN_1403b41d9` reads 34 int words from record 2 **[fact]**. Words 1–28 are text and are not
swapped; words 29–34 are ints and are swapped.

Field names are those of DEFORM's header printer (`FUN_1403d84b0`) and debug dump (`FUN_1401e0a30`);
the writer is `FUN_1401bffb0` → `FUN_1403b400b`.

| Bytes | Words | Name | Content | Status |
|---|---|---|---|---|
| 0–79 | 1–20 | TITLE | problem title (80 chars, blank padded) | **[fact][tested]** |
| 80–87 | 21–22 | SMI1 | version text, written as `%5.2f` of version/1000, e.g. `11.00` (see §3.1) | **[fact][tested]** |
| 88–95 | 23–24 | SMI2 | release date of the writing program | **[fact]** |
| 96–103 | 25–26 | SMI3 | date the simulation was first written | **[fact]**, `MM/DD/YY` order **[inferred]** |
| 104–111 | 27–28 | SMI4 | date of the last write | **[fact]** |
| 112–115 | 29 | ISTART | directory entry last written by the pre-processor (where the solver starts) | **[inferred]** |
| 116–119 | 30 | ISMTYP | simulation type (keyword `STYPE`, forced to 1 for 3D); the reader also uses the raw value (0..3) as a byte-order sanity check | **[fact]** |
| 120–123 | 31 | NRSETS | number of directory entries; every directory search runs over entries 1..NRSETS | **[fact][tested]** |
| 124–127 | 32 | NXTREC | next free record (17 in a new file) | **[fact][tested]** |
| 128–131 | 33 | IPTYPE | always written as 1 | **[fact]**, meaning unknown |
| 132–135 | 34 | — | always 0, never read | **[fact]** |

Derived values printed by DEFORM (`FUN_1403d96fc`): NUMRST = entries with MARSize ≠ 0, NUMRMS =
entries with a negative step number. The pre-processor stores its own steps as −(NSTART+1)
(`FUN_1401bb550`) **[fact]**.

### 3.1 Version number

`FUN_1403b32b5` maps the version text to an int **[fact]**:

* family 23: `10.00` → 10000, `11.00` → 11000, … `20.00` → 20000; anything else → 10000;
* family 3: `2.3` → 2300, `2.4`/`3.0` → 3000, `3.1.1` → 3100, `3.2` → 3200, `3.3` → 3300, `4.0` → 4000,
  `5.0` → 5000, `5.1` → 5100, `6.0` → 6000, `6.01` → 6010, else 6100;
* family 2: `5.0`/`5.0 CNV` → 5000, `5.1`/`5.1 CNV` → 5100, `5.2` → 5200, `5.3` → 5300, `6.0` → 6000,
  `7.0` → 7000, `7.10` → 7100, `7.20` → 7200, `8.00` → 8000, `8.10` → 8100, `8.20` → 8200,
  `9.00` → 9000, `9.01` → 9010, else 9100.

For family 23 the per-step version (directory word 9) is what selects the step layout.
For families 2 and 3 the dimension is the family code and the version is the global one
(`FUN_1403b6679`) **[fact]**.

## 4. Step directory

### 4.1 Index chain

| Item | Rule | Evidence |
|---|---|---|
| First index record | record 3 | `FUN_1403b600f` **[fact][tested]** |
| Index record | words 1–127 = first record of directory chunks 1..127 (of this index record), word 128 = record of the next index record | `FUN_1403b600f`, `FUN_1403b92bc` **[fact]** |
| Chunk | 128 entries × 13 words = 1664 words = 13 records | `FUN_1403b92bc` reads 0x680 words **[fact]** |
| Entry e | chunk `(e-1)/128 + 1`, position `(e-1) mod 128 + 1` | `FUN_1403b92bc` **[fact][tested]** with 790 entries |
| Coverage | one index record covers 127 × 128 = 16 256 entries; DEFORM keeps up to 200 index records in memory (3 251 200 entries) | **[fact]** |

### 4.2 Entry layout (13 int words)

Field names come from DEFORM's step printer (`FUN_1403b4aec`: "StepNo MeshNo SimNo SimMode HSize
MARSize ASize DimStep VerStep") **[fact]**.

| Word | Name | Meaning |
|---|---|---|
| 1 | MeshNo | mesh number |
| 2 | StepNo | step number; negative = input/restart step written by the pre-processor or by remeshing |
| 3 | HSize | number of real words in the step header |
| 4 | MARSize | integer array size (see §5) |
| 5 | ASize | real array size (see §5) |
| 6 | SimNo | simulation / operation number |
| 7 | SimMode | simulation mode |
| 8 | DimStep | 2 or 3 (family 23 only) |
| 9 | VerStep | version of the step layout, e.g. 11000 (family 23 only) |
| 10 | — | first record of the dynamic-array descriptor, 0 = none (version ≥ 11000) (`FUN_1403bcf76`) |
| 11 | — | first record of the step header |
| 12 | — | first record of the MAR data |
| 13 | — | first record of the A data |

Lookup by step number scans entries 1..word 31 and returns the first match (`FUN_1403b6679`).
The same step number can appear twice (`25` and `-25`).

Entries with MARSize = ASize = 0 are steps saved with a header only. Their records 12–13 point at
the space the next step reused **[tested]**; asking DEFORM for their arrays returns error 202
(`FUN_1403b6679`) **[fact]**.

## 5. Step payload

`FUN_1403b5a04` dispatches on an operation code **[fact]**: 0 open (header + index),
1 list directory, 2 read the step header, 3 read the MAR and A arrays, 4 write a step,
5 read dynamic arrays. Op codes ≤ −30 read only MAR region `−(op+30)`, op codes ≥ 30 only A
region `op−30` (3D, version ≥ 6000).

### 5.1 Step header

HSize reals starting at record word 11, swapped, never compressed (`FUN_1403b6679`, op 2) **[fact]**.
Contents are in §7.

### 5.2 MAR (integer) and A (real) work arrays

Three storage variants **[fact]** (`FUN_1403b6679`):

| Step | MAR / A storage | Compression flag |
|---|---|---|
| dim 2, any version | MARSize ints at word 12, ASize reals at word 13, each one block | version > 3109 |
| dim 3, version < 6000 | same as 2D | version > 999 |
| dim 3, version ≥ 6000 | MARSize / ASize are the lengths of a **region table**; each region is stored separately | version > 999 |

Region table (ints, `FUN_1403b9d4c` for MAR, `FUN_1403baf0e` for A) **[fact][tested]**:

| Word | Content |
|---|---|
| 1 | R, number of regions |
| 2 | not used by the reader |
| 4r−1 | decoded length of region r |
| 4r | start address of region r in the full array (1-based) |
| 4r+1 | stored word count of region r (0 = region absent) |
| 4r+2 | first record of region r |

So the table is 4R + 2 words (234 = 4·58 + 2 for MAR, 506 = 4·126 + 2 for A in v11 3D steps).
Each stored region is read, swapped and RLE-decoded on its own and placed at its start address.
Region r is the r-th entry of DEFORM's address-name list for the step's dimension and version
(`IIAD1, IIAD2, IIFNC, IIOBJ, IINOD, …` for MAR, `LLFNC, LLMPR, LLTFM, …` for A; full lists in
[`docs/re/address_region_names.json`](re/address_region_names.json)). Consecutive regions are
contiguous: start(r+1) = start(r) + length(r) **[tested]**.

### 5.3 Compression

Run-length coding applied per array (whole array, or per region) **[fact]**. Decoder
`FUN_14044aeeb` (int) / `FUN_14044b3f5` (real), encoder `FUN_14044ac31` / `FUN_14044b0fa`.

Stored array s[1..n]:

* s[n] = position of the link word of the last run (0 = the array contains no runs);
* s[n−1] = decoded length N;
* the remaining words are literals and run triples (value, count, link), where link is the
  position of the previous run's link word (0 for the first run);
* the writer only makes a run from 4 or more equal consecutive values; shorter repeats stay literal;
  arrays shorter than 4 words are stored literally followed by (n, 0);
* in real arrays, count, link and N are stored as reals.

Decoding walks backwards: i = n−2, j = N; if i is the current link, write `value` `count` times
ending at j, continue with link = s[i] and i −= 3; otherwise copy s[i] to position j. Because the
output never overtakes the input, DEFORM decodes in place.

### 5.4 Dynamic arrays (version ≥ 11000)

Present when directory word 10 ≠ 0 (`FUN_1403bcf76`) **[fact]**. Neither example file uses them
(word 10 = 0 in all 822 entries) **[tested]**.

Descriptor: 20 ints at record word 10:

| Word | Content |
|---|---|
| 1 | NI, number of integer dynamic arrays (≤ 50) |
| 2 | largest integer array (recomputed on load) |
| 3 | record of the integer table |
| 4 | length of the integer table in words (written as 500 = 50 × 10) |
| 5–10 | unused |
| 11 | NR, number of real dynamic arrays (≤ 200) |
| 12 | largest real array (recomputed on load) |
| 13 | record of the real table |
| 14 | length of the real table in words (written as 2000 = 200 × 10) |
| 15–20 | unused |

(`FUN_1403bcf76`, `FUN_1403be093`, writer `FUN_1403be3d4`) **[fact]**.

Each table row is 10 ints **[fact]** (`FUN_1403bcaa6`, `FUN_1403bcda9`, `FUN_1401d2760`):

| Word | Content |
|---|---|
| 1 | decoded length (rows × components) |
| 2 | memory handle, runtime only |
| 3 | stored length (ints, or reals for the real table) |
| 4 | first record of the stored data |
| 5 | owning object number |
| 6 | DYNARY class, always 1 |
| 7 | DYNARY array type: 1 = nodal coordinates (real), 2 = element connectivity (int, 3/4/8 nodes), 3 = state variables (real) |
| 8 | components per row; rows = word 1 / word 8 |
| 9–10 | unused |

Every array is RLE-compressed (`FUN_1403be165`, `FUN_1403be25d`) **[fact]**. Slot numbers carry no
meaning: the writer hands them out object by object, in the order of each object's DYNARY list,
counting integer and real arrays separately; the meaning is in words 5–8. These arrays hold the
DYNARY keyword data (flow-net / auxiliary meshes); the main mesh and results stay in MAR/A.

---

## 6. Region addressing inside MAR and A

### 6.1 Address tables are stored in the step

The first two MAR regions describe all regions **[fact]** (`FUN_1401cd1f0` → DBMADR `FUN_1403dd950`):
I1 = MAR[2] − MAR[1], I2 = MAR[3] − MAR[2]; NADS1 = MAR[1..I1], NADS2 = MAR[I1+1..I1+I2].

For step version ≥ 11000 (2D and 3D) these are copied verbatim **[fact][tested]**:

| Region | Size | Content |
|---|---|---|
| IIAD1 | 140 ints | words 1–70: 1-based start of MAR region k (k = 1..70); words 71–140: width of MAR region k |
| IIAD2 | 280 ints | words 1–140: 1-based start of A region k (k = 1..140); words 141–280: width of A region k |

"Width" is the number of words per item (per node, element, object, …) the generator used to size
the region. Region k ends where region k+1 starts; the last slot (`IILAST`, `LLAST`) is an end marker.
A width of 0 means the reader applies its default (§8.4) **[fact][tested]**. In 3D the loader treats
region IIFLD (k = 24, 1-based) as running up to IIDST (k = 59) **[fact]**.

Region k is the k-th name of the list for the step's dimension and version in
[`address_region_names.json`](re/address_region_names.json) **[fact]** (DEFORM's own name tables
at `0x1405d7cb0` / `0x1405d6a00`). One correction for 3D v > 10000: DEFORM's table prints `LLRFL` at
A slot 48 and `LLSIZ` at slot 49, but the loader uses slot 48 for the element size field (SIZESH) and
slot 49 (sized by boundary faces) for the face radiation flux. The reader in `deformdb` swaps the two
names so that each name matches its content **[fact]** (index globals set by `FUN_1401c77c0`,
consumers `FUN_140170ed0`, `FUN_140176df0`) **[tested]** (slot 49 length = number of boundary faces).

### 6.2 Older versions

Files with step version < 11000 store shorter address tables (n starts followed by n widths) that
DEFORM remaps to the v > 10000 slot numbers with static tables **[fact]** (`FUN_1401cd1f0`):

| Dim / version | MAR table / map | A table / map |
|---|---|---|
| 3D 2300–2999 | 80 words, map `0x1405d6de0` | 116 words, map `0x1405d6f20` |
| 3D 3000–3999 | 80 words, map `0x1405d70f0` | 140 words, map `0x1405d7230` |
| 3D 4000–10999 | I1 words (≤ 2×70), map `0x1405d7460` | I2 words (≤ 2×140), map `0x1405d75a0` |
| 2D 6000–6999 | 60 words, map `0x1405d5f00` | 140 words, map `0x1405d5ff0` |
| 2D 7000–10999 | I1 words, map `0x1405d6220` | I2 words, map `0x1405d6310` |
| 3D < 2300, 2D < 6000 | rejected: "Direct loading for this version is not supported" | |

Slot maps (address part; widths follow the same pattern) **[fact]**:

* 3D 4000–10999 MAR: old 1–23 → 1–23 (IIAD1..IIEAF), old 24–35 (IIDST..IILAST) → 59–70, old 36 (IIFLD) → 24.
  A: old 1–56 → 1–56, old 57–70 (LLHGO..LLAST) → 127–140; old names 48/49 are LLCSN/LLPSN.
* 2D 7000–10999 MAR: old 1–20 → 1–20, old 21–28 (IINWN..IILAST) → 63–70, old 29/30 (IIERH/IIERF) → 21/22.
  A: old 1–46 → 1–46, old 47–58 → 48–59, old 59–69 (LLHGO..LLAST) → 130–140.
* 3D 2300–3999 MAR: old 1–17 → 1–17, old 18–29 → 59–70. 3D 3000–3999 A: old 1–51 → 1–51, 52 → 55, 53 → 56,
  54–67 → 127–140. 3D 2300–2999 A: 1,2 → 1,2; 3–8 → 4–9; 9–38 → 12–41; 39 → 51; 40 → 55; 41 → 56; 42–55 → 127–140.
* 2D 6000–6999 MAR: old 1–14 → 1–14, 15–22 → 63–70. A: old 1–46 → 1–46, 47–52 → 48–53, 53 → 59, 54–64 → 130–140.
  DEFORM's own map has a defect here: width entry 111 lands on slot 40 (LLET1 start).

`deformdb` currently decodes these older steps to raw MAR/A arrays only; the typed view (§8) needs
version ≥ 11000. Neither example file contains an older step.

### 6.3 Region sizes (generator side)

Not needed for reading (the starts are stored), but useful for validation. Generator
`FUN_1401c1980` (3D) / `FUN_1401c2d10` (2D); counts come from the IISIM region (§7.2). Main 3D v > 10000
regions (length in words) **[fact]**:

| MAR region | Length | A region | Length |
|---|---|---|---|
| IIAD1 / IIAD2 | 140 / 280 | LLFNC | function data length |
| IIFNC | 2 + 2·NFN | LLMPR | 500 · NMAT |
| IIOBJ | 50 · NOBJ | LLSIM / LLCPU | 100 / 50 |
| IINOD | Σ elements · nodes per element | LLOBJ | 100 · NOBJ |
| IIMTR | NUMEL | LLBND | 200 · objects |
| IISIM | 100 | LLCRZ | 3 · NUMNP |
| IIBCD, IIBFD | 3 · NUMNP + 4 · NOBJ | LLDRZ, LLPFN, LLCFN | 3 · NPX + 4 · NOBJ |
| IIBCT | 3 · NUMNP | LLURZ | 4 · NPX + 4 · NOBJ |
| IIBFT | NUMNP | LLTMP, LLPHT, LLHIT, LLHGN, LLDTM | NUMNP |
| IIBDY | 4 · NBF | LLFLX | 3 · NUMNP |
| IIBEL, IIEDE..IIEAF, IIERH, IIERF | NBF | LLSTS | (NUMEL or NUMEL+NUMNP) · 6 · points |
| IIUVL | 500 | LLTEP, LLDMG | (NUMEL or NUMEL+NUMNP) · width · points |
| | | LLEPS, LLYLS | 6 · NUMEL |
| | | LLRDT, LLFTS, LLFEP, LLFLW | NUMEL |

NPX = NUMNP + number of objects; NBF = boundary faces. 2D uses 20 ints per object in IIOBJ, 4 nodes
per element and 2 (or 3) components per vector.

## 7. Step header contents

### 7.1 Layout

HSize = F + S · NOBJ: a global part of F reals, then one block of S reals per object **[fact]**
(`FUN_1401e1de0`, writer `FUN_1401e2180`). This build writes F = 100, S = 400. For version ≥ 11000 the
reader takes F = H[13] and S = H[10]; older steps use 2D: F = 34, S = 100; 3D < 3000: F = 8, S = 100;
3D ≥ 3000: F = 34, S = 200 **[fact][tested]** (COG step 25: 100 + 7 · 400 = 2900).

Text in the header is ASCII, 8 characters per real word **[fact]**; after the 8-byte swap applied to
all header words the characters are in the host-order bytes of each real **[tested]**.

### 7.2 Global part (1-based real index)

| H | Meaning | Status |
|---|---|---|
| 1 | number of objects | **[fact][tested]** |
| 2 | primary die (keyword `PDIE`) | **[fact]** |
| 3, 4 | step start / end time (both written from `TNOW`) | **[fact]** |
| 5 | simulation number (`CURSIM` value 1) | **[fact]** |
| 6 | 3D: platform code (2); 2D: unused | **[fact][tested]** |
| 7 | total MAR length | **[fact][tested]** |
| 8 | total A length | **[fact][tested]** |
| 9 | 2D: platform code; 3D: fold-mesh flag (model `+0xcc` on write, `+0x1734` on read, gates FLDGEO/FLWNET; see [re/dbmap/stepdriver.md](re/dbmap/stepdriver.md)) | **[fact]** |
| 10 | per-object stride S | **[fact][tested]** |
| 11 | operation number (`CURSIM` value 2) | **[fact][tested]** |
| 12 | process type (`CURSIM` value 3) | **[fact]** |
| 13 | global part length F | **[fact][tested]** |
| 17–24 | operation name (`OPRNAM`, 64 chars) | **[fact]** |
| 25–34 | simulation name (`SIMNAM`, 80 chars) | **[fact]** |

### 7.3 Per-object block (3D, 0-based offset inside the block)

| Offset | Content |
|---|---|
| 0–2 | load X, Y, Z (keyword `FORCE`) |
| 3–5 | stroke X, Y, Z |
| 6–8 | velocity X, Y, Z |
| 9, 10, 11 | torque, angle, angular velocity |
| 12–23 | min/max pairs: velocity, temperature, relative density, effective strain, effective strain rate, effective stress |
| 24 | volume |
| 25–30 | bounding box (X min/max, Y min/max, Z min/max) |
| 31–38 | min/max: displacement, damage, mean stress, principal stress |
| 39–50 | min/max of the 6 stress components |
| 51–64 | principal strain min/max, 6 strain components min/max |
| 65–78 | principal strain-rate min/max, 6 strain-rate components min/max |
| 79–86 | min/max of user node variables 1–2 and user element variables 1–2 |
| 88, 89 | number of nodes, number of elements |
| 98–105 | min/max: dominant atom, hardness, cooling time, voltage |
| 106–113 | object name (64 chars) |
| 114–124 | heat flux, surface area, power, furnace energy terms |

The 2D block (S = 100 in older files) has the same items in a different order: load 0–1, stroke 2–3,
velocity 4–5, min/max pairs 6–19, volume 20, rotation 21–23, user variables 24–31, damage 32–33,
name 34–41, node/element count 42/43, then atom/hardness/cooling/voltage, energy, mean/principal stress
and the component min/max blocks **[fact]** (`FUN_1401c7f80`, labels at `0x1405d81a0` / `0x1405d8c00`).
When consecutive steps belong to the same simulation the writer starts from the previous step's
blocks, so slots it does not recompute carry over **[fact]**.

## 8. Objects and keyword data (version ≥ 11000)

### 8.1 Simulation control: IISIM (ints) and LLSIM (reals), 0-based

| Item | 3D | 2D | Status |
|---|---|---|---|
| MESHNO | IISIM[1] | IISIM[3] | **[fact][tested]** |
| NSTART (step number) | IISIM[2] | IISIM[4] | **[fact][tested]** |
| STPINC | IISIM[5] | — | **[fact]** |
| PDIE | IISIM[7] | — | **[fact]** |
| UNIT | IISIM[18] | IISIM[25] | **[fact]** |
| NUMNP (all objects) | IISIM[19] | IISIM[26] | **[fact][tested]** |
| NUMEL (all objects) | IISIM[20] | IISIM[27] | **[fact][tested]** |
| number of materials | IISIM[22] | IISIM[29] | **[fact]** |
| number of objects | IISIM[23] | IISIM[30] | **[fact]** |
| ELMNOD options 1–10 | IISIM[57..66] | IISIM[59..68] | **[fact]** |
| TNOW: global time, local time, function time mode, second-stage time | LLSIM[0], [50], (int)[51], [56] | LLSIM[0], [42], (int)[43], [39] | **[fact][tested]** (global time) |

ELMNOD options: 1 damage, 2 strain, 3 stress (0 = per element, 2 = per element and per node,
3 = per integration point), 4 density, 5 grain (0 element, 1 node).

### 8.2 Object records

Object k (0-based) has an int record in IIOBJ and a real record in LLOBJ, each of the region's width
(50 ints / 100 reals in 3D) **[fact][tested]** (`FUN_1401d2230`, `FUN_14016caf0`):

| IIOBJ word (0-based) | Meaning |
|---|---|
| 0 | 0 = empty slot, 1 = object with mesh, 2 = object without mesh data |
| 1 | deformability: 0 rigid, 1 plastic, 2 elastic, 3 elasto-plastic, 4 user (keyword `OBJTYP` maps 0→1, 1→2, 2→4, 3→5, 4→7) |
| 2 | 1 = porous (overrides word 1) |
| 4, 5 | first and last global node number |
| 6, 7 | first and last global element number |
| 8, 9 | first and last boundary record (LLBND) |
| 10 | 2D only: flag |
| 12 | 3D: nodes per element, 4 = tetrahedron, 8 = brick |
| 13 | 3D: 1-based offset of the object's connectivity inside IINOD |

LLOBJ reals 4–11 hold the object name (`OBJNAM`, 64 chars) **[fact][tested]**. Other LLOBJ reals hold
movement-control data that is not mapped yet.

### 8.3 Global numbering

Nodes and elements of all objects are numbered globally and stored back to back **[fact][tested]**.
With W = region width, a per-node value is

    A[start(R) + (firstNode − 1 + n − 1) · W + c]      n = 1..nodeCount, c = 0..W−1

and per-element values use firstElem the same way. MAR fields follow the same rule.
Connectivity holds global node numbers; local = global − firstNode + 1.

* 3D connectivity: MAR[start(IINOD) − 1 + (IIOBJ[13] − 1) + i], i < elements · nodes per element.
* 2D connectivity: MAR[start(IINOD) − 1 + (firstElem − 1) · 4 + i], i < elements · 4.
* ELMNOD option 2: the region has NUMEL element rows followed by NUMNP node rows; node g is row NUMEL + g.
* ELMNOD option 3: each element row holds `points × components` values, point-major
  (8 points for a brick, 1 for a tetrahedron, 4 in 2D).
* Width-3 exceptions: IIBCT and LLFLX switch to component-major storage when their width is 3:
  component c of global node g is at start + c · NUMNP + (g − 1) (`FUN_14016dfb0`, `FUN_140170410`).

### 8.4 Keyword map

Node fields read by `FUN_14016dfb0` (A) and `FUN_140170410` (MAR), element fields by `FUN_140170ed0`
(A) and `FUN_140174130` (MAR); KEY names from the writer `FUN_1402a4a80` **[fact]**. "Default width" is
used when the address table stores width 0. Keywords marked ✔ were compared value by value with
DEFORM's own KEY export (§10).

| Keyword | Region | Kind | Default width | Notes | |
|---|---|---|---|---|---|
| RZ | LLCRZ | node real | dim | coordinates | ✔ |
| DRZ | LLDRZ | node real | dim | displacement | ✔ |
| URZ | LLURZ | node real | dim | velocity | ✔ |
| FRZ | LLPFN | node real | dim | nodal force | ✔ |
| *FRZB | LLCFN | node real | dim | | |
| PRZ, *PRZB | LLPPR, LLCPR | node real | dim | 3D | |
| DRMESH | LLDRM | node real | dim | 3D | |
| NDTMP | LLTMP | node real | 1 | temperature (2D KEY writes the pair T−ΔT, T with LLDTM) | ✔ |
| NDHEAT, *HITN | LLPHT, LLHIT | node real | 1 | | ✔ (NDHEAT) |
| NDFLUX / RHTFLX / CRBFLX | LLFLX | node real | 1 | components 1/2/3 when width is 3 | ✔ (NDFLUX) |
| USRNOD | LLND1 | node real | from table | user node variables | ✔ |
| VOTAGE, DATOM | LLVLT, LLATM | node real | 1 | | |
| NWEAR | LLWER | node real | 5 | | |
| HTGN | LLHGN | node real | 1 | region sized NUMNP; DBREAD expects an element field of width 2, so it is never loaded, and GENDB never writes it ([objarrays](re/dbmap/objarrays.md) §14) | |
| TIMERC, NRECVL | LLTRC, LLNRV | node real | from table | v ≥ 11000 | |
| BCCDEF | IIBCD | node int | dim | BC code per DOF | ✔ |
| BCCDFN | IIBFD | node int | dim | BC function per DOF | ✔ |
| BCCTMP / BCCRHT / BCCCRB | IIBCT | node int | 1 | components 1/2/3 when width is 3 | ✔ (BCCTMP) |
| BCCTFN | IIBFT | node int | 1 | | ✔ |
| STRESS | LLSTS | element real | 2·dim · points | Sxx Syy Szz Txy Tyz Tzx (3D) | ✔ |
| *EPS | LLEPS | element real | 2·dim | strain-rate components | |
| STNCMP | LLTSR | element real | from table | strain components | ✔ |
| *EFSTS, *EFEPS | LLFTS, LLFEP | element real | 1 | effective stress, effective strain rate | |
| STRAIN | LLTEP | element real | points | effective plastic strain | ✔ |
| DENSTY | LLRDT | element real | 1 | relative density | ✔ |
| DAMAGE | LLDMG | element real | from table | | ✔ |
| MATAXI | LLMTA | element real | 9 (3D) | | |
| USRELM | LLET1 | element real | from table | user element variables | ✔ |
| VOLFC, VOLFS | LLVLF, LLVLN | element real | materials | one column per phase | |
| DVOLF, TICF | LLDVF, LLICB | element real | relations | | |
| GRAIN | LLGSZ | element (or node) real | from table | node rows when ELMNOD grain option = 1 | |
| HDNOBJ, ZEFI | LLHDN, LLZEI | element real | 2 | | |
| YLDS | LLYLS | element real | 2·dim | | |
| CURRNT | LLCUR | element real | dim | | |
| SIZESH, ZMFI, TXTODF, ERECVL | LLSIZ*, LLZMI, LLODF, LLERV | element real | from table | v ≥ 11000 (*see §6.1) | |
| ELMCON | IINOD | element int | nodes per element | see §8.3 | ✔ |
| MTLGRP | IIMTR | element int | 1 | material group | ✔ |

Element-face boundary conditions (`FUN_140176df0`, 3D v ≥ 4000, 2D v ≥ 7000) are indexed by boundary
face: ECCDEF IIEDE, ECDEFN IIEDF, ECCTMP IIETM, ECTMFN IIETF, ECCATM IIEAT, ECATFN IIEAF, ECCRHT IIERH,
ECRHFN IIERF (MAR); ECPRES LLPRE, ECHFLX LLHFL, ECAFLX LLAFL, ECRFLX LLRFL* (A). Boundary faces come
from LLBND records and 4 node numbers per face in IIBDY **[fact]**; not yet exposed by `deformdb`.

## 9. Not yet recovered

The keyword ↔ array mapping of every class is now in [re/db-model-mapping.md](re/db-model-mapping.md); the list
below is kept for history.


* ~~Where STROKE is filled from on load~~: from words 18–20 of the object's first LLBND record (movement
  block), not from the step header, which GENDB writes but DBREAD never reads ([object](re/dbmap/object.md) §4).
* Most LLOBJ movement-control reals, material data (LLMPR, LLTFM), functions (IIFNC/LLFNC),
  boundary function tables, BCCANG/BCCFNC, inter-object data.
* 3D header slot 9, a few per-object slots (87, 90–97, 125–128), IPTYPE.
* Typed view for step versions < 11000 (the remap in §6.2 is documented but not implemented).

## 10. Verification

All rules above were taken from the code first. The example databases were then used as tests:

* `deformdb` reads the header and directory of both files (32 and 790 entries) and decodes every
  step's MAR/A arrays: all 790 steps of the 5 GB file in 1.8 s with 16 threads (7.2 s single-threaded)
  on the test PC, with the file in the OS cache.
* DEFORM's documented text export (`DEF_PRE` with `DBREAD <step>` + `KFWRIT`) was run on **copies** of
  the databases as an oracle. `scripts/compare_key.py` compared 20 keywords (RZ, DRZ, URZ, FRZ, NDTMP,
  NDHEAT, NDFLUX, ELMCON, STRESS, STRAIN, DAMAGE, STNCMP, USRELM, USRNOD, DENSTY, MTLGRP, BCCDEF,
  BCCDFN, BCCTMP, BCCTFN) for every object of COG_P1_B1 step 25 and Inc718 steps 690 and −1:
  every value matched.
