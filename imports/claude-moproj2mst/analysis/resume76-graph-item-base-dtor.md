# CP76: `c1c690` and the histogram graphics-item base destructor

## Result

The helper at `0x140c1c690` is not a local-resource-only cleanup. Its body installs a vtable and calls the imported `QGraphicsItem::~QGraphicsItem`. The matching import resolves in the retained original `QtGui4_64.dll` to RVA `0x558950` (preferred VA `0x65558950`). That framework destructor includes a conditional path that obtains `QObjectPrivate` for the item-associated object, checks private state, and—when its pointer at `+0x78` is non-null—calls through a function pointer with the item pointer (`this-0x10`) as an argument. It then clears that pointer. This is evidence of framework-level object/scene-associated teardown, beyond just a local member release.

The retained static bytes do not identify the indirect target at global `0x656974e8` or prove that the conditional path is taken for `SfGraphHistogramItem`. Therefore this does not establish whether the destructor removes the item from a `QGraphicsScene` in this instance, or the precise scene/list unlink sequence. It is unsafe to infer scene ownership from the base-destructor call alone.

## Bounded evidence

- Original executable: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Qt GUI DLL: `evidence/original-binaries/QtGui4_64.dll`, SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
- `dumpbin /IMPORTS` on the executable maps `??1QGraphicsItem@@UEAA@XZ` to QtGui ordinal hint `0x50f`; `dumpbin /EXPORTS` on that DLL gives ordinal 1296, hint `0x50f`, RVA `0x558950`.
- `analysis/resume49-histogram-base-chain.asm` at `0x140c38fb1` passes `this+0x10` to `0x140c1c690`.
- `analysis/resume76-helper-c1c690.asm` is the bounded helper capture `[0x140c1c690,0x140c1c6b8)`. It resets the vptr at `[this]` to `0x14282f4d8` then calls IAT slot `0x141e07e38`.
- `analysis/resume76-qgraphicsitem-dtor.asm` is the bounded Qt destructor capture `[0x65558950,0x65558a20)`. Relevant instructions: destructor sets its own vptr at `0x65558983`; at `0x65558998` it calls `QObjectPrivate::get` (IAT `0x65698b70`, name confirmed by retained import map); it checks a private flag at `+0x13c`, tests a pointer at `+0x78`, passes the item pointer in `rdx` to indirect call `[r8]` at `0x655589b9`, then zeroes `[rbx+0x78]` at `0x655589bc`.

## Boundary / next missing edge

Resolve the target value behind `0x656974e8` in the original QtGui image and its function signature/implementation, then establish the relevant private-flag and pointer state for this item or find a direct graph-specific scene removal call. Until then, the original app evidence supports “QGraphicsItem framework destructor with conditional associated-object teardown,” not a proven histogram scene unlink or scene ownership policy.

No source, build, GUI, or mutable REA state was accessed or changed.
