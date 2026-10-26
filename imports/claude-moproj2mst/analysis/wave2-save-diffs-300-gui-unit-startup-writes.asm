Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014001C240: 24 60              and         al,60h
  000000014001C242: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014001C247: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014001C24C: 33 D2              xor         edx,edx
  000000014001C24E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014001C253: E8 98 B2 08 00     call        00000001400A74F0
  000000014001C258: 90                 nop
  000000014001C259: 41 B1 01           mov         r9b,1
  000000014001C25C: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  000000014001C261: 48 8B D0           mov         rdx,rax
  000000014001C264: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014001C269: E8 52 A1 08 00     call        00000001400A63C0
  000000014001C26E: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014001C276: 83 38 01           cmp         dword ptr [rax],1
  000000014001C279: 75 0C              jne         000000014001C287
  000000014001C27B: C7 05 9F D4 D5 03  mov         dword ptr [0000000143D79724h],1
                    01 00 00 00
  000000014001C285: EB 23              jmp         000000014001C2AA
  000000014001C287: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014001C28F: 83 38 02           cmp         dword ptr [rax],2
  000000014001C292: 75 0C              jne         000000014001C2A0
  000000014001C294: C7 05 86 D4 D5 03  mov         dword ptr [0000000143D79724h],2
                    02 00 00 00
  000000014001C29E: EB 0A              jmp         000000014001C2AA
  000000014001C2A0: C7 05 7A D4 D5 03  mov         dword ptr [0000000143D79724h],1
                    01 00 00 00
  000000014001C2AA: 48 8D 0D BF C3 D5  lea         rcx,[0000000143D78670h]
                    03
  000000014001C2B1: E8 BA 66 7E 00     call        0000000140802970
  000000014001C2B6: 48
  000000014001C2B7: 8B
  000000014001C2B8: 84 24 80           test        byte ptr [rax+rax*4],ah
  000000014001C2BB: 00 00              add         byte ptr [rax],al

  Summary

     1E02000 .text
