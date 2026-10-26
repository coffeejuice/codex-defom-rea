"""Writes cases.json: scripted DEF_PRE sessions for the edit menus 5 and 6 (packet p10-02).

Every session reads a seed KEY (menu 2 -> 1), runs one menu-5 or menu-6 script, saves the
keywords with menu 8 to OUT.KEY and exits with E / Y, so no session waits on EOF. The field
"replay" holds what the port's replay test (port/def_pre/tests/test_menu56_replay.cpp) needs:
the menu number and the stdin of the menu alone (after the main-menu answer 5 or 6).

Run with: python -I tests/menus/MENU5_6/make_cases.py
then:     python -I tools/oracle/def_pre_oracle.py cases tests/menus/MENU5_6/cases.json
"""
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
SEEDS = {
    "3d": ("UPSET_Brick.KEY", "C:/Users/alext/Projects/DEFORM_corpus/key/3D/SI/Forging/UPSET_Brick.KEY"),
    "2d": ("SPIKE_SI.KEY", "C:/Users/alext/Projects/DEFORM_corpus/key/2D/SI/Forging/SPIKE_SI.KEY"),
}

# (name, seed, menu, answers inside the menu (each one line), comment)
CASES = [
    # ---- menu 5 ----------------------------------------------------------------------------------
    ("m5_baseline", "3d", 5, ["E"], "menu 5 shown once and left: the save is the seed's round trip"),
    ("m5_baseline", "2d", 5, ["E"], "menu 5 shown once and left (2D list has items 11 and 12)"),
    ("m5_number_name", "3d", 5, ["1", "3", "2", "NEW_DIE", "R", "1", "2", "E"],
     "item 1 selects object 3, item 2 renames it, R shows the list twice, back to object 2"),
    ("m5_type", "3d", 5, ["3", "4", "3", "x", "E"],
     "item 3: object 2 becomes type 3 (Elastic); a non-number keeps it"),
    ("m5_rigid_na", "3d", 5, ["1", "1", "6", "7", "8", "9", "10", "4", "E", "E"],
     "rigid object 1: items 6-10 print the Not Applicable warning, item 4 is allowed"),
    ("m5_char", "3d", 5, ["4", "4", "500", "5", "10", "1", "0", "2.5", "1", "7", "1", "3", "8", "0", "1",
                          "90", "1", "0", "9", "45", "2", "5", "1", "2", "3", "R", "E", "E"],
     "3D characteristics: REFTMP, STROKE (falls into MOVCTL: speed constant 2.5), ANGMOV RPUT, "
     "CNTRAX (falls into ROTSYM), ROTSYM, items 1-3 not implemented, R"),
    ("m5_movement", "3d", 5, ["4", "6", "2", "100", "1", "6", "3", "7", "0", "6", "4", "8", "0", "6", "0",
                              "1", "1", "7", "0", "4", "E", "E"],
     "3D MOVCTL: hammer, screw press, mechanical press, force by function (not implemented); "
     "ANGMOV torque"),
    ("m5_die", "3d", 5, ["1", "1", "5", "1", "3", "5", "6", "1", "2", "3", "R", "1", "1", "E", "E"],
     "die geometry of rigid object 1: type 1, items 3/5/6 not implemented, type 2 (points menu), "
     "back to type 1"),
    ("m5_die_slash", "3d", 5, ["1", "3", "5", "2", "/"], "die geometry form left with / (exits menu 5 too)"),
    ("m5_bcfunc", "3d", 5, ["8", "1", "2", "R", "E", "E"],
     "BC function form: items not implemented, R shows the menu 5 list"),
    ("m5_remesh", "3d", 5, ["10", "1", "0.5", "2", "3", "3", "0.1", "4", "7", "5", "100", "11", "20", "6", "2",
                            "7", "0.5", "8", "0.6", "9", "0.7", "10", "0.8", "R", "E", "E"],
     "3D remeshing criteria, every item; MGWCUV (10) writes MGWSTR"),
    ("m5_remesh_nostore", "3d", 5, ["10", "4", "x", "6", "9", "7", "", "4", "5", "6", "3", "E", "E"],
     "after a non-number RMSTEP items 6-10 store nothing; an empty answer is 0 for read_double_range"),
    ("m5_remesh_slash", "3d", 5, ["10", "/"], "remeshing form left with /"),
    ("m5_char", "2d", 5, ["4", "1", "0.2", "2", "0.001", "3", "4", "400", "x", "5", "600", "6", "5", "7", "1",
                          "0", "3", "1", "8", "0", "1.5", "9", "10", "11", "900", "12", "13", "1", "1", "13",
                          "2", "7", "R", "E", "E"],
     "2D characteristics: every item (3, 9, 12 end the form with the warning when not applicable)"),
    ("m5_char_rigid", "2d", 5, ["1", "3", "4", "1", "9", "1", "4", "4", "10", "5", "12", "E", "E"],
     "2D characteristics of rigid object 3: AVGSTR not applicable, ANGMOV, CNTRAX, item 12"),
    ("m5_remesh", "2d", 5, ["10", "5", "300", "6", "3", "4", "2", "11", "30", "R", "E", "E"],
     "2D remeshing criteria (MGNELM single int, MGTELM row)"),
    ("m5_die", "2d", 5, ["5", "1", "2", "4", "R", "E", "E"], "2D die menu of object 1"),
    # ---- menu 6 ----------------------------------------------------------------------------------
    ("m6_baseline", "3d", 6, ["E"], "menu 6 shown once and left"),
    ("m6_objects", "3d", 6, ["1", "0", "1", "1", "R", "E"],
     "Display Defined Objects; graphics option 1 is not implemented"),
    ("m6_status_y", "3d", 6, ["2", "Y", "", "n", "E", "E"],
     "status: any answer but n shows it again; n opens the form, E leaves both"),
    ("m6_interaction", "3d", 6, ["2", "n", "2", "2", "3", "1", "0", "0.3", "4", "0", "5", "5", "1", "0.01",
                                 "R", "E", "E"],
     "interaction form on the appended pair (1, 1): CNTACT, friction, IHTCOF, separation"),
    ("m6_interaction_pair", "3d", 6, ["2", "n", "1", "2", "2", "4", "3", "2", "x", "1", "E", "E"],
     "pair 2 selected by one number (2, 2); CNTACT 4 (no echo of the new value); friction type 2, method x keeps 0, FRCFAC 1"),
    ("m6_interaction_slash", "3d", 6, ["2", "n", "/", "3", "E", "E"],
     "the status form drops the form's /: menu 6 goes on"),
    ("m6_offset", "3d", 6, ["3", "3", "1", "4", "", "R", "E", "E"],
     "positioning by offset (1, 1, 1) of object 1"),
    ("m6_rotation", "3d", 6, ["3", "1", "3", "2", "3", "3", "0", "4", "1", "5", "90", "6", "", "E", "E"],
     "positioning by rotation of object 3 about (0,0,0) axis (1,1,1) by 90"),
    ("m6_interference", "3d", 6, ["3", "1", "2", "2", "1", "3", "2", "4", "0", "5", "0.001", "6", "", "E",
                                  "E"],
     "positioning by interference of object 1 on object 2 along (0, 0, 0)"),
    ("m6_interference_dir", "3d", 6, ["3", "1", "2", "2", "3", "3", "2", "5", "0.01", "6", "", "E", "E"],
     "interference of object 3 on object 2 along the default relative velocity (0, 0, -1)"),
    ("m6_position_slash", "3d", 6, ["3", "/"], "positioning form left with /: menu 6 returns"),
    ("m6_bcc", "3d", 6, ["4", "1", "0.002", "3", "2", "R", "E", "E"],
     "BCC: error limit, initialize contact of object 2"),
    ("m6_bcc_generate", "3d", 6, ["4", "2", "", "E", "E"], "BCC generation (GENCTC 3D, 1401a6c80)"),
    ("m6_interaction", "2d", 6, ["1", "0", "2", "n", "R", "5", "2", "0.5", "E", "E"],
     "2D: object status, interaction form (existing pairs)"),
    ("m6_offset", "2d", 6, ["3", "2", "2", "3", "5", "4", "", "E", "E"], "2D positioning by offset of object 2"),
]


def main():
    out = []
    seen = set()
    for name, seed, menu, answers, comment in CASES:
        label = f"MENU5_6/{name}_{seed}"
        assert label not in seen, label
        seen.add(label)
        seed_name, seed_src = SEEDS[seed]
        menu_stdin = "".join(a + "\n" for a in answers)
        stdin = f"\n2\n1\n{seed_name}\n\nE\n{menu}\n" + menu_stdin + "8\nOUT.KEY\nE\nY\n"
        out.append({
            "label": label,
            "args": [],
            "stdin": stdin,
            "timeout": 120,
            "inputs": [{"name": seed_name, "source": seed_src}],
            "replay": {"menu": menu, "seed": seed_name, "stdin": menu_stdin},
            "comment": comment,
        })
    (HERE / "cases.json").write_text(json.dumps(out, indent=1) + "\n", encoding="utf-8")
    print(len(out), "cases")


if __name__ == "__main__":
    main()
