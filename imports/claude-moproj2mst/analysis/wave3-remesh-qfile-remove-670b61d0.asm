Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B61D0: 40 57              push        rdi
  00000000670B61D2: 48 83 EC 40        sub         rsp,40h
  00000000670B61D6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B61DF: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670B61E4: 48 8B D9           mov         rbx,rcx
  00000000670B61E7: B9 30 01 00 00     mov         ecx,130h
  00000000670B61EC: E8 95 28 0C 00     call        0000000067178A86
  00000000670B61F1: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000670B61F6: 48 85 C0           test        rax,rax
  00000000670B61F9: 74 0A              je          00000000670B6205
  00000000670B61FB: 48 8B C8           mov         rcx,rax
  00000000670B61FE: E8 2D F9 FF FF     call        00000000670B5B30
  00000000670B6203: EB 02              jmp         00000000670B6207
  00000000670B6205: 33 C0              xor         eax,eax
  00000000670B6207: 45 33 C0           xor         r8d,r8d
  00000000670B620A: 48 8B D0           mov         rdx,rax
  00000000670B620D: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B6212: E8 69 3D 00 00     call        00000000670B9F80
  00000000670B6217: 90                 nop
  00000000670B6218: 48 8D 3D F9 3A 17  lea         rdi,[0000000067229D18h]
                    00
  00000000670B621F: 48 89 7C 24 28     mov         qword ptr [rsp+28h],rdi
  00000000670B6224: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670B6229: 48 81 C1 E8 00 00  add         rcx,0E8h
                    00
  00000000670B6230: 48 8B D3           mov         rdx,rbx
  00000000670B6233: E8 98 A1 FB FF     call        00000000670703D0
  00000000670B6238: 90                 nop
  00000000670B6239: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B623E: E8 BD E6 FF FF     call        00000000670B4900
  00000000670B6243: 0F B6 D8           movzx       ebx,al
  00000000670B6246: 48 89 7C 24 28     mov         qword ptr [rsp+28h],rdi
  00000000670B624B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B6250: E8 DB FC FF FF     call        00000000670B5F30
  00000000670B6255: 90                 nop
  00000000670B6256: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B625B: E8 50 3D 00 00     call        00000000670B9FB0
  00000000670B6260: 8A C3              mov         al,bl
  00000000670B6262: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670B6267: 48 83 C4 40        add         rsp,40h
  00000000670B626B: 5F                 pop         rdi
  00000000670B626C: C3                 ret

  Summary

      1A7000 .text
