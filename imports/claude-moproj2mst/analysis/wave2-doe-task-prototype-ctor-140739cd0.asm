
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140739CD0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140739CD5: 48 83 EC 48        sub         rsp,48h
  0000000140739CD9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140739CE2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140739CE7: E8 24 95 CC 00     call        0000000141403210
  0000000140739CEC: 90                 nop
  0000000140739CED: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140739CF2: 48 8D 05 FF EF E6  lea         rax,[00000001425A8CF8h]
                    01
  0000000140739CF9: 48 89 01           mov         qword ptr [rcx],rax
  0000000140739CFC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140739D01: 48 81 C1 B8 00 00  add         rcx,0B8h
                    00
  0000000140739D08: FF 15 B2 A7 6C 01  call        qword ptr [0000000141E044C0h]
  0000000140739D0E: 90                 nop
  0000000140739D0F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140739D14: 48 81 C1 C0 00 00  add         rcx,0C0h
                    00
  0000000140739D1B: E8 C0 B6 92 FF     call        00000001400653E0
  0000000140739D20: 90                 nop
  0000000140739D21: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140739D26: E8 A5 00 00 00     call        0000000140739DD0
  0000000140739D2B: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140739D30: 4C 8D 0D 51 EB E6  lea         r9,[00000001425A8888h]
                    01
  0000000140739D37: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140739D3C: 48 8D 15 65 EB E6  lea         rdx,[00000001425A88A8h]
                    01
  0000000140739D43: 48 8D 0D B6 C9 63  lea         rcx,[0000000143D76700h]
                    03
  0000000140739D4A: E8 91 5E B9 FF     call        00000001402CFBE0
  0000000140739D4F: 90                 nop
  0000000140739D50: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140739D55: 48 83 C4 48        add         rsp,48h
  0000000140739D59: C3                 ret

  Summary

     1E02000 .text
