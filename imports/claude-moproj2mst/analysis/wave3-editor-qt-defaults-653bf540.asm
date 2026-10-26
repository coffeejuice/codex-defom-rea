Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653BF540: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653BF545: 57                 push        rdi
  00000000653BF546: 48 83 EC 30        sub         rsp,30h
  00000000653BF54A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653BF553: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653BF558: 48 8B DA           mov         rbx,rdx
  00000000653BF55B: 48 8B F9           mov         rdi,rcx
  00000000653BF55E: B9 30 02 00 00     mov         ecx,230h
  00000000653BF563: E8 0A 1C 22 00     call        00000000655E1172
  00000000653BF568: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000653BF56D: 48 85 C0           test        rax,rax
  00000000653BF570: 74 0A              je          00000000653BF57C
  00000000653BF572: 48 8B C8           mov         rcx,rax
  00000000653BF575: E8 46 FD FF FF     call        00000000653BF2C0
  00000000653BF57A: EB 02              jmp         00000000653BF57E
  00000000653BF57C: 33 C0              xor         eax,eax
  00000000653BF57E: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  00000000653BF586: 4C 8D 4C 24 50     lea         r9,[rsp+50h]
  00000000653BF58B: 4C 8B C3           mov         r8,rbx
  00000000653BF58E: 48 8B D0           mov         rdx,rax
  00000000653BF591: 48 8B CF           mov         rcx,rdi
  00000000653BF594: E8 67 19 CB FF     call        0000000065070F00
  00000000653BF599: 90                 nop
  00000000653BF59A: 48 8D 05 07 28 34  lea         rax,[0000000065701DA8h]
                    00
  00000000653BF5A1: 48 89 07           mov         qword ptr [rdi],rax
  00000000653BF5A4: 48 8D 05 BD 27 34  lea         rax,[0000000065701D68h]
                    00
  00000000653BF5AB: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  00000000653BF5AF: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  00000000653BF5B3: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000653BF5B8: FF 15 AA 71 2D 00  call        qword ptr [0000000065696768h]
  00000000653BF5BE: 90                 nop
  00000000653BF5BF: 48 8B D0           mov         rdx,rax
  00000000653BF5C2: 48 8B CB           mov         rcx,rbx
  00000000653BF5C5: E8 56 F5 FF FF     call        00000000653BEB20
  00000000653BF5CA: 90                 nop
  00000000653BF5CB: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000653BF5D0: FF 15 8A 71 2D 00  call        qword ptr [0000000065696760h]
  00000000653BF5D6: 90                 nop
  00000000653BF5D7: 48 8B C7           mov         rax,rdi
  00000000653BF5DA: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000653BF5DF: 48 83 C4 30        add         rsp,30h
  00000000653BF5E3: 5F                 pop         rdi
  00000000653BF5E4: C3                 ret

  Summary

      695000 .text
