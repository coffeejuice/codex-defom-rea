"""Generate the static option-label tables from DEF_PRE_64.exe's own initialiser code.

The 38 C++ dynamic initialisers at 0x140501bd0-0x1405036a0 (packet p1-02; called from _initterm)
each fill one global array of VS2010 std::string (0x28 bytes: +0x08 buffer, +0x18 size,
+0x20 capacity) with literal labels, then register the array's destructor with atexit. The tables
come in two identical sets (0x1405be8c0.. and 0x1405c0ce0..): a header with `static const
std::string x[] = {...}` included by two translation units. Nothing else in the decompiled code
reads them.

This script decodes every initialiser's machine code (.text, read-only) with a strict matcher of
the exact instruction forms the compiler emitted, follows each `lea rdx, [label]` / length pair
into .rdata, and writes

  port/defmodel/include/defmodel/options/option_tables_data.hpp   constexpr tables
  docs/re/option_tables.tsv                                       one row per string

Per string the instruction sequence is (the first string of a table has no pre-initialisation):

  48 c7 05 d32 0f000000   mov qword [obj+0x20], 0xf      capacity
  48 c7 05 d32 00000000   mov qword [obj+0x18], 0        size
  c6 05 d32 00            mov byte  [obj+0x08], 0        buffer[0]
  41 b8 imm32             mov r8d, len
  48 8d 15 d32            lea rdx, [label]
  48 8d 0d d32            lea rcx, [obj]
  e8 rel32                call FUN_140005c80             std::string::assign(const char*, size_t)
  90                      nop

and each function is `sub rsp,0x38; mov qword [rsp+0x20],-2; <strings>; lea rcx,[dtor];
add rsp,0x38; jmp atexit`. The two 8-string tables and the two 5-string tables use a second form:
`push rbx; sub rsp,0x30`, `xor ebx,ebx` before the second string, then `mov [size],rbx`,
`mov [buf],bl` and `lea r8d,[rbx+len]` (len as imm8) instead of the immediates, and
`add rsp,0x30; pop rbx` before the jmp.

Usage: gen_option_tables.py [--exe PATH] [--check]
  --check   do not write; exit 1 if the committed files differ from what the binary gives
            (77 = skipped, the binary is not installed).
The binary is only read. Default path: $DEFORM_PRE_EXE or the v11.0.2 install.
"""
import argparse, os, pathlib, struct, sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
from gen_keyword_registry import Pe, DEFAULT_EXE  # noqa: E402  (same PE reader)

ROOT = pathlib.Path(__file__).resolve().parents[3]
HEADER = ROOT / "port/defmodel/include/defmodel/options/option_tables_data.hpp"
TSV = ROOT / "docs/re/option_tables.tsv"

FIRST_INIT = 0x140501BD0
END_INIT = 0x140503710      # first initialiser that is not a string table (licence globals follow)
INIT_COUNT = 38
ASSIGN_FN = 0x140005C80     # std::string::assign(const char*, size_t)
STRING_SIZE = 0x28

# Names for the 19 tables of one set, in initialiser order. Inferred from the labels only: no
# decompiled code reads the tables, so no caller names them.
NAMES = [
    "heat_transfer_mode",   # Isothermal, Heat transfer , Non-isothermal, uniform die temp
    "unit_system",          # SI, English
    "temperature_source",   # User, System , Temperature
    "iteration_method",     # Newton-Raphson, Direct, Explicit
    "no_yes",               # No, Yes
    "solver",               # Sparse, Conjugate-Gradient, Skyline, GMRES
    "solver_short",         # Sparse, Conjugate-Gradient, Skyline
    "value_type",           # Constant, Time Function, Temperature Function
    "object_type",          # Undefined, Rigid, Plastic, Prorus, Elastic, Elasto-Plastic, ...
    "movement_type",        # Force, Speed, Hammer, Screw, Mechanical Press
    "torque_type",          # Torque, RPUT
    "geometry_source",      # None, Polygons, Points
    "contact_relation",     # " None", " Slave-Master", " Master-Slave"
    "friction_type",        # None, Shear, Coloumb
    "tolerance_mode",       # System, User (%), User (Value)
    "positioning_mode",     # Offset, Interference, Rotation
    "simulation_mode",      # Incremental, Discrete, Steady State
    "geometry_type",        # Axisymmetric (no rotation about Z-axis), Plane Strain, ...
    "variable_constant",    # Variable, Constant
]

