"""Build/inspect the pinned WORKER-only fixture; never execute it. Native HOLD."""
from __future__ import annotations

import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess

HERE = Path(__file__).resolve().parent
PE_GATE_SPEC = importlib.util.spec_from_file_location("cp316_pe_gate", HERE / "build_cp302_fixture.py")
if PE_GATE_SPEC is None or PE_GATE_SPEC.loader is None:
    raise ImportError("CP302 PE gate could not be loaded")
pe_gate = importlib.util.module_from_spec(PE_GATE_SPEC)
PE_GATE_SPEC.loader.exec_module(pe_gate)

SOURCE = HERE / "fixtures" / "cp316" / "worker.c"
SOURCE_SHA256 = "e657f3f4bcc69732238a50094a7766c91f801c3b05a38db88cd2795a63f48703"
OUTPUT = Path(r"C:\PROBLEM\s316\worker.exe")
OBJECT = OUTPUT.with_suffix(".obj")
RECEIPT = HERE.parent / "analysis" / "cp316-worker-fixture-build.json"
COMPILE_FLAGS = ("/nologo", "/W4", "/WX", "/O1", "/MT", "/std:c11", "/c",
                 "/Fo" + str(OBJECT), str(SOURCE))
LINK_FLAGS = ("/nologo", "/MACHINE:X64", "/SUBSYSTEM:CONSOLE",
              "/OUT:" + str(OUTPUT), str(OBJECT), "kernel32.lib")
ALLOWED_IMPORTS = frozenset({"KERNEL32.DLL"})
NATIVE_ENTRY = "HARD HOLD"
OPTION_ENVIRONMENT = frozenset({"CL", "_CL_", "LINK", "_LINK_"})


def compiler_environment() -> dict[str, str]:
    """Preserve SDK lookup while removing implicit compiler/linker options."""
    return {name: value for name, value in os.environ.items()
            if name.upper() not in OPTION_ENVIRONMENT}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_source() -> None:
    pe_gate.check_plain_path(SOURCE)
    if SOURCE.is_symlink() or not SOURCE.is_file() or sha256(SOURCE) != SOURCE_SHA256:
        raise ValueError("CP316 source is missing, redirected, or changed")


def inspect_pe(path: Path) -> dict:
    # Reuse the bounded KERNEL32-only PE gate; rejects TLS/delay imports.
    return pe_gate.inspect_pe(path)


def build() -> dict:
    check_source()
    for path in (SOURCE, OUTPUT, OBJECT, RECEIPT):
        pe_gate.check_plain_path(path)
    if any(path.exists() for path in (OUTPUT, OBJECT, RECEIPT)):
        raise FileExistsError("CP316 output, object, or receipt already exists")
    if len(str(OUTPUT)) >= 100:
        raise ValueError("CP316 output path too long")
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
    child_environment = compiler_environment()
    subprocess.run(compile_command, check=True, cwd=OUTPUT.parent, timeout=120, env=child_environment)
    subprocess.run(link_command, check=True, cwd=OUTPUT.parent, timeout=120, env=child_environment)
    result = inspect_pe(OUTPUT)
    receipt = {"source": str(SOURCE), "source_sha256": SOURCE_SHA256,
               "compiler": str(cl), "linker": str(link), **tool_identity,
               "compile_command": compile_command, "link_command": link_command,
               "option_environment_removed": sorted(OPTION_ENVIRONMENT),
               "output": str(OUTPUT), "pe": result,
               "execution": "HARD HOLD: builder never starts WORKER; source has no child-launch mode",
               "transport_limit": "Supervisor must prove inherited client handles were opened FILE_FLAG_OVERLAPPED and pass a pre-create absolute GetTickCount64 deadline.",
               "identity_limit": "Paths can change after inspection; retain a non-writable/non-deletable image identity through creation and bind exact bytes before use."}
    RECEIPT.parent.mkdir(parents=True, exist_ok=True)
    RECEIPT.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return receipt


if __name__ == "__main__":
    print(json.dumps(build(), indent=2))
