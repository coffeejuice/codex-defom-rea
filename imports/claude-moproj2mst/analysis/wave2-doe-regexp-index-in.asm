Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670686A0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670686A5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000000670686AA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000000670686AF: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  00000000670686B4: 41 54              push        r12
  00000000670686B6: 48 83 EC 40        sub         rsp,40h
  00000000670686BA: 48 8B 19           mov         rbx,qword ptr [rcx]
  00000000670686BD: 41 8B F1           mov         esi,r9d
  00000000670686C0: 41 8B F8           mov         edi,r8d
  00000000670686C3: 48 83 3B 00        cmp         qword ptr [rbx],0
  00000000670686C7: 48 8B EA           mov         rbp,rdx
  00000000670686CA: 4C 8B E1           mov         r12,rcx
  00000000670686CD: 75 08              jne         00000000670686D7
  00000000670686CF: 48 8B CB           mov         rcx,rbx
  00000000670686D2: E8 D9 FA FF FF     call        00000000670681B0
  00000000670686D7: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000670686DA: 48 8D 4B 30        lea         rcx,[rbx+30h]
  00000000670686DE: E8 3D 14 FF FF     call        0000000067059B20
  00000000670686E3: 48 8D 4B 20        lea         rcx,[rbx+20h]
  00000000670686E7: 48 8B D5           mov         rdx,rbp
  00000000670686EA: E8 E1 7C 00 00     call        00000000670703D0
  00000000670686EF: 48 8D 4B 28        lea         rcx,[rbx+28h]
  00000000670686F3: E8 58 21 FF FF     call        000000006705A850
  00000000670686F8: 85 FF              test        edi,edi
  00000000670686FA: 79 07              jns         0000000067068703
  00000000670686FC: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067068700: 03 78 08           add         edi,dword ptr [rax+8]
  0000000067068703: 85 F6              test        esi,esi
  0000000067068705: 75 05              jne         000000006706870C
  0000000067068707: 45 33 C0           xor         r8d,r8d
  000000006706870A: EB 0B              jmp         0000000067068717
  000000006706870C: 41 83 C8 FF        or          r8d,0FFFFFFFFh
  0000000067068710: 83 FE 01           cmp         esi,1
  0000000067068713: 44 0F 44 C7        cmove       r8d,edi
  0000000067068717: 49 8B 04 24        mov         rax,qword ptr [r12]
  000000006706871B: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  000000006706871F: 44 89 44 24 30     mov         dword ptr [rsp+30h],r8d
  0000000067068724: 44 8B 42 08        mov         r8d,dword ptr [rdx+8]
  0000000067068728: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000006706872C: 48 8D 48 30        lea         rcx,[rax+30h]
  0000000067068730: 0F B6 40 18        movzx       eax,byte ptr [rax+18h]
  0000000067068734: 44 8B CF           mov         r9d,edi
  0000000067068737: C6 44 24 28 00     mov         byte ptr [rsp+28h],0
  000000006706873C: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000067068740: E8 FB E0 FF FF     call        0000000067066840
  0000000067068745: 4D 8B 1C 24        mov         r11,qword ptr [r12]
  0000000067068749: 49 8B 83 B0 00 00  mov         rax,qword ptr [r11+00000000000000B0h]
                    00
  0000000067068750: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  0000000067068755: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  000000006706875A: 8B 00              mov         eax,dword ptr [rax]
  000000006706875C: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  0000000067068761: 48
  0000000067068762: 8B
  0000000067068763: 7C 24              jl          0000000067068789

  Summary

      1A7000 .text
