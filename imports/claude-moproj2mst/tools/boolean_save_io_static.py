"""Retain exact caller status and shipped Qt failed-write paths; never load a DLL."""
import hashlib
import json
from pathlib import Path
import subprocess
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import DUMPBIN, imports


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    output = ROOT/'analysis/wave3-boolean-save-io-contract.json'
    assert not output.exists()
    captures = []
    originals = PE()
    qt = PE(ROOT/'evidence/original-binaries/QtCore4_64.dll')
    assert sha(originals.data) == 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc'
    assert sha(qt.data) == 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160'
    for pe, filename, entries in [
        (originals,'DEF_GUI_MO_64.exe',[0x1400980d0,0x140010b80,0x140028520]),
        (qt,'QtCore4_64.dll',[0x670c6d60,0x670c2f50,0x670b5f30,0x670b5e00,
                               0x670b4ae0,0x670ea5c0,0x670f8150])]:
        for entry in entries:
            bounds = pe.bounds(entry)
            assert int(bounds['begin'],0) == entry
            end = int(bounds['end_exclusive'],0)
            binary = ROOT/'evidence/original-binaries'/filename
            command = [str(DUMPBIN),'/disasm:bytes',f'/range:{hex(entry)},{hex(end-1)}',str(binary)]
            raw = subprocess.run(command,capture_output=True,check=True).stdout
            path = ROOT/f'analysis/wave3-boolean-save-io-{entry:x}.asm'
            assert not path.exists()
            path.write_bytes(raw)
            captures.append({'binary':str(binary.relative_to(ROOT)),'binary_sha256':sha(pe.data),
                             'entry':hex(entry),'end_exclusive':hex(end),
                             'bounds_qualification':bounds['qualification'],
                             'code_sha256':sha(pe.data[pe.off(entry):pe.off(end)]),
                             'artifact':str(path.relative_to(ROOT)),'artifact_sha256':sha(raw),'command':command})
    qi = imports(qt)
    assert qi[0x671a8138] == 'CreateFileW'
    assert qt.u64(qt.off(0x672346b8)+8) == 0x670ea5c0
    assert qt.u64(qt.off(0x672346b8)+0x70) == 0x670f9420
    # Exported QTextStream::flush is a leaf tail-jump to the private flush body.
    assert qt.data[qt.off(0x670c37f0):qt.off(0x670c37f0)+9].hex() == '488b4908e957f7ffff'
    literals = {}
    for address in [0x1420f85d0,0x1420f85e0,0x1420ffdc8,0x1420ffe20,0x1420ffe00]:
        offset = originals.off(address)
        literals[hex(address)] = originals.data[offset:originals.data.find(b'\0',offset)].decode()
    matrix = [
        {'case':'precheck_reject','condition':'subproject check returns other than0x10000',
         'effects':'Return that status before active-op/project/property/MST saves.',
         'checked_save_wrapper':'false/canceled message'},
        {'case':'mst_open_fail','condition':'earlier checks pass; final MST destination open fails',
         'mst_internal_result':False,'boolean_sidefile_attempts':[],
         'study_followups':'Eligible calls still occur in0x1400a20d0 after MST failure.',
         'top_save_result':'0x10000 after successful subproject checks',
         'checked_save_wrapper':'true/saved message','auto_save_wrapper':'1/saved message'},
        {'case':'bwz_open_fail','condition':'MST open succeeded; BWZ open fails',
         'boolean_sidefile_attempts':['BWZ','INI'],'boolean_helper_result':0,
         'mst_behavior':'Ignores helper result; emits following EXECMD/output.',
         'top_save_result':'0x10000 if prior checks passed'},
        {'case':'ini_open_fail','condition':'MST and BWZ open succeeded; INI open fails',
         'boolean_sidefile_attempts':['BWZ','INI'],'boolean_helper_result':1,
         'mst_behavior':'Continues; successful BWZ result is returned regardless INI result.'},
        {'case':'boolean_write_or_flush_fail_after_open','condition':'An endl write is nonpositive/short or QFile flush returns false',
         'stream_state':'Private+0xcc becomes3; future flush returns early while status nonzero.',
         'file_effect':'Direct final target can retain incomplete bytes; no rollback or rename.',
         'helper_result':'Successful-open Boolean writer still returns1 after QFile::close.',
         'following_file':'Next sidefile is still attempted through0x1411e1820.'},
        {'case':'mst_write_or_flush_fail_after_open','condition':'MST destination opened, then its QTextStream records failure',
         'mst_internal_result':True,'generator_behavior':'MST writer does not test QTextStream status; generation/callbacks continue.',
         'top_save_result':'0x10000 if prior checks passed'},
        {'case':'existing_target_without_share_delete','condition':'Other reader permits read/write sharing but not delete sharing',
         'original_open':'QFS native CreateFileW requests GENERIC_WRITE, share3 (READ|WRITE), OPEN_ALWAYS4, normal attributes0x80.',
         'candidate_difference':'Atomic MoveFileEx replacement requires a different sharing operation and can fail where direct writing is permitted.'}]
    sources = ['evidence/gui_mo/L5d/sv.c','evidence/gui_mo/L5d/sv3.c',
               'evidence/gui_mo/s1/mdt/save_d0.c','evidence/gui_mo/s1/s1-02/a1s.c',
               'evidence/gui_mo/s1/s1-04/d1c.c','evidence/gui_mo/s1/s1-04/d2c.c',
               'analysis/wave3-boolean-lifecycle-failure-static.json']
    result = {'schema':'wave3-boolean-save-io-v1','method':'Offline DUMPBIN and direct PE only; no original runtime, DLL load, app control, native fault injection, build or shared REA.',
              'captures':captures,'failure_matrix':matrix,'caller_literals':literals,
              'qt_flush_contract':{'endl':'0x670c6d60 inserts LF then calls0x670c2f50',
                  'status_guard':'0x670c2f7e checks private+0xcc; nonzero exits.',
                  'buffer_clear':'0x670c3032 clears source QString buffer before device write0x670c3047.',
                  'device_write':'0x670bb750 is exported QIODevice::write(char const*,qint64).',
                  'nonpositive_write':'0x670c304f..0x670c3054 sets status3.',
                  'qfile_flush':'0x670c309b invokes QFile::flush; false sets status3 at0x670c30b9.',
                  'short_write':'Positive result unequal encoded length also sets status3 at0x670c30b9; no retry loop appears in this flush body.'},
              'qt_native_open_contract':{'qfile_open':'0x670b4ae0 passes requested mode OR0x20 to engine.',
                  'qfs_open':'0x670ea644..0x670ea651 adds0x8 when write bit2 is set and read/append bits1/4 are clear.',
                  'native_open':'0x670f8150','createfile_iat':'0x671a8138','createfile_symbol':qi[0x671a8138],
                  'mode2_windows_arguments':{'desired_access':'0x40000000','share_mode':3,'creation_disposition':4,'attributes':'0x80'},
                  'truncate':'After successful CreateFileW, bit8 invokes engine vt+0x70 with0 at0x670f827a; QFS vtable0x672346b8 resolves this slot to exported setSize0x670f9420. Its result is ignored in nativeOpen, which returns1.',
                  'qualification':'Native setSize and lower WriteFile implementation are not fully recovered here; no claim about exact OS partial-write byte counts.'},
              'status_qualification':'Original internal false/stream status3 and GUI wrapper success are recovered independently. The m2m adapter error-code compatibility policy is a coordinator decision; no ABI change follows automatically.',
              'files_sha256':{s:sha((ROOT/s).read_bytes()) for s in sources},
              'reproducer':str(Path(__file__).relative_to(ROOT)),'reproducer_sha256':sha(Path(__file__).read_bytes())}
    output.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'receipt':str(output),'sha256':sha(output.read_bytes()),'captures':len(captures),'cases':len(matrix)}))


if __name__ == '__main__':
    main()
