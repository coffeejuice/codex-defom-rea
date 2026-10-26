
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065045540: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065045545: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  000000006504554A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000006504554F: 57                 push        rdi
  0000000065045550: 48 81 EC B0 01 00  sub         rsp,1B0h
                    00
  0000000065045557: 48 8B FA           mov         rdi,rdx
  000000006504555A: E8 41 F5 FF FF     call        0000000065044AA0
  000000006504555F: 48 8B CF           mov         rcx,rdi
  0000000065045562: 48 8B F0           mov         rsi,rax
  0000000065045565: FF 15 FD 12 65 00  call        qword ptr [0000000065696868h]
  000000006504556B: 48 8B D8           mov         rbx,rax
  000000006504556E: 48 85 C0           test        rax,rax
  0000000065045571: 74 39              je          00000000650455AC
  0000000065045573: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  0000000065045577: 44 8B 41 08        mov         r8d,dword ptr [rcx+8]
  000000006504557B: 41 C1 E8 07        shr         r8d,7
  000000006504557F: 41 F6 C0 01        test        r8b,1
  0000000065045583: 74 19              je          000000006504559E
  0000000065045585: 48 8B C8           mov         rcx,rax
  0000000065045588: E8 93 89 01 00     call        000000006505DF20
  000000006504558D: 48 85 C0           test        rax,rax
  0000000065045590: 74 0C              je          000000006504559E
  0000000065045592: 48 8D 48 10        lea         rcx,[rax+10h]
  0000000065045596: 48 8B D7           mov         rdx,rdi
  0000000065045599: E8 42 E4 FF FF     call        00000000650439E0
  000000006504559E: 48 85 F6           test        rsi,rsi
  00000000650455A1: 74 3D              je          00000000650455E0
  00000000650455A3: 33 C0              xor         eax,eax
  00000000650455A5: 48 3B DE           cmp         rbx,rsi
  00000000650455A8: 48 0F 45 D8        cmovne      rbx,rax
  00000000650455AC: 48 85 F6           test        rsi,rsi
  00000000650455AF: 74 2F              je          00000000650455E0
  00000000650455B1: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  00000000650455B5: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000650455B8: C1 E9 0F           shr         ecx,0Fh
  00000000650455BB: F6 C1 01           test        cl,1
  00000000650455BE: 74 20              je          00000000650455E0
  00000000650455C0: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650455C4: F6 40 0A 01        test        byte ptr [rax+0Ah],1
  00000000650455C8: 74 11              je          00000000650455DB
  00000000650455CA: BA 45 00 00 00     mov         edx,45h
  00000000650455CF: 48 8B CF           mov         rcx,rdi
  00000000650455D2: E8 39 8A 01 00     call        000000006505E010
  00000000650455D7: 84 C0              test        al,al
  00000000650455D9: 75 05              jne         00000000650455E0
  00000000650455DB: 40 B5 01           mov         bpl,1
  00000000650455DE: EB 03              jmp         00000000650455E3
  00000000650455E0: 40 32 ED           xor         bpl,bpl
  00000000650455E3: 48 85 DB           test        rbx,rbx
  00000000650455E6: 75 10              jne         00000000650455F8
  00000000650455E8: 48 85 F6           test        rsi,rsi
  00000000650455EB: 74 0B              je          00000000650455F8
  00000000650455ED: 48 8B D6           mov         rdx,rsi
  00000000650455F0: 48 8B CF           mov         rcx,rdi
  00000000650455F3: E8 68 B4 02 00     call        0000000065070A60
  00000000650455F8: 41 B0 01           mov         r8b,1
  00000000650455FB: BA 07 00 00 00     mov         edx,7
  0000000065045600: 48 8B CF           mov         rcx,rdi
  0000000065045603: E8 D8 39 02 00     call        0000000065068FE0
  0000000065045608: 40 84 ED           test        bpl,bpl
  000000006504560B: 0F 84 27 02 00 00  je          0000000065045838
  0000000065045611: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000065045619: 45 33 C0           xor         r8d,r8d
  000000006504561C: 33 D2              xor         edx,edx
  000000006504561E: FF 15 CC 1B 65 00  call        qword ptr [00000000656971F0h]
  0000000065045624: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  000000006504562C: 45 33 C0           xor         r8d,r8d
  000000006504562F: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045632: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045636: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045639: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006504563D: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000065045645: 33 D2              xor         edx,edx
  0000000065045647: FF 15 A3 1B 65 00  call        qword ptr [00000000656971F0h]
  000000006504564D: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000065045655: 45 33 C0           xor         r8d,r8d
  0000000065045658: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006504565B: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006504565F: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045662: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045666: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  000000006504566E: 33 D2              xor         edx,edx
  0000000065045670: FF 15 7A 1B 65 00  call        qword ptr [00000000656971F0h]
  0000000065045676: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  000000006504567E: 45 33 C0           xor         r8d,r8d
  0000000065045681: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045684: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045688: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006504568B: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006504568F: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000065045697: 33 D2              xor         edx,edx
  0000000065045699: FF 15 51 1B 65 00  call        qword ptr [00000000656971F0h]
  000000006504569F: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000650456A7: 45 33 C0           xor         r8d,r8d
  00000000650456AA: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650456AD: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650456B1: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000650456B4: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650456B8: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  00000000650456C0: 33 D2              xor         edx,edx
  00000000650456C2: FF 15 28 1B 65 00  call        qword ptr [00000000656971F0h]
  00000000650456C8: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  00000000650456D0: 45 33 C0           xor         r8d,r8d
  00000000650456D3: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650456D6: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650456DA: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000650456DD: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650456E1: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000000650456E9: 33 D2              xor         edx,edx
  00000000650456EB: FF 15 FF 1A 65 00  call        qword ptr [00000000656971F0h]
  00000000650456F1: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000650456F9: 45 33 C0           xor         r8d,r8d
  00000000650456FC: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650456FF: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045703: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045706: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006504570A: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000065045712: 33 D2              xor         edx,edx
  0000000065045714: FF 15 D6 1A 65 00  call        qword ptr [00000000656971F0h]
  000000006504571A: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000065045722: 45 33 C0           xor         r8d,r8d
  0000000065045725: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045728: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006504572C: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006504572F: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045733: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  000000006504573B: 33 D2              xor         edx,edx
  000000006504573D: FF 15 AD 1A 65 00  call        qword ptr [00000000656971F0h]
  0000000065045743: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  000000006504574B: 45 33 C0           xor         r8d,r8d
  000000006504574E: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045751: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000065045755: 48 89 0A           mov         qword ptr [rdx],rcx
  0000000065045758: 48 89 42 08        mov         qword ptr [rdx+8],rax
  000000006504575C: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  0000000065045764: 33 D2              xor         edx,edx
  0000000065045766: FF 15 84 1A 65 00  call        qword ptr [00000000656971F0h]
  000000006504576C: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  0000000065045774: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065045777: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006504577B: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006504577E: 48 89 42 08        mov         qword ptr [rdx+8],rax
  0000000065045782: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  000000006504578A: 45 33 C0           xor         r8d,r8d
  000000006504578D: 33 D2              xor         edx,edx
  000000006504578F: FF 15 5B 1A 65 00  call        qword ptr [00000000656971F0h]
  0000000065045795: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006504579A: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  000000006504579F: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650457A2: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000000650457A6: 41 B8 02 00 00 00  mov         r8d,2
  00000000650457AC: 48 89 0A           mov         qword ptr [rdx],rcx
  00000000650457AF: 48 89 42 08        mov         qword ptr [rdx+8],rax
  00000000650457B3: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  00000000650457BB: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000650457C0: 48 8D 84 24 90 00  lea         rax,[rsp+0000000000000090h]
                    00 00
  00000000650457C8: 48 8D 15 09 FE 65  lea         rdx,[00000000656A55D8h]
                    00
  00000000650457CF: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000650457D4: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  00000000650457DC: 48 8B CF           mov         rcx,rdi
  00000000650457DF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000650457E4: 48 8D 84 24 B0 00  lea         rax,[rsp+00000000000000B0h]
                    00 00
  00000000650457EC: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650457F1: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  00000000650457F9: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000650457FE: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  0000000065045806: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006504580B: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  0000000065045813: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000065045818: 48 8D 84 24 C0 00  lea         rax,[rsp+00000000000000C0h]
                    00 00
  0000000065045820: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000065045825: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  000000006504582D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000065045832: FF 15 B0 19 65 00  call        qword ptr [00000000656971E8h]
  0000000065045838: 4C 8D 9C 24 B0 01  lea         r11,[rsp+00000000000001B0h]
                    00 00
  0000000065045840: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  0000000065045844: 49 8B 6B 18        mov         rbp,qword ptr [r11+18h]
  0000000065045848: 49 8B 73 20        mov         rsi,qword ptr [r11+20h]
  000000006504584C: 49 8B E3           mov         rsp,r11
  000000006504584F: 5F                 pop         rdi
  0000000065045850: C3                 ret

  Summary

      695000 .text
