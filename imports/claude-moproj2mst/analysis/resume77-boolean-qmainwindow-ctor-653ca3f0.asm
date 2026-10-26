Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653CA3F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653CA3F5: 53                 push        rbx
  00000000653CA3F6: 56                 push        rsi
  00000000653CA3F7: 57                 push        rdi
  00000000653CA3F8: 48 83 EC 30        sub         rsp,30h
  00000000653CA3FC: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653CA405: 49 8B F8           mov         rdi,r8
  00000000653CA408: 48 8B F2           mov         rsi,rdx
  00000000653CA40B: 48 8B D9           mov         rbx,rcx
  00000000653CA40E: B9 18 02 00 00     mov         ecx,218h
  00000000653CA413: E8 5A 6D 21 00     call        00000000655E1172
  00000000653CA418: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000000653CA41D: 48 85 C0           test        rax,rax
  00000000653CA420: 74 0D              je          00000000653CA42F
  00000000653CA422: 48 8B C8           mov         rcx,rax
  00000000653CA425: E8 26 FF FF FF     call        00000000653CA350
  00000000653CA42A: 48 8B D0           mov         rdx,rax
  00000000653CA42D: EB 02              jmp         00000000653CA431
  00000000653CA42F: 33 D2              xor         edx,edx
  00000000653CA431: 8B 07              mov         eax,dword ptr [rdi]
  00000000653CA433: 83 C8 01           or          eax,1
  00000000653CA436: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000000653CA43A: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  00000000653CA43F: 4C 8B C6           mov         r8,rsi
  00000000653CA442: 48 8B CB           mov         rcx,rbx
  00000000653CA445: E8 B6 6A CA FF     call        0000000065070F00
  00000000653CA44A: 90                 nop
  00000000653CA44B: 48 8D 05 D6 84 33  lea         rax,[0000000065702928h]
                    00
  00000000653CA452: 48 89 03           mov         qword ptr [rbx],rax
  00000000653CA455: 48 8D 05 94 84 33  lea         rax,[00000000657028F0h]
                    00
  00000000653CA45C: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  00000000653CA460: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000653CA464: E8 E7 F3 FF FF     call        00000000653C9850
  00000000653CA469: 90                 nop
  00000000653CA46A: 48 8B C3           mov         rax,rbx
  00000000653CA46D: 48 83 C4 30        add         rsp,30h
  00000000653CA471: 5F                 pop         rdi
  00000000653CA472: 5E                 pop         rsi
  00000000653CA473: 5B                 pop         rbx
  00000000653CA474: C3                 ret

  Summary

      695000 .text
