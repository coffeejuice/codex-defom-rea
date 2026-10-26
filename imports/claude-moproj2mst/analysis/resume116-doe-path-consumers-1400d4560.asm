Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D4560: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400D4565: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400D456A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400D456F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D4574: 53                 push        rbx
  00000001400D4575: 56                 push        rsi
  00000001400D4576: 57                 push        rdi
  00000001400D4577: 48 81 EC 90 02 00  sub         rsp,290h
                    00
  00000001400D457E: 48 C7 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400D458A: C6 84 24 94 00 00  mov         byte ptr [rsp+0000000000000094h],0
                    00 00
  00000001400D4592: 33 D2              xor         edx,edx
  00000001400D4594: 48 8B 8C 24 C8 02  mov         rcx,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D459C: E8 3F 7B 79 00     call        000000014086C0E0
  00000001400D45A1: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400D45A9: 48 8D 0D 08 21 CA  lea         rcx,[0000000143D766B8h]
                    03
  00000001400D45B0: E8 DB 3A 1F 00     call        00000001402C8090
  00000001400D45B5: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001400D45BD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400D45C5: 4C 8D 0D 8C 4C 49  lea         r9,[0000000143569258h]
                    03
  00000001400D45CC: 4C 8D 05 AD 4C 49  lea         r8,[0000000143569280h]
                    03
  00000001400D45D3: 33 D2              xor         edx,edx
  00000001400D45D5: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400D45DD: E8 1A B3 39 01     call        000000014146F8FC
  00000001400D45E2: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001400D45EA: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001400D45F2: E8 99 8F F4 FF     call        000000014001D590
  00000001400D45F7: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400D45FF: 48 8B 84 24 C8 02  mov         rax,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D4607: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000001400D460A: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400D4612: E8 39 B6 F8 FF     call        000000014005FC50
  00000001400D4617: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  00000001400D461E: 48 8B 84 24 B0 02  mov         rax,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D4626: 48 C7 80 B0 00 00  mov         qword ptr [rax+00000000000000B0h],0
                    00 00 00 00 00
  00000001400D4631: 48 8B 84 24 B0 02  mov         rax,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D4639: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D463C: 44 8B 8C 24 90 00  mov         r9d,dword ptr [rsp+0000000000000090h]
                    00 00
  00000001400D4644: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  00000001400D464C: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000001400D4654: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D465C: FF 90 B0 01 00 00  call        qword ptr [rax+00000000000001B0h]
  00000001400D4662: 90                 nop
  00000001400D4663: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D466B: E8 A0 2D 23 00     call        0000000140307410
  00000001400D4670: 0F B6 C0           movzx       eax,al
  00000001400D4673: 85 C0              test        eax,eax
  00000001400D4675: 0F 85 9B 07 00 00  jne         00000001400D4E16
  00000001400D467B: C7 84 24 B4 00 00  mov         dword ptr [rsp+00000000000000B4h],0
                    00 00 00 00 00
  00000001400D4686: EB 11              jmp         00000001400D4699
  00000001400D4688: 8B 84 24 B4 00 00  mov         eax,dword ptr [rsp+00000000000000B4h]
                    00
  00000001400D468F: 83 C0 01           add         eax,1
  00000001400D4692: 89 84 24 B4 00 00  mov         dword ptr [rsp+00000000000000B4h],eax
                    00
  00000001400D4699: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D46A1: E8 3A 04 F3 FF     call        0000000140004AE0
  00000001400D46A6: 39 84 24 B4 00 00  cmp         dword ptr [rsp+00000000000000B4h],eax
                    00
  00000001400D46AD: 0F 8D E7 06 00 00  jge         00000001400D4D9A
  00000001400D46B3: 8B 94 24 B4 00 00  mov         edx,dword ptr [rsp+00000000000000B4h]
                    00
  00000001400D46BA: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D46C2: E8 49 C5 08 00     call        0000000140160C10
  00000001400D46C7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D46CA: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  00000001400D46D2: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  00000001400D46DA: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D46E2: E8 D9 00 23 00     call        00000001403047C0
  00000001400D46E7: 90                 nop
  00000001400D46E8: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400D46F0: FF 15 CA FD D2 01  call        qword ptr [0000000141E044C0h]
  00000001400D46F6: 90                 nop
  00000001400D46F7: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400D46FF: FF 15 BB FD D2 01  call        qword ptr [0000000141E044C0h]
  00000001400D4705: 90                 nop
  00000001400D4706: 4C 8D 8C 24 F8 00  lea         r9,[rsp+00000000000000F8h]
                    00 00
  00000001400D470E: 4C 8D 84 24 E8 00  lea         r8,[rsp+00000000000000E8h]
                    00 00
  00000001400D4716: 33 D2              xor         edx,edx
  00000001400D4718: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D4720: E8 1B 32 74 00     call        0000000140817940
  00000001400D4725: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D472D: E8 3E E9 6F 00     call        00000001407D3070
  00000001400D4732: 89 84 24 08 01 00  mov         dword ptr [rsp+0000000000000108h],eax
                    00
  00000001400D4739: B9 28 00 00 00     mov         ecx,28h
  00000001400D473E: E8 79 A3 39 01     call        000000014146EABC
  00000001400D4743: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  00000001400D474B: 48 83 BC 24 18 01  cmp         qword ptr [rsp+0000000000000118h],0
                    00 00 00
  00000001400D4754: 74 17              je          00000001400D476D
  00000001400D4756: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001400D475E: E8 FD 38 00 00     call        00000001400D8060
  00000001400D4763: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  00000001400D476B: EB 0C              jmp         00000001400D4779
  00000001400D476D: 48 C7 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],0
                    00 00 00 00 00 00
  00000001400D4779: 48 8B 84 24 E8 01  mov         rax,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000001400D4781: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001400D4789: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400D4791: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  00000001400D4799: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D47A1: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D47A9: 48 89 01           mov         qword ptr [rcx],rax
  00000001400D47AC: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  00000001400D47B4: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  00000001400D47BC: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  00000001400D47C4: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001400D47CC: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  00000001400D47D4: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001400D47DC: FF 15 F6 11 D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D47E2: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  00000001400D47EA: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001400D47F2: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  00000001400D47FA: 48 8B 94 24 C8 02  mov         rdx,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D4802: 48 81 C2 28 08 00  add         rdx,828h
                    00
  00000001400D4809: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001400D4811: FF 15 69 FC D2 01  call        qword ptr [0000000141E04480h]
  00000001400D4817: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  00000001400D481F: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001400D4827: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D482C: 4C 8B 8C 24 00 02  mov         r9,qword ptr [rsp+0000000000000200h]
                    00 00
  00000001400D4834: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  00000001400D483C: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  00000001400D4844: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D484C: E8 1F 11 00 00     call        00000001400D5970
  00000001400D4851: 90                 nop
  00000001400D4852: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400D485A: E8 81 B1 0B 00     call        000000014018F9E0
  00000001400D485F: 90                 nop
  00000001400D4860: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001400D4868: E8 73 B1 0B 00     call        000000014018F9E0
  00000001400D486D: 90                 nop
  00000001400D486E: B9 C8 00 00 00     mov         ecx,0C8h
  00000001400D4873: E8 44 A2 39 01     call        000000014146EABC
  00000001400D4878: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001400D4880: 48 83 BC 24 48 01  cmp         qword ptr [rsp+0000000000000148h],0
                    00 00 00
  00000001400D4889: 74 17              je          00000001400D48A2
  00000001400D488B: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001400D4893: E8 E8 81 81 00     call        00000001408ECA80
  00000001400D4898: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  00000001400D48A0: EB 0C              jmp         00000001400D48AE
  00000001400D48A2: 48 C7 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],0
                    00 00 00 00 00 00
  00000001400D48AE: 48 8B 84 24 08 02  mov         rax,qword ptr [rsp+0000000000000208h]
                    00 00
  00000001400D48B6: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  00000001400D48BE: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D48C6: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400D48CE: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001400D48D2: 48 8D 94 24 50 01  lea         rdx,[rsp+0000000000000150h]
                    00 00
  00000001400D48DA: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400D48E2: FF 15 68 F6 D2 01  call        qword ptr [0000000141E03F50h]
  00000001400D48E8: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  00000001400D48F0: 48 8B 84 24 10 02  mov         rax,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400D48F8: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  00000001400D4900: 48 8B 8C 24 18 02  mov         rcx,qword ptr [rsp+0000000000000218h]
                    00 00
  00000001400D4908: FF 15 4A F6 D2 01  call        qword ptr [0000000141E03F58h]
  00000001400D490E: 45 33 C9           xor         r9d,r9d
  00000001400D4911: 41 B8 02 00 00 00  mov         r8d,2
  00000001400D4917: 48 8B D0           mov         rdx,rax
  00000001400D491A: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4922: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001400D4926: E8 15 9C 81 00     call        00000001408EE540
  00000001400D492B: 89 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],eax
                    00
  00000001400D4932: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001400D493A: FF 15 50 F9 D2 01  call        qword ptr [0000000141E04290h]
  00000001400D4940: 83 BC 24 B0 00 00  cmp         dword ptr [rsp+00000000000000B0h],0
                    00 00
  00000001400D4948: 0F 85 54 01 00 00  jne         00000001400D4AA2
  00000001400D494E: C6 84 24 58 01 00  mov         byte ptr [rsp+0000000000000158h],0
                    00 00
  00000001400D4956: 48 8D 94 24 58 01  lea         rdx,[rsp+0000000000000158h]
                    00 00
  00000001400D495E: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4966: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001400D496A: E8 21 A6 81 00     call        00000001408EEF90
  00000001400D496F: 4C 8B 84 24 D0 00  mov         r8,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4977: 49 83 C0 18        add         r8,18h
  00000001400D497B: 33 D2              xor         edx,edx
  00000001400D497D: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D4985: E8 96 3A 74 00     call        0000000140818420
  00000001400D498A: B9 C8 00 00 00     mov         ecx,0C8h
  00000001400D498F: E8 28 A1 39 01     call        000000014146EABC
  00000001400D4994: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001400D499C: 48 83 BC 24 68 01  cmp         qword ptr [rsp+0000000000000168h],0
                    00 00 00
  00000001400D49A5: 74 17              je          00000001400D49BE
  00000001400D49A7: 48 8B 8C 24 68 01  mov         rcx,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001400D49AF: E8 CC 80 81 00     call        00000001408ECA80
  00000001400D49B4: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  00000001400D49BC: EB 0C              jmp         00000001400D49CA
  00000001400D49BE: 48 C7 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],0
                    00 00 00 00 00 00
  00000001400D49CA: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001400D49D2: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001400D49DA: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D49E2: 48 8B 84 24 60 01  mov         rax,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001400D49EA: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001400D49EE: 48 8D 94 24 70 01  lea         rdx,[rsp+0000000000000170h]
                    00 00
  00000001400D49F6: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400D49FE: FF 15 4C F5 D2 01  call        qword ptr [0000000141E03F50h]
  00000001400D4A04: 48 89 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],rax
                    00 00
  00000001400D4A0C: 48 8B 84 24 28 02  mov         rax,qword ptr [rsp+0000000000000228h]
                    00 00
  00000001400D4A14: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  00000001400D4A1C: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D4A24: FF 15 2E F5 D2 01  call        qword ptr [0000000141E03F58h]
  00000001400D4A2A: 45 33 C9           xor         r9d,r9d
  00000001400D4A2D: 41 B8 02 00 00 00  mov         r8d,2
  00000001400D4A33: 48 8B D0           mov         rdx,rax
  00000001400D4A36: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4A3E: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001400D4A42: E8 F9 9A 81 00     call        00000001408EE540
  00000001400D4A47: 89 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],eax
                    00
  00000001400D4A4E: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000001400D4A56: FF 15 34 F8 D2 01  call        qword ptr [0000000141E04290h]
  00000001400D4A5C: 83 BC 24 B0 00 00  cmp         dword ptr [rsp+00000000000000B0h],0
                    00 00
  00000001400D4A64: 75 3C              jne         00000001400D4AA2
  00000001400D4A66: C6 84 24 78 01 00  mov         byte ptr [rsp+0000000000000178h],0
                    00 00
  00000001400D4A6E: 48 8D 94 24 78 01  lea         rdx,[rsp+0000000000000178h]
                    00 00
  00000001400D4A76: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4A7E: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001400D4A82: E8 09 A5 81 00     call        00000001408EEF90
  00000001400D4A87: 4C 8B 84 24 D0 00  mov         r8,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4A8F: 49 83 C0 20        add         r8,20h
  00000001400D4A93: 33 D2              xor         edx,edx
  00000001400D4A95: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D4A9D: E8 EE 39 74 00     call        0000000140818490
  00000001400D4AA2: C6 84 24 D8 00 00  mov         byte ptr [rsp+00000000000000D8h],0
                    00 00
  00000001400D4AAA: 48 8B 84 24 C8 02  mov         rax,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D4AB2: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  00000001400D4AB6: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  00000001400D4ABE: 48 8D 94 24 80 01  lea         rdx,[rsp+0000000000000180h]
                    00 00
  00000001400D4AC6: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D4ACE: E8 ED 0D 00 00     call        00000001400D58C0
  00000001400D4AD3: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  00000001400D4ADB: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001400D4AE3: 48 89 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],rax
                    00 00
  00000001400D4AEB: 48 8B 94 24 40 02  mov         rdx,qword ptr [rsp+0000000000000240h]
                    00 00
  00000001400D4AF3: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000001400D4AFB: FF 15 07 F4 D2 01  call        qword ptr [0000000141E03F08h]
  00000001400D4B01: 88 84 24 79 01 00  mov         byte ptr [rsp+0000000000000179h],al
                    00
  00000001400D4B08: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  00000001400D4B10: FF 15 CA 0E D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D4B16: 44 0F B6 9C 24 79  movzx       r11d,byte ptr [rsp+0000000000000179h]
                    01 00 00
  00000001400D4B1F: 45 85 DB           test        r11d,r11d
  00000001400D4B22: 74 08              je          00000001400D4B2C
  00000001400D4B24: C6 84 24 D8 00 00  mov         byte ptr [rsp+00000000000000D8h],1
                    00 01
  00000001400D4B2C: 83 BC 24 B4 00 00  cmp         dword ptr [rsp+00000000000000B4h],0
                    00 00
  00000001400D4B34: 75 0A              jne         00000001400D4B40
  00000001400D4B36: C6 84 24 48 02 00  mov         byte ptr [rsp+0000000000000248h],1
                    00 01
  00000001400D4B3E: EB 08              jmp         00000001400D4B48
  00000001400D4B40: C6 84 24 48 02 00  mov         byte ptr [rsp+0000000000000248h],0
                    00 00
  00000001400D4B48: 48 8D 84 24 88 01  lea         rax,[rsp+0000000000000188h]
                    00 00
  00000001400D4B50: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  00000001400D4B58: 48 8D 84 24 98 01  lea         rax,[rsp+0000000000000198h]
                    00 00
  00000001400D4B60: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001400D4B68: 48 8D 84 24 A8 01  lea         rax,[rsp+00000000000001A8h]
                    00 00
  00000001400D4B70: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  00000001400D4B78: 48 8D 84 24 B8 01  lea         rax,[rsp+00000000000001B8h]
                    00 00
  00000001400D4B80: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001400D4B88: 48 8B B4 24 D0 00  mov         rsi,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4B90: 48 83 C6 20        add         rsi,20h
  00000001400D4B94: 48 8B BC 24 D0 00  mov         rdi,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4B9C: 48 83 C7 18        add         rdi,18h
  00000001400D4BA0: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D4BA8: E8 C3 2C 74 00     call        0000000140817870
  00000001400D4BAD: 8B D8              mov         ebx,eax
  00000001400D4BAF: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  00000001400D4BB7: 48 8B 8C 24 90 01  mov         rcx,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001400D4BBF: FF 15 13 0E D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D4BC5: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  00000001400D4BCD: 48 8B 84 24 50 02  mov         rax,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001400D4BD5: 48 89 84 24 58 02  mov         qword ptr [rsp+0000000000000258h],rax
                    00 00
  00000001400D4BDD: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  00000001400D4BE5: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001400D4BED: FF 15 E5 0D D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D4BF3: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  00000001400D4BFB: 48 8B 84 24 60 02  mov         rax,qword ptr [rsp+0000000000000260h]
                    00 00
  00000001400D4C03: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  00000001400D4C0B: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  00000001400D4C13: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001400D4C1B: FF 15 B7 0D D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D4C21: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  00000001400D4C29: 48 8B 84 24 70 02  mov         rax,qword ptr [rsp+0000000000000270h]
                    00 00
  00000001400D4C31: 48 89 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],rax
                    00 00
  00000001400D4C39: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  00000001400D4C41: 48 8B 8C 24 C0 01  mov         rcx,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001400D4C49: FF 15 89 0D D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D4C4F: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  00000001400D4C57: 0F B6 84 24 48 02  movzx       eax,byte ptr [rsp+0000000000000248h]
                    00 00
  00000001400D4C5F: 88 44 24 78        mov         byte ptr [rsp+78h],al
  00000001400D4C63: 48 89 74 24 70     mov         qword ptr [rsp+70h],rsi
  00000001400D4C68: 48 89 7C 24 68     mov         qword ptr [rsp+68h],rdi
  00000001400D4C6D: 89 5C 24 60        mov         dword ptr [rsp+60h],ebx
  00000001400D4C71: 48 8B 84 24 58 02  mov         rax,qword ptr [rsp+0000000000000258h]
                    00 00
  00000001400D4C79: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400D4C7E: 48 8B 84 24 68 02  mov         rax,qword ptr [rsp+0000000000000268h]
                    00 00
  00000001400D4C86: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400D4C8B: 48 8B 84 24 78 02  mov         rax,qword ptr [rsp+0000000000000278h]
                    00 00
  00000001400D4C93: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400D4C98: 8B 84 24 D0 02 00  mov         eax,dword ptr [rsp+00000000000002D0h]
                    00
  00000001400D4C9F: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000001400D4CA3: 0F B6 84 24 D8 00  movzx       eax,byte ptr [rsp+00000000000000D8h]
                    00 00
  00000001400D4CAB: 88 44 24 38        mov         byte ptr [rsp+38h],al
  00000001400D4CAF: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4CB7: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001400D4CBB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400D4CC0: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4CC8: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001400D4CCC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400D4CD1: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400D4CD9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D4CDE: 4C 8B 8C 24 C8 02  mov         r9,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D4CE6: 44 8B 84 24 08 01  mov         r8d,dword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D4CEE: 48 8B 94 24 80 02  mov         rdx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400D4CF6: 48 8B 8C 24 B8 02  mov         rcx,qword ptr [rsp+00000000000002B8h]
                    00 00
  00000001400D4CFE: E8 ED DB 11 00     call        00000001401F28F0
  00000001400D4D03: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D4D0B: 48 8B 84 24 C8 02  mov         rax,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D4D13: 48 89 81 B0 00 00  mov         qword ptr [rcx+00000000000000B0h],rax
                    00
  00000001400D4D1A: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D4D22: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D4D29: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  00000001400D4D31: E8 AA 8A 65 01     call        000000014172D7E0
  00000001400D4D36: C6 84 24 94 00 00  mov         byte ptr [rsp+0000000000000094h],1
                    00 01
  00000001400D4D3E: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001400D4D46: E8 E5 B2 50 00     call        00000001405E0030
  00000001400D4D4B: 90                 nop
  00000001400D4D4C: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400D4D54: E8 D7 B2 50 00     call        00000001405E0030
  00000001400D4D59: 90                 nop
  00000001400D4D5A: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400D4D62: FF 15 78 0C D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D4D68: 90                 nop
  00000001400D4D69: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400D4D71: FF 15 69 0C D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D4D77: 90                 nop
  00000001400D4D78: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400D4D80: FF 15 5A 0C D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D4D86: 90                 nop
  00000001400D4D87: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000001400D4D8F: FF 15 4B 0C D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D4D95: E9 EE F8 FF FF     jmp         00000001400D4688
  00000001400D4D9A: 48 8B 84 24 C8 02  mov         rax,qword ptr [rsp+00000000000002C8h]
                    00 00
  00000001400D4DA2: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  00000001400D4DA6: 44 8B 84 24 C0 02  mov         r8d,dword ptr [rsp+00000000000002C0h]
                    00 00
  00000001400D4DAE: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000001400D4DB6: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D4DBE: E8 FD 0A 00 00     call        00000001400D58C0
  00000001400D4DC3: 90                 nop
  00000001400D4DC4: 48 8D 84 24 C8 01  lea         rax,[rsp+00000000000001C8h]
                    00 00
  00000001400D4DCC: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  00000001400D4DD4: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000001400D4DDC: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001400D4DE4: FF 15 EE 0B D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D4DEA: 48 89 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],rax
                    00 00
  00000001400D4DF2: 48 8B 94 24 88 02  mov         rdx,qword ptr [rsp+0000000000000288h]
                    00 00
  00000001400D4DFA: 48 8B 8C 24 B8 02  mov         rcx,qword ptr [rsp+00000000000002B8h]
                    00 00
  00000001400D4E02: E8 A9 C6 11 00     call        00000001401F14B0
  00000001400D4E07: 90                 nop
  00000001400D4E08: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400D4E10: FF 15 CA 0B D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D4E16: 0F B6 84 24 94 00  movzx       eax,byte ptr [rsp+0000000000000094h]
                    00 00
  00000001400D4E1E: 88 84 24 D8 01 00  mov         byte ptr [rsp+00000000000001D8h],al
                    00
  00000001400D4E25: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400D4E2D: E8 6E 33 13 00     call        00000001402081A0
  00000001400D4E32: 8A 84 24 D8 01 00  mov         al,byte ptr [rsp+00000000000001D8h]
                    00
  00000001400D4E39: 48 81 C4 90 02 00  add         rsp,290h
                    00
  00000001400D4E40: 5F                 pop         rdi
  00000001400D4E41: 5E                 pop         rsi
  00000001400D4E42: 5B                 pop         rbx
  00000001400D4E43: C3                 ret

  Summary

     1E02000 .text
