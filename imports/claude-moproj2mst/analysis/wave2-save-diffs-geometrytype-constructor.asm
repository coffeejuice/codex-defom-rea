Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001410580A0: mov         qword ptr [rsp+10h],rdx
  00000001410580A5: mov         qword ptr [rsp+8],rcx
  00000001410580AA: push        rdi
  00000001410580AB: sub         rsp,40h
  00000001410580AF: mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
  00000001410580B8: xor         r8d,r8d
  00000001410580BB: mov         rdx,qword ptr [rsp+58h]
  00000001410580C0: mov         rcx,qword ptr [rsp+50h]
  00000001410580C5: call        0000000140130740
  00000001410580CA: nop
  00000001410580CB: mov         rcx,qword ptr [rsp+50h]
  00000001410580D0: lea         rax,[0000000142B794F8h]
  00000001410580D7: mov         qword ptr [rcx],rax
  00000001410580DA: mov         rcx,qword ptr [rsp+50h]
  00000001410580DF: lea         rax,[0000000142B794C0h]
  00000001410580E6: mov         qword ptr [rcx+10h],rax
  00000001410580EA: mov         qword ptr [rsp+20h],0B0h
  00000001410580F3: mov         rcx,qword ptr [rsp+20h]
  00000001410580F8: call        000000014146EABC
  00000001410580FD: mov         qword ptr [rsp+28h],rax
  0000000141058102: cmp         qword ptr [rsp+28h],0
  0000000141058108: je          0000000141058124
  000000014105810A: mov         rdi,qword ptr [rsp+28h]
  000000014105810F: xor         eax,eax
  0000000141058111: mov         rcx,qword ptr [rsp+20h]
  0000000141058116: rep stos    byte ptr [rdi]
  0000000141058118: mov         rax,qword ptr [rsp+28h]
  000000014105811D: mov         qword ptr [rsp+38h],rax
  0000000141058122: jmp         000000014105812D
  0000000141058124: mov         qword ptr [rsp+38h],0
  000000014105812D: mov         rcx,qword ptr [rsp+50h]
  0000000141058132: mov         rax,qword ptr [rsp+38h]
  0000000141058137: mov         qword ptr [rcx+48h],rax
  000000014105813B: mov         rdx,qword ptr [rsp+50h]
  0000000141058140: mov         rcx,qword ptr [rsp+50h]
  0000000141058145: mov         rcx,qword ptr [rcx+48h]
  0000000141058149: call        0000000141058F20
  000000014105814E: mov         rdx,qword ptr [rsp+50h]
  0000000141058153: mov         dword ptr [rdx+50h],0
  000000014105815A: mov         rcx,qword ptr [rsp+50h]
  000000014105815F: call        00000001410582D0
  0000000141058164: nop
  0000000141058165: mov         rax,qword ptr [rsp+50h]
  000000014105816A: add         rsp,40h
  000000014105816E: pop         rdi
  000000014105816F: ret
  0000000141058170: int         3
  0000000141058171: int         3
  0000000141058172: int         3
  0000000141058173: int         3
  0000000141058174: int         3
  0000000141058175: int         3
  0000000141058176: int         3
  0000000141058177: int         3
  0000000141058178: int         3
  0000000141058179: int         3
  000000014105817A: int         3
  000000014105817B: int         3
  000000014105817C: int         3
  000000014105817D: int         3
  000000014105817E: int         3
  000000014105817F: int         3
  0000000141058180: mov         qword ptr [rsp+8],rcx
  0000000141058185: sub         rsp,38h
  0000000141058189: mov         rcx,qword ptr [rsp+40h]
  000000014105818E: lea         rax,[0000000142B794F8h]
  0000000141058195: mov         qword ptr [rcx],rax
  0000000141058198: mov         rcx,qword ptr [rsp+40h]
  000000014105819D: 48
  000000014105819E: 8D
  000000014105819F: 05
  00000001410581A0: 1C

  Summary

     1E02000 .text
