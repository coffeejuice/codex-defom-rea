Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D34730: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D34735: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3473A: 48 83 EC 48        sub         rsp,48h
  0000000140D3473E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D34747: 48 8D 44 24 20     lea         rax,[rsp+20h]
  0000000140D3474C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D34751: 33 D2              xor         edx,edx
  0000000140D34753: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D34758: E8 13 25 81 FF     call        0000000140546C70
  0000000140D3475D: 4C 8B C0           mov         r8,rax
  0000000140D34760: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D34765: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D3476A: FF 15 10 73 0D 01  call        qword ptr [0000000141E0BA80h]
  0000000140D34770: 90                 nop
  0000000140D34771: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D34776: 48 8D 05 1B 26 CD  lea         rax,[0000000142A06D98h]
                    01
  0000000140D3477D: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D34780: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D34785: 48 8D 05 D4 25 CD  lea         rax,[0000000142A06D60h]
                    01
  0000000140D3478C: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140D34790: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D34795: 48 83 C1 28        add         rcx,28h
  0000000140D34799: E8 D2 89 42 FF     call        000000014015D170
  0000000140D3479E: 90                 nop
  0000000140D3479F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D347A4: 48 83 C1 30        add         rcx,30h
  0000000140D347A8: FF 15 12 FD 0C 01  call        qword ptr [0000000141E044C0h]
  0000000140D347AE: 90                 nop
  0000000140D347AF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D347B4: E8 97 00 00 00     call        0000000140D34850
  0000000140D347B9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D347BE: E8 AD 00 00 00     call        0000000140D34870
  0000000140D347C3: 90                 nop
  0000000140D347C4: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D347C9: 48 83 C4 48        add         rsp,48h
  0000000140D347CD: C3                 ret

  Summary

     1E02000 .text
