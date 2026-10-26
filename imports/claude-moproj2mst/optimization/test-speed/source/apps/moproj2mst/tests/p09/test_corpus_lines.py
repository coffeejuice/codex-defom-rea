"""P09 corpus lines: the P09 emitters' line forms (tools/moproj2mst/forms/p09.py) in every corpus MST.
Added with the shared checker (claude/m2m-tools); P09 owns this file and its forms from here on."""
import os, pathlib, sys

sys.dont_write_bytecode = True
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[4] / "tools" / "moproj2mst"))
import corpus_lines  # noqa: E402

corpus = os.environ.get("M2M_CORPUS")
if not corpus:
    print("SKIP corpus lines: M2M_CORPUS unset")
    sys.exit(77)
sys.exit(corpus_lines.main(["p09", "--corpus", corpus, "--quiet"]))
