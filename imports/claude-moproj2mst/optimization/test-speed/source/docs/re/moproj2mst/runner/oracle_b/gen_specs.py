"""gen_specs.py: write final.json, the oracle-B case list (case dir name = cases.tsv case_id)."""
import json

G3 = "57_Gear_blank"              # 3D SI base: Forming, Forming (op ids 1, 2)
C2 = "51_MO2_Operation_Cycles2"   # 2D SI base: Forming2D, Forming2D, Cycle (op ids 1, 2, 3)

# HeatTreatWizard.HTWZ (V3): 3 steps, first written step 2; step 1 has a user keyword file,
# step 2 has none (token absent), step 3 says "None". The 2D case starts at step 1 (user key written). Media block as P16's reader test.
HTWZ = ("HEAT_TREATMENT_SPECS_FILE_V3\n"
        "NO_MEDIA  1\n"
        "MEDIA   1\n"
        "Water quench\n"
        "NO_ZONES   1\n"
        "ZONE   0\n"
        "CNVCOF 0 5000.0\n"
        "RADIATION 0\n"
        "ACVCOF 0 0.0\n"
        "NO_ECTMFN  0\n"
        "NO_PROCS  3\n"
        "PROC 1 120.0 1 0 0 0 0 0 0 {DIR}/UserHT.KEY 0\n"
        "PROC 2 60.0 1 0 0 0 0 0 0\n"
        "PROC 3 30.0 1 0 0 0 0 0 0 None 0\n"
        "START_OPTNO  2\n")

def mdwz(passes):
    return ("WZD_MACHDIST_WORKPIECE_OBJECT_NO\n  * OBJECT # FOR WORKPIECE \n1\n\n"
            "WZD_MACHDIST_NUM_FIXTURES\n  * NUMBER OF FIXTURES AND THEIR OBJECT #\n0\n\n"
            "WZD_MACHDIST_NUM_PASSES\n  * NUMBER OF PASSES AND THEIR OBJECT #\n%d\n%s\n"
            "BEGIN_FREE_DIST_BCC_INFO\nBCC_INIT    \t0\nFIXEDNODE    \t0\nFIXEDCOORD    \t0\t0\t0\n"
            "FIXED_Z_COORD    \t0\t0\t0\nFIXED_X_COORD    \t0\t0\t0\nFIXED_Y_COORD    \t0\t0\t0\nAUTO    \t0\n"
            "FIXEDZNODE    \t0\nFIXEDZANGLE    \t0\nFIXEDXNODE    \t0\nFIXEDXANGLE    \t0\nFIXEDYNODE    \t0\n"
            "FIXEDYANGLE    \t0\nEND_FREE_DIST_BCC_INFO\n INIT_VEL_BCC\t0\nDONE\n") % (len(passes), "".join("%d\n" % p for p in passes))

def op(t, name, objs_from=1, obj_types=None):
    return {"type": t, "name": name, "objs_from": objs_from, "obj_types": obj_types}

WP = [1]
cases = [
    ("59_B_HT3", G3, [op("SfOprHeatTreat", "Heat Treat", 1, WP)],
     {"Operations/Task00003/HeatTreatWizard.HTWZ": HTWZ, "UserHT.KEY": "copy:Operations/Task00001/SimCtrl.KEY"}),
    ("60_B_HT2", C2, [op("SfOprHeatTreat2D", "Heat Treat", 1, WP)],
     {"Operations/Task00004/HeatTreatWizard.HTWZ": HTWZ.replace("START_OPTNO  2", "START_OPTNO  1"), "UserHT.KEY": "copy:Operations/Task00001/SimCtrl.KEY"}),
    ("61_B_DS3", G3, [op("SfOprDiestress", "Die Stress")], {}),
    ("62_B_BM2", C2, [op("SfOprDiestressBM2D", "Die Stress BM")], {}),
    ("63_B_BM3", G3, [op("SfOprDiestressBM3D", "Die Stress BM")], {}),
    ("64_B_RPT", G3, [op("SfOprReport", "Report", None)], {}),
    ("65_B_SOPT", G3, [op("SfOprShapeOptim", "Shape Optimization", None)], {}),
    ("66_B_DIS3", G3, [op("SfOprDistortion3D", "Distortion")], {"Operations/Task00003/MachDistortion.MDWZ": mdwz([2, 3])}),
    ("67_B_DIS2", C2, [op("SfOprDistortion2D", "Distortion")], {"Operations/Task00004/MachDistortion.MDWZ": mdwz([2, 3])}),
    ("68_B_BOOL3", G3, [op("SfOprBoolean3D", "Boolean")], {}),
    ("69_B_CNV", C2, [op("SfOprConvert2Dto3D", "2D to 3D", 1, WP)], {}),
    ("70_B_PRE3", G3, [op("SfOprPre3D", "Pre")], {}),
    ("71_B_PRE2", C2, [op("SfOprPre2D", "Pre")], {}),
    ("72_B_HTF", G3, [op("SfOprHTFurnace", "HT Furnace", 1, WP)], {}),
    ("73_B_HTX3", G3, [op("SfOprHeatTransferExpress", "Heat Transfer Express", 1, WP)], {}),
    ("74_B_MB2", C2, [op("SfOprMultiBlow2D", "Multi Blow")], {}),
    ("75_B_MB3", G3, [op("SfOprMultiBlow3D", "Multi Blow")], {}),
    ("76_B_FE2", C2, [op("SfOprFormingExpress2D", "Forming Express")], {}),
    ("77_B_FE3", G3, [op("SfOprFormingExpress", "Forming Express")], {}),
]
json.dump([{"case": c, "base": b, "append": a, "files": f} for c, b, a, f in cases], open("final.json", "w"), indent=1)
print(len(cases))
