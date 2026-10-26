Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704D4F0: 40 53              push        rbx
  000000006704D4F2: 48 83 EC 20        sub         rsp,20h
  000000006704D4F6: 0F B7 05 4B 68 2A  movzx       eax,word ptr [00000000672F3D48h]
                    00
  000000006704D4FD: 48 C7 01 00 00 00  mov         qword ptr [rcx],0
                    00
  000000006704D504: 48 8B D9           mov         rbx,rcx
  000000006704D507: 66 89 41 02        mov         word ptr [rcx+2],ax
  000000006704D50B: 48 8B 0D 2E 68 2A  mov         rcx,qword ptr [00000000672F3D40h]
                    00
  000000006704D512: 48 85 C9           test        rcx,rcx
  000000006704D515: 75 35              jne         000000006704D54C
  000000006704D517: 48 8B 05 1A 68 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  000000006704D51E: 48 85 C0           test        rax,rax
  000000006704D521: 74 05              je          000000006704D528
  000000006704D523: 66 39 08           cmp         word ptr [rax],cx
  000000006704D526: 75 0C              jne         000000006704D534
  000000006704D528: E8 03 F8 FF FF     call        000000006704CD30
  000000006704D52D: 48 8B 05 04 68 2A  mov         rax,qword ptr [00000000672F3D38h]
                    00
  000000006704D534: 48 89 05 05 68 2A  mov         qword ptr [00000000672F3D40h],rax
                    00
  000000006704D53B: B8 87 01 00 00     mov         eax,187h
  000000006704D540: 66 89 03           mov         word ptr [rbx],ax
  000000006704D543: 48 8B C3           mov         rax,rbx
  000000006704D546: 48 83 C4 20        add         rsp,20h
  000000006704D54A: 5B                 pop         rbx
  000000006704D54B: C3                 ret
  000000006704D54C: 48 3B 0D E5 67 2A  cmp         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704D553: 75 11              jne         000000006704D566
  000000006704D555: B8 87 01 00 00     mov         eax,187h
  000000006704D55A: 66 89 03           mov         word ptr [rbx],ax
  000000006704D55D: 48 8B C3           mov         rax,rbx
  000000006704D560: 48 83 C4 20        add         rsp,20h
  000000006704D564: 5B                 pop         rbx
  000000006704D565: C3                 ret
  000000006704D566: 48 8D 05 D3 4A 16  lea         rax,[00000000671B2040h]
                    00
  000000006704D56D: 48 2B C8           sub         rcx,rax
  000000006704D570: 48 B8 39 8E E3 38  mov         rax,0E38E38E38E38E39h
                    8E E3 38 0E
  000000006704D57A: 48 F7 E9           imul        rcx
  000000006704D57D: 48 C1 FA 03        sar         rdx,3
  000000006704D581: 48 8B C2           mov         rax,rdx
  000000006704D584: 48 C1 E8 3F        shr         rax,3Fh
  000000006704D588: 48 03 D0           add         rdx,rax
  000000006704D58B: 48 8B C3           mov         rax,rbx
  000000006704D58E: 66 89 13           mov         word ptr [rbx],dx
  000000006704D591: 48 83 C4 20        add         rsp,20h
  000000006704D595: 5B                 pop         rbx
  000000006704D596: C3                 ret

  Summary

      1A7000 .text
