Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065370BE0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065370BE5: 57                 push        rdi
  0000000065370BE6: 48 83 EC 30        sub         rsp,30h
  0000000065370BEA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065370BF3: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065370BF8: 8B DA              mov         ebx,edx
  0000000065370BFA: 48 8B F9           mov         rdi,rcx
  0000000065370BFD: BA 04 08 04 00     mov         edx,40804h
  0000000065370C02: E8 79 DB CE FF     call        000000006505E780
  0000000065370C07: 90                 nop
  0000000065370C08: 48 8D 05 09 28 38  lea         rax,[00000000656F3418h]
                    00
  0000000065370C0F: 48 89 07           mov         qword ptr [rdi],rax
  0000000065370C12: 48 8D 8F E0 01 00  lea         rcx,[rdi+00000000000001E0h]
                    00
  0000000065370C19: FF 15 49 5B 32 00  call        qword ptr [0000000065696768h]
  0000000065370C1F: 90                 nop
  0000000065370C20: 48 8D 8F E8 01 00  lea         rcx,[rdi+00000000000001E8h]
                    00
  0000000065370C27: E8 04 35 CA FF     call        0000000065014130
  0000000065370C2C: 90                 nop
  0000000065370C2D: 48 8D 8F F0 01 00  lea         rcx,[rdi+00000000000001F0h]
                    00
  0000000065370C34: FF 15 3E 5B 32 00  call        qword ptr [0000000065696778h]
  0000000065370C3A: 48 8D 8F F8 01 00  lea         rcx,[rdi+00000000000001F8h]
                    00
  0000000065370C41: E8 4A FD CC FF     call        0000000065040990
  0000000065370C46: 90                 nop
  0000000065370C47: 33 C0              xor         eax,eax
  0000000065370C49: 89 87 00 02 00 00  mov         dword ptr [rdi+0000000000000200h],eax
  0000000065370C4F: 83 A7 04 02 00 00  and         dword ptr [rdi+0000000000000204h],0FFFFFF80h
                    80
  0000000065370C56: 48 89 87 08 02 00  mov         qword ptr [rdi+0000000000000208h],rax
                    00
  0000000065370C5D: 48 8D 8F 10 02 00  lea         rcx,[rdi+0000000000000210h]
                    00
  0000000065370C64: FF 15 D6 5F 32 00  call        qword ptr [0000000065696C40h]
  0000000065370C6A: 90                 nop
  0000000065370C6B: 48 8D 8F 14 02 00  lea         rcx,[rdi+0000000000000214h]
                    00
  0000000065370C72: FF 15 C8 5F 32 00  call        qword ptr [0000000065696C40h]
  0000000065370C78: 90                 nop
  0000000065370C79: C7 87 18 02 00 00  mov         dword ptr [rdi+0000000000000218h],12Ch
                    2C 01 00 00
  0000000065370C83: C7 87 1C 02 00 00  mov         dword ptr [rdi+000000000000021Ch],64h
                    64 00 00 00
  0000000065370C8D: 89 9F 20 02 00 00  mov         dword ptr [rdi+0000000000000220h],ebx
  0000000065370C93: 48 8D 8F 24 02 00  lea         rcx,[rdi+0000000000000224h]
                    00
  0000000065370C9A: FF 15 D8 5A 32 00  call        qword ptr [0000000065696778h]
  0000000065370CA0: 90                 nop
  0000000065370CA1: 48 8B C7           mov         rax,rdi
  0000000065370CA4: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065370CA9: 48 83 C4 30        add         rsp,30h
  0000000065370CAD: 5F                 pop         rdi
  0000000065370CAE: C3                 ret

  Summary

      695000 .text
