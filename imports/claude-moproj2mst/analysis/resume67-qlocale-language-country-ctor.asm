Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704D5A0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006704D5A5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006704D5AA: 57                 push        rdi
  000000006704D5AB: 48 83 EC 20        sub         rsp,20h
  000000006704D5AF: 48 8B D9           mov         rbx,rcx
  000000006704D5B2: 33 FF              xor         edi,edi
  000000006704D5B4: 8B C2              mov         eax,edx
  000000006704D5B6: 48 89 39           mov         qword ptr [rcx],rdi
  000000006704D5B9: 48 8D 0D 90 48 16  lea         rcx,[00000000671B1E50h]
                    00
  000000006704D5C0: 48 8D 35 79 4A 16  lea         rsi,[00000000671B2040h]
                    00
  000000006704D5C7: 44 0F B7 14 41     movzx       r10d,word ptr [rcx+rax*2]
  000000006704D5CC: 45 8B D8           mov         r11d,r8d
  000000006704D5CF: 4B 8D 0C D2        lea         rcx,[r10+r10*8]
  000000006704D5D3: 48 C1 E1 04        shl         rcx,4
  000000006704D5D7: 48 03 CE           add         rcx,rsi
  000000006704D5DA: 4C 8B C1           mov         r8,rcx
  000000006704D5DD: 45 85 D2           test        r10d,r10d
  000000006704D5E0: 74 31              je          000000006704D613
  000000006704D5E2: 45 85 DB           test        r11d,r11d
  000000006704D5E5: 74 2C              je          000000006704D613
  000000006704D5E7: 0F B7 01           movzx       eax,word ptr [rcx]
  000000006704D5EA: 3B C2              cmp         eax,edx
  000000006704D5EC: 75 22              jne         000000006704D610
  000000006704D5EE: 66 90              nop
  000000006704D5F0: 66 41 39 78 02     cmp         word ptr [r8+2],di
  000000006704D5F5: 75 0A              jne         000000006704D601
  000000006704D5F7: 41 0F B7 40 04     movzx       eax,word ptr [r8+4]
  000000006704D5FC: 41 3B C3           cmp         eax,r11d
  000000006704D5FF: 74 12              je          000000006704D613
  000000006704D601: 49 81 C0 90 00 00  add         r8,90h
                    00
  000000006704D608: 41 0F B7 00        movzx       eax,word ptr [r8]
  000000006704D60C: 3B C2              cmp         eax,edx
  000000006704D60E: 74 E0              je          000000006704D5F0
  000000006704D610: 4C 8B C1           mov         r8,rcx
  000000006704D613: 66 41 83 38 01     cmp         word ptr [r8],1
  000000006704D618: 75 62              jne         000000006704D67C
  000000006704D61A: 83 FA 01           cmp         edx,1
  000000006704D61D: 74 5D              je          000000006704D67C
  000000006704D61F: 48 8B 0D 1A 67 2A  mov         rcx,qword ptr [00000000672F3D40h]
                    00
  000000006704D626: 0F B7 05 1B 67 2A  movzx       eax,word ptr [00000000672F3D48h]
                    00
  000000006704D62D: 66 89 43 02        mov         word ptr [rbx+2],ax
  000000006704D631: 48 85 C9           test        rcx,rcx
  000000006704D634: 75 2B              jne         000000006704D661
  000000006704D636: 48 8B 05 FB 66 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  000000006704D63D: 48 85 C0           test        rax,rax
  000000006704D640: 74 05              je          000000006704D647
  000000006704D642: 66 39 38           cmp         word ptr [rax],di
  000000006704D645: 75 0C              jne         000000006704D653
  000000006704D647: E8 E4 F6 FF FF     call        000000006704CD30
  000000006704D64C: 48 8B 05 E5 66 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  000000006704D653: 48 89 05 E6 66 2A  mov         qword ptr [00000000672F3D40h],rax
                    00
  000000006704D65A: B8 87 01 00 00     mov         eax,187h
  000000006704D65F: EB 4D              jmp         000000006704D6AE
  000000006704D661: 48 3B 0D D0 66 2A  cmp         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704D668: 74 1F              je          000000006704D689
  000000006704D66A: 48 2B CE           sub         rcx,rsi
  000000006704D66D: 48 B8 39 8E E3 38  mov         rax,0E38E38E38E38E39h
                    8E E3 38 0E
  000000006704D677: 48 F7 E9           imul        rcx
  000000006704D67A: EB 24              jmp         000000006704D6A0
  000000006704D67C: 66 89 7B 02        mov         word ptr [rbx+2],di
  000000006704D680: 4C 3B 05 B1 66 2A  cmp         r8,qword ptr [00000000672F3D38h]
                    00
  000000006704D687: 75 07              jne         000000006704D690
  000000006704D689: B8 87 01 00 00     mov         eax,187h
  000000006704D68E: EB 1E              jmp         000000006704D6AE
  000000006704D690: 4C 2B C6           sub         r8,rsi
  000000006704D693: 48 B8 39 8E E3 38  mov         rax,0E38E38E38E38E39h
                    8E E3 38 0E
  000000006704D69D: 49 F7 E8           imul        r8
  000000006704D6A0: 48 C1 FA 03        sar         rdx,3
  000000006704D6A4: 48 8B C2           mov         rax,rdx
  000000006704D6A7: 48 C1 E8 3F        shr         rax,3Fh
  000000006704D6AB: 48 03 C2           add         rax,rdx
  000000006704D6AE: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006704D6B3: 66 89 03           mov         word ptr [rbx],ax
  000000006704D6B6: 48 8B C3           mov         rax,rbx
  000000006704D6B9: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006704D6BE: 48 83 C4 20        add         rsp,20h
  000000006704D6C2: 5F                 pop         rdi
  000000006704D6C3: C3                 ret
  000000006704D6C4: CC                 int         3
  000000006704D6C5: CC                 int         3
  000000006704D6C6: CC                 int         3
  000000006704D6C7: CC                 int         3
  000000006704D6C8: CC                 int         3
  000000006704D6C9: CC                 int         3
  000000006704D6CA: CC                 int         3
  000000006704D6CB: CC                 int         3
  000000006704D6CC: CC                 int         3
  000000006704D6CD: CC                 int         3
  000000006704D6CE: CC                 int         3
  000000006704D6CF: CC                 int         3
  000000006704D6D0: 48

  Summary

      1A7000 .text
