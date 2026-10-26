"""Retain bounded shipped CRT/Qt evidence and frozen Cogging native writer hashes."""
import hashlib
import json
import re
import subprocess
from doe_static_pe import PE, ROOT
from boolean_lifecycle_static import DUMPBIN


def main():
    sha = lambda data: hashlib.sha256(data).hexdigest()
    output = ROOT/'analysis/wave3-cogging-native-io-contract.json'
    assert not output.exists()
    captures = []
    selections = {
        'DEF_GUI_MO_64.exe': [0x1407617e0, 0x14146fcd8, 0x14146fba0,
                             0x1414820ac, 0x1414883ec, 0x1414882b4,
                             0x1414879c4, 0x14148a7c0, 0x14147fc20,
                             0x141476b38, 0x14146da0c, 0x14146fb28],
        'QtCore4_64.dll': [0x67174f70, 0x67070630, 0x6706e470, 0x670c4530],
    }
    for name, entries in selections.items():
        binary = ROOT/'evidence/original-binaries'/name
        pe = PE(binary)
        for entry in entries:
            bounds = pe.bounds(entry)
            if entry == 0x14146fcd8: end = entry+11
            elif entry == 0x67174f70: end = entry+5
            else:
                assert bounds and int(bounds['begin'], 0) == entry
                end = int(bounds['end_exclusive'], 0)
            artifact = ROOT/f'analysis/wave3-cogging-verified-{entry:x}.asm'
            command = [str(DUMPBIN), '/disasm:bytes', f'/range:{hex(entry)},{hex(end-1)}', str(binary)]
            assert not artifact.exists()
            artifact.write_bytes(subprocess.run(command, capture_output=True, check=True).stdout)
            captures.append({'binary': name, 'binary_sha256': sha(pe.data),
                             'entry': hex(entry), 'end_exclusive': hex(end),
                             'bounds_qualification': 'Exact leaf jump.' if not bounds else bounds['qualification'],
                             'code_sha256': sha(pe.data[pe.off(entry):pe.off(end)]),
                             'artifact': str(artifact.relative_to(ROOT)),
                             'artifact_sha256': sha(artifact.read_bytes()), 'command': command})
    sources = ['source/apps/moproj2mst/src/datafiles/mdt_write.cpp',
               'source/apps/moproj2mst/src/datafiles/cogging_file.hpp',
               'source/apps/moproj2mst/src/datafiles/cogging_file.cpp',
               'source/apps/moproj2mst/tests/mdt/test_cogging_io.cpp']
    evidence = ['evidence/gui_mo/s1/mdt/w_0.c', 'evidence/gui_mo/s1/mdt/w_1.c',
                'evidence/gui_mo/s1/mdt/w_d3.c', 'evidence/gui_mo/s1/mdt/17e0.asm',
                'evidence/gui_mo/s1/mdt/save_d0.c', 'analysis/save-semantics.md',
                'analysis/wave3-cogging-path-1400024f0.asm',
                'analysis/wave3-boolean-open-sequence-contract.json']
    result = {'schema': 'wave3-cogging-native-io-v1',
              'method': 'Offline fixed original PE/DUMPBIN and retained decompilation. No original/runtime/UI, DLL execution or builds by this worker.',
              'status': 'source_and_7_focused_tests_frozen_for_coordinator_build',
              'declaration': 'bool saveCoggingMdt(Project&, Operation&, const std::string& header, bool produce, bool native, std::vector<SideFile>* out);',
              'captures': captures,
              'contract': [
                  {'addresses': '0x1400982e0 -> properties+0xb8 ->0x140760400 ->0x1407616d0',
                   'behavior': 'Property loop runs operation rows in order and ignores individual returns. Cogging checks model owner-properties and its operation before computing task directory.'},
                  {'addresses': '0x1407618ba,0x1407618bf,0x140761c4b',
                   'behavior': 'fopen(path,wt) BEFORE masterWrite. Null skips all serialization/global effects and returns1. Nonnull prints fragments, ignores printf/fclose returns, then returns1.'},
                  {'addresses': '0x14146fcd8 ->0x14146fba0 ->0x1414820ac',
                   'behavior': 'fopen sets sharing40 (_SH_DENYNO); wt parses to flags4301 and stream write flag2. Normal regular-file path reaches low-level sopen helper.'},
                  {'addresses': '0x1414879c4,0x141487abf,0x141487afa,0x141487b4c,0x141487c38',
                   'behavior': 'wt/SH_DENYNO normal path: CreateFileA,GENERIC_WRITE40000000,shareREAD|WRITE3,CREATE_ALWAYS2,NORMAL80,SECURITY_ATTRIBUTES24/null/inheritTRUE,nulltemplate. No mkdir, temporary replacement or FlushFileBuffers.'},
                  {'addresses': '0x14147fcd6 ->0x14148a7c0',
                   'behavior': 'Write-buffer slow path normally allocates4096; getbuf sets FILE base/pointer/bufsize4096 and count0. Allocation failure instead uses internal2-byte char buffer.'},
                  {'addresses': '0x141476b75..0x141476ba3',
                   'behavior': 'CRT flush sets IOERR on short/error and returnsEOF, but always resets count0/pointerbase, discarding attempted buffer; it does not retain bytes for QFile-style Close retry.'},
                  {'addresses': '0x14146db46..0x14146db67,0x14147fc20',
                   'behavior': 'Regular printf calls are independent; prior IOERR does not create a QTextStream sticky suppression gate. Each formatter result is ignored by Cogging writer.'},
                  {'addresses': '0x67174f70 ->0x67070630 ->0x6706e470',
                   'behavior': 'QString constchar/ascii caches NUL-terminated toAscii bytes. Null CStringcodec ->Latin1; nonnull ->configured CStringcodec. CreateFileA stops at firstNUL.'},
                  {'addresses': '0x670c4577..0x670c4595',
                   'behavior': 'Related root Distortion constchar stream insertion uses fromLatin1_helper6706bf40 with length-1, then private QString writer; it does not decode these bytes via fromAscii/fromUtf8.'},
              ],
              'implementation': ['Native per-op seam opens before generation and returns handled true after open/write/close failures; native accepts null out and stages no replay.',
                                 'Normal native Win32 CreateFileA flags fixed explicitly; UCRT descriptor/FILE supplies text CRLF and4KiB explicitly supplied buffer.',
                                 'Shared serializer emits same formatted fragments to independent fprintf calls after successful open; existing recording path retains CRLF bytes/refusals.',
                                 'No shared core/header or original evidence edits by this worker; coordinator integrates declaration/per-row call.'],
              'qualification': ['Null-CString-codec/default-language adapter profile only; configured-language CStringcodec binding is RFORMAT dependency.',
                                'Normal regular-file allocation branch. Original two-byte allocation-failure fallback, FILE/descriptor exhaustion and host setvbuf failure are not generalized. Native bridge FILE allocation occurs after Win32 open, so exhaustion may differ.',
                                'Host UCRT fprintf/text low-level writes are used with pinned size and independent calls; full old-static-CRT low-level partial/disk-full byte accounting is not ported or claimed closed.',
                                'No original failure injection; focused Windows lock/sharing tests validate candidate branches only. Coordinator owns tests and full regression.',
                                'Existing null CogModel handling remains adapter behavior; original wrapper dereferences its data pointer.'],
              'tests': {'target': 'm2m_mdt_cogging_io', 'ctest': 'm2m.mdt.cogging_io',
                        'cases': re.findall(r'TEST_CASE\(([^)]+)', (ROOT/sources[-1]).read_text()),
                        'execution': 'Not run by this worker; source frozen for coordinator.'},
              'source_sha256': {p: sha((ROOT/p).read_bytes()) for p in sources},
              'evidence_sha256': {p: sha((ROOT/p).read_bytes()) for p in evidence}}
    output.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({'receipt': str(output.relative_to(ROOT)), 'sha256': sha(output.read_bytes()),
                      'captures': len(captures), 'tests': len(result['tests']['cases'])}))


if __name__ == '__main__':
    main()
