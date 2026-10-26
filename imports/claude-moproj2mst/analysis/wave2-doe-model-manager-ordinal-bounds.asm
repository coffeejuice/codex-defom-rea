
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C794D0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C794D4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C794D9: 48 83 EC 28        sub         rsp,28h
  0000000140C794DD: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140C794E2: 7D 04              jge         0000000140C794E8
  0000000140C794E4: 32 C0              xor         al,al
  0000000140C794E6: EB 1A              jmp         0000000140C79502
  0000000140C794E8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C794ED: 48 83 C1 18        add         rcx,18h
  0000000140C794F1: E8 EA B5 38 FF     call        0000000140004AE0
  0000000140C794F6: 3B 44 24 38        cmp         eax,dword ptr [rsp+38h]
  0000000140C794FA: 7F 04              jg          0000000140C79500
  0000000140C794FC: 32 C0              xor         al,al
  0000000140C794FE: EB 02              jmp         0000000140C79502
  0000000140C79500: B0 01              mov         al,1
  0000000140C79502: 48 83 C4 28        add         rsp,28h
  0000000140C79506: C3                 ret
  0000000140C79507: CC                 int         3
  0000000140C79508: CC                 int         3
  0000000140C79509: CC                 int         3
  0000000140C7950A: CC                 int         3
  0000000140C7950B: CC                 int         3
  0000000140C7950C: CC                 int         3
  0000000140C7950D: CC                 int         3
  0000000140C7950E: CC                 int         3
  0000000140C7950F: CC                 int         3
  0000000140C79510: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140C79514: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C79519: 48 83 EC 38        sub         rsp,38h
  0000000140C7951D: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140C79522: 7F 04              jg          0000000140C79528
  0000000140C79524: 32 C0              xor         al,al
  0000000140C79526: EB 29              jmp         0000000140C79551
  0000000140C79528: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  0000000140C7952C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C79531: E8 AA F9 FF FF     call        0000000140C78EE0
  0000000140C79536: 48 85 C0           test        rax,rax
  0000000140C79539: 74 0A              je          0000000140C79545
  0000000140C7953B: C7
  0000000140C7953C: 44 24 20           and         al,20h
  0000000140C7953F: 01

  Summary

     1E02000 .text
