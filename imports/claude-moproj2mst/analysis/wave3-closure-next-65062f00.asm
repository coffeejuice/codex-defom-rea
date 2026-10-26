Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065062F00: 40 57              push        rdi
  0000000065062F02: 48 83 EC 30        sub         rsp,30h
  0000000065062F06: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065062F0F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065062F14: 48 8B D9           mov         rbx,rcx
  0000000065062F17: 48 83 B9 88 00 00  cmp         qword ptr [rcx+0000000000000088h],0
                    00 00
  0000000065062F1F: 0F 85 E1 00 00 00  jne         0000000065063006
  0000000065062F25: B9 70 00 00 00     mov         ecx,70h
  0000000065062F2A: E8 43 E2 57 00     call        00000000655E1172
  0000000065062F2F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000065062F34: 33 FF              xor         edi,edi
  0000000065062F36: 48 85 C0           test        rax,rax
  0000000065062F39: 74 0A              je          0000000065062F45
  0000000065062F3B: 48 8B C8           mov         rcx,rax
  0000000065062F3E: E8 AD D5 FF FF     call        00000000650604F0
  0000000065062F43: EB 03              jmp         0000000065062F48
  0000000065062F45: 48 8B C7           mov         rax,rdi
  0000000065062F48: 48 89 83 88 00 00  mov         qword ptr [rbx+0000000000000088h],rax
                    00
  0000000065062F4F: 48 89 38           mov         qword ptr [rax],rdi
  0000000065062F52: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F59: 48 89 78 08        mov         qword ptr [rax+8],rdi
  0000000065062F5D: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F64: 48 89 78 10        mov         qword ptr [rax+10h],rdi
  0000000065062F68: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F6F: 48 89 78 18        mov         qword ptr [rax+18h],rdi
  0000000065062F73: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F7A: 89 78 40           mov         dword ptr [rax+40h],edi
  0000000065062F7D: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F84: 89 78 44           mov         dword ptr [rax+44h],edi
  0000000065062F87: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F8E: C7 40 48 FF FF FF  mov         dword ptr [rax+48h],0FFFFFFh
                    00
  0000000065062F95: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062F9C: C7 40 4C FF FF FF  mov         dword ptr [rax+4Ch],0FFFFFFh
                    00
  0000000065062FA3: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FAA: 66 89 78 50        mov         word ptr [rax+50h],di
  0000000065062FAE: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FB5: 66 89 78 52        mov         word ptr [rax+52h],di
  0000000065062FB9: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FC0: 83 60 5C FC        and         dword ptr [rax+5Ch],0FFFFFFFCh
  0000000065062FC4: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FCB: 83 60 5C F3        and         dword ptr [rax+5Ch],0FFFFFFF3h
  0000000065062FCF: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FD6: 83 60 5C EF        and         dword ptr [rax+5Ch],0FFFFFFEFh
  0000000065062FDA: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FE1: 83 60 5C DF        and         dword ptr [rax+5Ch],0FFFFFFDFh
  0000000065062FE5: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FEC: 83 60 5C BF        and         dword ptr [rax+5Ch],0FFFFFFBFh
  0000000065062FF0: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  0000000065062FF7: 81 60 5C 7F FF FF  and         dword ptr [rax+5Ch],0FFFFFF7Fh
                    FF
  0000000065062FFE: 48 8B CB           mov         rcx,rbx
  0000000065063001: E8 6A 43 03 00     call        0000000065097370
  0000000065063006: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006506300B: 48 83 C4 30        add         rsp,30h
  000000006506300F: 5F                 pop         rdi
  0000000065063010: C3                 ret

  Summary

      695000 .text
