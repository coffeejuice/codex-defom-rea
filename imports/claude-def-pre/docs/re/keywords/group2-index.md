# Group 2 (object) keywords: index

All 137 registry keywords with `group_code == 2` (ids 144–303) have a spec in this folder,
recovered from `DEF_PRE_64.exe` v11.0.2 by static analysis. Corpus KEY files
(`DEFORM_corpus`, read-only) were used only to check each grammar afterwards; every corpus
occurrence of every keyword parses with its spec's grammar.

Start with:

* [group2-helpers.md](group2-helpers.md): prologue (object number, get-or-create object),
  return codes, all shared line readers, node/element/edge readers, lists, defaults.
* [group2-parser-map.md](group2-parser-map.md): `FUN_14038b8a0` switch case → keyword.
* [group2-writer-map.md](group2-writer-map.md): what `FUN_14029e250` writes for one object, in
  order, with conditions.
* [group2-statevar-writers.md](group2-statevar-writers.md): the state-variable writer
  `FUN_1402a4a80` (all codes), its line writers and the component-count table.
* `_common.md` (groups 1+3 thread, branch `claude/project-thread-qj8op8`): table object,
  table reader/writer, KEY loop error handling.

Specs that hold shared detail for a family: MOVCTL.md (movement), WINPRS.md (window BCs and
`LLWINDEF_DATA`), ECCDEF.md (edge/face BCs), MGNELM.md (mesh-generation block order),
DIEGEO.md (geometry storage and writers), DRZ.md (`FUN_1402f5c10` output), BCCDEF.md (nodal BC
int arrays), LOCTMP.md (local BC lists, table writer mode 1).

## Not reachable in batch KEY output

* **OBJPHS** (204): no parser case (`default: return 2`) and no writer.
* **MOVANG**, **BCCANG**, **MGSIZM**: parsed (or skipped) but never written.
* **GEO23**, **GEOSEC**, **MSHSEC**, **CRDSYS**: parsed; their only writer is reached from an
  exporter with no callers.

## Round-trip defects found (KEY → KEY is not identity)

Specs give the detail; a byte-identical port must reproduce them:
OBJTYP code 6 keeps the old type; DIEEDG unknown-type sets are written as `DIEPNT` lines;
BCCDEF 3D negative triples are rewritten; GRAIN uniform non-zero arrays with W > 1; CUSVAR
uniform non-zero arrays; SPDLMT drops `a` without a table; REFPOS mode 3; TRGVOL clears the
volume when the mode is negative (the writer changes the model); WINPRS/WINATM/WINRSE gap
windows; WINTMP cleanup removes shapeless windows during the write; LOC* fill gaps with zero
entries; MGUSER window types 12/13 written as 11; OTPRNG mode 3 and VOLCRG invalid classes;
3D curve DIEGEO written twice; values are rounded to `%20.10E` (three-digit exponent).

## Keywords

