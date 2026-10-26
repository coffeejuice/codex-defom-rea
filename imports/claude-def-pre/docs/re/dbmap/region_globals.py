"""Map DEF_PRE_64 region-index globals (DAT_1406e8xxx) to MAR/A region names.

Source: FUN_1401c6e60 (MAR 2D), FUN_1401c7080 (MAR 3D), FUN_1401c72a0 (A 2D), FUN_1401c77c0 (A 3D),
selected by FUN_1401c7d40(dim). A global holds a 0-based index into the model's address table:
MAR table at model+0x1b90 (70 starts, then 70 widths), A table at model+0x1dc0 (140 starts, then 140 widths).
usage: region_globals.py loader_closure.c address_region_names.json > region_globals.tsv
"""
import json, re, sys

def body(src, addr):
    m = re.search(r"^// ===== \S+ @ %s .*?(?=^// ===== )" % addr, src, re.S | re.M)
    return dict((g, int(v, 0)) for g, v in re.findall(r"(DAT_[0-9a-f]+) = (0x[0-9a-f]+|\d+);", m.group(0)))

src = open(sys.argv[1]).read()
names = json.load(open(sys.argv[2]))
mar3 = list(names["mar_3d_v_gt_10000"]); a3 = list(names["a_3d_v_gt_10000"])
mar2 = list(names["mar_2d_v_gt_10000"]); a2 = list(names["a_2d_v_gt_10000"])
# 3D v>10000: 0-based slot 47 holds SIZESH (LLSIZ), slot 48 the face radiation flux (LLRFL); see db-format-spec 6.1.
# With the swap, every global names the same region in 2D and 3D.
if a3[47] == "LLRFL" and a3[48] == "LLSIZ":
    a3[47], a3[48] = "LLSIZ", "LLRFL"
sets = [("MAR", 70, {"2": (body(src, "1401c6e60"), mar2), "3": (body(src, "1401c7080"), mar3)}),
        ("A", 140, {"2": (body(src, "1401c72a0"), a2), "3": (body(src, "1401c77c0"), a3)})]
print("global\tarray\tidx2d\tname2d\tidx3d\tname3d")
for arr, n, d in sets:
    keys = sorted(set(d["2"][0]) | set(d["3"][0]))
    for g in keys:
        row = [g, arr]
        for dim in "23":
            vals, lst = d[dim]
            if g not in vals:
                row += ["", ""]; continue
            i = vals[g]
            nm = (lst[i] if i < len(lst) else "?%d" % i) if i < n else (("width:" + lst[i - n]) if i - n < len(lst) else "?%d" % i)
            row += [str(i), nm]
        print("\t".join(row))
