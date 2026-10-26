Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E2870: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E2875: 53                 push        rbx
  00000001411E2876: 48 81 EC A0 01 00  sub         rsp,1A0h
                    00
  00000001411E287D: 48 C7 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001411E2889: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001411E2891: E8 5A E2 F4 FE     call        0000000140130AF0
  00000001411E2896: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411E289B: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001411E28A1: 75 05              jne         00000001411E28A8
  00000001411E28A3: E9 1C 05 00 00     jmp         00000001411E2DC4
  00000001411E28A8: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E28AD: E8 7E E9 E6 FE     call        0000000140051230
  00000001411E28B2: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001411E28B7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001411E28BF: 4C 8D 0D 3A 3C 89  lea         r9,[0000000143A76500h]
                    02
  00000001411E28C6: 4C 8D 05 93 72 39  lea         r8,[0000000143579B60h]
                    02
  00000001411E28CD: 33 D2              xor         edx,edx
  00000001411E28CF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411E28D4: E8 23 D0 28 00     call        000000014146F8FC
  00000001411E28D9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001411E28DE: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001411E28E4: 75 05              jne         00000001411E28EB
  00000001411E28E6: E9 D9 04 00 00     jmp         00000001411E2DC4
  00000001411E28EB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411E28F0: E8 1B 27 E4 FE     call        0000000140025010
  00000001411E28F5: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411E28FA: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  00000001411E2900: 0F 84 BE 04 00 00  je          00000001411E2DC4
  00000001411E2906: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001411E290E: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  00000001411E2912: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001411E2917: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001411E291B: FF 15 87 8B C2 00  call        qword ptr [0000000141E0B4A8h]
  00000001411E2921: 90                 nop
  00000001411E2922: 48 8D 15 FF 70 AF  lea         rdx,[0000000142CD9A28h]
                    01
  00000001411E2929: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E292E: FF 15 34 1B C2 00  call        qword ptr [0000000141E04468h]
  00000001411E2934: 0F B6 C0           movzx       eax,al
  00000001411E2937: 85 C0              test        eax,eax
  00000001411E2939: 0F 84 91 00 00 00  je          00000001411E29D0
  00000001411E293F: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E2944: E8 E7 DB E7 FE     call        0000000140060530
  00000001411E2949: 83 C0 01           add         eax,1
  00000001411E294C: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001411E2950: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E2955: E8 46 52 E7 FE     call        0000000140057BA0
  00000001411E295A: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001411E295F: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001411E2964: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000001411E2967: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411E296C: E8 5F B0 21 00     call        00000001413FD9D0
  00000001411E2971: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001411E2976: 48 81 C1 28 08 00  add         rcx,828h
                    00
  00000001411E297D: 48 8D 15 B4 70 AF  lea         rdx,[0000000142CD9A38h]
                    01
  00000001411E2984: E8 37 D5 28 00     call        000000014146FEC0
  00000001411E2989: 48 8D 44 24 68     lea         rax,[rsp+68h]
  00000001411E298E: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001411E2993: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001411E2998: 48 81 C2 28 08 00  add         rdx,828h
                    00
  00000001411E299F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001411E29A4: FF 15 D6 1A C2 00  call        qword ptr [0000000141E04480h]
  00000001411E29AA: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001411E29B2: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  00000001411E29B6: 83 EA 01           sub         edx,1
  00000001411E29B9: 4C 8B 84 24 38 01  mov         r8,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001411E29C1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E29C6: E8 15 5C E7 FE     call        00000001400585E0
  00000001411E29CB: E9 6B 01 00 00     jmp         00000001411E2B3B
  00000001411E29D0: 48 8D 44 24 78     lea         rax,[rsp+78h]
  00000001411E29D5: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001411E29DD: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  00000001411E29E5: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001411E29ED: 33 D2              xor         edx,edx
  00000001411E29EF: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E29F7: E8 64 20 E2 FE     call        0000000140004A60
  00000001411E29FC: 48 8B D8           mov         rbx,rax
  00000001411E29FF: B2 28              mov         dl,28h
  00000001411E2A01: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411E2A09: FF 15 F9 18 C2 00  call        qword ptr [0000000141E04308h]
  00000001411E2A0F: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  00000001411E2A14: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001411E2A1C: 41 B9 01 00 00 00  mov         r9d,1
  00000001411E2A22: 44 0F B7 00        movzx       r8d,word ptr [rax]
  00000001411E2A26: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  00000001411E2A2E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2A33: FF 15 4F 29 C2 00  call        qword ptr [0000000141E05388h]
  00000001411E2A39: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  00000001411E2A41: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001411E2A49: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001411E2A51: 48 8B 94 24 48 01  mov         rdx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411E2A59: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2A5E: FF 15 0C 1A C2 00  call        qword ptr [0000000141E04470h]
  00000001411E2A64: 90                 nop
  00000001411E2A65: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001411E2A6D: FF 15 6D 2F C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2A73: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  00000001411E2A7B: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001411E2A83: 48 8D 84 24 B0 00  lea         rax,[rsp+00000000000000B0h]
                    00 00
  00000001411E2A8B: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001411E2A93: 33 D2              xor         edx,edx
  00000001411E2A95: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001411E2A9D: E8 BE 1F E2 FE     call        0000000140004A60
  00000001411E2AA2: 48 8B D8           mov         rbx,rax
  00000001411E2AA5: B2 29              mov         dl,29h
  00000001411E2AA7: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E2AAF: FF 15 53 18 C2 00  call        qword ptr [0000000141E04308h]
  00000001411E2AB5: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  00000001411E2ABA: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001411E2AC2: 45 33 C9           xor         r9d,r9d
  00000001411E2AC5: 44 0F B7 00        movzx       r8d,word ptr [rax]
  00000001411E2AC9: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  00000001411E2AD1: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2AD6: FF 15 AC 28 C2 00  call        qword ptr [0000000141E05388h]
  00000001411E2ADC: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  00000001411E2AE4: 48 8B 84 24 50 01  mov         rax,qword ptr [rsp+0000000000000150h]
                    00 00
  00000001411E2AEC: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001411E2AF4: 48 8B 94 24 58 01  mov         rdx,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001411E2AFC: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2B01: FF 15 69 19 C2 00  call        qword ptr [0000000141E04470h]
  00000001411E2B07: 90                 nop
  00000001411E2B08: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001411E2B10: FF 15 CA 2E C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2B16: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001411E2B1C: 33 D2              xor         edx,edx
  00000001411E2B1E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2B23: FF 15 67 15 C2 00  call        qword ptr [0000000141E04090h]
  00000001411E2B29: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001411E2B2D: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  00000001411E2B31: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411E2B36: E8 95 AE 21 00     call        00000001413FD9D0
  00000001411E2B3B: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001411E2B43: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  00000001411E2B47: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  00000001411E2B4F: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001411E2B53: FF 15 4F 89 C2 00  call        qword ptr [0000000141E0B4A8h]
  00000001411E2B59: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001411E2B61: 48 8B 84 24 60 01  mov         rax,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411E2B69: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001411E2B71: 48 8B 94 24 68 01  mov         rdx,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001411E2B79: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2B7E: FF 15 EC 18 C2 00  call        qword ptr [0000000141E04470h]
  00000001411E2B84: 90                 nop
  00000001411E2B85: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001411E2B8D: FF 15 4D 2E C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2B93: 48 8D 15 AE 6E AF  lea         rdx,[0000000142CD9A48h]
                    01
  00000001411E2B9A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2B9F: FF 15 C3 18 C2 00  call        qword ptr [0000000141E04468h]
  00000001411E2BA5: 0F B6 C0           movzx       eax,al
  00000001411E2BA8: 85 C0              test        eax,eax
  00000001411E2BAA: 0F 84 9A 00 00 00  je          00000001411E2C4A
  00000001411E2BB0: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E2BB5: E8 76 D9 E7 FE     call        0000000140060530
  00000001411E2BBA: 83 C0 01           add         eax,1
  00000001411E2BBD: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001411E2BC1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E2BC6: E8 D5 4F E7 FE     call        0000000140057BA0
  00000001411E2BCB: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001411E2BD0: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001411E2BD5: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000001411E2BD8: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411E2BDD: E8 BE EE 18 00     call        0000000141371AA0
  00000001411E2BE2: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001411E2BE7: 48 81 C1 28 08 00  add         rcx,828h
                    00
  00000001411E2BEE: 48 8D 15 3F 50 AF  lea         rdx,[0000000142CD7C34h]
                    01
  00000001411E2BF5: E8 C6 D2 28 00     call        000000014146FEC0
  00000001411E2BFA: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  00000001411E2C02: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  00000001411E2C0A: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001411E2C0F: 48 81 C2 28 08 00  add         rdx,828h
                    00
  00000001411E2C16: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001411E2C1E: FF 15 5C 18 C2 00  call        qword ptr [0000000141E04480h]
  00000001411E2C24: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001411E2C2C: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  00000001411E2C30: 83 EA 01           sub         edx,1
  00000001411E2C33: 4C 8B 84 24 70 01  mov         r8,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001411E2C3B: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E2C40: E8 9B 59 E7 FE     call        00000001400585E0
  00000001411E2C45: E9 6F 01 00 00     jmp         00000001411E2DB9
  00000001411E2C4A: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  00000001411E2C52: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000001411E2C5A: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  00000001411E2C62: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001411E2C6A: 33 D2              xor         edx,edx
  00000001411E2C6C: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001411E2C74: E8 E7 1D E2 FE     call        0000000140004A60
  00000001411E2C79: 48 8B D8           mov         rbx,rax
  00000001411E2C7C: B2 28              mov         dl,28h
  00000001411E2C7E: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001411E2C86: FF 15 7C 16 C2 00  call        qword ptr [0000000141E04308h]
  00000001411E2C8C: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  00000001411E2C91: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001411E2C99: 41 B9 01 00 00 00  mov         r9d,1
  00000001411E2C9F: 44 0F B7 00        movzx       r8d,word ptr [rax]
  00000001411E2CA3: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  00000001411E2CAB: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2CB0: FF 15 D2 26 C2 00  call        qword ptr [0000000141E05388h]
  00000001411E2CB6: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  00000001411E2CBE: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001411E2CC6: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  00000001411E2CCE: 48 8B 94 24 80 01  mov         rdx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001411E2CD6: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2CDB: FF 15 8F 17 C2 00  call        qword ptr [0000000141E04470h]
  00000001411E2CE1: 90                 nop
  00000001411E2CE2: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001411E2CEA: FF 15 F0 2C C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2CF0: 48 8D 84 24 08 01  lea         rax,[rsp+0000000000000108h]
                    00 00
  00000001411E2CF8: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001411E2D00: 48 8D 84 24 18 01  lea         rax,[rsp+0000000000000118h]
                    00 00
  00000001411E2D08: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  00000001411E2D10: 33 D2              xor         edx,edx
  00000001411E2D12: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001411E2D1A: E8 41 1D E2 FE     call        0000000140004A60
  00000001411E2D1F: 48 8B D8           mov         rbx,rax
  00000001411E2D22: B2 29              mov         dl,29h
  00000001411E2D24: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001411E2D2C: FF 15 D6 15 C2 00  call        qword ptr [0000000141E04308h]
  00000001411E2D32: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  00000001411E2D37: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001411E2D3F: 45 33 C9           xor         r9d,r9d
  00000001411E2D42: 44 0F B7 00        movzx       r8d,word ptr [rax]
  00000001411E2D46: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  00000001411E2D4E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2D53: FF 15 2F 26 C2 00  call        qword ptr [0000000141E05388h]
  00000001411E2D59: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  00000001411E2D61: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001411E2D69: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  00000001411E2D71: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001411E2D79: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2D7E: FF 15 EC 16 C2 00  call        qword ptr [0000000141E04470h]
  00000001411E2D84: 90                 nop
  00000001411E2D85: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  00000001411E2D8D: FF 15 4D 2C C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2D93: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001411E2D99: 33 D2              xor         edx,edx
  00000001411E2D9B: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2DA0: FF 15 EA 12 C2 00  call        qword ptr [0000000141E04090h]
  00000001411E2DA6: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001411E2DAA: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  00000001411E2DAE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411E2DB3: E8 E8 EC 18 00     call        0000000141371AA0
  00000001411E2DB8: 90                 nop
  00000001411E2DB9: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001411E2DBE: FF 15 1C 2C C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E2DC4: 48 81 C4 A0 01 00  add         rsp,1A0h
                    00
  00000001411E2DCB: 5B                 pop         rbx
  00000001411E2DCC: C3                 ret

  Summary

     1E02000 .text
