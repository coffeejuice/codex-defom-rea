r"""Build a capture folder for DEF_PRE's external programs (diagnostic runs only).

DEF_PRE sets DEFORM_PC_PATH to its own folder at start-up (FUN_1401b8e10) and launches external
programs as  cmd /c ""<folder>\3D\<name>" args"  (or 2D\) when <folder>\3D\<name>[.EXE|.COM]
exists. This script builds OUTDIR with an unmodified copy of DEF_PRE_64.exe at the top and, in
OUTDIR\2D and OUTDIR\3D, for every program an empty marker file <name> (so DEF_PRE's existence
probe succeeds) and <name>.bat (which cmd then runs through PATHEXT). The wrapper:

  1. copies the working directory (inputs DEF_PRE wrote) to _ext_capture\<n>_<dim>_<name>\before\,
  2. runs the real, unchanged program from the DEFORM install (same subfolder) with the same
     arguments and stdin, its stdout saved to ...\stdout.txt and then typed to the caller,
  3. copies the directory again to ...\after\ and records the exit code.

DEF_PRE deletes most exchange files itself, so this is how to see them. Run the oracle with
--exe OUTDIR/DEF_PRE_64.exe, the DEFORM install folder on PATH (DLLs) and a separate --cache
(the oracle key does not include the exe location).

    python -I make_wrappers.py OUTDIR [--deform "C:/Program Files/SFTC/DEFORM/v11.0.2"]
"""
import argparse
import shutil
from pathlib import Path

# name DEF_PRE uses -> (subfolder, real file)
PROGRAMS = {
    "MESH3D_GUI": None, "BRKMSH": None, "CHKVOL": None, "PDA2KEY3": None, "STB2KEY3": None,
    "DEF_REM3": None, "DEF_AMG": None, "DEF_AMG3_GUI": None, "DEF_REM3_GUI": None, "DEF_INT": None,
    "DEF_FRZ": None, "DEF_FRZ3": None, "DEF_CVT3": None, "DEF_IGS": None, "FIXGEO": None,
    "DEF_MORP2": None, "DEF_MORP3": None, "DEF_LOCREM3": None, "COATINGMESH": None,
    "DEF_COG_AMG": None, "AMG_DXT": None, "AMG_DXT3": None,
}

TEMPLATE = r"""@echo off
setlocal
if not exist _ext_capture mkdir _ext_capture
set /a N=0
:next
set /a N+=1
if exist "_ext_capture\%N%_{dim}_{name}" goto next
set C=_ext_capture\%N%_{dim}_{name}
mkdir "%C%\before"
for %%F in (*) do copy /y "%%F" "%C%\before\" >nul
echo %* > "%C%\args.txt"
"{real}" %* > "%C%\stdout.txt"
set RC=%ERRORLEVEL%
echo %RC% > "%C%\exit_code.txt"
mkdir "%C%\after"
for %%F in (*) do copy /y "%%F" "%C%\after\" >nul
type "%C%\stdout.txt"
exit /b %RC%
"""


def find(deform: Path, name: str, sub: str) -> Path | None:
    for ext in (".exe", ".EXE", ".com", ".COM"):
        p = deform / sub / (name + ext)
        if p.exists():
            return p
    return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    ap.add_argument("--deform", default="C:/Program Files/SFTC/DEFORM/v11.0.2")
    a = ap.parse_args()
    out = Path(a.out)
    deform = Path(a.deform)
    out.mkdir(parents=True, exist_ok=True)
    shutil.copy2(deform / "DEF_PRE_64.exe", out / "DEF_PRE_64.exe")
    for dim in ("2D", "3D"):
        (out / dim).mkdir(exist_ok=True)
        for name in PROGRAMS:
            real = find(deform, name, dim)
            if real is None:
                continue
            (out / dim / name).write_bytes(b"")
            (out / dim / f"{name}.bat").write_text(
                TEMPLATE.format(name=name, dim=dim, real=str(real).replace("/", "\\")))
            print(f"{dim}/{name}.bat -> {real}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