| Id | Keyword | Case | Corpus files | KEY output | Summary |
|---:|---|---|---:|---|---|
| 144 | [OBJNAM](OBJNAM.md) | 0x00 | 468 | written | Object name: a text line stored in the object at `obj+0x828` (81-byte buffer; cleared by the |
| 145 | [OBJTYP](OBJTYP.md) | 0x01 | 550 | written | Object type code (rigid / plastic / elastic / porous / ... / empty) plus a second integer. |
| 146 | [AVGSTR](AVGSTR.md) | 0x02 | 454 | written | One double per object at `obj+0x1ea8` (object property sub-structure at `obj+0x1ea0`, defaults |
| 147 | [LMTSTR](LMTSTR.md) | 0x03 | 454 | written | One double per object at `obj+0x1ea0` (first field of the property sub-structure at |
| 148 | [ZSTR](ZSTR.md) | 0x04 | 247 | written | 2D-only object setting: an integer flag and a double at `obj+0x1f30` / `obj+0x1f28` (property |
| 149 | [PENVOL](PENVOL.md) | 0x05 | 454 | written | Object penalty constant (double at `obj+0x1eb0`, default 1.0E+6 — the typical volume-constancy |
| 150 | [REFTMP](REFTMP.md) | 0x06 | 440 | written | One double per object at `obj+0x1eb8`, the object's reference temperature (default 20 or 68 |
| 151 | [MOVCTL](MOVCTL.md) | 0x07 | 608 | written | Movement control of an object: movement type (force, speed, hammer, screw press, mechanical |
| 152 | [MOVANG](MOVANG.md) | 0x08 | 0 | never written | Obsolete keyword: it is parsed but its value is thrown away, and nothing writes it. Its name |
| 153 | [STROKE](STROKE.md) | 0x09 | 475 | written | The object's stroke vector: its accumulated displacement, one double per coordinate. It is |
| 155 | [SPDLMT](SPDLMT.md) | 0x0b | 2 | written | The speed-limit table of a hydraulic press movement. Its table object is at `obj+0x2a20` |
| 156 | [ANGMOV](ANGMOV.md) | 0x0c | 608 | written | The angular movement control of the object: a method and a constant or table value. |
| 157 | [CNTRAX](CNTRAX.md) | 0x0d | 608 | written | The centre (axis) of the object's rotation for ANGMOV. |
| 158 | [DIEGEO](DIEGEO.md) | 0x0e | 364 | written | The object's boundary geometry. It has three record forms: |
| 159 | [RZ](RZ.md) | 0x0f | 423 | written | Node coordinates of the object's FE mesh: (r, z) in 2D, (x, y, z) in 3D. The node count it |
| 160 | [DRZ](DRZ.md) | 0x10 | 406 | written | Nodal displacement vector, one per node, in `obj+0x8b0`. Its component count is the model's |
| 161 | [BCCFNC](BCCFNC.md) | 0x11 | 20 | written | A numbered 1-D function (table) attached to the object: a list of tables at `obj+0x2e68`, each |
| 162 | [BCCDEF](BCCDEF.md) | 0x12 | 405 | written | Per-node integer boundary-condition code array for deformation (one code per displacement component). … |
| 163 | [BCCDFN](BCCDFN.md) | 0x13 | 406 | written | Per-node integer array companion to `BCCDEF` (one int per displacement component, `obj+0x998`). The name … |
| 164 | [BCCANG](BCCANG.md) | 0x14 | 0 | never written | Registered keyword (registry flag 8, handler1/2/3 all no-op stubs) whose parser case is empty: the |
| 165 | [URZ](URZ.md) | 0x15 | 406 | written | Nodal velocity vector: one vector per node, stored in `double* obj+0x8b8`, with component count |
| 166 | [FRZ](FRZ.md) | 0x16 | 406 | written | Nodal force vector: one vector per node, stored in `double* obj+0x8c0`, with component count |
| 167 | [PRZ](PRZ.md) | 0x17 | 173 | written | Per-node vector `PRZ` (array `obj+0x8d0`; no string in the code names it): one vector per node, stored in … |
| 168 | [BCCTMP](BCCTMP.md) | 0x18 | 406 | written | Per-node integer thermal boundary-condition code (`obj+0x9a0`, one int per node). |
| 169 | [BCCTFN](BCCTFN.md) | 0x19 | 406 | written | Per-node integer companion to `BCCTMP` (`obj+0x9a8`, one int per node; by its name the thermal BC function … |
| 170 | [NDTMP](NDTMP.md) | 0x1a | 420 | written | Nodal temperature of the object: one double per node in `obj+0x8e0`; in 2D the KEY line can also |
| 171 | [NDHEAT](NDHEAT.md) | 0x1b | 406 | written | Per-node heat value of the object: one double per node in `obj+0x8e8` (state variable code 9). |
| 172 | [NDFLUX](NDFLUX.md) | 0x1c | 173 | written | Per-node heat flux value of the object: one double per node in `obj+0x8f8` (state variable code |
| 173 | [ELMCON](ELMCON.md) | 0x1d | 423 | written | Element connectivity of the object's FE mesh: 4-node quadrilaterals in 2D; 4-node tetrahedra or |
| 174 | [MTLGRP](MTLGRP.md) | 0x1e | 423 | written | Material group number of each element: an integer array `obj+0x9f8` with |
| 175 | [STRAIN](STRAIN.md) | 0x1f | 406 | written | Effective strain of the object: per element `obj+0xa28` (state variable code 0x2b, `m = |
| 176 | [STNCMP](STNCMP.md) | 0x20 | 406 | written | Per-element strain-component state of the object: `m` doubles per element in `obj+0xa10` |
| 177 | [DENSTY](DENSTY.md) | 0x21 | 406 | written | Per-element relative density of the object: one double per element in `obj+0xa30` (state |
| 178 | [DAMAGE](DAMAGE.md) | 0x22 | 406 | written | Damage value(s) of the object: per element `obj+0xa38` (element container `obj+0x9e0`, slot |
| 179 | [STRESS](STRESS.md) | 0x23 | 406 | written | Stress tensor components of the object: per element `obj+0xa00` with `m = ncomp[0x26] = |
| 180 | [RMDPTH](RMDPTH.md) | 0x24 | 497 | written | One per-object remesh-criterion value (the `RM*` family). It is a double in the object's |
| 181 | [RMSTRK](RMSTRK.md) | 0x25 | 497 | written | One per-object remesh-criterion value (the `RM*` family). It is a double in the object's |
| 182 | [RMTIME](RMTIME.md) | 0x26 | 497 | written | One per-object remesh-criterion value (the `RM*` family). It is a double in the object's |
| 183 | [RMSTEP](RMSTEP.md) | 0x27 | 497 | written | One per-object remesh-criterion value (the `RM*` family). It is a double in the object's |
| 184 | [MGNELM](MGNELM.md) | 0x28 | 511 | written | Mesh-generation element counts for an object (name: "mesh number of elements"). 2D stores one |
| 185 | [MGTELM](MGTELM.md) | 0x29 | 494 | written | Mesh-generation element type/topology code (name: "MG type of element"); an integer stored at |
| 186 | [MGSIZR](MGSIZR.md) | 0x2a | 497 | written | Mesh-generation size ratio (name; "MG" = mesh generation, "SIZR" = size ratio). One double per object, … |
| 187 | [MGWCUV](MGWCUV.md) | 0x2b | 497 | written | Mesh-density weighting factor for boundary curvature (name: MG weight CUV). One double per object, stored … |
| 188 | [MGWTMP](MGWTMP.md) | 0x2c | 497 | written | Mesh-density weighting factor for temperature (name: MG weight TMP). One double per object, stored at … |
| 189 | [MGWSTN](MGWSTN.md) | 0x2d | 497 | written | Mesh-density weighting factor for strain (name: MG weight STN). One double per object, stored at … |
| 190 | [MGWSTR](MGWSTR.md) | 0x2e | 497 | written | Mesh-density weighting factor for strain rate (name: MG weight STR). One double per object, stored at … |
| 191 | [MGWUSR](MGWUSR.md) | 0x2f | 497 | written | Mesh-density weighting for user-defined windows (name: "MG weight user"); see MGUSER for the |
| 192 | [MGGRID](MGGRID.md) | 0x30 | 497 | written | Mesh-generation grid resolution (name: "MG grid"): two integers stored at `obj+0x2dc0` and |
| 193 | [MGERR](MGERR.md) | 0x31 | 497 | written | Mesh-generation error/tolerance pair (name: "MG error"): two doubles at `obj+0x2db0` and |
| 194 | [MGUSER](MGUSER.md) | 0x32 | 49 | written | One user-defined mesh window (local mesh-density region) of an object: a header line with the |
| 195 | [MGSIZM](MGSIZM.md) | 0x33 | 0 | never written | A 3D-only mesh-size double at `obj+0x2da8` (name: "MG size ..."). It is read in 3D but never |
| 196 | [USRNOD](USRNOD.md) | 0x34 | 389 | written | User-defined nodal state variables of the object: `m` doubles per node (state variable code |
| 197 | [USRELM](USRELM.md) | 0x35 | 383 | written | User-defined element state variables of the object: `m` doubles per element (state variable |
| 198 | [CUSVAR](CUSVAR.md) | 0x36 | 0 | written | Custom (user-named) state variable of the object: a `StateVarCustom` entry in the object's |
| 199 | [FRCSTP](FRCSTP.md) | 0x37 | 440 | written | Per-object integer flag/option of the "force" control group (`FRC*`). The code only stores and prints it. … |
| 200 | [FRCNEL](FRCNEL.md) | 0x38 | 440 | written | Per-object integer of the `FRC*` control group (the name suggests a node/element count). The code only … |
| 201 | [FRCMTH](FRCMTH.md) | 0x39 | 440 | written | Per-object integer selecting a method in the `FRC*` control group. The code only stores and prints it. It … |
| 202 | [TMPLMT](TMPLMT.md) | 0x3a | 431 | written | One double per object at `obj+0x1f10` (property sub-structure `obj+0x1ea0`, field `+0x70`; |
| 203 | [ANGMVY](ANGMVY.md) | 0x3b | 4 | written | The 2D angular movement of the object about its axis (2D only), used for torsion and inertia |
| 204 | [OBJPHS](OBJPHS.md) | none | 0 | not parsed, never written | OBJPHS is a registry entry with no reader and no writer. The binary still has a debug string |
| 205 | [VOLFC](VOLFC.md) | 0x3d | 22 | written | Per-element phase volume fractions of the object (state variable code 0x31): stored as |
| 206 | [DVOLF](DVOLF.md) | 0x3e | 22 | written | Per-element phase-related state of the object: `m` doubles per element in `obj+0xa68` (state |
| 207 | [TICF](TICF.md) | 0x3f | 22 | written | Per-element phase-related state of the object: `m` doubles per element in `obj+0xa70` (state |
| 208 | [GRAIN](GRAIN.md) | 0x40 | 4 | written | Per-element grain state (state-variable id 0x35, DB array LLGSZ element rows); width = number of |
| 209 | [GRAIN2](GRAIN2.md) | 0x41 | 0 | written | Per-**node** grain state (state-variable id 0x1d, DB array LLGSZ node rows): the nodal alternative |
| 210 | [SIZESH](SIZESH.md) | 0x42 | 0 | written | Per-element state variable with a material-dependent width (`W[0x36]`, from the material grain/phase … |
| 211 | [TXTODF](TXTODF.md) | 0x43 | 1 | written | Per-element state variable with a material-dependent width (`W[0x37]`); crystallographic texture (ODF) … |
| 212 | [CRPTIM](CRPTIM.md) | 0x44 | 440 | written | Per-object integer (the name suggests creep time). The code only stores and prints it. It is stored at … |
| 213 | [VOTAGE](VOTAGE.md) | 0x45 | 406 | written | Per-node scalar state variable (one value per node by default). Written next to BCCRHT/RHTFLX (resistance … |
| 214 | [DATOM](DATOM.md) | 0x46 | 406 | written | Per-node scalar state variable, written among the diffusion/carburising keywords (HDNEST, BCCCRB); nodal … |
| 215 | [REFPOS](REFPOS.md) | 0x47 | 43 | written | Per-object reference position. Mode 1 gives explicit coordinates (x, y, z). Mode 2 gives an |
| 216 | [VOLFS](VOLFS.md) | 0x48 | 22 | written | Per-element phase volume fractions (state-variable id 0x32, written with VOLFC/DVOLF). The model |
| 217 | [HDNOBJ](HDNOBJ.md) | 0x49 | 406 | written | Per-**element** double array with 2 components per element (`obj+0xa90`, `m` = `model+0x16d8` = 2); by … |
| 218 | [HDNEST](HDNEST.md) | 0x4a | 440 | written | Per-object hardness-estimation settings (by name): an on/off/mode int, a second int and two |
| 219 | [BCCCRB](BCCCRB.md) | 0x4b | 406 | written | Per-node integer BC code (`obj+0x9b0`, one int per node). The name and its neighbour `CRBFLX` suggest the … |
| 220 | [BCCRHT](BCCRHT.md) | 0x4c | 406 | written | Per-node integer BC code (`obj+0x9b8`, one int per node), companion of `RHTFLX`; it carries non-zero … |
| 221 | [CRBFLX](CRBFLX.md) | 0x4d | 173 | written | Per-node double value (`obj+0x910`, one per node); by name the carbon flux paired with `BCCCRB`. |
| 222 | [RHTFLX](RHTFLX.md) | 0x4e | 406 | written | Per-node double value (`obj+0x920`, one per node) paired with `BCCRHT`. |
| 223 | [TIMERC](TIMERC.md) | 0x4f | 0 | written | Per-node state variable whose width equals the TIMEID point count (`model+0x11c4`): one value per node and … |
| 224 | [ERECVL](ERECVL.md) | 0x50 | 0 | written | Per-element state variable whose width equals the ERECID point count (`model+0x123c`): element recorded … |
| 225 | [NRECVL](NRECVL.md) | 0x51 | 0 | written | Per-node state variable whose width equals the NRECID point count (`model+0x12b4`): nodal recorded values. |
| 226 | [WINPRS](WINPRS.md) | 0x52 | 0 | written | Window-defined pressure boundary condition: a list of `LLWINDEF_DATA` "window" records (a 2D |
| 227 | [WINTMP](WINTMP.md) | 0x53 | 8 | written | Window-defined thermal boundary condition (heat exchange with the environment inside a 2D polygon |
| 228 | [WINATM](WINATM.md) | 0x54 | 0 | written | Window-defined atmosphere boundary condition (diffusion/carburizing analogue of WINTMP: an |
| 229 | [WINRSE](WINRSE.md) | 0x55 | 0 | written | Window region for an "RSE" condition (the name suggests a link to RSEDEF, written later in the |
| 230 | [LOCTMP](LOCTMP.md) | 0x56 | 12 | written | A numbered local thermal boundary condition of the object: an entry of the object's list at |
| 231 | [LOCATM](LOCATM.md) | 0x57 | 0 | written | A numbered local atom (diffusion) boundary condition: an entry of the object's list at |
| 232 | [LOCDEF](LOCDEF.md) | 0x58 | 8 | written | A numbered local deformation boundary condition: an entry of the object's list at `obj+0x2fa0` |
| 233 | [YLDS](YLDS.md) | 0x59 | 406 | written | Per-element tensor-like state variable with `2·dim` components (written right after STRESS in the element … |
| 234 | [ELPSOL](ELPSOL.md) | 0x5a | 440 | written | Per-object integer option (the name suggests elasto-plastic solution). The code only stores and prints it. … |
| 235 | [CSFREQ](CSFREQ.md) | 0x5b | 440 | written | Per-object frequency for induction heating. It is either a constant or a 1-D table against one |
| 236 | [VOLCRG](VOLCRG.md) | 0x5c | 440 | written | Per-object volume charge or source, judging by the induction and resistance-heating examples. It |
| 237 | [ZEFI](ZEFI.md) | 0x5d | 406 | written | Per-element 2-component state variable, written with ZMFI/CURRNT after CSFREQ/VOLCRG (electromagnetic … |
| 238 | [ZMFI](ZMFI.md) | 0x5e | 114 | written | Per-element state variable with 2 (2D) / 3 (3D) components, written after ZEFI (magnetic field intensity … |
| 239 | [CURRNT](CURRNT.md) | 0x5f | 283 | written | Per-element vector state variable with `dim` components (electric current density by name; last of the … |
| 240 | [OTPRNG](OTPRNG.md) | 0x60 | 518 | written | Per-object output range (output-printing control). It holds a mode, two doubles and an integer, |
| 241 | [ECCDEF](ECCDEF.md) | 0x61 | 406 | written | Per boundary edge (2D) / boundary face (3D) integer code of the object's **deformation boundary |
| 242 | [ECCTMP](ECCTMP.md) | 0x62 | 420 | written | Per boundary edge (2D) / boundary face (3D) int array, the **code** member of the triple |
| 243 | [ECCATM](ECCATM.md) | 0x63 | 406 | written | Per boundary edge (2D) / boundary face (3D) int array, the **code** member of the triple |
| 244 | [ECDEFN](ECDEFN.md) | 0x64 | 406 | written | Per boundary edge (2D) / boundary face (3D) int array, the **function number** member of the triple |
| 245 | [ECTMFN](ECTMFN.md) | 0x65 | 406 | written | Per boundary edge (2D) / boundary face (3D) int array, the **function number** member of the triple |
| 246 | [ECATFN](ECATFN.md) | 0x66 | 406 | written | Per boundary edge (2D) / boundary face (3D) int array, the **function number** member of the triple |
| 247 | [ECPRES](ECPRES.md) | 0x67 | 406 | written | Per boundary edge (2D) / boundary face (3D) double array, the **value (pressure, inferred from the name)** … |
| 248 | [ECHFLX](ECHFLX.md) | 0x68 | 406 | written | Per boundary edge (2D) / boundary face (3D) double array, the **value (heat flux, inferred from the … |
| 249 | [ECAFLX](ECAFLX.md) | 0x69 | 406 | written | Per boundary edge (2D) / boundary face (3D) double array, the **value (atom flux, inferred from the … |
| 250 | [ECCRHT](ECCRHT.md) | 0x6a | 287 | written | Per boundary edge (2D) / boundary face (3D) int array, the **code** member of the triple |
| 251 | [ECRHFN](ECRHFN.md) | 0x6b | 287 | written | Per boundary edge (2D) / boundary face (3D) int array, the **function number** member of the triple |
| 252 | [ECRFLX](ECRFLX.md) | 0x6c | 287 | written | Per boundary edge (2D) / boundary face (3D) double array, the **value (resistance-heating flux, inferred … |
| 253 | [USRSUB](USRSUB.md) | 0x6d | 26 | written | Per-object integer that selects a user subroutine (by its name). The code only stores and prints it. It is … |
| 254 | [MATAXI](MATAXI.md) | 0x6e | 244 | written | Per-element material axis data: a double array stored as state variable 0x2e of the |
| 255 | [NWEAR](NWEAR.md) | 0x6f | 17 | written | Per-node tool-wear state variable with 5 components (written near the end of the object block; non-zero in … |
| 256 | [PRSTRE](PRSTRE.md) | 0x70 | 0 | written | Per-object triple of doubles in the movement/equipment sub-structure `obj+0x2160`. The name |
| 257 | [PRSNAM](PRSNAM.md) | 0x71 | 598 | written | Per-object name of the press/equipment, a string at the start of the movement sub-structure |
| 258 | [ROTSYM](ROTSYM.md) | 0x72 | 193 | written | 3D-only rotational-symmetry definition of an object. It holds one scalar (presumably the sector |
| 259 | [TRGVOL](TRGVOL.md) | 0x73 | 454 | written | Per-object target volume. It is an integer mode/flag plus a double volume, stored in the |
| 260 | [ANGMO2](ANGMO2.md) | 0x74 | 222 | written | The angular movement about the second (3D-only) rotation axis CNTRA2. It is the same layout as |
| 261 | [CNTRA2](CNTRA2.md) | 0x75 | 222 | written | The second rotation axis (3D only), used with ANGMO2: centre point, direction vector and current |
| 262 | [SYMSUF](SYMSUF.md) | 0x76 | 48 | written | Symmetry surfaces of a 3D object: a list of planes, each given as a point and a normal |
| 263 | [DRMESH](DRMESH.md) | 0x77 | 164 | written | Per-node vector, 3D only, stored in `double* obj+0x930` with component count |
| 264 | [WPAXIS](WPAXIS.md) | 0x78 | 67 | written | Work-piece axes: a numbered list of axis definitions (0xe8-byte records in the linked list |
| 265 | [OPSTOP](OPSTOP.md) | 0x79 | 5 | written | Per-object list of operation-stop entries (`OPSTOP_DATA` records in an `OpStopLinkedList` at |
| 266 | [DYNARY](DYNARY.md) | 0x7a | 0 | written | Per-object list of generic "dynamic arrays" (`DYNARY_DATA` records in a `DynAryLinkedList` at |
| 267 | [OBJUPD](OBJUPD.md) | 0x7b | 568 | written | Per-object integer "object update" option, in the 0x1ea0 sub-structure. The code only stores and prints … |
| 268 | [OBJSTP](OBJSTP.md) | 0x7c | 8 | written | Per-object "object stop" definition, in the movement sub-structure `obj+0x2160`. It consists of a |
| 269 | [TPLOGY](TPLOGY.md) | 0x7d | 178 | written | Boundary-representation topology of the object's curve geometry (geometry 0 of `obj+0x3998`): |
| 270 | [CSPMTX](CSPMTX.md) | 0x7e | 21 | written | A 4×4 matrix of doubles belonging to the object's first curve/surface geometry item |
| 282 | [CRDSYS](CRDSYS.md) | 0x8a | 0 | dead-code writer only | One integer per object, stored at `obj+0x2c38`. The only code that writes it out is the |
| 283 | [GEO23](GEO23.md) | 0x8b | 0 | dead-code writer only | A 2D→3D geometry conversion mode, stored in a sub-structure at `obj+0x2be8` (call it **G23**, |
| 284 | [GEOSEC](GEOSEC.md) | 0x8c | 0 | dead-code writer only | An integer count, `obj+0x2c24` (default 20), plus an on/off byte, `obj+0x2c21`. It belongs to the |
| 285 | [MSHSEC](MSHSEC.md) | 0x8d | 0 | dead-code writer only | An integer count, `obj+0x2c28` (default 21), plus an on/off byte, `obj+0x2c20`. It belongs to the |
| 289 | [FLDGEO](FLDGEO.md) | 0x91 | 0 | written | A surface mesh (nodes and 4-node facets) stored in the surface object at `obj+0x3328` (built by |
| 290 | [FLWNET](FLWNET.md) | 0x92 | 0 | written | A second surface mesh (nodes and 4-node facets) in the surface object at `obj+0x3458`, with the |
| 299 | [POLYTP](POLYTP.md) | 0x9b | 112 | written | Per-element integer "polygon type" of the object's mesh (`mesh = *(obj+0x3c00)`, read by |
| 300 | [DIEPNT](DIEPNT.md) | 0x9c | 2 | written | Die point sets of the object's mesh (`mesh = *(obj+0x3c00)`). The mesh keeps a vector of |
| 301 | [DIEEDG](DIEEDG.md) | 0x9d | 2 | written | Die edge sets of the object's mesh (`mesh = *(obj+0x3c00)`). The mesh keeps a vector of |
| 302 | [RSEDEF](RSEDEF.md) | 0x9e | 24 | written | A vector of n doubles held in the table object (`_common.md` §3) at `obj+0x20d8`. The writer |
| 303 | [FORCE](FORCE.md) | 0x9f | 180 | written | The object's force (load) vector: `obj+0x2300` (X), `obj+0x2308` (Y) and `obj+0x2310` |

Regenerate the table with
`python -I tools/g2_index.py ../keyword_registry.tsv tools/g2_casemap.tsv tools/g2_corpus_kw.tsv .`
(`g2_corpus_kw.tsv` from `tools/g2_corpus_index.py`; "Corpus files" counts KEY files under
`DEFORM_corpus` that use the keyword).
