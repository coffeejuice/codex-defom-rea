Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409F0230: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001409F0235: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409F023A: 48 83 EC 48        sub         rsp,48h
  00000001409F023E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001409F0243: 48 83 C1 40        add         rcx,40h
  00000001409F0247: E8 24 2E DE FF     call        00000001407D3070
  00000001409F024C: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  00000001409F0250: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001409F0255: E8 A6 FF FF FF     call        00000001409F0200
  00000001409F025A: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  00000001409F025F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001409F0264: E8 77 00 00 00     call        00000001409F02E0
  00000001409F0269: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001409F0271: EB 0B              jmp         00000001409F027E
  00000001409F0273: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001409F0277: 83 C0 01           add         eax,1
  00000001409F027A: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001409F027E: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  00000001409F0282: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  00000001409F0286: 7F 3C              jg          00000001409F02C4
  00000001409F0288: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001409F028D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001409F0292: E8 C9 03 00 00     call        00000001409F0660
  00000001409F0297: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001409F029C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001409F02A1: 48 83 C1 40        add         rcx,40h
  00000001409F02A5: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  00000001409F02A9: E8 12 2F EB FF     call        00000001408A31C0
  00000001409F02AE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001409F02B3: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001409F02B8: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001409F02BD: E8 9E AA EA FF     call        000000014089AD60
  00000001409F02C2: EB AF              jmp         00000001409F0273
  00000001409F02C4: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001409F02C9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001409F02CE: E8 ED FB A7 00     call        000000014146FEC0
  00000001409F02D3: 48 83 C4 48        add         rsp,48h
  00000001409F02D7: C3                 ret

  Summary

     1E02000 .text
