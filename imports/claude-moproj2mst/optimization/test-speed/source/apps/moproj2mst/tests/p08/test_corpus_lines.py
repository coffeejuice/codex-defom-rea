"""P08 corpus lines: the P08 emitters' line forms (tools/moproj2mst/forms/p08.py) in every corpus MST."""
import os, pathlib, sys

sys.dont_write_bytecode = True
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[4] / "tools" / "moproj2mst"))
import corpus_lines  # noqa: E402

corpus = os.environ.get("M2M_CORPUS")
if not corpus:
    print("SKIP corpus lines: M2M_CORPUS unset")
    sys.exit(77)
sys.exit(corpus_lines.main(["p08", "--corpus", corpus, "--quiet"]))
