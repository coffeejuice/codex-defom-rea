Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EA5C0: 40 55              push        rbp
  00000000670EA5C2: 56                 push        rsi
  00000000670EA5C3: 57                 push        rdi
  00000000670EA5C4: 48 83 EC 30        sub         rsp,30h
  00000000670EA5C8: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670EA5D1: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670EA5D6: 48 8B DA           mov         rbx,rdx
  00000000670EA5D9: 48 8B E9           mov         rbp,rcx
  00000000670EA5DC: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000670EA5E0: 48 8D 4F 20        lea         rcx,[rdi+20h]
  00000000670EA5E4: E8 07 55 00 00     call        00000000670EFAF0
  00000000670EA5E9: 84 C0              test        al,al
  00000000670EA5EB: 74 4C              je          00000000670EA639
  00000000670EA5ED: 48 8D 0D 74 A3 14  lea         rcx,[0000000067234968h]
                    00
  00000000670EA5F4: E8 E7 28 F3 FF     call        000000006701CEE0
  00000000670EA5F9: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670EA5FC: 48 8D 0D 4D A3 14  lea         rcx,[0000000067234950h]
                    00
  00000000670EA603: E8 38 19 F8 FF     call        000000006706BF40
  00000000670EA608: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000670EA60D: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000000670EA612: BA 05 00 00 00     mov         edx,5
  00000000670EA617: 48 8B CD           mov         rcx,rbp
  00000000670EA61A: E8 21 D4 FB FF     call        00000000670A7A40
  00000000670EA61F: 90                 nop
  00000000670EA620: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000000670EA625: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EA629: 75 0A              jne         00000000670EA635
  00000000670EA62B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000000670EA630: E8 9B 5A F8 FF     call        00000000670700D0
  00000000670EA635: 32 C0              xor         al,al
  00000000670EA637: EB 51              jmp         00000000670EA68A
  00000000670EA639: 8B 03              mov         eax,dword ptr [rbx]
  00000000670EA63B: A8 04              test        al,4
  00000000670EA63D: 74 05              je          00000000670EA644
  00000000670EA63F: 83 C8 02           or          eax,2
  00000000670EA642: 89 03              mov         dword ptr [rbx],eax
  00000000670EA644: 8B 03              mov         eax,dword ptr [rbx]
  00000000670EA646: A8 02              test        al,2
  00000000670EA648: 74 09              je          00000000670EA653
  00000000670EA64A: A8 05              test        al,5
  00000000670EA64C: 75 05              jne         00000000670EA653
  00000000670EA64E: 83 C8 08           or          eax,8
  00000000670EA651: 89 03              mov         dword ptr [rbx],eax
  00000000670EA653: 8B 03              mov         eax,dword ptr [rbx]
  00000000670EA655: 89 47 38           mov         dword ptr [rdi+38h],eax
  00000000670EA658: C6 87 A0 00 00 00  mov         byte ptr [rdi+00000000000000A0h],0
                    00
  00000000670EA65F: 83 A7 A4 00 00 00  and         dword ptr [rdi+00000000000000A4h],0FFFFFFF7h
                    F7
  00000000670EA666: 48 C7 47 70 00 00  mov         qword ptr [rdi+70h],0
                    00 00
  00000000670EA66E: 83 CE FF           or          esi,0FFFFFFFFh
  00000000670EA671: 89 B7 98 00 00 00  mov         dword ptr [rdi+0000000000000098h],esi
  00000000670EA677: 8B 03              mov         eax,dword ptr [rbx]
  00000000670EA679: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000000670EA67D: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670EA682: 48 8B CF           mov         rcx,rdi
  00000000670EA685: E8 C6 DA 00 00     call        00000000670F8150
  00000000670EA68A: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670EA68F: 48 83 C4 30        add         rsp,30h
  00000000670EA693: 5F                 pop         rdi
  00000000670EA694: 5E                 pop         rsi
  00000000670EA695: 5D                 pop         rbp
  00000000670EA696: C3                 ret

  Summary

      1A7000 .text
