# Checkpoint 56: bounded DBLIST callee and dispatch tail

## Captures and identity

Read-only DUMPBIN `/DISASM /RANGE` captures were produced from `evidence/original-binaries/DEF_GUI_MO_64.exe` (SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`):

- [resume56-dblist-callee-cf740.asm](resume56-dblist-callee-cf740.asm), requested range `0x1402cf740` through `0x1402cfb10`, SHA-256 `A61B6198D563DC3853599F95393EB5D597F7662A39F9D0B05DC3E9A03F6B751D`.
- [resume56-dblist-virtual-tail.asm](resume56-dblist-virtual-tail.asm), requested range `0x140656156` through `0x1406561ec`, SHA-256 `EABB1AB60A99FCE8C6AD4D32BCCF74BCF4857E560DE8341C77717E8FC12889B2`.

The first range includes the complete direct callee `0x1402cf740`, whose cleanup ends in `ret` at `0x1402cfacc`, plus padding and the beginning of the next function at `0x1402cfae0`. The second is the exact final dispatch/cleanup tail from `FUN_140655df0`, through its `ret` at `0x1406561eb`. The existing handler capture already establishes its full function boundary. No other binary ranges were requested.

## Direct callee boundary

At the handler's call to `0x1402cf740`, the visible argument setup passes global pointer `0x143d76700` in `rcx`, a constant pointer `0x1424e3fd8` in `rdx`, and addresses of locally assembled generic argument records in the remaining register/stack arguments. The callee checks byte offset `+0x141` on the first object and takes an early cleanup path when it is nonzero. Otherwise it prepares/copies a series of fixed-size argument records, iterates over a count, and calls through an indirect import slot at `0x141e05348` with a flag value of `1`. It stores the returned byte in a local, then cleans up and returns.

This establishes a dynamic argument-dispatch boundary, but not the semantic name or implementation of the indirect target. The constant at `0x1424e3fd8` is not decoded here. No DBLIST data is directly stored to a field of the incoming part pointer in this callee. Its return value is also not identified from this slice: the indirect call result is saved locally, but the visible epilogue does not establish that value as the returned result.

## Handler's final virtual call

The new tail excerpt shows `FUN_140655df0` checks a local pointer at `[rsp+0x78]`. If non-null, it calls an indirect helper using a local wrapper and a DOM-related local (`0x14065617e`), stores that return value, then invokes the method at vtable offset `+0x218` on the object from `[rsp+0x78]`, with that returned value and `r8b=1` (`0x14065618c`–`0x1406561a4`). Both the null and non-null paths converge on a local return-byte set to `1` at `0x1406561aa`; the handler returns at `0x1406561eb`.

The object in `[rsp+0x78]` is not proven to be the incoming part model or a field within it. Its construction/provenance and the virtual implementation at `+0x218` remain unresolved. That method is a plausible effect boundary, but the assembly cannot tell whether it mutates session state, writes a side file, or delegates further. The dynamic dispatch inside `0x1402cf740` is a second unresolved effect boundary. No direct filesystem call is visible in the captured instructions; this does not prove the indirect targets are free of file effects.

## Updated conclusion

These captures sharpen the previously unknown route: the original DBLIST helper packages node-derived values into dynamic arguments, routes them through an indirect dispatcher, and may then call a virtual method on a separately held object. They do not reveal the DBLIST owner field or the dispatch target's behavior. The earlier gate result remains exact: DBLIST reaches this helper only when the global object's 32-bit `+0x1c` field is zero. Keep the candidate reader detached and keep public B127/Type2 activation guarded. The smallest next static dependency is identifying the import target at `0x141e05348` and the runtime class/vtable implementation for the receiver at `[rsp+0x78]`, without broadening the original-analysis session or inferring effects from candidate output.

No source/build/GUI/REA mutable state or original runtime was touched. The only new artifacts are the two uniquely named bounded captures and this analysis note.