OP = {name: bytes.fromhex(h) for name, h in [
    ("sub38", "4883ec38"), ("push_sub30", "40534883ec30"), ("guard", "48c7442420feffffff"),
    ("movq_rip", "48c705"), ("xor_ebx", "33db"), ("movq_rbx", "48891d"), ("movb_bl", "881d"),
    ("movb_rip", "c605"), ("zero8", "00"), ("mov_r8d", "41b8"), ("lea_r8d_rbx", "448d43"),
    ("lea_rdx", "488d15"), ("lea_rcx", "488d0d"), ("call", "e8"), ("nop", "90"),
    ("add38", "4883c438"), ("add30_pop", "4883c4305b"), ("jmp", "e9")]}


class Decoder:
    def __init__(self, pe, va):
        self.pe, self.va = pe, va
        self.o = pe.off(va)

    def peek(self, op):
        pat = OP[op]
        return self.pe.d[self.o:self.o + len(pat)] == pat

    def take(self, op):
        pat = OP[op]
        if not self.peek(op):
            sys.exit("0x%x: expected %s (%s), found %s" % (self.va, op, pat.hex(" "),
                     self.pe.d[self.o:self.o + len(pat)].hex(" ")))
        self.o += len(pat)
        self.va += len(pat)

    def i32(self):
        v, = struct.unpack_from("<i", self.pe.d, self.o)
        self.o += 4
        self.va += 4
        return v

    def u8(self):
        v = self.pe.d[self.o]
        self.o += 1
        self.va += 1
        return v

    def rip(self, extra=0):
        """rip-relative target of a disp32 followed by `extra` immediate bytes."""
        d = self.i32()
        return self.va + extra + d


def decode_init(pe, va):
    dec = Decoder(pe, va)
    # Two prologues: a plain frame, or one that saves rbx and keeps 0 in it for the stores.
    uses_rbx = dec.peek("push_sub30")
    dec.take("push_sub30" if uses_rbx else "sub38")
    dec.take("guard")
    rbx_zero = False
    entries = []
    while True:
        preinit = dec.peek("movq_rip")
        obj_init = None
        if preinit:
            dec.take("movq_rip")
            cap_va = dec.rip(4)
            cap = dec.i32()
            if uses_rbx and dec.peek("xor_ebx"):
                dec.take("xor_ebx")
                rbx_zero = True
            if rbx_zero:
                dec.take("movq_rbx")
                size_va, size = dec.rip(), 0
                dec.take("movb_bl")
                buf_va = dec.rip()
            else:
                dec.take("movq_rip")
                size_va = dec.rip(4)
                size = dec.i32()
                dec.take("movb_rip")
                buf_va = dec.rip(1)
                dec.take("zero8")
            obj_init = (cap_va, cap, size_va, size, buf_va)
        code_va = dec.va
        if dec.peek("mov_r8d"):
            dec.take("mov_r8d")
            length = dec.i32()
        elif rbx_zero and dec.peek("lea_r8d_rbx"):
            dec.take("lea_r8d_rbx")
            length = dec.u8()
        else:
            if preinit:
                sys.exit("0x%x: pre-initialisation without assign" % dec.va)
            break
        dec.take("lea_rdx")
        str_va = dec.rip()
        dec.take("lea_rcx")
        obj_va = dec.rip()
        dec.take("call")
        target = dec.rip()
        if target != ASSIGN_FN:
            sys.exit("0x%x: call to 0x%x, not the string assign" % (dec.va, target))
        dec.take("nop")
        if obj_init and obj_init != (obj_va + 0x20, 0xF, obj_va + 0x18, 0, obj_va + 0x08):
            sys.exit("0x%x: unexpected pre-initialisation %r" % (code_va, obj_init))
        if pe.section(str_va) != ".rdata":
            sys.exit("0x%x: label 0x%x is not in .rdata" % (code_va, str_va))
        so = pe.off(str_va)
        text = pe.d[so:so + length]
        if b"\0" in text:
            sys.exit("0x%x: label 0x%x shorter than its length %d" % (code_va, str_va, length))
        entries.append(dict(obj=obj_va, str=str_va, len=length, code=code_va, preinit=preinit,
                            text=text.decode("ascii"), nul=pe.d[so + length] == 0))
    dec.take("lea_rcx")
    dtor = dec.rip()
    dec.take("add30_pop" if uses_rbx else "add38")
    dec.take("jmp")
    atexit = dec.rip()
    if not entries:
        sys.exit("0x%x: no strings" % va)
    for k, e in enumerate(entries):
        if e["obj"] != entries[0]["obj"] + k * STRING_SIZE or e["preinit"] != (k > 0):
            sys.exit("0x%x: string %d is not element %d of the array" % (va, k, k))
    return dict(init=va, end=dec.va, data=entries[0]["obj"], dtor=dtor, atexit=atexit,
                entries=entries)


