140816900: mov         dword ptr [rsp+10h],edx
140816904: mov         qword ptr [rsp+8],rcx
140816909: sub         rsp,38h
14081690d: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
140816916: mov         rcx,qword ptr [rsp+40h]
14081691b: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140816921: nop
140816922: mov         rcx,qword ptr [rsp+40h]
140816927: add         rcx,8
14081692b: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140816931: nop
140816932: mov         rcx,qword ptr [rsp+40h]
140816937: add         rcx,28h
14081693b: call        14018F9E0
140816940: nop
140816941: mov         rcx,qword ptr [rsp+40h]
140816946: add         rcx,30h
14081694a: call        14018F9E0
14081694f: nop
140816950: mov         rcx,qword ptr [rsp+40h]
140816955: add         rcx,38h
140816959: call        14018F9E0
14081695e: mov         r11,qword ptr [rsp+40h]
140816963: mov         eax,dword ptr [rsp+48h]
140816967: mov         dword ptr [r11+10h],eax
14081696b: mov         rax,qword ptr [rsp+40h]
140816970: mov         dword ptr [rax+14h],0
140816977: mov         rax,qword ptr [rsp+40h]
14081697c: mov         qword ptr [rax+40h],0
140816984: mov         rax,qword ptr [rsp+40h]
140816989: mov         dword ptr [rax+20h],64h
140816990: mov         rax,qword ptr [rsp+40h]
140816995: mov         dword ptr [rax+18h],0FFFFFFFFh
14081699c: mov         rax,qword ptr [rsp+40h]
1408169a1: mov         dword ptr [rax+1Ch],0
1408169a8: mov         rax,qword ptr [rsp+40h]
1408169ad: mov         dword ptr [rax+24h],3
1408169b4: mov         rax,qword ptr [rsp+40h]
1408169b9: add         rsp,38h
1408169bd: ret
140816b40: mov         dword ptr [rsp+20h],r9d
140816b45: mov         qword ptr [rsp+18h],r8
140816b4a: mov         dword ptr [rsp+10h],edx
140816b4e: mov         qword ptr [rsp+8],rcx
140816b53: sub         rsp,0A8h
140816b5a: mov         qword ptr [rsp+080],0FFFFFFFFFFFFFFFEh
140816b66: mov         rcx,qword ptr [rsp+0B0]
140816b6e: add         rcx,30h
140816b72: call        140004AE0
140816b77: mov         dword ptr [rsp+48h],eax
140816b7b: mov         byte ptr [rsp+40h],0
140816b80: mov         rcx,qword ptr [rsp+0B0]
140816b88: mov         eax,dword ptr [rsp+0D0]
140816b8f: mov         dword ptr [rcx+24h],eax
140816b92: cmp         dword ptr [rsp+0B8],131h
140816b9d: je          140816C53
140816ba3: mov         dword ptr [rsp+44h],0
140816bab: jmp         140816BB8
140816bad: mov         eax,dword ptr [rsp+44h]
140816bb1: add         eax,1
140816bb4: mov         dword ptr [rsp+44h],eax
140816bb8: mov         eax,dword ptr [rsp+48h]
140816bbc: cmp         dword ptr [rsp+44h],eax
140816bc0: jge         140816C53
140816bc6: mov         rcx,qword ptr [rsp+0B0]
140816bce: add         rcx,30h
140816bd2: mov         edx,dword ptr [rsp+44h]
140816bd6: call        140160C10
140816bdb: mov         rcx,qword ptr [rax]
140816bde: mov         eax,dword ptr [rsp+0B8]
140816be5: cmp         dword ptr [rcx+2Ch],eax
140816be8: jne         140816C4E
140816bea: mov         rcx,qword ptr [rsp+0B0]
140816bf2: add         rcx,30h
140816bf6: mov         edx,dword ptr [rsp+44h]
140816bfa: call        140160C10
140816bff: mov         rdx,qword ptr [rsp+0C0]
140816c07: mov         rcx,qword ptr [rax]
140816c0a: call        qword ptr [141E03F08h] ; QtCore4_64.dll!??8QString@@QEBA_NAEBV0@@Z
140816c10: movzx       eax,al
140816c13: test        eax,eax
140816c15: je          140816C4E
140816c17: mov         rcx,qword ptr [rsp+0B0]
140816c1f: add         rcx,30h
140816c23: mov         edx,dword ptr [rsp+44h]
140816c27: call        140160C10
140816c2c: mov         rcx,qword ptr [rax]
140816c2f: add         rcx,50h
140816c33: mov         rdx,qword ptr [rsp+0E8]
140816c3b: call        1400D0FF0
140816c40: movzx       eax,al
140816c43: test        eax,eax
140816c45: je          140816C4E
140816c47: mov         byte ptr [rsp+40h],1
140816c4c: jmp         140816C53
140816c4e: jmp         140816BAD
140816c53: movzx       eax,byte ptr [rsp+40h]
140816c58: test        eax,eax
140816c5a: jne         140816DB7
140816c60: mov         ecx,0A8h
140816c65: call        14146EABC
140816c6a: mov         qword ptr [rsp+60h],rax
140816c6f: cmp         qword ptr [rsp+60h],0
140816c75: je          140816CF0
140816c77: lea         rax,[rsp+68h]
140816c7c: mov         qword ptr [rsp+70h],rax
140816c81: mov         rdx,qword ptr [rsp+0C0]
140816c89: mov         rcx,qword ptr [rsp+70h]
140816c8e: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140816c94: mov         qword ptr [rsp+088],rax
140816c9c: mov         rax,qword ptr [rsp+0E8]
140816ca4: mov         qword ptr [rsp+30h],rax
140816ca9: mov         rax,qword ptr [rsp+0E0]
140816cb1: mov         qword ptr [rsp+28h],rax
140816cb6: movsd       xmm0,mmword ptr [rsp+0D8]
140816cbf: movsd       mmword ptr [rsp+20h],xmm0
140816cc5: mov         r9d,dword ptr [rsp+0C8]
140816ccd: mov         r8,qword ptr [rsp+088]
140816cd5: mov         edx,dword ptr [rsp+0B8]
140816cdc: mov         rcx,qword ptr [rsp+60h]
140816ce1: call        1409206E0
140816ce6: mov         qword ptr [rsp+090],rax
140816cee: jmp         140816CFC
140816cf0: mov         qword ptr [rsp+090],0
140816cfc: mov         rax,qword ptr [rsp+090]
140816d04: mov         qword ptr [rsp+58h],rax
140816d09: mov         rax,qword ptr [rsp+58h]
140816d0e: mov         qword ptr [rsp+50h],rax
140816d13: mov         rcx,qword ptr [rsp+0B0]
140816d1b: add         rcx,30h
140816d1f: lea         rdx,[rsp+50h]
140816d24: call        140CD9AF0
140816d29: mov         rcx,qword ptr [rsp+0E0]
140816d31: call        140004AE0
140816d36: mov         dword ptr [rsp+48h],eax
140816d3a: mov         dword ptr [rsp+44h],0
140816d42: jmp         140816D4F
140816d44: mov         eax,dword ptr [rsp+44h]
140816d48: add         eax,1
140816d4b: mov         dword ptr [rsp+44h],eax
140816d4f: mov         eax,dword ptr [rsp+48h]
140816d53: cmp         dword ptr [rsp+44h],eax
140816d57: jge         140816DB7
140816d59: mov         edx,dword ptr [rsp+44h]
140816d5d: mov         rcx,qword ptr [rsp+0E0]
140816d65: call        1400250C0
140816d6a: mov         rcx,qword ptr [rsp+0B0]
140816d72: add         rcx,38h
140816d76: mov         r8,rax
140816d79: lea         rdx,[rsp+78h]
140816d7e: call        1400251D0
140816d83: mov         rcx,rax
140816d86: call        140004A20
140816d8b: test        rax,rax
140816d8e: jne         140816DB5
140816d90: mov         edx,dword ptr [rsp+44h]
140816d94: mov         rcx,qword ptr [rsp+0E0]
140816d9c: call        1400250C0
140816da1: mov         rcx,qword ptr [rsp+0B0]
140816da9: add         rcx,38h
140816dad: mov         rdx,rax
140816db0: call        140025100
140816db5: jmp         140816D44
140816db7: mov         rcx,qword ptr [rsp+0C0]
140816dbf: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140816dc5: add         rsp,0A8h
140816dcc: ret
1409220e0: mov         qword ptr [rsp+8],rcx
1409220e5: sub         rsp,38h
1409220e9: mov         dword ptr [rsp+20h],130h
1409220f1: mov         dword ptr [rsp+24h],0
1409220f9: jmp         140922106
1409220fb: mov         eax,dword ptr [rsp+24h]
1409220ff: add         eax,1
140922102: mov         dword ptr [rsp+24h],eax
140922106: mov         eax,dword ptr [rsp+20h]
14092210a: cmp         dword ptr [rsp+24h],eax
14092210e: jge         14092214A
140922110: movsxd      rax,dword ptr [rsp+24h]
140922115: imul        rax,rax,30h
140922119: lea         rdx,[00000001427C06E0h]
140922120: mov         rdx,qword ptr [rdx+rax+8]
140922125: mov         rcx,qword ptr [rsp+40h]
14092212a: call        14146F040
14092212f: test        eax,eax
140922131: jne         140922148
140922133: movsxd      rcx,dword ptr [rsp+24h]
140922138: imul        rcx,rcx,30h
14092213c: lea         rax,[00000001427C06E0h]
140922143: add         rax,rcx
140922146: jmp         14092214C
140922148: jmp         1409220FB
14092214a: xor         eax,eax
14092214c: add         rsp,38h
140922150: ret
14091b5d0: mov         qword ptr [rsp+10h],rdx
14091b5d5: mov         qword ptr [rsp+8],rcx
14091b5da: sub         rsp,68h
14091b5de: mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
14091b5e7: mov         dword ptr [rsp+24h],0FFFFFFFFh
14091b5ef: mov         dword ptr [rsp+20h],0
14091b5f7: jmp         14091B604
14091b5f9: mov         eax,dword ptr [rsp+20h]
14091b5fd: add         eax,1
14091b600: mov         dword ptr [rsp+20h],eax
14091b604: mov         rcx,qword ptr [rsp+70h]
14091b609: add         rcx,38h
14091b60d: call        140004AE0
14091b612: cmp         dword ptr [rsp+20h],eax
14091b616: jge         14091B67F
14091b618: mov         rcx,qword ptr [rsp+70h]
14091b61d: add         rcx,38h
14091b621: mov         edx,dword ptr [rsp+20h]
14091b625: call        140160C10
14091b62a: lea         rdx,[rsp+30h]
14091b62f: mov         rcx,qword ptr [rax]
14091b632: call        1403047C0
14091b637: mov         qword ptr [rsp+48h],rax
14091b63c: mov         rax,qword ptr [rsp+48h]
14091b641: mov         qword ptr [rsp+50h],rax
14091b646: mov         rdx,qword ptr [rsp+78h]
14091b64b: mov         rcx,qword ptr [rsp+50h]
14091b650: call        qword ptr [141E03F08h] ; QtCore4_64.dll!??8QString@@QEBA_NAEBV0@@Z
14091b656: mov         byte ptr [rsp+28h],al
14091b65a: lea         rcx,[rsp+30h]
14091b65f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091b665: movzx       r11d,byte ptr [rsp+28h]
14091b66b: test        r11d,r11d
14091b66e: je          14091B67A
14091b670: mov         eax,dword ptr [rsp+20h]
14091b674: mov         dword ptr [rsp+24h],eax
14091b678: jmp         14091B67F
14091b67a: jmp         14091B5F9
14091b67f: mov         eax,dword ptr [rsp+24h]
14091b683: mov         dword ptr [rsp+38h],eax
14091b687: mov         rcx,qword ptr [rsp+78h]
14091b68c: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091b692: mov         eax,dword ptr [rsp+38h]
14091b696: add         rsp,68h
14091b69a: ret
14091b950: mov         dword ptr [rsp+10h],edx
14091b954: mov         qword ptr [rsp+8],rcx
14091b959: sub         rsp,38h
14091b95d: mov         qword ptr [rsp+20h],0
14091b966: cmp         dword ptr [rsp+48h],0FFFFFFFFh
14091b96b: jle         14091B9A0
14091b96d: mov         rax,qword ptr [rsp+40h]
14091b972: mov         rax,qword ptr [rax]
14091b975: mov         rcx,qword ptr [rsp+40h]
14091b97a: call        qword ptr [rax+0000000000000178h]
14091b980: cmp         dword ptr [rsp+48h],eax
14091b984: jge         14091B9A0
14091b986: mov         rcx,qword ptr [rsp+40h]
14091b98b: add         rcx,38h
14091b98f: mov         edx,dword ptr [rsp+48h]
14091b993: call        140160C10
14091b998: mov         rax,qword ptr [rax]
14091b99b: mov         qword ptr [rsp+20h],rax
14091b9a0: mov         rax,qword ptr [rsp+20h]
14091b9a5: add         rsp,38h
14091b9a9: ret
