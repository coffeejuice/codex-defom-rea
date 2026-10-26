Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670FED20: 48 8B C4           mov         rax,rsp
  00000000670FED23: 57                 push        rdi
  00000000670FED24: 48 81 EC A0 02 00  sub         rsp,2A0h
                    00
  00000000670FED2B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FED34: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  00000000670FED38: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  00000000670FED3C: 48 8B 05 5D 4E 1F  mov         rax,qword ptr [00000000672F3BA0h]
                    00
  00000000670FED43: 48 33 C4           xor         rax,rsp
  00000000670FED46: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  00000000670FED4E: 48 8B F2           mov         rsi,rdx
  00000000670FED51: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670FED54: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000670FED59: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670FED5D: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FED62: 83 78 08 02        cmp         dword ptr [rax+8],2
  00000000670FED66: 75 68              jne         00000000670FEDD0
  00000000670FED68: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670FED6C: 66 83 79 02 3A     cmp         word ptr [rcx+2],3Ah
  00000000670FED71: 75 5D              jne         00000000670FEDD0
  00000000670FED73: 8B 08              mov         ecx,dword ptr [rax]
  00000000670FED75: 83 F9 01           cmp         ecx,1
  00000000670FED78: 75 0F              jne         00000000670FED89
  00000000670FED7A: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000670FED7D: FF C2              inc         edx
  00000000670FED7F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FED84: 3B 51 04           cmp         edx,dword ptr [rcx+4]
  00000000670FED87: 7E 1B              jle         00000000670FEDA4
  00000000670FED89: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000670FED8C: FF C1              inc         ecx
  00000000670FED8E: E8 6D C5 F6 FF     call        000000006706B300
  00000000670FED93: 8B D0              mov         edx,eax
  00000000670FED95: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670FED9A: E8 01 14 F7 FF     call        00000000670701A0
  00000000670FED9F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FEDA4: 48 63 51 08        movsxd      rdx,dword ptr [rcx+8]
  00000000670FEDA8: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000670FEDAC: B9 5C 00 00 00     mov         ecx,5Ch
  00000000670FEDB1: 66 89 0C 50        mov         word ptr [rax+rdx*2],cx
  00000000670FEDB5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEDBA: FF 40 08           inc         dword ptr [rax+8]
  00000000670FEDBD: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEDC2: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  00000000670FEDC6: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670FEDCA: 33 C0              xor         eax,eax
  00000000670FEDCC: 66 89 04 51        mov         word ptr [rcx+rdx*2],ax
  00000000670FEDD0: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670FEDD5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670FEDDA: E8 E1 91 FF FF     call        00000000670F7FC0
  00000000670FEDDF: 90                 nop
  00000000670FEDE0: 48 8B C8           mov         rcx,rax
  00000000670FEDE3: E8 68 1C F7 FF     call        0000000067070A50
  00000000670FEDE8: 48 8B C8           mov         rcx,rax
  00000000670FEDEB: FF 15 4F 94 0A 00  call        qword ptr [00000000671A8240h]
  00000000670FEDF1: 8B F8              mov         edi,eax
  00000000670FEDF3: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000000670FEDF8: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670FEDFC: 75 0A              jne         00000000670FEE08
  00000000670FEDFE: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670FEE03: E8 C8 12 F7 FF     call        00000000670700D0
  00000000670FEE08: 83 FF FF           cmp         edi,0FFFFFFFFh
  00000000670FEE0B: 75 33              jne         00000000670FEE40
  00000000670FEE0D: FF 15 CD 95 0A 00  call        qword ptr [00000000671A83E0h]
  00000000670FEE13: 83 F8 05           cmp         eax,5
  00000000670FEE16: 74 05              je          00000000670FEE1D
  00000000670FEE18: 83 F8 20           cmp         eax,20h
  00000000670FEE1B: 75 23              jne         00000000670FEE40
  00000000670FEE1D: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670FEE22: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000670FEE27: E8 94 91 FF FF     call        00000000670F7FC0
  00000000670FEE2C: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670FEE31: 48 8B C8           mov         rcx,rax
  00000000670FEE34: E8 97 FB FF FF     call        00000000670FE9D0
  00000000670FEE39: 84 C0              test        al,al
  00000000670FEE3B: 0F 45 7C 24 40     cmovne      edi,dword ptr [rsp+40h]
  00000000670FEE40: 48 85 F6           test        rsi,rsi
  00000000670FEE43: 74 08              je          00000000670FEE4D
  00000000670FEE45: 83 FF FF           cmp         edi,0FFFFFFFFh
  00000000670FEE48: 0F 95 C0           setne       al
  00000000670FEE4B: 88 06              mov         byte ptr [rsi],al
  00000000670FEE4D: 83 FF FF           cmp         edi,0FFFFFFFFh
  00000000670FEE50: 75 19              jne         00000000670FEE6B
  00000000670FEE52: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEE57: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FEE5B: 75 0A              jne         00000000670FEE67
  00000000670FEE5D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FEE62: E8 69 12 F7 FF     call        00000000670700D0
  00000000670FEE67: 32 C0              xor         al,al
  00000000670FEE69: EB 1F              jmp         00000000670FEE8A
  00000000670FEE6B: C1 EF 04           shr         edi,4
  00000000670FEE6E: 40 80 E7 01        and         dil,1
  00000000670FEE72: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEE77: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FEE7B: 75 0A              jne         00000000670FEE87
  00000000670FEE7D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FEE82: E8 49 12 F7 FF     call        00000000670700D0
  00000000670FEE87: 40 8A C7           mov         al,dil
  00000000670FEE8A: 48 8B 8C 24 90 02  mov         rcx,qword ptr [rsp+0000000000000290h]
                    00 00
  00000000670FEE92: 48 33 CC           xor         rcx,rsp
  00000000670FEE95: E8 76 9D 07 00     call        0000000067178C10
  00000000670FEE9A: 4C 8D 9C 24 A0 02  lea         r11,[rsp+00000000000002A0h]
                    00 00
  00000000670FEEA2: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  00000000670FEEA6: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  00000000670FEEAA: 49 8B E3           mov         rsp,r11
  00000000670FEEAD: 5F                 pop         rdi
  00000000670FEEAE: C3                 ret

  Summary

      1A7000 .text
