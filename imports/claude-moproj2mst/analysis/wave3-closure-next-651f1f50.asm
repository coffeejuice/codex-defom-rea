Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F1F50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000651F1F55: 57                 push        rdi
  00000000651F1F56: 48 83 EC 30        sub         rsp,30h
  00000000651F1F5A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000651F1F63: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000651F1F68: 48 8B DA           mov         rbx,rdx
  00000000651F1F6B: 48 8B F9           mov         rdi,rcx
  00000000651F1F6E: 33 D2              xor         edx,edx
  00000000651F1F70: FF 15 02 51 4A 00  call        qword ptr [0000000065697078h]
  00000000651F1F76: 48 8D 53 08        lea         rdx,[rbx+8]
  00000000651F1F7A: 48 8D 4F 08        lea         rcx,[rdi+8]
  00000000651F1F7E: E8 8D FB FF FF     call        00000000651F1B10
  00000000651F1F83: 90                 nop
  00000000651F1F84: 48 C7 47 30 00 00  mov         qword ptr [rdi+30h],0
                    00 00
  00000000651F1F8C: 8B 43 38           mov         eax,dword ptr [rbx+38h]
  00000000651F1F8F: 89 47 38           mov         dword ptr [rdi+38h],eax
  00000000651F1F92: 8B 43 3C           mov         eax,dword ptr [rbx+3Ch]
  00000000651F1F95: 89 47 3C           mov         dword ptr [rdi+3Ch],eax
  00000000651F1F98: 8B 47 48           mov         eax,dword ptr [rdi+48h]
  00000000651F1F9B: 33 43 48           xor         eax,dword ptr [rbx+48h]
  00000000651F1F9E: 83 E0 01           and         eax,1
  00000000651F1FA1: 31 47 48           xor         dword ptr [rdi+48h],eax
  00000000651F1FA4: 8B 47 48           mov         eax,dword ptr [rdi+48h]
  00000000651F1FA7: 8B 4B 48           mov         ecx,dword ptr [rbx+48h]
  00000000651F1FAA: 33 C8              xor         ecx,eax
  00000000651F1FAC: 83 E1 02           and         ecx,2
  00000000651F1FAF: 33 C8              xor         ecx,eax
  00000000651F1FB1: 89 4F 48           mov         dword ptr [rdi+48h],ecx
  00000000651F1FB4: 8B 43 48           mov         eax,dword ptr [rbx+48h]
  00000000651F1FB7: 33 C1              xor         eax,ecx
  00000000651F1FB9: 83 E0 04           and         eax,4
  00000000651F1FBC: 33 C1              xor         eax,ecx
  00000000651F1FBE: 89 47 48           mov         dword ptr [rdi+48h],eax
  00000000651F1FC1: 8B 4B 48           mov         ecx,dword ptr [rbx+48h]
  00000000651F1FC4: 33 C8              xor         ecx,eax
  00000000651F1FC6: 83 E1 08           and         ecx,8
  00000000651F1FC9: 33 C8              xor         ecx,eax
  00000000651F1FCB: 89 4F 48           mov         dword ptr [rdi+48h],ecx
  00000000651F1FCE: 8B 53 48           mov         edx,dword ptr [rbx+48h]
  00000000651F1FD1: 33 D1              xor         edx,ecx
  00000000651F1FD3: 83 E2 10           and         edx,10h
  00000000651F1FD6: 33 D1              xor         edx,ecx
  00000000651F1FD8: 89 57 48           mov         dword ptr [rdi+48h],edx
  00000000651F1FDB: 8B 43 48           mov         eax,dword ptr [rbx+48h]
  00000000651F1FDE: 33 C2              xor         eax,edx
  00000000651F1FE0: 25 E0 00 00 00     and         eax,0E0h
  00000000651F1FE5: 33 C2              xor         eax,edx
  00000000651F1FE7: 89 47 48           mov         dword ptr [rdi+48h],eax
  00000000651F1FEA: 0F B6 43 4C        movzx       eax,byte ptr [rbx+4Ch]
  00000000651F1FEE: 32 47 4C           xor         al,byte ptr [rdi+4Ch]
  00000000651F1FF1: 24 01              and         al,1
  00000000651F1FF3: 30 47 4C           xor         byte ptr [rdi+4Ch],al
  00000000651F1FF6: 8B 43 50           mov         eax,dword ptr [rbx+50h]
  00000000651F1FF9: 89 47 50           mov         dword ptr [rdi+50h],eax
  00000000651F1FFC: 8B 43 54           mov         eax,dword ptr [rbx+54h]
  00000000651F1FFF: 89 47 54           mov         dword ptr [rdi+54h],eax
  00000000651F2002: 48 8B 43 58        mov         rax,qword ptr [rbx+58h]
  00000000651F2006: 48 89 47 58        mov         qword ptr [rdi+58h],rax
  00000000651F200A: 48 8B 43 40        mov         rax,qword ptr [rbx+40h]
  00000000651F200E: 48 89 47 40        mov         qword ptr [rdi+40h],rax
  00000000651F2012: 48 8B 4F 58        mov         rcx,qword ptr [rdi+58h]
  00000000651F2016: 48 85 C9           test        rcx,rcx
  00000000651F2019: 74 16              je          00000000651F2031
  00000000651F201B: 48 3B CF           cmp         rcx,rdi
  00000000651F201E: 74 0C              je          00000000651F202C
  00000000651F2020: FF 15 9A 48 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F2026: 90                 nop
  00000000651F2027: 48 8B C7           mov         rax,rdi
  00000000651F202A: EB 08              jmp         00000000651F2034
  00000000651F202C: 48 8B C7           mov         rax,rdi
  00000000651F202F: EB 03              jmp         00000000651F2034
  00000000651F2031: 48 8B C7           mov         rax,rdi
  00000000651F2034: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000651F2039: 48 83 C4 30        add         rsp,30h
  00000000651F203D: 5F                 pop         rdi
  00000000651F203E: C3                 ret

  Summary

      695000 .text
