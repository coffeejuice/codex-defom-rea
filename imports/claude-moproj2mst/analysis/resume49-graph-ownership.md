# Checkpoint 49 SfGraphHistogramItem lifetime evidence

## Identity and vtable binding

Target: retained immutable `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The PE `.rdata` contains the `.?AVSfGraphHistogramItem@@` TypeDescriptor at VA `0x143706c20` (the object begins at `0x143706c10`). Its Complete Object Locator has self RVA `0x30ef528`; the pointer stored at VA `0x14283dfc0` refers to that locator, and the primary vftable begins at `0x14283dfc8`. This binding is corroborated by constructor/destructor code at `0x140c2b731` and `0x140c2c622`, both writing that vftable address into the object. The table's slot at offset `+0x18` points to `0x140c301e0`.

The bounded receipt `analysis/resume49-histogram-vftable-slot18.asm` covers `[0x140c301e0,0x140c302d0)`. At `0x140c301e0`, the scalar deleting wrapper calls `0x140c2c600`, checks flag bit 0, and on set calls `0x1414dfc68` to free the object. This binds the native deleting virtual used by `c15db0` to the concrete class path, rather than an adjacent guessed function.

## Destructor and construction prefix

`analysis/resume49-histogram-destructor-chain.asm` includes `0x140c2c600` through its return at `0x140c2c8b5`. It resets the primary and secondary vtables, processes owned members including the name field at `+0xc0`, emits `2signalHideSecondaryWidgetInSplitView()` through dispatcher `0x1402cf3c0` near the end, then calls `0x140c38f40` for the base destructor. The base destructor view `analysis/resume49-histogram-base-chain.asm` shows its own vtable resets and destruction of member fields at `+0xc8`, `+0xc0`, and `+0x98`; the bounded portion does not bind the Qt framework's complete item-destruction callback. No explicit `QGraphicsScene::removeItem` or manager `+0x10` mutation is visible in the captured concrete destructor body.

`analysis/resume49-histogram-constructor-body.asm` captures constructor candidate `0x140c2b6b0`. It first calls base constructor `0x140c38ca0`, then writes the bound primary/secondary vtables and initializes fields, including a `+0xc0` name string. Two call sites in `0x140c36d9d` and `0x140c36e55` invoke this constructor. The captured constructor prefix has no direct `QGraphicsScene::addItem` call.

## Ownership result

This closes concrete `SfGraphHistogramItem` RTTI → COL/vftable → slot `+0x18` deleting wrapper → concrete destructor, and finds its constructor call sites. It does **not** establish who owns scene lifetime. The custom destructor performs application cleanup and then its own base/member destruction, but the available bounded code does not show scene removal. The constructor itself does not insert into a scene in the captured body; a caller or subsequent stage may do so. QGraphicsScene may also update membership implicitly through framework item destruction, which is not proven by this application's PE calls.

Therefore the manager list remains demonstrably nonowning/guarded and actively requests deletion; the scene's `unique_ptr` model remains unvalidated. Before choosing it, trace the constructor caller's post-construction path and bind the framework/base destructor behavior or explicit scene unregister. Do not infer scene ownership from an absent `removeItem` call in a single bounded function.

## New immutable bounded captures

- `analysis/resume49-histogram-vftable-slot18.asm`: concrete slot `+0x18` deleting wrapper.
- `analysis/resume49-histogram-destructor-chain.asm`: destructor body range through the exact return.
- `analysis/resume49-histogram-constructor-body.asm`: constructor implementation prefix/body slice containing the vtable writes and initialization.
- `analysis/resume49-histogram-constructor-callers.asm`: caller sites for that constructor.
- `analysis/resume49-histogram-base-chain.asm`: bounded base destructor prefix.
- `analysis/resume49-histogram-scene-insert-search.asm`: extended caller context checked for direct scene insertion; no `QGraphicsScene::addItem` IAT call appears in this bounded caller region.
