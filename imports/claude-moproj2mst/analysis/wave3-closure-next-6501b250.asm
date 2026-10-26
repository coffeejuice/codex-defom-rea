Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006501B250: 40 53              push        rbx
  000000006501B252: 48 83 EC 30        sub         rsp,30h
  000000006501B256: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006501B25F: 48 8B 05 02 E5 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B266: 48 85 C0           test        rax,rax
  000000006501B269: 0F 85 AD 00 00 00  jne         000000006501B31C
  000000006501B26F: 38 05 FB E4 9A 00  cmp         byte ptr [00000000659C9770h],al
  000000006501B275: 0F 85 98 00 00 00  jne         000000006501B313
  000000006501B27B: 8D 48 08           lea         ecx,[rax+8]
  000000006501B27E: E8 EF 5E 5C 00     call        00000000655E1172
  000000006501B283: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006501B288: 48 85 C0           test        rax,rax
  000000006501B28B: 74 10              je          000000006501B29D
  000000006501B28D: 33 D2              xor         edx,edx
  000000006501B28F: 48 8B C8           mov         rcx,rax
  000000006501B292: FF 15 60 B9 67 00  call        qword ptr [0000000065696BF8h]
  000000006501B298: 48 8B D8           mov         rbx,rax
  000000006501B29B: EB 02              jmp         000000006501B29F
  000000006501B29D: 33 DB              xor         ebx,ebx
  000000006501B29F: 33 C0              xor         eax,eax
  000000006501B2A1: F0 48 0F B1 1D BE  lock cmpxchg qword ptr [00000000659C9768h],rbx
                    E4 9A 00
  000000006501B2AA: 74 28              je          000000006501B2D4
  000000006501B2AC: 48 85 DB           test        rbx,rbx
  000000006501B2AF: 74 1A              je          000000006501B2CB
  000000006501B2B1: 48 8B CB           mov         rcx,rbx
  000000006501B2B4: FF 15 3E B8 67 00  call        qword ptr [0000000065696AF8h]
  000000006501B2BA: 48 8B CB           mov         rcx,rbx
  000000006501B2BD: E8 EC 5D 5C 00     call        00000000655E10AE
  000000006501B2C2: 48 8B 05 9F E4 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B2C9: EB 58              jmp         000000006501B323
  000000006501B2CB: 48 8B 05 96 E4 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B2D2: EB 4F              jmp         000000006501B323
  000000006501B2D4: 8B 05 86 E4 9A 00  mov         eax,dword ptr [00000000659C9760h]
  000000006501B2DA: A8 01              test        al,1
  000000006501B2DC: 75 2C              jne         000000006501B30A
  000000006501B2DE: 83 C8 01           or          eax,1
  000000006501B2E1: 89 05 79 E4 9A 00  mov         dword ptr [00000000659C9760h],eax
  000000006501B2E7: 48 8D 05 7A E4 9A  lea         rax,[00000000659C9768h]
                    00
  000000006501B2EE: 48 89 05 63 E4 9A  mov         qword ptr [00000000659C9758h],rax
                    00
  000000006501B2F5: 48 8D 0D 34 93 67  lea         rcx,[0000000065694630h]
                    00
  000000006501B2FC: E8 2B 5D 5C 00     call        00000000655E102C
  000000006501B301: 48 8B 05 60 E4 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B308: EB 19              jmp         000000006501B323
  000000006501B30A: 48 8B 05 57 E4 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B311: EB 10              jmp         000000006501B323
  000000006501B313: 48 8B 05 4E E4 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B31A: EB 07              jmp         000000006501B323
  000000006501B31C: 48 8B 05 45 E4 9A  mov         rax,qword ptr [00000000659C9768h]
                    00
  000000006501B323: 48 83 C4 30        add         rsp,30h
  000000006501B327: 5B                 pop         rbx
  000000006501B328: C3                 ret

  Summary

      695000 .text
