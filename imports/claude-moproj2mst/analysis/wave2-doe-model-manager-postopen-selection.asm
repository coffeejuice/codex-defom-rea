
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014002A0E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014002A0E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014002A0EA: 56                 push        rsi
  000000014002A0EB: 57                 push        rdi
  000000014002A0EC: 48 81 EC 88 04 00  sub         rsp,488h
                    00
  000000014002A0F3: 48 C7 84 24 70 04  mov         qword ptr [rsp+0000000000000470h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014002A0FF: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A107: C6 80 C0 03 00 00  mov         byte ptr [rax+00000000000003C0h],0
                    00
  000000014002A10E: 45 33 C0           xor         r8d,r8d
  000000014002A111: 33 D2              xor         edx,edx
  000000014002A113: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A11B: E8 20 9C FE FF     call        0000000140013D40
  000000014002A120: 48 83 BC 24 A8 04  cmp         qword ptr [rsp+00000000000004A8h],0
                    00 00 00
  000000014002A129: 74 7B              je          000000014002A1A6
  000000014002A12B: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A133: 48 83 B8 E0 02 00  cmp         qword ptr [rax+00000000000002E0h],0
                    00 00
  000000014002A13B: 74 16              je          000000014002A153
  000000014002A13D: B2 01              mov         dl,1
  000000014002A13F: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A147: 48 8B 89 E0 02 00  mov         rcx,qword ptr [rcx+00000000000002E0h]
                    00
  000000014002A14E: E8 4D CD 0E 00     call        0000000140116EA0
  000000014002A153: C7 44 24 70 00 00  mov         dword ptr [rsp+70h],0
                    00 00
  000000014002A15B: 48 8B 8C 24 A8 04  mov         rcx,qword ptr [rsp+00000000000004A8h]
                    00 00
  000000014002A163: E8 28 B7 07 00     call        00000001400A5890
  000000014002A168: 48 8B F8           mov         rdi,rax
  000000014002A16B: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A173: E8 28 92 FE FF     call        00000001400133A0
  000000014002A178: 48 8B D7           mov         rdx,rdi
  000000014002A17B: 48 8B C8           mov         rcx,rax
  000000014002A17E: E8 5D FD C4 00     call        0000000140C79EE0
  000000014002A183: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  000000014002A187: 41 B1 01           mov         r9b,1
  000000014002A18A: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000014002A18F: 48 8D 54 24 74     lea         rdx,[rsp+74h]
  000000014002A194: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A19C: E8 0F 08 00 00     call        000000014002A9B0
  000000014002A1A1: E9 A8 07 00 00     jmp         000000014002A94E
  000000014002A1A6: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A1AE: 48 83 B8 E0 02 00  cmp         qword ptr [rax+00000000000002E0h],0
                    00 00
  000000014002A1B6: 74 16              je          000000014002A1CE
  000000014002A1B8: 33 D2              xor         edx,edx
  000000014002A1BA: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A1C2: 48 8B 89 E0 02 00  mov         rcx,qword ptr [rcx+00000000000002E0h]
                    00
  000000014002A1C9: E8 D2 CC 0E 00     call        0000000140116EA0
  000000014002A1CE: 48 8D 15 0A 1F 0D  lea         rdx,[00000001420FC0DFh]
                    02
  000000014002A1D5: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014002A1DD: FF 15 2D 9F DD 01  call        qword ptr [0000000141E04110h]
  000000014002A1E3: 48 8B D0           mov         rdx,rax
  000000014002A1E6: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014002A1EB: FF 15 27 9F DD 01  call        qword ptr [0000000141E04118h]
  000000014002A1F1: 90                 nop
  000000014002A1F2: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  000000014002A1F7: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A1FF: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  000000014002A206: FF 15 D4 1A DE 01  call        qword ptr [0000000141E0BCE0h]
  000000014002A20C: 90                 nop
  000000014002A20D: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014002A212: FF 15 C8 B7 DD 01  call        qword ptr [0000000141E059E0h]
  000000014002A218: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A220: E8 EB E8 FE FF     call        0000000140018B10
  000000014002A225: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014002A22A: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A232: 48 83 B8 60 04 00  cmp         qword ptr [rax+0000000000000460h],0
                    00 00
  000000014002A23A: 74 53              je          000000014002A28F
  000000014002A23C: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A244: 48 8B 80 60 04 00  mov         rax,qword ptr [rax+0000000000000460h]
                    00
  000000014002A24B: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A253: 48 8B 89 60 04 00  mov         rcx,qword ptr [rcx+0000000000000460h]
                    00
  000000014002A25A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002A25D: 33 D2              xor         edx,edx
  000000014002A25F: FF 50 58           call        qword ptr [rax+58h]
  000000014002A262: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A26A: 48 8B 89 60 04 00  mov         rcx,qword ptr [rcx+0000000000000460h]
                    00
  000000014002A271: FF 15 79 18 DE 01  call        qword ptr [0000000141E0BAF0h]
  000000014002A277: 48 8B D0           mov         rdx,rax
  000000014002A27A: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A282: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  000000014002A289: FF 15 09 17 DE 01  call        qword ptr [0000000141E0B998h]
  000000014002A28F: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A297: 48 83 B8 70 04 00  cmp         qword ptr [rax+0000000000000470h],0
                    00 00
  000000014002A29F: 74 53              je          000000014002A2F4
  000000014002A2A1: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A2A9: 48 8B 80 70 04 00  mov         rax,qword ptr [rax+0000000000000470h]
                    00
  000000014002A2B0: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A2B8: 48 8B 89 70 04 00  mov         rcx,qword ptr [rcx+0000000000000470h]
                    00
  000000014002A2BF: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002A2C2: 33 D2              xor         edx,edx
  000000014002A2C4: FF 50 58           call        qword ptr [rax+58h]
  000000014002A2C7: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A2CF: 48 8B 89 70 04 00  mov         rcx,qword ptr [rcx+0000000000000470h]
                    00
  000000014002A2D6: FF 15 14 18 DE 01  call        qword ptr [0000000141E0BAF0h]
  000000014002A2DC: 48 8B D0           mov         rdx,rax
  000000014002A2DF: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A2E7: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  000000014002A2EE: FF 15 A4 16 DE 01  call        qword ptr [0000000141E0B998h]
  000000014002A2F4: 4C 8D 84 24 A8 04  lea         r8,[rsp+00000000000004A8h]
                    00 00
  000000014002A2FC: 48 8D 15 45 5E 0D  lea         rdx,[0000000142100148h]
                    02
  000000014002A303: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  000000014002A30B: E8 70 AD FF FF     call        0000000140025080
  000000014002A310: 48 8D BC 24 60 04  lea         rdi,[rsp+0000000000000460h]
                    00 00
  000000014002A318: 48 8B F0           mov         rsi,rax
  000000014002A31B: B9 10 00 00 00     mov         ecx,10h
  000000014002A320: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A322: 48 8D BC 24 18 01  lea         rdi,[rsp+0000000000000118h]
                    00 00
  000000014002A32A: 48 8D B4 24 60 04  lea         rsi,[rsp+0000000000000460h]
                    00 00
  000000014002A332: B9 10 00 00 00     mov         ecx,10h
  000000014002A337: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A339: 45 33 C0           xor         r8d,r8d
  000000014002A33C: 33 D2              xor         edx,edx
  000000014002A33E: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014002A346: FF 15 1C 9E DD 01  call        qword ptr [0000000141E04168h]
  000000014002A34C: 48 8D BC 24 10 03  lea         rdi,[rsp+0000000000000310h]
                    00 00
  000000014002A354: 48 8B F0           mov         rsi,rax
  000000014002A357: B9 10 00 00 00     mov         ecx,10h
  000000014002A35C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A35E: 45 33 C0           xor         r8d,r8d
  000000014002A361: 33 D2              xor         edx,edx
  000000014002A363: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014002A36B: FF 15 F7 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A371: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  000000014002A379: 48 8B F0           mov         rsi,rax
  000000014002A37C: B9 10 00 00 00     mov         ecx,10h
  000000014002A381: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A383: 45 33 C0           xor         r8d,r8d
  000000014002A386: 33 D2              xor         edx,edx
  000000014002A388: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014002A390: FF 15 D2 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A396: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  000000014002A39E: 48 8B F0           mov         rsi,rax
  000000014002A3A1: B9 10 00 00 00     mov         ecx,10h
  000000014002A3A6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A3A8: 45 33 C0           xor         r8d,r8d
  000000014002A3AB: 33 D2              xor         edx,edx
  000000014002A3AD: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014002A3B5: FF 15 AD 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A3BB: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  000000014002A3C3: 48 8B F0           mov         rsi,rax
  000000014002A3C6: B9 10 00 00 00     mov         ecx,10h
  000000014002A3CB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A3CD: 45 33 C0           xor         r8d,r8d
  000000014002A3D0: 33 D2              xor         edx,edx
  000000014002A3D2: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014002A3DA: FF 15 88 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A3E0: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  000000014002A3E8: 48 8B F0           mov         rsi,rax
  000000014002A3EB: B9 10 00 00 00     mov         ecx,10h
  000000014002A3F0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A3F2: 45 33 C0           xor         r8d,r8d
  000000014002A3F5: 33 D2              xor         edx,edx
  000000014002A3F7: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000014002A3FF: FF 15 63 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A405: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  000000014002A40D: 48 8B F0           mov         rsi,rax
  000000014002A410: B9 10 00 00 00     mov         ecx,10h
  000000014002A415: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A417: 45 33 C0           xor         r8d,r8d
  000000014002A41A: 33 D2              xor         edx,edx
  000000014002A41C: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  000000014002A424: FF 15 3E 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A42A: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  000000014002A432: 48 8B F0           mov         rsi,rax
  000000014002A435: B9 10 00 00 00     mov         ecx,10h
  000000014002A43A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A43C: 45 33 C0           xor         r8d,r8d
  000000014002A43F: 33 D2              xor         edx,edx
  000000014002A441: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014002A449: FF 15 19 9D DD 01  call        qword ptr [0000000141E04168h]
  000000014002A44F: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  000000014002A457: 48 8B F0           mov         rsi,rax
  000000014002A45A: B9 10 00 00 00     mov         ecx,10h
  000000014002A45F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A461: 45 33 C0           xor         r8d,r8d
  000000014002A464: 33 D2              xor         edx,edx
  000000014002A466: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000014002A46E: FF 15 F4 9C DD 01  call        qword ptr [0000000141E04168h]
  000000014002A474: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  000000014002A47C: 48 8B F0           mov         rsi,rax
  000000014002A47F: B9 10 00 00 00     mov         ecx,10h
  000000014002A484: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A486: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  000000014002A48E: 48 8D B4 24 18 01  lea         rsi,[rsp+0000000000000118h]
                    00 00
  000000014002A496: B9 10 00 00 00     mov         ecx,10h
  000000014002A49B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A49D: 48 8D 84 24 10 03  lea         rax,[rsp+0000000000000310h]
                    00 00
  000000014002A4A5: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014002A4AA: 48 8D 84 24 00 03  lea         rax,[rsp+0000000000000300h]
                    00 00
  000000014002A4B2: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014002A4B7: 48 8D 84 24 F0 02  lea         rax,[rsp+00000000000002F0h]
                    00 00
  000000014002A4BF: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014002A4C4: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  000000014002A4CC: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014002A4D1: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  000000014002A4D9: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014002A4DE: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  000000014002A4E6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014002A4EB: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  000000014002A4F3: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014002A4F8: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  000000014002A500: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002A505: 4C 8D 8C 24 90 02  lea         r9,[rsp+0000000000000290h]
                    00 00
  000000014002A50D: 4C 8D 84 24 80 02  lea         r8,[rsp+0000000000000280h]
                    00 00
  000000014002A515: 48 8D 15 3C 5C 0D  lea         rdx,[0000000142100158h]
                    02
  000000014002A51C: 48 8D 0D DD C1 D4  lea         rcx,[0000000143D76700h]
                    03
  000000014002A523: E8 98 4E 2A 00     call        00000001402CF3C0
  000000014002A528: 45 33 C0           xor         r8d,r8d
  000000014002A52B: 33 D2              xor         edx,edx
  000000014002A52D: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  000000014002A535: FF 15 2D 9C DD 01  call        qword ptr [0000000141E04168h]
  000000014002A53B: 48 8D BC 24 B0 03  lea         rdi,[rsp+00000000000003B0h]
                    00 00
  000000014002A543: 48 8B F0           mov         rsi,rax
  000000014002A546: B9 10 00 00 00     mov         ecx,10h
  000000014002A54B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A54D: 45 33 C0           xor         r8d,r8d
  000000014002A550: 33 D2              xor         edx,edx
  000000014002A552: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  000000014002A55A: FF 15 08 9C DD 01  call        qword ptr [0000000141E04168h]
  000000014002A560: 48 8D BC 24 A0 03  lea         rdi,[rsp+00000000000003A0h]
                    00 00
  000000014002A568: 48 8B F0           mov         rsi,rax
  000000014002A56B: B9 10 00 00 00     mov         ecx,10h
  000000014002A570: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A572: 45 33 C0           xor         r8d,r8d
  000000014002A575: 33 D2              xor         edx,edx
  000000014002A577: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  000000014002A57F: FF 15 E3 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A585: 48 8D BC 24 90 03  lea         rdi,[rsp+0000000000000390h]
                    00 00
  000000014002A58D: 48 8B F0           mov         rsi,rax
  000000014002A590: B9 10 00 00 00     mov         ecx,10h
  000000014002A595: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A597: 45 33 C0           xor         r8d,r8d
  000000014002A59A: 33 D2              xor         edx,edx
  000000014002A59C: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014002A5A4: FF 15 BE 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A5AA: 48 8D BC 24 80 03  lea         rdi,[rsp+0000000000000380h]
                    00 00
  000000014002A5B2: 48 8B F0           mov         rsi,rax
  000000014002A5B5: B9 10 00 00 00     mov         ecx,10h
  000000014002A5BA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A5BC: 45 33 C0           xor         r8d,r8d
  000000014002A5BF: 33 D2              xor         edx,edx
  000000014002A5C1: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  000000014002A5C9: FF 15 99 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A5CF: 48 8D BC 24 70 03  lea         rdi,[rsp+0000000000000370h]
                    00 00
  000000014002A5D7: 48 8B F0           mov         rsi,rax
  000000014002A5DA: B9 10 00 00 00     mov         ecx,10h
  000000014002A5DF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A5E1: 45 33 C0           xor         r8d,r8d
  000000014002A5E4: 33 D2              xor         edx,edx
  000000014002A5E6: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  000000014002A5EE: FF 15 74 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A5F4: 48 8D BC 24 60 03  lea         rdi,[rsp+0000000000000360h]
                    00 00
  000000014002A5FC: 48 8B F0           mov         rsi,rax
  000000014002A5FF: B9 10 00 00 00     mov         ecx,10h
  000000014002A604: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A606: 45 33 C0           xor         r8d,r8d
  000000014002A609: 33 D2              xor         edx,edx
  000000014002A60B: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  000000014002A613: FF 15 4F 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A619: 48 8D BC 24 50 03  lea         rdi,[rsp+0000000000000350h]
                    00 00
  000000014002A621: 48 8B F0           mov         rsi,rax
  000000014002A624: B9 10 00 00 00     mov         ecx,10h
  000000014002A629: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A62B: 45 33 C0           xor         r8d,r8d
  000000014002A62E: 33 D2              xor         edx,edx
  000000014002A630: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  000000014002A638: FF 15 2A 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A63E: 48 8D BC 24 40 03  lea         rdi,[rsp+0000000000000340h]
                    00 00
  000000014002A646: 48 8B F0           mov         rsi,rax
  000000014002A649: B9 10 00 00 00     mov         ecx,10h
  000000014002A64E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A650: 45 33 C0           xor         r8d,r8d
  000000014002A653: 33 D2              xor         edx,edx
  000000014002A655: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  000000014002A65D: FF 15 05 9B DD 01  call        qword ptr [0000000141E04168h]
  000000014002A663: 48 8D BC 24 30 03  lea         rdi,[rsp+0000000000000330h]
                    00 00
  000000014002A66B: 48 8B F0           mov         rsi,rax
  000000014002A66E: B9 10 00 00 00     mov         ecx,10h
  000000014002A673: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A675: 45 33 C0           xor         r8d,r8d
  000000014002A678: 33 D2              xor         edx,edx
  000000014002A67A: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  000000014002A682: FF 15 E0 9A DD 01  call        qword ptr [0000000141E04168h]
  000000014002A688: 48 8D BC 24 20 03  lea         rdi,[rsp+0000000000000320h]
                    00 00
  000000014002A690: 48 8B F0           mov         rsi,rax
  000000014002A693: B9 10 00 00 00     mov         ecx,10h
  000000014002A698: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A69A: 48 8D 84 24 B0 03  lea         rax,[rsp+00000000000003B0h]
                    00 00
  000000014002A6A2: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014002A6A7: 48 8D 84 24 A0 03  lea         rax,[rsp+00000000000003A0h]
                    00 00
  000000014002A6AF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014002A6B4: 48 8D 84 24 90 03  lea         rax,[rsp+0000000000000390h]
                    00 00
  000000014002A6BC: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014002A6C1: 48 8D 84 24 80 03  lea         rax,[rsp+0000000000000380h]
                    00 00
  000000014002A6C9: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014002A6CE: 48 8D 84 24 70 03  lea         rax,[rsp+0000000000000370h]
                    00 00
  000000014002A6D6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014002A6DB: 48 8D 84 24 60 03  lea         rax,[rsp+0000000000000360h]
                    00 00
  000000014002A6E3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014002A6E8: 48 8D 84 24 50 03  lea         rax,[rsp+0000000000000350h]
                    00 00
  000000014002A6F0: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014002A6F5: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  000000014002A6FD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002A702: 4C 8D 8C 24 30 03  lea         r9,[rsp+0000000000000330h]
                    00 00
  000000014002A70A: 4C 8D 84 24 20 03  lea         r8,[rsp+0000000000000320h]
                    00 00
  000000014002A712: 48 8D 15 57 5A 0D  lea         rdx,[0000000142100170h]
                    02
  000000014002A719: 48 8D 0D E0 BF D4  lea         rcx,[0000000143D76700h]
                    03
  000000014002A720: E8 9B 4C 2A 00     call        00000001402CF3C0
  000000014002A725: 45 33 C0           xor         r8d,r8d
  000000014002A728: 33 D2              xor         edx,edx
  000000014002A72A: 48 8D 8C 24 D8 01  lea         rcx,[rsp+00000000000001D8h]
                    00 00
  000000014002A732: FF 15 30 9A DD 01  call        qword ptr [0000000141E04168h]
  000000014002A738: 48 8D BC 24 50 04  lea         rdi,[rsp+0000000000000450h]
                    00 00
  000000014002A740: 48 8B F0           mov         rsi,rax
  000000014002A743: B9 10 00 00 00     mov         ecx,10h
  000000014002A748: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A74A: 45 33 C0           xor         r8d,r8d
  000000014002A74D: 33 D2              xor         edx,edx
  000000014002A74F: 48 8D 8C 24 E8 01  lea         rcx,[rsp+00000000000001E8h]
                    00 00
  000000014002A757: FF 15 0B 9A DD 01  call        qword ptr [0000000141E04168h]
  000000014002A75D: 48 8D BC 24 40 04  lea         rdi,[rsp+0000000000000440h]
                    00 00
  000000014002A765: 48 8B F0           mov         rsi,rax
  000000014002A768: B9 10 00 00 00     mov         ecx,10h
  000000014002A76D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A76F: 45 33 C0           xor         r8d,r8d
  000000014002A772: 33 D2              xor         edx,edx
  000000014002A774: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  000000014002A77C: FF 15 E6 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A782: 48 8D BC 24 30 04  lea         rdi,[rsp+0000000000000430h]
                    00 00
  000000014002A78A: 48 8B F0           mov         rsi,rax
  000000014002A78D: B9 10 00 00 00     mov         ecx,10h
  000000014002A792: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A794: 45 33 C0           xor         r8d,r8d
  000000014002A797: 33 D2              xor         edx,edx
  000000014002A799: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  000000014002A7A1: FF 15 C1 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A7A7: 48 8D BC 24 20 04  lea         rdi,[rsp+0000000000000420h]
                    00 00
  000000014002A7AF: 48 8B F0           mov         rsi,rax
  000000014002A7B2: B9 10 00 00 00     mov         ecx,10h
  000000014002A7B7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A7B9: 45 33 C0           xor         r8d,r8d
  000000014002A7BC: 33 D2              xor         edx,edx
  000000014002A7BE: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  000000014002A7C6: FF 15 9C 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A7CC: 48 8D BC 24 10 04  lea         rdi,[rsp+0000000000000410h]
                    00 00
  000000014002A7D4: 48 8B F0           mov         rsi,rax
  000000014002A7D7: B9 10 00 00 00     mov         ecx,10h
  000000014002A7DC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A7DE: 45 33 C0           xor         r8d,r8d
  000000014002A7E1: 33 D2              xor         edx,edx
  000000014002A7E3: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  000000014002A7EB: FF 15 77 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A7F1: 48 8D BC 24 00 04  lea         rdi,[rsp+0000000000000400h]
                    00 00
  000000014002A7F9: 48 8B F0           mov         rsi,rax
  000000014002A7FC: B9 10 00 00 00     mov         ecx,10h
  000000014002A801: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A803: 45 33 C0           xor         r8d,r8d
  000000014002A806: 33 D2              xor         edx,edx
  000000014002A808: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  000000014002A810: FF 15 52 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A816: 48 8D BC 24 F0 03  lea         rdi,[rsp+00000000000003F0h]
                    00 00
  000000014002A81E: 48 8B F0           mov         rsi,rax
  000000014002A821: B9 10 00 00 00     mov         ecx,10h
  000000014002A826: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A828: 45 33 C0           xor         r8d,r8d
  000000014002A82B: 33 D2              xor         edx,edx
  000000014002A82D: 48 8D 8C 24 48 02  lea         rcx,[rsp+0000000000000248h]
                    00 00
  000000014002A835: FF 15 2D 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A83B: 48 8D BC 24 E0 03  lea         rdi,[rsp+00000000000003E0h]
                    00 00
  000000014002A843: 48 8B F0           mov         rsi,rax
  000000014002A846: B9 10 00 00 00     mov         ecx,10h
  000000014002A84B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A84D: 45 33 C0           xor         r8d,r8d
  000000014002A850: 33 D2              xor         edx,edx
  000000014002A852: 48 8D 8C 24 58 02  lea         rcx,[rsp+0000000000000258h]
                    00 00
  000000014002A85A: FF 15 08 99 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A860: 48 8D BC 24 D0 03  lea         rdi,[rsp+00000000000003D0h]
                    00 00
  000000014002A868: 48 8B F0           mov         rsi,rax
  000000014002A86B: B9 10 00 00 00     mov         ecx,10h
  000000014002A870: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A872: 45 33 C0           xor         r8d,r8d
  000000014002A875: 33 D2              xor         edx,edx
  000000014002A877: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  000000014002A87F: FF 15 E3 98 DD 01  call        qword ptr [0000000141E04168h]
  000000014002A885: 48 8D BC 24 C0 03  lea         rdi,[rsp+00000000000003C0h]
                    00 00
  000000014002A88D: 48 8B F0           mov         rsi,rax
  000000014002A890: B9 10 00 00 00     mov         ecx,10h
  000000014002A895: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014002A897: 48 8D 84 24 50 04  lea         rax,[rsp+0000000000000450h]
                    00 00
  000000014002A89F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014002A8A4: 48 8D 84 24 40 04  lea         rax,[rsp+0000000000000440h]
                    00 00
  000000014002A8AC: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014002A8B1: 48 8D 84 24 30 04  lea         rax,[rsp+0000000000000430h]
                    00 00
  000000014002A8B9: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014002A8BE: 48 8D 84 24 20 04  lea         rax,[rsp+0000000000000420h]
                    00 00
  000000014002A8C6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014002A8CB: 48 8D 84 24 10 04  lea         rax,[rsp+0000000000000410h]
                    00 00
  000000014002A8D3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014002A8D8: 48 8D 84 24 00 04  lea         rax,[rsp+0000000000000400h]
                    00 00
  000000014002A8E0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014002A8E5: 48 8D 84 24 F0 03  lea         rax,[rsp+00000000000003F0h]
                    00 00
  000000014002A8ED: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014002A8F2: 48 8D 84 24 E0 03  lea         rax,[rsp+00000000000003E0h]
                    00 00
  000000014002A8FA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002A8FF: 4C 8D 8C 24 D0 03  lea         r9,[rsp+00000000000003D0h]
                    00 00
  000000014002A907: 4C 8D 84 24 C0 03  lea         r8,[rsp+00000000000003C0h]
                    00 00
  000000014002A90F: 48 8D 15 7A 58 0D  lea         rdx,[0000000142100190h]
                    02
  000000014002A916: 48 8D 0D E3 BD D4  lea         rcx,[0000000143D76700h]
                    03
  000000014002A91D: E8 9E 4A 2A 00     call        00000001402CF3C0
  000000014002A922: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A92A: E8 21 8E FE FF     call        0000000140013750
  000000014002A92F: 48 8B C8           mov         rcx,rax
  000000014002A932: E8 D9 A5 FF FF     call        0000000140024F10
  000000014002A937: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014002A93C: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  000000014002A942: 74 0A              je          000000014002A94E
  000000014002A944: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014002A949: E8 92 77 31 01     call        00000001413420E0
  000000014002A94E: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A956: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002A959: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A961: FF 90 C0 02 00 00  call        qword ptr [rax+00000000000002C0h]
  000000014002A967: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A96F: 0F B6 80 C0 03 00  movzx       eax,byte ptr [rax+00000000000003C0h]
                    00
  000000014002A976: 85 C0              test        eax,eax
  000000014002A978: 75 1B              jne         000000014002A995
  000000014002A97A: 48 8B 84 24 A0 04  mov         rax,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A982: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002A985: 33 D2              xor         edx,edx
  000000014002A987: 48 8B 8C 24 A0 04  mov         rcx,qword ptr [rsp+00000000000004A0h]
                    00 00
  000000014002A98F: FF 90 F0 07 00 00  call        qword ptr [rax+00000000000007F0h]
  000000014002A995: 48 81 C4 88 04 00  add         rsp,488h
                    00
  000000014002A99C: 5F                 pop         rdi
  000000014002A99D: 5E                 pop         rsi
  000000014002A99E: C3                 ret

  Summary

     1E02000 .text
