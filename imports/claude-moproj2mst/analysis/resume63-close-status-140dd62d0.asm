
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD62D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD62D5: 48 83 EC 38        sub         rsp,38h
  0000000140DD62D9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DD62DE: 48 83 C1 08        add         rcx,8
  0000000140DD62E2: E8 69 27 30 FF     call        00000001400D8A50
  0000000140DD62E7: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140DD62EC: 74 19              je          0000000140DD6307
  0000000140DD62EE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DD62F3: 48 83 C1 08        add         rcx,8
  0000000140DD62F7: E8 54 27 30 FF     call        00000001400D8A50
  0000000140DD62FC: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140DD6300: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD6305: EB 0C              jmp         0000000140DD6313
  0000000140DD6307: 48 8D 05 82 1E 8E  lea         rax,[00000001446B8190h]
                    03
  0000000140DD630E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DD6313: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DD6318: 48 83 C4 38        add         rsp,38h
  0000000140DD631C: C3                 ret

  Summary

     1E02000 .text
