# CP131 Type3 row append and delimiter frontier

## Conclusion

The retained parser assembly and bounded helper windows close the successful-call append shapes more tightly than CP128. `NUM_CON` passes six converted values to DefOptModel vtable `+0x8` (target `0x14091e110`) and appends a temporary row to member `+0x18`. `REGIONS` passes integer zero and a QString reference to vtable `+0xa8` (target `0x14091f1a0`), which builds an `{int, QString}` temporary and appends to member `+0x20`. Both virtual methods return `size - 1`; the parser ignores those returns. A value-only projection can accept these already prepared inputs, but source-equivalent file parsing remains blocked.

## Immutable evidence qualification

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256 was verified as `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Reused `analysis/resume127-type3-opt-parser.asm` and `evidence/gui_mo/s1/vtables.tsv`; bounded MSVC dumpbin 14.44.35207 windows were:

- `0x14091e110–0x14091e1e0` and `0x14091f1a0–0x14091f230`: the DefOptModel vtable `+0x8` / `+0xa8` methods, bounded by the next vtable target.
- `0x14091fab0–0x14091fb60`, `0x14091fb60–0x14091fce0`, `0x14091faf0–0x14091fb60`, and `0x14091fce0–0x14091fe00`: directly called temporary constructor/append helper code.
- `0x140919640–0x1409196a0` and `0x1409196a0–0x140919700`: the two temporary destructors.

No source, build, test, GUI, Ghidra import, mutable REA database, or original process was used.

## `NUM_CON` append sequence

Parser `0x140908b71` reads token 1 as row count with `toInt(base 10)`. It counts rows and checks QTextStream EOF before each read. For each available row it splits using the unresolved global regex, then converts fields 1, 2, 3, and 4 in order as `int`, `int`, `double`, `int` (`0x140908c48–0x140908ce5`). The first integer is sent to virtual `+0xa0` and its result retained as another integer (`0x140908cf7–0x140908d0c`). The parser constructs a QString via the static string/QString path, then calls virtual `+0x8` with inputs in this order: field-1 integer (`edx`), classifier result (`r8d`), field-3 double (`xmm3`), constructed QString (first stack argument), field-2 integer (second stack argument), field-4 integer (third stack argument).

The append method constructs a stack row whose QString is at `+0x20` and whose constructor writes integer defaults `+0x8=1` and `+0xc=1`. The method then assigns the four integer members at `+0/+4/+8/+0xc`, a double at `+0x18`, and the passed QString at `+0x20`. It calls `0x14091fb60` with the address of this local and member `+0x18`; that helper uses a growth/copy helper chain. The virtual method queries the vector size, returns `size - 1`, and destroys the temporary. Destructor `0x140919640` destroys the QString at row `+0x20`. The parser does not consume the method return. Appends occur immediately; count completion or EOF returns to the top-level loop without undoing rows already appended.

These windows establish successful input order and observed member stores. They do not close the formatter QString's meaning, short-row behavior, the classifier's complete semantics, Qt allocation/copy/refcount behavior, or member teardown.

## `REGIONS` append sequence

Parser `0x140908db9` converts token 1 to an integer count. The loop checks EOF, reads and splits each available row using the same unresolved regex, derives a QString value, then calls virtual `+0xa8` at `0x140909252–0x14090925a` with `edx=0` and `r8` pointing to that QString. Method `0x14091f1a0` builds a local `{int at +0, QString at +8}` value, copies the passed QString, then calls helper `0x14091fce0` for member `+0x20`. It queries size and returns `size - 1`; the caller ignores this result. Destructor `0x1409196a0` destroys the temporary QString at `+0x8`. Reaching count or EOF returns to top-level parsing; earlier appends remain.

This closes the append input shape, not the conversion of the split row into its QString, the regex, or vector data ownership/allocation.

## Smallest next cut and blocker

A bounded, private value-only cut could project a `NUM_CON` row from the six already-converted append inputs and a `REGIONS` `{int, QString}` row from already-derived inputs, then append to value containers in encounter order. Such a cut must not tokenize file text, derive the region QString, run the classifier absent its own contract, construct native containers, or claim allocator/refcount/exception parity. If that projection is not a useful isolated increment, keep both row keys guarded.

The static QRegExp at `0x143dd9a28` is passed at parser sites `0x1409088ed`, `0x140908c24`, and `0x140908e97`; no retained capture locates its initializer, pattern, or options. This prevents reproducing line-to-token behavior and the inputs to the row projections. Full row-owner parity also needs the global initializer/lifetime, member copy/cleanup paths for `+0x18/+0x20`, and relevant Qt shared-data allocation/unwind behavior. CP130's tokenized `Terminate` result handles only ENDDATA control and does not close these gaps.
