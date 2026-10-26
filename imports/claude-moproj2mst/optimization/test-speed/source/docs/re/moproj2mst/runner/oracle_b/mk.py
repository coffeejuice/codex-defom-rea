"""mk.py <spec.json>: build oracle-B scratch projects from corpus base cases.
spec: list of {"case": dir, "base": corpus case, "append": [{"type", "name", "objs_from": op id|null,
"obj_types": [..]|null, "group": {"kind": "SfOprCycle"|"SfOprGroup", "cols": [b, e], "count": n}|null}]}"""
import json, os, re, shutil, sys
CORPUS = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\corpus"
ROOT = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b"
EXP = ('   <Expansion_Items Number="5">\n    <Expansion_Item Type="Text" Label="  Process"/>\n'
       '    <Expansion_Item LineColor="0, 0, 0, 255" Type="Line" LineStyle="Solid"/>\n    <Expansion_Item Type="Space"/>\n'
       '    <Expansion_Item Type="Space"/>\n    <Expansion_Item LineColor="0, 0, 0, 255" Type="Line" LineStyle="Solid"/>\n   </Expansion_Items>\n')
def op_blocks(s):
    return [(m.start(), m.end(), m.group()) for m in re.finditer(r"  <Operation>\n.*?\n  </Operation>\n", s, re.S)]
def build(c):
    src = os.path.join(CORPUS, c["base"]); dst = os.path.join(ROOT, c["case"])
    if os.path.exists(dst):  # the GUI may hold the folder as its cwd: empty it instead of removing it
        for f in os.listdir(dst):
            q = os.path.join(dst, f); shutil.rmtree(q) if os.path.isdir(q) else os.remove(q)
    shutil.copytree(src, dst, dirs_exist_ok=True)
    mp = [f for f in os.listdir(dst) if f.lower().endswith(".moproj")][0]
    for f in os.listdir(dst):
        if f.upper().endswith(".MST"): os.rename(os.path.join(dst, f), os.path.join(dst, f + ".oracleA"))
    p = os.path.join(dst, mp); s = open(p, encoding="utf-8").read()
    ops = op_blocks(s); ids = [int(re.search(r"<Id>(\d+)</Id>", b).group(1)) for _, _, b in ops]
    nid = max(ids) + 1; new_ops = ""; new_items = []
    items = re.findall(r"  <DiagramItem .*?\n  </DiagramItem>\n", s, re.S)
    ncol = max([int(x) for x in re.findall(r'<CellPos Row="0" Col="(\d+)"/>', s)] + [-1]) + 1
    for a in c["append"]:
        objs = '   <Objects Number="0"/>\n'
        if a.get("objs_from") is not None:
            b = ops[ids.index(a["objs_from"])][2]
            objs = re.search(r"   <Objects Number=.*?(?:/>\n|\n   </Objects>\n)", b, re.S).group()
            if a.get("obj_types") is not None:
                obl = re.findall(r"    <Object .*?\n    </Object>\n", objs, re.S)
                keep = [o for o in obl if int(re.search(r'ObjectType="(\d+)"', o).group(1)) in a["obj_types"]]
                objs = '   <Objects Number="%d">\n%s   </Objects>\n' % (len(keep), "".join(keep))
        new_ops += ("  <Operation>\n   <Name>%s</Name>\n   <Id>%d</Id>\n   <Type>%s</Type>\n   <OprFlag>1</OprFlag>\n"
                    '   <Equipment Id="-1"/>\n   <Lubricant InterObjectMethod="User" Id="-1"/>\n%s'
                    '   <Properties Number="0"/>\n  </Operation>\n') % (a["name"], nid, a["type"], objs)
        g = a.get("group")
        if g:
            item = ('  <DiagramItem Expanded="yes" Operation_Id="%d">\n   <ClassName>%s</ClassName>\n   <ItemName>%s</ItemName>\n'
                    '   <RectF>\n    <x>0</x>\n    <y>0</y>\n    <height>0</height>\n    <width>0</width>\n   </RectF>\n'
                    '   <GroupInfo BeginCol="%d" EndCol="%d" BeginRow="0" EndRow="0"/>\n%s   <Connectors Number="1">\n    <Connections Number="0"/>\n   </Connectors>\n%s  </DiagramItem>\n'
                    ) % (nid, a["type"], a["name"], g["cols"][0], g["cols"][1],
                         ('   <ItemData CycleCount="%d"/>\n' % g["count"]) if "count" in g else "", EXP)
            new_items.append(("group", item))
        else:
            item = ('  <DiagramItem Expanded="yes" Operation_Id="%d">\n   <ClassName>%s</ClassName>\n   <ItemName>%s</ItemName>\n'
                    '   <RectF>\n    <x>%d</x>\n    <y>0</y>\n    <height>0</height>\n    <width>0</width>\n   </RectF>\n'
                    '   <CellPos Row="0" Col="%d"/>\n   <Connectors Number="1">\n    <Connections Number="0"/>\n   </Connectors>\n%s  </DiagramItem>\n'
                    ) % (nid, a["type"], a["name"], 205 * ncol, ncol, EXP)
            new_items.append(("cell", item)); ncol += 1
        nid += 1
    # chain: the cell items in order; connect the last existing cell item to the next one
    allitems = items + [i for _, i in new_items]
    cells = [k for k, it in enumerate(allitems) if "<CellPos" in it]
    cells.sort(key=lambda k: int(re.search(r'Col="(\d+)"', allitems[k]).group(1)))
    for a_, b_ in zip(cells, cells[1:]):
        allitems[a_] = re.sub(r'    <Connections Number="0"/>\n', '    <Connections Number="1">\n     <ItemIndex>%d</ItemIndex>\n    </Connections>\n' % b_, allitems[a_], count=1)
    last = ops[-1][1]
    s = s[:last] + new_ops + s[last:]
    s = re.sub(r'<Operation_List Number="\d+">', '<Operation_List Number="%d">' % (len(ops) + len(c["append"])), s)
    first = s.index("  <DiagramItem "); endi = s.rindex("  </DiagramItem>\n") + len("  </DiagramItem>\n")
    s = s[:first] + "".join(allitems) + s[endi:]
    s = re.sub(r'(<Sub_Project Active="\d+" Number=")\d+', lambda m: m.group(1) + str(len(allitems)), s, count=1)
    open(p, "w", encoding="utf-8", newline="\n").write(s)
    D = dst.replace("\\", "/")
    for rel, body in c.get("files", {}).items():
        q = os.path.join(dst, rel); os.makedirs(os.path.dirname(q), exist_ok=True)
        if body.startswith("copy:"): shutil.copyfile(os.path.join(dst, body[5:]), q)
        else: open(q, "w", newline="\n").write(body.replace("{DIR}", D))
    print(c["case"], p)
for c in json.load(open(sys.argv[1])):
    if len(sys.argv) < 3 or c["case"] in sys.argv[2:]: build(c)
