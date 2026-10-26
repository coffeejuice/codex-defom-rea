"""Read-only fixture/static-evidence inspection; writes only wave2-doe-evidence.json."""
import hashlib
import json
from pathlib import Path
import struct
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def project_profile(folder):
    project = next(folder.glob("*.moproj"))
    root = ET.fromstring(project.read_bytes())
    return {"path": str(project.relative_to(ROOT)), "sha256": sha(project), "bytes": project.stat().st_size,
            "subprojects": [{"attributes": e.attrib, "studies": [x.attrib for x in e.findall("SfDoeStudy")]}
                            for e in root.findall("Sub_Project")],
            "study_files": {p.relative_to(folder).as_posix(): {"bytes": p.stat().st_size, "sha256": sha(p)}
                            for p in sorted(folder.glob("DoeStudies/**/*")) if p.is_file()},
            "temp_kwd": {"sha256": sha(folder / "temp.KWD"), "bytes": (folder / "temp.KWD").stat().st_size}
                        if (folder / "temp.KWD").exists() else None}


def pe_strings():
    binary = ROOT / "evidence/original-binaries/DEF_GUI_MO_64.exe"
    data = binary.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3c)[0]
    assert data[pe:pe+4] == b"PE\0\0"
    count, optional_size = struct.unpack_from("<H", data, pe+6)[0], struct.unpack_from("<H", data, pe+20)[0]
    optional = pe+24
    assert struct.unpack_from("<H", data, optional)[0] == 0x20b
    image_base = struct.unpack_from("<Q", data, optional+24)[0]
    table = optional+optional_size
    strings = {}
    for va in (0x14214ae98, 0x14213fb44, 0x142140210, 0x142140220, 0x142140230,
               0x14213fb08, 0x14213fb18, 0x14213fb28, 0x1427bba00, 0x142166b78):
        rva = va-image_base
        for i in range(count):
            at = table+40*i
            virtual_size, start, raw_size, offset = struct.unpack_from("<IIII", data, at+8)
            if start <= rva < start+raw_size:
                file_offset = offset+rva-start
                end = data.find(b"\0", file_offset, file_offset+256)
                assert end >= file_offset
                raw = data[file_offset:end]
                strings[hex(va)] = {"ascii": raw.decode("ascii"), "file_offset": file_offset, "raw_hex": raw.hex()}
                break
        else:
            raise ValueError(f"unmapped string {va:x}")
    return {"path": str(binary.relative_to(ROOT)), "sha256": sha(binary), "image_base": hex(image_base), "strings": strings}


def main():
    base = ROOT / "evidence/gui_mo/oracle_b"
    pairs = {}
    for name in ("120_B_DOE", "121_B_DOE_VAR", "122_B_DOE_GEO", "127_B_DOE_OPEN", "128_B_DOE_VAR_OPEN"):
        before, after = project_profile(base / (name+"_before")), project_profile(base / name)
        a, b = before["study_files"], after["study_files"]
        pairs[name] = {"before": before, "after": after,
                       "study_file_delta": {"added": sorted(b.keys()-a.keys()), "removed": sorted(a.keys()-b.keys()),
                                            "changed": sorted(k for k in a.keys() & b.keys() if a[k] != b[k])}}
    previews = {}
    for name in ("120_B_DOE", "121_B_DOE_VAR", "122_B_DOE_GEO"):
        control = base / name / "DoeStudies/DoeStudy1/Control"
        par = (control / "DEF_DOE.PAR").read_bytes()
        lhs = (control / "Samples_LHS.par").read_bytes()
        ini = (control / "DEF_DOE.INI").read_bytes()
        previews[name] = {"par_equals_lhs": par == lhs, "par_line_count": len(par.splitlines()),
                          "par_first_lines": [x.decode("ascii") for x in par.splitlines()[:5]],
                          "par_last_line": par.splitlines()[-1].decode("ascii"),
                          "ini_line_count": len(ini.splitlines()),
                          "ini_first_lines": [x.decode("ascii") for x in ini.splitlines()[:10]]}
    selected = [
        "source/apps/moproj2mst/src/model/subproject.cpp", "source/apps/moproj2mst/src/core/convert.cpp",
        "source/apps/moproj2mst/src/mst/master.cpp", "source/apps/moproj2mst/src/mst/doe.cpp",
        "source/apps/moproj2mst/src/doe/doe_study.cpp", "source/apps/moproj2mst/src/moproj/read.cpp",
        "source/apps/moproj2mst/src/moproj/write.cpp", "source/apps/moproj2mst/tests/s/test_save_folder.cpp",
        "source/apps/moproj2mst/include/m2m/doe_study.hpp", "source/apps/moproj2mst/include/m2m/mst/doe_model.hpp",
        "evidence/gui_mo/L5d/b5470.c", "evidence/gui_mo/out_scope/save2.c",
        *[f"evidence/gui_mo/s1/mdt/DOE/{name}.c" for name in ("d0", "d1", "d3", "d4", "d5", "d6", "d7", "d9", "e0", "e1")],
        "source/docs/re/moproj2mst/mdt/L6/doe.md", "source/docs/re/moproj2mst/s1-05/spec.md",
        "build/test-runs/1792752423664000600/m2m.s.save_folder.log"]
    result = {"schema": "wave2-doe-static-evidence-v1", "pairs": pairs, "content_previews": previews,
              "pe_string_evidence": pe_strings(),
              "source_identities": {p: {"sha256": sha(ROOT/p), "bytes": (ROOT/p).stat().st_size} for p in selected},
              "qualification": "Retained historical original GUI artifacts and decompiles, plus current source inspection. No original/candidate execution, REA connection, source edit, build or simulation/study execution. Byte-derived strings do not establish runtime behavior. Imported decompilation of DOE functions has not been independently byte-validated in this pass."}
    out = ROOT / "analysis/wave2-doe-evidence.json"
    out.write_text(json.dumps(result, indent=2, sort_keys=True)+"\n", encoding="utf-8")
    print(json.dumps({"output": str(out), "sha256": sha(out), "pairs": {k: {"before_subs": len(v["before"]["subprojects"]),
                      "after_subs": len(v["after"]["subprojects"]), "delta": v["study_file_delta"]} for k,v in pairs.items()},
                      "strings": result["pe_string_evidence"]["strings"]}, indent=2))


if __name__ == "__main__":
    main()
