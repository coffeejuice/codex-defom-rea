"""mk2.py <case>...: batch-2 cases.

kind "mb"/"fe": a GUI-saved batch-1 case copied and its MultiBlow / FormingExpress property tree
edited (mbprops.py). kind "first": a corpus case whose first operation is turned into another class
(it keeps that operation's objects, ActionKeywords and NSTART settings; properties left to the GUI).
"""
import os, re, shutil, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mbprops

ROOT = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\oracle_b"
CORPUS = r"C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\corpus"
T = "true"

# HeatTreatWizard.HTWZ as in gen_specs.py, first written step 1
HTWZ1 = ("HEAT_TREATMENT_SPECS_FILE_V3\nNO_MEDIA  1\nMEDIA   1\nWater quench\nNO_ZONES   1\nZONE   0\n"
         "CNVCOF 0 5000.0\nRADIATION 0\nACVCOF 0 0.0\nNO_ECTMFN  0\nNO_PROCS  3\n"
         "PROC 1 120.0 1 0 0 0 0 0 0 {DIR}/UserHT.KEY 0\nPROC 2 60.0 1 0 0 0 0 0 0\n"
         "PROC 3 30.0 1 0 0 0 0 0 0 None 0\nSTART_OPTNO  1\n")

CASES = {
    # MultiBlow2D: 2 heats x 2 blows, rotation/flip on with flip at (1,2), heating with reheat at (2,1), (2,2), strain reset
    "78_B_MB2X": ("74_B_MB2", "mb",
                  {"UseWorkpieceRotation": T, "UseHeating": T, "HeatingTemperature": "1150", "InitStrain": T},
                  [[{}, {"BlowFlip": T}], [{"BlowDwellTime": "2", "BlowReheat": T}, {"BlowReheat": T}]], ["1100", "1050"]),
    # MultiBlow3D: NDTMP + strain reset; rotation without self-center about a skew axis (1,2), about -Y with self-center
    # (2,2), rotate flag with angle 0 (2,1: error text)
    "79_B_MB3X": ("75_B_MB3", "mb",
                  {"UseWorkpieceRotation": T, "InitTemp": T, "InitStrain": T},
                  [[{}, {"BlowFlip": T, "BlowRotAngle": "90", "BlowRotSelfCenter": "false", "BlowRotCenterZ": "10",
                         "BlowRotAxisX": "1", "BlowRotAxisY": "1"}],
                   [{"BlowFlip": T, "BlowRotAxisZ": "1"}, {"BlowFlip": T, "BlowRotAngle": "45", "BlowRotAxisY": "-1"}]],
                  ["1100", "1050"]),
    # fresh-process reruns for P14: 79's input again, and a 2D case with UseRelativeEnergy on
    "79F_B_MB3X": ("75_B_MB3", "mb",
                   {"UseWorkpieceRotation": T, "InitTemp": T, "InitStrain": T},
                   [[{}, {"BlowFlip": T, "BlowRotAngle": "90", "BlowRotSelfCenter": "false", "BlowRotCenterZ": "10",
                          "BlowRotAxisX": "1", "BlowRotAxisY": "1"}],
                    [{"BlowFlip": T, "BlowRotAxisZ": "1"}, {"BlowFlip": T, "BlowRotAngle": "45", "BlowRotAxisY": "-1"}]],
                   ["1100", "1050"]),
    "92_B_MB2REL": ("74_B_MB2", "mb", {"UseRelativeEnergy": T}, [[{}]], ["100"]),
    # FormingExpress, blow table (express) on: 2 blows x 3 steps, gap 2 at (1,2), drop at (1,3), strain reset
    "82_B_FE3X": ("77_B_FE3", "fe", {"Equipment": "3", "UseBlowTable": T, "InitStrain": T},
                  [[{}, {"BlowDwellTime": "2"}, {"BlowReheat": T}], [{}, {}, {}]], ["1100", "1050"]),
    # FormingExpress2D, express: flip at (1,2), drop at (2,1)
    "83_B_FE2X": ("76_B_FE2", "fe", {"Equipment": "4", "UseBlowTable": T},
                  [[{}, {"BlowFlip": T}], [{"BlowReheat": T}, {}]], ["1100", "1050"]),
    "84_B_HT2F": ("51_MO2_Operation_Cycles2", "first", "SfOprHeatTreat2D", "Heat Treat",
                  {"Operations/Task00001/HeatTreatWizard.HTWZ": HTWZ1, "UserHT.KEY": "copy:Operations/Task00001/SimCtrl.KEY"}),
    "85_B_CNVF": ("51_MO2_Operation_Cycles2", "first", "SfOprConvert2Dto3D", "2D to 3D", {}),
    "86_B_PRE3F": ("57_Gear_blank", "first", "SfOprPre3D", "Pre", {}),
    "87_B_HTFF": ("57_Gear_blank", "first", "SfOprHTFurnace", "HT Furnace", {}),
    "88_B_FE3F": ("57_Gear_blank", "first", "SfOprFormingExpress", "Forming Express", {}),
}


