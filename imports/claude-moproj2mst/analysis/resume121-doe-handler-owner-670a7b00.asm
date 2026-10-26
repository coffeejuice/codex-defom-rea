Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7B00: 40 53              push        rbx
  00000000670A7B02: 48 83 EC 30        sub         rsp,30h
  00000000670A7B06: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670A7B0F: 48 8B 05 D2 C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7B16: 48 85 C0           test        rax,rax
  00000000670A7B19: 0F 85 AE 00 00 00  jne         00000000670A7BCD
  00000000670A7B1F: 38 05 CB C3 24 00  cmp         byte ptr [00000000672F3EF0h],al
  00000000670A7B25: 0F 85 99 00 00 00  jne         00000000670A7BC4
  00000000670A7B2B: 8D 48 08           lea         ecx,[rax+8]
  00000000670A7B2E: E8 53 0F 0D 00     call        0000000067178A86
  00000000670A7B33: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670A7B38: 48 85 C0           test        rax,rax
  00000000670A7B3B: 74 12              je          00000000670A7B4F
  00000000670A7B3D: BA 01 00 00 00     mov         edx,1
  00000000670A7B42: 48 8B C8           mov         rcx,rax
  00000000670A7B45: E8 16 E4 F7 FF     call        0000000067025F60
  00000000670A7B4A: 48 8B D8           mov         rbx,rax
  00000000670A7B4D: EB 02              jmp         00000000670A7B51
  00000000670A7B4F: 33 DB              xor         ebx,ebx
  00000000670A7B51: 33 C0              xor         eax,eax
  00000000670A7B53: F0 48 0F B1 1D 8C  lock cmpxchg qword ptr [00000000672F3EE8h],rbx
                    C3 24 00
  00000000670A7B5C: 74 27              je          00000000670A7B85
  00000000670A7B5E: 48 85 DB           test        rbx,rbx
  00000000670A7B61: 74 19              je          00000000670A7B7C
  00000000670A7B63: 48 8B CB           mov         rcx,rbx
  00000000670A7B66: E8 A5 E8 F7 FF     call        0000000067026410
  00000000670A7B6B: 48 8B CB           mov         rcx,rbx
  00000000670A7B6E: E8 E3 0D 0D 00     call        0000000067178956
  00000000670A7B73: 48 8B 05 6E C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7B7A: EB 58              jmp         00000000670A7BD4
  00000000670A7B7C: 48 8B 05 65 C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7B83: EB 4F              jmp         00000000670A7BD4
  00000000670A7B85: 8B 05 55 C3 24 00  mov         eax,dword ptr [00000000672F3EE0h]
  00000000670A7B8B: A8 01              test        al,1
  00000000670A7B8D: 75 2C              jne         00000000670A7BBB
  00000000670A7B8F: 83 C8 01           or          eax,1
  00000000670A7B92: 89 05 48 C3 24 00  mov         dword ptr [00000000672F3EE0h],eax
  00000000670A7B98: 48 8D 05 49 C3 24  lea         rax,[00000000672F3EE8h]
                    00
  00000000670A7B9F: 48 89 05 32 C3 24  mov         qword ptr [00000000672F3ED8h],rax
                    00
  00000000670A7BA6: 48 8D 0D 83 E9 0F  lea         rcx,[00000000671A6530h]
                    00
  00000000670A7BAD: E8 A2 0F 0D 00     call        0000000067178B54
  00000000670A7BB2: 48 8B 05 2F C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7BB9: EB 19              jmp         00000000670A7BD4
  00000000670A7BBB: 48 8B 05 26 C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7BC2: EB 10              jmp         00000000670A7BD4
  00000000670A7BC4: 48 8B 05 1D C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7BCB: EB 07              jmp         00000000670A7BD4
  00000000670A7BCD: 48 8B 05 14 C3 24  mov         rax,qword ptr [00000000672F3EE8h]
                    00
  00000000670A7BD4: 48 83 C4 30        add         rsp,30h
  00000000670A7BD8: 5B                 pop         rbx
  00000000670A7BD9: C3                 ret

  Summary

      1A7000 .text
