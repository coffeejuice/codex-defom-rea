# CP143 FRCROT group-3 parser and pair ordering

## Result

The retained original-PE disassembly closes the FRCROT prefix that CP142 left open: the group-3 reader creates or finds the forward master/slave pair before parsing FRCROT’s mode/value line, and its common successful tail may create a reverse pair and copy the forward payload into it. This is enough to define pair-identity/order observations for a bounded private projection. It is not enough to claim public FRCROT import or exact scratch-model outputs: the actual three input texts are not supplied by a source-qualified active owner, and table payload parsing / mirror-copy failure behavior remains outside the captured contract.

## Parser path and order

`FUN_140b667a0` is the group-3 parser called by `FUN_140916530` for nominal, upper, and lower scratch models (CP142, `analysis/resume142-frcrot-input-provenance.md`). The parser first distinguishes the special ID `0x10f` from the general group-3 route. FRCROT is ID `0x114`, so it takes the general route. The dispatch computes `id - 0x110` and indexes the jump table at `0x140b68f84`; index 4 enters the FRCROT case at `0x140b67701`.

In that common route, the parser reads the pair’s two integer identifiers using the retained `%i %i` format at image RVA `0x027e6b80` (VA `0x1427f4a10`) through `0x141472ac8`. It then finds the `(master, slave)` entry in the model pair list at `model+0x13c8` using `0x1408e6bd0`. If absent, it appends one through `0x1408e6980`, then stores master and slave at pair offsets `+0x168` and `+0x16c`. Thus forward pair creation precedes FRCROT-specific parsing. The format and ordering are established; exact line consumption and malformed partial state are not generalized beyond the decoded calls.

The FRCROT case calls `0x140af20b0(file, &mode, &value)`. That helper zero-initializes its outputs, reads a bounded line with `fgets`, translates commas to spaces, and scans `%i %Flf` (format at RVA `0x027e8538`). EOF or a scan result of `-1` is a failure. On success, the case resets the pair’s `+0x240` table through `0x14089b9e0` using template `0x16`, then applies these mode-specific mutations:

- mode `0`: dimension `0` at `+0x244`; constant double at `+0x248`.
- mode `1`: dimension `1`; code `1` at table `+0x250`; integer-cast value at `+0x25c`.
- mode `2`: dimension `1`; code `4` at table `+0x250`; integer-cast value at `+0x25c`.
- Other modes: retain the reset defaults.

All successful mode branches then call `0x140af73f0(file, table, 0)` for additional table input. The function’s detailed row grammar and its exact partial mutation on failure have not been closed here.

## Pair completion and failure ordering

After the FRCROT branch returns to the parser’s common tail, a self-pair (`master == slave`) skips reverse-pair work. Otherwise it looks up `(slave, master)`; if missing, it appends a reverse entry and writes the reversed identifiers at `+0x168/+0x16c`. It sets the reverse direction flag at `+0x170` relative to the forward entry and calls `0x1408e5d60(reverse, forward)` to copy mirrored pair data. Only after this tail does the parser inspect the saved helper status and return its parse error code.

The FRCROT mode/value helper’s early failure path returns before the common mirroring tail, so the forward pair may already exist while the reverse pair does not. A later `0x140af73f0` failure happens after the common tail is reachable; precise returned status and payload validity depend on that helper and are not asserted here. This ordering matters for any failure-prefix test: pair existence is not an all-or-nothing transaction.

## Smallest bounded implementation candidate

A private value-level seam can represent the facts already closed: ordered forward-pair ensure, mode reset/field selection, and the conditional reverse-pair ensure after the table-read stage. Its inputs must be explicit prepared observations (IDs, mode, parsed numeric value, and table-read outcome); it should not read files or synthesize `KwOperations` results. Tests can assert self-pair behavior, forward-before-mode order, the three recognized mode encodings, other-mode reset retention, and that early mode-line failure leaves only the forward pair.

Do not include reverse payload-copy semantics or detailed table rows in that helper until `0x1408e5d60` and `0x140af73f0` are separately closed. The active source still has no qualified producer for the nominal/upper/lower text inputs, so wiring the helper into public conversion remains blocked. CP142’s handler3 selection rule remains downstream: it borrows the selected pair’s `+0x240` and the reader deep-copies only after successful parse.

## Evidence qualification

This is a bounded static reconstruction from the immutable original GUI PE and retained CP142 caller/handler evidence. The original image SHA-256 is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Format strings are additionally present in `evidence/gui_mo/strings.txt` at RVAs `0x027e6b80` (`%i %i`) and `0x027e8538` (`%i %Flf`). No source, build, GUI, original execution, or mutable REA database was used.
