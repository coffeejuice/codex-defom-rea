Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DAE450: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DAE455: 56                 push        rsi
  0000000140DAE456: 57                 push        rdi
  0000000140DAE457: 48 81 EC 08 02 00  sub         rsp,208h
                    00
  0000000140DAE45E: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE466: 48 83 C1 18        add         rcx,18h
  0000000140DAE46A: E8 81 CA E6 FF     call        0000000140C1AEF0
  0000000140DAE46F: 48 85 C0           test        rax,rax
  0000000140DAE472: 0F 84 BE 03 00 00  je          0000000140DAE836
  0000000140DAE478: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE480: 48 83 C1 18        add         rcx,18h
  0000000140DAE484: E8 67 CA E6 FF     call        0000000140C1AEF0
  0000000140DAE489: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140DAE48E: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140DAE494: 0F 84 9C 03 00 00  je          0000000140DAE836
  0000000140DAE49A: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DAE4A2: 4C 8D 0D 4F E9 7B  lea         r9,[000000014356CDF8h]
                    02
  0000000140DAE4A9: 4C 8D 05 A8 90 7B  lea         r8,[0000000143567558h]
                    02
  0000000140DAE4B0: 33 D2              xor         edx,edx
  0000000140DAE4B2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140DAE4B7: E8 40 14 6C 00     call        000000014146F8FC
  0000000140DAE4BC: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140DAE4C1: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140DAE4C7: 74 13              je          0000000140DAE4DC
  0000000140DAE4C9: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140DAE4CE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DAE4D1: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140DAE4D6: FF 90 D0 01 00 00  call        qword ptr [rax+00000000000001D0h]
  0000000140DAE4DC: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE4E4: 48 83 C1 18        add         rcx,18h
  0000000140DAE4E8: E8 03 CA E6 FF     call        0000000140C1AEF0
  0000000140DAE4ED: 48 8B D0           mov         rdx,rax
  0000000140DAE4F0: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE4F8: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  0000000140DAE4FC: FF 15 CE 88 05 01  call        qword ptr [0000000141E06DD0h]
  0000000140DAE502: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE50A: 0F B6 40 10        movzx       eax,byte ptr [rax+10h]
  0000000140DAE50E: 85 C0              test        eax,eax
  0000000140DAE510: 0F 84 20 03 00 00  je          0000000140DAE836
  0000000140DAE516: 48 8B 94 24 20 02  mov         rdx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE51E: 48 83 C2 18        add         rdx,18h
  0000000140DAE522: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE52A: 48 83 C1 38        add         rcx,38h
  0000000140DAE52E: E8 CD D4 EA FF     call        0000000140C5BA00
  0000000140DAE533: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE53B: 48 83 C1 38        add         rcx,38h
  0000000140DAE53F: E8 AC C9 E6 FF     call        0000000140C1AEF0
  0000000140DAE544: 33 D2              xor         edx,edx
  0000000140DAE546: 48 8B C8           mov         rcx,rax
  0000000140DAE549: FF 15 B9 AA 05 01  call        qword ptr [0000000141E09008h]
  0000000140DAE54F: BA 0F 00 00 00     mov         edx,0Fh
  0000000140DAE554: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE55C: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  0000000140DAE560: FF 15 A2 5B 05 01  call        qword ptr [0000000141E04108h]
  0000000140DAE566: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE56E: 48 83 C1 38        add         rcx,38h
  0000000140DAE572: E8 79 C9 E6 FF     call        0000000140C1AEF0
  0000000140DAE577: 48 8B C8           mov         rcx,rax
  0000000140DAE57A: FF 15 30 D6 05 01  call        qword ptr [0000000141E0BBB0h]
  0000000140DAE580: C6 84 24 10 01 00  mov         byte ptr [rsp+0000000000000110h],1
                    00 01
  0000000140DAE588: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE590: 48 83 C1 38        add         rcx,38h
  0000000140DAE594: E8 57 C9 E6 FF     call        0000000140C1AEF0
  0000000140DAE599: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  0000000140DAE5A1: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  0000000140DAE5A9: 48 8D 15 10 C9 CA  lea         rdx,[0000000142A5AEC0h]
                    01
  0000000140DAE5B0: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140DAE5B8: E8 C3 6A 27 FF     call        0000000140025080
  0000000140DAE5BD: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140DAE5C5: 48 8B F0           mov         rsi,rax
  0000000140DAE5C8: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE5CD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE5CF: 48 8D BC 24 F0 00  lea         rdi,[rsp+00000000000000F0h]
                    00 00
  0000000140DAE5D7: 48 8D B4 24 F0 01  lea         rsi,[rsp+00000000000001F0h]
                    00 00
  0000000140DAE5DF: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE5E4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE5E6: 4C 8D 84 24 38 01  lea         r8,[rsp+0000000000000138h]
                    00 00
  0000000140DAE5EE: 48 8D 15 D3 C8 CA  lea         rdx,[0000000142A5AEC8h]
                    01
  0000000140DAE5F5: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140DAE5FD: E8 7E 6A 27 FF     call        0000000140025080
  0000000140DAE602: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140DAE60A: 48 8B F0           mov         rsi,rax
  0000000140DAE60D: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE612: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE614: 48 8D BC 24 18 01  lea         rdi,[rsp+0000000000000118h]
                    00 00
  0000000140DAE61C: 48 8D B4 24 E0 01  lea         rsi,[rsp+00000000000001E0h]
                    00 00
  0000000140DAE624: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE629: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE62B: 45 33 C0           xor         r8d,r8d
  0000000140DAE62E: 33 D2              xor         edx,edx
  0000000140DAE630: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140DAE635: FF 15 2D 5B 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE63B: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  0000000140DAE643: 48 8B F0           mov         rsi,rax
  0000000140DAE646: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE64B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE64D: 45 33 C0           xor         r8d,r8d
  0000000140DAE650: 33 D2              xor         edx,edx
  0000000140DAE652: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140DAE65A: FF 15 08 5B 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE660: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  0000000140DAE668: 48 8B F0           mov         rsi,rax
  0000000140DAE66B: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE670: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE672: 45 33 C0           xor         r8d,r8d
  0000000140DAE675: 33 D2              xor         edx,edx
  0000000140DAE677: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140DAE67F: FF 15 E3 5A 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE685: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140DAE68D: 48 8B F0           mov         rsi,rax
  0000000140DAE690: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE695: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE697: 45 33 C0           xor         r8d,r8d
  0000000140DAE69A: 33 D2              xor         edx,edx
  0000000140DAE69C: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140DAE6A4: FF 15 BE 5A 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE6AA: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140DAE6B2: 48 8B F0           mov         rsi,rax
  0000000140DAE6B5: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE6BA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE6BC: 45 33 C0           xor         r8d,r8d
  0000000140DAE6BF: 33 D2              xor         edx,edx
  0000000140DAE6C1: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140DAE6C9: FF 15 99 5A 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE6CF: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140DAE6D7: 48 8B F0           mov         rsi,rax
  0000000140DAE6DA: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE6DF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE6E1: 45 33 C0           xor         r8d,r8d
  0000000140DAE6E4: 33 D2              xor         edx,edx
  0000000140DAE6E6: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140DAE6EE: FF 15 74 5A 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE6F4: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140DAE6FC: 48 8B F0           mov         rsi,rax
  0000000140DAE6FF: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE704: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE706: 45 33 C0           xor         r8d,r8d
  0000000140DAE709: 33 D2              xor         edx,edx
  0000000140DAE70B: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140DAE713: FF 15 4F 5A 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE719: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140DAE721: 48 8B F0           mov         rsi,rax
  0000000140DAE724: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE729: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE72B: 45 33 C0           xor         r8d,r8d
  0000000140DAE72E: 33 D2              xor         edx,edx
  0000000140DAE730: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140DAE738: FF 15 2A 5A 05 01  call        qword ptr [0000000141E04168h]
  0000000140DAE73E: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140DAE746: 48 8B F0           mov         rsi,rax
  0000000140DAE749: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE74E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE750: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140DAE758: 48 8D B4 24 F0 00  lea         rsi,[rsp+00000000000000F0h]
                    00 00
  0000000140DAE760: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE765: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE767: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140DAE76F: 48 8D B4 24 18 01  lea         rsi,[rsp+0000000000000118h]
                    00 00
  0000000140DAE777: B9 10 00 00 00     mov         ecx,10h
  0000000140DAE77C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140DAE77E: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  0000000140DAE786: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140DAE78B: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  0000000140DAE793: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140DAE798: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140DAE7A0: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140DAE7A5: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140DAE7AD: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140DAE7B2: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140DAE7BA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140DAE7BF: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140DAE7C7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140DAE7CC: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140DAE7D4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140DAE7D9: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140DAE7E1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140DAE7E6: 4C 8D 8C 24 50 01  lea         r9,[rsp+0000000000000150h]
                    00 00
  0000000140DAE7EE: 4C 8D 84 24 40 01  lea         r8,[rsp+0000000000000140h]
                    00 00
  0000000140DAE7F6: 48 8D 15 DB C6 CA  lea         rdx,[0000000142A5AED8h]
                    01
  0000000140DAE7FD: 48 8D 0D FC 7E FC  lea         rcx,[0000000143D76700h]
                    02
  0000000140DAE804: E8 B7 0B 52 FF     call        00000001402CF3C0
  0000000140DAE809: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE811: 48 83 C1 38        add         rcx,38h
  0000000140DAE815: E8 D6 C6 E6 FF     call        0000000140C1AEF0
  0000000140DAE81A: 48 8B C8           mov         rcx,rax
  0000000140DAE81D: FF 15 FD 6D 05 01  call        qword ptr [0000000141E05620h]
  0000000140DAE823: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140DAE82B: 48 83 C1 18        add         rcx,18h
  0000000140DAE82F: 33 D2              xor         edx,edx
  0000000140DAE831: E8 EA E0 F0 FF     call        0000000140CBC920
  0000000140DAE836: 48 81 C4 08 02 00  add         rsp,208h
                    00
  0000000140DAE83D: 5F                 pop         rdi
  0000000140DAE83E: 5E                 pop         rsi
  0000000140DAE83F: C3                 ret

  Summary

     1E02000 .text
