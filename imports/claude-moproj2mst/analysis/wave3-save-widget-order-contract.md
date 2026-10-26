Base Save calls widget update and then unconditionally copies properties into the engine. This second step was missing from the adapter's shown-editor branch.

Raw `0x1405ef510` calls virtual+0xd0 at `0x1405ef53f`, then `0x1400543a0` unconditionally at `0x1405ef54a`, followed by virtual+0xd8, +0xe8 and +0x80. It does not branch on the widget slot return value. Base widget update `0x1405f1e20` uses the property-stack index: zero calls `0x1400543a0`; nonzero emits widget UpdateData then calls `0x140054350`. The latter invokes props+0x58; `0x1400543a0` invokes props+0x60.

Thus a shown editor executes apply, propsLoad, propsToEngine, then the remaining Save slots. Index zero executes propsToEngine twice. Root owns the shared correction and dispatch-order tests. This receipt covers the base Save path; ShapeOptim and other class-specific Save overrides retain their separate contracts. No original/runtime/UI execution was used.
