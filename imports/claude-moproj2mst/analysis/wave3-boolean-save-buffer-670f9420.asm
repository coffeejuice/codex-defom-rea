Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F9420: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000000670F9425: 53                 push        rbx
  00000000670F9426: 55                 push        rbp
  00000000670F9427: 56                 push        rsi
  00000000670F9428: 57                 push        rdi
  00000000670F9429: 48 83 EC 48        sub         rsp,48h
  00000000670F942D: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F9436: 48 8B F2           mov         rsi,rdx
  00000000670F9439: 48 8B F9           mov         rdi,rcx
  00000000670F943C: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670F9440: 48 8B 58 78        mov         rbx,qword ptr [rax+78h]
  00000000670F9444: 48 83 FB FF        cmp         rbx,0FFFFFFFFFFFFFFFFh
  00000000670F9448: 0F 85 F8 00 00 00  jne         00000000670F9546
  00000000670F944E: 39 98 98 00 00 00  cmp         dword ptr [rax+0000000000000098h],ebx
  00000000670F9454: 0F 85 EC 00 00 00  jne         00000000670F9546
  00000000670F945A: 48 83 78 70 00     cmp         qword ptr [rax+70h],0
  00000000670F945F: 0F 85 E1 00 00 00  jne         00000000670F9546
  00000000670F9465: 48 8D 58 20        lea         rbx,[rax+20h]
  00000000670F9469: 48 8B CB           mov         rcx,rbx
  00000000670F946C: E8 7F 66 FF FF     call        00000000670EFAF0
  00000000670F9471: 84 C0              test        al,al
  00000000670F9473: 0F 85 C6 00 00 00  jne         00000000670F953F
  00000000670F9479: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000670F9481: 48 8B CB           mov         rcx,rbx
  00000000670F9484: E8 B7 5B FF FF     call        00000000670EF040
  00000000670F9489: 90                 nop
  00000000670F948A: 48 8B D0           mov         rdx,rax
  00000000670F948D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F9492: E8 D9 C7 FB FF     call        00000000670B5C70
  00000000670F9497: 90                 nop
  00000000670F9498: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670F94A0: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670F94A4: 75 0D              jne         00000000670F94B3
  00000000670F94A6: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670F94AE: E8 1D 6C F7 FF     call        00000000670700D0
  00000000670F94B3: C7 44 24 70 03 00  mov         dword ptr [rsp+70h],3
                    00 00
  00000000670F94BB: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000000670F94C0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F94C5: E8 16 B6 FB FF     call        00000000670B4AE0
  00000000670F94CA: 84 C0              test        al,al
  00000000670F94CC: 74 67              je          00000000670F9535
  00000000670F94CE: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000000670F94D3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F94D8: E8 43 D4 FB FF     call        00000000670B6920
  00000000670F94DD: 0F B6 D8           movzx       ebx,al
  00000000670F94E0: 84 C0              test        al,al
  00000000670F94E2: 75 40              jne         00000000670F9524
  00000000670F94E4: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  00000000670F94EC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F94F1: E8 2A 2C FC FF     call        00000000670BC120
  00000000670F94F6: 90                 nop
  00000000670F94F7: 4C 8B C0           mov         r8,rax
  00000000670F94FA: BA 0C 00 00 00     mov         edx,0Ch
  00000000670F94FF: 48 8B CF           mov         rcx,rdi
  00000000670F9502: E8 39 E5 FA FF     call        00000000670A7A40
  00000000670F9507: 90                 nop
  00000000670F9508: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F9510: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F9514: 75 0E              jne         00000000670F9524
  00000000670F9516: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F951E: E8 AD 6B F7 FF     call        00000000670700D0
  00000000670F9523: 90                 nop
  00000000670F9524: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F9529: E8 62 CC FB FF     call        00000000670B6190
  00000000670F952E: 8A C3              mov         al,bl
  00000000670F9530: E9 AA 00 00 00     jmp         00000000670F95DF
  00000000670F9535: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670F953A: E8 51 CC FB FF     call        00000000670B6190
  00000000670F953F: 32 C0              xor         al,al
  00000000670F9541: E9 99 00 00 00     jmp         00000000670F95DF
  00000000670F9546: 48 83 FB FF        cmp         rbx,0FFFFFFFFFFFFFFFFh
  00000000670F954A: 75 35              jne         00000000670F9581
  00000000670F954C: 48 8B 48 70        mov         rcx,qword ptr [rax+70h]
  00000000670F9550: 48 85 C9           test        rcx,rcx
  00000000670F9553: 74 13              je          00000000670F9568
  00000000670F9555: FF 15 45 F2 0A 00  call        qword ptr [00000000671A87A0h]
  00000000670F955B: 8B C8              mov         ecx,eax
  00000000670F955D: FF 15 35 F2 0A 00  call        qword ptr [00000000671A8798h]
  00000000670F9563: 48 8B D8           mov         rbx,rax
  00000000670F9566: EB 0F              jmp         00000000670F9577
  00000000670F9568: 8B 88 98 00 00 00  mov         ecx,dword ptr [rax+0000000000000098h]
  00000000670F956E: FF 15 24 F2 0A 00  call        qword ptr [00000000671A8798h]
  00000000670F9574: 48 8B D8           mov         rbx,rax
  00000000670F9577: 48 83 FB FF        cmp         rbx,0FFFFFFFFFFFFFFFFh
  00000000670F957B: 75 04              jne         00000000670F9581
  00000000670F957D: 32 C0              xor         al,al
  00000000670F957F: EB 5E              jmp         00000000670F95DF
  00000000670F9581: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670F9584: 48 8B CF           mov         rcx,rdi
  00000000670F9587: FF 50 28           call        qword ptr [rax+28h]
  00000000670F958A: 48 8B E8           mov         rbp,rax
  00000000670F958D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000670F9592: 4C 8B 07           mov         r8,qword ptr [rdi]
  00000000670F9595: 48 8B D6           mov         rdx,rsi
  00000000670F9598: 48 8B CF           mov         rcx,rdi
  00000000670F959B: 41 FF 50 30        call        qword ptr [r8+30h]
  00000000670F959F: 84 C0              test        al,al
  00000000670F95A1: 74 2E              je          00000000670F95D1
  00000000670F95A3: 48 8B CB           mov         rcx,rbx
  00000000670F95A6: FF 15 0C EC 0A 00  call        qword ptr [00000000671A81B8h]
  00000000670F95AC: 85 C0              test        eax,eax
  00000000670F95AE: 74 21              je          00000000670F95D1
  00000000670F95B0: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670F95B3: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670F95B8: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000000670F95BD: 48 3B EE           cmp         rbp,rsi
  00000000670F95C0: 48 0F 4D D1        cmovge      rdx,rcx
  00000000670F95C4: 48 8B 12           mov         rdx,qword ptr [rdx]
  00000000670F95C7: 48 8B CF           mov         rcx,rdi
  00000000670F95CA: FF 50 30           call        qword ptr [rax+30h]
  00000000670F95CD: B0 01              mov         al,1
  00000000670F95CF: EB 0E              jmp         00000000670F95DF
  00000000670F95D1: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670F95D4: 48 8B D5           mov         rdx,rbp
  00000000670F95D7: 48 8B CF           mov         rcx,rdi
  00000000670F95DA: FF 50 30           call        qword ptr [rax+30h]
  00000000670F95DD: 32 C0              xor         al,al
  00000000670F95DF: 48 83 C4 48        add         rsp,48h
  00000000670F95E3: 5F                 pop         rdi
  00000000670F95E4: 5E                 pop         rsi
  00000000670F95E5: 5D                 pop         rbp
  00000000670F95E6: 5B                 pop         rbx
  00000000670F95E7: C3                 ret

  Summary

      1A7000 .text
