Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712E180: 40 55              push        rbp
  000000006712E182: 56                 push        rsi
  000000006712E183: 57                 push        rdi
  000000006712E184: 41 54              push        r12
  000000006712E186: 41 55              push        r13
  000000006712E188: 48 83 EC 70        sub         rsp,70h
  000000006712E18C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712E195: 48 89 9C 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rbx
                    00 00
  000000006712E19D: 49 8B E8           mov         rbp,r8
  000000006712E1A0: 4C 8B E2           mov         r12,rdx
  000000006712E1A3: 4C 8B E9           mov         r13,rcx
  000000006712E1A6: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006712E1AA: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006712E1AD: FF 10              call        qword ptr [rax]
  000000006712E1AF: 48 8B F8           mov         rdi,rax
  000000006712E1B2: 4D 85 E4           test        r12,r12
  000000006712E1B5: 0F 84 EA 01 00 00  je          000000006712E3A5
  000000006712E1BB: 48 85 C0           test        rax,rax
  000000006712E1BE: 0F 84 E1 01 00 00  je          000000006712E3A5
  000000006712E1C4: 49 8B D4           mov         rdx,r12
  000000006712E1C7: 48 8B C8           mov         rcx,rax
  000000006712E1CA: E8 11 00 FF FF     call        000000006711E1E0
  000000006712E1CF: 85 C0              test        eax,eax
  000000006712E1D1: 0F 89 AC 01 00 00  jns         000000006712E383
  000000006712E1D7: 48 83 7B 38 00     cmp         qword ptr [rbx+38h],0
  000000006712E1DC: 75 48              jne         000000006712E226
  000000006712E1DE: B9 18 00 00 00     mov         ecx,18h
  000000006712E1E3: E8 9E A8 04 00     call        0000000067178A86
  000000006712E1E8: 48 85 C0           test        rax,rax
  000000006712E1EB: 74 33              je          000000006712E220
  000000006712E1ED: 48 8D 0D C4 29 1C  lea         rcx,[00000000672F0BB8h]
                    00
  000000006712E1F4: 48 89 08           mov         qword ptr [rax],rcx
  000000006712E1F7: F0 83 05 B9 29 1C  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  000000006712E1FF: 48 8D 0D 4A 28 1C  lea         rcx,[00000000672F0A50h]
                    00
  000000006712E206: 48 89 48 08        mov         qword ptr [rax+8],rcx
  000000006712E20A: F0 83 05 3E 28 1C  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  000000006712E212: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  000000006712E216: F0 83 05 32 28 1C  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  000000006712E21E: EB 02              jmp         000000006712E222
  000000006712E220: 33 C0              xor         eax,eax
  000000006712E222: 48 89 43 38        mov         qword ptr [rbx+38h],rax
  000000006712E226: 49 8B D4           mov         rdx,r12
  000000006712E229: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006712E231: E8 4A F2 EF FF     call        000000006702D480
  000000006712E236: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006712E23A: 48 83 C1 08        add         rcx,8
  000000006712E23E: 45 33 C0           xor         r8d,r8d
  000000006712E241: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006712E249: E8 22 BF FF FF     call        000000006712A170
  000000006712E24E: 8B F8              mov         edi,eax
  000000006712E250: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712E258: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000006712E25C: 75 0D              jne         000000006712E26B
  000000006712E25E: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712E266: E8 45 21 EF FF     call        00000000670203B0
  000000006712E26B: F7 45 08 FF FF FF  test        dword ptr [rbp+8],3FFFFFFFh
                    3F
  000000006712E272: 75 2C              jne         000000006712E2A0
  000000006712E274: 83 FF FF           cmp         edi,0FFFFFFFFh
  000000006712E277: 75 07              jne         000000006712E280
  000000006712E279: 32 C0              xor         al,al
  000000006712E27B: E9 27 01 00 00     jmp         000000006712E3A7
  000000006712E280: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006712E284: 48 83 C1 08        add         rcx,8
  000000006712E288: 8B D7              mov         edx,edi
  000000006712E28A: E8 41 70 F8 FF     call        00000000670B52D0
  000000006712E28F: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006712E293: 48 83 C1 10        add         rcx,10h
  000000006712E297: 8B D7              mov         edx,edi
  000000006712E299: E8 32 E3 FF FF     call        000000006712C5D0
  000000006712E29E: EB 72              jmp         000000006712E312
  000000006712E2A0: 83 FF FF           cmp         edi,0FFFFFFFFh
  000000006712E2A3: 75 53              jne         000000006712E2F8
  000000006712E2A5: 49 8B D4           mov         rdx,r12
  000000006712E2A8: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006712E2B0: E8 CB F1 EF FF     call        000000006702D480
  000000006712E2B5: 90                 nop
  000000006712E2B6: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006712E2BA: 48 83 C1 08        add         rcx,8
  000000006712E2BE: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  000000006712E2C6: E8 B5 39 F0 FF     call        0000000067031C80
  000000006712E2CB: 90                 nop
  000000006712E2CC: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006712E2D4: F0 01 38           lock add    dword ptr [rax],edi
  000000006712E2D7: 75 0D              jne         000000006712E2E6
  000000006712E2D9: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006712E2E1: E8 CA 20 EF FF     call        00000000670203B0
  000000006712E2E6: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006712E2EA: 48 83 C1 10        add         rcx,10h
  000000006712E2EE: 48 8B D5           mov         rdx,rbp
  000000006712E2F1: E8 BA 29 FB FF     call        00000000670E0CB0
  000000006712E2F6: EB 1A              jmp         000000006712E312
  000000006712E2F8: 48 8B 4B 38        mov         rcx,qword ptr [rbx+38h]
  000000006712E2FC: 48 83 C1 10        add         rcx,10h
  000000006712E300: 8B D7              mov         edx,edi
  000000006712E302: E8 79 E2 FF FF     call        000000006712C580
  000000006712E307: 48 8B C8           mov         rcx,rax
  000000006712E30A: 48 8B D5           mov         rdx,rbp
  000000006712E30D: E8 7E B3 00 00     call        0000000067139690
  000000006712E312: 49 8B D4           mov         rdx,r12
  000000006712E315: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006712E31D: E8 5E F1 EF FF     call        000000006702D480
  000000006712E322: 90                 nop
  000000006712E323: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  000000006712E32B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006712E330: E8 0B F3 FE FF     call        000000006711D640
  000000006712E335: 90                 nop
  000000006712E336: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000006712E33E: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006712E342: 75 0D              jne         000000006712E351
  000000006712E344: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000006712E34C: E8 5F 20 EF FF     call        00000000670203B0
  000000006712E351: B8 FD FF 00 00     mov         eax,0FFFDh
  000000006712E356: 66 21 44 24 3A     and         word ptr [rsp+3Ah],ax
  000000006712E35B: 48 8B 0D BE 5E 1C  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006712E362: 48 85 C9           test        rcx,rcx
  000000006712E365: 74 0E              je          000000006712E375
  000000006712E367: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  000000006712E36C: 49 8B D5           mov         rdx,r13
  000000006712E36F: E8 FC B1 FE FF     call        0000000067119570
  000000006712E374: 90                 nop
  000000006712E375: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006712E37A: E8 01 F3 FE FF     call        000000006711D680
  000000006712E37F: 32 C0              xor         al,al
  000000006712E381: EB 24              jmp         000000006712E3A7
  000000006712E383: 44 8B C0           mov         r8d,eax
  000000006712E386: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006712E38B: 48 8B CF           mov         rcx,rdi
  000000006712E38E: E8 0D 17 FF FF     call        000000006711FAA0
  000000006712E393: 4C 8B C5           mov         r8,rbp
  000000006712E396: 49 8B D5           mov         rdx,r13
  000000006712E399: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006712E39E: E8 5D 4B FF FF     call        0000000067122F00
  000000006712E3A3: EB 02              jmp         000000006712E3A7
  000000006712E3A5: 32 C0              xor         al,al
  000000006712E3A7: 48 8B 9C 24 B0 00  mov         rbx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000006712E3AF: 48 83 C4 70        add         rsp,70h
  000000006712E3B3: 41 5D              pop         r13
  000000006712E3B5: 41 5C              pop         r12
  000000006712E3B7: 5F                 pop         rdi
  000000006712E3B8: 5E                 pop         rsi
  000000006712E3B9: 5D                 pop         rbp
  000000006712E3BA: C3                 ret

  Summary

      1A7000 .text
