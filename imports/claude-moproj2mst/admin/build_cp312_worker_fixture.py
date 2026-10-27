"""Build and inspect the fixed CP312 WORKER fixture; never start it or a child."""
from __future__ import annotations

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
PE_GATE_PATH = HERE / "build_cp302_fixture.py"
PE_GATE_SPEC = importlib.util.spec_from_file_location("cp312_pe_gate", PE_GATE_PATH)
if PE_GATE_SPEC is None or PE_GATE_SPEC.loader is None:
    raise ImportError("CP302 PE gate could not be loaded")
pe_gate = importlib.util.module_from_spec(PE_GATE_SPEC)
PE_GATE_SPEC.loader.exec_module(pe_gate)

SOURCE = HERE / "fixtures" / "cp312" / "worker.c"
SOURCE_SHA256 = "7a6eb4d1ded0ef6fb7961a9bda3285ae7c63bdd96bc4a469ae92076bb522bc15"
OUTPUT = Path(r"C:\PROBLEM\s312\worker.exe")
OBJECT = OUTPUT.with_suffix(".obj")
RECEIPT = HERE.parent / "analysis" / "cp312-worker-fixture-build.json"
COMPILE_FLAGS = ("/nologo", "/W4", "/WX", "/O1", "/MT", "/std:c11", "/c",
                 "/Fo" + str(OBJECT), str(SOURCE))
LINK_FLAGS = ("/nologo", "/MACHINE:X64", "/SUBSYSTEM:CONSOLE",
              "/OUT:" + str(OUTPUT), str(OBJECT), "bcrypt.lib")
ALLOWED_IMPORTS = frozenset({"KERNEL32.DLL", "BCRYPT.DLL"})


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_source() -> None:
    pe_gate.check_plain_path(SOURCE)
    if SOURCE.is_symlink() or not SOURCE.is_file() or sha256(SOURCE) != SOURCE_SHA256:
        raise ValueError("CP312 source is missing, redirected, or changed")


def inspect_pe(path: Path) -> dict:
    # Reuse CP302's bounded PE parser with CP312's explicit BCrypt import.
    previous = pe_gate.ALLOWED_IMPORTS
    try:
        pe_gate.ALLOWED_IMPORTS = ALLOWED_IMPORTS
        result = pe_gate.inspect_pe(path)
    finally:
        pe_gate.ALLOWED_IMPORTS = previous
    if "BCRYPT.DLL" not in result["imports"]:
        raise ValueError("CP312 image is missing the pinned-image hash API")
    return result


def build() -> dict:
    check_source()
    for path in (SOURCE, OUTPUT, OBJECT, RECEIPT):
        pe_gate.check_plain_path(path)
    if any(path.exists() for path in (OUTPUT, OBJECT, RECEIPT)):
        raise FileExistsError("CP312 output, object, or receipt already exists")
    if len(str(OUTPUT)) >= 100:
        raise ValueError("CP312 output path too long")
    tools = Path(os.environ["VCToolsInstallDir"])
    if not tools.is_absolute():
        raise ValueError("VCToolsInstallDir must be absolute")
    cl = tools / "bin" / "Hostx64" / "x64" / "cl.exe"
    link = tools / "bin" / "Hostx64" / "x64" / "link.exe"
    for tool in (cl, link):
        pe_gate.check_plain_path(tool)
        if not tool.is_file():
            raise FileNotFoundError(tool)
    tool_identity = {"compiler_sha256": sha256(cl), "linker_sha256": sha256(link)}
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    compile_command = [str(cl), *COMPILE_FLAGS]
    link_command = [str(link), *LINK_FLAGS]
    subprocess.run(compile_command, check=True, cwd=OUTPUT.parent, timeout=120)
    subprocess.run(link_command, check=True, cwd=OUTPUT.parent, timeout=120)
    result = inspect_pe(OUTPUT)
    receipt = {"source": str(SOURCE), "source_sha256": SOURCE_SHA256,
               "compiler": str(cl), "linker": str(link), **tool_identity,
               "compile_command": compile_command, "link_command": link_command,
               "output": str(OUTPUT), "pe": result,
               "execution": "HARD HOLD: builder never starts WORKER or CP302 child",
               "identity_limit": "Tool and image paths can be replaced after inspection; bind a fresh receipt and exact bytes before use."}
    RECEIPT.parent.mkdir(parents=True, exist_ok=True)
    RECEIPT.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return receipt


if __name__ == "__main__":
    print(json.dumps(build(), indent=2))
