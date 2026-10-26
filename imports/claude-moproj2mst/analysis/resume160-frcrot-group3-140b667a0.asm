Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140B667A0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140B667A5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140B667AA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140B667AF: 48 81 EC 38 01 00  sub         rsp,138h
                    00
  0000000140B667B6: C7 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],0
                    00 00 00 00 00
  0000000140B667C1: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140B667C9: C7 84 24 94 00 00  mov         dword ptr [rsp+0000000000000094h],0
                    00 00 00 00 00
  0000000140B667D4: C7 84 24 D4 00 00  mov         dword ptr [rsp+00000000000000D4h],0
                    00 00 00 00 00
  0000000140B667DF: C7 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00
  0000000140B667EA: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  0000000140B667F5: C7 84 24 D0 00 00  mov         dword ptr [rsp+00000000000000D0h],0
                    00 00 00 00 00
  0000000140B66800: C7 84 24 DC 00 00  mov         dword ptr [rsp+00000000000000DCh],0
                    00 00 00 00 00
  0000000140B6680B: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  0000000140B66817: 81 BC 24 50 01 00  cmp         dword ptr [rsp+0000000000000150h],10Fh
                    00 0F 01 00 00
  0000000140B66822: 0F 85 6A 02 00 00  jne         0000000140B66A92
  0000000140B66828: 41 B8 03 00 00 00  mov         r8d,3
  0000000140B6682E: 48 8D 94 24 DC 00  lea         rdx,[rsp+00000000000000DCh]
                    00 00
  0000000140B66836: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B6683E: E8 AD 9C F8 FF     call        0000000140AF04F0
  0000000140B66843: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B6684A: 83 BC 24 DC 00 00  cmp         dword ptr [rsp+00000000000000DCh],1
                    00 01
  0000000140B66852: 75 40              jne         0000000140B66894
  0000000140B66854: 33 C9              xor         ecx,ecx
  0000000140B66856: E8 E5 9D F8 FF     call        0000000140AF0640
  0000000140B6685B: 48 8B C8           mov         rcx,rax
  0000000140B6685E: E8 49 CF 90 00     call        00000001414737AC
  0000000140B66863: 89 84 24 E8 00 00  mov         dword ptr [rsp+00000000000000E8h],eax
                    00
  0000000140B6686A: 83 BC 24 E8 00 00  cmp         dword ptr [rsp+00000000000000E8h],0
                    00 00
  0000000140B66872: 75 14              jne         0000000140B66888
  0000000140B66874: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B6687C: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B66883: E8 38 04 D8 FF     call        00000001408E6CC0
  0000000140B66888: 33 C0              xor         eax,eax
  0000000140B6688A: E9 EB 26 00 00     jmp         0000000140B68F7A
  0000000140B6688F: E9 F9 01 00 00     jmp         0000000140B66A8D
  0000000140B66894: 83 BC 24 DC 00 00  cmp         dword ptr [rsp+00000000000000DCh],3
                    00 03
  0000000140B6689C: 0F 85 EB 01 00 00  jne         0000000140B66A8D
  0000000140B668A2: 33 C9              xor         ecx,ecx
  0000000140B668A4: E8 97 9D F8 FF     call        0000000140AF0640
  0000000140B668A9: 48 8B C8           mov         rcx,rax
  0000000140B668AC: E8 FB CE 90 00     call        00000001414737AC
  0000000140B668B1: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140B668B5: B9 01 00 00 00     mov         ecx,1
  0000000140B668BA: E8 81 9D F8 FF     call        0000000140AF0640
  0000000140B668BF: 48 8B C8           mov         rcx,rax
  0000000140B668C2: E8 E5 CE 90 00     call        00000001414737AC
  0000000140B668C7: 89 84 24 94 00 00  mov         dword ptr [rsp+0000000000000094h],eax
                    00
  0000000140B668CE: B9 02 00 00 00     mov         ecx,2
  0000000140B668D3: E8 68 9D F8 FF     call        0000000140AF0640
  0000000140B668D8: 48 8B C8           mov         rcx,rax
  0000000140B668DB: E8 CC CE 90 00     call        00000001414737AC
  0000000140B668E0: 89 84 24 D4 00 00  mov         dword ptr [rsp+00000000000000D4h],eax
                    00
  0000000140B668E7: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B668EF: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B668F6: 44 8B 84 24 94 00  mov         r8d,dword ptr [rsp+0000000000000094h]
                    00 00
  0000000140B668FE: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  0000000140B66902: E8 C9 02 D8 FF     call        00000001408E6BD0
  0000000140B66907: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140B6690F: 48 83 BC 24 C8 00  cmp         qword ptr [rsp+00000000000000C8h],0
                    00 00 00
  0000000140B66918: 75 43              jne         0000000140B6695D
  0000000140B6691A: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B66922: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B66929: E8 52 00 D8 FF     call        00000001408E6980
  0000000140B6692E: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140B66936: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6693E: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140B66942: 89 81 68 01 00 00  mov         dword ptr [rcx+0000000000000168h],eax
  0000000140B66948: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66950: 8B 84 24 94 00 00  mov         eax,dword ptr [rsp+0000000000000094h]
                    00
  0000000140B66957: 89 81 6C 01 00 00  mov         dword ptr [rcx+000000000000016Ch],eax
  0000000140B6695D: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B66965: 75 12              jne         0000000140B66979
  0000000140B66967: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6696F: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],0
                    00 00 00 00
  0000000140B66979: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B66981: E8 DA EB 4F FF     call        0000000140065560
  0000000140B66986: 0F B6 C0           movzx       eax,al
  0000000140B66989: 85 C0              test        eax,eax
  0000000140B6698B: 74 3F              je          0000000140B669CC
  0000000140B6698D: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],1
                    00 01
  0000000140B66995: 75 14              jne         0000000140B669AB
  0000000140B66997: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6699F: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],1
                    01 00 00 00
  0000000140B669A9: EB 1C              jmp         0000000140B669C7
  0000000140B669AB: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],2
                    00 02
  0000000140B669B3: 75 12              jne         0000000140B669C7
  0000000140B669B5: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B669BD: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],2
                    02 00 00 00
  0000000140B669C7: E9 C1 00 00 00     jmp         0000000140B66A8D
  0000000140B669CC: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],1
                    00 01
  0000000140B669D4: 75 29              jne         0000000140B669FF
  0000000140B669D6: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B669DE: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],1
                    01 00 00 00
  0000000140B669E8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B669F0: C7 80 74 01 00 00  mov         dword ptr [rax+0000000000000174h],1
                    01 00 00 00
  0000000140B669FA: E9 8E 00 00 00     jmp         0000000140B66A8D
  0000000140B669FF: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],2
                    00 02
  0000000140B66A07: 75 26              jne         0000000140B66A2F
  0000000140B66A09: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66A11: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],2
                    02 00 00 00
  0000000140B66A1B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66A23: C7 80 74 01 00 00  mov         dword ptr [rax+0000000000000174h],1
                    01 00 00 00
  0000000140B66A2D: EB 5E              jmp         0000000140B66A8D
  0000000140B66A2F: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],3
                    00 03
  0000000140B66A37: 75 26              jne         0000000140B66A5F
  0000000140B66A39: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66A41: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],1
                    01 00 00 00
  0000000140B66A4B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66A53: C7 80 74 01 00 00  mov         dword ptr [rax+0000000000000174h],0
                    00 00 00 00
  0000000140B66A5D: EB 2E              jmp         0000000140B66A8D
  0000000140B66A5F: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],4
                    00 04
  0000000140B66A67: 75 24              jne         0000000140B66A8D
  0000000140B66A69: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66A71: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],2
                    02 00 00 00
  0000000140B66A7B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66A83: C7 80 74 01 00 00  mov         dword ptr [rax+0000000000000174h],0
                    00 00 00 00
  0000000140B66A8D: E9 DD 23 00 00     jmp         0000000140B68E6F
  0000000140B66A92: 4C 8D 8C 24 94 00  lea         r9,[rsp+0000000000000094h]
                    00 00
  0000000140B66A9A: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  0000000140B66A9F: 48 8D 15 6A DF C8  lea         rdx,[00000001427F4A10h]
                    01
  0000000140B66AA6: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B66AAE: E8 15 C0 90 00     call        0000000141472AC8
  0000000140B66AB3: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000140B66AB6: 75 0A              jne         0000000140B66AC2
  0000000140B66AB8: B8 02 00 00 00     mov         eax,2
  0000000140B66ABD: E9 B8 24 00 00     jmp         0000000140B68F7A
  0000000140B66AC2: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B66ACA: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B66AD1: 44 8B 84 24 94 00  mov         r8d,dword ptr [rsp+0000000000000094h]
                    00 00
  0000000140B66AD9: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  0000000140B66ADD: E8 EE 00 D8 FF     call        00000001408E6BD0
  0000000140B66AE2: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140B66AEA: 48 83 BC 24 C8 00  cmp         qword ptr [rsp+00000000000000C8h],0
                    00 00 00
  0000000140B66AF3: 75 43              jne         0000000140B66B38
  0000000140B66AF5: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B66AFD: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B66B04: E8 77 FE D7 FF     call        00000001408E6980
  0000000140B66B09: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140B66B11: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66B19: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140B66B1D: 89 81 68 01 00 00  mov         dword ptr [rcx+0000000000000168h],eax
  0000000140B66B23: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66B2B: 8B 84 24 94 00 00  mov         eax,dword ptr [rsp+0000000000000094h]
                    00
  0000000140B66B32: 89 81 6C 01 00 00  mov         dword ptr [rcx+000000000000016Ch],eax
  0000000140B66B38: 8B 84 24 50 01 00  mov         eax,dword ptr [rsp+0000000000000150h]
                    00
  0000000140B66B3F: 89 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],eax
                    00
  0000000140B66B46: 8B 84 24 10 01 00  mov         eax,dword ptr [rsp+0000000000000110h]
                    00
  0000000140B66B4D: 2D 10 01 00 00     sub         eax,110h
  0000000140B66B52: 89 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],eax
                    00
  0000000140B66B59: 83 BC 24 10 01 00  cmp         dword ptr [rsp+0000000000000110h],9
                    00 09
  0000000140B66B61: 0F 87 FE 22 00 00  ja          0000000140B68E65
  0000000140B66B67: 48 63 84 24 10 01  movsxd      rax,dword ptr [rsp+0000000000000110h]
                    00 00
  0000000140B66B6F: 48 8D 0D 8A 94 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B66B76: 8B 84 81 84 8F B6  mov         eax,dword ptr [rcx+rax*4+0000000000B68F84h]
                    00
  0000000140B66B7D: 48 03 C1           add         rax,rcx
  0000000140B66B80: FF E0              jmp         rax
  0000000140B66B82: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140B66B87: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140B66B8C: 48 8D 44 24 40     lea         rax,[rsp+40h]
  0000000140B66B91: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B66B96: 4C 8D 8C 24 E0 00  lea         r9,[rsp+00000000000000E0h]
                    00 00
  0000000140B66B9E: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  0000000140B66BA6: 48 8D 94 24 D4 00  lea         rdx,[rsp+00000000000000D4h]
                    00 00
  0000000140B66BAE: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B66BB6: E8 35 C1 F8 FF     call        0000000140AF2CF0
  0000000140B66BBB: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B66BC2: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B66BCA: 74 0A              je          0000000140B66BD6
  0000000140B66BCC: B8 02 00 00 00     mov         eax,2
  0000000140B66BD1: E9 A4 23 00 00     jmp         0000000140B68F7A
  0000000140B66BD6: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B66BDE: 75 10              jne         0000000140B66BF0
  0000000140B66BE0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66BE8: C7 00 00 00 00 00  mov         dword ptr [rax],0
  0000000140B66BEE: EB 66              jmp         0000000140B66C56
  0000000140B66BF0: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],1
                    00 01
  0000000140B66BF8: 75 10              jne         0000000140B66C0A
  0000000140B66BFA: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C02: C7 00 01 00 00 00  mov         dword ptr [rax],1
  0000000140B66C08: EB 4C              jmp         0000000140B66C56
  0000000140B66C0A: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],2
                    00 02
  0000000140B66C12: 75 10              jne         0000000140B66C24
  0000000140B66C14: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C1C: C7 00 02 00 00 00  mov         dword ptr [rax],2
  0000000140B66C22: EB 32              jmp         0000000140B66C56
  0000000140B66C24: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],3
                    00 03
  0000000140B66C2C: 75 10              jne         0000000140B66C3E
  0000000140B66C2E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C36: C7 00 03 00 00 00  mov         dword ptr [rax],3
  0000000140B66C3C: EB 18              jmp         0000000140B66C56
  0000000140B66C3E: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],4
                    00 04
  0000000140B66C46: 75 0E              jne         0000000140B66C56
  0000000140B66C48: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C50: C7 00 04 00 00 00  mov         dword ptr [rax],4
  0000000140B66C56: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C5E: 83 38 04           cmp         dword ptr [rax],4
  0000000140B66C61: 0F 84 35 03 00 00  je          0000000140B66F9C
  0000000140B66C67: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B66C6F: 7D 4A              jge         0000000140B66CBB
  0000000140B66C71: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C79: C6 40 04 01        mov         byte ptr [rax+4],1
  0000000140B66C7D: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B66C84: F7 D9              neg         ecx
  0000000140B66C86: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C8E: 89 48 08           mov         dword ptr [rax+8],ecx
  0000000140B66C91: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66C99: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B66CA0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66CA8: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66CB1: F2 0F 11 40 20     movsd       mmword ptr [rax+20h],xmm0
  0000000140B66CB6: E9 DC 02 00 00     jmp         0000000140B66F97
  0000000140B66CBB: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66CC3: C6 40 04 00        mov         byte ptr [rax+4],0
  0000000140B66CC7: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66CCF: 48 83 C1 18        add         rcx,18h
  0000000140B66CD3: BA 15 00 00 00     mov         edx,15h
  0000000140B66CD8: E8 03 4D D3 FF     call        000000014089B9E0
  0000000140B66CDD: 4C 8B 9C 24 C8 00  mov         r11,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66CE5: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B66CEC: 41 89 43 10        mov         dword ptr [r11+10h],eax
  0000000140B66CF0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66CF8: 8B 40 10           mov         eax,dword ptr [rax+10h]
  0000000140B66CFB: 89 84 24 14 01 00  mov         dword ptr [rsp+0000000000000114h],eax
                    00
  0000000140B66D02: 83 BC 24 14 01 00  cmp         dword ptr [rsp+0000000000000114h],8
                    00 08
  0000000140B66D0A: 0F 87 56 02 00 00  ja          0000000140B66F66
  0000000140B66D10: 48 63 84 24 14 01  movsxd      rax,dword ptr [rsp+0000000000000114h]
                    00 00
  0000000140B66D18: 48 8D 0D E1 92 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B66D1F: 8B 84 81 AC 8F B6  mov         eax,dword ptr [rcx+rax*4+0000000000B68FACh]
                    00
  0000000140B66D26: 48 03 C1           add         rax,rcx
  0000000140B66D29: FF E0              jmp         rax
  0000000140B66D2B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66D33: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B66D3A: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66D42: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66D4B: F2 0F 11 40 20     movsd       mmword ptr [rax+20h],xmm0
  0000000140B66D50: E9 11 02 00 00     jmp         0000000140B66F66
  0000000140B66D55: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66D5D: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B66D64: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66D6C: C7 40 28 01 00 00  mov         dword ptr [rax+28h],1
                    00
  0000000140B66D73: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66D7C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66D84: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B66D87: E9 DA 01 00 00     jmp         0000000140B66F66
  0000000140B66D8C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66D94: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B66D9B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66DA3: C7 40 28 04 00 00  mov         dword ptr [rax+28h],4
                    00
  0000000140B66DAA: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66DB3: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66DBB: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B66DBE: E9 A3 01 00 00     jmp         0000000140B66F66
  0000000140B66DC3: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66DCB: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B66DD2: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66DDA: C7 40 28 03 00 00  mov         dword ptr [rax+28h],3
                    00
  0000000140B66DE1: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66DEA: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66DF2: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B66DF5: E9 6C 01 00 00     jmp         0000000140B66F66
  0000000140B66DFA: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E02: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B66E09: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E11: C7 40 28 15 00 00  mov         dword ptr [rax+28h],15h
                    00
  0000000140B66E18: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66E21: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E29: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B66E2C: E9 35 01 00 00     jmp         0000000140B66F66
  0000000140B66E31: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E39: C7 40 1C 03 00 00  mov         dword ptr [rax+1Ch],3
                    00
  0000000140B66E40: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E48: C7 40 28 04 00 00  mov         dword ptr [rax+28h],4
                    00
  0000000140B66E4F: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E57: C7 40 2C 03 00 00  mov         dword ptr [rax+2Ch],3
                    00
  0000000140B66E5E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E66: C7 40 30 15 00 00  mov         dword ptr [rax+30h],15h
                    00
  0000000140B66E6D: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66E76: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E7E: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B66E81: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B66E87: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66E8F: 89 48 38           mov         dword ptr [rax+38h],ecx
  0000000140B66E92: F2 0F 2C 4C 24 48  cvttsd2si   ecx,mmword ptr [rsp+48h]
  0000000140B66E98: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66EA0: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B66EA3: E9 BE 00 00 00     jmp         0000000140B66F66
  0000000140B66EA8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66EB0: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B66EB7: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66EBF: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66EC8: F2 0F 11 80 08 01  movsd       mmword ptr [rax+0000000000000108h],xmm0
                    00 00
  0000000140B66ED0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66ED8: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B66EDE: F2 0F 11 80 10 01  movsd       mmword ptr [rax+0000000000000110h],xmm0
                    00 00
  0000000140B66EE6: EB 7E              jmp         0000000140B66F66
  0000000140B66EE8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66EF0: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B66EF7: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66EFF: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66F08: F2 0F 11 80 18 01  movsd       mmword ptr [rax+0000000000000118h],xmm0
                    00 00
  0000000140B66F10: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66F18: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B66F1E: F2 0F 11 80 20 01  movsd       mmword ptr [rax+0000000000000120h],xmm0
                    00 00
  0000000140B66F26: EB 3E              jmp         0000000140B66F66
  0000000140B66F28: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66F30: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B66F37: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66F3F: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B66F48: F2 0F 11 80 28 01  movsd       mmword ptr [rax+0000000000000128h],xmm0
                    00 00
  0000000140B66F50: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66F58: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B66F5E: F2 0F 11 80 30 01  movsd       mmword ptr [rax+0000000000000130h],xmm0
                    00 00
  0000000140B66F66: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66F6E: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  0000000140B66F72: 74 23              je          0000000140B66F97
  0000000140B66F74: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66F7C: 48 83 C2 18        add         rdx,18h
  0000000140B66F80: 45 33 C0           xor         r8d,r8d
  0000000140B66F83: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B66F8B: E8 60 04 F9 FF     call        0000000140AF73F0
  0000000140B66F90: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B66F97: E9 48 07 00 00     jmp         0000000140B676E4
  0000000140B66F9C: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140B66FA1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B66FA6: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  0000000140B66FAB: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B66FB3: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B66FBB: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B66FC3: E8 A8 B4 F8 FF     call        0000000140AF2470
  0000000140B66FC8: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B66FCF: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B66FD7: 74 0A              je          0000000140B66FE3
  0000000140B66FD9: B8 02 00 00 00     mov         eax,2
  0000000140B66FDE: E9 97 1F 00 00     jmp         0000000140B68F7A
  0000000140B66FE3: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B66FEB: 7D 4A              jge         0000000140B67037
  0000000140B66FED: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B66FF5: C6 40 04 01        mov         byte ptr [rax+4],1
  0000000140B66FF9: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B67000: F7 D9              neg         ecx
  0000000140B67002: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6700A: 89 48 08           mov         dword ptr [rax+8],ecx
  0000000140B6700D: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67015: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B6701C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67024: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6702D: F2 0F 11 40 20     movsd       mmword ptr [rax+20h],xmm0
  0000000140B67032: E9 DC 02 00 00     jmp         0000000140B67313
  0000000140B67037: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6703F: C6 40 04 00        mov         byte ptr [rax+4],0
  0000000140B67043: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6704B: 48 83 C1 18        add         rcx,18h
  0000000140B6704F: BA 15 00 00 00     mov         edx,15h
  0000000140B67054: E8 87 49 D3 FF     call        000000014089B9E0
  0000000140B67059: 4C 8B 9C 24 C8 00  mov         r11,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67061: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B67068: 41 89 43 10        mov         dword ptr [r11+10h],eax
  0000000140B6706C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67074: 8B 40 10           mov         eax,dword ptr [rax+10h]
  0000000140B67077: 89 84 24 18 01 00  mov         dword ptr [rsp+0000000000000118h],eax
                    00
  0000000140B6707E: 83 BC 24 18 01 00  cmp         dword ptr [rsp+0000000000000118h],8
                    00 08
  0000000140B67086: 0F 87 56 02 00 00  ja          0000000140B672E2
  0000000140B6708C: 48 63 84 24 18 01  movsxd      rax,dword ptr [rsp+0000000000000118h]
                    00 00
  0000000140B67094: 48 8D 0D 65 8F 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B6709B: 8B 84 81 D0 8F B6  mov         eax,dword ptr [rcx+rax*4+0000000000B68FD0h]
                    00
  0000000140B670A2: 48 03 C1           add         rax,rcx
  0000000140B670A5: FF E0              jmp         rax
  0000000140B670A7: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B670AF: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B670B6: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B670BE: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B670C7: F2 0F 11 40 20     movsd       mmword ptr [rax+20h],xmm0
  0000000140B670CC: E9 11 02 00 00     jmp         0000000140B672E2
  0000000140B670D1: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B670D9: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B670E0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B670E8: C7 40 28 01 00 00  mov         dword ptr [rax+28h],1
                    00
  0000000140B670EF: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B670F8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67100: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B67103: E9 DA 01 00 00     jmp         0000000140B672E2
  0000000140B67108: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67110: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B67117: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6711F: C7 40 28 04 00 00  mov         dword ptr [rax+28h],4
                    00
  0000000140B67126: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6712F: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67137: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B6713A: E9 A3 01 00 00     jmp         0000000140B672E2
  0000000140B6713F: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67147: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B6714E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67156: C7 40 28 03 00 00  mov         dword ptr [rax+28h],3
                    00
  0000000140B6715D: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67166: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6716E: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B67171: E9 6C 01 00 00     jmp         0000000140B672E2
  0000000140B67176: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6717E: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
                    00
  0000000140B67185: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6718D: C7 40 28 15 00 00  mov         dword ptr [rax+28h],15h
                    00
  0000000140B67194: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6719D: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B671A5: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B671A8: E9 35 01 00 00     jmp         0000000140B672E2
  0000000140B671AD: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B671B5: C7 40 1C 03 00 00  mov         dword ptr [rax+1Ch],3
                    00
  0000000140B671BC: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B671C4: C7 40 28 04 00 00  mov         dword ptr [rax+28h],4
                    00
  0000000140B671CB: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B671D3: C7 40 2C 03 00 00  mov         dword ptr [rax+2Ch],3
                    00
  0000000140B671DA: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B671E2: C7 40 30 15 00 00  mov         dword ptr [rax+30h],15h
                    00
  0000000140B671E9: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B671F2: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B671FA: 89 48 34           mov         dword ptr [rax+34h],ecx
  0000000140B671FD: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B67203: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6720B: 89 48 38           mov         dword ptr [rax+38h],ecx
  0000000140B6720E: F2 0F 2C 4C 24 48  cvttsd2si   ecx,mmword ptr [rsp+48h]
  0000000140B67214: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6721C: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B6721F: E9 BE 00 00 00     jmp         0000000140B672E2
  0000000140B67224: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6722C: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B67233: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6723B: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67244: F2 0F 11 80 08 01  movsd       mmword ptr [rax+0000000000000108h],xmm0
                    00 00
  0000000140B6724C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67254: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B6725A: F2 0F 11 80 10 01  movsd       mmword ptr [rax+0000000000000110h],xmm0
                    00 00
  0000000140B67262: EB 7E              jmp         0000000140B672E2
  0000000140B67264: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6726C: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B67273: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6727B: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67284: F2 0F 11 80 18 01  movsd       mmword ptr [rax+0000000000000118h],xmm0
                    00 00
  0000000140B6728C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67294: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B6729A: F2 0F 11 80 20 01  movsd       mmword ptr [rax+0000000000000120h],xmm0
                    00 00
  0000000140B672A2: EB 3E              jmp         0000000140B672E2
  0000000140B672A4: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B672AC: C7 40 1C 00 00 00  mov         dword ptr [rax+1Ch],0
                    00
  0000000140B672B3: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B672BB: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B672C4: F2 0F 11 80 28 01  movsd       mmword ptr [rax+0000000000000128h],xmm0
                    00 00
  0000000140B672CC: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B672D4: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B672DA: F2 0F 11 80 30 01  movsd       mmword ptr [rax+0000000000000130h],xmm0
                    00 00
  0000000140B672E2: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B672EA: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  0000000140B672EE: 74 23              je          0000000140B67313
  0000000140B672F0: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B672F8: 48 83 C2 18        add         rdx,18h
  0000000140B672FC: 45 33 C0           xor         r8d,r8d
  0000000140B672FF: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67307: E8 E4 00 F9 FF     call        0000000140AF73F0
  0000000140B6730C: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67313: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140B67318: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B6731D: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  0000000140B67322: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B6732A: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B67332: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B6733A: E8 31 B1 F8 FF     call        0000000140AF2470
  0000000140B6733F: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67346: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B6734E: 74 0A              je          0000000140B6735A
  0000000140B67350: B8 02 00 00 00     mov         eax,2
  0000000140B67355: E9 20 1C 00 00     jmp         0000000140B68F7A
  0000000140B6735A: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B67362: 7D 50              jge         0000000140B673B4
  0000000140B67364: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6736C: C6 40 05 01        mov         byte ptr [rax+5],1
  0000000140B67370: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B67377: F7 D9              neg         ecx
  0000000140B67379: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67381: 89 48 0C           mov         dword ptr [rax+0Ch],ecx
  0000000140B67384: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6738C: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],0
                    00 00 00 00
  0000000140B67396: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6739E: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B673A7: F2 0F 11 80 98 00  movsd       mmword ptr [rax+0000000000000098h],xmm0
                    00 00
  0000000140B673AF: E9 30 03 00 00     jmp         0000000140B676E4
  0000000140B673B4: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B673BC: C6 40 05 00        mov         byte ptr [rax+5],0
  0000000140B673C0: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B673C8: 48 81 C1 90 00 00  add         rcx,90h
                    00
  0000000140B673CF: BA 15 00 00 00     mov         edx,15h
  0000000140B673D4: E8 07 46 D3 FF     call        000000014089B9E0
  0000000140B673D9: 4C 8B 9C 24 C8 00  mov         r11,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B673E1: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B673E8: 41 89 43 14        mov         dword ptr [r11+14h],eax
  0000000140B673EC: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B673F4: 8B 40 14           mov         eax,dword ptr [rax+14h]
  0000000140B673F7: 89 84 24 1C 01 00  mov         dword ptr [rsp+000000000000011Ch],eax
                    00
  0000000140B673FE: 83 BC 24 1C 01 00  cmp         dword ptr [rsp+000000000000011Ch],8
                    00 08
  0000000140B67406: 0F 87 A1 02 00 00  ja          0000000140B676AD
  0000000140B6740C: 48 63 84 24 1C 01  movsxd      rax,dword ptr [rsp+000000000000011Ch]
                    00 00
  0000000140B67414: 48 8D 0D E5 8B 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B6741B: 8B 84 81 F4 8F B6  mov         eax,dword ptr [rcx+rax*4+0000000000B68FF4h]
                    00
  0000000140B67422: 48 03 C1           add         rax,rcx
  0000000140B67425: FF E0              jmp         rax
  0000000140B67427: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6742F: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],0
                    00 00 00 00
  0000000140B67439: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67441: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6744A: F2 0F 11 80 98 00  movsd       mmword ptr [rax+0000000000000098h],xmm0
                    00 00
  0000000140B67452: E9 56 02 00 00     jmp         0000000140B676AD
  0000000140B67457: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6745F: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],1
                    01 00 00 00
  0000000140B67469: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67471: C7 80 A0 00 00 00  mov         dword ptr [rax+00000000000000A0h],1
                    01 00 00 00
  0000000140B6747B: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67484: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6748C: 89 88 AC 00 00 00  mov         dword ptr [rax+00000000000000ACh],ecx
  0000000140B67492: E9 16 02 00 00     jmp         0000000140B676AD
  0000000140B67497: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6749F: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],1
                    01 00 00 00
  0000000140B674A9: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B674B1: C7 80 A0 00 00 00  mov         dword ptr [rax+00000000000000A0h],4
                    04 00 00 00
  0000000140B674BB: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B674C4: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B674CC: 89 88 AC 00 00 00  mov         dword ptr [rax+00000000000000ACh],ecx
  0000000140B674D2: E9 D6 01 00 00     jmp         0000000140B676AD
  0000000140B674D7: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B674DF: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],1
                    01 00 00 00
  0000000140B674E9: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B674F1: C7 80 A0 00 00 00  mov         dword ptr [rax+00000000000000A0h],3
                    03 00 00 00
  0000000140B674FB: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67504: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6750C: 89 88 AC 00 00 00  mov         dword ptr [rax+00000000000000ACh],ecx
  0000000140B67512: E9 96 01 00 00     jmp         0000000140B676AD
  0000000140B67517: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6751F: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],1
                    01 00 00 00
  0000000140B67529: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67531: C7 80 A0 00 00 00  mov         dword ptr [rax+00000000000000A0h],15h
                    15 00 00 00
  0000000140B6753B: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67544: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6754C: 89 88 AC 00 00 00  mov         dword ptr [rax+00000000000000ACh],ecx
  0000000140B67552: E9 56 01 00 00     jmp         0000000140B676AD
  0000000140B67557: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6755F: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],3
                    03 00 00 00
  0000000140B67569: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67571: C7 80 A0 00 00 00  mov         dword ptr [rax+00000000000000A0h],4
                    04 00 00 00
  0000000140B6757B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67583: C7 80 A4 00 00 00  mov         dword ptr [rax+00000000000000A4h],3
                    03 00 00 00
  0000000140B6758D: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67595: C7 80 A8 00 00 00  mov         dword ptr [rax+00000000000000A8h],15h
                    15 00 00 00
  0000000140B6759F: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B675A8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B675B0: 89 88 AC 00 00 00  mov         dword ptr [rax+00000000000000ACh],ecx
  0000000140B675B6: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B675BC: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B675C4: 89 88 B0 00 00 00  mov         dword ptr [rax+00000000000000B0h],ecx
  0000000140B675CA: F2 0F 2C 4C 24 48  cvttsd2si   ecx,mmword ptr [rsp+48h]
  0000000140B675D0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B675D8: 89 88 B4 00 00 00  mov         dword ptr [rax+00000000000000B4h],ecx
  0000000140B675DE: E9 CA 00 00 00     jmp         0000000140B676AD
  0000000140B675E3: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B675EB: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],0
                    00 00 00 00
  0000000140B675F5: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B675FD: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67606: F2 0F 11 80 38 01  movsd       mmword ptr [rax+0000000000000138h],xmm0
                    00 00
  0000000140B6760E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67616: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B6761C: F2 0F 11 80 40 01  movsd       mmword ptr [rax+0000000000000140h],xmm0
                    00 00
  0000000140B67624: E9 84 00 00 00     jmp         0000000140B676AD
  0000000140B67629: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67631: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],0
                    00 00 00 00
  0000000140B6763B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67643: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6764C: F2 0F 11 80 48 01  movsd       mmword ptr [rax+0000000000000148h],xmm0
                    00 00
  0000000140B67654: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6765C: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B67662: F2 0F 11 80 50 01  movsd       mmword ptr [rax+0000000000000150h],xmm0
                    00 00
  0000000140B6766A: EB 41              jmp         0000000140B676AD
  0000000140B6766C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67674: C7 80 94 00 00 00  mov         dword ptr [rax+0000000000000094h],0
                    00 00 00 00
  0000000140B6767E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67686: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6768F: F2 0F 11 80 58 01  movsd       mmword ptr [rax+0000000000000158h],xmm0
                    00 00
  0000000140B67697: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6769F: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B676A5: F2 0F 11 80 60 01  movsd       mmword ptr [rax+0000000000000160h],xmm0
                    00 00
  0000000140B676AD: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B676B5: 83 B8 94 00 00 00  cmp         dword ptr [rax+0000000000000094h],0
                    00
  0000000140B676BC: 74 26              je          0000000140B676E4
  0000000140B676BE: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B676C6: 48 81 C2 90 00 00  add         rdx,90h
                    00
  0000000140B676CD: 45 33 C0           xor         r8d,r8d
  0000000140B676D0: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B676D8: E8 13 FD F8 FF     call        0000000140AF73F0
  0000000140B676DD: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B676E4: E9 86 17 00 00     jmp         0000000140B68E6F
  0000000140B676E9: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B676F1: E8 6A DE 4F FF     call        0000000140065560
  0000000140B676F6: 0F B6 C0           movzx       eax,al
  0000000140B676F9: 85 C0              test        eax,eax
  0000000140B676FB: 0F 84 42 01 00 00  je          0000000140B67843
  0000000140B67701: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B67709: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B67711: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67719: E8 92 A9 F8 FF     call        0000000140AF20B0
  0000000140B6771E: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67725: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B6772D: 74 0A              je          0000000140B67739
  0000000140B6772F: B8 02 00 00 00     mov         eax,2
  0000000140B67734: E9 41 18 00 00     jmp         0000000140B68F7A
  0000000140B67739: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67741: 48 81 C1 40 02 00  add         rcx,240h
                    00
  0000000140B67748: BA 16 00 00 00     mov         edx,16h
  0000000140B6774D: E8 8E 42 D3 FF     call        000000014089B9E0
  0000000140B67752: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B6775A: 75 30              jne         0000000140B6778C
  0000000140B6775C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67764: C7 80 44 02 00 00  mov         dword ptr [rax+0000000000000244h],0
                    00 00 00 00
  0000000140B6776E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67776: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6777F: F2 0F 11 80 48 02  movsd       mmword ptr [rax+0000000000000248h],xmm0
                    00 00
  0000000140B67787: E9 8C 00 00 00     jmp         0000000140B67818
  0000000140B6778C: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],1
                    00 01
  0000000140B67794: 75 3D              jne         0000000140B677D3
  0000000140B67796: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6779E: C7 80 44 02 00 00  mov         dword ptr [rax+0000000000000244h],1
                    01 00 00 00
  0000000140B677A8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B677B0: C7 80 50 02 00 00  mov         dword ptr [rax+0000000000000250h],1
                    01 00 00 00
  0000000140B677BA: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B677C3: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B677CB: 89 88 5C 02 00 00  mov         dword ptr [rax+000000000000025Ch],ecx
  0000000140B677D1: EB 45              jmp         0000000140B67818
  0000000140B677D3: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],2
                    00 02
  0000000140B677DB: 75 3B              jne         0000000140B67818
  0000000140B677DD: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B677E5: C7 80 44 02 00 00  mov         dword ptr [rax+0000000000000244h],1
                    01 00 00 00
  0000000140B677EF: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B677F7: C7 80 50 02 00 00  mov         dword ptr [rax+0000000000000250h],4
                    04 00 00 00
  0000000140B67801: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6780A: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67812: 89 88 5C 02 00 00  mov         dword ptr [rax+000000000000025Ch],ecx
  0000000140B67818: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67820: 48 81 C2 40 02 00  add         rdx,240h
                    00
  0000000140B67827: 45 33 C0           xor         r8d,r8d
  0000000140B6782A: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67832: E8 B9 FB F8 FF     call        0000000140AF73F0
  0000000140B67837: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B6783E: E9 2C 16 00 00     jmp         0000000140B68E6F
  0000000140B67843: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B6784B: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B67853: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B6785B: E8 50 A8 F8 FF     call        0000000140AF20B0
  0000000140B67860: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67867: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B6786F: 74 0A              je          0000000140B6787B
  0000000140B67871: B8 02 00 00 00     mov         eax,2
  0000000140B67876: E9 FF 16 00 00     jmp         0000000140B68F7A
  0000000140B6787B: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67883: 48 81 C1 B8 01 00  add         rcx,1B8h
                    00
  0000000140B6788A: BA 17 00 00 00     mov         edx,17h
  0000000140B6788F: E8 4C 41 D3 FF     call        000000014089B9E0
  0000000140B67894: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B6789C: 75 30              jne         0000000140B678CE
  0000000140B6789E: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B678A6: C7 80 BC 01 00 00  mov         dword ptr [rax+00000000000001BCh],0
                    00 00 00 00
  0000000140B678B0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B678B8: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B678C1: F2 0F 11 80 C0 01  movsd       mmword ptr [rax+00000000000001C0h],xmm0
                    00 00
  0000000140B678C9: E9 D8 00 00 00     jmp         0000000140B679A6
  0000000140B678CE: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],1
                    00 01
  0000000140B678D6: 75 63              jne         0000000140B6793B
  0000000140B678D8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B678E0: C7 80 BC 01 00 00  mov         dword ptr [rax+00000000000001BCh],1
                    01 00 00 00
  0000000140B678EA: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B678F2: C7 80 C8 01 00 00  mov         dword ptr [rax+00000000000001C8h],1
                    01 00 00 00
  0000000140B678FC: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67905: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6790D: 89 88 D4 01 00 00  mov         dword ptr [rax+00000000000001D4h],ecx
  0000000140B67913: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6791B: 48 81 C2 B8 01 00  add         rdx,1B8h
                    00
  0000000140B67922: 45 33 C0           xor         r8d,r8d
  0000000140B67925: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B6792D: E8 BE FA F8 FF     call        0000000140AF73F0
  0000000140B67932: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67939: EB 6B              jmp         0000000140B679A6
  0000000140B6793B: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],2
                    00 02
  0000000140B67943: 75 61              jne         0000000140B679A6
  0000000140B67945: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6794D: C7 80 BC 01 00 00  mov         dword ptr [rax+00000000000001BCh],1
                    01 00 00 00
  0000000140B67957: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6795F: C7 80 C8 01 00 00  mov         dword ptr [rax+00000000000001C8h],4
                    04 00 00 00
  0000000140B67969: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67972: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B6797A: 89 88 D4 01 00 00  mov         dword ptr [rax+00000000000001D4h],ecx
  0000000140B67980: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67988: 48 81 C2 B8 01 00  add         rdx,1B8h
                    00
  0000000140B6798F: 45 33 C0           xor         r8d,r8d
  0000000140B67992: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B6799A: E8 51 FA F8 FF     call        0000000140AF73F0
  0000000140B6799F: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B679A6: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B679AE: 7D 2B              jge         0000000140B679DB
  0000000140B679B0: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B679B7: F7 D9              neg         ecx
  0000000140B679B9: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B679C1: 89 88 30 02 00 00  mov         dword ptr [rax+0000000000000230h],ecx
  0000000140B679C7: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B679CF: C7 80 34 02 00 00  mov         dword ptr [rax+0000000000000234h],1
                    01 00 00 00
  0000000140B679D9: EB 24              jmp         0000000140B679FF
  0000000140B679DB: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B679E3: C7 80 30 02 00 00  mov         dword ptr [rax+0000000000000230h],0
                    00 00 00 00
  0000000140B679ED: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B679F5: C7 80 34 02 00 00  mov         dword ptr [rax+0000000000000234h],0
                    00 00 00 00
  0000000140B679FF: E9 6B 14 00 00     jmp         0000000140B68E6F
  0000000140B67A04: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B67A0C: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B67A14: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67A1C: E8 8F A6 F8 FF     call        0000000140AF20B0
  0000000140B67A21: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67A28: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B67A30: 74 0A              je          0000000140B67A3C
  0000000140B67A32: B8 02 00 00 00     mov         eax,2
  0000000140B67A37: E9 3E 15 00 00     jmp         0000000140B68F7A
  0000000140B67A3C: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67A44: 48 81 C1 B8 02 00  add         rcx,2B8h
                    00
  0000000140B67A4B: BA 4E 00 00 00     mov         edx,4Eh
  0000000140B67A50: E8 8B 3F D3 FF     call        000000014089B9E0
  0000000140B67A55: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B67A5D: 75 30              jne         0000000140B67A8F
  0000000140B67A5F: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67A67: C7 80 BC 02 00 00  mov         dword ptr [rax+00000000000002BCh],0
                    00 00 00 00
  0000000140B67A71: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67A79: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67A82: F2 0F 11 80 C0 02  movsd       mmword ptr [rax+00000000000002C0h],xmm0
                    00 00
  0000000140B67A8A: E9 8C 00 00 00     jmp         0000000140B67B1B
  0000000140B67A8F: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],1
                    00 01
  0000000140B67A97: 75 3D              jne         0000000140B67AD6
  0000000140B67A99: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67AA1: C7 80 BC 02 00 00  mov         dword ptr [rax+00000000000002BCh],1
                    01 00 00 00
  0000000140B67AAB: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67AB3: C7 80 C8 02 00 00  mov         dword ptr [rax+00000000000002C8h],3
                    03 00 00 00
  0000000140B67ABD: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67AC6: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67ACE: 89 88 D4 02 00 00  mov         dword ptr [rax+00000000000002D4h],ecx
  0000000140B67AD4: EB 45              jmp         0000000140B67B1B
  0000000140B67AD6: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],2
                    00 02
  0000000140B67ADE: 75 3B              jne         0000000140B67B1B
  0000000140B67AE0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67AE8: C7 80 BC 02 00 00  mov         dword ptr [rax+00000000000002BCh],1
                    01 00 00 00
  0000000140B67AF2: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67AFA: C7 80 C8 02 00 00  mov         dword ptr [rax+00000000000002C8h],4
                    04 00 00 00
  0000000140B67B04: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67B0D: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67B15: 89 88 D4 02 00 00  mov         dword ptr [rax+00000000000002D4h],ecx
  0000000140B67B1B: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67B23: 48 81 C2 B8 02 00  add         rdx,2B8h
                    00
  0000000140B67B2A: 45 33 C0           xor         r8d,r8d
  0000000140B67B2D: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67B35: E8 B6 F8 F8 FF     call        0000000140AF73F0
  0000000140B67B3A: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67B41: E9 29 13 00 00     jmp         0000000140B68E6F
  0000000140B67B46: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B67B4E: 48 8D 94 24 D4 00  lea         rdx,[rsp+00000000000000D4h]
                    00 00
  0000000140B67B56: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67B5E: E8 4D A5 F8 FF     call        0000000140AF20B0
  0000000140B67B63: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67B6A: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B67B72: 74 0A              je          0000000140B67B7E
  0000000140B67B74: B8 02 00 00 00     mov         eax,2
  0000000140B67B79: E9 FC 13 00 00     jmp         0000000140B68F7A
  0000000140B67B7E: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B67B86: 75 14              jne         0000000140B67B9C
  0000000140B67B88: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67B90: C7 80 78 01 00 00  mov         dword ptr [rax+0000000000000178h],0
                    00 00 00 00
  0000000140B67B9A: EB 3A              jmp         0000000140B67BD6
  0000000140B67B9C: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],1
                    00 01
  0000000140B67BA4: 75 14              jne         0000000140B67BBA
  0000000140B67BA6: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67BAE: C7 80 78 01 00 00  mov         dword ptr [rax+0000000000000178h],1
                    01 00 00 00
  0000000140B67BB8: EB 1C              jmp         0000000140B67BD6
  0000000140B67BBA: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],2
                    00 02
  0000000140B67BC2: 75 12              jne         0000000140B67BD6
  0000000140B67BC4: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67BCC: C7 80 78 01 00 00  mov         dword ptr [rax+0000000000000178h],2
                    02 00 00 00
  0000000140B67BD6: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67BDE: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B67BE7: F2 0F 11 80 80 01  movsd       mmword ptr [rax+0000000000000180h],xmm0
                    00 00
  0000000140B67BEF: E9 7B 12 00 00     jmp         0000000140B68E6F
  0000000140B67BF4: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B67BFC: E8 5F D9 4F FF     call        0000000140065560
  0000000140B67C01: 0F B6 C0           movzx       eax,al
  0000000140B67C04: 85 C0              test        eax,eax
  0000000140B67C06: 74 23              je          0000000140B67C2B
  0000000140B67C08: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67C10: 48 81 C2 38 02 00  add         rdx,238h
                    00
  0000000140B67C17: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67C1F: E8 5C 99 F8 FF     call        0000000140AF1580
  0000000140B67C24: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67C2B: E9 3F 12 00 00     jmp         0000000140B68E6F
  0000000140B67C30: C7 84 24 00 01 00  mov         dword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00
  0000000140B67C3B: C7 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00
  0000000140B67C46: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B67C4E: E8 0D D9 4F FF     call        0000000140065560
  0000000140B67C53: 0F B6 C0           movzx       eax,al
  0000000140B67C56: 85 C0              test        eax,eax
  0000000140B67C58: 0F 84 B9 00 00 00  je          0000000140B67D17
  0000000140B67C5E: 41 B8 08 00 00 00  mov         r8d,8
  0000000140B67C64: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000140B67C6C: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67C74: E8 77 88 F8 FF     call        0000000140AF04F0
  0000000140B67C79: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67C80: 33 C9              xor         ecx,ecx
  0000000140B67C82: E8 B9 89 F8 FF     call        0000000140AF0640
  0000000140B67C87: 48 8B C8           mov         rcx,rax
  0000000140B67C8A: E8 1D BB 90 00     call        00000001414737AC
  0000000140B67C8F: 89 84 24 FC 00 00  mov         dword ptr [rsp+00000000000000FCh],eax
                    00
  0000000140B67C96: B9 01 00 00 00     mov         ecx,1
  0000000140B67C9B: E8 A0 89 F8 FF     call        0000000140AF0640
  0000000140B67CA0: 48 8B C8           mov         rcx,rax
  0000000140B67CA3: E8 04 BB 90 00     call        00000001414737AC
  0000000140B67CA8: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  0000000140B67CAF: B9 02 00 00 00     mov         ecx,2
  0000000140B67CB4: E8 87 89 F8 FF     call        0000000140AF0640
  0000000140B67CB9: 48 8B C8           mov         rcx,rax
  0000000140B67CBC: E8 EB BA 90 00     call        00000001414737AC
  0000000140B67CC1: 89 84 24 D4 00 00  mov         dword ptr [rsp+00000000000000D4h],eax
                    00
  0000000140B67CC8: B9 03 00 00 00     mov         ecx,3
  0000000140B67CCD: E8 6E 89 F8 FF     call        0000000140AF0640
  0000000140B67CD2: 48 8B C8           mov         rcx,rax
  0000000140B67CD5: E8 D2 BA 90 00     call        00000001414737AC
  0000000140B67CDA: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140B67CE1: B9 04 00 00 00     mov         ecx,4
  0000000140B67CE6: E8 55 89 F8 FF     call        0000000140AF0640
  0000000140B67CEB: 48 8B C8           mov         rcx,rax
  0000000140B67CEE: E8 B1 BA 90 00     call        00000001414737A4
  0000000140B67CF3: F2 0F 11 44 24 40  movsd       mmword ptr [rsp+40h],xmm0
  0000000140B67CF9: B9 05 00 00 00     mov         ecx,5
  0000000140B67CFE: E8 3D 89 F8 FF     call        0000000140AF0640
  0000000140B67D03: 48 8B C8           mov         rcx,rax
  0000000140B67D06: E8 A1 BA 90 00     call        00000001414737AC
  0000000140B67D0B: 89 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],eax
                    00
  0000000140B67D12: E9 BF 00 00 00     jmp         0000000140B67DD6
  0000000140B67D17: C7 84 24 D0 00 00  mov         dword ptr [rsp+00000000000000D0h],0
                    00 00 00 00 00
  0000000140B67D22: 41 B8 06 00 00 00  mov         r8d,6
  0000000140B67D28: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000140B67D30: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67D38: E8 B3 87 F8 FF     call        0000000140AF04F0
  0000000140B67D3D: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67D44: 33 C9              xor         ecx,ecx
  0000000140B67D46: E8 F5 88 F8 FF     call        0000000140AF0640
  0000000140B67D4B: 48 8B C8           mov         rcx,rax
  0000000140B67D4E: E8 59 BA 90 00     call        00000001414737AC
  0000000140B67D53: 89 84 24 FC 00 00  mov         dword ptr [rsp+00000000000000FCh],eax
                    00
  0000000140B67D5A: B9 01 00 00 00     mov         ecx,1
  0000000140B67D5F: E8 DC 88 F8 FF     call        0000000140AF0640
  0000000140B67D64: 48 8B C8           mov         rcx,rax
  0000000140B67D67: E8 40 BA 90 00     call        00000001414737AC
  0000000140B67D6C: 89 84 24 D0 00 00  mov         dword ptr [rsp+00000000000000D0h],eax
                    00
  0000000140B67D73: B9 02 00 00 00     mov         ecx,2
  0000000140B67D78: E8 C3 88 F8 FF     call        0000000140AF0640
  0000000140B67D7D: 48 8B C8           mov         rcx,rax
  0000000140B67D80: E8 27 BA 90 00     call        00000001414737AC
  0000000140B67D85: 89 84 24 D4 00 00  mov         dword ptr [rsp+00000000000000D4h],eax
                    00
  0000000140B67D8C: B9 03 00 00 00     mov         ecx,3
  0000000140B67D91: E8 AA 88 F8 FF     call        0000000140AF0640
  0000000140B67D96: 48 8B C8           mov         rcx,rax
  0000000140B67D99: E8 0E BA 90 00     call        00000001414737AC
  0000000140B67D9E: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140B67DA5: B9 04 00 00 00     mov         ecx,4
  0000000140B67DAA: E8 91 88 F8 FF     call        0000000140AF0640
  0000000140B67DAF: 48 8B C8           mov         rcx,rax
  0000000140B67DB2: E8 ED B9 90 00     call        00000001414737A4
  0000000140B67DB7: F2 0F 11 44 24 40  movsd       mmword ptr [rsp+40h],xmm0
  0000000140B67DBD: B9 05 00 00 00     mov         ecx,5
  0000000140B67DC2: E8 79 88 F8 FF     call        0000000140AF0640
  0000000140B67DC7: 48 8B C8           mov         rcx,rax
  0000000140B67DCA: E8 DD B9 90 00     call        00000001414737AC
  0000000140B67DCF: 89 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],eax
                    00
  0000000140B67DD6: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B67DDE: 74 0A              je          0000000140B67DEA
  0000000140B67DE0: B8 02 00 00 00     mov         eax,2
  0000000140B67DE5: E9 90 11 00 00     jmp         0000000140B68F7A
  0000000140B67DEA: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67DF2: 48 81 C1 88 01 00  add         rcx,188h
                    00
  0000000140B67DF9: 8B 94 24 FC 00 00  mov         edx,dword ptr [rsp+00000000000000FCh]
                    00
  0000000140B67E00: E8 EB 52 EB FF     call        0000000140A1D0F0
  0000000140B67E05: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140B67E0D: BA 01 00 00 00     mov         edx,1
  0000000140B67E12: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67E1A: E8 B1 88 EA FF     call        0000000140A106D0
  0000000140B67E1F: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],1
                    00 01
  0000000140B67E27: 75 11              jne         0000000140B67E3A
  0000000140B67E29: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67E31: C7 40 08 01 00 00  mov         dword ptr [rax+8],1
                    00
  0000000140B67E38: EB 4F              jmp         0000000140B67E89
  0000000140B67E3A: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],2
                    00 02
  0000000140B67E42: 75 11              jne         0000000140B67E55
  0000000140B67E44: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67E4C: C7 40 08 02 00 00  mov         dword ptr [rax+8],2
                    00
  0000000140B67E53: EB 34              jmp         0000000140B67E89
  0000000140B67E55: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],3
                    00 03
  0000000140B67E5D: 75 11              jne         0000000140B67E70
  0000000140B67E5F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67E67: C7 40 08 03 00 00  mov         dword ptr [rax+8],3
                    00
  0000000140B67E6E: EB 19              jmp         0000000140B67E89
  0000000140B67E70: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],4
                    00 04
  0000000140B67E78: 75 0F              jne         0000000140B67E89
  0000000140B67E7A: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67E82: C7 40 08 04 00 00  mov         dword ptr [rax+8],4
                    00
  0000000140B67E89: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B67E91: E8 CA D6 4F FF     call        0000000140065560
  0000000140B67E96: 0F B6 C0           movzx       eax,al
  0000000140B67E99: 85 C0              test        eax,eax
  0000000140B67E9B: 74 70              je          0000000140B67F0D
  0000000140B67E9D: 41 B1 01           mov         r9b,1
  0000000140B67EA0: 44 8B 84 24 98 00  mov         r8d,dword ptr [rsp+0000000000000098h]
                    00 00
  0000000140B67EA8: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67EB0: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67EB8: E8 D3 03 F9 FF     call        0000000140AF8290
  0000000140B67EBD: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67EC4: B9 06 00 00 00     mov         ecx,6
  0000000140B67EC9: E8 72 87 F8 FF     call        0000000140AF0640
  0000000140B67ECE: 48 8B C8           mov         rcx,rax
  0000000140B67ED1: E8 CE B8 90 00     call        00000001414737A4
  0000000140B67ED6: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67EDE: F2 0F 11 80 50 03  movsd       mmword ptr [rax+0000000000000350h],xmm0
                    00 00
  0000000140B67EE6: B9 07 00 00 00     mov         ecx,7
  0000000140B67EEB: E8 50 87 F8 FF     call        0000000140AF0640
  0000000140B67EF0: 48 8B C8           mov         rcx,rax
  0000000140B67EF3: E8 AC B8 90 00     call        00000001414737A4
  0000000140B67EF8: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67F00: F2 0F 11 80 58 03  movsd       mmword ptr [rax+0000000000000358h],xmm0
                    00 00
  0000000140B67F08: E9 C7 00 00 00     jmp         0000000140B67FD4
  0000000140B67F0D: 4C 8B 8C 24 F0 00  mov         r9,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67F15: 49 81 C1 60 03 00  add         r9,360h
                    00
  0000000140B67F1C: 4C 8B 84 24 F0 00  mov         r8,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67F24: 49 81 C0 58 03 00  add         r8,358h
                    00
  0000000140B67F2B: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67F33: 48 81 C2 50 03 00  add         rdx,350h
                    00
  0000000140B67F3A: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67F42: E8 C9 98 F8 FF     call        0000000140AF1810
  0000000140B67F47: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67F4E: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B67F56: 74 0A              je          0000000140B67F62
  0000000140B67F58: B8 02 00 00 00     mov         eax,2
  0000000140B67F5D: E9 18 10 00 00     jmp         0000000140B68F7A
  0000000140B67F62: 83 BC 24 D0 00 00  cmp         dword ptr [rsp+00000000000000D0h],1
                    00 01
  0000000140B67F6A: 75 14              jne         0000000140B67F80
  0000000140B67F6C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67F74: C7 80 10 02 00 00  mov         dword ptr [rax+0000000000000210h],1
                    01 00 00 00
  0000000140B67F7E: EB 28              jmp         0000000140B67FA8
  0000000140B67F80: 83 BC 24 D0 00 00  cmp         dword ptr [rsp+00000000000000D0h],2
                    00 02
  0000000140B67F88: 75 14              jne         0000000140B67F9E
  0000000140B67F8A: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67F92: C7 80 10 02 00 00  mov         dword ptr [rax+0000000000000210h],2
                    02 00 00 00
  0000000140B67F9C: EB 0A              jmp         0000000140B67FA8
  0000000140B67F9E: B8 02 00 00 00     mov         eax,2
  0000000140B67FA3: E9 D2 0F 00 00     jmp         0000000140B68F7A
  0000000140B67FA8: 41 B0 01           mov         r8b,1
  0000000140B67FAB: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67FB3: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B67FBB: E8 00 04 F9 FF     call        0000000140AF83C0
  0000000140B67FC0: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B67FC7: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67FCF: E8 BC 2C EB FF     call        0000000140A1AC90
  0000000140B67FD4: 83 BC 24 F8 00 00  cmp         dword ptr [rsp+00000000000000F8h],0
                    00 00
  0000000140B67FDC: 75 66              jne         0000000140B68044
  0000000140B67FDE: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B67FE6: C7 80 68 03 00 00  mov         dword ptr [rax+0000000000000368h],2
                    02 00 00 00
  0000000140B67FF0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B67FF8: 83 B8 70 01 00 00  cmp         dword ptr [rax+0000000000000170h],1
                    01
  0000000140B67FFF: 75 20              jne         0000000140B68021
  0000000140B68001: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68009: 8B 88 6C 01 00 00  mov         ecx,dword ptr [rax+000000000000016Ch]
  0000000140B6800F: F7 D9              neg         ecx
  0000000140B68011: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68019: 89 88 6C 03 00 00  mov         dword ptr [rax+000000000000036Ch],ecx
  0000000140B6801F: EB 1E              jmp         0000000140B6803F
  0000000140B68021: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68029: 8B 88 68 01 00 00  mov         ecx,dword ptr [rax+0000000000000168h]
  0000000140B6802F: F7 D9              neg         ecx
  0000000140B68031: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68039: 89 88 6C 03 00 00  mov         dword ptr [rax+000000000000036Ch],ecx
  0000000140B6803F: E9 82 00 00 00     jmp         0000000140B680C6
  0000000140B68044: 83 BC 24 F8 00 00  cmp         dword ptr [rsp+00000000000000F8h],1
                    00 01
  0000000140B6804C: 75 14              jne         0000000140B68062
  0000000140B6804E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68056: C7 80 68 03 00 00  mov         dword ptr [rax+0000000000000368h],0
                    00 00 00 00
  0000000140B68060: EB 64              jmp         0000000140B680C6
  0000000140B68062: 83 BC 24 F8 00 00  cmp         dword ptr [rsp+00000000000000F8h],2
                    00 02
  0000000140B6806A: 75 14              jne         0000000140B68080
  0000000140B6806C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68074: C7 80 68 03 00 00  mov         dword ptr [rax+0000000000000368h],1
                    01 00 00 00
  0000000140B6807E: EB 46              jmp         0000000140B680C6
  0000000140B68080: 83 BC 24 F8 00 00  cmp         dword ptr [rsp+00000000000000F8h],0
                    00 00
  0000000140B68088: 7D 32              jge         0000000140B680BC
  0000000140B6808A: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68092: C7 80 68 03 00 00  mov         dword ptr [rax+0000000000000368h],2
                    02 00 00 00
  0000000140B6809C: 8B 8C 24 F8 00 00  mov         ecx,dword ptr [rsp+00000000000000F8h]
                    00
  0000000140B680A3: E8 A4 CA C8 00     call        00000001417F4B4C
  0000000140B680A8: 44 8B D8           mov         r11d,eax
  0000000140B680AB: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B680B3: 44 89 98 6C 03 00  mov         dword ptr [rax+000000000000036Ch],r11d
                    00
  0000000140B680BA: EB 0A              jmp         0000000140B680C6
  0000000140B680BC: B8 02 00 00 00     mov         eax,2
  0000000140B680C1: E9 B4 0E 00 00     jmp         0000000140B68F7A
  0000000140B680C6: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B680CE: 83 78 08 04        cmp         dword ptr [rax+8],4
  0000000140B680D2: 0F 84 AC 03 00 00  je          0000000140B68484
  0000000140B680D8: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B680E0: 7D 47              jge         0000000140B68129
  0000000140B680E2: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B680EA: C6 40 0C 01        mov         byte ptr [rax+0Ch],1
  0000000140B680EE: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B680F5: F7 D9              neg         ecx
  0000000140B680F7: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B680FF: 89 48 10           mov         dword ptr [rax+10h],ecx
  0000000140B68102: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6810A: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B68111: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68119: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B6811F: F2 0F 11 40 28     movsd       mmword ptr [rax+28h],xmm0
  0000000140B68124: E9 56 03 00 00     jmp         0000000140B6847F
  0000000140B68129: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68131: C6 40 0C 00        mov         byte ptr [rax+0Ch],0
  0000000140B68135: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6813D: 48 83 C1 20        add         rcx,20h
  0000000140B68141: BA 15 00 00 00     mov         edx,15h
  0000000140B68146: E8 95 38 D3 FF     call        000000014089B9E0
  0000000140B6814B: 4C 8B 9C 24 F0 00  mov         r11,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68153: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B6815A: 41 89 43 18        mov         dword ptr [r11+18h],eax
  0000000140B6815E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68166: 8B 40 18           mov         eax,dword ptr [rax+18h]
  0000000140B68169: 89 84 24 20 01 00  mov         dword ptr [rsp+0000000000000120h],eax
                    00
  0000000140B68170: 83 BC 24 20 01 00  cmp         dword ptr [rsp+0000000000000120h],8
                    00 08
  0000000140B68178: 0F 87 D0 02 00 00  ja          0000000140B6844E
  0000000140B6817E: 48 63 84 24 20 01  movsxd      rax,dword ptr [rsp+0000000000000120h]
                    00 00
  0000000140B68186: 48 8D 0D 73 7E 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B6818D: 8B 84 81 18 90 B6  mov         eax,dword ptr [rcx+rax*4+0000000000B69018h]
                    00
  0000000140B68194: 48 03 C1           add         rax,rcx
  0000000140B68197: FF E0              jmp         rax
  0000000140B68199: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B681A1: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B681A8: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B681B0: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B681B6: F2 0F 11 40 28     movsd       mmword ptr [rax+28h],xmm0
  0000000140B681BB: E9 8E 02 00 00     jmp         0000000140B6844E
  0000000140B681C0: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B681C8: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B681CF: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B681D7: C7 40 30 01 00 00  mov         dword ptr [rax+30h],1
                    00
  0000000140B681DE: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B681E4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B681EC: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B681EF: E9 5A 02 00 00     jmp         0000000140B6844E
  0000000140B681F4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B681FC: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B68203: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6820B: C7 40 30 04 00 00  mov         dword ptr [rax+30h],4
                    00
  0000000140B68212: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B68218: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68220: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B68223: E9 26 02 00 00     jmp         0000000140B6844E
  0000000140B68228: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68230: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B68237: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6823F: C7 40 30 03 00 00  mov         dword ptr [rax+30h],3
                    00
  0000000140B68246: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B6824C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68254: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B68257: E9 F2 01 00 00     jmp         0000000140B6844E
  0000000140B6825C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68264: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B6826B: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68273: C7 40 30 15 00 00  mov         dword ptr [rax+30h],15h
                    00
  0000000140B6827A: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B68280: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68288: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B6828B: E9 BE 01 00 00     jmp         0000000140B6844E
  0000000140B68290: 4C 8D 8C 24 A8 00  lea         r9,[rsp+00000000000000A8h]
                    00 00
  0000000140B68298: 4C 8D 84 24 A4 00  lea         r8,[rsp+00000000000000A4h]
                    00 00
  0000000140B682A0: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140B682A9: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140B682B1: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B682B9: E8 E2 88 F8 FF     call        0000000140AF0BA0
  0000000140B682BE: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B682C5: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B682CD: C7 40 24 03 00 00  mov         dword ptr [rax+24h],3
                    00
  0000000140B682D4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B682DC: C7 40 30 04 00 00  mov         dword ptr [rax+30h],4
                    00
  0000000140B682E3: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B682EB: C7 40 34 03 00 00  mov         dword ptr [rax+34h],3
                    00
  0000000140B682F2: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B682FA: C7 40 38 15 00 00  mov         dword ptr [rax+38h],15h
                    00
  0000000140B68301: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68309: 8B 84 24 A0 00 00  mov         eax,dword ptr [rsp+00000000000000A0h]
                    00
  0000000140B68310: 89 41 3C           mov         dword ptr [rcx+3Ch],eax
  0000000140B68313: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6831B: 8B 84 24 A4 00 00  mov         eax,dword ptr [rsp+00000000000000A4h]
                    00
  0000000140B68322: 89 41 40           mov         dword ptr [rcx+40h],eax
  0000000140B68325: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6832D: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  0000000140B68334: 89 41 44           mov         dword ptr [rcx+44h],eax
  0000000140B68337: E9 12 01 00 00     jmp         0000000140B6844E
  0000000140B6833C: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  0000000140B68341: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140B68346: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B6834E: E8 DD 92 F8 FF     call        0000000140AF1630
  0000000140B68353: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B6835A: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68362: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B68369: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68371: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68377: F2 0F 11 80 10 01  movsd       mmword ptr [rax+0000000000000110h],xmm0
                    00 00
  0000000140B6837F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68387: F2 0F 10 44 24 48  movsd       xmm0,mmword ptr [rsp+48h]
  0000000140B6838D: F2 0F 11 80 18 01  movsd       mmword ptr [rax+0000000000000118h],xmm0
                    00 00
  0000000140B68395: E9 B4 00 00 00     jmp         0000000140B6844E
  0000000140B6839A: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  0000000140B6839F: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140B683A4: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B683AC: E8 7F 92 F8 FF     call        0000000140AF1630
  0000000140B683B1: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B683B8: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B683C0: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B683C7: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B683CF: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B683D5: F2 0F 11 80 20 01  movsd       mmword ptr [rax+0000000000000120h],xmm0
                    00 00
  0000000140B683DD: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B683E5: F2 0F 10 44 24 48  movsd       xmm0,mmword ptr [rsp+48h]
  0000000140B683EB: F2 0F 11 80 28 01  movsd       mmword ptr [rax+0000000000000128h],xmm0
                    00 00
  0000000140B683F3: EB 59              jmp         0000000140B6844E
  0000000140B683F5: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  0000000140B683FA: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140B683FF: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68407: E8 24 92 F8 FF     call        0000000140AF1630
  0000000140B6840C: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68413: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6841B: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B68422: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6842A: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68430: F2 0F 11 80 30 01  movsd       mmword ptr [rax+0000000000000130h],xmm0
                    00 00
  0000000140B68438: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68440: F2 0F 10 44 24 48  movsd       xmm0,mmword ptr [rsp+48h]
  0000000140B68446: F2 0F 11 80 38 01  movsd       mmword ptr [rax+0000000000000138h],xmm0
                    00 00
  0000000140B6844E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68456: 83 78 24 00        cmp         dword ptr [rax+24h],0
  0000000140B6845A: 74 23              je          0000000140B6847F
  0000000140B6845C: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68464: 48 83 C2 20        add         rdx,20h
  0000000140B68468: 45 33 C0           xor         r8d,r8d
  0000000140B6846B: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68473: E8 78 EF F8 FF     call        0000000140AF73F0
  0000000140B68478: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B6847F: E9 48 07 00 00     jmp         0000000140B68BCC
  0000000140B68484: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140B68489: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B6848E: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  0000000140B68493: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B6849B: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B684A3: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B684AB: E8 C0 9F F8 FF     call        0000000140AF2470
  0000000140B684B0: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B684B7: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B684BF: 74 0A              je          0000000140B684CB
  0000000140B684C1: B8 02 00 00 00     mov         eax,2
  0000000140B684C6: E9 AF 0A 00 00     jmp         0000000140B68F7A
  0000000140B684CB: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B684D3: 7D 4A              jge         0000000140B6851F
  0000000140B684D5: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B684DD: C6 40 0C 01        mov         byte ptr [rax+0Ch],1
  0000000140B684E1: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B684E8: F7 D9              neg         ecx
  0000000140B684EA: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B684F2: 89 48 10           mov         dword ptr [rax+10h],ecx
  0000000140B684F5: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B684FD: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B68504: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6850C: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68515: F2 0F 11 40 28     movsd       mmword ptr [rax+28h],xmm0
  0000000140B6851A: E9 DC 02 00 00     jmp         0000000140B687FB
  0000000140B6851F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68527: C6 40 0C 00        mov         byte ptr [rax+0Ch],0
  0000000140B6852B: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68533: 48 83 C1 20        add         rcx,20h
  0000000140B68537: BA 15 00 00 00     mov         edx,15h
  0000000140B6853C: E8 9F 34 D3 FF     call        000000014089B9E0
  0000000140B68541: 4C 8B 9C 24 F0 00  mov         r11,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68549: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B68550: 41 89 43 18        mov         dword ptr [r11+18h],eax
  0000000140B68554: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6855C: 8B 40 18           mov         eax,dword ptr [rax+18h]
  0000000140B6855F: 89 84 24 24 01 00  mov         dword ptr [rsp+0000000000000124h],eax
                    00
  0000000140B68566: 83 BC 24 24 01 00  cmp         dword ptr [rsp+0000000000000124h],8
                    00 08
  0000000140B6856E: 0F 87 56 02 00 00  ja          0000000140B687CA
  0000000140B68574: 48 63 84 24 24 01  movsxd      rax,dword ptr [rsp+0000000000000124h]
                    00 00
  0000000140B6857C: 48 8D 0D 7D 7A 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B68583: 8B 84 81 3C 90 B6  mov         eax,dword ptr [rcx+rax*4+0000000000B6903Ch]
                    00
  0000000140B6858A: 48 03 C1           add         rax,rcx
  0000000140B6858D: FF E0              jmp         rax
  0000000140B6858F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68597: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B6859E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B685A6: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B685AF: F2 0F 11 40 28     movsd       mmword ptr [rax+28h],xmm0
  0000000140B685B4: E9 11 02 00 00     jmp         0000000140B687CA
  0000000140B685B9: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B685C1: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B685C8: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B685D0: C7 40 30 01 00 00  mov         dword ptr [rax+30h],1
                    00
  0000000140B685D7: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B685E0: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B685E8: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B685EB: E9 DA 01 00 00     jmp         0000000140B687CA
  0000000140B685F0: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B685F8: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B685FF: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68607: C7 40 30 04 00 00  mov         dword ptr [rax+30h],4
                    00
  0000000140B6860E: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68617: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6861F: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B68622: E9 A3 01 00 00     jmp         0000000140B687CA
  0000000140B68627: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6862F: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B68636: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6863E: C7 40 30 03 00 00  mov         dword ptr [rax+30h],3
                    00
  0000000140B68645: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6864E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68656: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B68659: E9 6C 01 00 00     jmp         0000000140B687CA
  0000000140B6865E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68666: C7 40 24 01 00 00  mov         dword ptr [rax+24h],1
                    00
  0000000140B6866D: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68675: C7 40 30 15 00 00  mov         dword ptr [rax+30h],15h
                    00
  0000000140B6867C: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68685: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6868D: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B68690: E9 35 01 00 00     jmp         0000000140B687CA
  0000000140B68695: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6869D: C7 40 24 03 00 00  mov         dword ptr [rax+24h],3
                    00
  0000000140B686A4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B686AC: C7 40 30 04 00 00  mov         dword ptr [rax+30h],4
                    00
  0000000140B686B3: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B686BB: C7 40 34 03 00 00  mov         dword ptr [rax+34h],3
                    00
  0000000140B686C2: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B686CA: C7 40 38 15 00 00  mov         dword ptr [rax+38h],15h
                    00
  0000000140B686D1: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B686DA: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B686E2: 89 48 3C           mov         dword ptr [rax+3Ch],ecx
  0000000140B686E5: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B686EB: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B686F3: 89 48 40           mov         dword ptr [rax+40h],ecx
  0000000140B686F6: F2 0F 2C 4C 24 48  cvttsd2si   ecx,mmword ptr [rsp+48h]
  0000000140B686FC: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68704: 89 48 44           mov         dword ptr [rax+44h],ecx
  0000000140B68707: E9 BE 00 00 00     jmp         0000000140B687CA
  0000000140B6870C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68714: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B6871B: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68723: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6872C: F2 0F 11 80 10 01  movsd       mmword ptr [rax+0000000000000110h],xmm0
                    00 00
  0000000140B68734: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6873C: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68742: F2 0F 11 80 18 01  movsd       mmword ptr [rax+0000000000000118h],xmm0
                    00 00
  0000000140B6874A: EB 7E              jmp         0000000140B687CA
  0000000140B6874C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68754: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B6875B: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68763: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6876C: F2 0F 11 80 20 01  movsd       mmword ptr [rax+0000000000000120h],xmm0
                    00 00
  0000000140B68774: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B6877C: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68782: F2 0F 11 80 28 01  movsd       mmword ptr [rax+0000000000000128h],xmm0
                    00 00
  0000000140B6878A: EB 3E              jmp         0000000140B687CA
  0000000140B6878C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68794: C7 40 24 00 00 00  mov         dword ptr [rax+24h],0
                    00
  0000000140B6879B: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B687A3: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B687AC: F2 0F 11 80 30 01  movsd       mmword ptr [rax+0000000000000130h],xmm0
                    00 00
  0000000140B687B4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B687BC: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B687C2: F2 0F 11 80 38 01  movsd       mmword ptr [rax+0000000000000138h],xmm0
                    00 00
  0000000140B687CA: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B687D2: 83 78 24 00        cmp         dword ptr [rax+24h],0
  0000000140B687D6: 74 23              je          0000000140B687FB
  0000000140B687D8: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B687E0: 48 83 C2 20        add         rdx,20h
  0000000140B687E4: 45 33 C0           xor         r8d,r8d
  0000000140B687E7: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B687EF: E8 FC EB F8 FF     call        0000000140AF73F0
  0000000140B687F4: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B687FB: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140B68800: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B68805: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  0000000140B6880A: 4C 8D 84 24 E0 00  lea         r8,[rsp+00000000000000E0h]
                    00 00
  0000000140B68812: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140B6881A: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68822: E8 49 9C F8 FF     call        0000000140AF2470
  0000000140B68827: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B6882E: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B68836: 74 0A              je          0000000140B68842
  0000000140B68838: B8 02 00 00 00     mov         eax,2
  0000000140B6883D: E9 38 07 00 00     jmp         0000000140B68F7A
  0000000140B68842: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140B6884A: 7D 50              jge         0000000140B6889C
  0000000140B6884C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68854: C6 40 0D 01        mov         byte ptr [rax+0Dh],1
  0000000140B68858: 8B 8C 24 90 00 00  mov         ecx,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B6885F: F7 D9              neg         ecx
  0000000140B68861: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68869: 89 48 14           mov         dword ptr [rax+14h],ecx
  0000000140B6886C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68874: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],0
                    00 00 00 00
  0000000140B6887E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68886: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6888F: F2 0F 11 80 A0 00  movsd       mmword ptr [rax+00000000000000A0h],xmm0
                    00 00
  0000000140B68897: E9 30 03 00 00     jmp         0000000140B68BCC
  0000000140B6889C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B688A4: C6 40 0D 00        mov         byte ptr [rax+0Dh],0
  0000000140B688A8: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B688B0: 48 81 C1 98 00 00  add         rcx,98h
                    00
  0000000140B688B7: BA 15 00 00 00     mov         edx,15h
  0000000140B688BC: E8 1F 31 D3 FF     call        000000014089B9E0
  0000000140B688C1: 4C 8B 9C 24 F0 00  mov         r11,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B688C9: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  0000000140B688D0: 41 89 43 1C        mov         dword ptr [r11+1Ch],eax
  0000000140B688D4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B688DC: 8B 40 1C           mov         eax,dword ptr [rax+1Ch]
  0000000140B688DF: 89 84 24 28 01 00  mov         dword ptr [rsp+0000000000000128h],eax
                    00
  0000000140B688E6: 83 BC 24 28 01 00  cmp         dword ptr [rsp+0000000000000128h],8
                    00 08
  0000000140B688EE: 0F 87 A1 02 00 00  ja          0000000140B68B95
  0000000140B688F4: 48 63 84 24 28 01  movsxd      rax,dword ptr [rsp+0000000000000128h]
                    00 00
  0000000140B688FC: 48 8D 0D FD 76 49  lea         rcx,[0000000140000000h]
                    FF
  0000000140B68903: 8B 84 81 60 90 B6  mov         eax,dword ptr [rcx+rax*4+0000000000B69060h]
                    00
  0000000140B6890A: 48 03 C1           add         rax,rcx
  0000000140B6890D: FF E0              jmp         rax
  0000000140B6890F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68917: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],0
                    00 00 00 00
  0000000140B68921: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68929: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68932: F2 0F 11 80 A0 00  movsd       mmword ptr [rax+00000000000000A0h],xmm0
                    00 00
  0000000140B6893A: E9 56 02 00 00     jmp         0000000140B68B95
  0000000140B6893F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68947: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],1
                    01 00 00 00
  0000000140B68951: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68959: C7 80 A8 00 00 00  mov         dword ptr [rax+00000000000000A8h],1
                    01 00 00 00
  0000000140B68963: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B6896C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68974: 89 88 B4 00 00 00  mov         dword ptr [rax+00000000000000B4h],ecx
  0000000140B6897A: E9 16 02 00 00     jmp         0000000140B68B95
  0000000140B6897F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68987: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],1
                    01 00 00 00
  0000000140B68991: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68999: C7 80 A8 00 00 00  mov         dword ptr [rax+00000000000000A8h],4
                    04 00 00 00
  0000000140B689A3: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B689AC: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B689B4: 89 88 B4 00 00 00  mov         dword ptr [rax+00000000000000B4h],ecx
  0000000140B689BA: E9 D6 01 00 00     jmp         0000000140B68B95
  0000000140B689BF: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B689C7: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],1
                    01 00 00 00
  0000000140B689D1: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B689D9: C7 80 A8 00 00 00  mov         dword ptr [rax+00000000000000A8h],3
                    03 00 00 00
  0000000140B689E3: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B689EC: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B689F4: 89 88 B4 00 00 00  mov         dword ptr [rax+00000000000000B4h],ecx
  0000000140B689FA: E9 96 01 00 00     jmp         0000000140B68B95
  0000000140B689FF: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A07: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],1
                    01 00 00 00
  0000000140B68A11: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A19: C7 80 A8 00 00 00  mov         dword ptr [rax+00000000000000A8h],15h
                    15 00 00 00
  0000000140B68A23: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68A2C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A34: 89 88 B4 00 00 00  mov         dword ptr [rax+00000000000000B4h],ecx
  0000000140B68A3A: E9 56 01 00 00     jmp         0000000140B68B95
  0000000140B68A3F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A47: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],3
                    03 00 00 00
  0000000140B68A51: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A59: C7 80 A8 00 00 00  mov         dword ptr [rax+00000000000000A8h],4
                    04 00 00 00
  0000000140B68A63: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A6B: C7 80 AC 00 00 00  mov         dword ptr [rax+00000000000000ACh],3
                    03 00 00 00
  0000000140B68A75: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A7D: C7 80 B0 00 00 00  mov         dword ptr [rax+00000000000000B0h],15h
                    15 00 00 00
  0000000140B68A87: F2 0F 2C 8C 24 E0  cvttsd2si   ecx,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68A90: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68A98: 89 88 B4 00 00 00  mov         dword ptr [rax+00000000000000B4h],ecx
  0000000140B68A9E: F2 0F 2C 4C 24 40  cvttsd2si   ecx,mmword ptr [rsp+40h]
  0000000140B68AA4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68AAC: 89 88 B8 00 00 00  mov         dword ptr [rax+00000000000000B8h],ecx
  0000000140B68AB2: F2 0F 2C 4C 24 48  cvttsd2si   ecx,mmword ptr [rsp+48h]
  0000000140B68AB8: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68AC0: 89 88 BC 00 00 00  mov         dword ptr [rax+00000000000000BCh],ecx
  0000000140B68AC6: E9 CA 00 00 00     jmp         0000000140B68B95
  0000000140B68ACB: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68AD3: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],0
                    00 00 00 00
  0000000140B68ADD: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68AE5: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68AEE: F2 0F 11 80 40 01  movsd       mmword ptr [rax+0000000000000140h],xmm0
                    00 00
  0000000140B68AF6: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68AFE: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68B04: F2 0F 11 80 48 01  movsd       mmword ptr [rax+0000000000000148h],xmm0
                    00 00
  0000000140B68B0C: E9 84 00 00 00     jmp         0000000140B68B95
  0000000140B68B11: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B19: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],0
                    00 00 00 00
  0000000140B68B23: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B2B: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68B34: F2 0F 11 80 50 01  movsd       mmword ptr [rax+0000000000000150h],xmm0
                    00 00
  0000000140B68B3C: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B44: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68B4A: F2 0F 11 80 58 01  movsd       mmword ptr [rax+0000000000000158h],xmm0
                    00 00
  0000000140B68B52: EB 41              jmp         0000000140B68B95
  0000000140B68B54: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B5C: C7 80 9C 00 00 00  mov         dword ptr [rax+000000000000009Ch],0
                    00 00 00 00
  0000000140B68B66: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B6E: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  0000000140B68B77: F2 0F 11 80 60 01  movsd       mmword ptr [rax+0000000000000160h],xmm0
                    00 00
  0000000140B68B7F: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B87: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68B8D: F2 0F 11 80 68 01  movsd       mmword ptr [rax+0000000000000168h],xmm0
                    00 00
  0000000140B68B95: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68B9D: 83 B8 9C 00 00 00  cmp         dword ptr [rax+000000000000009Ch],0
                    00
  0000000140B68BA4: 74 26              je          0000000140B68BCC
  0000000140B68BA6: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140B68BAE: 48 81 C2 98 00 00  add         rdx,98h
                    00
  0000000140B68BB5: 45 33 C0           xor         r8d,r8d
  0000000140B68BB8: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68BC0: E8 2B E8 F8 FF     call        0000000140AF73F0
  0000000140B68BC5: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68BCC: E9 9E 02 00 00     jmp         0000000140B68E6F
  0000000140B68BD1: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  0000000140B68BD6: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  0000000140B68BDE: 48 8D 94 24 D4 00  lea         rdx,[rsp+00000000000000D4h]
                    00 00
  0000000140B68BE6: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68BEE: E8 FD 9E F8 FF     call        0000000140AF2AF0
  0000000140B68BF3: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68BFA: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B68C02: 75 14              jne         0000000140B68C18
  0000000140B68C04: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68C0C: C7 80 30 03 00 00  mov         dword ptr [rax+0000000000000330h],0
                    00 00 00 00
  0000000140B68C16: EB 3D              jmp         0000000140B68C55
  0000000140B68C18: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68C20: C7 80 30 03 00 00  mov         dword ptr [rax+0000000000000330h],1
                    01 00 00 00
  0000000140B68C2A: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68C32: 8B 84 24 A0 00 00  mov         eax,dword ptr [rsp+00000000000000A0h]
                    00
  0000000140B68C39: 89 81 34 03 00 00  mov         dword ptr [rcx+0000000000000334h],eax
  0000000140B68C3F: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68C47: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68C4D: F2 0F 11 80 38 03  movsd       mmword ptr [rax+0000000000000338h],xmm0
                    00 00
  0000000140B68C55: E9 15 02 00 00     jmp         0000000140B68E6F
  0000000140B68C5A: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  0000000140B68C62: 48 8D 94 24 D4 00  lea         rdx,[rsp+00000000000000D4h]
                    00 00
  0000000140B68C6A: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68C72: E8 59 7E F8 FF     call        0000000140AF0AD0
  0000000140B68C77: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68C7E: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B68C86: 74 0A              je          0000000140B68C92
  0000000140B68C88: B8 02 00 00 00     mov         eax,2
  0000000140B68C8D: E9 E8 02 00 00     jmp         0000000140B68F7A
  0000000140B68C92: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B68C9A: 75 17              jne         0000000140B68CB3
  0000000140B68C9C: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68CA4: C7 80 40 03 00 00  mov         dword ptr [rax+0000000000000340h],0
                    00 00 00 00
  0000000140B68CAE: E9 F8 00 00 00     jmp         0000000140B68DAB
  0000000140B68CB3: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],1
                    00 01
  0000000140B68CBB: 75 6B              jne         0000000140B68D28
  0000000140B68CBD: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68CC5: C7 80 40 03 00 00  mov         dword ptr [rax+0000000000000340h],1
                    01 00 00 00
  0000000140B68CCF: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68CD7: 48 05 60 03 00 00  add         rax,360h
  0000000140B68CDD: 4C 8B 8C 24 C8 00  mov         r9,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68CE5: 49 81 C1 58 03 00  add         r9,358h
                    00
  0000000140B68CEC: 4C 8B 84 24 C8 00  mov         r8,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68CF4: 49 81 C0 50 03 00  add         r8,350h
                    00
  0000000140B68CFB: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68D03: 48 81 C2 48 03 00  add         rdx,348h
                    00
  0000000140B68D0A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B68D0F: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68D17: E8 14 8D F8 FF     call        0000000140AF1A30
  0000000140B68D1C: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68D23: E9 83 00 00 00     jmp         0000000140B68DAB
  0000000140B68D28: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],2
                    00 02
  0000000140B68D30: 75 46              jne         0000000140B68D78
  0000000140B68D32: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68D3A: C7 80 40 03 00 00  mov         dword ptr [rax+0000000000000340h],2
                    02 00 00 00
  0000000140B68D44: 4C 8B 84 24 C8 00  mov         r8,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68D4C: 49 81 C0 70 03 00  add         r8,370h
                    00
  0000000140B68D53: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68D5B: 48 81 C2 68 03 00  add         rdx,368h
                    00
  0000000140B68D62: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68D6A: E8 C1 88 F8 FF     call        0000000140AF1630
  0000000140B68D6F: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68D76: EB 33              jmp         0000000140B68DAB
  0000000140B68D78: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B68D80: 7D 29              jge         0000000140B68DAB
  0000000140B68D82: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68D8A: C7 80 40 03 00 00  mov         dword ptr [rax+0000000000000340h],3
                    03 00 00 00
  0000000140B68D94: 8B 8C 24 D4 00 00  mov         ecx,dword ptr [rsp+00000000000000D4h]
                    00
  0000000140B68D9B: F7 D9              neg         ecx
  0000000140B68D9D: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68DA5: 89 88 78 03 00 00  mov         dword ptr [rax+0000000000000378h],ecx
  0000000140B68DAB: E9 BF 00 00 00     jmp         0000000140B68E6F
  0000000140B68DB0: 4C 8D 8C 24 A4 00  lea         r9,[rsp+00000000000000A4h]
                    00 00
  0000000140B68DB8: 48 8D 44 24 40     lea         rax,[rsp+40h]
  0000000140B68DBD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140B68DC2: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  0000000140B68DCA: 48 8D 94 24 D4 00  lea         rdx,[rsp+00000000000000D4h]
                    00 00
  0000000140B68DD2: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140B68DDA: E8 F1 A2 F8 FF     call        0000000140AF30D0
  0000000140B68DDF: 89 84 24 D8 00 00  mov         dword ptr [rsp+00000000000000D8h],eax
                    00
  0000000140B68DE6: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B68DEE: 74 0A              je          0000000140B68DFA
  0000000140B68DF0: B8 02 00 00 00     mov         eax,2
  0000000140B68DF5: E9 80 01 00 00     jmp         0000000140B68F7A
  0000000140B68DFA: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],0
                    00 00
  0000000140B68E02: 7C 5F              jl          0000000140B68E63
  0000000140B68E04: 83 BC 24 D4 00 00  cmp         dword ptr [rsp+00000000000000D4h],4
                    00 04
  0000000140B68E0C: 7F 55              jg          0000000140B68E63
  0000000140B68E0E: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68E16: 8B 84 24 D4 00 00  mov         eax,dword ptr [rsp+00000000000000D4h]
                    00
  0000000140B68E1D: 89 81 7C 03 00 00  mov         dword ptr [rcx+000000000000037Ch],eax
  0000000140B68E23: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68E2B: 8B 84 24 A0 00 00  mov         eax,dword ptr [rsp+00000000000000A0h]
                    00
  0000000140B68E32: 89 81 80 03 00 00  mov         dword ptr [rcx+0000000000000380h],eax
  0000000140B68E38: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68E40: 8B 84 24 A4 00 00  mov         eax,dword ptr [rsp+00000000000000A4h]
                    00
  0000000140B68E47: 89 81 84 03 00 00  mov         dword ptr [rcx+0000000000000384h],eax
  0000000140B68E4D: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68E55: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000140B68E5B: F2 0F 11 80 88 03  movsd       mmword ptr [rax+0000000000000388h],xmm0
                    00 00
  0000000140B68E63: EB 0A              jmp         0000000140B68E6F
  0000000140B68E65: B8 02 00 00 00     mov         eax,2
  0000000140B68E6A: E9 0B 01 00 00     jmp         0000000140B68F7A
  0000000140B68E6F: 8B 84 24 94 00 00  mov         eax,dword ptr [rsp+0000000000000094h]
                    00
  0000000140B68E76: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  0000000140B68E7A: 0F 84 E7 00 00 00  je          0000000140B68F67
  0000000140B68E80: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B68E88: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B68E8F: 44 8B 44 24 30     mov         r8d,dword ptr [rsp+30h]
  0000000140B68E94: 8B 94 24 94 00 00  mov         edx,dword ptr [rsp+0000000000000094h]
                    00
  0000000140B68E9B: E8 30 DD D7 FF     call        00000001408E6BD0
  0000000140B68EA0: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140B68EA8: 48 83 BC 24 08 01  cmp         qword ptr [rsp+0000000000000108h],0
                    00 00 00
  0000000140B68EB1: 75 43              jne         0000000140B68EF6
  0000000140B68EB3: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140B68EBB: 48 81 C1 C8 13 00  add         rcx,13C8h
                    00
  0000000140B68EC2: E8 B9 DA D7 FF     call        00000001408E6980
  0000000140B68EC7: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140B68ECF: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140B68ED7: 8B 84 24 94 00 00  mov         eax,dword ptr [rsp+0000000000000094h]
                    00
  0000000140B68EDE: 89 81 68 01 00 00  mov         dword ptr [rcx+0000000000000168h],eax
  0000000140B68EE4: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140B68EEC: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140B68EF0: 89 81 6C 01 00 00  mov         dword ptr [rcx+000000000000016Ch],eax
  0000000140B68EF6: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68EFE: 83 B8 70 01 00 00  cmp         dword ptr [rax+0000000000000170h],1
                    01
  0000000140B68F05: 75 14              jne         0000000140B68F1B
  0000000140B68F07: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140B68F0F: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],2
                    02 00 00 00
  0000000140B68F19: EB 37              jmp         0000000140B68F52
  0000000140B68F1B: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68F23: 83 B8 70 01 00 00  cmp         dword ptr [rax+0000000000000170h],2
                    02
  0000000140B68F2A: 75 14              jne         0000000140B68F40
  0000000140B68F2C: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140B68F34: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],1
                    01 00 00 00
  0000000140B68F3E: EB 12              jmp         0000000140B68F52
  0000000140B68F40: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140B68F48: C7 80 70 01 00 00  mov         dword ptr [rax+0000000000000170h],0
                    00 00 00 00
  0000000140B68F52: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140B68F5A: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140B68F62: E8 F9 CD D7 FF     call        00000001408E5D60
  0000000140B68F67: 83 BC 24 D8 00 00  cmp         dword ptr [rsp+00000000000000D8h],0
                    00 00
  0000000140B68F6F: 74 07              je          0000000140B68F78
  0000000140B68F71: B8 02 00 00 00     mov         eax,2
  0000000140B68F76: EB 02              jmp         0000000140B68F7A
  0000000140B68F78: 33 C0              xor         eax,eax
  0000000140B68F7A: 48 81 C4 38 01 00  add         rsp,138h
                    00
  0000000140B68F81: C3                 ret
  0000000140B68F82: 66 90              nop
  0000000140B68F84: 82
  0000000140B68F85: 6B B6 00 43 78 B6  imul        esi,dword ptr [rsi+FFFFFFFFB6784300h],0
                    00
  0000000140B68F8C: 46 7B B6           jnp         0000000140B68F45
  0000000140B68F8F: 00 04 7A           add         byte ptr [rdx+rdi*2],al
  0000000140B68F92: B6 00              mov         dh,0
  0000000140B68F94: 01 77 B6           add         dword ptr [rdi-4Ah],esi
  0000000140B68F97: 00 F4              add         ah,dh
  0000000140B68F99: 7B B6              jnp         0000000140B68F51
  0000000140B68F9B: 00 30              add         byte ptr [rax],dh
  0000000140B68F9D: 7C B6              jl          0000000140B68F55
  0000000140B68F9F: 00 D1              add         cl,dl
  0000000140B68FA1: 8B B6 00 5A 8C B6  mov         esi,dword ptr [rsi+FFFFFFFFB68C5A00h]
  0000000140B68FA7: 00 B0 8D B6 00 2B  add         byte ptr [rax+000000002B00B68Dh],dh
  0000000140B68FAD: 6D                 ins         dword ptr [rdi],dx
  0000000140B68FAE: B6 00              mov         dh,0
  0000000140B68FB0: 55                 push        rbp
  0000000140B68FB1: 6D                 ins         dword ptr [rdi],dx
  0000000140B68FB2: B6 00              mov         dh,0
  0000000140B68FB4: 8C 6D B6           mov         word ptr [rbp-4Ah],gs
  0000000140B68FB7: 00 C3              add         bl,al
  0000000140B68FB9: 6D                 ins         dword ptr [rdi],dx
  0000000140B68FBA: B6 00              mov         dh,0
  0000000140B68FBC: FA                 cli
  0000000140B68FBD: 6D                 ins         dword ptr [rdi],dx
  0000000140B68FBE: B6 00              mov         dh,0
  0000000140B68FC0: 31 6E B6           xor         dword ptr [rsi-4Ah],ebp
  0000000140B68FC3: 00 A8 6E B6 00 E8  add         byte ptr [rax+FFFFFFFFE800B66Eh],ch
  0000000140B68FC9: 6E                 outs        dx,byte ptr [rsi]
  0000000140B68FCA: B6 00              mov         dh,0
  0000000140B68FCC: 28 6F B6           sub         byte ptr [rdi-4Ah],ch
  0000000140B68FCF: 00 A7 70 B6 00 D1  add         byte ptr [rdi+FFFFFFFFD100B670h],ah
  0000000140B68FD5: 70 B6              jo          0000000140B68F8D
  0000000140B68FD7: 00 08              add         byte ptr [rax],cl
  0000000140B68FD9: 71 B6              jno         0000000140B68F91
  0000000140B68FDB: 00 3F              add         byte ptr [rdi],bh
  0000000140B68FDD: 71 B6              jno         0000000140B68F95
  0000000140B68FDF: 00 76 71           add         byte ptr [rsi+71h],dh
  0000000140B68FE2: B6 00              mov         dh,0
  0000000140B68FE4: AD                 lods        dword ptr [rsi]
  0000000140B68FE5: 71 B6              jno         0000000140B68F9D
  0000000140B68FE7: 00 24 72           add         byte ptr [rdx+rsi*2],ah
  0000000140B68FEA: B6 00              mov         dh,0
  0000000140B68FEC: 64 72 B6           jb          0000000140B68FA5
  0000000140B68FEF: 00 A4 72 B6 00 27  add         byte ptr [rdx+rsi*2+00000000742700B6h],ah
                    74
  0000000140B68FF6: B6 00              mov         dh,0
  0000000140B68FF8: 57                 push        rdi
  0000000140B68FF9: 74 B6              je          0000000140B68FB1
  0000000140B68FFB: 00 97 74 B6 00 D7  add         byte ptr [rdi+FFFFFFFFD700B674h],dl
  0000000140B69001: 74 B6              je          0000000140B68FB9
  0000000140B69003: 00 17              add         byte ptr [rdi],dl
  0000000140B69005: 75 B6              jne         0000000140B68FBD
  0000000140B69007: 00 57 75           add         byte ptr [rdi+75h],dl
  0000000140B6900A: B6 00              mov         dh,0
  0000000140B6900C: E3 75              jrcxz       0000000140B69083
  0000000140B6900E: B6 00              mov         dh,0
  0000000140B69010: 29 76 B6           sub         dword ptr [rsi-4Ah],esi
  0000000140B69013: 00 6C 76 B6        add         byte ptr [rsi+rsi*2-4Ah],ch
  0000000140B69017: 00 99 81 B6 00 C0  add         byte ptr [rcx+FFFFFFFFC000B681h],bl
  0000000140B6901D: 81 B6 00 F4 81 B6  xor         dword ptr [rsi+FFFFFFFFB681F400h],0B6822800h
                    00 28 82 B6
  0000000140B69027: 00 5C 82 B6        add         byte ptr [rdx+rax*4-4Ah],bl
  0000000140B6902B: 00 90 82 B6 00 3C  add         byte ptr [rax+000000003C00B682h],dl
  0000000140B69031: 83 B6 00 9A 83 B6  xor         dword ptr [rsi+FFFFFFFFB6839A00h],0
                    00
  0000000140B69038: F5                 cmc
  0000000140B69039: 83 B6 00 8F 85 B6  xor         dword ptr [rsi+FFFFFFFFB6858F00h],0
                    00
  0000000140B69040: B9 85 B6 00 F0     mov         ecx,0F000B685h
  0000000140B69045: 85 B6 00 27 86 B6  test        dword ptr [rsi+FFFFFFFFB6862700h],esi
  0000000140B6904B: 00 5E 86           add         byte ptr [rsi-7Ah],bl
  0000000140B6904E: B6 00              mov         dh,0
  0000000140B69050: 95                 xchg        eax,ebp
  0000000140B69051: 86 B6 00 0C 87 B6  xchg        dh,byte ptr [rsi+FFFFFFFFB6870C00h]
  0000000140B69057: 00 4C 87 B6        add         byte ptr [rdi+rax*4-4Ah],cl
  0000000140B6905B: 00 8C 87 B6 00 0F  add         byte ptr [rdi+rax*4+FFFFFFFF890F00B6h],cl
                    89
  0000000140B69062: B6 00              mov         dh,0
  0000000140B69064: 3F
  0000000140B69065: 89 B6 00 7F 89 B6  mov         dword ptr [rsi+FFFFFFFFB6897F00h],esi
  0000000140B6906B: 00 BF 89 B6 00 FF  add         byte ptr [rdi+FFFFFFFFFF00B689h],bh
  0000000140B69071: 89 B6 00 3F 8A B6  mov         dword ptr [rsi+FFFFFFFFB68A3F00h],esi
  0000000140B69077: 00 CB              add         bl,cl
  0000000140B69079: 8A B6 00 11 8B B6  mov         dh,byte ptr [rsi+FFFFFFFFB68B1100h]
  0000000140B6907F: 00 54 8B B6        add         byte ptr [rbx+rcx*4-4Ah],dl
  0000000140B69083: 00

  Summary

     1E02000 .text
