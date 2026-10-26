"""The shared corpus line checker finds planted errors (tools/moproj2mst/corpus_lines.py --self-test)."""
import pathlib, sys

sys.dont_write_bytecode = True
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[4] / "tools" / "moproj2mst"))
import corpus_lines  # noqa: E402

sys.exit(corpus_lines.main(["--self-test"]))