def read_tables(exe):
    pe = Pe(exe)
    tables, va = [], FIRST_INIT
    while va < END_INIT:
        t = decode_init(pe, va)
        tables.append(t)
        va = (t["end"] + 15) & ~15
        pad = pe.d[pe.off(t["end"]):pe.off(va)]
        if pad.strip(b"\xcc"):
            sys.exit("0x%x: unexpected bytes between initialisers" % t["end"])
    if va != END_INIT or len(tables) != INIT_COUNT:
        sys.exit("found %d initialisers ending at 0x%x; wrong binary?" % (len(tables), va))
    if len({t["atexit"] for t in tables}) != 1:
        sys.exit("initialisers end in different jmp targets")
    half = INIT_COUNT // 2
    for a, b in zip(tables[:half], tables[half:]):
        if [e["text"] for e in a["entries"]] != [e["text"] for e in b["entries"]]:
            sys.exit("table sets differ at 0x%x / 0x%x" % (a["init"], b["init"]))
    for i, t in enumerate(tables):
        t["name"] = NAMES[i % half] + ("" if i < half else "_b")
    return tables


def cxx_string(s):
    return '"' + s.replace("\\", "\\\\").replace('"', '\\"') + '"'


def render_header(tables, exe_name):
    out = ["// Generated by port/defmodel/scripts/gen_option_tables.py from %s. Do not edit." % exe_name,
           "// Source: the 38 static initialisers at 0x140501bd0-0x1405036a0, decoded instruction by",
           "// instruction; every label is the .rdata string the initialiser passes to",
           "// std::string::assign (FUN_140005c80), with the length it passes.",
           "#pragma once",
           "",
           '#include "defmodel/options/option_table_types.hpp"',
           "",
           "namespace def::model::options {",
           ""]
    for t in tables:
        n = len(t["entries"])
        out.append("// @src %x  static initialiser of %s (atexit dtor FUN_%x)" % (t["init"], t["name"], t["dtor"]))
        out.append("// @data %x  std::string[%d]" % (t["data"], n))
        out.append("inline constexpr OptionLabel g_%s[%d] = {" % (t["name"], n))
        for e in t["entries"]:
            out.append("    {%s, 0x%X, 0x%X, 0x%X}," % (cxx_string(e["text"]), e["obj"], e["str"], e["code"]))
        out.append("};")
        out.append("")
    out.append("// Every table, in initialiser (= _initterm) order.")
    out.append("inline constexpr OptionTable kOptionTables[kOptionTableCount] = {")
    for t in tables:
        out.append('    {"%s", 0x%X, 0x%X, g_%s},' % (t["name"], t["init"], t["data"], t["name"]))
    out += ["};", "", "}  // namespace def::model::options", ""]
    return "\n".join(out)


def render_tsv(tables):
    out = ["table\tinit\tindex\tobject\tlabel_addr\tlength\tnul_terminated\tcode\tlabel"]
    for t in tables:
        for k, e in enumerate(t["entries"]):
            out.append("%s\t0x%x\t%d\t0x%x\t0x%x\t%d\t%d\t0x%x\t%s" % (
                t["name"], t["init"], k, e["obj"], e["str"], e["len"], e["nul"], e["code"], e["text"]))
    return "\n".join(out) + "\n"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--exe", default=DEFAULT_EXE)
    ap.add_argument("--check", action="store_true")
    a = ap.parse_args()
    if a.check and not os.path.exists(a.exe):
        print("skipped: %s not found" % a.exe)
        return 77
    tables = read_tables(a.exe)
    files = {HEADER: render_header(tables, pathlib.Path(a.exe).name), TSV: render_tsv(tables)}
    nstr = sum(len(t["entries"]) for t in tables)
    if a.check:
        bad = [p for p, text in files.items()
               if not p.exists() or p.read_text(encoding="ascii").replace("\r\n", "\n") != text]
        for p in bad:
            print("out of date: %s" % p.relative_to(ROOT))
        print("option tables: %d tables, %d labels, %s" % (len(tables), nstr,
              "MISMATCH" if bad else "match the binary"))
        return 1 if bad else 0
    for p, text in files.items():
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("ascii"))
        print("wrote %s" % p.relative_to(ROOT))
    print("%d tables, %d labels" % (len(tables), nstr))
    return 0


if __name__ == "__main__":
    sys.exit(main())
