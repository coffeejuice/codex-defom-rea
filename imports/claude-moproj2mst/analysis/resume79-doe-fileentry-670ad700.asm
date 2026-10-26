Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670AD700: 40 57              push        rdi
  00000000670AD702: 48 83 EC 30        sub         rsp,30h
  00000000670AD706: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670AD70F: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670AD714: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  00000000670AD719: 48 8B F2           mov         rsi,rdx
  00000000670AD71C: 48 8B F9           mov         rdi,rcx
  00000000670AD71F: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  00000000670AD727: B8 5C 00 00 00     mov         eax,5Ch
  00000000670AD72C: 66 89 44 24 50     mov         word ptr [rsp+50h],ax
  00000000670AD731: 44 8D 48 A5        lea         r9d,[rax-5Bh]
  00000000670AD735: 45 33 C0           xor         r8d,r8d
  00000000670AD738: 0F B7 D0           movzx       edx,ax
  00000000670AD73B: 48 8B CE           mov         rcx,rsi
  00000000670AD73E: E8 2D 07 FC FF     call        000000006706DE70
  00000000670AD743: 48 63 D8           movsxd      rbx,eax
  00000000670AD746: 83 FB FF           cmp         ebx,0FFFFFFFFh
  00000000670AD749: 0F 84 93 00 00 00  je          00000000670AD7E2
  00000000670AD74F: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000670AD752: 48 89 4C 24 58     mov         qword ptr [rsp+58h],rcx
  00000000670AD757: F0 83 01 01        lock add    dword ptr [rcx],1
  00000000670AD75B: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670AD760: 8B 11              mov         edx,dword ptr [rcx]
  00000000670AD762: 83 FA 01           cmp         edx,1
  00000000670AD765: 75 0F              jne         00000000670AD776
  00000000670AD767: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000670AD76C: 48 8D 48 1A        lea         rcx,[rax+1Ah]
  00000000670AD770: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  00000000670AD774: 74 0F              je          00000000670AD785
  00000000670AD776: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000670AD77B: E8 40 2C FC FF     call        00000000670703C0
  00000000670AD780: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000670AD785: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670AD789: 41 B8 2F 00 00 00  mov         r8d,2Fh
  00000000670AD78F: 66 44 89 04 59     mov         word ptr [rcx+rbx*2],r8w
  00000000670AD794: FF C3              inc         ebx
  00000000670AD796: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000670AD79B: 48 63 D3           movsxd      rdx,ebx
  00000000670AD79E: 3B 58 08           cmp         ebx,dword ptr [rax+8]
  00000000670AD7A1: 7D 1E              jge         00000000670AD7C1
  00000000670AD7A3: 48 8D 0C 51        lea         rcx,[rcx+rdx*2]
  00000000670AD7A7: 66 83 39 5C        cmp         word ptr [rcx],5Ch
  00000000670AD7AB: 75 09              jne         00000000670AD7B6
  00000000670AD7AD: 66 44 89 01        mov         word ptr [rcx],r8w
  00000000670AD7B1: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000670AD7B6: FF C3              inc         ebx
  00000000670AD7B8: 48 83 C1 02        add         rcx,2
  00000000670AD7BC: 3B 58 08           cmp         ebx,dword ptr [rax+8]
  00000000670AD7BF: 7C E6              jl          00000000670AD7A7
  00000000670AD7C1: 48 89 07           mov         qword ptr [rdi],rax
  00000000670AD7C4: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670AD7C8: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000000670AD7CD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670AD7D1: 75 0A              jne         00000000670AD7DD
  00000000670AD7D3: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670AD7D8: E8 F3 28 FC FF     call        00000000670700D0
  00000000670AD7DD: 48 8B C7           mov         rax,rdi
  00000000670AD7E0: EB 0D              jmp         00000000670AD7EF
  00000000670AD7E2: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000670AD7E5: 48 89 07           mov         qword ptr [rdi],rax
  00000000670AD7E8: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670AD7EC: 48 8B C7           mov         rax,rdi
  00000000670AD7EF: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000670AD7F4: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000670AD7F9: 48 83 C4 30        add         rsp,30h
  00000000670AD7FD: 5F                 pop         rdi
  00000000670AD7FE: C3                 ret

  Summary

      1A7000 .text
