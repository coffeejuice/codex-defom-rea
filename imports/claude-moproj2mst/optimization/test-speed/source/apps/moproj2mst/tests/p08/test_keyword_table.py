"""P08 keyword table: src/mst/keyword_table.inc equals the table in the DEF_GUI_MO_64.exe copy
(tools/gen_keyword_table.py --check; exit 77 when the exe copy is missing)."""
import os, pathlib, sys

sys.dont_write_bytecode = True
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[2] / "tools"))
import gen_keyword_table  # noqa: E402

reference = os.environ.get("M2M_REFERENCE_EXE")
if not reference:
    print("SKIP keyword-table reference: M2M_REFERENCE_EXE unset")
    sys.exit(77)
sys.argv = [sys.argv[0], "--check", "--exe", reference]
gen_keyword_table.main()
