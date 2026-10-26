import os, sys, json, hashlib
roots = [r"C:\Program Files\SFTC", r"C:\PROBLEM", r"C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\corpus"]
skip = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\corpus\oracle_b".lower()
out = {}
for r in roots:
    for dp, dn, fn in os.walk(r):
        if dp.lower().startswith(skip): dn[:] = []; continue
        for f in fn:
            p = os.path.join(dp, f)
            try: st = os.stat(p)
            except OSError as e: out[p] = ["err", str(e)]; continue
            v = [st.st_size, st.st_mtime_ns]
            if r.endswith("SFTC") and f.lower().endswith((".exe", ".dll", ".lic", ".ini", ".dat")) and st.st_size < 300_000_000:
                try: v.append(hashlib.sha256(open(p, "rb").read()).hexdigest())
                except OSError as e: v.append("err")
            out[p] = v
json.dump(out, open(sys.argv[1], "w"))
print(len(out))
