Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552E110: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006552E115: 56                 push        rsi
  000000006552E116: 57                 push        rdi
  000000006552E117: 41 54              push        r12
  000000006552E119: 48 83 EC 40        sub         rsp,40h
  000000006552E11D: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552E126: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  000000006552E12B: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  000000006552E130: 49 8B D9           mov         rbx,r9
  000000006552E133: 45 8B E0           mov         r12d,r8d
  000000006552E136: 8B F2              mov         esi,edx
  000000006552E138: 48 8B F9           mov         rdi,rcx
  000000006552E13B: B9 E0 00 00 00     mov         ecx,0E0h
  000000006552E140: E8 2D 30 0B 00     call        00000000655E1172
  000000006552E145: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006552E14A: 48 85 C0           test        rax,rax
  000000006552E14D: 74 0A              je          000000006552E159
  000000006552E14F: 48 8B C8           mov         rcx,rax
  000000006552E152: E8 29 D5 FF FF     call        000000006552B680
  000000006552E157: EB 02              jmp         000000006552E15B
  000000006552E159: 33 C0              xor         eax,eax
  000000006552E15B: 4C 8B C3           mov         r8,rbx
  000000006552E15E: 48 8B D0           mov         rdx,rax
  000000006552E161: 48 8B CF           mov         rcx,rdi
  000000006552E164: FF 15 B6 B0 16 00  call        qword ptr [0000000065699220h]
  000000006552E16A: 90                 nop
  000000006552E16B: 48 8D 05 06 F1 20  lea         rax,[000000006573D278h]
                    00
  000000006552E172: 48 89 07           mov         qword ptr [rdi],rax
  000000006552E175: 48 8B 6F 08        mov         rbp,qword ptr [rdi+8]
  000000006552E179: 48 8B 4D 08        mov         rcx,qword ptr [rbp+8]
  000000006552E17D: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000006552E185: 4C 8D 0D AC F0 20  lea         r9,[000000006573D238h]
                    00
  000000006552E18C: 4C 8B C1           mov         r8,rcx
  000000006552E18F: 48 8D 15 32 BC 1C  lea         rdx,[00000000656F9DC8h]
                    00
  000000006552E196: FF 15 7C 85 16 00  call        qword ptr [0000000065696718h]
  000000006552E19C: 45 8B C4           mov         r8d,r12d
  000000006552E19F: 33 D2              xor         edx,edx
  000000006552E1A1: 48 8B 8D C8 00 00  mov         rcx,qword ptr [rbp+00000000000000C8h]
                    00
  000000006552E1A8: E8 F3 F9 FF FF     call        000000006552DBA0
  000000006552E1AD: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000006552E1B6: BA 01 00 00 00     mov         edx,1
  000000006552E1BB: 48 8B 8D B8 00 00  mov         rcx,qword ptr [rbp+00000000000000B8h]
                    00
  000000006552E1C2: FF 15 A8 88 16 00  call        qword ptr [0000000065696A70h]
  000000006552E1C8: 84 C0              test        al,al
  000000006552E1CA: 74 1A              je          000000006552E1E6
  000000006552E1CC: 48 8B 85 B8 00 00  mov         rax,qword ptr [rbp+00000000000000B8h]
                    00
  000000006552E1D3: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006552E1D7: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552E1DA: 48 8D 8D B8 00 00  lea         rcx,[rbp+00000000000000B8h]
                    00
  000000006552E1E1: E8 DA 1A FE FF     call        000000006550FCC0
  000000006552E1E6: 48 8B 95 B8 00 00  mov         rdx,qword ptr [rbp+00000000000000B8h]
                    00
  000000006552E1ED: 48 83 C2 10        add         rdx,10h
  000000006552E1F1: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  000000006552E1F6: 45 8B C4           mov         r8d,r12d
  000000006552E1F9: 48 8D 8D B8 00 00  lea         rcx,[rbp+00000000000000B8h]
                    00
  000000006552E200: E8 DB C3 FF FF     call        000000006552A5E0
  000000006552E205: 44 8B C6           mov         r8d,esi
  000000006552E208: 33 D2              xor         edx,edx
  000000006552E20A: 48 8B 8D C8 00 00  mov         rcx,qword ptr [rbp+00000000000000C8h]
                    00
  000000006552E211: E8 0A F9 FF FF     call        000000006552DB20
  000000006552E216: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000006552E21F: BA 01 00 00 00     mov         edx,1
  000000006552E224: 48 8B 8D C0 00 00  mov         rcx,qword ptr [rbp+00000000000000C0h]
                    00
  000000006552E22B: FF 15 3F 88 16 00  call        qword ptr [0000000065696A70h]
  000000006552E231: 84 C0              test        al,al
  000000006552E233: 74 1A              je          000000006552E24F
  000000006552E235: 48 8B 85 C0 00 00  mov         rax,qword ptr [rbp+00000000000000C0h]
                    00
  000000006552E23C: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006552E240: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552E243: 48 8D 8D C0 00 00  lea         rcx,[rbp+00000000000000C0h]
                    00
  000000006552E24A: E8 71 1A FE FF     call        000000006550FCC0
  000000006552E24F: 48 8B 95 C0 00 00  mov         rdx,qword ptr [rbp+00000000000000C0h]
                    00
  000000006552E256: 48 83 C2 10        add         rdx,10h
  000000006552E25A: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  000000006552E25F: 44 8B C6           mov         r8d,esi
  000000006552E262: 48 8D 8D C0 00 00  lea         rcx,[rbp+00000000000000C0h]
                    00
  000000006552E269: E8 72 C3 FF FF     call        000000006552A5E0
  000000006552E26E: 4C 8B 9D C8 00 00  mov         r11,qword ptr [rbp+00000000000000C8h]
                    00
  000000006552E275: 48 8B D7           mov         rdx,rdi
  000000006552E278: 49 8B 4B 08        mov         rcx,qword ptr [r11+8]
  000000006552E27C: E8 BF E0 FF FF     call        000000006552C340
  000000006552E281: 90                 nop
  000000006552E282: 48 8B C7           mov         rax,rdi
  000000006552E285: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  000000006552E28A: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  000000006552E28F: 48 83 C4 40        add         rsp,40h
  000000006552E293: 41 5C              pop         r12
  000000006552E295: 5F                 pop         rdi
  000000006552E296: 5E                 pop         rsi
  000000006552E297: C3                 ret

  Summary

      695000 .text
