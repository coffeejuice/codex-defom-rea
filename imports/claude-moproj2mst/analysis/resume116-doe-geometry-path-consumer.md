# CP116 stored Geometry paths and the first file consumer

A concrete downstream consumer of the stored upper/lower Geometry paths is now identified: `0x1400d4560` retrieves row-zero paths and passes their locale bytes to a native geometry loader, whose first action on the upper path includes a read-text stream open and two line reads. This closes a missing stored-path-to-file-consumer edge. It does **not** establish that the selected single-project Open→Save calls that loader. The evidence does not support a whole-lifecycle claim that no GEO consumer is reachable, and no public guard is changed.

I first read `analysis/resume116-doe-geometry-payload-boundary.md`. Its negative result remains correct for the bounded XML reader: the Geometry branch stores path/row/correspondence state but has no identified GEO open/read in that reader body. Its remaining external-consumer dependency can now be narrowed to the concrete chain below and its invocation/lifetime, rather than an unidentified generic geometry loader.

## Bound stored fields and consumers

The immutable original GUI PE is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. New DUMPBIN captures bind these getter effects:

- `0x140817940` checks the signed row index, selects `Var+0x28[index]`, and assigns the row's `+8` and `+0x10` strings to its two output arguments. These are the upper/lower paths written by the reader insertion routine. Invalid indexes leave outputs alone.
- `0x140817a80` similarly retrieves the row's nominal `+0` path. These getters assign values; they are not payload loaders.

An offline rel32/RIP pattern scan found getter call candidates, then bounded disassembly confirmed the concrete consumer calls used here. Direct upper/lower getter uses include `0x1400d4720`, `0x1400d4f31`, `0x14013393f`, and XML writer `0x14090c653`. Nominal uses include `0x140133691` and XML writer `0x14090c66f`. The scan is a locator: it does not find every possible inline field access, indirect dispatch or Qt connection and is not an exhaustive callgraph proof.

## Concrete upper-path file chain

In `0x1400d4560`:

1. It gets the manager's selected geometry-variable population through virtual `+0x1b0` at `0x1400d465c`, iterates it and obtains a Var. This is a downstream manager/context consumer, not a call made inside the XML Geometry reader.
2. At `0x1400d4720`, it invokes `0x140817940(var,0,upper,lower)`, with upper at `[rsp+0xe8]` and lower at `[rsp+0xf8]`.
3. At `0x1400d48e2`, it calls imported `QString::toLocal8Bit` on that upper string, then imported `QByteArray::constData` at `0x1400d4908`. Those IAT identities are independently parsed from the same PE. Thus the loader receives the converted **stored upper path**, not a freshly generated unrelated filename.
4. At `0x1400d4926`, it calls `0x1408ee540` on an allocated geometry object with that byte pointer, format argument `2`, and final argument zero.
5. The format-2 branch at `0x1408ee71d..0x1408ee751` constructs a local loader (`0x1408f6d10`) and calls `0x1408f7010`, passing the saved filename and geometry target. Other format branches are not this chain.
6. Before the main parser opens its stream, `0x1408f7010` calls probe `0x1408f83a0` at `0x1408f7064`. The probe's **first file attempt** is at `0x1408f83f5`: `0x14146fcd8(filename, "rt")`. The literal at `0x1427b79c4` is independently retained as bytes `72 74 00`. The wrapper supplies a fixed `0x40` third argument and jumps to `0x14146fba0`, which allocates a stream and calls its mode-parsing open routine `0x1414820ac` with the filename/mode/stream. This is a CRT-style read-text stream-open operation, not a QFileInfo construction or filename-only projection. A null stream returns false from the probe at `0x1408f8402..0x1408f840f`.
7. On a nonnull stream, the probe calls line-reader `0x1414733ac(buffer,2000,stream)` twice at `0x1408f8426` and `0x1408f843d`, then tokenizes the buffer using the retained delimiter bytes `20 2c 09 0a 0d 00`. It closes through `0x14146fb28` at `0x1408f849f`. The captured line-reader body advances stream-buffer bytes and handles a line terminator; it is a concrete read consumer. This first probe precedes the larger parser's allocations and payload loop.
8. A false probe returns false from `0x1408f7010` before its main open. If the probe passes, the main parser reopens the same saved filename at `0x1408f709d`, again through `0x14146fcd8`, this time with the `"rt"` literal at `0x1427b79b4`. Null returns before the payload loop; the successful parser later reads through `0x1409068f0`. Complete GEO grammar, allocation and malformed-file behavior are outside this cut.

Back in `0x1400d4560`, `0x1400d492b..0x1400d4948` tests the loader's integer result. The format-2 wrapper returns zero for a true parser result and -1 for false. The nonzero branch skips the later lower-file section. Otherwise the lower stored path is converted at `0x1400d49fe` / `0x1400d4a24` and passed to the same format-2 loader at `0x1400d4a42`. These are ordered file attempts; no claim is made that either original file opened or parsed successfully in a retained run.

This trace stops at the recovered CRT stream boundary. It does not claim the operating system CreateFile/ReadFile implementation, installed CRT runtime configuration, selected locale bytes or a dynamic file receipt. Those are not needed to distinguish an actual native open/read consumer from a stored path getter, but they are needed before reproducing all low-level failure effects.

