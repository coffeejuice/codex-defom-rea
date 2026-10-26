Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_PRE_64.exe

File Type: EXECUTABLE IMAGE

  00000001401D0FC0: mov         byte ptr [rsp+10h],dl
  00000001401D0FC4: mov         qword ptr [rsp+8],rcx
  00000001401D0FC9: sub         rsp,218h
  00000001401D0FD0: mov         qword ptr [rsp+0000000000000208h],0FFFFFFFFFFFFFFFEh
  00000001401D0FDC: mov         dword ptr [rsp+0000000000000190h],0
  00000001401D0FE7: mov         dword ptr [rsp+00000000000001CCh],0
  00000001401D0FF2: mov         dword ptr [rsp+00000000000001C8h],0
  00000001401D0FFD: mov         qword ptr [rsp+00000000000001C0h],0
  00000001401D1009: mov         qword ptr [rsp+00000000000001B8h],0
  00000001401D1015: mov         dword ptr [rsp+0000000000000190h],0
  00000001401D1020: jmp         00000001401D1033
  00000001401D1022: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D1029: add         eax,1
  00000001401D102C: mov         dword ptr [rsp+0000000000000190h],eax
  00000001401D1033: cmp         dword ptr [rsp+0000000000000190h],4Fh
  00000001401D103B: jge         00000001401D1062
  00000001401D103D: movsxd      rdx,dword ptr [rsp+0000000000000190h]
  00000001401D1045: movsxd      rcx,dword ptr [rsp+0000000000000190h]
  00000001401D104D: mov         rax,qword ptr [rsp+0000000000000220h]
  00000001401D1055: mov         eax,dword ptr [rax+rdx*4+00000000000015F8h]
  00000001401D105C: mov         dword ptr [rsp+rcx*4+50h],eax
  00000001401D1060: jmp         00000001401D1022
  00000001401D1062: xor         edx,edx
  00000001401D1064: lea         rcx,[rsp+0000000000000198h]
  00000001401D106C: call        000000014017A6C0
  00000001401D1071: nop
  00000001401D1072: lea         rcx,[rsp+0000000000000198h]
  00000001401D107A: call        000000014017A700
  00000001401D107F: mov         rdx,qword ptr [rsp+0000000000000220h]
  00000001401D1087: lea         rcx,[rsp+0000000000000198h]
  00000001401D108F: call        00000001401D8340
  00000001401D1094: movzx       eax,byte ptr [rsp+0000000000000228h]
  00000001401D109C: test        eax,eax
  00000001401D109E: je          00000001401D10D8
  00000001401D10A0: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D10A8: add         rcx,1328h
  00000001401D10AF: call        00000001401D8FA0
  00000001401D10B4: mov         qword ptr [rsp+00000000000001C0h],rax
  00000001401D10BC: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D10C4: add         rcx,1328h
  00000001401D10CB: call        00000001401D8FB0
  00000001401D10D0: mov         qword ptr [rsp+00000000000001B8h],rax
  00000001401D10D8: mov         dword ptr [rsp+00000000000001C8h],0
  00000001401D10E3: jmp         00000001401D10F6
  00000001401D10E5: mov         eax,dword ptr [rsp+00000000000001C8h]
  00000001401D10EC: add         eax,1
  00000001401D10EF: mov         dword ptr [rsp+00000000000001C8h],eax
  00000001401D10F6: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D10FE: add         rcx,1380h
  00000001401D1105: call        000000014019A280
  00000001401D110A: cmp         dword ptr [rsp+00000000000001C8h],eax
  00000001401D1111: jge         00000001401D125F
  00000001401D1117: mov         edx,dword ptr [rsp+00000000000001C8h]
  00000001401D111E: add         edx,1
  00000001401D1121: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D1129: add         rcx,1380h
  00000001401D1130: call        000000014017AA80
  00000001401D1135: mov         qword ptr [rsp+00000000000001D0h],rax
  00000001401D113D: movzx       eax,byte ptr [rsp+0000000000000228h]
  00000001401D1145: test        eax,eax
  00000001401D1147: je          00000001401D125A
  00000001401D114D: cmp         qword ptr [rsp+00000000000001D0h],0
  00000001401D1156: je          00000001401D125A
  00000001401D115C: mov         rcx,qword ptr [rsp+00000000000001D0h]
  00000001401D1164: call        0000000140160F10
  00000001401D1169: movzx       eax,al
  00000001401D116C: test        eax,eax
  00000001401D116E: je          00000001401D125A
  00000001401D1174: cmp         qword ptr [rsp+00000000000001C0h],0
  00000001401D117D: je          00000001401D125A
  00000001401D1183: cmp         qword ptr [rsp+00000000000001B8h],0
  00000001401D118C: je          00000001401D125A
  00000001401D1192: lea         rcx,[rsp+0000000000000198h]
  00000001401D119A: call        000000014017A880
  00000001401D119F: mov         qword ptr [rsp+00000000000001D8h],rax
  00000001401D11A7: mov         byte ptr [rsp+00000000000001FCh],1
  00000001401D11AF: mov         byte ptr [rsp+00000000000001FDh],0
  00000001401D11B7: mov         byte ptr [rsp+00000000000001FEh],1
  00000001401D11BF: mov         byte ptr [rsp+00000000000001FFh],1
  00000001401D11C7: mov         byte ptr [rsp+0000000000000200h],1
  00000001401D11CF: mov         byte ptr [rsp+0000000000000201h],1
  00000001401D11D7: mov         r8,qword ptr [rsp+0000000000000220h]
  00000001401D11DF: add         r8,15F8h
  00000001401D11E6: lea         rax,[rsp+00000000000001FCh]
  00000001401D11EE: mov         qword ptr [rsp+40h],rax
  00000001401D11F3: lea         rax,[rsp+00000000000001FDh]
  00000001401D11FB: mov         qword ptr [rsp+38h],rax
  00000001401D1200: lea         rax,[rsp+00000000000001FEh]
  00000001401D1208: mov         qword ptr [rsp+30h],rax
  00000001401D120D: lea         rax,[rsp+00000000000001FFh]
  00000001401D1215: mov         qword ptr [rsp+28h],rax
  00000001401D121A: lea         rax,[rsp+0000000000000200h]
  00000001401D1222: mov         qword ptr [rsp+20h],rax
  00000001401D1227: lea         r9,[rsp+0000000000000201h]
  00000001401D122F: mov         rdx,qword ptr [rsp+00000000000001D0h]
  00000001401D1237: mov         rcx,qword ptr [rsp+00000000000001D8h]
  00000001401D123F: call        00000001401613A0
  00000001401D1244: mov         rcx,qword ptr [rsp+00000000000001D8h]
  00000001401D124C: mov         rax,qword ptr [rsp+00000000000001D0h]
  00000001401D1254: mov         eax,dword ptr [rax+8]
  00000001401D1257: mov         dword ptr [rcx+8],eax
  00000001401D125A: jmp         00000001401D10E5
  00000001401D125F: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D1267: call        00000001401D0320
  00000001401D126C: mov         dword ptr [rsp+00000000000001C8h],0
  00000001401D1277: jmp         00000001401D128A
  00000001401D1279: mov         eax,dword ptr [rsp+00000000000001C8h]
  00000001401D1280: add         eax,1
  00000001401D1283: mov         dword ptr [rsp+00000000000001C8h],eax
  00000001401D128A: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D1292: add         rcx,1380h
  00000001401D1299: call        000000014019A280
  00000001401D129E: cmp         dword ptr [rsp+00000000000001C8h],eax
  00000001401D12A5: jge         00000001401D1636
  00000001401D12AB: mov         edx,dword ptr [rsp+00000000000001C8h]
  00000001401D12B2: add         edx,1
  00000001401D12B5: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D12BD: add         rcx,1380h
  00000001401D12C4: call        000000014017AA80
  00000001401D12C9: mov         qword ptr [rsp+00000000000001E0h],rax
  00000001401D12D1: cmp         qword ptr [rsp+00000000000001E0h],0
  00000001401D12DA: je          00000001401D1631
  00000001401D12E0: mov         rcx,qword ptr [rsp+00000000000001E0h]
  00000001401D12E8: call        0000000140160F10
  00000001401D12ED: movzx       eax,al
  00000001401D12F0: test        eax,eax
  00000001401D12F2: je          00000001401D1631
  00000001401D12F8: mov         rdx,qword ptr [rsp+0000000000000220h]
  00000001401D1300: add         rdx,15F8h
  00000001401D1307: lea         r8,[rsp+50h]
  00000001401D130C: mov         rcx,qword ptr [rsp+00000000000001E0h]
  00000001401D1314: call        0000000140199650
  00000001401D1319: movzx       eax,byte ptr [rsp+0000000000000228h]
  00000001401D1321: test        eax,eax
  00000001401D1323: je          00000001401D1631
  00000001401D1329: cmp         qword ptr [rsp+00000000000001C0h],0
  00000001401D1332: je          00000001401D1631
  00000001401D1338: cmp         qword ptr [rsp+00000000000001B8h],0
  00000001401D1341: je          00000001401D1631
  00000001401D1347: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D134F: mov         edx,dword ptr [rax+8]
  00000001401D1352: lea         rcx,[rsp+0000000000000198h]
  00000001401D135A: call        000000014017AAF0
  00000001401D135F: mov         qword ptr [rsp+00000000000001E8h],rax
  00000001401D1367: cmp         qword ptr [rsp+00000000000001E8h],0
  00000001401D1370: je          00000001401D1631
  00000001401D1376: xor         r8d,r8d
  00000001401D1379: mov         edx,31h
  00000001401D137E: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D1386: call        00000001401D64F0
  00000001401D138B: movzx       eax,al
  00000001401D138E: test        eax,eax
  00000001401D1390: je          00000001401D1631
  00000001401D1396: mov         dword ptr [rsp+00000000000001CCh],0
  00000001401D13A1: jmp         00000001401D13B4
  00000001401D13A3: mov         eax,dword ptr [rsp+00000000000001CCh]
  00000001401D13AA: add         eax,1
  00000001401D13AD: mov         dword ptr [rsp+00000000000001CCh],eax
  00000001401D13B4: mov         rax,qword ptr [rsp+0000000000000220h]
  00000001401D13BC: mov         eax,dword ptr [rax+00000000000016BCh]
  00000001401D13C2: cmp         dword ptr [rsp+00000000000001CCh],eax
  00000001401D13C9: jge         00000001401D147E
  00000001401D13CF: mov         dword ptr [rsp+0000000000000190h],0
  00000001401D13DA: jmp         00000001401D13ED
  00000001401D13DC: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D13E3: add         eax,1
  00000001401D13E6: mov         dword ptr [rsp+0000000000000190h],eax
  00000001401D13ED: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D13F5: mov         eax,dword ptr [rax+0000000000000884h]
  00000001401D13FB: cmp         dword ptr [rsp+0000000000000190h],eax
  00000001401D1402: jge         00000001401D1479
  00000001401D1404: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D140C: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D1413: imul        eax,dword ptr [rcx+00000000000016BCh]
  00000001401D141A: add         eax,dword ptr [rsp+00000000000001CCh]
  00000001401D1421: movsxd      rcx,eax
  00000001401D1424: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D142C: mov         rax,qword ptr [rax+0000000000000A58h]
  00000001401D1433: xorpd       xmm0,xmm0
  00000001401D1437: movsd       mmword ptr [rax+rcx*8],xmm0
  00000001401D143C: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D1444: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D144B: imul        eax,dword ptr [rcx+00000000000016BCh]
  00000001401D1452: add         eax,dword ptr [rsp+00000000000001CCh]
  00000001401D1459: movsxd      rcx,eax
  00000001401D145C: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D1464: mov         rax,qword ptr [rax+0000000000000A60h]
  00000001401D146B: xorpd       xmm0,xmm0
  00000001401D146F: movsd       mmword ptr [rax+rcx*8],xmm0
  00000001401D1474: jmp         00000001401D13DC
  00000001401D1479: jmp         00000001401D13A3
  00000001401D147E: mov         dword ptr [rsp+00000000000001CCh],0
  00000001401D1489: jmp         00000001401D149C
  00000001401D148B: mov         eax,dword ptr [rsp+00000000000001CCh]
  00000001401D1492: add         eax,1
  00000001401D1495: mov         dword ptr [rsp+00000000000001CCh],eax
  00000001401D149C: mov         eax,dword ptr [rsp+0000000000000114h]
  00000001401D14A3: cmp         dword ptr [rsp+00000000000001CCh],eax
  00000001401D14AA: jge         00000001401D1631
  00000001401D14B0: movsxd      rcx,dword ptr [rsp+00000000000001CCh]
  00000001401D14B8: mov         rax,qword ptr [rsp+00000000000001C0h]
  00000001401D14C0: mov         eax,dword ptr [rax+rcx*4]
  00000001401D14C3: mov         dword ptr [rsp+00000000000001F8h],eax
  00000001401D14CA: movsxd      rcx,dword ptr [rsp+00000000000001CCh]
  00000001401D14D2: mov         rax,qword ptr [rsp+00000000000001B8h]
  00000001401D14DA: mov         eax,dword ptr [rax+rcx*4]
  00000001401D14DD: mov         dword ptr [rsp+00000000000001F4h],eax
  00000001401D14E4: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D14EC: add         rcx,1328h
  00000001401D14F3: mov         edx,dword ptr [rsp+00000000000001F8h]
  00000001401D14FA: call        000000014029C870
  00000001401D14FF: mov         dword ptr [rsp+00000000000001F0h],eax
  00000001401D1506: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D150E: add         rcx,1328h
  00000001401D1515: mov         edx,dword ptr [rsp+00000000000001F8h]
  00000001401D151C: call        000000014029C7F0
  00000001401D1521: test        eax,eax
  00000001401D1523: jge         00000001401D152A
  00000001401D1525: jmp         00000001401D148B
  00000001401D152A: mov         dword ptr [rsp+0000000000000190h],0
  00000001401D1535: jmp         00000001401D1548
  00000001401D1537: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D153E: add         eax,1
  00000001401D1541: mov         dword ptr [rsp+0000000000000190h],eax
  00000001401D1548: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D1550: mov         eax,dword ptr [rax+0000000000000884h]
  00000001401D1556: cmp         dword ptr [rsp+0000000000000190h],eax
  00000001401D155D: jge         00000001401D162C
  00000001401D1563: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D156A: imul        eax,dword ptr [rsp+0000000000000114h]
  00000001401D1572: add         eax,dword ptr [rsp+00000000000001F4h]
  00000001401D1579: movsxd      r8,eax
  00000001401D157C: mov         rdx,qword ptr [rsp+00000000000001E8h]
  00000001401D1584: mov         rdx,qword ptr [rdx+0000000000000A58h]
  00000001401D158B: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D1593: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D159A: imul        eax,dword ptr [rcx+00000000000016BCh]
  00000001401D15A1: add         eax,dword ptr [rsp+00000000000001F0h]
  00000001401D15A8: movsxd      rcx,eax
  00000001401D15AB: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D15B3: mov         rax,qword ptr [rax+0000000000000A58h]
  00000001401D15BA: movsd       xmm0,mmword ptr [rdx+r8*8]
  00000001401D15C0: movsd       mmword ptr [rax+rcx*8],xmm0
  00000001401D15C5: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D15CC: imul        eax,dword ptr [rsp+0000000000000114h]
  00000001401D15D4: add         eax,dword ptr [rsp+00000000000001F4h]
  00000001401D15DB: movsxd      r8,eax
  00000001401D15DE: mov         rdx,qword ptr [rsp+00000000000001E8h]
  00000001401D15E6: mov         rdx,qword ptr [rdx+0000000000000A60h]
  00000001401D15ED: mov         rcx,qword ptr [rsp+0000000000000220h]
  00000001401D15F5: mov         eax,dword ptr [rsp+0000000000000190h]
  00000001401D15FC: imul        eax,dword ptr [rcx+00000000000016BCh]
  00000001401D1603: add         eax,dword ptr [rsp+00000000000001F0h]
  00000001401D160A: movsxd      rcx,eax
  00000001401D160D: mov         rax,qword ptr [rsp+00000000000001E0h]
  00000001401D1615: mov         rax,qword ptr [rax+0000000000000A60h]
  00000001401D161C: movsd       xmm0,mmword ptr [rdx+r8*8]
  00000001401D1622: movsd       mmword ptr [rax+rcx*8],xmm0
  00000001401D1627: jmp         00000001401D1537
  00000001401D162C: jmp         00000001401D148B
  00000001401D1631: jmp         00000001401D1279
  00000001401D1636: lea         rcx,[rsp+0000000000000198h]
  00000001401D163E: call        000000014017AB70
  00000001401D1643: nop
  00000001401D1644: lea         rcx,[rsp+0000000000000198h]
  00000001401D164C: call        000000014017A740
  00000001401D1651: add         rsp,218h
  00000001401D1658: ret
  00000001401D1659: int         3

  Summary

      504000 .text
