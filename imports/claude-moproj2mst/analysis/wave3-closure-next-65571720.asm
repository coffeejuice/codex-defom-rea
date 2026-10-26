Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065571720: 41 54              push        r12
  0000000065571722: 48 83 EC 50        sub         rsp,50h
  0000000065571726: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006557172F: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065571734: 48 89 6C 24 68     mov         qword ptr [rsp+68h],rbp
  0000000065571739: 48 89 74 24 70     mov         qword ptr [rsp+70h],rsi
  000000006557173E: 48 89 7C 24 78     mov         qword ptr [rsp+78h],rdi
  0000000065571743: 4C 8B E1           mov         r12,rcx
  0000000065571746: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  000000006557174A: FF 87 00 01 00 00  inc         dword ptr [rdi+0000000000000100h]
  0000000065571750: 48 8B 87 08 01 00  mov         rax,qword ptr [rdi+0000000000000108h]
                    00
  0000000065571757: 83 78 14 00        cmp         dword ptr [rax+14h],0
  000000006557175B: 40 0F 95 C6        setne       sil
  000000006557175F: 48 8D 97 08 01 00  lea         rdx,[rdi+0000000000000108h]
                    00
  0000000065571766: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006557176B: E8 40 BD FB FF     call        000000006552D4B0
  0000000065571770: 90                 nop
  0000000065571771: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  0000000065571776: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  0000000065571780: 8B 4C 24 30        mov         ecx,dword ptr [rsp+30h]
  0000000065571784: 8B C1              mov         eax,ecx
  0000000065571786: FF C1              inc         ecx
  0000000065571788: 89 4C 24 30        mov         dword ptr [rsp+30h],ecx
  000000006557178C: 85 C0              test        eax,eax
  000000006557178E: 75 3F              jne         00000000655717CF
  0000000065571790: 48 3B 54 24 40     cmp         rdx,qword ptr [rsp+40h]
  0000000065571795: 74 38              je          00000000655717CF
  0000000065571797: 48 8B 5A 10        mov         rbx,qword ptr [rdx+10h]
  000000006557179B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000655717A0: 85 C9              test        ecx,ecx
  00000000655717A2: 74 16              je          00000000655717BA
  00000000655717A4: 33 D2              xor         edx,edx
  00000000655717A6: 48 8B CB           mov         rcx,rbx
  00000000655717A9: E8 42 DB FD FF     call        000000006554F2F0
  00000000655717AE: 8B 4C 24 30        mov         ecx,dword ptr [rsp+30h]
  00000000655717B2: FF C9              dec         ecx
  00000000655717B4: 89 4C 24 30        mov         dword ptr [rsp+30h],ecx
  00000000655717B8: EB E6              jmp         00000000655717A0
  00000000655717BA: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000000655717BF: FF 15 83 53 12 00  call        qword ptr [0000000065696B48h]
  00000000655717C5: 48 8B D0           mov         rdx,rax
  00000000655717C8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000655717CD: EB B1              jmp         0000000065571780
  00000000655717CF: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000655717D4: 48 83 C1 10        add         rcx,10h
  00000000655717D8: FF 15 9A 51 12 00  call        qword ptr [0000000065696978h]
  00000000655717DE: 84 C0              test        al,al
  00000000655717E0: 75 12              jne         00000000655717F4
  00000000655717E2: 48 8D 15 C7 DA B2  lea         rdx,[000000006509F2B0h]
                    FF
  00000000655717E9: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000655717EE: FF 15 BC 53 12 00  call        qword ptr [0000000065696BB0h]
  00000000655717F4: 48 8D 8F 08 01 00  lea         rcx,[rdi+0000000000000108h]
                    00
  00000000655717FB: E8 D0 4A FF FF     call        00000000655662D0
  0000000065571800: 83 87 00 01 00 00  add         dword ptr [rdi+0000000000000100h],0FFFFFFFFh
                    FF
  0000000065571807: 75 1C              jne         0000000065571825
  0000000065571809: 40 84 F6           test        sil,sil
  000000006557180C: 74 17              je          0000000065571825
  000000006557180E: 45 33 C9           xor         r9d,r9d
  0000000065571811: 45 8D 41 02        lea         r8d,[r9+2]
  0000000065571815: 48 8D 15 D4 62 45  lea         rdx,[00000000659C7AF0h]
                    00
  000000006557181C: 49 8B CC           mov         rcx,r12
  000000006557181F: FF 15 8B 50 12 00  call        qword ptr [00000000656968B0h]
  0000000065571825: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  000000006557182A: 48 8B 6C 24 68     mov         rbp,qword ptr [rsp+68h]
  000000006557182F: 48 8B 74 24 70     mov         rsi,qword ptr [rsp+70h]
  0000000065571834: 48 8B 7C 24 78     mov         rdi,qword ptr [rsp+78h]
  0000000065571839: 48 83 C4 50        add         rsp,50h
  000000006557183D: 41 5C              pop         r12
  000000006557183F: C3                 ret

  Summary

      695000 .text
