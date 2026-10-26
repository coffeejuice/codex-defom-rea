Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D4B8C0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140D4B8C5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D4B8CA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4B8CF: 56                 push        rsi
  0000000140D4B8D0: 57                 push        rdi
  0000000140D4B8D1: 48 83 EC 68        sub         rsp,68h
  0000000140D4B8D5: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140D4B8DD: 7D 3D              jge         0000000140D4B91C
  0000000140D4B8DF: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140D4B8E7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D4B8EF: FF 15 1B 89 0B 01  call        qword ptr [0000000141E04210h]
  0000000140D4B8F5: 8B D0              mov         edx,eax
  0000000140D4B8F7: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140D4B8FE: 03 C2              add         eax,edx
  0000000140D4B900: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140D4B904: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D4B909: 48 8D 4C 24 34     lea         rcx,[rsp+34h]
  0000000140D4B90E: E8 8D 7C 2B FF     call        00000001400035A0
  0000000140D4B913: 8B 00              mov         eax,dword ptr [rax]
  0000000140D4B915: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140D4B91C: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D4B924: FF 15 E6 88 0B 01  call        qword ptr [0000000141E04210h]
  0000000140D4B92A: 39 84 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140D4B931: 0F 8D 9D 00 00 00  jge         0000000140D4B9D4
  0000000140D4B937: 8B 94 24 90 00 00  mov         edx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140D4B93E: 83 EA 01           sub         edx,1
  0000000140D4B941: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D4B949: FF 15 B9 88 0B 01  call        qword ptr [0000000141E04208h]
  0000000140D4B94F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D4B954: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D4B95C: FF 15 8E 88 0B 01  call        qword ptr [0000000141E041F0h]
  0000000140D4B962: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D4B967: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D4B96C: 48 83 C0 08        add         rax,8
  0000000140D4B970: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D4B975: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D4B97A: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  0000000140D4B97F: 74 53              je          0000000140D4B9D4
  0000000140D4B981: 48 8D 7C 24 40     lea         rdi,[rsp+40h]
  0000000140D4B986: 48 8B B4 24 88 00  mov         rsi,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D4B98E: B9 20 00 00 00     mov         ecx,20h
  0000000140D4B993: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D4B995: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D4B99A: E8 B1 DD 8C FF     call        0000000140619750
  0000000140D4B99F: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140D4B9A4: 48 8B C8           mov         rcx,rax
  0000000140D4B9A7: E8 34 00 00 00     call        0000000140D4B9E0
  0000000140D4B9AC: 0F B6 C0           movzx       eax,al
  0000000140D4B9AF: 85 C0              test        eax,eax
  0000000140D4B9B1: 74 1F              je          0000000140D4B9D2
  0000000140D4B9B3: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D4B9BB: FF 15 1F 88 0B 01  call        qword ptr [0000000141E041E0h]
  0000000140D4B9C1: 48 8B D0           mov         rdx,rax
  0000000140D4B9C4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D4B9C9: 48 2B C2           sub         rax,rdx
  0000000140D4B9CC: 48 C1 F8 03        sar         rax,3
  0000000140D4B9D0: EB 07              jmp         0000000140D4B9D9
  0000000140D4B9D2: EB 93              jmp         0000000140D4B967
  0000000140D4B9D4: B8 FF FF FF FF     mov         eax,0FFFFFFFFh
  0000000140D4B9D9: 48 83 C4 68        add         rsp,68h
  0000000140D4B9DD: 5F                 pop         rdi
  0000000140D4B9DE: 5E                 pop         rsi
  0000000140D4B9DF: C3                 ret

  Summary

     1E02000 .text
