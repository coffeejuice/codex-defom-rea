"""Expected output for the kw-g1a tier-1 tests, taken from the original DEF_PRE.

For every cNN_*.KEY here: run the oracle case "KFREAD IN.KEY / KFWRIT OUT.KEY" (a fresh model,
the KEY read, then the KEY written; tools/oracle) and store
  cNN_*.expected   OUT.KEY from the "Process Definition" banner up to the "Iteration Controls"
                   banner (the two sections kw-g1a writes)
  cNN_*.msgs       the KEY reader's messages from DEF_PRE's stdout ("ERROR: Keyword ..." and
                   "WARNING: Keyword ..." lines), in order, without the KFREAD command's own
                   "Reading Error" (the reader returns its last error code to the command)

Usage: python -I make_expected.py [--offline]   (from any directory; needs the repo's tools/oracle)
"""
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[4]
sys.path.insert(0, str(REPO / "tools" / "oracle"))
from def_pre_oracle import Case, Input, Oracle  # noqa: E402

START = "*\n*  Process Definition\n"
END = "*\n*  Iteration Controls\n"


def main() -> int:
    oracle = Oracle(offline="--offline" in sys.argv)
    bad = 0
    for key in sorted(HERE.glob("c*.KEY")):
        case = Case.keyfile("KFREAD\nIN.KEY\nKFWRIT\nOUT.KEY\n", [Input("IN.KEY", data=key.read_bytes())])
        case.label = "kw-g1a/" + key.name
        r = oracle.run(case)
        out = r.read_text("OUT.KEY").replace("\r\n", "\n")
        i, j = out.find(START), out.find(END)
        if i < 0 or j < i:
            print(key.name, "no section slice in OUT.KEY", file=sys.stderr)
            bad += 1
            continue
        key.with_suffix(".expected").write_text(out[i:j], encoding="latin-1", newline="\n")
        msgs = [ln for ln in r.stdout.decode("latin-1").replace("\r\n", "\n").split("\n")
                if ln.startswith(("ERROR: Keyword", "WARNING: Keyword"))
                and not ln.startswith("ERROR: Keyword KFREAD ")]
        key.with_suffix(".msgs").write_text("".join(m + "\n" for m in msgs), encoding="latin-1",
                                            newline="\n")
        print(key.name, "cached" if r.cached else "ran", r.exit_code, len(msgs))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
