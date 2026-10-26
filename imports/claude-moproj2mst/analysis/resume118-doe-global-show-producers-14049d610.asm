Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014049D610: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014049D615: 56                 push        rsi
  000000014049D616: 57                 push        rdi
  000000014049D617: 48 81 EC 88 02 00  sub         rsp,288h
                    00
  000000014049D61E: 48 C7 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014049D62A: 33 D2              xor         edx,edx
  000000014049D62C: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D634: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014049D638: E8 B3 EB 3C 00     call        000000014086C1F0
  000000014049D63D: 0F B6 C0           movzx       eax,al
  000000014049D640: 85 C0              test        eax,eax
  000000014049D642: 0F 85 BC 00 00 00  jne         000000014049D704
  000000014049D648: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D650: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014049D657: FF 15 6B 69 96 01  call        qword ptr [0000000141E03FC8h]
  000000014049D65D: 0F B6 C0           movzx       eax,al
  000000014049D660: 85 C0              test        eax,eax
  000000014049D662: 0F 85 9C 00 00 00  jne         000000014049D704
  000000014049D668: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014049D66D: E8 9E 96 45 00     call        00000001408F6D10
  000000014049D672: 90                 nop
  000000014049D673: C6 84 24 A0 00 00  mov         byte ptr [rsp+00000000000000A0h],0
                    00 00
  000000014049D67B: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D683: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014049D68A: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  000000014049D692: FF 15 B8 68 96 01  call        qword ptr [0000000141E03F50h]
  000000014049D698: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  000000014049D6A0: 48 8B 84 24 68 02  mov         rax,qword ptr [rsp+0000000000000268h]
                    00 00
  000000014049D6A8: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  000000014049D6B0: 48 8B 8C 24 70 02  mov         rcx,qword ptr [rsp+0000000000000270h]
                    00 00
  000000014049D6B8: FF 15 6A 6B 96 01  call        qword ptr [0000000141E04228h]
  000000014049D6BE: 48 8B D0           mov         rdx,rax
  000000014049D6C1: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D6C9: 48 8B 40 48        mov         rax,qword ptr [rax+48h]
  000000014049D6CD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014049D6D2: 41 B9 02 00 00 00  mov         r9d,2
  000000014049D6D8: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  000000014049D6E0: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014049D6E5: E8 96 98 45 00     call        00000001408F6F80
  000000014049D6EA: 90                 nop
  000000014049D6EB: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014049D6F3: FF 15 97 6B 96 01  call        qword ptr [0000000141E04290h]
  000000014049D6F9: 90                 nop
  000000014049D6FA: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014049D6FF: E8 4C 96 45 00     call        00000001408F6D50
  000000014049D704: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D70C: C6 80 88 00 00 00  mov         byte ptr [rax+0000000000000088h],1
                    01
  000000014049D713: C6 84 24 40 01 00  mov         byte ptr [rsp+0000000000000140h],0
                    00 00
  000000014049D71B: 48 8B 84 24 A0 02  mov         rax,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D723: 48 8B 00           mov         rax,qword ptr [rax]
  000000014049D726: 48 8B 8C 24 A0 02  mov         rcx,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D72E: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  000000014049D734: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  000000014049D73C: 4C 8D 84 24 40 01  lea         r8,[rsp+0000000000000140h]
                    00 00
  000000014049D744: 48 8D 15 7D 87 F4  lea         rdx,[00000001423E5EC8h]
                    01
  000000014049D74B: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  000000014049D753: E8 28 79 B8 FF     call        0000000140025080
  000000014049D758: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  000000014049D760: 48 8B F0           mov         rsi,rax
  000000014049D763: B9 10 00 00 00     mov         ecx,10h
  000000014049D768: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D76A: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  000000014049D772: 48 8D B4 24 50 02  lea         rsi,[rsp+0000000000000250h]
                    00 00
  000000014049D77A: B9 10 00 00 00     mov         ecx,10h
  000000014049D77F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D781: 4C 8D 84 24 68 01  lea         r8,[rsp+0000000000000168h]
                    00 00
  000000014049D789: 48 8D 15 40 87 F4  lea         rdx,[00000001423E5ED0h]
                    01
  000000014049D790: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  000000014049D798: E8 E3 78 B8 FF     call        0000000140025080
  000000014049D79D: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  000000014049D7A5: 48 8B F0           mov         rsi,rax
  000000014049D7A8: B9 10 00 00 00     mov         ecx,10h
  000000014049D7AD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D7AF: 48 8D BC 24 48 01  lea         rdi,[rsp+0000000000000148h]
                    00 00
  000000014049D7B7: 48 8D B4 24 40 02  lea         rsi,[rsp+0000000000000240h]
                    00 00
  000000014049D7BF: B9 10 00 00 00     mov         ecx,10h
  000000014049D7C4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D7C6: 4C 8B 84 24 A0 02  mov         r8,qword ptr [rsp+00000000000002A0h]
                    00 00
  000000014049D7CE: 49 83 C0 48        add         r8,48h
  000000014049D7D2: 48 8D 15 07 87 F4  lea         rdx,[00000001423E5EE0h]
                    01
  000000014049D7D9: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  000000014049D7E1: E8 9A 78 B8 FF     call        0000000140025080
  000000014049D7E6: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  000000014049D7EE: 48 8B F0           mov         rsi,rax
  000000014049D7F1: B9 10 00 00 00     mov         ecx,10h
  000000014049D7F6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D7F8: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  000000014049D800: 48 8D B4 24 30 02  lea         rsi,[rsp+0000000000000230h]
                    00 00
  000000014049D808: B9 10 00 00 00     mov         ecx,10h
  000000014049D80D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D80F: 45 33 C0           xor         r8d,r8d
  000000014049D812: 33 D2              xor         edx,edx
  000000014049D814: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014049D81C: FF 15 46 69 96 01  call        qword ptr [0000000141E04168h]
  000000014049D822: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  000000014049D82A: 48 8B F0           mov         rsi,rax
  000000014049D82D: B9 10 00 00 00     mov         ecx,10h
  000000014049D832: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D834: 45 33 C0           xor         r8d,r8d
  000000014049D837: 33 D2              xor         edx,edx
  000000014049D839: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000014049D841: FF 15 21 69 96 01  call        qword ptr [0000000141E04168h]
  000000014049D847: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  000000014049D84F: 48 8B F0           mov         rsi,rax
  000000014049D852: B9 10 00 00 00     mov         ecx,10h
  000000014049D857: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D859: 45 33 C0           xor         r8d,r8d
  000000014049D85C: 33 D2              xor         edx,edx
  000000014049D85E: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000014049D866: FF 15 FC 68 96 01  call        qword ptr [0000000141E04168h]
  000000014049D86C: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  000000014049D874: 48 8B F0           mov         rsi,rax
  000000014049D877: B9 10 00 00 00     mov         ecx,10h
  000000014049D87C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D87E: 45 33 C0           xor         r8d,r8d
  000000014049D881: 33 D2              xor         edx,edx
  000000014049D883: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  000000014049D88B: FF 15 D7 68 96 01  call        qword ptr [0000000141E04168h]
  000000014049D891: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  000000014049D899: 48 8B F0           mov         rsi,rax
  000000014049D89C: B9 10 00 00 00     mov         ecx,10h
  000000014049D8A1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D8A3: 45 33 C0           xor         r8d,r8d
  000000014049D8A6: 33 D2              xor         edx,edx
  000000014049D8A8: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000014049D8B0: FF 15 B2 68 96 01  call        qword ptr [0000000141E04168h]
  000000014049D8B6: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  000000014049D8BE: 48 8B F0           mov         rsi,rax
  000000014049D8C1: B9 10 00 00 00     mov         ecx,10h
  000000014049D8C6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D8C8: 45 33 C0           xor         r8d,r8d
  000000014049D8CB: 33 D2              xor         edx,edx
  000000014049D8CD: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000014049D8D5: FF 15 8D 68 96 01  call        qword ptr [0000000141E04168h]
  000000014049D8DB: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  000000014049D8E3: 48 8B F0           mov         rsi,rax
  000000014049D8E6: B9 10 00 00 00     mov         ecx,10h
  000000014049D8EB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D8ED: 45 33 C0           xor         r8d,r8d
  000000014049D8F0: 33 D2              xor         edx,edx
  000000014049D8F2: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000014049D8FA: FF 15 68 68 96 01  call        qword ptr [0000000141E04168h]
  000000014049D900: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  000000014049D908: 48 8B F0           mov         rsi,rax
  000000014049D90B: B9 10 00 00 00     mov         ecx,10h
  000000014049D910: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D912: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  000000014049D91A: 48 8D B4 24 20 01  lea         rsi,[rsp+0000000000000120h]
                    00 00
  000000014049D922: B9 10 00 00 00     mov         ecx,10h
  000000014049D927: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D929: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  000000014049D931: 48 8D B4 24 48 01  lea         rsi,[rsp+0000000000000148h]
                    00 00
  000000014049D939: B9 10 00 00 00     mov         ecx,10h
  000000014049D93E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D940: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  000000014049D948: 48 8D B4 24 70 01  lea         rsi,[rsp+0000000000000170h]
                    00 00
  000000014049D950: B9 10 00 00 00     mov         ecx,10h
  000000014049D955: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014049D957: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  000000014049D95F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014049D964: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  000000014049D96C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014049D971: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  000000014049D979: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014049D97E: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  000000014049D986: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014049D98B: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  000000014049D993: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014049D998: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  000000014049D9A0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014049D9A5: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  000000014049D9AD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014049D9B2: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  000000014049D9BA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014049D9BF: 4C 8D 8C 24 A0 01  lea         r9,[rsp+00000000000001A0h]
                    00 00
  000000014049D9C7: 4C 8D 84 24 90 01  lea         r8,[rsp+0000000000000190h]
                    00 00
  000000014049D9CF: 48 8D 15 1A 85 F4  lea         rdx,[00000001423E5EF0h]
                    01
  000000014049D9D6: 48 8D 0D 23 8D 8D  lea         rcx,[0000000143D76700h]
                    03
  000000014049D9DD: E8 DE 19 E3 FF     call        00000001402CF3C0
  000000014049D9E2: 48 81 C4 88 02 00  add         rsp,288h
                    00
  000000014049D9E9: 5F                 pop         rdi
  000000014049D9EA: 5E                 pop         rsi
  000000014049D9EB: C3                 ret

  Summary

     1E02000 .text