## Save-related consumers and remaining reachability

The native DOE MST geometry block `0x140132bd0` gets nominal at `0x140133691` and upper/lower at `0x14013393f`. It constructs relative `../../Control/` filename references and conditionally derives `.COR` names from enabled, nonempty paths. Its call at `0x140133eb3` goes to `0x140909a30`; that routine reads row-zero upper/lower correspondence lists through `0x140818420` / `0x140818490` and calls `0x140909c50` only for nonempty output path/list. The latter constructs a QFile and opens it WriteOnly|Text at `0x140909cde` to **write correspondence output**. This is a real side effect, but it does not read GEO payload. The candidate MST emitter currently suppresses that COR writer; that historical candidate policy is not evidence about the native call.

XML writer `0x14090a550` also gets stored paths for Geometry attribute serialization. The prior complete retained writer capture binds the getter calls; it must not be described as loading GEO merely because it serializes their names. The new trace does not make an exhaustive negative claim about every downstream Save callback or every file engine.

The loader chain's direct caller is now bounded precisely: `0x140032afe` in the `0x140032800` caller invokes `0x1400d4560`. A separate path at `0x14003487e` in `0x140033cf0` calls `0x1400d4e50`, which also consumes stored upper/lower paths and uses geometry processing helpers, but its full file semantics are not claimed here. Retained decompile `evidence/gui_mo/s1/mdt/L0/C/c4s.c` shows Qt meta-dispatch cases 0x22..0x24 forwarding to `0x140032800` and 0x25..0x26 to `0x140033cf0`; that is a hypothesis about invocation roles, not proof of a live connection or Save occurrence. The native bounded caller windows independently confirm the two concrete direct calls.

No inspected receipt ties a normal Open/Save call or the `DoeTableRefresh` callback's normal-return path to `0x140032800` / `0x140033cf0`. Nor does a rel32-only caller search exclude such an indirect invocation. Therefore the correct result is **consumer recovered; single-project Save reachability unresolved**. A public Geometry implementation must not invent a read at XML entry, and should not require or execute this external loader there merely because it is now identified.

## Smallest next edge

Recover the native Qt metadata/connection or direct caller that invokes main-window method `0x140032800` (and its arguments/receiver lifetime), then test whether normal Open→Save or the actual `SfMoDoeTable::slotUpdateUI` return path reaches that invocation. The sharp static target is the connection/call edge **into `0x140032800`, before `0x140032afe -> 0x1400d4560`**. If excluded by a closed normal-Save graph, treat GEO loading as a separate workflow rather than a missing XML reader effect. If included, bind the same actual selected manager/Var/object and its locale-byte filename before reusing the loader contract.

Independently, actual reader admission still needs its native QFileInfo owner/normal return and numeric conversion/provider state. CP113/114/115 isolated value/row/pair helpers do not grant those capabilities. No further source change is supported by this evidence-only lane.

## Capture receipts and limits

New immutable analysis receipts bind individual bounded assembly outputs and the original PE hash:

| Receipt | SHA-256 |
| --- | --- |
| resume116-doe-path-getter-xrefs.json | `E356E3A3F68B9A648E1A079E039A5EE38DA779C0E60538DFB3CDF7BD602501EA` |
| resume116-doe-path-consumer-caller-xrefs.json | `CC3B393D969B1DB2A4B90CB9AC446A32D12DD6DEEC021B238871AB0BAD982CC0` |
| resume116-doe-path-consumers-static.json | `8F0B45A6D33E9D36B95B07D9063812299299C6C00934EA5BCD1B22B6FDB50CF3` |
| resume116-doe-path-file-edges-static.json | `AD40846E8FFC948126D6445543122136DD8D7865A6C57E2BF02DEB4D55006939` |
| resume116-doe-geo-read-mode2-static.json | `EBF732D5C92603BD6A2AEB7EF1DF38B5CBF2057DB947EABFFBDD9F7B9FB63D57` |
| resume116-doe-geo-fopen-edge-static.json | `81AB923445BDCFA33DD8B95443DEA808896945C9743FFD367A84A388256C53F2` |
| resume116-doe-crt-open-static.json | `EB970701A28391B86005C97F9B275F29EAA87386B991C836ABCCCCA8A59C3F94` |
| resume116-doe-openfile-edge-static.json | `FD1874DBD65B9CF2D9DA9CDEB13C9EA7AEA61283FB69CC0155D1A6D463B1BE9E` |
| resume116-doe-geo-open-literals.json | `BCD0C925CB2DC5A804B9D7D8551E1BA59B89E20FA5C3C1229B118969EC8EFDBF` |
| resume116-doe-path-consumer-imports.json | `9A1E169172C9E9972A9F13C3EB069385270AC1339F993BF7CB87D4ADA0DA9FF0` |

`resume116-doe-consumer-caller-windows.json` separately binds the explicit small caller windows; the trailing partial instructions outside the cited calls are not used. Unwind-region captures do not by themselves prove complete logical functions; the 16-byte wrapper capture likewise includes adjacent bytes after the wrapper jump, excluded from claims. Old provisional specification comments were used to locate branches and are not substitutes for the qualified native calls above. No source/test/build, original execution, GUI, network or mutable REA operation occurred.
