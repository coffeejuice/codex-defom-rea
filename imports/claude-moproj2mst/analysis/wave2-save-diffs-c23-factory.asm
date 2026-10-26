Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401CCA90: mov         qword ptr [rsp+10h],rdx
  00000001401CCA95: mov         qword ptr [rsp+8],rcx
  00000001401CCA9A: sub         rsp,0F8h
  00000001401CCAA1: mov         qword ptr [rsp+00000000000000B0h],0FFFFFFFFFFFFFFFEh
  00000001401CCAAD: mov         qword ptr [rsp+38h],0
  00000001401CCAB6: lea         rdx,[rsp+30h]
  00000001401CCABB: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCAC3: call        0000000140D4E9A0
  00000001401CCAC8: mov         rcx,qword ptr [rsp+0000000000000100h]
  00000001401CCAD0: call        00000001405FF070
  00000001401CCAD5: mov         rcx,rax
  00000001401CCAD8: call        00000001405EF2B0
  00000001401CCADD: mov         qword ptr [rsp+28h],rax
  00000001401CCAE2: cmp         qword ptr [rsp+28h],0
  00000001401CCAE8: jne         00000001401CCAF1
  00000001401CCAEA: xor         eax,eax
  00000001401CCAEC: jmp         00000001401CCE42
  00000001401CCAF1: mov         rcx,qword ptr [rsp+28h]
  00000001401CCAF6: call        000000014005F770
  00000001401CCAFB: mov         qword ptr [rsp+20h],rax
  00000001401CCB00: cmp         qword ptr [rsp+20h],0
  00000001401CCB06: jne         00000001401CCB0F
  00000001401CCB08: xor         eax,eax
  00000001401CCB0A: jmp         00000001401CCE42
  00000001401CCB0F: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCB17: call        0000000140D4EA70
  00000001401CCB1C: xor         r8d,r8d
  00000001401CCB1F: mov         edx,eax
  00000001401CCB21: mov         rcx,qword ptr [rsp+20h]
  00000001401CCB26: call        0000000140C7B1F0
  00000001401CCB2B: mov         rcx,qword ptr [rsp+0000000000000100h]
  00000001401CCB33: call        00000001401424C0
  00000001401CCB38: lea         rdx,[000000014220DF28h]
  00000001401CCB3F: lea         rcx,[rsp+40h]
  00000001401CCB44: call        qword ptr [0000000141E04110h]
  00000001401CCB4A: mov         rdx,rax
  00000001401CCB4D: lea         rcx,[rsp+30h]
  00000001401CCB52: call        0000000140142460
  00000001401CCB57: movzx       eax,al
  00000001401CCB5A: test        eax,eax
  00000001401CCB5C: je          00000001401CCB63
  00000001401CCB5E: jmp         00000001401CCDB7
  00000001401CCB63: lea         rdx,[000000014220DF30h]
  00000001401CCB6A: lea         rcx,[rsp+48h]
  00000001401CCB6F: call        qword ptr [0000000141E04110h]
  00000001401CCB75: mov         rdx,rax
  00000001401CCB78: lea         rcx,[rsp+30h]
  00000001401CCB7D: call        0000000140142460
  00000001401CCB82: movzx       eax,al
  00000001401CCB85: test        eax,eax
  00000001401CCB87: je          00000001401CCBF1
  00000001401CCB89: mov         ecx,1A8h
  00000001401CCB8E: call        000000014146EABC
  00000001401CCB93: mov         qword ptr [rsp+58h],rax
  00000001401CCB98: cmp         qword ptr [rsp+58h],0
  00000001401CCB9E: je          00000001401CCBC9
  00000001401CCBA0: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCBA8: call        0000000140D4EA70
  00000001401CCBAD: mov         r8d,eax
  00000001401CCBB0: mov         rdx,qword ptr [rsp+28h]
  00000001401CCBB5: mov         rcx,qword ptr [rsp+58h]
  00000001401CCBBA: call        00000001401E1A80
  00000001401CCBBF: mov         qword ptr [rsp+00000000000000B8h],rax
  00000001401CCBC7: jmp         00000001401CCBD5
  00000001401CCBC9: mov         qword ptr [rsp+00000000000000B8h],0
  00000001401CCBD5: mov         rax,qword ptr [rsp+00000000000000B8h]
  00000001401CCBDD: mov         qword ptr [rsp+50h],rax
  00000001401CCBE2: mov         rax,qword ptr [rsp+50h]
  00000001401CCBE7: mov         qword ptr [rsp+38h],rax
  00000001401CCBEC: jmp         00000001401CCDB7
  00000001401CCBF1: lea         rdx,[000000014220DF40h]
  00000001401CCBF8: lea         rcx,[rsp+60h]
  00000001401CCBFD: call        qword ptr [0000000141E04110h]
  00000001401CCC03: mov         rdx,rax
  00000001401CCC06: lea         rcx,[rsp+30h]
  00000001401CCC0B: call        0000000140142460
  00000001401CCC10: movzx       eax,al
  00000001401CCC13: test        eax,eax
  00000001401CCC15: je          00000001401CCC7F
  00000001401CCC17: mov         ecx,1A8h
  00000001401CCC1C: call        000000014146EABC
  00000001401CCC21: mov         qword ptr [rsp+70h],rax
  00000001401CCC26: cmp         qword ptr [rsp+70h],0
  00000001401CCC2C: je          00000001401CCC57
  00000001401CCC2E: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCC36: call        0000000140D4EA70
  00000001401CCC3B: mov         r8d,eax
  00000001401CCC3E: mov         rdx,qword ptr [rsp+28h]
  00000001401CCC43: mov         rcx,qword ptr [rsp+70h]
  00000001401CCC48: call        00000001401D9920
  00000001401CCC4D: mov         qword ptr [rsp+00000000000000C0h],rax
  00000001401CCC55: jmp         00000001401CCC63
  00000001401CCC57: mov         qword ptr [rsp+00000000000000C0h],0
  00000001401CCC63: mov         rax,qword ptr [rsp+00000000000000C0h]
  00000001401CCC6B: mov         qword ptr [rsp+68h],rax
  00000001401CCC70: mov         rax,qword ptr [rsp+68h]
  00000001401CCC75: mov         qword ptr [rsp+38h],rax
  00000001401CCC7A: jmp         00000001401CCDB7
  00000001401CCC7F: lea         rdx,[000000014220DF4Ch]
  00000001401CCC86: lea         rcx,[rsp+78h]
  00000001401CCC8B: call        qword ptr [0000000141E04110h]
  00000001401CCC91: mov         rdx,rax
  00000001401CCC94: lea         rcx,[rsp+30h]
  00000001401CCC99: call        0000000140142460
  00000001401CCC9E: movzx       eax,al
  00000001401CCCA1: test        eax,eax
  00000001401CCCA3: je          00000001401CCD1C
  00000001401CCCA5: mov         ecx,308h
  00000001401CCCAA: call        000000014146EABC
  00000001401CCCAF: mov         qword ptr [rsp+0000000000000088h],rax
  00000001401CCCB7: cmp         qword ptr [rsp+0000000000000088h],0
  00000001401CCCC0: je          00000001401CCCEE
  00000001401CCCC2: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCCCA: call        0000000140D4EA70
  00000001401CCCCF: mov         r8d,eax
  00000001401CCCD2: mov         rdx,qword ptr [rsp+28h]
  00000001401CCCD7: mov         rcx,qword ptr [rsp+0000000000000088h]
  00000001401CCCDF: call        00000001401D1B80
  00000001401CCCE4: mov         qword ptr [rsp+00000000000000C8h],rax
  00000001401CCCEC: jmp         00000001401CCCFA
  00000001401CCCEE: mov         qword ptr [rsp+00000000000000C8h],0
  00000001401CCCFA: mov         rax,qword ptr [rsp+00000000000000C8h]
  00000001401CCD02: mov         qword ptr [rsp+0000000000000080h],rax
  00000001401CCD0A: mov         rax,qword ptr [rsp+0000000000000080h]
  00000001401CCD12: mov         qword ptr [rsp+38h],rax
  00000001401CCD17: jmp         00000001401CCDB7
  00000001401CCD1C: lea         rdx,[000000014220DF58h]
  00000001401CCD23: lea         rcx,[rsp+0000000000000090h]
  00000001401CCD2B: call        qword ptr [0000000141E04110h]
  00000001401CCD31: mov         rdx,rax
  00000001401CCD34: lea         rcx,[rsp+30h]
  00000001401CCD39: call        0000000140142460
  00000001401CCD3E: movzx       eax,al
  00000001401CCD41: test        eax,eax
  00000001401CCD43: je          00000001401CCDB7
  00000001401CCD45: mov         ecx,50h
  00000001401CCD4A: call        000000014146EABC
  00000001401CCD4F: mov         qword ptr [rsp+00000000000000A0h],rax
  00000001401CCD57: cmp         qword ptr [rsp+00000000000000A0h],0
  00000001401CCD60: je          00000001401CCD8E
  00000001401CCD62: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCD6A: call        0000000140D4EA70
  00000001401CCD6F: mov         r8d,eax
  00000001401CCD72: mov         rdx,qword ptr [rsp+28h]
  00000001401CCD77: mov         rcx,qword ptr [rsp+00000000000000A0h]
  00000001401CCD7F: call        00000001401CE7C0
  00000001401CCD84: mov         qword ptr [rsp+00000000000000D0h],rax
  00000001401CCD8C: jmp         00000001401CCD9A
  00000001401CCD8E: mov         qword ptr [rsp+00000000000000D0h],0
  00000001401CCD9A: mov         rax,qword ptr [rsp+00000000000000D0h]
  00000001401CCDA2: mov         qword ptr [rsp+0000000000000098h],rax
  00000001401CCDAA: mov         rax,qword ptr [rsp+0000000000000098h]
  00000001401CCDB2: mov         qword ptr [rsp+38h],rax
  00000001401CCDB7: cmp         qword ptr [rsp+38h],0
  00000001401CCDBD: jne         00000001401CCDD9
  00000001401CCDBF: mov         rdx,qword ptr [rsp+0000000000000108h]
  00000001401CCDC7: mov         rcx,qword ptr [rsp+0000000000000100h]
  00000001401CCDCF: call        00000001405FF0B0
  00000001401CCDD4: mov         qword ptr [rsp+38h],rax
  00000001401CCDD9: mov         rcx,qword ptr [rsp+0000000000000100h]
  00000001401CCDE1: call        00000001406008E0
  00000001401CCDE6: cmp         qword ptr [rsp+38h],0
  00000001401CCDEC: je          00000001401CCE3D
  00000001401CCDEE: lea         rdx,[rsp+00000000000000A8h]
  00000001401CCDF6: mov         rcx,qword ptr [rsp+0000000000000108h]
  00000001401CCDFE: call        0000000140D4E9D0
  00000001401CCE03: mov         qword ptr [rsp+00000000000000D8h],rax
  00000001401CCE0B: mov         rax,qword ptr [rsp+00000000000000D8h]
  00000001401CCE13: mov         qword ptr [rsp+00000000000000E0h],rax
  00000001401CCE1B: mov         rdx,qword ptr [rsp+00000000000000E0h]
  00000001401CCE23: mov         rcx,qword ptr [rsp+38h]
  00000001401CCE28: call        qword ptr [0000000141E0BCE0h]
  00000001401CCE2E: nop
  00000001401CCE2F: lea         rcx,[rsp+00000000000000A8h]
  00000001401CCE37: call        qword ptr [0000000141E059E0h]
  00000001401CCE3D: mov         rax,qword ptr [rsp+38h]
  00000001401CCE42: add         rsp,0F8h
  00000001401CCE49: ret
  00000001401CCE4A: int         3

  Summary

     1E02000 .text
