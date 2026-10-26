Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7620: 40 53              push        rbx
  00000000670A7622: 56                 push        rsi
  00000000670A7623: 57                 push        rdi
  00000000670A7624: 48 83 EC 70        sub         rsp,70h
  00000000670A7628: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670A7631: 33 FF              xor         edi,edi
  00000000670A7633: 89 BC 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],edi
                    00
  00000000670A763A: 48 8B D1           mov         rdx,rcx
  00000000670A763D: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670A7642: E8 C9 76 04 00     call        00000000670EED10
  00000000670A7647: 90                 nop
  00000000670A7648: 89 7C 24 40        mov         dword ptr [rsp+40h],edi
  00000000670A764C: 89 7C 24 44        mov         dword ptr [rsp+44h],edi
  00000000670A7650: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670A7655: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670A765A: E8 21 8A 04 00     call        00000000670F0080
  00000000670A765F: 48 8B F0           mov         rsi,rax
  00000000670A7662: 48 85 C0           test        rax,rax
  00000000670A7665: 0F 85 9C 00 00 00  jne         00000000670A7707
  00000000670A766B: 8D 4F 10           lea         ecx,[rdi+10h]
  00000000670A766E: E8 13 14 0D 00     call        0000000067178A86
  00000000670A7673: 48 8B D8           mov         rbx,rax
  00000000670A7676: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000000670A767E: 48 85 C0           test        rax,rax
  00000000670A7681: 74 2F              je          00000000670A76B2
  00000000670A7683: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670A768B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670A7690: E8 AB 79 04 00     call        00000000670EF040
  00000000670A7695: 90                 nop
  00000000670A7696: BF 01 00 00 00     mov         edi,1
  00000000670A769B: 89 BC 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],edi
                    00
  00000000670A76A2: 48 8B D0           mov         rdx,rax
  00000000670A76A5: 48 8B CB           mov         rcx,rbx
  00000000670A76A8: E8 43 35 04 00     call        00000000670EABF0
  00000000670A76AD: 48 8B F0           mov         rsi,rax
  00000000670A76B0: EB 03              jmp         00000000670A76B5
  00000000670A76B2: 48 8B F7           mov         rsi,rdi
  00000000670A76B5: 40 F6 C7 01        test        dil,1
  00000000670A76B9: 74 1C              je          00000000670A76D7
  00000000670A76BB: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000670A76C3: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670A76C7: 75 0E              jne         00000000670A76D7
  00000000670A76C9: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000670A76D1: E8 FA 89 FC FF     call        00000000670700D0
  00000000670A76D6: 90                 nop
  00000000670A76D7: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000000670A76DC: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670A76E0: 75 0B              jne         00000000670A76ED
  00000000670A76E2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670A76E7: E8 E4 89 FC FF     call        00000000670700D0
  00000000670A76EC: 90                 nop
  00000000670A76ED: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670A76F2: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670A76F6: 75 0A              jne         00000000670A7702
  00000000670A76F8: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670A76FD: E8 CE 89 FC FF     call        00000000670700D0
  00000000670A7702: 48 8B C6           mov         rax,rsi
  00000000670A7705: EB 2E              jmp         00000000670A7735
  00000000670A7707: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670A770C: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670A7710: 75 0B              jne         00000000670A771D
  00000000670A7712: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670A7717: E8 B4 89 FC FF     call        00000000670700D0
  00000000670A771C: 90                 nop
  00000000670A771D: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670A7722: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670A7726: 75 0A              jne         00000000670A7732
  00000000670A7728: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670A772D: E8 9E 89 FC FF     call        00000000670700D0
  00000000670A7732: 48 8B C6           mov         rax,rsi
  00000000670A7735: 48 83 C4 70        add         rsp,70h
  00000000670A7739: 5F                 pop         rdi
  00000000670A773A: 5E                 pop         rsi
  00000000670A773B: 5B                 pop         rbx
  00000000670A773C: C3                 ret

  Summary

      1A7000 .text
