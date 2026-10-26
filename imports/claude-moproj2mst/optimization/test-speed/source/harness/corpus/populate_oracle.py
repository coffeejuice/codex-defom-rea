"""Fill the oracle cache with the original's result for every corpus case (resumable).

    python -I harness/corpus/populate_oracle.py --log %USERPROFILE%\\.deform-oracle\\populate.log
    python -I harness/corpus/populate_oracle.py --primary --kind dbkey -j 8

Cached cases are skipped, so a run that is stopped (or loses the machine) is simply started again.
Runner errors (timeouts) are logged and the case is retried on the next run. Progress goes to
--log as one JSON line per case and a status line every 25 cases; --status prints a summary of the
log.
"""
from __future__ import annotations

import argparse
import concurrent.futures
import json
import os
import sys
import time
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from cases import all_cases  # noqa: E402
from def_pre_oracle import Oracle, OracleError  # noqa: E402

# Cheap cases first so the cache is useful early; the 5 GB DB's 790 exports last.
ORDER = {"m": 0, "kfrw": 1, "cmd": 2, "dbkey": 3}


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--primary", action="store_true", help="SI files and the test DBs only")
    ap.add_argument("--kind", action="append")
    ap.add_argument("-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 4) // 2))
    ap.add_argument("--log", default=str(Path.home() / ".deform-oracle" / "populate.log"))
    ap.add_argument("--status", action="store_true", help="summarise the log and exit")
    a = ap.parse_args(argv)

    if a.status:
        last = {}
        for ln in Path(a.log).read_text(encoding="utf-8").splitlines():
            if ln.startswith("{"):
                d = json.loads(ln)
                last[d["id"]] = d
        c = Counter(d["state"] for d in last.values())
        print(f"{len(last)} cases logged: {dict(c)}")
        return 0

    cases = [c for c in all_cases() if (not a.primary or c.set == "primary")
             and (not a.kind or c.kind in a.kind)]
    cases.sort(key=lambda c: (ORDER[c.kind], c.set != "primary", c.cost, c.id))
    oracle = Oracle()
    log = open(a.log, "a", encoding="utf-8", buffering=1)
    log.write(f"# start {time.strftime('%Y-%m-%dT%H:%M:%S')} pid {os.getpid()} "
              f"{len(cases)} cases\n")

    def one(tc):
        t0 = time.time()
        try:
            case = tc.oracle_case()
            if oracle.lookup(case):
                return {"id": tc.id, "state": "cached"}
            r = oracle.run(case)
            return {"id": tc.id, "state": "ran", "exit_code": r.exit_code, "key": r.key,
                    "s": round(time.time() - t0, 1)}
        except (OracleError, OSError) as e:
            return {"id": tc.id, "state": "error", "error": str(e)[:300]}

    counts = Counter()
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        for n, rep in enumerate(ex.map(one, cases), 1):
            counts[rep["state"]] += 1
            if rep["state"] != "cached":
                log.write(json.dumps(rep) + "\n")
            if n % 25 == 0 or n == len(cases):
                log.write(f"# {time.strftime('%H:%M:%S')} {n}/{len(cases)} {dict(counts)}\n")
    log.write(f"# done {time.strftime('%Y-%m-%dT%H:%M:%S')} {dict(counts)}\n")
    print(dict(counts))
    return 1 if counts["error"] else 0


if __name__ == "__main__":
    sys.exit(main())
