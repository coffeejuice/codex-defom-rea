Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141476B38: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141476B3D: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  0000000141476B42: 57                 push        rdi
  0000000141476B43: 48 83 EC 20        sub         rsp,20h
  0000000141476B47: 8B 41 18           mov         eax,dword ptr [rcx+18h]
  0000000141476B4A: 33 F6              xor         esi,esi
  0000000141476B4C: 48 8B D9           mov         rbx,rcx
  0000000141476B4F: 24 03              and         al,3
  0000000141476B51: 3C 02              cmp         al,2
  0000000141476B53: 75 3F              jne         0000000141476B94
  0000000141476B55: F7 41 18 08 01 00  test        dword ptr [rcx+18h],108h
                    00
  0000000141476B5C: 74 36              je          0000000141476B94
  0000000141476B5E: 8B 39              mov         edi,dword ptr [rcx]
  0000000141476B60: 2B 79 10           sub         edi,dword ptr [rcx+10h]
  0000000141476B63: 85 FF              test        edi,edi
  0000000141476B65: 7E 2D              jle         0000000141476B94
  0000000141476B67: E8 DC 2E 00 00     call        0000000141479A48
  0000000141476B6C: 48 8B 53 10        mov         rdx,qword ptr [rbx+10h]
  0000000141476B70: 44 8B C7           mov         r8d,edi
  0000000141476B73: 8B C8              mov         ecx,eax
  0000000141476B75: E8 2E E9 00 00     call        00000001414854A8
  0000000141476B7A: 3B C7              cmp         eax,edi
  0000000141476B7C: 75 0F              jne         0000000141476B8D
  0000000141476B7E: 8B 43 18           mov         eax,dword ptr [rbx+18h]
  0000000141476B81: 84 C0              test        al,al
  0000000141476B83: 79 0F              jns         0000000141476B94
  0000000141476B85: 83 E0 FD           and         eax,0FFFFFFFDh
  0000000141476B88: 89 43 18           mov         dword ptr [rbx+18h],eax
  0000000141476B8B: EB 07              jmp         0000000141476B94
  0000000141476B8D: 83 4B 18 20        or          dword ptr [rbx+18h],20h
  0000000141476B91: 83 CE FF           or          esi,0FFFFFFFFh
  0000000141476B94: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  0000000141476B98: 83 63 08 00        and         dword ptr [rbx+8],0
  0000000141476B9C: 8B C6              mov         eax,esi
  0000000141476B9E: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000141476BA3: 48 89 0B           mov         qword ptr [rbx],rcx
  0000000141476BA6: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000141476BAB: 48 83 C4 20        add         rsp,20h
  0000000141476BAF: 5F                 pop         rdi
  0000000141476BB0: C3                 ret

  Summary

     1E02000 .text
