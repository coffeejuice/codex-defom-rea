"""Reproducibly build and inspect the fixed CP282 benign native startup child."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess

ROOT = Path(__file__).resolve().parent.parent
SOURCE = ROOT / "admin/native_color_entry_fixture.cpp"
OUT = ROOT / "build/cp282-native-startup-fixture"
VS = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat")
CL = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\cl.exe")


def parse_pe(data: bytes) -> dict:
    """Read-only, bounds-checked PE32+ export and raw-section parser."""
    def u16(off): return struct.unpack_from("<H", data, off)[0]
    def u32(off): return struct.unpack_from("<I", data, off)[0]
    if data[:2] != b"MZ": raise ValueError("missing MZ")
    pe = u32(0x3c)
    if data[pe:pe+4] != b"PE\0\0": raise ValueError("missing PE signature")
    nsec, opt_size = u16(pe+6), u16(pe+20)
    opt = pe+24
    if u16(opt) != 0x20b: raise ValueError("fixture must be PE32+")
    if u16(pe+4) != 0x8664: raise ValueError("fixture must be AMD64")
    image_size, export_rva = u32(opt+56), u32(opt+112)
    sec = opt+opt_size
    sections = []
    for i in range(nsec):
        p = sec+i*40
        name = data[p:p+8].split(b"\0", 1)[0].decode("ascii")
        virtual_size, va, raw_size, raw_ptr = struct.unpack_from("<IIII", data, p+8)
        characteristics = u32(p+36)
        sections.append(dict(name=name, va=va, virtual_size=virtual_size,
                             raw_size=raw_size, raw_ptr=raw_ptr,
                             characteristics=characteristics))
    def offset(rva, length=1):
        matches = [s for s in sections if s["va"] <= rva and
                   rva+length <= s["va"]+s["raw_size"] and
                   s["raw_ptr"]+rva-s["va"]+length <= len(data)]
        if len(matches) != 1: raise ValueError(f"RVA is not uniquely raw-backed: {rva:#x}")
        return matches[0]["raw_ptr"]+rva-matches[0]["va"]
    e = offset(export_rva, 40)
    nfunc, nname = u32(e+20), u32(e+24)
    funcs, names, ords = u32(e+28), u32(e+32), u32(e+36)
    exports = {}
    for i in range(nname):
        nr = u32(offset(names+i*4, 4)); p = offset(nr)
        end = data.find(b"\0", p)
        if end < 0: raise ValueError("unterminated export name")
        name = data[p:end].decode("ascii")
        ordinal = u16(offset(ords+i*2, 2))
        if ordinal >= nfunc: raise ValueError("bad export ordinal")
        exports[name] = u32(offset(funcs+ordinal*4, 4))
    if "EntryProbe" not in exports: raise ValueError("EntryProbe export missing")
    rva = exports["EntryProbe"]
    section = next((s for s in sections if s["va"] <= rva < s["va"]+s["virtual_size"]), None)
    if not section or not (section["characteristics"] & 0x20000000):
        raise ValueError("EntryProbe is not in executable section")
    probe = data[offset(rva, 16):offset(rva, 16)+16]
    return dict(machine="AMD64", image_size=image_size, sections=sections,
                exports=exports, entry_rva=rva, entry_bytes=probe.hex())


def main() -> int:
    if not SOURCE.is_file() or not CL.is_file() or not VS.is_file():
        raise SystemExit("HOLD: fixed source or installed MSVC Build Tools not found")
    OUT.mkdir(parents=True, exist_ok=True)
    env_result = subprocess.run(f'cmd.exe /d /s /c ""{VS}" >nul && set"',
                                capture_output=True, text=True, check=True)
    env = dict(os.environ)
    for line in env_result.stdout.splitlines():
        key, sep, value = line.partition("=")
        if sep and key:
            for old in list(env):
                if old.lower() == key.lower(): del env[old]
            env[key] = value
    exe = OUT / "native_color_entry_fixture.exe"
    command = [str(CL), "/nologo", "/std:c++17", "/EHsc", "/O2", "/MT", "/GS", "/Gy-",
               str(SOURCE), "/link", "/INCREMENTAL:NO", "/OPT:NOICF", "/OPT:NOREF", "/Brepro",
               "/EXPORT:EntryProbe", f"/OUT:{exe}"]
    builds = []
    first_hash = None
    for iteration in range(2):
        run = subprocess.run(command, cwd=OUT, env=env, capture_output=True, text=True)
        row = {"iteration": iteration + 1, "command": command, "exit": run.returncode,
               "stdout": run.stdout, "stderr": run.stderr}
        if run.returncode:
            builds.append(row)
            (OUT / "build.log.json").write_text(json.dumps({"builds": builds}, indent=2)+"\n", encoding="utf-8")
            raise SystemExit(run.stdout+run.stderr)
        row["sha256"] = hashlib.sha256(exe.read_bytes()).hexdigest()
        builds.append(row)
        if iteration == 0: first_hash = row["sha256"]
    reproducible = first_hash == builds[1]["sha256"]
    (OUT / "build.log.json").write_text(json.dumps({"builds": builds,
        "reproducible_two_builds": reproducible}, indent=2)+"\n", encoding="utf-8")
    if not reproducible: raise SystemExit("two consecutive /Brepro builds did not match")
    blob = exe.read_bytes()
    pe = parse_pe(blob)
    version_command = [str(CL), "/nologo", "/Bv", "/Zs", str(SOURCE)]
    version = subprocess.run(version_command, cwd=OUT, env=env, capture_output=True, text=True)
    version_text = version.stdout+version.stderr
    match = re.search(r"Compiler Passes:\s*(.*?)(?:\n\s*\n|\Z)", version_text, re.S)
    if version.returncode != 0 or not match:
        raise SystemExit("unable to collect clean compiler version: "+version_text[:1000])
    receipt = dict(schema="cp282-native-color-entry-fixture-v1", exe=str(exe),
        size=len(blob), sha256=hashlib.sha256(blob).hexdigest(),
        source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
        compiler=str(CL), compiler_sha256=hashlib.sha256(CL.read_bytes()).hexdigest(),
        compiler_version=match.group(1).strip(), compiler_version_command=version_command,
        builds=builds, reproducible_two_builds=reproducible, command=command, **pe)
    (OUT / "fixture.json").write_text(json.dumps(receipt, indent=2)+"\n", encoding="utf-8")
    print(json.dumps({k: receipt[k] for k in ("exe", "size", "sha256", "compiler_sha256", "entry_rva", "entry_bytes")}))
    return 0


if __name__ == "__main__": raise SystemExit(main())
