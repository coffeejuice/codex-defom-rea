Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140825440: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140825445: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014082544A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014082544F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140825454: 48 83 EC 28        sub         rsp,28h
  0000000140825458: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014082545E: 75 07              jne         0000000140825467
  0000000140825460: 32 C0              xor         al,al
  0000000140825462: E9 C6 00 00 00     jmp         000000014082552D
  0000000140825467: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014082546C: 48 63 90 40 07 00  movsxd      rdx,dword ptr [rax+0000000000000740h]
                    00
  0000000140825473: 48 6B D2 28        imul        rdx,rdx,28h
  0000000140825477: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014082547C: 48 8B 80 D8 0A 00  mov         rax,qword ptr [rax+0000000000000AD8h]
                    00
  0000000140825483: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140825488: 8B 44 10 08        mov         eax,dword ptr [rax+rdx+8]
  000000014082548C: 89 01              mov         dword ptr [rcx],eax
  000000014082548E: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140825493: 48 63 90 40 07 00  movsxd      rdx,dword ptr [rax+0000000000000740h]
                    00
  000000014082549A: 48 6B D2 28        imul        rdx,rdx,28h
  000000014082549E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001408254A3: 48 8B 80 D8 0A 00  mov         rax,qword ptr [rax+0000000000000AD8h]
                    00
  00000001408254AA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001408254AF: 8B 44 10 0C        mov         eax,dword ptr [rax+rdx+0Ch]
  00000001408254B3: 89 01              mov         dword ptr [rcx],eax
  00000001408254B5: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408254BA: 48 63 90 40 07 00  movsxd      rdx,dword ptr [rax+0000000000000740h]
                    00
  00000001408254C1: 48 6B D2 28        imul        rdx,rdx,28h
  00000001408254C5: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001408254CA: 48 8B 80 D8 0A 00  mov         rax,qword ptr [rax+0000000000000AD8h]
                    00
  00000001408254D1: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001408254D6: 8B 44 10 10        mov         eax,dword ptr [rax+rdx+10h]
  00000001408254DA: 89 01              mov         dword ptr [rcx],eax
  00000001408254DC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001408254E1: 48 63 90 40 07 00  movsxd      rdx,dword ptr [rax+0000000000000740h]
                    00
  00000001408254E8: 48 6B D2 28        imul        rdx,rdx,28h
  00000001408254EC: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001408254F1: 48 8B 80 D8 0A 00  mov         rax,qword ptr [rax+0000000000000AD8h]
                    00
  00000001408254F8: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001408254FD: 8B 44 10 20        mov         eax,dword ptr [rax+rdx+20h]
  0000000140825501: 89 01              mov         dword ptr [rcx],eax
  0000000140825503: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140825508: E8 13 FF FF FF     call        0000000140825420
  000000014082550D: 44 8B D8           mov         r11d,eax
  0000000140825510: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140825515: 44 3B 18           cmp         r11d,dword ptr [rax]
  0000000140825518: 74 11              je          000000014082552B
  000000014082551A: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014082551F: 8B 10              mov         edx,dword ptr [rax]
  0000000140825521: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140825526: E8 D5 FE FF FF     call        0000000140825400
  000000014082552B: B0 01              mov         al,1
  000000014082552D: 48 83 C4 28        add         rsp,28h
  0000000140825531: C3                 ret

  Summary

     1E02000 .text
