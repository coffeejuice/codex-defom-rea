# CP467 Type3 `.OPT` writer getter contract

## Evidence identity and scope

This is a static trace of the `DefOptModel` vtable calls in the CP465 writer. The retained PE is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes). Its file-version resource is absent; the PE timestamp and image-version fields are recorded in `cp467-opt-getters-byte-verification.json`. Every new getter range is bounded by the PE exception directory, exported with bounded `dumpbin /disasm:bytes`, and compared byte-for-byte against the mapped PE. All five ranges have zero mismatches. No original executable was run.

## Writer call to recovered field crosswalk

| Writer vtable slot | PE target | Getter result | Backing value / behavior |
|---|---:|---|---|
| `+0x98(owner, group, out)` | `0x14091eff0` | Temporary field1-value count | Filters NUM_CON rows by classifier, appends matching row field1 values to its by-value temporary, returns the count and destroys the temporary. CP468 proves the writer tests a distinct original empty list, so preliminary +0x60 calls are skipped in the stable normal profile. |
| `+0x60(owner, i)` | `0x14091e520` | signed 32-bit | NUM_CON field at node `+0x0c` (field4); invalid signed index returns `-1`. |
| `+0x50(owner, i)` | `0x14091e460` | double | NUM_CON field at node `+0x18` (field3); invalid signed index returns `0.0`. |
| `+0x70(owner, i)` | `0x14091e5e0` | signed 32-bit | NUM_CON field at node `+0x08` (field2); invalid signed index returns `-1`. |
| `+0x40(owner, i)` | `0x14091e3a0` | signed 32-bit | NUM_CON field at node `+0x04` (field1); invalid signed index returns `-1`. |

These map to the physical row layout already qualified by CP450: classifierResult `+0`, field1 `+4`, field2 `+8`, field4 `+0xc`, uninitialized double `+0x10`, field3 `+0x18`, formatted QString `+0x20`. The new getter exports confirm that the writer’s row calls are projections over that layout; they do not make the uninitialized `+0x10` deterministic or writable.

## Existing getters and source crosswalk

CP450 already binds writer slots `+0x118/+0x128/+0x138` to `owner+0x30/+0x34/+0x38` (`int32`), `+0x148` to `owner+0x40` (`double`), `+0x158` to `owner+0x48` (`byte`), direct helper `0x14091e0b0` to `owner+0x0c`, and `0x14091e0f0` to `owner+0x10`. Slot `+0x28` gives the NUM_CON list size at `owner+0x18`; `+0xc0` gives the region list size at `owner+0x20`; `+0xd8` returns a region name in list order or `Selected Object` for an invalid index. Those prior contracts and the CP465 writer call order were not recaptured.

The checked source crosswalk is `Type3OptimizerScalarState` and its existing prepared row vectors in `source/apps/moproj2mst/src/ops/type3_optimizer_scalar_state.hpp/.cpp`. Scalars and row member order exist. The existing private DefOpt view is the integration point for four CP467 scalar row getters with native invalid-index defaults. The +0x98 filter mutates a copied temporary and must not be exposed as a writer-visible matching list; CP468 closes that identity in the stable normal profile. Existing scalar and region helpers remain available. This does not model native QList copy-on-write, QString ownership, allocator failure, exceptions, or the owner’s liveness.

CP468 supersedes the earlier writer-visible group-list inference. Its byte-bound copy, append, destruction and Qt detach trace establishes why the separate writer lists remain empty under stable normal calls.

## Remaining limits

The byte evidence establishes getter load/return behavior and the writer’s chosen getter order. It does not establish exact Qt text/codec bytes, stream buffering, QFile write/close effects, or native owner construction/lifetime. Do not describe the projection as a Qt output serializer or claim byte-for-byte `.OPT` parity.

## Reproduction

From this checkout run `python analysis/cp467-opt-getters-verify.py`. It verifies the immutable PE SHA, vtable targets and prior table hash; derives complete getter bounds from the PE exception directory; regenerates the five bounded disassemblies; compares every addressed byte; and refreshes the JSON receipt and this report.
