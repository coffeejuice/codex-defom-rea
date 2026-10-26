# CP107 — Geometry receiver vtable target

## Static caller check

The retained caller for `0x14090ee20` is the `DoeVariables` branch in `0x14090e080`, at `0x14090e48d`. Its visible setup loads the filename copy into `RCX`, the current DOM variable element into `RDX`, and a third argument from `[rsp+0x3a8]` into `R8`, then makes a direct call to the reader. This confirms the known outer call edge and filename provenance, but it does not expose the dynamic vtable used by the reader's separate `[rsp+0x1030]` object.

Inside the reader, the geometry receiver is returned by an indirect dispatch at `0x14090f0ef` through vtable slot `+0x1a0` on `[rsp+0x1030]`. The immediately preceding call at `0x14090f0c5` dispatches slot `+0x178` on the same object; its result is decremented and passed to slot `+0x1a0`. No concrete vtable value or direct target is embedded at either callsite. The known `0x14090e080 -> 0x14090ee20` caller does not statically resolve those indirect targets.

## Ownership/file-consumer status

Without a concrete vtable for `[rsp+0x1030]`, the receiver's constructor, concrete class, and teardown cannot be named. The available reader evidence establishes that receiver `+0x28` gets an allocated 0x38-byte list record and that upper/lower correspondence pairs are passed to receiver methods. It does not show the receiver being deleted at reader exit. Nor does the reader body show a file open/read after storing the geometry filenames.

## Exact missing dependency

The next required evidence is a caller or object-construction path that fixes the value of `[rsp+0x1030]` at the `0x14090ee20` invocation (or a verified runtime vtable value). That binds slot `+0x1a0` / `+0x178` to concrete code and allows tracing the same object's destructor and any filename consumer. The currently retained static caller capture supplies no such binding, so further ownership conclusions would be speculative.

## Evidence scope

- Original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- `analysis/resume104-doe-geometry-path-caller-14090e080.asm`, `0x14090e45f..0x14090e492`: visible direct caller setup and call.
- `analysis/resume50-doe-variable-body-pe-annotated.asm`, `0x14090f0c5..0x14090f0f5`: two indirect calls and receiver return; `0x140911f99`, `0x1409122fe`, `0x14091264c`: mutations; `0x140912a0e`: reader return.
- No dynamic REA state, original execution, or mutable database was used. Static caller presence is not runtime reachability evidence.
