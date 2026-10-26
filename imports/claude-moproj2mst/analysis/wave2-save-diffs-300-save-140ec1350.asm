Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC1350: mov         qword ptr [rsp+8],rcx
  0000000140EC1355: sub         rsp,0D8h
  0000000140EC135C: mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
  0000000140EC1368: lea         rcx,[rsp+40h]
  0000000140EC136D: call        qword ptr [0000000141E044C0h]
  0000000140EC1373: nop
  0000000140EC1374: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC137C: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC1383: mov         rcx,qword ptr [rcx+00000000000009B8h]
  0000000140EC138A: call        qword ptr [0000000141E0B4C0h]
  0000000140EC1390: mov         dword ptr [rsp+24h],eax
  0000000140EC1394: mov         edx,dword ptr [rsp+24h]
  0000000140EC1398: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC13A0: call        0000000140ECEB60
  0000000140EC13A5: mov         dword ptr [rsp+38h],0
  0000000140EC13AD: jmp         0000000140EC13BA
  0000000140EC13AF: mov         eax,dword ptr [rsp+38h]
  0000000140EC13B3: add         eax,1
  0000000140EC13B6: mov         dword ptr [rsp+38h],eax
  0000000140EC13BA: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC13C2: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC13C9: mov         rcx,qword ptr [rcx+00000000000009B8h]
  0000000140EC13D0: call        qword ptr [0000000141E0B4A0h]
  0000000140EC13D6: cmp         dword ptr [rsp+38h],eax
  0000000140EC13DA: jge         0000000140EC14F5
  0000000140EC13E0: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC13E8: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC13EF: mov         r8d,dword ptr [rsp+38h]
  0000000140EC13F4: lea         rdx,[rsp+70h]
  0000000140EC13F9: mov         rcx,qword ptr [rcx+00000000000009B8h]
  0000000140EC1400: call        qword ptr [0000000141E08F50h]
  0000000140EC1406: mov         qword ptr [rsp+00000000000000A0h],rax
  0000000140EC140E: mov         rax,qword ptr [rsp+00000000000000A0h]
  0000000140EC1416: mov         qword ptr [rsp+00000000000000A8h],rax
  0000000140EC141E: mov         rdx,qword ptr [rsp+00000000000000A8h]
  0000000140EC1426: lea         rcx,[rsp+40h]
  0000000140EC142B: call        qword ptr [0000000141E04470h]
  0000000140EC1431: nop
  0000000140EC1432: lea         rcx,[rsp+70h]
  0000000140EC1437: call        qword ptr [0000000141E059E0h]
  0000000140EC143D: lea         rdx,[rsp+48h]
  0000000140EC1442: lea         rcx,[rsp+40h]
  0000000140EC1447: call        qword ptr [0000000141E03F50h]
  0000000140EC144D: nop
  0000000140EC144E: lea         rcx,[rsp+48h]
  0000000140EC1453: call        qword ptr [0000000141E04228h]
  0000000140EC1459: mov         qword ptr [rsp+28h],rax
  0000000140EC145E: lea         r8,[rsp+20h]
  0000000140EC1463: lea         rdx,[0000000142B14C3Ch]
  0000000140EC146A: mov         rcx,qword ptr [rsp+28h]
  0000000140EC146F: call        000000014146DFD4
  0000000140EC1474: mov         rax,qword ptr [rsp+00000000000000E0h]
  0000000140EC147C: mov         rax,qword ptr [rax]
  0000000140EC147F: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC1487: call        qword ptr [rax+0000000000000198h]
  0000000140EC148D: mov         edx,dword ptr [rsp+20h]
  0000000140EC1491: mov         rcx,rax
  0000000140EC1494: call        0000000140C7AE80
  0000000140EC1499: mov         qword ptr [rsp+30h],rax
  0000000140EC149E: cmp         qword ptr [rsp+30h],0
  0000000140EC14A4: je          0000000140EC14E5
  0000000140EC14A6: mov         rax,qword ptr [rsp+00000000000000E0h]
  0000000140EC14AE: cmp         qword ptr [rax+00000000000000F8h],0
  0000000140EC14B6: je          0000000140EC14E5
  0000000140EC14B8: movsxd      rdx,dword ptr [rsp+38h]
  0000000140EC14BD: imul        rdx,rdx,158h
  0000000140EC14C4: mov         rax,qword ptr [rsp+00000000000000E0h]
  0000000140EC14CC: add         rdx,qword ptr [rax+00000000000000F8h]
  0000000140EC14D3: mov         rcx,qword ptr [rsp+30h]
  0000000140EC14D8: add         rcx,2D10h
  0000000140EC14DF: call        000000014085E050
  0000000140EC14E4: nop
  0000000140EC14E5: lea         rcx,[rsp+48h]
  0000000140EC14EA: call        qword ptr [0000000141E04290h]
  0000000140EC14F0: jmp         0000000140EC13AF
  0000000140EC14F5: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC14FD: call        0000000140130EE0
  0000000140EC1502: movzx       eax,al
  0000000140EC1505: test        eax,eax
  0000000140EC1507: je          0000000140EC179D
  0000000140EC150D: mov         qword ptr [rsp+58h],0
  0000000140EC1516: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC151E: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC1525: mov         rcx,qword ptr [rcx+0000000000000A40h]
  0000000140EC152C: call        qword ptr [0000000141E05D68h]
  0000000140EC1532: movzx       eax,al
  0000000140EC1535: test        eax,eax
  0000000140EC1537: je          0000000140EC155E
  0000000140EC1539: mov         rax,qword ptr [rsp+00000000000000E0h]
  0000000140EC1541: mov         rax,qword ptr [rax+0000000000000140h]
  0000000140EC1548: mov         rax,qword ptr [rax+0000000000000A40h]
  0000000140EC154F: mov         qword ptr [rsp+58h],rax
  0000000140EC1554: mov         dword ptr [rsp+60h],0
  0000000140EC155C: jmp         0000000140EC15A4
  0000000140EC155E: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC1566: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC156D: mov         rcx,qword ptr [rcx+0000000000000A48h]
  0000000140EC1574: call        qword ptr [0000000141E05D68h]
  0000000140EC157A: movzx       eax,al
  0000000140EC157D: test        eax,eax
  0000000140EC157F: je          0000000140EC15A4
  0000000140EC1581: mov         rax,qword ptr [rsp+00000000000000E0h]
  0000000140EC1589: mov         rax,qword ptr [rax+0000000000000140h]
  0000000140EC1590: mov         rax,qword ptr [rax+0000000000000A48h]
  0000000140EC1597: mov         qword ptr [rsp+58h],rax
  0000000140EC159C: mov         dword ptr [rsp+60h],1
  0000000140EC15A4: call        00000001402C7ED0
  0000000140EC15A9: mov         qword ptr [rsp+50h],rax
  0000000140EC15AE: cmp         dword ptr [rsp+60h],0
  0000000140EC15B3: jne         0000000140EC15CE
  0000000140EC15B5: mov         byte ptr [rsp+78h],0
  0000000140EC15BA: lea         rdx,[rsp+78h]
  0000000140EC15BF: mov         rcx,qword ptr [rsp+50h]
  0000000140EC15C4: call        00000001400AB150
  0000000140EC15C9: jmp         0000000140EC179D
  0000000140EC15CE: mov         byte ptr [rsp+79h],1
  0000000140EC15D3: lea         rdx,[rsp+79h]
  0000000140EC15D8: mov         rcx,qword ptr [rsp+50h]
  0000000140EC15DD: call        00000001400AB150
  0000000140EC15E2: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC15EA: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC15F1: mov         rcx,qword ptr [rcx+0000000000000A00h]
  0000000140EC15F8: call        qword ptr [0000000141E05D68h]
  0000000140EC15FE: movzx       eax,al
  0000000140EC1601: test        eax,eax
  0000000140EC1603: je          0000000140EC161E
  0000000140EC1605: mov         dword ptr [rsp+7Ch],1
  0000000140EC160D: lea         rdx,[rsp+7Ch]
  0000000140EC1612: mov         rcx,qword ptr [rsp+50h]
  0000000140EC1617: call        000000014092DB70
  0000000140EC161C: jmp         0000000140EC165E
  0000000140EC161E: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC1626: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC162D: mov         rcx,qword ptr [rcx+0000000000000A08h]
  0000000140EC1634: call        qword ptr [0000000141E05D68h]
  0000000140EC163A: movzx       eax,al
  0000000140EC163D: test        eax,eax
  0000000140EC163F: je          0000000140EC165E
  0000000140EC1641: mov         dword ptr [rsp+0000000000000080h],3
  0000000140EC164C: lea         rdx,[rsp+0000000000000080h]
  0000000140EC1654: mov         rcx,qword ptr [rsp+50h]
  0000000140EC1659: call        000000014092DB70
  0000000140EC165E: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC1666: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC166D: lea         rdx,[rsp+0000000000000088h]
  0000000140EC1675: mov         rcx,qword ptr [rcx+0000000000000A10h]
  0000000140EC167C: call        qword ptr [0000000141E066B0h]
  0000000140EC1682: mov         qword ptr [rsp+00000000000000B0h],rax
  0000000140EC168A: mov         rax,qword ptr [rsp+00000000000000B0h]
  0000000140EC1692: mov         qword ptr [rsp+00000000000000B8h],rax
  0000000140EC169A: xor         edx,edx
  0000000140EC169C: mov         rcx,qword ptr [rsp+00000000000000B8h]
  0000000140EC16A4: call        qword ptr [0000000141E05810h]
  0000000140EC16AA: movsd       mmword ptr [rsp+68h],xmm0
  0000000140EC16B0: lea         rcx,[rsp+0000000000000088h]
  0000000140EC16B8: call        qword ptr [0000000141E059E0h]
  0000000140EC16BE: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140EC16C6: comisd      xmm0,mmword ptr [rsp+68h]
  0000000140EC16CC: jbe         0000000140EC16E1
  0000000140EC16CE: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140EC16D6: movsd       mmword ptr [rsp+00000000000000C0h],xmm0
  0000000140EC16DF: jmp         0000000140EC1725
  0000000140EC16E1: movsd       xmm0,mmword ptr [rsp+68h]
  0000000140EC16E7: comisd      xmm0,mmword ptr [0000000142142078h]
  0000000140EC16EF: jbe         0000000140EC1704
  0000000140EC16F1: movsd       xmm0,mmword ptr [0000000142142078h]
  0000000140EC16F9: movsd       mmword ptr [rsp+00000000000000C8h],xmm0
  0000000140EC1702: jmp         0000000140EC1713
  0000000140EC1704: movsd       xmm0,mmword ptr [rsp+68h]
  0000000140EC170A: movsd       mmword ptr [rsp+00000000000000C8h],xmm0
  0000000140EC1713: movsd       xmm0,mmword ptr [rsp+00000000000000C8h]
  0000000140EC171C: movsd       mmword ptr [rsp+00000000000000C0h],xmm0
  0000000140EC1725: movsd       xmm0,mmword ptr [rsp+00000000000000C0h]
  0000000140EC172E: movsd       mmword ptr [rsp+68h],xmm0
  0000000140EC1734: lea         rdx,[rsp+68h]
  0000000140EC1739: mov         rcx,qword ptr [rsp+50h]
  0000000140EC173E: call        0000000140DBBA90
  0000000140EC1743: mov         rcx,qword ptr [rsp+00000000000000E0h]
  0000000140EC174B: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EC1752: mov         rcx,qword ptr [rcx+0000000000000A28h]
  0000000140EC1759: call        qword ptr [0000000141E05D68h]
  0000000140EC175F: movzx       eax,al
  0000000140EC1762: test        eax,eax
  0000000140EC1764: je          0000000140EC1782
  0000000140EC1766: mov         byte ptr [rsp+0000000000000090h],0
  0000000140EC176E: lea         rdx,[rsp+0000000000000090h]
  0000000140EC1776: mov         rcx,qword ptr [rsp+50h]
  0000000140EC177B: call        0000000140E7B850
  0000000140EC1780: jmp         0000000140EC179D
  0000000140EC1782: mov         byte ptr [rsp+0000000000000091h],1
  0000000140EC178A: lea         rdx,[rsp+0000000000000091h]
  0000000140EC1792: mov         rcx,qword ptr [rsp+50h]
  0000000140EC1797: call        0000000140E7B850
  0000000140EC179C: nop
  0000000140EC179D: lea         rcx,[rsp+40h]
  0000000140EC17A2: call        qword ptr [0000000141E059E0h]
  0000000140EC17A8: add         rsp,0D8h
  0000000140EC17AF: ret
  0000000140EC17B0: int         3

  Summary

     1E02000 .text
