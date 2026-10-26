Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014147663C: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141476641: 57                 push        rdi
  0000000141476642: 48 83 EC 30        sub         rsp,30h
  0000000141476646: F6 42 18 40        test        byte ptr [rdx+18h],40h
  000000014147664A: 48 8B DA           mov         rbx,rdx
  000000014147664D: 8B F9              mov         edi,ecx
  000000014147664F: 0F 85 9A 00 00 00  jne         00000001414766EF
  0000000141476655: 48 8B CA           mov         rcx,rdx
  0000000141476658: E8 EB 33 00 00     call        0000000141479A48
  000000014147665D: 48 8D 15 8C 80 6E  lea         rdx,[0000000143B5E6F0h]
                    02
  0000000141476664: 4C 8D 05 95 DA BD  lea         r8,[0000000145054100h]
                    03
  000000014147666B: 4C 63 D8           movsxd      r11,eax
  000000014147666E: 41 83 FB FF        cmp         r11d,0FFFFFFFFh
  0000000141476672: 74 1D              je          0000000141476691
  0000000141476674: 41 83 FB FE        cmp         r11d,0FFFFFFFEh
  0000000141476678: 74 17              je          0000000141476691
  000000014147667A: 49 8B CB           mov         rcx,r11
  000000014147667D: 49 8B C3           mov         rax,r11
  0000000141476680: 83 E1 1F           and         ecx,1Fh
  0000000141476683: 48 C1 F8 05        sar         rax,5
  0000000141476687: 48 6B C9 58        imul        rcx,rcx,58h
  000000014147668B: 49 03 0C C0        add         rcx,qword ptr [r8+rax*8]
  000000014147668F: EB 03              jmp         0000000141476694
  0000000141476691: 48 8B CA           mov         rcx,rdx
  0000000141476694: F6 41 38 7F        test        byte ptr [rcx+38h],7Fh
  0000000141476698: 75 27              jne         00000001414766C1
  000000014147669A: 41 83 FB FF        cmp         r11d,0FFFFFFFFh
  000000014147669E: 74 1B              je          00000001414766BB
  00000001414766A0: 41 83 FB FE        cmp         r11d,0FFFFFFFEh
  00000001414766A4: 74 15              je          00000001414766BB
  00000001414766A6: 49 8B D3           mov         rdx,r11
  00000001414766A9: 49 8B C3           mov         rax,r11
  00000001414766AC: 83 E2 1F           and         edx,1Fh
  00000001414766AF: 48 C1 F8 05        sar         rax,5
  00000001414766B3: 48 6B D2 58        imul        rdx,rdx,58h
  00000001414766B7: 49 03 14 C0        add         rdx,qword ptr [r8+rax*8]
  00000001414766BB: F6 42 38 80        test        byte ptr [rdx+38h],80h
  00000001414766BF: 74 2E              je          00000001414766EF
  00000001414766C1: E8 6E DA FF FF     call        0000000141474134
  00000001414766C6: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  00000001414766CC: 45 33 C9           xor         r9d,r9d
  00000001414766CF: 45 33 C0           xor         r8d,r8d
  00000001414766D2: 33 D2              xor         edx,edx
  00000001414766D4: 33 C9              xor         ecx,ecx
  00000001414766D6: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  00000001414766DC: E8 87 9E FF FF     call        0000000141470568
  00000001414766E1: 83 C8 FF           or          eax,0FFFFFFFFh
  00000001414766E4: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000001414766E9: 48 83 C4 30        add         rsp,30h
  00000001414766ED: 5F                 pop         rdi
  00000001414766EE: C3                 ret
  00000001414766EF: 83 FF FF           cmp         edi,0FFFFFFFFh
  00000001414766F2: 74 ED              je          00000001414766E1
  00000001414766F4: F6 43 18 01        test        byte ptr [rbx+18h],1
  00000001414766F8: 75 0C              jne         0000000141476706
  00000001414766FA: F6 43 18 80        test        byte ptr [rbx+18h],80h
  00000001414766FE: 74 E1              je          00000001414766E1
  0000000141476700: F6 43 18 02        test        byte ptr [rbx+18h],2
  0000000141476704: 75 DB              jne         00000001414766E1
  0000000141476706: 48 83 7B 10 00     cmp         qword ptr [rbx+10h],0
  000000014147670B: 75 08              jne         0000000141476715
  000000014147670D: 48 8B CB           mov         rcx,rbx
  0000000141476710: E8 AB 40 01 00     call        000000014148A7C0
  0000000141476715: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000141476718: 48 3B 43 10        cmp         rax,qword ptr [rbx+10h]
  000000014147671C: 75 0C              jne         000000014147672A
  000000014147671E: 83 7B 08 00        cmp         dword ptr [rbx+8],0
  0000000141476722: 75 BD              jne         00000001414766E1
  0000000141476724: 48 FF C0           inc         rax
  0000000141476727: 48 89 03           mov         qword ptr [rbx],rax
  000000014147672A: 48 FF 0B           dec         qword ptr [rbx]
  000000014147672D: F6 43 18 40        test        byte ptr [rbx+18h],40h
  0000000141476731: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000141476734: 74 0D              je          0000000141476743
  0000000141476736: 40 38 38           cmp         byte ptr [rax],dil
  0000000141476739: 74 0B              je          0000000141476746
  000000014147673B: 48 FF C0           inc         rax
  000000014147673E: 48 89 03           mov         qword ptr [rbx],rax
  0000000141476741: EB 9E              jmp         00000001414766E1
  0000000141476743: 40 88 38           mov         byte ptr [rax],dil
  0000000141476746: FF 43 08           inc         dword ptr [rbx+8]
  0000000141476749: 83 63 18 EF        and         dword ptr [rbx+18h],0FFFFFFEFh
  000000014147674D: 83 4B 18 01        or          dword ptr [rbx+18h],1
  0000000141476751: 40 0F B6 C7        movzx       eax,dil
  0000000141476755: EB 8D              jmp         00000001414766E4

  Summary

     1E02000 .text
