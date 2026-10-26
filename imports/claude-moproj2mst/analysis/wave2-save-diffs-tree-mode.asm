Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001405EEBB0: mov         byte ptr [rsp+20h],r9b
  00000001405EEBB5: mov         byte ptr [rsp+18h],r8b
  00000001405EEBBA: mov         byte ptr [rsp+10h],dl
  00000001405EEBBE: mov         qword ptr [rsp+8],rcx
  00000001405EEBC3: sub         rsp,98h
  00000001405EEBCA: mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
  00000001405EEBD3: mov         rcx,qword ptr [rsp+00000000000000A0h]
  00000001405EEBDB: call        00000001405EF3A0
  00000001405EEBE0: mov         qword ptr [rsp+30h],rax
  00000001405EEBE5: cmp         qword ptr [rsp+30h],0
  00000001405EEBEB: jne         00000001405EEBF2
  00000001405EEBED: jmp         00000001405EEED0
  00000001405EEBF2: mov         rcx,qword ptr [rsp+00000000000000A0h]
  00000001405EEBFA: call        00000001405EF2B0
  00000001405EEBFF: mov         qword ptr [rsp+20h],rax
  00000001405EEC04: cmp         qword ptr [rsp+20h],0
  00000001405EEC0A: jne         00000001405EEC11
  00000001405EEC0C: jmp         00000001405EEED0
  00000001405EEC11: lea         rcx,[0000000143D768F0h]
  00000001405EEC18: call        0000000140040C40
  00000001405EEC1D: mov         byte ptr [rsp+38h],al
  00000001405EEC21: lea         rdx,[rsp+48h]
  00000001405EEC26: mov         rcx,qword ptr [rsp+20h]
  00000001405EEC2B: call        0000000140050D00
  00000001405EEC30: nop
  00000001405EEC31: mov         rcx,qword ptr [rsp+20h]
  00000001405EEC36: call        000000014005F7E0
  00000001405EEC3B: mov         qword ptr [rsp+40h],rax
  00000001405EEC40: mov         rcx,qword ptr [rsp+30h]
  00000001405EEC45: call        00000001400C3A50
  00000001405EEC4A: mov         dword ptr [rsp+28h],eax
  00000001405EEC4E: mov         dword ptr [rsp+50h],0
  00000001405EEC56: jmp         00000001405EEC63
  00000001405EEC58: mov         eax,dword ptr [rsp+50h]
  00000001405EEC5C: add         eax,1
  00000001405EEC5F: mov         dword ptr [rsp+50h],eax
  00000001405EEC63: mov         eax,dword ptr [rsp+28h]
  00000001405EEC67: cmp         dword ptr [rsp+50h],eax
  00000001405EEC6B: jge         00000001405EEEC5
  00000001405EEC71: mov         edx,dword ptr [rsp+50h]
  00000001405EEC75: add         edx,1
  00000001405EEC78: mov         rcx,qword ptr [rsp+30h]
  00000001405EEC7D: call        00000001400C3CB0
  00000001405EEC82: mov         qword ptr [rsp+58h],rax
  00000001405EEC87: cmp         qword ptr [rsp+58h],0
  00000001405EEC8D: jne         00000001405EEC91
  00000001405EEC8F: jmp         00000001405EEC58
  00000001405EEC91: mov         rcx,qword ptr [rsp+58h]
  00000001405EEC96: call        00000001400C04E0
  00000001405EEC9B: mov         qword ptr [rsp+60h],rax
  00000001405EECA0: cmp         qword ptr [rsp+60h],0
  00000001405EECA6: jne         00000001405EECAA
  00000001405EECA8: jmp         00000001405EEC58
  00000001405EECAA: movzx       eax,byte ptr [rsp+00000000000000A8h]
  00000001405EECB2: test        eax,eax
  00000001405EECB4: je          00000001405EEDBA
  00000001405EECBA: mov         rax,qword ptr [rsp+60h]
  00000001405EECBF: mov         edx,dword ptr [rax+8]
  00000001405EECC2: mov         rcx,qword ptr [rsp+20h]
  00000001405EECC7: call        00000001400612D0
  00000001405EECCC: mov         qword ptr [rsp+70h],rax
  00000001405EECD1: cmp         qword ptr [rsp+70h],0
  00000001405EECD7: je          00000001405EECEC
  00000001405EECD9: mov         rcx,qword ptr [rsp+70h]
  00000001405EECDE: call        000000014004D660
  00000001405EECE3: mov         dword ptr [rsp+0000000000000080h],eax
  00000001405EECEA: jmp         00000001405EECFB
  00000001405EECEC: mov         rax,qword ptr [rsp+60h]
  00000001405EECF1: mov         eax,dword ptr [rax+0Ch]
  00000001405EECF4: mov         dword ptr [rsp+0000000000000080h],eax
  00000001405EECFB: mov         eax,dword ptr [rsp+0000000000000080h]
  00000001405EED02: mov         dword ptr [rsp+68h],eax
  00000001405EED06: mov         rax,qword ptr [rsp+60h]
  00000001405EED0B: cmp         dword ptr [rax+0Ch],5
  00000001405EED0F: jne         00000001405EED8F
  00000001405EED11: lea         rdx,[00000001424973C0h]
  00000001405EED18: lea         rcx,[rsp+48h]
  00000001405EED1D: call        qword ptr [0000000141E04468h]
  00000001405EED23: movzx       eax,al
  00000001405EED26: test        eax,eax
  00000001405EED28: jne         00000001405EED43
  00000001405EED2A: lea         rdx,[00000001424973D0h]
  00000001405EED31: lea         rcx,[rsp+48h]
  00000001405EED36: call        qword ptr [0000000141E04468h]
  00000001405EED3C: movzx       eax,al
  00000001405EED3F: test        eax,eax
  00000001405EED41: je          00000001405EED5A
  00000001405EED43: xor         r8d,r8d
  00000001405EED46: mov         rax,qword ptr [rsp+60h]
  00000001405EED4B: mov         edx,dword ptr [rax+8]
  00000001405EED4E: mov         rcx,qword ptr [rsp+30h]
  00000001405EED53: call        00000001400C4370
  00000001405EED58: jmp         00000001405EED8D
  00000001405EED5A: cmp         dword ptr [rsp+68h],0
  00000001405EED5F: jne         00000001405EED78
  00000001405EED61: xor         r8d,r8d
  00000001405EED64: mov         rax,qword ptr [rsp+60h]
  00000001405EED69: mov         edx,dword ptr [rax+8]
  00000001405EED6C: mov         rcx,qword ptr [rsp+30h]
  00000001405EED71: call        00000001400C4370
  00000001405EED76: jmp         00000001405EED8D
  00000001405EED78: mov         r8b,1
  00000001405EED7B: mov         rax,qword ptr [rsp+60h]
  00000001405EED80: mov         edx,dword ptr [rax+8]
  00000001405EED83: mov         rcx,qword ptr [rsp+30h]
  00000001405EED88: call        00000001400C4370
  00000001405EED8D: jmp         00000001405EEDBA
  00000001405EED8F: cmp         qword ptr [rsp+40h],0
  00000001405EED95: je          00000001405EEDBA
  00000001405EED97: mov         rcx,qword ptr [rsp+40h]
  00000001405EED9C: call        00000001408DFD60
  00000001405EEDA1: test        eax,eax
  00000001405EEDA3: je          00000001405EEDBA
  00000001405EEDA5: mov         r8b,1
  00000001405EEDA8: mov         rax,qword ptr [rsp+60h]
  00000001405EEDAD: mov         edx,dword ptr [rax+8]
  00000001405EEDB0: mov         rcx,qword ptr [rsp+30h]
  00000001405EEDB5: call        00000001400C4370
  00000001405EEDBA: movzx       eax,byte ptr [rsp+00000000000000B0h]
  00000001405EEDC2: test        eax,eax
  00000001405EEDC4: je          00000001405EEE3D
  00000001405EEDC6: mov         rax,qword ptr [rsp+00000000000000A0h]
  00000001405EEDCE: mov         rax,qword ptr [rax]
  00000001405EEDD1: mov         rcx,qword ptr [rsp+00000000000000A0h]
  00000001405EEDD9: call        qword ptr [rax+00000000000001E8h]
  00000001405EEDDF: movzx       eax,al
  00000001405EEDE2: test        eax,eax
  00000001405EEDE4: je          00000001405EEE28
  00000001405EEDE6: movzx       eax,byte ptr [rsp+38h]
  00000001405EEDEB: test        eax,eax
  00000001405EEDED: je          00000001405EEE28
  00000001405EEDEF: mov         rax,qword ptr [rsp+60h]
  00000001405EEDF4: cmp         dword ptr [rax+0Ch],5
  00000001405EEDF8: jne         00000001405EEE11
  00000001405EEDFA: xor         r8d,r8d
  00000001405EEDFD: mov         rax,qword ptr [rsp+60h]
  00000001405EEE02: mov         edx,dword ptr [rax+8]
  00000001405EEE05: mov         rcx,qword ptr [rsp+30h]
  00000001405EEE0A: call        00000001400C42B0
  00000001405EEE0F: jmp         00000001405EEE26
  00000001405EEE11: mov         r8b,1
  00000001405EEE14: mov         rax,qword ptr [rsp+60h]
  00000001405EEE19: mov         edx,dword ptr [rax+8]
  00000001405EEE1C: mov         rcx,qword ptr [rsp+30h]
  00000001405EEE21: call        00000001400C42B0
  00000001405EEE26: jmp         00000001405EEE3D
  00000001405EEE28: mov         r8b,1
  00000001405EEE2B: mov         rax,qword ptr [rsp+60h]
  00000001405EEE30: mov         edx,dword ptr [rax+8]
  00000001405EEE33: mov         rcx,qword ptr [rsp+30h]
  00000001405EEE38: call        00000001400C42B0
  00000001405EEE3D: movzx       eax,byte ptr [rsp+00000000000000B8h]
  00000001405EEE45: test        eax,eax
  00000001405EEE47: je          00000001405EEEC0
  00000001405EEE49: mov         rax,qword ptr [rsp+00000000000000A0h]
  00000001405EEE51: mov         rax,qword ptr [rax]
  00000001405EEE54: mov         rcx,qword ptr [rsp+00000000000000A0h]
  00000001405EEE5C: call        qword ptr [rax+00000000000001E8h]
  00000001405EEE62: movzx       eax,al
  00000001405EEE65: test        eax,eax
  00000001405EEE67: je          00000001405EEEAB
  00000001405EEE69: movzx       eax,byte ptr [rsp+38h]
  00000001405EEE6E: test        eax,eax
  00000001405EEE70: je          00000001405EEEAB
  00000001405EEE72: mov         rax,qword ptr [rsp+60h]
  00000001405EEE77: cmp         dword ptr [rax+0Ch],5
  00000001405EEE7B: jne         00000001405EEE94
  00000001405EEE7D: xor         r8d,r8d
  00000001405EEE80: mov         rax,qword ptr [rsp+60h]
  00000001405EEE85: mov         edx,dword ptr [rax+8]
  00000001405EEE88: mov         rcx,qword ptr [rsp+30h]
  00000001405EEE8D: call        00000001400C4310
  00000001405EEE92: jmp         00000001405EEEA9
  00000001405EEE94: mov         r8b,1
  00000001405EEE97: mov         rax,qword ptr [rsp+60h]
  00000001405EEE9C: mov         edx,dword ptr [rax+8]
  00000001405EEE9F: mov         rcx,qword ptr [rsp+30h]
  00000001405EEEA4: call        00000001400C4310
  00000001405EEEA9: jmp         00000001405EEEC0
  00000001405EEEAB: mov         r8b,1
  00000001405EEEAE: mov         rax,qword ptr [rsp+60h]
  00000001405EEEB3: mov         edx,dword ptr [rax+8]
  00000001405EEEB6: mov         rcx,qword ptr [rsp+30h]
  00000001405EEEBB: call        00000001400C4310
  00000001405EEEC0: jmp         00000001405EEC58
  00000001405EEEC5: lea         rcx,[rsp+48h]
  00000001405EEECA: call        qword ptr [0000000141E059E0h]
  00000001405EEED0: add         rsp,98h
  00000001405EEED7: ret
  00000001405EEED8: int         3

  Summary

     1E02000 .text
