Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701C900: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006701C905: 57                 push        rdi
  000000006701C906: 48 83 EC 30        sub         rsp,30h
  000000006701C90A: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006701C913: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006701C918: 48 8B FA           mov         rdi,rdx
  000000006701C91B: 48 8B D9           mov         rbx,rcx
  000000006701C91E: 33 C0              xor         eax,eax
  000000006701C920: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000006701C924: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006701C929: 48 8D 05 D0 3F 2D  lea         rax,[00000000672F0900h]
                    00
  000000006701C930: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006701C935: F0 83 05 C3 3F 2D  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006701C93D: 4C 8B CA           mov         r9,rdx
  000000006701C940: 45 33 C0           xor         r8d,r8d
  000000006701C943: 33 D2              xor         edx,edx
  000000006701C945: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006701C94A: FF 15 98 BC 18 00  call        qword ptr [00000000671A85E8h]
  000000006701C950: 4C 8B 5C 24 20     mov         r11,qword ptr [rsp+20h]
  000000006701C955: 4D 85 DB           test        r11,r11
  000000006701C958: 75 31              jne         000000006701C98B
  000000006701C95A: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000006701C95F: 48 89 03           mov         qword ptr [rbx],rax
  000000006701C962: F0 83 00 01        lock add    dword ptr [rax],1
  000000006701C966: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  000000006701C96E: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000006701C973: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006701C977: 75 0A              jne         000000006701C983
  000000006701C979: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006701C97E: E8 2D 3A 00 00     call        00000000670203B0
  000000006701C983: 48 8B C3           mov         rax,rbx
  000000006701C986: E9 8E 00 00 00     jmp         000000006701CA19
  000000006701C98B: 41 8B D3           mov         edx,r11d
  000000006701C98E: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006701C993: E8 68 22 01 00     call        000000006702EC00
  000000006701C998: 4C 8B 5C 24 58     mov         r11,qword ptr [rsp+58h]
  000000006701C99D: 41 8B 0B           mov         ecx,dword ptr [r11]
  000000006701C9A0: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000006701C9A5: 83 F9 01           cmp         ecx,1
  000000006701C9A8: 75 0A              jne         000000006701C9B4
  000000006701C9AA: 48 8D 48 18        lea         rcx,[rax+18h]
  000000006701C9AE: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  000000006701C9B2: 74 12              je          000000006701C9C6
  000000006701C9B4: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006701C9B7: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006701C9BC: E8 4F 0D 01 00     call        000000006702D710
  000000006701C9C1: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000006701C9C6: 4C 8B CF           mov         r9,rdi
  000000006701C9C9: 4C 8B 44 24 20     mov         r8,qword ptr [rsp+20h]
  000000006701C9CE: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  000000006701C9D2: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006701C9D7: FF 15 0B BC 18 00  call        qword ptr [00000000671A85E8h]
  000000006701C9DD: BA 01 00 00 00     mov         edx,1
  000000006701C9E2: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006701C9E7: E8 04 3C 01 00     call        00000000670305F0
  000000006701C9EC: 4C 8B 5C 24 58     mov         r11,qword ptr [rsp+58h]
  000000006701C9F1: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006701C9F4: F0 41 83 03 01     lock add    dword ptr [r11],1
  000000006701C9F9: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  000000006701CA01: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000006701CA06: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006701CA0A: 75 0A              jne         000000006701CA16
  000000006701CA0C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006701CA11: E8 9A 39 00 00     call        00000000670203B0
  000000006701CA16: 48 8B C3           mov         rax,rbx
  000000006701CA19: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006701CA1E: 48 83 C4 30        add         rsp,30h
  000000006701CA22: 5F                 pop         rdi
  000000006701CA23: C3                 ret
  000000006701CA24: CC                 int         3

  Summary

      1A7000 .text
