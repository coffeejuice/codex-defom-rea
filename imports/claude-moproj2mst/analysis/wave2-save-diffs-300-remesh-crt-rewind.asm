Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141470298: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000014147029D: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001414702A2: 57                 push        rdi
  00000001414702A3: 48 83 EC 30        sub         rsp,30h
  00000001414702A7: 48 8B D9           mov         rbx,rcx
  00000001414702AA: 33 C0              xor         eax,eax
  00000001414702AC: 48 85 C9           test        rcx,rcx
  00000001414702AF: 0F 95 C0           setne       al
  00000001414702B2: 85 C0              test        eax,eax
  00000001414702B4: 75 25              jne         00000001414702DB
  00000001414702B6: E8 79 3E 00 00     call        0000000141474134
  00000001414702BB: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  00000001414702C1: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  00000001414702C7: 45 33 C9           xor         r9d,r9d
  00000001414702CA: 45 33 C0           xor         r8d,r8d
  00000001414702CD: 33 D2              xor         edx,edx
  00000001414702CF: 33 C9              xor         ecx,ecx
  00000001414702D1: E8 92 02 00 00     call        0000000141470568
  00000001414702D6: E9 80 00 00 00     jmp         000000014147035B
  00000001414702DB: E8 68 97 00 00     call        0000000141479A48
  00000001414702E0: 48 63 F8           movsxd      rdi,eax
  00000001414702E3: 48 8B CB           mov         rcx,rbx
  00000001414702E6: E8 AD D9 FF FF     call        000000014146DC98
  00000001414702EB: 90                 nop
  00000001414702EC: 48 8B CB           mov         rcx,rbx
  00000001414702EF: E8 44 68 00 00     call        0000000141476B38
  00000001414702F4: 83 63 18 CF        and         dword ptr [rbx+18h],0FFFFFFCFh
  00000001414702F8: 83 FF FF           cmp         edi,0FFFFFFFFh
  00000001414702FB: 74 23              je          0000000141470320
  00000001414702FD: 83 FF FE           cmp         edi,0FFFFFFFEh
  0000000141470300: 74 1E              je          0000000141470320
  0000000141470302: 48 8B CF           mov         rcx,rdi
  0000000141470305: 48 8B C7           mov         rax,rdi
  0000000141470308: 48 C1 F8 05        sar         rax,5
  000000014147030C: 48 8D 15 ED 3D BE  lea         rdx,[0000000145054100h]
                    03
  0000000141470313: 83 E1 1F           and         ecx,1Fh
  0000000141470316: 48 6B C9 58        imul        rcx,rcx,58h
  000000014147031A: 48 03 0C C2        add         rcx,qword ptr [rdx+rax*8]
  000000014147031E: EB 07              jmp         0000000141470327
  0000000141470320: 48 8D 0D C9 E3 6E  lea         rcx,[0000000143B5E6F0h]
                    02
  0000000141470327: 0F BE 41 08        movsx       eax,byte ptr [rcx+8]
  000000014147032B: 83 E0 FD           and         eax,0FFFFFFFDh
  000000014147032E: 88 41 08           mov         byte ptr [rcx+8],al
  0000000141470331: 8B 43 18           mov         eax,dword ptr [rbx+18h]
  0000000141470334: 84 C0              test        al,al
  0000000141470336: 79 06              jns         000000014147033E
  0000000141470338: 83 E0 FC           and         eax,0FFFFFFFCh
  000000014147033B: 89 43 18           mov         dword ptr [rbx+18h],eax
  000000014147033E: 45 33 C0           xor         r8d,r8d
  0000000141470341: 33 D2              xor         edx,edx
  0000000141470343: 8B CF              mov         ecx,edi
  0000000141470345: E8 72 30 01 00     call        00000001414833BC
  000000014147034A: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000014147034D: 75 04              jne         0000000141470353
  000000014147034F: 83 4B 18 20        or          dword ptr [rbx+18h],20h
  0000000141470353: 48 8B CB           mov         rcx,rbx
  0000000141470356: E8 CD D9 FF FF     call        000000014146DD28
  000000014147035B: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000141470360: 48 83 C4 30        add         rsp,30h
  0000000141470364: 5F                 pop         rdi
  0000000141470365: C3                 ret
  0000000141470366: CC                 int         3

  Summary

     1E02000 .text
