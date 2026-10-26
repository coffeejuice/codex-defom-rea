import json, sys
a = json.load(open(sys.argv[1])); b = json.load(open(sys.argv[2]))
for k in sorted(set(a) | set(b)):
    if a.get(k) != b.get(k): print("CHANGED" if k in a and k in b else ("ADDED" if k in b else "REMOVED"), k, a.get(k), b.get(k))
