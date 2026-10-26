Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CE6250: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CE6255: 56                 push        rsi
  0000000140CE6256: 57                 push        rdi
  0000000140CE6257: 48 81 EC 58 03 00  sub         rsp,358h
                    00
  0000000140CE625E: 48 C7 84 24 40 03  mov         qword ptr [rsp+0000000000000340h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CE626A: B9 68 00 00 00     mov         ecx,68h
  0000000140CE626F: E8 48 88 78 00     call        000000014146EABC
  0000000140CE6274: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140CE6279: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140CE627F: 74 24              je          0000000140CE62A5
  0000000140CE6281: 48 8B 8C 24 70 03  mov         rcx,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140CE6289: E8 B2 1F 00 00     call        0000000140CE8240
  0000000140CE628E: 48 8B D0           mov         rdx,rax
  0000000140CE6291: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CE6296: E8 35 19 9D 00     call        00000001416B7BD0
  0000000140CE629B: 48 89 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],rax
                    00 00
  0000000140CE62A3: EB 0C              jmp         0000000140CE62B1
  0000000140CE62A5: 48 C7 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],0
                    00 00 00 00 00 00
  0000000140CE62B1: 48 8B 84 24 48 03  mov         rax,qword ptr [rsp+0000000000000348h]
                    00 00
  0000000140CE62B9: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140CE62BE: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140CE62C3: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140CE62C8: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000140CE62CD: 48 8D 15 5C BF D0  lea         rdx,[00000001429F2230h]
                    01
  0000000140CE62D4: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140CE62DC: E8 9F ED 33 FF     call        0000000140025080
  0000000140CE62E1: 48 8D BC 24 30 03  lea         rdi,[rsp+0000000000000330h]
                    00 00
  0000000140CE62E9: 48 8B F0           mov         rsi,rax
  0000000140CE62EC: B9 10 00 00 00     mov         ecx,10h
  0000000140CE62F1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE62F3: 48 8D BC 24 08 01  lea         rdi,[rsp+0000000000000108h]
                    00 00
  0000000140CE62FB: 48 8D B4 24 30 03  lea         rsi,[rsp+0000000000000330h]
                    00 00
  0000000140CE6303: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6308: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE630A: 45 33 C0           xor         r8d,r8d
  0000000140CE630D: 33 D2              xor         edx,edx
  0000000140CE630F: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140CE6314: FF 15 4E DE 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE631A: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  0000000140CE6322: 48 8B F0           mov         rsi,rax
  0000000140CE6325: B9 10 00 00 00     mov         ecx,10h
  0000000140CE632A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE632C: 45 33 C0           xor         r8d,r8d
  0000000140CE632F: 33 D2              xor         edx,edx
  0000000140CE6331: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140CE6339: FF 15 29 DE 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE633F: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  0000000140CE6347: 48 8B F0           mov         rsi,rax
  0000000140CE634A: B9 10 00 00 00     mov         ecx,10h
  0000000140CE634F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6351: 45 33 C0           xor         r8d,r8d
  0000000140CE6354: 33 D2              xor         edx,edx
  0000000140CE6356: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140CE635E: FF 15 04 DE 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6364: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  0000000140CE636C: 48 8B F0           mov         rsi,rax
  0000000140CE636F: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6374: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6376: 45 33 C0           xor         r8d,r8d
  0000000140CE6379: 33 D2              xor         edx,edx
  0000000140CE637B: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140CE6383: FF 15 DF DD 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6389: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  0000000140CE6391: 48 8B F0           mov         rsi,rax
  0000000140CE6394: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6399: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE639B: 45 33 C0           xor         r8d,r8d
  0000000140CE639E: 33 D2              xor         edx,edx
  0000000140CE63A0: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140CE63A8: FF 15 BA DD 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE63AE: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  0000000140CE63B6: 48 8B F0           mov         rsi,rax
  0000000140CE63B9: B9 10 00 00 00     mov         ecx,10h
  0000000140CE63BE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE63C0: 45 33 C0           xor         r8d,r8d
  0000000140CE63C3: 33 D2              xor         edx,edx
  0000000140CE63C5: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140CE63CD: FF 15 95 DD 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE63D3: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  0000000140CE63DB: 48 8B F0           mov         rsi,rax
  0000000140CE63DE: B9 10 00 00 00     mov         ecx,10h
  0000000140CE63E3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE63E5: 45 33 C0           xor         r8d,r8d
  0000000140CE63E8: 33 D2              xor         edx,edx
  0000000140CE63EA: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140CE63F2: FF 15 70 DD 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE63F8: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  0000000140CE6400: 48 8B F0           mov         rsi,rax
  0000000140CE6403: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6408: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE640A: 45 33 C0           xor         r8d,r8d
  0000000140CE640D: 33 D2              xor         edx,edx
  0000000140CE640F: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140CE6417: FF 15 4B DD 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE641D: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  0000000140CE6425: 48 8B F0           mov         rsi,rax
  0000000140CE6428: B9 10 00 00 00     mov         ecx,10h
  0000000140CE642D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE642F: 45 33 C0           xor         r8d,r8d
  0000000140CE6432: 33 D2              xor         edx,edx
  0000000140CE6434: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140CE643C: FF 15 26 DD 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6442: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140CE644A: 48 8B F0           mov         rsi,rax
  0000000140CE644D: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6452: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6454: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140CE645C: 48 8D B4 24 08 01  lea         rsi,[rsp+0000000000000108h]
                    00 00
  0000000140CE6464: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6469: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE646B: 48 8D 84 24 70 02  lea         rax,[rsp+0000000000000270h]
                    00 00
  0000000140CE6473: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE6478: 48 8D 84 24 60 02  lea         rax,[rsp+0000000000000260h]
                    00 00
  0000000140CE6480: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE6485: 48 8D 84 24 50 02  lea         rax,[rsp+0000000000000250h]
                    00 00
  0000000140CE648D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE6492: 48 8D 84 24 40 02  lea         rax,[rsp+0000000000000240h]
                    00 00
  0000000140CE649A: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE649F: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  0000000140CE64A7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE64AC: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  0000000140CE64B4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE64B9: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  0000000140CE64C1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE64C6: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  0000000140CE64CE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE64D3: 4C 8D 8C 24 F0 01  lea         r9,[rsp+00000000000001F0h]
                    00 00
  0000000140CE64DB: 4C 8D 84 24 E0 01  lea         r8,[rsp+00000000000001E0h]
                    00 00
  0000000140CE64E3: 48 8D 15 56 BD D0  lea         rdx,[00000001429F2240h]
                    01
  0000000140CE64EA: 48 8D 0D 0F 02 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE64F1: E8 CA 8E 5E FF     call        00000001402CF3C0
  0000000140CE64F6: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000140CE64FB: 48 8D 15 5E BD D0  lea         rdx,[00000001429F2260h]
                    01
  0000000140CE6502: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  0000000140CE650A: E8 71 EB 33 FF     call        0000000140025080
  0000000140CE650F: 48 8D BC 24 20 03  lea         rdi,[rsp+0000000000000320h]
                    00 00
  0000000140CE6517: 48 8B F0           mov         rsi,rax
  0000000140CE651A: B9 10 00 00 00     mov         ecx,10h
  0000000140CE651F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6521: 48 8D BC 24 B8 01  lea         rdi,[rsp+00000000000001B8h]
                    00 00
  0000000140CE6529: 48 8D B4 24 20 03  lea         rsi,[rsp+0000000000000320h]
                    00 00
  0000000140CE6531: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6536: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6538: 45 33 C0           xor         r8d,r8d
  0000000140CE653B: 33 D2              xor         edx,edx
  0000000140CE653D: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140CE6545: FF 15 1D DC 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE654B: 48 8D BC 24 10 03  lea         rdi,[rsp+0000000000000310h]
                    00 00
  0000000140CE6553: 48 8B F0           mov         rsi,rax
  0000000140CE6556: B9 10 00 00 00     mov         ecx,10h
  0000000140CE655B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE655D: 45 33 C0           xor         r8d,r8d
  0000000140CE6560: 33 D2              xor         edx,edx
  0000000140CE6562: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140CE656A: FF 15 F8 DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6570: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  0000000140CE6578: 48 8B F0           mov         rsi,rax
  0000000140CE657B: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6580: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6582: 45 33 C0           xor         r8d,r8d
  0000000140CE6585: 33 D2              xor         edx,edx
  0000000140CE6587: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140CE658F: FF 15 D3 DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6595: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  0000000140CE659D: 48 8B F0           mov         rsi,rax
  0000000140CE65A0: B9 10 00 00 00     mov         ecx,10h
  0000000140CE65A5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE65A7: 45 33 C0           xor         r8d,r8d
  0000000140CE65AA: 33 D2              xor         edx,edx
  0000000140CE65AC: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140CE65B4: FF 15 AE DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE65BA: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  0000000140CE65C2: 48 8B F0           mov         rsi,rax
  0000000140CE65C5: B9 10 00 00 00     mov         ecx,10h
  0000000140CE65CA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE65CC: 45 33 C0           xor         r8d,r8d
  0000000140CE65CF: 33 D2              xor         edx,edx
  0000000140CE65D1: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140CE65D9: FF 15 89 DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE65DF: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  0000000140CE65E7: 48 8B F0           mov         rsi,rax
  0000000140CE65EA: B9 10 00 00 00     mov         ecx,10h
  0000000140CE65EF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE65F1: 45 33 C0           xor         r8d,r8d
  0000000140CE65F4: 33 D2              xor         edx,edx
  0000000140CE65F6: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140CE65FE: FF 15 64 DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6604: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  0000000140CE660C: 48 8B F0           mov         rsi,rax
  0000000140CE660F: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6614: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6616: 45 33 C0           xor         r8d,r8d
  0000000140CE6619: 33 D2              xor         edx,edx
  0000000140CE661B: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140CE6623: FF 15 3F DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6629: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  0000000140CE6631: 48 8B F0           mov         rsi,rax
  0000000140CE6634: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6639: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE663B: 45 33 C0           xor         r8d,r8d
  0000000140CE663E: 33 D2              xor         edx,edx
  0000000140CE6640: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140CE6648: FF 15 1A DB 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE664E: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  0000000140CE6656: 48 8B F0           mov         rsi,rax
  0000000140CE6659: B9 10 00 00 00     mov         ecx,10h
  0000000140CE665E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6660: 45 33 C0           xor         r8d,r8d
  0000000140CE6663: 33 D2              xor         edx,edx
  0000000140CE6665: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  0000000140CE666D: FF 15 F5 DA 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6673: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  0000000140CE667B: 48 8B F0           mov         rsi,rax
  0000000140CE667E: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6683: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6685: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  0000000140CE668D: 48 8D B4 24 B8 01  lea         rsi,[rsp+00000000000001B8h]
                    00 00
  0000000140CE6695: B9 10 00 00 00     mov         ecx,10h
  0000000140CE669A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE669C: 48 8D 84 24 10 03  lea         rax,[rsp+0000000000000310h]
                    00 00
  0000000140CE66A4: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE66A9: 48 8D 84 24 00 03  lea         rax,[rsp+0000000000000300h]
                    00 00
  0000000140CE66B1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE66B6: 48 8D 84 24 F0 02  lea         rax,[rsp+00000000000002F0h]
                    00 00
  0000000140CE66BE: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE66C3: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  0000000140CE66CB: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE66D0: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  0000000140CE66D8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE66DD: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  0000000140CE66E5: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE66EA: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  0000000140CE66F2: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE66F7: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  0000000140CE66FF: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE6704: 4C 8D 8C 24 90 02  lea         r9,[rsp+0000000000000290h]
                    00 00
  0000000140CE670C: 4C 8D 84 24 80 02  lea         r8,[rsp+0000000000000280h]
                    00 00
  0000000140CE6714: 48 8D 15 55 BB D0  lea         rdx,[00000001429F2270h]
                    01
  0000000140CE671B: 48 8D 0D DE FF 08  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE6722: E8 99 8C 5E FF     call        00000001402CF3C0
  0000000140CE6727: 48 81 C4 58 03 00  add         rsp,358h
                    00
  0000000140CE672E: 5F                 pop         rdi
  0000000140CE672F: 5E                 pop         rsi
  0000000140CE6730: C3                 ret

  Summary

     1E02000 .text
