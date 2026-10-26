Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006550A03F: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  000000006550A044: 0F 8D 91 00 00 00  jge         000000006550A0DB
  000000006550A04A: 48 8B 4C D9 10     mov         rcx,qword ptr [rcx+rbx*8+10h]
  000000006550A04F: 4C 3B C1           cmp         r8,rcx
  000000006550A052: 0F 84 83 00 00 00  je          000000006550A0DB
  000000006550A058: 48 85 C9           test        rcx,rcx
  000000006550A05B: 74 12              je          000000006550A06F
  000000006550A05D: 48 C7 41 18 00 00  mov         qword ptr [rcx+18h],0
                    00 00
  000000006550A065: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006550A068: BA 01 00 00 00     mov         edx,1
  000000006550A06D: FF 10              call        qword ptr [rax]
  000000006550A06F: 48 8B CE           mov         rcx,rsi
  000000006550A072: FF 15 F0 C7 18 00  call        qword ptr [0000000065696868h]
  000000006550A078: 48 8D 0D 11 C5 22  lea         rcx,[0000000065736590h]
                    00
  000000006550A07F: 48 8B D0           mov         rdx,rax
  000000006550A082: FF 15 80 C9 18 00  call        qword ptr [0000000065696A08h]
  000000006550A088: 48 85 FF           test        rdi,rdi
  000000006550A08B: 74 2C              je          000000006550A0B9
  000000006550A08D: 8B 57 28           mov         edx,dword ptr [rdi+28h]
  000000006550A090: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006550A095: 48 89 47 18        mov         qword ptr [rdi+18h],rax
  000000006550A099: 0F BA EA 07        bts         edx,7
  000000006550A09D: FF 15 DD CD 18 00  call        qword ptr [0000000065696E80h]
  000000006550A0A3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006550A0A8: 44 8B 00           mov         r8d,dword ptr [rax]
  000000006550A0AB: 44 89 44 24 48     mov         dword ptr [rsp+48h],r8d
  000000006550A0B0: FF 15 A2 CD 18 00  call        qword ptr [0000000065696E58h]
  000000006550A0B6: 89 47 28           mov         dword ptr [rdi+28h],eax
  000000006550A0B9: 48 8D 4E 28        lea         rcx,[rsi+28h]
  000000006550A0BD: 8B D3              mov         edx,ebx
  000000006550A0BF: E8 AC F4 FF FF     call        0000000065509570
  000000006550A0C4: 44 8B CB           mov         r9d,ebx
  000000006550A0C7: 44 8B C3           mov         r8d,ebx
  000000006550A0CA: BA 01 00 00 00     mov         edx,1
  000000006550A0CF: 48 8B CE           mov         rcx,rsi
  000000006550A0D2: 48 89 38           mov         qword ptr [rax],rdi
  000000006550A0D5: FF 15 DD E8 18 00  call        qword ptr [00000000656989B8h]
  000000006550A0DB: 48 8B 6C 24 40     mov         rbp,qword ptr [rsp+40h]
  000000006550A0E0: 48 83 C4 20        add         rsp,20h
  000000006550A0E4: 5F                 pop         rdi
  000000006550A0E5: 5E                 pop         rsi
  000000006550A0E6: 5B                 pop         rbx

  Summary

      695000 .text
