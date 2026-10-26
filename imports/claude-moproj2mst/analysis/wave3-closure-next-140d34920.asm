Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D34920: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D34925: 48 83 EC 48        sub         rsp,48h
  0000000140D34929: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D3492E: 48 83 C1 28        add         rcx,28h
  0000000140D34932: E8 B9 65 EE FF     call        0000000140C1AEF0
  0000000140D34937: 48 85 C0           test        rax,rax
  0000000140D3493A: 74 4A              je          0000000140D34986
  0000000140D3493C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D34941: 48 83 C1 28        add         rcx,28h
  0000000140D34945: E8 A6 65 EE FF     call        0000000140C1AEF0
  0000000140D3494A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D3494F: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D34954: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D34959: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140D3495F: 74 1C              je          0000000140D3497D
  0000000140D34961: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D34966: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D34969: BA 01 00 00 00     mov         edx,1
  0000000140D3496E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D34973: FF 50 18           call        qword ptr [rax+18h]
  0000000140D34976: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D3497B: EB 09              jmp         0000000140D34986
  0000000140D3497D: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140D34986: 48 83 C4 48        add         rsp,48h
  0000000140D3498A: C3                 ret

  Summary

     1E02000 .text
