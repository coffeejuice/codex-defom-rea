Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006702D210: 48 89 5C 24 18     mov         qword ptr [rsp+18h],rbx
  000000006702D215: 57                 push        rdi
  000000006702D216: 48 83 EC 20        sub         rsp,20h
  000000006702D21A: 0F B7 41 0E        movzx       eax,word ptr [rcx+0Eh]
  000000006702D21E: 66 44 8B 0A        mov         r9w,word ptr [rdx]
  000000006702D222: 0F B7 59 08        movzx       ebx,word ptr [rcx+8]
  000000006702D226: 48 8B FA           mov         rdi,rdx
  000000006702D229: 66 44 3B C8        cmp         r9w,ax
  000000006702D22D: 72 1F              jb          000000006702D24E
  000000006702D22F: 44 8D 40 0A        lea         r8d,[rax+0Ah]
  000000006702D233: 66 45 3B C8        cmp         r9w,r8w
  000000006702D237: 73 15              jae         000000006702D24E
  000000006702D239: 0F B7 C8           movzx       ecx,ax
  000000006702D23C: 44 2B C9           sub         r9d,ecx
  000000006702D23F: 41 8D 41 30        lea         eax,[r9+30h]
  000000006702D243: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D248: 48 83 C4 20        add         rsp,20h
  000000006702D24C: 5F                 pop         rdi
  000000006702D24D: C3                 ret
  000000006702D24E: 66 41 83 F9 30     cmp         r9w,30h
  000000006702D253: 72 2C              jb          000000006702D281
  000000006702D255: 66 41 83 F9 39     cmp         r9w,39h
  000000006702D25A: 77 25              ja          000000006702D281
  000000006702D25C: B8 FF 00 00 00     mov         eax,0FFh
  000000006702D261: 66 44 3B C8        cmp         r9w,ax
  000000006702D265: 76 0D              jbe         000000006702D274
  000000006702D267: 33 C0              xor         eax,eax
  000000006702D269: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D26E: 48 83 C4 20        add         rsp,20h
  000000006702D272: 5F                 pop         rdi
  000000006702D273: C3                 ret
  000000006702D274: 8A 02              mov         al,byte ptr [rdx]
  000000006702D276: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D27B: 48 83 C4 20        add         rsp,20h
  000000006702D27F: 5F                 pop         rdi
  000000006702D280: C3                 ret
  000000006702D281: 66 44 3B 49 12     cmp         r9w,word ptr [rcx+12h]
  000000006702D286: 75 0D              jne         000000006702D295
  000000006702D288: B0 2B              mov         al,2Bh
  000000006702D28A: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D28F: 48 83 C4 20        add         rsp,20h
  000000006702D293: 5F                 pop         rdi
  000000006702D294: C3                 ret
  000000006702D295: 66 44 3B 49 10     cmp         r9w,word ptr [rcx+10h]
  000000006702D29A: 75 0D              jne         000000006702D2A9
  000000006702D29C: B0 2D              mov         al,2Dh
  000000006702D29E: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D2A3: 48 83 C4 20        add         rsp,20h
  000000006702D2A7: 5F                 pop         rdi
  000000006702D2A8: C3                 ret
  000000006702D2A9: 66 44 3B 49 06     cmp         r9w,word ptr [rcx+6]
  000000006702D2AE: 75 0D              jne         000000006702D2BD
  000000006702D2B0: B0 2E              mov         al,2Eh
  000000006702D2B2: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D2B7: 48 83 C4 20        add         rsp,20h
  000000006702D2BB: 5F                 pop         rdi
  000000006702D2BC: C3                 ret
  000000006702D2BD: 66 44 3B CB        cmp         r9w,bx
  000000006702D2C1: 75 0D              jne         000000006702D2D0
  000000006702D2C3: B0 2C              mov         al,2Ch
  000000006702D2C5: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D2CA: 48 83 C4 20        add         rsp,20h
  000000006702D2CE: 5F                 pop         rdi
  000000006702D2CF: C3                 ret
  000000006702D2D0: 0F B7 41 14        movzx       eax,word ptr [rcx+14h]
  000000006702D2D4: 66 44 3B C8        cmp         r9w,ax
  000000006702D2D8: 74 39              je          000000006702D313
  000000006702D2DA: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006702D2DF: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006702D2E4: 66 89 44 24 30     mov         word ptr [rsp+30h],ax
  000000006702D2E9: E8 62 D4 03 00     call        000000006706A750
  000000006702D2EE: 66 8B 0F           mov         cx,word ptr [rdi]
  000000006702D2F1: 66 3B 08           cmp         cx,word ptr [rax]
  000000006702D2F4: 74 1D              je          000000006702D313
  000000006702D2F6: B8 A0 00 00 00     mov         eax,0A0h
  000000006702D2FB: 66 3B D8           cmp         bx,ax
  000000006702D2FE: 75 06              jne         000000006702D306
  000000006702D300: 66 83 F9 20        cmp         cx,20h
  000000006702D304: 74 BD              je          000000006702D2C3
  000000006702D306: 32 C0              xor         al,al
  000000006702D308: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D30D: 48 83 C4 20        add         rsp,20h
  000000006702D311: 5F                 pop         rdi
  000000006702D312: C3                 ret
  000000006702D313: B0 65              mov         al,65h
  000000006702D315: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006702D31A: 48 83 C4 20        add         rsp,20h
  000000006702D31E: 5F                 pop         rdi
  000000006702D31F: C3                 ret

  Summary

      1A7000 .text