def first(d, typ, name, files):
    mp = [f for f in os.listdir(d) if f.lower().endswith(".moproj")][0]
    p = os.path.join(d, mp); s = open(p, encoding="utf-8").read()
    i = s.index("  <Operation>\n"); j = s.index("  </Operation>\n", i)
    blk = s[i:j]
    old_type = re.search(r"<Type>([^<]*)</Type>", blk).group(1)
    blk = re.sub(r"<Name>[^<]*</Name>", "<Name>%s</Name>" % name, blk, count=1)
    blk = blk.replace("<Type>%s</Type>" % old_type, "<Type>%s</Type>" % typ)
    blk = re.sub(r"   <Properties Number=.*?\n   </Properties>\n", '   <Properties Number="0"/>\n', blk, flags=re.S)
    s = s[:i] + blk + s[j:]
    k = s.index('<DiagramItem Expanded="'); k2 = s.index("</DiagramItem>", k)
    it = s[k:k2]
    assert 'Operation_Id="1"' in it
    it = re.sub(r"<ClassName>[^<]*</ClassName>", "<ClassName>%s</ClassName>" % typ, it)
    it = re.sub(r"<ItemName>[^<]*</ItemName>", "<ItemName>%s</ItemName>" % name, it)
    s = s[:k] + it + s[k2:]
    open(p, "w", encoding="utf-8", newline="\n").write(s)
    for rel, body in files.items():
        q = os.path.join(d, rel); os.makedirs(os.path.dirname(q), exist_ok=True)
        if body.startswith("copy:"): shutil.copyfile(os.path.join(d, body[5:]), q)
        else: open(q, "w", newline="\n").write(body.replace("{DIR}", d.replace("\\", "/")))


def build(cid):
    spec = CASES[cid]; src, kind = spec[0], spec[1]
    s = os.path.join(CORPUS if kind == "first" else ROOT, src); d = os.path.join(ROOT, cid)
    if os.path.exists(d):
        for f in os.listdir(d):
            q = os.path.join(d, f); shutil.rmtree(q) if os.path.isdir(q) else os.remove(q)
    shutil.copytree(s, d, dirs_exist_ok=True)
    for f in os.listdir(d):
        if f.upper().endswith(".MST"):
            os.replace(os.path.join(d, f), os.path.join(d, f + (".oracleA" if kind == "first" else ".prev")))
        if f.endswith(".png"): os.remove(os.path.join(d, f))
    mp = [f for f in os.listdir(d) if f.lower().endswith(".moproj")][0]
    if kind == "mb": mbprops.apply(os.path.join(d, mp), *spec[2:])
    if kind == "fe": mbprops.apply(os.path.join(d, mp), *spec[2:], type_prefix="<Type>SfOprFormingExpress", fields=mbprops.FE_BLOW)
    if kind == "first": first(d, *spec[2:])
    print(cid, src)


for c in sys.argv[1:]: build(c)
