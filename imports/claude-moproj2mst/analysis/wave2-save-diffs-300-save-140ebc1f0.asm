Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EBC1F0: mov         qword ptr [rsp+10h],rdx
  0000000140EBC1F5: mov         qword ptr [rsp+8],rcx
  0000000140EBC1FA: sub         rsp,228h
  0000000140EBC201: mov         qword ptr [rsp+00000000000000F8h],0FFFFFFFFFFFFFFFEh
  0000000140EBC20D: mov         dword ptr [rsp+58h],0
  0000000140EBC215: mov         dword ptr [rsp+20h],0
  0000000140EBC21D: lea         rcx,[rsp+60h]
  0000000140EBC222: call        qword ptr [0000000141E044C0h]
  0000000140EBC228: nop
  0000000140EBC229: mov         rdx,qword ptr [rsp+0000000000000230h]
  0000000140EBC231: lea         rcx,[rsp+30h]
  0000000140EBC236: call        qword ptr [0000000141E08098h]
  0000000140EBC23C: nop
  0000000140EBC23D: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC245: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC24C: mov         rcx,qword ptr [rcx+00000000000002F0h]
  0000000140EBC253: call        qword ptr [0000000141E05D68h]
  0000000140EBC259: movzx       eax,al
  0000000140EBC25C: test        eax,eax
  0000000140EBC25E: je          0000000140EBC26A
  0000000140EBC260: mov         dword ptr [rsp+20h],0
  0000000140EBC268: jmp         0000000140EBC2C2
  0000000140EBC26A: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC272: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC279: mov         rcx,qword ptr [rcx+00000000000002F8h]
  0000000140EBC280: call        qword ptr [0000000141E05D68h]
  0000000140EBC286: movzx       eax,al
  0000000140EBC289: test        eax,eax
  0000000140EBC28B: je          0000000140EBC297
  0000000140EBC28D: mov         dword ptr [rsp+20h],1
  0000000140EBC295: jmp         0000000140EBC2C2
  0000000140EBC297: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC29F: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC2A6: mov         rcx,qword ptr [rcx+0000000000000300h]
  0000000140EBC2AD: call        qword ptr [0000000141E05D68h]
  0000000140EBC2B3: movzx       eax,al
  0000000140EBC2B6: test        eax,eax
  0000000140EBC2B8: je          0000000140EBC2C2
  0000000140EBC2BA: mov         dword ptr [rsp+20h],2
  0000000140EBC2C2: cmp         dword ptr [rsp+20h],2
  0000000140EBC2C7: jne         0000000140EBC2E5
  0000000140EBC2C9: mov         dword ptr [rsp+6Ch],1
  0000000140EBC2D1: lea         rdx,[rsp+6Ch]
  0000000140EBC2D6: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBC2DE: call        00000001408DA6F0
  0000000140EBC2E3: jmp         0000000140EBC2F7
  0000000140EBC2E5: lea         rdx,[rsp+20h]
  0000000140EBC2EA: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBC2F2: call        00000001408DA6F0
  0000000140EBC2F7: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBC2FF: call        00000001408DA6D0
  0000000140EBC304: cmp         eax,1
  0000000140EBC307: jne         0000000140EBC3D4
  0000000140EBC30D: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC315: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC31C: lea         rdx,[rsp+70h]
  0000000140EBC321: mov         rcx,qword ptr [rcx+0000000000000248h]
  0000000140EBC328: call        qword ptr [0000000141E066B0h]
  0000000140EBC32E: mov         qword ptr [rsp+0000000000000100h],rax
  0000000140EBC336: mov         rax,qword ptr [rsp+0000000000000100h]
  0000000140EBC33E: mov         qword ptr [rsp+0000000000000108h],rax
  0000000140EBC346: mov         rdx,qword ptr [rsp+0000000000000108h]
  0000000140EBC34E: lea         rcx,[rsp+60h]
  0000000140EBC353: call        qword ptr [0000000141E04470h]
  0000000140EBC359: nop
  0000000140EBC35A: lea         rcx,[rsp+70h]
  0000000140EBC35F: call        qword ptr [0000000141E059E0h]
  0000000140EBC365: lea         r8,[rsp+58h]
  0000000140EBC36A: lea         rdx,[rsp+60h]
  0000000140EBC36F: lea         rcx,[rsp+30h]
  0000000140EBC374: call        qword ptr [0000000141E080B8h]
  0000000140EBC37A: mov         dword ptr [rsp+24h],eax
  0000000140EBC37E: cmp         dword ptr [rsp+24h],0
  0000000140EBC383: jne         0000000140EBC3A1
  0000000140EBC385: lea         rcx,[rsp+30h]
  0000000140EBC38A: call        qword ptr [0000000141E080D0h]
  0000000140EBC390: nop
  0000000140EBC391: lea         rcx,[rsp+60h]
  0000000140EBC396: call        qword ptr [0000000141E059E0h]
  0000000140EBC39C: jmp         0000000140EBD170
  0000000140EBC3A1: xor         edx,edx
  0000000140EBC3A3: lea         rcx,[rsp+60h]
  0000000140EBC3A8: call        qword ptr [0000000141E05810h]
  0000000140EBC3AE: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC3B6: movsd       mmword ptr [rax+00000000000001D0h],xmm0
  0000000140EBC3BE: xorpd       xmm1,xmm1
  0000000140EBC3C2: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBC3CA: call        00000001405FD4B0
  0000000140EBC3CF: jmp         0000000140EBC49B
  0000000140EBC3D4: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC3DC: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC3E3: lea         rdx,[rsp+78h]
  0000000140EBC3E8: mov         rcx,qword ptr [rcx+0000000000000200h]
  0000000140EBC3EF: call        qword ptr [0000000141E066B0h]
  0000000140EBC3F5: mov         qword ptr [rsp+0000000000000110h],rax
  0000000140EBC3FD: mov         rax,qword ptr [rsp+0000000000000110h]
  0000000140EBC405: mov         qword ptr [rsp+0000000000000118h],rax
  0000000140EBC40D: mov         rdx,qword ptr [rsp+0000000000000118h]
  0000000140EBC415: lea         rcx,[rsp+60h]
  0000000140EBC41A: call        qword ptr [0000000141E04470h]
  0000000140EBC420: nop
  0000000140EBC421: lea         rcx,[rsp+78h]
  0000000140EBC426: call        qword ptr [0000000141E059E0h]
  0000000140EBC42C: lea         r8,[rsp+58h]
  0000000140EBC431: lea         rdx,[rsp+60h]
  0000000140EBC436: lea         rcx,[rsp+30h]
  0000000140EBC43B: call        qword ptr [0000000141E080B8h]
  0000000140EBC441: mov         dword ptr [rsp+24h],eax
  0000000140EBC445: cmp         dword ptr [rsp+24h],0
  0000000140EBC44A: jne         0000000140EBC468
  0000000140EBC44C: lea         rcx,[rsp+30h]
  0000000140EBC451: call        qword ptr [0000000141E080D0h]
  0000000140EBC457: nop
  0000000140EBC458: lea         rcx,[rsp+60h]
  0000000140EBC45D: call        qword ptr [0000000141E059E0h]
  0000000140EBC463: jmp         0000000140EBD170
  0000000140EBC468: xor         edx,edx
  0000000140EBC46A: lea         rcx,[rsp+60h]
  0000000140EBC46F: call        qword ptr [0000000141E05810h]
  0000000140EBC475: movapd      xmm1,xmm0
  0000000140EBC479: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBC481: call        00000001405FD4B0
  0000000140EBC486: mov         r11,qword ptr [rsp+0000000000000238h]
  0000000140EBC48E: xorpd       xmm0,xmm0
  0000000140EBC492: movsd       mmword ptr [r11+00000000000001D0h],xmm0
  0000000140EBC49B: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC4A3: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC4AA: mov         rcx,qword ptr [rcx+0000000000000318h]
  0000000140EBC4B1: call        qword ptr [0000000141E05D68h]
  0000000140EBC4B7: movzx       eax,al
  0000000140EBC4BA: test        eax,eax
  0000000140EBC4BC: je          0000000140EBC4D2
  0000000140EBC4BE: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC4C6: mov         dword ptr [rax+00000000000001C8h],1
  0000000140EBC4D0: jmp         0000000140EBC507
  0000000140EBC4D2: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC4DA: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC4E1: mov         rcx,qword ptr [rcx+0000000000000320h]
  0000000140EBC4E8: call        qword ptr [0000000141E05D68h]
  0000000140EBC4EE: movzx       eax,al
  0000000140EBC4F1: test        eax,eax
  0000000140EBC4F3: je          0000000140EBC507
  0000000140EBC4F5: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC4FD: mov         dword ptr [rax+00000000000001C8h],2
  0000000140EBC507: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC50F: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC516: mov         rcx,qword ptr [rcx+0000000000000300h]
  0000000140EBC51D: call        qword ptr [0000000141E05D68h]
  0000000140EBC523: movzx       eax,al
  0000000140EBC526: test        eax,eax
  0000000140EBC528: je          0000000140EBC53C
  0000000140EBC52A: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC532: mov         dword ptr [rax+00000000000001C8h],3
  0000000140EBC53C: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC544: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC54B: lea         rdx,[rsp+0000000000000080h]
  0000000140EBC553: mov         rcx,qword ptr [rcx+0000000000000348h]
  0000000140EBC55A: call        qword ptr [0000000141E066B0h]
  0000000140EBC560: mov         qword ptr [rsp+0000000000000120h],rax
  0000000140EBC568: mov         rax,qword ptr [rsp+0000000000000120h]
  0000000140EBC570: mov         qword ptr [rsp+0000000000000128h],rax
  0000000140EBC578: mov         rdx,qword ptr [rsp+0000000000000128h]
  0000000140EBC580: lea         rcx,[rsp+60h]
  0000000140EBC585: call        qword ptr [0000000141E04470h]
  0000000140EBC58B: nop
  0000000140EBC58C: lea         rcx,[rsp+0000000000000080h]
  0000000140EBC594: call        qword ptr [0000000141E059E0h]
  0000000140EBC59A: lea         r8,[rsp+58h]
  0000000140EBC59F: lea         rdx,[rsp+60h]
  0000000140EBC5A4: lea         rcx,[rsp+30h]
  0000000140EBC5A9: call        qword ptr [0000000141E080B8h]
  0000000140EBC5AF: mov         dword ptr [rsp+24h],eax
  0000000140EBC5B3: cmp         dword ptr [rsp+24h],0
  0000000140EBC5B8: jne         0000000140EBC5D6
  0000000140EBC5BA: lea         rcx,[rsp+30h]
  0000000140EBC5BF: call        qword ptr [0000000141E080D0h]
  0000000140EBC5C5: nop
  0000000140EBC5C6: lea         rcx,[rsp+60h]
  0000000140EBC5CB: call        qword ptr [0000000141E059E0h]
  0000000140EBC5D1: jmp         0000000140EBD170
  0000000140EBC5D6: xor         edx,edx
  0000000140EBC5D8: lea         rcx,[rsp+60h]
  0000000140EBC5DD: call        qword ptr [0000000141E05810h]
  0000000140EBC5E3: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC5EB: movsd       mmword ptr [rax+0000000000000260h],xmm0
  0000000140EBC5F3: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC5FB: call        0000000140130EE0
  0000000140EBC600: movzx       eax,al
  0000000140EBC603: test        eax,eax
  0000000140EBC605: je          0000000140EBC6C2
  0000000140EBC60B: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC613: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC61A: lea         rdx,[rsp+0000000000000088h]
  0000000140EBC622: mov         rcx,qword ptr [rcx+00000000000003C0h]
  0000000140EBC629: call        qword ptr [0000000141E066B0h]
  0000000140EBC62F: mov         qword ptr [rsp+0000000000000130h],rax
  0000000140EBC637: mov         rax,qword ptr [rsp+0000000000000130h]
  0000000140EBC63F: mov         qword ptr [rsp+0000000000000138h],rax
  0000000140EBC647: mov         rdx,qword ptr [rsp+0000000000000138h]
  0000000140EBC64F: lea         rcx,[rsp+60h]
  0000000140EBC654: call        qword ptr [0000000141E04470h]
  0000000140EBC65A: nop
  0000000140EBC65B: lea         rcx,[rsp+0000000000000088h]
  0000000140EBC663: call        qword ptr [0000000141E059E0h]
  0000000140EBC669: lea         r8,[rsp+58h]
  0000000140EBC66E: lea         rdx,[rsp+60h]
  0000000140EBC673: lea         rcx,[rsp+30h]
  0000000140EBC678: call        qword ptr [0000000141E080B8h]
  0000000140EBC67E: mov         dword ptr [rsp+24h],eax
  0000000140EBC682: cmp         dword ptr [rsp+24h],0
  0000000140EBC687: jne         0000000140EBC6A5
  0000000140EBC689: lea         rcx,[rsp+30h]
  0000000140EBC68E: call        qword ptr [0000000141E080D0h]
  0000000140EBC694: nop
  0000000140EBC695: lea         rcx,[rsp+60h]
  0000000140EBC69A: call        qword ptr [0000000141E059E0h]
  0000000140EBC6A0: jmp         0000000140EBD170
  0000000140EBC6A5: xor         edx,edx
  0000000140EBC6A7: lea         rcx,[rsp+60h]
  0000000140EBC6AC: call        qword ptr [0000000141E05810h]
  0000000140EBC6B2: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC6BA: movsd       mmword ptr [rax+00000000000002C8h],xmm0
  0000000140EBC6C2: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC6CA: call        0000000140130EE0
  0000000140EBC6CF: movzx       eax,al
  0000000140EBC6D2: test        eax,eax
  0000000140EBC6D4: je          0000000140EBC793
  0000000140EBC6DA: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC6E2: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC6E9: lea         rdx,[rsp+0000000000000090h]
  0000000140EBC6F1: mov         rcx,qword ptr [rcx+00000000000003F8h]
  0000000140EBC6F8: call        qword ptr [0000000141E066B0h]
  0000000140EBC6FE: mov         qword ptr [rsp+0000000000000140h],rax
  0000000140EBC706: mov         rax,qword ptr [rsp+0000000000000140h]
  0000000140EBC70E: mov         qword ptr [rsp+0000000000000148h],rax
  0000000140EBC716: mov         rdx,qword ptr [rsp+0000000000000148h]
  0000000140EBC71E: lea         rcx,[rsp+60h]
  0000000140EBC723: call        qword ptr [0000000141E04470h]
  0000000140EBC729: nop
  0000000140EBC72A: lea         rcx,[rsp+0000000000000090h]
  0000000140EBC732: call        qword ptr [0000000141E059E0h]
  0000000140EBC738: lea         r8,[rsp+58h]
  0000000140EBC73D: lea         rdx,[rsp+60h]
  0000000140EBC742: lea         rcx,[rsp+30h]
  0000000140EBC747: call        qword ptr [0000000141E080B8h]
  0000000140EBC74D: mov         dword ptr [rsp+24h],eax
  0000000140EBC751: cmp         dword ptr [rsp+24h],0
  0000000140EBC756: jne         0000000140EBC774
  0000000140EBC758: lea         rcx,[rsp+30h]
  0000000140EBC75D: call        qword ptr [0000000141E080D0h]
  0000000140EBC763: nop
  0000000140EBC764: lea         rcx,[rsp+60h]
  0000000140EBC769: call        qword ptr [0000000141E059E0h]
  0000000140EBC76F: jmp         0000000140EBD170
  0000000140EBC774: xor         edx,edx
  0000000140EBC776: lea         rcx,[rsp+60h]
  0000000140EBC77B: call        qword ptr [0000000141E05810h]
  0000000140EBC781: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC789: movsd       mmword ptr [rax+00000000000002C0h],xmm0
  0000000140EBC791: jmp         0000000140EBC7F1
  0000000140EBC793: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC79B: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC7A2: mov         rcx,qword ptr [rcx+0000000000000370h]
  0000000140EBC7A9: call        qword ptr [0000000141E05D68h]
  0000000140EBC7AF: mov         byte ptr [rsp+68h],al
  0000000140EBC7B3: movzx       eax,byte ptr [rsp+68h]
  0000000140EBC7B8: test        eax,eax
  0000000140EBC7BA: jne         0000000140EBC7C9
  0000000140EBC7BC: mov         dword ptr [rsp+0000000000000150h],1
  0000000140EBC7C7: jmp         0000000140EBC7D4
  0000000140EBC7C9: mov         dword ptr [rsp+0000000000000150h],0
  0000000140EBC7D4: movd        xmm0,dword ptr [rsp+0000000000000150h]
  0000000140EBC7DD: cvtdq2pd    xmm0,xmm0
  0000000140EBC7E1: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC7E9: movsd       mmword ptr [rax+00000000000002C0h],xmm0
  0000000140EBC7F1: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC7F9: call        0000000140130EA0
  0000000140EBC7FE: movzx       eax,al
  0000000140EBC801: test        eax,eax
  0000000140EBC803: je          0000000140EBC977
  0000000140EBC809: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC811: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC818: lea         rdx,[rsp+0000000000000098h]
  0000000140EBC820: mov         rcx,qword ptr [rcx+00000000000004F8h]
  0000000140EBC827: call        qword ptr [0000000141E066B0h]
  0000000140EBC82D: mov         qword ptr [rsp+0000000000000158h],rax
  0000000140EBC835: mov         rax,qword ptr [rsp+0000000000000158h]
  0000000140EBC83D: mov         qword ptr [rsp+0000000000000160h],rax
  0000000140EBC845: mov         rdx,qword ptr [rsp+0000000000000160h]
  0000000140EBC84D: lea         rcx,[rsp+60h]
  0000000140EBC852: call        qword ptr [0000000141E04470h]
  0000000140EBC858: nop
  0000000140EBC859: lea         rcx,[rsp+0000000000000098h]
  0000000140EBC861: call        qword ptr [0000000141E059E0h]
  0000000140EBC867: lea         r8,[rsp+58h]
  0000000140EBC86C: lea         rdx,[rsp+60h]
  0000000140EBC871: lea         rcx,[rsp+30h]
  0000000140EBC876: call        qword ptr [0000000141E080B8h]
  0000000140EBC87C: mov         dword ptr [rsp+24h],eax
  0000000140EBC880: cmp         dword ptr [rsp+24h],0
  0000000140EBC885: jne         0000000140EBC8A3
  0000000140EBC887: lea         rcx,[rsp+30h]
  0000000140EBC88C: call        qword ptr [0000000141E080D0h]
  0000000140EBC892: nop
  0000000140EBC893: lea         rcx,[rsp+60h]
  0000000140EBC898: call        qword ptr [0000000141E059E0h]
  0000000140EBC89E: jmp         0000000140EBD170
  0000000140EBC8A3: xor         edx,edx
  0000000140EBC8A5: lea         rcx,[rsp+60h]
  0000000140EBC8AA: call        qword ptr [0000000141E05810h]
  0000000140EBC8B0: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC8B8: movsd       mmword ptr [rax+0000000000000278h],xmm0
  0000000140EBC8C0: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC8C8: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC8CF: lea         rdx,[rsp+00000000000000A0h]
  0000000140EBC8D7: mov         rcx,qword ptr [rcx+0000000000000500h]
  0000000140EBC8DE: call        qword ptr [0000000141E066B0h]
  0000000140EBC8E4: mov         qword ptr [rsp+0000000000000168h],rax
  0000000140EBC8EC: mov         rax,qword ptr [rsp+0000000000000168h]
  0000000140EBC8F4: mov         qword ptr [rsp+0000000000000170h],rax
  0000000140EBC8FC: mov         rdx,qword ptr [rsp+0000000000000170h]
  0000000140EBC904: lea         rcx,[rsp+60h]
  0000000140EBC909: call        qword ptr [0000000141E04470h]
  0000000140EBC90F: nop
  0000000140EBC910: lea         rcx,[rsp+00000000000000A0h]
  0000000140EBC918: call        qword ptr [0000000141E059E0h]
  0000000140EBC91E: lea         r8,[rsp+58h]
  0000000140EBC923: lea         rdx,[rsp+60h]
  0000000140EBC928: lea         rcx,[rsp+30h]
  0000000140EBC92D: call        qword ptr [0000000141E080B8h]
  0000000140EBC933: mov         dword ptr [rsp+24h],eax
  0000000140EBC937: cmp         dword ptr [rsp+24h],0
  0000000140EBC93C: jne         0000000140EBC95A
  0000000140EBC93E: lea         rcx,[rsp+30h]
  0000000140EBC943: call        qword ptr [0000000141E080D0h]
  0000000140EBC949: nop
  0000000140EBC94A: lea         rcx,[rsp+60h]
  0000000140EBC94F: call        qword ptr [0000000141E059E0h]
  0000000140EBC955: jmp         0000000140EBD170
  0000000140EBC95A: xor         edx,edx
  0000000140EBC95C: lea         rcx,[rsp+60h]
  0000000140EBC961: call        qword ptr [0000000141E05810h]
  0000000140EBC967: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBC96F: movsd       mmword ptr [rax+0000000000000280h],xmm0
  0000000140EBC977: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC97F: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC986: mov         rcx,qword ptr [rcx+00000000000002F0h]
  0000000140EBC98D: call        qword ptr [0000000141E05D68h]
  0000000140EBC993: movzx       eax,al
  0000000140EBC996: test        eax,eax
  0000000140EBC998: je          0000000140EBC9A4
  0000000140EBC99A: mov         dword ptr [rsp+20h],0
  0000000140EBC9A2: jmp         0000000140EBC9FC
  0000000140EBC9A4: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC9AC: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC9B3: mov         rcx,qword ptr [rcx+00000000000002F8h]
  0000000140EBC9BA: call        qword ptr [0000000141E05D68h]
  0000000140EBC9C0: movzx       eax,al
  0000000140EBC9C3: test        eax,eax
  0000000140EBC9C5: je          0000000140EBC9D1
  0000000140EBC9C7: mov         dword ptr [rsp+20h],1
  0000000140EBC9CF: jmp         0000000140EBC9FC
  0000000140EBC9D1: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBC9D9: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBC9E0: mov         rcx,qword ptr [rcx+0000000000000300h]
  0000000140EBC9E7: call        qword ptr [0000000141E05D68h]
  0000000140EBC9ED: movzx       eax,al
  0000000140EBC9F0: test        eax,eax
  0000000140EBC9F2: je          0000000140EBC9FC
  0000000140EBC9F4: mov         dword ptr [rsp+20h],2
  0000000140EBC9FC: cmp         dword ptr [rsp+20h],2
  0000000140EBCA01: jne         0000000140EBCCE8
  0000000140EBCA07: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCA0F: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCA16: lea         rdx,[rsp+00000000000000A8h]
  0000000140EBCA1E: mov         rcx,qword ptr [rcx+0000000000000288h]
  0000000140EBCA25: call        qword ptr [0000000141E066B0h]
  0000000140EBCA2B: mov         qword ptr [rsp+0000000000000178h],rax
  0000000140EBCA33: mov         rax,qword ptr [rsp+0000000000000178h]
  0000000140EBCA3B: mov         qword ptr [rsp+0000000000000180h],rax
  0000000140EBCA43: mov         rdx,qword ptr [rsp+0000000000000180h]
  0000000140EBCA4B: lea         rcx,[rsp+60h]
  0000000140EBCA50: call        qword ptr [0000000141E04470h]
  0000000140EBCA56: nop
  0000000140EBCA57: lea         rcx,[rsp+00000000000000A8h]
  0000000140EBCA5F: call        qword ptr [0000000141E059E0h]
  0000000140EBCA65: lea         r8,[rsp+58h]
  0000000140EBCA6A: lea         rdx,[rsp+60h]
  0000000140EBCA6F: lea         rcx,[rsp+30h]
  0000000140EBCA74: call        qword ptr [0000000141E080B8h]
  0000000140EBCA7A: mov         dword ptr [rsp+24h],eax
  0000000140EBCA7E: cmp         dword ptr [rsp+24h],0
  0000000140EBCA83: jne         0000000140EBCAA1
  0000000140EBCA85: lea         rcx,[rsp+30h]
  0000000140EBCA8A: call        qword ptr [0000000141E080D0h]
  0000000140EBCA90: nop
  0000000140EBCA91: lea         rcx,[rsp+60h]
  0000000140EBCA96: call        qword ptr [0000000141E059E0h]
  0000000140EBCA9C: jmp         0000000140EBD170
  0000000140EBCAA1: xor         edx,edx
  0000000140EBCAA3: lea         rcx,[rsp+60h]
  0000000140EBCAA8: call        qword ptr [0000000141E05810h]
  0000000140EBCAAE: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCAB6: movsd       mmword ptr [rax+00000000000001D0h],xmm0
  0000000140EBCABE: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCAC6: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCACD: lea         rdx,[rsp+00000000000000B0h]
  0000000140EBCAD5: mov         rcx,qword ptr [rcx+00000000000002A0h]
  0000000140EBCADC: call        qword ptr [0000000141E066B0h]
  0000000140EBCAE2: mov         qword ptr [rsp+0000000000000188h],rax
  0000000140EBCAEA: mov         rax,qword ptr [rsp+0000000000000188h]
  0000000140EBCAF2: mov         qword ptr [rsp+0000000000000190h],rax
  0000000140EBCAFA: mov         rdx,qword ptr [rsp+0000000000000190h]
  0000000140EBCB02: lea         rcx,[rsp+60h]
  0000000140EBCB07: call        qword ptr [0000000141E04470h]
  0000000140EBCB0D: nop
  0000000140EBCB0E: lea         rcx,[rsp+00000000000000B0h]
  0000000140EBCB16: call        qword ptr [0000000141E059E0h]
  0000000140EBCB1C: lea         r8,[rsp+58h]
  0000000140EBCB21: lea         rdx,[rsp+60h]
  0000000140EBCB26: lea         rcx,[rsp+30h]
  0000000140EBCB2B: call        qword ptr [0000000141E080B8h]
  0000000140EBCB31: mov         dword ptr [rsp+24h],eax
  0000000140EBCB35: cmp         dword ptr [rsp+24h],0
  0000000140EBCB3A: jne         0000000140EBCB58
  0000000140EBCB3C: lea         rcx,[rsp+30h]
  0000000140EBCB41: call        qword ptr [0000000141E080D0h]
  0000000140EBCB47: nop
  0000000140EBCB48: lea         rcx,[rsp+60h]
  0000000140EBCB4D: call        qword ptr [0000000141E059E0h]
  0000000140EBCB53: jmp         0000000140EBD170
  0000000140EBCB58: xor         edx,edx
  0000000140EBCB5A: lea         rcx,[rsp+60h]
  0000000140EBCB5F: call        qword ptr [0000000141E05810h]
  0000000140EBCB65: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCB6D: movsd       mmword ptr [rax+0000000000000270h],xmm0
  0000000140EBCB75: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCB7D: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCB84: lea         rdx,[rsp+00000000000000B8h]
  0000000140EBCB8C: mov         rcx,qword ptr [rcx+00000000000002C0h]
  0000000140EBCB93: call        qword ptr [0000000141E066B0h]
  0000000140EBCB99: mov         qword ptr [rsp+0000000000000198h],rax
  0000000140EBCBA1: mov         rax,qword ptr [rsp+0000000000000198h]
  0000000140EBCBA9: mov         qword ptr [rsp+00000000000001A0h],rax
  0000000140EBCBB1: mov         rdx,qword ptr [rsp+00000000000001A0h]
  0000000140EBCBB9: lea         rcx,[rsp+60h]
  0000000140EBCBBE: call        qword ptr [0000000141E04470h]
  0000000140EBCBC4: nop
  0000000140EBCBC5: lea         rcx,[rsp+00000000000000B8h]
  0000000140EBCBCD: call        qword ptr [0000000141E059E0h]
  0000000140EBCBD3: lea         r8,[rsp+58h]
  0000000140EBCBD8: lea         rdx,[rsp+60h]
  0000000140EBCBDD: lea         rcx,[rsp+30h]
  0000000140EBCBE2: call        qword ptr [0000000141E080B8h]
  0000000140EBCBE8: mov         dword ptr [rsp+24h],eax
  0000000140EBCBEC: cmp         dword ptr [rsp+24h],0
  0000000140EBCBF1: jne         0000000140EBCC0F
  0000000140EBCBF3: lea         rcx,[rsp+30h]
  0000000140EBCBF8: call        qword ptr [0000000141E080D0h]
  0000000140EBCBFE: nop
  0000000140EBCBFF: lea         rcx,[rsp+60h]
  0000000140EBCC04: call        qword ptr [0000000141E059E0h]
  0000000140EBCC0A: jmp         0000000140EBD170
  0000000140EBCC0F: xor         edx,edx
  0000000140EBCC11: lea         rcx,[rsp+60h]
  0000000140EBCC16: call        qword ptr [0000000141E05810h]
  0000000140EBCC1C: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCC24: movsd       mmword ptr [rax+00000000000002A0h],xmm0
  0000000140EBCC2C: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCC34: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCC3B: lea         rdx,[rsp+00000000000000C0h]
  0000000140EBCC43: mov         rcx,qword ptr [rcx+00000000000002C8h]
  0000000140EBCC4A: call        qword ptr [0000000141E066B0h]
  0000000140EBCC50: mov         qword ptr [rsp+00000000000001A8h],rax
  0000000140EBCC58: mov         rax,qword ptr [rsp+00000000000001A8h]
  0000000140EBCC60: mov         qword ptr [rsp+00000000000001B0h],rax
  0000000140EBCC68: mov         rdx,qword ptr [rsp+00000000000001B0h]
  0000000140EBCC70: lea         rcx,[rsp+60h]
  0000000140EBCC75: call        qword ptr [0000000141E04470h]
  0000000140EBCC7B: nop
  0000000140EBCC7C: lea         rcx,[rsp+00000000000000C0h]
  0000000140EBCC84: call        qword ptr [0000000141E059E0h]
  0000000140EBCC8A: lea         r8,[rsp+58h]
  0000000140EBCC8F: lea         rdx,[rsp+60h]
  0000000140EBCC94: lea         rcx,[rsp+30h]
  0000000140EBCC99: call        qword ptr [0000000141E080B8h]
  0000000140EBCC9F: mov         dword ptr [rsp+24h],eax
  0000000140EBCCA3: cmp         dword ptr [rsp+24h],0
  0000000140EBCCA8: jne         0000000140EBCCC6
  0000000140EBCCAA: lea         rcx,[rsp+30h]
  0000000140EBCCAF: call        qword ptr [0000000141E080D0h]
  0000000140EBCCB5: nop
  0000000140EBCCB6: lea         rcx,[rsp+60h]
  0000000140EBCCBB: call        qword ptr [0000000141E059E0h]
  0000000140EBCCC1: jmp         0000000140EBD170
  0000000140EBCCC6: xor         edx,edx
  0000000140EBCCC8: lea         rcx,[rsp+60h]
  0000000140EBCCCD: call        qword ptr [0000000141E05810h]
  0000000140EBCCD3: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCCDB: movsd       mmword ptr [rax+00000000000002A8h],xmm0
  0000000140EBCCE3: jmp         0000000140EBCF0D
  0000000140EBCCE8: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCCF0: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCCF7: lea         rdx,[rsp+00000000000000C8h]
  0000000140EBCCFF: mov         rcx,qword ptr [rcx+0000000000000450h]
  0000000140EBCD06: call        qword ptr [0000000141E066B0h]
  0000000140EBCD0C: mov         qword ptr [rsp+00000000000001B8h],rax
  0000000140EBCD14: mov         rax,qword ptr [rsp+00000000000001B8h]
  0000000140EBCD1C: mov         qword ptr [rsp+00000000000001C0h],rax
  0000000140EBCD24: mov         rdx,qword ptr [rsp+00000000000001C0h]
  0000000140EBCD2C: lea         rcx,[rsp+60h]
  0000000140EBCD31: call        qword ptr [0000000141E04470h]
  0000000140EBCD37: nop
  0000000140EBCD38: lea         rcx,[rsp+00000000000000C8h]
  0000000140EBCD40: call        qword ptr [0000000141E059E0h]
  0000000140EBCD46: lea         r8,[rsp+58h]
  0000000140EBCD4B: lea         rdx,[rsp+60h]
  0000000140EBCD50: lea         rcx,[rsp+30h]
  0000000140EBCD55: call        qword ptr [0000000141E080B8h]
  0000000140EBCD5B: mov         dword ptr [rsp+24h],eax
  0000000140EBCD5F: cmp         dword ptr [rsp+24h],0
  0000000140EBCD64: jne         0000000140EBCD82
  0000000140EBCD66: lea         rcx,[rsp+30h]
  0000000140EBCD6B: call        qword ptr [0000000141E080D0h]
  0000000140EBCD71: nop
  0000000140EBCD72: lea         rcx,[rsp+60h]
  0000000140EBCD77: call        qword ptr [0000000141E059E0h]
  0000000140EBCD7D: jmp         0000000140EBD170
  0000000140EBCD82: xor         edx,edx
  0000000140EBCD84: lea         rcx,[rsp+60h]
  0000000140EBCD89: call        qword ptr [0000000141E05810h]
  0000000140EBCD8F: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCD97: movsd       mmword ptr [rax+0000000000000270h],xmm0
  0000000140EBCD9F: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCDA7: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCDAE: lea         rdx,[rsp+00000000000000D0h]
  0000000140EBCDB6: mov         rcx,qword ptr [rcx+0000000000000430h]
  0000000140EBCDBD: call        qword ptr [0000000141E066B0h]
  0000000140EBCDC3: mov         qword ptr [rsp+00000000000001C8h],rax
  0000000140EBCDCB: mov         rax,qword ptr [rsp+00000000000001C8h]
  0000000140EBCDD3: mov         qword ptr [rsp+00000000000001D0h],rax
  0000000140EBCDDB: mov         rdx,qword ptr [rsp+00000000000001D0h]
  0000000140EBCDE3: lea         rcx,[rsp+60h]
  0000000140EBCDE8: call        qword ptr [0000000141E04470h]
  0000000140EBCDEE: nop
  0000000140EBCDEF: lea         rcx,[rsp+00000000000000D0h]
  0000000140EBCDF7: call        qword ptr [0000000141E059E0h]
  0000000140EBCDFD: lea         r8,[rsp+58h]
  0000000140EBCE02: lea         rdx,[rsp+60h]
  0000000140EBCE07: lea         rcx,[rsp+30h]
  0000000140EBCE0C: call        qword ptr [0000000141E080B8h]
  0000000140EBCE12: mov         dword ptr [rsp+24h],eax
  0000000140EBCE16: cmp         dword ptr [rsp+24h],0
  0000000140EBCE1B: jne         0000000140EBCE39
  0000000140EBCE1D: lea         rcx,[rsp+30h]
  0000000140EBCE22: call        qword ptr [0000000141E080D0h]
  0000000140EBCE28: nop
  0000000140EBCE29: lea         rcx,[rsp+60h]
  0000000140EBCE2E: call        qword ptr [0000000141E059E0h]
  0000000140EBCE34: jmp         0000000140EBD170
  0000000140EBCE39: xor         edx,edx
  0000000140EBCE3B: lea         rcx,[rsp+60h]
  0000000140EBCE40: call        qword ptr [0000000141E05810h]
  0000000140EBCE46: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCE4E: movsd       mmword ptr [rax+00000000000002A0h],xmm0
  0000000140EBCE56: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCE5E: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCE65: lea         rdx,[rsp+00000000000000D8h]
  0000000140EBCE6D: mov         rcx,qword ptr [rcx+0000000000000438h]
  0000000140EBCE74: call        qword ptr [0000000141E066B0h]
  0000000140EBCE7A: mov         qword ptr [rsp+00000000000001D8h],rax
  0000000140EBCE82: mov         rax,qword ptr [rsp+00000000000001D8h]
  0000000140EBCE8A: mov         qword ptr [rsp+00000000000001E0h],rax
  0000000140EBCE92: mov         rdx,qword ptr [rsp+00000000000001E0h]
  0000000140EBCE9A: lea         rcx,[rsp+60h]
  0000000140EBCE9F: call        qword ptr [0000000141E04470h]
  0000000140EBCEA5: nop
  0000000140EBCEA6: lea         rcx,[rsp+00000000000000D8h]
  0000000140EBCEAE: call        qword ptr [0000000141E059E0h]
  0000000140EBCEB4: lea         r8,[rsp+58h]
  0000000140EBCEB9: lea         rdx,[rsp+60h]
  0000000140EBCEBE: lea         rcx,[rsp+30h]
  0000000140EBCEC3: call        qword ptr [0000000141E080B8h]
  0000000140EBCEC9: mov         dword ptr [rsp+24h],eax
  0000000140EBCECD: cmp         dword ptr [rsp+24h],0
  0000000140EBCED2: jne         0000000140EBCEF0
  0000000140EBCED4: lea         rcx,[rsp+30h]
  0000000140EBCED9: call        qword ptr [0000000141E080D0h]
  0000000140EBCEDF: nop
  0000000140EBCEE0: lea         rcx,[rsp+60h]
  0000000140EBCEE5: call        qword ptr [0000000141E059E0h]
  0000000140EBCEEB: jmp         0000000140EBD170
  0000000140EBCEF0: xor         edx,edx
  0000000140EBCEF2: lea         rcx,[rsp+60h]
  0000000140EBCEF7: call        qword ptr [0000000141E05810h]
  0000000140EBCEFD: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCF05: movsd       mmword ptr [rax+00000000000002A8h],xmm0
  0000000140EBCF0D: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCF15: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCF1C: lea         rdx,[rsp+00000000000000E0h]
  0000000140EBCF24: mov         rcx,qword ptr [rcx+00000000000004C8h]
  0000000140EBCF2B: call        qword ptr [0000000141E066B0h]
  0000000140EBCF31: mov         qword ptr [rsp+00000000000001E8h],rax
  0000000140EBCF39: mov         rax,qword ptr [rsp+00000000000001E8h]
  0000000140EBCF41: mov         qword ptr [rsp+00000000000001F0h],rax
  0000000140EBCF49: mov         rdx,qword ptr [rsp+00000000000001F0h]
  0000000140EBCF51: lea         rcx,[rsp+60h]
  0000000140EBCF56: call        qword ptr [0000000141E04470h]
  0000000140EBCF5C: nop
  0000000140EBCF5D: lea         rcx,[rsp+00000000000000E0h]
  0000000140EBCF65: call        qword ptr [0000000141E059E0h]
  0000000140EBCF6B: lea         r8,[rsp+58h]
  0000000140EBCF70: lea         rdx,[rsp+60h]
  0000000140EBCF75: lea         rcx,[rsp+30h]
  0000000140EBCF7A: call        qword ptr [0000000141E080B8h]
  0000000140EBCF80: mov         dword ptr [rsp+24h],eax
  0000000140EBCF84: cmp         dword ptr [rsp+24h],0
  0000000140EBCF89: jne         0000000140EBCFA7
  0000000140EBCF8B: lea         rcx,[rsp+30h]
  0000000140EBCF90: call        qword ptr [0000000141E080D0h]
  0000000140EBCF96: nop
  0000000140EBCF97: lea         rcx,[rsp+60h]
  0000000140EBCF9C: call        qword ptr [0000000141E059E0h]
  0000000140EBCFA2: jmp         0000000140EBD170
  0000000140EBCFA7: xor         edx,edx
  0000000140EBCFA9: lea         rcx,[rsp+60h]
  0000000140EBCFAE: call        qword ptr [0000000141E05810h]
  0000000140EBCFB4: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBCFBC: movsd       mmword ptr [rax+0000000000000268h],xmm0
  0000000140EBCFC4: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCFCC: call        0000000140130EA0
  0000000140EBCFD1: movzx       eax,al
  0000000140EBCFD4: test        eax,eax
  0000000140EBCFD6: je          0000000140EBD112
  0000000140EBCFDC: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBCFE4: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBCFEB: lea         rdx,[rsp+00000000000000E8h]
  0000000140EBCFF3: mov         rcx,qword ptr [rcx+0000000000000490h]
  0000000140EBCFFA: call        qword ptr [0000000141E066B0h]
  0000000140EBD000: mov         qword ptr [rsp+00000000000001F8h],rax
  0000000140EBD008: mov         rax,qword ptr [rsp+00000000000001F8h]
  0000000140EBD010: mov         qword ptr [rsp+0000000000000200h],rax
  0000000140EBD018: mov         rdx,qword ptr [rsp+0000000000000200h]
  0000000140EBD020: lea         rcx,[rsp+60h]
  0000000140EBD025: call        qword ptr [0000000141E04470h]
  0000000140EBD02B: nop
  0000000140EBD02C: lea         rcx,[rsp+00000000000000E8h]
  0000000140EBD034: call        qword ptr [0000000141E059E0h]
  0000000140EBD03A: lea         r8,[rsp+58h]
  0000000140EBD03F: lea         rdx,[rsp+60h]
  0000000140EBD044: lea         rcx,[rsp+30h]
  0000000140EBD049: call        qword ptr [0000000141E080B8h]
  0000000140EBD04F: mov         dword ptr [rsp+24h],eax
  0000000140EBD053: cmp         dword ptr [rsp+24h],0
  0000000140EBD058: je          0000000140EBD077
  0000000140EBD05A: xor         edx,edx
  0000000140EBD05C: lea         rcx,[rsp+60h]
  0000000140EBD061: call        qword ptr [0000000141E05810h]
  0000000140EBD067: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBD06F: movsd       mmword ptr [rax+00000000000002B0h],xmm0
  0000000140EBD077: mov         rcx,qword ptr [rsp+0000000000000230h]
  0000000140EBD07F: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBD086: lea         rdx,[rsp+00000000000000F0h]
  0000000140EBD08E: mov         rcx,qword ptr [rcx+0000000000000480h]
  0000000140EBD095: call        qword ptr [0000000141E066B0h]
  0000000140EBD09B: mov         qword ptr [rsp+0000000000000208h],rax
  0000000140EBD0A3: mov         rax,qword ptr [rsp+0000000000000208h]
  0000000140EBD0AB: mov         qword ptr [rsp+0000000000000210h],rax
  0000000140EBD0B3: mov         rdx,qword ptr [rsp+0000000000000210h]
  0000000140EBD0BB: lea         rcx,[rsp+60h]
  0000000140EBD0C0: call        qword ptr [0000000141E04470h]
  0000000140EBD0C6: nop
  0000000140EBD0C7: lea         rcx,[rsp+00000000000000F0h]
  0000000140EBD0CF: call        qword ptr [0000000141E059E0h]
  0000000140EBD0D5: lea         r8,[rsp+58h]
  0000000140EBD0DA: lea         rdx,[rsp+60h]
  0000000140EBD0DF: lea         rcx,[rsp+30h]
  0000000140EBD0E4: call        qword ptr [0000000141E080B8h]
  0000000140EBD0EA: mov         dword ptr [rsp+24h],eax
  0000000140EBD0EE: cmp         dword ptr [rsp+24h],0
  0000000140EBD0F3: je          0000000140EBD112
  0000000140EBD0F5: xor         edx,edx
  0000000140EBD0F7: lea         rcx,[rsp+60h]
  0000000140EBD0FC: call        qword ptr [0000000141E05810h]
  0000000140EBD102: mov         rax,qword ptr [rsp+0000000000000238h]
  0000000140EBD10A: movsd       mmword ptr [rax+00000000000002B8h],xmm0
  0000000140EBD112: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBD11A: call        00000001408DA6D0
  0000000140EBD11F: mov         dword ptr [rsp+28h],eax
  0000000140EBD123: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBD12B: add         rcx,1E8h
  0000000140EBD132: mov         rdx,qword ptr [rsp+0000000000000230h]
  0000000140EBD13A: mov         rdx,qword ptr [rdx+00000000000000E8h]
  0000000140EBD141: call        000000014089AD60
  0000000140EBD146: lea         rdx,[rsp+28h]
  0000000140EBD14B: mov         rcx,qword ptr [rsp+0000000000000238h]
  0000000140EBD153: call        00000001408DA6F0
  0000000140EBD158: nop
  0000000140EBD159: lea         rcx,[rsp+30h]
  0000000140EBD15E: call        qword ptr [0000000141E080D0h]
  0000000140EBD164: nop
  0000000140EBD165: lea         rcx,[rsp+60h]
  0000000140EBD16A: call        qword ptr [0000000141E059E0h]
  0000000140EBD170: add         rsp,228h
  0000000140EBD177: ret
  0000000140EBD178: int         3

  Summary

     1E02000 .text
