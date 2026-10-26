14090ee20: mov         qword ptr [rsp+18h],r8
14090ee25: mov         qword ptr [rsp+10h],rdx
14090ee2a: mov         qword ptr [rsp+8],rcx
14090ee2f: push        rsi
14090ee30: push        rdi
14090ee31: mov         eax,1008h
14090ee36: call        1417F5860
14090ee3b: sub         rsp,rax
14090ee3e: mov         qword ptr [rsp+970],0FFFFFFFFFFFFFFFEh
14090ee4a: lea         rcx,[rsp+70h]
14090ee4f: call        14018F9E0
14090ee54: nop
14090ee55: lea         rcx,[rsp+68h]
14090ee5a: call        14018F9E0
14090ee5f: nop
14090ee60: mov         byte ptr [rsp+64h],1
14090ee65: lea         rdx,[rsp+58h]
14090ee6a: mov         rcx,qword ptr [rsp+0000000000001028h]
14090ee72: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
14090ee78: nop
14090ee79: lea         rcx,[rsp+58h]
14090ee7e: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090ee84: movzx       eax,al
14090ee87: test        eax,eax
14090ee89: jne         1409128A3
14090ee8f: lea         rdx,[rsp+78h]
14090ee94: lea         rcx,[rsp+58h]
14090ee99: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
14090ee9f: nop
14090eea0: lea         rcx,[rsp+78h]
14090eea5: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090eeab: movzx       eax,al
14090eeae: test        eax,eax
14090eeb0: jne         140912845
14090eeb6: lea         rdx,[rsp+340]
14090eebe: lea         rcx,[rsp+78h]
14090eec3: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090eec9: mov         qword ptr [rsp+978],rax
14090eed1: mov         rax,qword ptr [rsp+978]
14090eed9: mov         qword ptr [rsp+980],rax
14090eee1: lea         rdx,[00000001427BB508h] ; 'DoeVariable'
14090eee8: mov         rcx,qword ptr [rsp+980]
14090eef0: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090eef6: mov         byte ptr [rsp+338],al
14090eefd: lea         rcx,[rsp+340]
14090ef05: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090ef0b: movzx       edx,byte ptr [rsp+338]
14090ef13: test        edx,edx
14090ef15: je          140912845
14090ef1b: lea         rdx,[00000001427BB514h] ; 'name'
14090ef22: lea         rcx,[rsp+350]
14090ef2a: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090ef30: nop
14090ef31: lea         rcx,[rsp+348]
14090ef39: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090ef3f: mov         qword ptr [rsp+988],rax
14090ef47: mov         rax,qword ptr [rsp+988]
14090ef4f: mov         qword ptr [rsp+990],rax
14090ef57: mov         r9,qword ptr [rsp+990]
14090ef5f: lea         r8,[rsp+350]
14090ef67: lea         rdx,[rsp+090]
14090ef6f: lea         rcx,[rsp+78h]
14090ef74: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090ef7a: nop
14090ef7b: lea         rcx,[rsp+348]
14090ef83: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090ef89: nop
14090ef8a: lea         rcx,[rsp+350]
14090ef92: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090ef98: lea         rdx,[00000001427BB520h] ; 'numSamples'
14090ef9f: lea         rcx,[rsp+360]
14090efa7: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090efad: nop
14090efae: lea         rcx,[rsp+358]
14090efb6: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090efbc: mov         qword ptr [rsp+998],rax
14090efc4: mov         rax,qword ptr [rsp+998]
14090efcc: mov         qword ptr [rsp+9A0],rax
14090efd4: mov         r9,qword ptr [rsp+9A0]
14090efdc: lea         r8,[rsp+360]
14090efe4: lea         rdx,[rsp+368]
14090efec: lea         rcx,[rsp+78h]
14090eff1: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090eff7: mov         qword ptr [rsp+9A8],rax
14090efff: mov         rax,qword ptr [rsp+9A8]
14090f007: mov         qword ptr [rsp+9B0],rax
14090f00f: mov         r8d,0Ah
14090f015: xor         edx,edx
14090f017: mov         rcx,qword ptr [rsp+9B0]
14090f01f: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090f025: mov         dword ptr [rsp+0B4],eax
14090f02c: lea         rcx,[rsp+368]
14090f034: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f03a: nop
14090f03b: lea         rcx,[rsp+358]
14090f043: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f049: nop
14090f04a: lea         rcx,[rsp+360]
14090f052: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f058: lea         rax,[rsp+370]
14090f060: mov         qword ptr [rsp+378],rax
14090f068: lea         rdx,[rsp+090]
14090f070: mov         rcx,qword ptr [rsp+378]
14090f078: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14090f07e: mov         qword ptr [rsp+9B8],rax
14090f086: mov         rax,qword ptr [rsp+0000000000001030h]
14090f08e: mov         rax,qword ptr [rax]
14090f091: xor         r9d,r9d
14090f094: mov         r8d,dword ptr [rsp+0B4]
14090f09c: mov         rdx,qword ptr [rsp+9B8]
14090f0a4: mov         rcx,qword ptr [rsp+0000000000001030h]
14090f0ac: call        qword ptr [rax+00000000000000F8h]
14090f0b2: mov         rax,qword ptr [rsp+0000000000001030h]
14090f0ba: mov         rax,qword ptr [rax]
14090f0bd: mov         rcx,qword ptr [rsp+0000000000001030h]
14090f0c5: call        qword ptr [rax+0000000000000178h]
14090f0cb: sub         eax,1
14090f0ce: mov         dword ptr [rsp+0B0],eax
14090f0d5: mov         rax,qword ptr [rsp+0000000000001030h]
14090f0dd: mov         rax,qword ptr [rax]
14090f0e0: mov         edx,dword ptr [rsp+0B0]
14090f0e7: mov         rcx,qword ptr [rsp+0000000000001030h]
14090f0ef: call        qword ptr [rax+00000000000001A0h]
14090f0f5: mov         qword ptr [rsp+0A8],rax
14090f0fd: mov         dword ptr [rsp+080],0
14090f108: lea         rdx,[00000001427BB52Ch] ; 'varId'
14090f10f: lea         rcx,[rsp+388]
14090f117: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f11d: nop
14090f11e: lea         rdx,[rsp+388]
14090f126: lea         rcx,[rsp+78h]
14090f12b: call        qword ptr [141E0C438h] ; QtXml4_64.dll!?hasAttribute@QDomElement@@QEBA_NAEBVQString@@@Z
14090f131: mov         byte ptr [rsp+380],al
14090f138: lea         rcx,[rsp+388]
14090f140: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f146: movzx       edx,byte ptr [rsp+380]
14090f14e: test        edx,edx
14090f150: je          14090F22A
14090f156: lea         rdx,[00000001427BB534h] ; 'varId'
14090f15d: lea         rcx,[rsp+398]
14090f165: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f16b: nop
14090f16c: lea         rcx,[rsp+390]
14090f174: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f17a: mov         qword ptr [rsp+9C0],rax
14090f182: mov         rax,qword ptr [rsp+9C0]
14090f18a: mov         qword ptr [rsp+9C8],rax
14090f192: mov         r9,qword ptr [rsp+9C8]
14090f19a: lea         r8,[rsp+398]
14090f1a2: lea         rdx,[rsp+3A0]
14090f1aa: lea         rcx,[rsp+78h]
14090f1af: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f1b5: mov         qword ptr [rsp+9D0],rax
14090f1bd: mov         rax,qword ptr [rsp+9D0]
14090f1c5: mov         qword ptr [rsp+9D8],rax
14090f1cd: mov         r8d,0Ah
14090f1d3: xor         edx,edx
14090f1d5: mov         rcx,qword ptr [rsp+9D8]
14090f1dd: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090f1e3: mov         dword ptr [rsp+080],eax
14090f1ea: lea         rcx,[rsp+3A0]
14090f1f2: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f1f8: nop
14090f1f9: lea         rcx,[rsp+390]
14090f201: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f207: nop
14090f208: lea         rcx,[rsp+398]
14090f210: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f216: mov         edx,dword ptr [rsp+080]
14090f21d: mov         rcx,qword ptr [rsp+0A8]
14090f225: call        1409FF590
14090f22a: mov         dword ptr [rsp+084],3
14090f235: lea         rdx,[00000001427BB540h] ; 'dimension'
14090f23c: lea         rcx,[rsp+3B0]
14090f244: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f24a: nop
14090f24b: lea         rdx,[rsp+3B0]
14090f253: lea         rcx,[rsp+78h]
14090f258: call        qword ptr [141E0C438h] ; QtXml4_64.dll!?hasAttribute@QDomElement@@QEBA_NAEBVQString@@@Z
14090f25e: mov         byte ptr [rsp+3A8],al
14090f265: lea         rcx,[rsp+3B0]
14090f26d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f273: movzx       edx,byte ptr [rsp+3A8]
14090f27b: test        edx,edx
14090f27d: je          14090F357
14090f283: lea         rdx,[00000001427BB550h] ; 'dimension'
14090f28a: lea         rcx,[rsp+3C0]
14090f292: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f298: nop
14090f299: lea         rcx,[rsp+3B8]
14090f2a1: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f2a7: mov         qword ptr [rsp+9E0],rax
14090f2af: mov         rax,qword ptr [rsp+9E0]
14090f2b7: mov         qword ptr [rsp+9E8],rax
14090f2bf: mov         r9,qword ptr [rsp+9E8]
14090f2c7: lea         r8,[rsp+3C0]
14090f2cf: lea         rdx,[rsp+3C8]
14090f2d7: lea         rcx,[rsp+78h]
14090f2dc: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f2e2: mov         qword ptr [rsp+9F0],rax
14090f2ea: mov         rax,qword ptr [rsp+9F0]
14090f2f2: mov         qword ptr [rsp+9F8],rax
14090f2fa: mov         r8d,0Ah
14090f300: xor         edx,edx
14090f302: mov         rcx,qword ptr [rsp+9F8]
14090f30a: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090f310: mov         dword ptr [rsp+084],eax
14090f317: lea         rcx,[rsp+3C8]
14090f31f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f325: nop
14090f326: lea         rcx,[rsp+3B8]
14090f32e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f334: nop
14090f335: lea         rcx,[rsp+3C0]
14090f33d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f343: mov         edx,dword ptr [rsp+084]
14090f34a: mov         rcx,qword ptr [rsp+0A8]
14090f352: call        1400D8040
14090f357: lea         rdx,[00000001427BB55Ch] ; 'LinkTo'
14090f35e: lea         rcx,[rsp+3D8]
14090f366: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f36c: nop
14090f36d: lea         rcx,[rsp+3D0]
14090f375: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f37b: mov         qword ptr [rsp+A00],rax
14090f383: mov         rax,qword ptr [rsp+A00]
14090f38b: mov         qword ptr [rsp+A08],rax
14090f393: mov         r9,qword ptr [rsp+A08]
14090f39b: lea         r8,[rsp+3D8]
14090f3a3: lea         rdx,[rsp+088]
14090f3ab: lea         rcx,[rsp+78h]
14090f3b0: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f3b6: nop
14090f3b7: lea         rcx,[rsp+3D0]
14090f3bf: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f3c5: nop
14090f3c6: lea         rcx,[rsp+3D8]
14090f3ce: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f3d4: lea         rcx,[rsp+088]
14090f3dc: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
14090f3e2: movzx       eax,al
14090f3e5: test        eax,eax
14090f3e7: jne         14090F40D
14090f3e9: lea         rdx,[rsp+0A8]
14090f3f1: lea         rcx,[rsp+70h]
14090f3f6: call        14172D7E0
14090f3fb: lea         rdx,[rsp+088]
14090f403: lea         rcx,[rsp+68h]
14090f408: call        140004B90
14090f40d: mov         dword ptr [rsp+0A0],0
14090f418: lea         rdx,[rsp+098]
14090f420: lea         rcx,[rsp+78h]
14090f425: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
14090f42b: nop
14090f42c: lea         rcx,[rsp+098]
14090f434: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090f43a: movzx       eax,al
14090f43d: test        eax,eax
14090f43f: jne         140912819
14090f445: lea         rdx,[rsp+0B8]
14090f44d: lea         rcx,[rsp+098]
14090f455: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
14090f45b: nop
14090f45c: lea         rcx,[rsp+0B8]
14090f464: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090f46a: movzx       eax,al
14090f46d: test        eax,eax
14090f46f: jne         1409127B2
14090f475: lea         rdx,[rsp+3E8]
14090f47d: lea         rcx,[rsp+0B8]
14090f485: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090f48b: mov         qword ptr [rsp+A10],rax
14090f493: mov         rax,qword ptr [rsp+A10]
14090f49b: mov         qword ptr [rsp+A18],rax
14090f4a3: lea         rdx,[00000001427BB568h] ; 'Description'
14090f4aa: mov         rcx,qword ptr [rsp+A18]
14090f4b2: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090f4b8: mov         byte ptr [rsp+3E0],al
14090f4bf: lea         rcx,[rsp+3E8]
14090f4c7: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f4cd: movzx       edx,byte ptr [rsp+3E0]
14090f4d5: test        edx,edx
14090f4d7: je          14090F557
14090f4d9: lea         rdx,[rsp+0C0]
14090f4e1: lea         rcx,[rsp+0B8]
14090f4e9: call        qword ptr [141E0C5A0h] ; QtXml4_64.dll!?text@QDomElement@@QEBA?AVQString@@XZ
14090f4ef: nop
14090f4f0: lea         rcx,[rsp+0C0]
14090f4f8: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
14090f4fe: movzx       eax,al
14090f501: test        eax,eax
14090f503: jne         14090F549
14090f505: lea         rax,[rsp+3F0]
14090f50d: mov         qword ptr [rsp+3F8],rax
14090f515: lea         rdx,[rsp+0C0]
14090f51d: mov         rcx,qword ptr [rsp+3F8]
14090f525: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14090f52b: mov         qword ptr [rsp+A20],rax
14090f533: mov         rdx,qword ptr [rsp+A20]
14090f53b: mov         rcx,qword ptr [rsp+0A8]
14090f543: call        14010F200
14090f548: nop
14090f549: lea         rcx,[rsp+0C0]
14090f551: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f557: lea         rdx,[rsp+408]
14090f55f: lea         rcx,[rsp+0B8]
14090f567: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090f56d: mov         qword ptr [rsp+A28],rax
14090f575: mov         rax,qword ptr [rsp+A28]
14090f57d: mov         qword ptr [rsp+A30],rax
14090f585: lea         rdx,[00000001427BB578h] ; 'Keyword'
14090f58c: mov         rcx,qword ptr [rsp+A30]
14090f594: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090f59a: mov         byte ptr [rsp+400],al
14090f5a1: lea         rcx,[rsp+408]
14090f5a9: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f5af: movzx       edx,byte ptr [rsp+400]
14090f5b7: test        edx,edx
14090f5b9: je          1409114CA
14090f5bf: lea         rcx,[rsp+110]
14090f5c7: call        1400653E0
14090f5cc: nop
14090f5cd: lea         rcx,[rsp+0E8]
14090f5d5: call        14018F9E0
14090f5da: nop
14090f5db: lea         rdx,[00000001427BB580h] ; 'name'
14090f5e2: lea         rcx,[rsp+418]
14090f5ea: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f5f0: nop
14090f5f1: lea         rcx,[rsp+410]
14090f5f9: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f5ff: mov         qword ptr [rsp+A38],rax
14090f607: mov         rax,qword ptr [rsp+A38]
14090f60f: mov         qword ptr [rsp+A40],rax
14090f617: mov         r9,qword ptr [rsp+A40]
14090f61f: lea         r8,[rsp+418]
14090f627: lea         rdx,[rsp+128]
14090f62f: lea         rcx,[rsp+0B8]
14090f637: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f63d: nop
14090f63e: lea         rcx,[rsp+410]
14090f646: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f64c: nop
14090f64d: lea         rcx,[rsp+418]
14090f655: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f65b: lea         rdx,[00000001427BB588h] ; 'unitType'
14090f662: lea         rcx,[rsp+428]
14090f66a: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f670: nop
14090f671: lea         rcx,[rsp+420]
14090f679: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f67f: mov         qword ptr [rsp+A48],rax
14090f687: mov         rax,qword ptr [rsp+A48]
14090f68f: mov         qword ptr [rsp+A50],rax
14090f697: mov         r9,qword ptr [rsp+A50]
14090f69f: lea         r8,[rsp+428]
14090f6a7: lea         rdx,[rsp+430]
14090f6af: lea         rcx,[rsp+0B8]
14090f6b7: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f6bd: mov         qword ptr [rsp+A58],rax
14090f6c5: mov         rax,qword ptr [rsp+A58]
14090f6cd: mov         qword ptr [rsp+A60],rax
14090f6d5: mov         r8d,0Ah
14090f6db: xor         edx,edx
14090f6dd: mov         rcx,qword ptr [rsp+A60]
14090f6e5: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090f6eb: mov         dword ptr [rsp+144],eax
14090f6f2: lea         rcx,[rsp+430]
14090f6fa: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f700: nop
14090f701: lea         rcx,[rsp+420]
14090f709: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f70f: nop
14090f710: lea         rcx,[rsp+428]
14090f718: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f71e: lea         rdx,[00000001427BB598h] ; 'parmType'
14090f725: lea         rcx,[rsp+440]
14090f72d: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f733: nop
14090f734: lea         rcx,[rsp+438]
14090f73c: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f742: mov         qword ptr [rsp+A68],rax
14090f74a: mov         rax,qword ptr [rsp+A68]
14090f752: mov         qword ptr [rsp+A70],rax
14090f75a: mov         r9,qword ptr [rsp+A70]
14090f762: lea         r8,[rsp+440]
14090f76a: lea         rdx,[rsp+448]
14090f772: lea         rcx,[rsp+0B8]
14090f77a: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f780: mov         qword ptr [rsp+A78],rax
14090f788: mov         rax,qword ptr [rsp+A78]
14090f790: mov         qword ptr [rsp+A80],rax
14090f798: lea         rdx,[rsp+100]
14090f7a0: mov         rcx,qword ptr [rsp+A80]
14090f7a8: call        qword ptr [141E04328h] ; QtCore4_64.dll!?toLower@QString@@QEBA?AV1@XZ
14090f7ae: nop
14090f7af: lea         rcx,[rsp+448]
14090f7b7: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f7bd: nop
14090f7be: lea         rcx,[rsp+438]
14090f7c6: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f7cc: nop
14090f7cd: lea         rcx,[rsp+440]
14090f7d5: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f7db: mov         byte ptr [rsp+140],0
14090f7e3: mov         byte ptr [rsp+0F8],0
14090f7eb: lea         rdx,[00000001427BB5A8h] ; 'function'
14090f7f2: lea         rcx,[rsp+100]
14090f7fa: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090f800: movzx       eax,al
14090f803: test        eax,eax
14090f805: je          14090F811
14090f807: mov         byte ptr [rsp+140],1
14090f80f: jmp         14090F835
14090f811: lea         rdx,[00000001427BB5B4h] ; 'enum'
14090f818: lea         rcx,[rsp+100]
14090f820: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090f826: movzx       eax,al
14090f829: test        eax,eax
14090f82b: je          14090F835
14090f82d: mov         byte ptr [rsp+0F8],1
14090f835: lea         rdx,[00000001427BB5C0h] ; 'parameter'
14090f83c: lea         rcx,[rsp+458]
14090f844: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f84a: nop
14090f84b: lea         rcx,[rsp+450]
14090f853: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f859: mov         qword ptr [rsp+A88],rax
14090f861: mov         rax,qword ptr [rsp+A88]
14090f869: mov         qword ptr [rsp+A90],rax
14090f871: mov         r9,qword ptr [rsp+A90]
14090f879: lea         r8,[rsp+458]
14090f881: lea         rdx,[rsp+460]
14090f889: lea         rcx,[rsp+0B8]
14090f891: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f897: mov         qword ptr [rsp+A98],rax
14090f89f: mov         rax,qword ptr [rsp+A98]
14090f8a7: mov         qword ptr [rsp+AA0],rax
14090f8af: mov         r8d,0Ah
14090f8b5: xor         edx,edx
14090f8b7: mov         rcx,qword ptr [rsp+AA0]
14090f8bf: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090f8c5: mov         dword ptr [rsp+0E0],eax
14090f8cc: lea         rcx,[rsp+460]
14090f8d4: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f8da: nop
14090f8db: lea         rcx,[rsp+450]
14090f8e3: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f8e9: nop
14090f8ea: lea         rcx,[rsp+458]
14090f8f2: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f8f8: lea         rcx,[rsp+108]
14090f900: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f906: nop
14090f907: lea         rdx,[00000001427BB5D0h] ; 'parameterName'
14090f90e: lea         rcx,[rsp+470]
14090f916: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f91c: nop
14090f91d: lea         rcx,[rsp+468]
14090f925: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f92b: mov         qword ptr [rsp+AA8],rax
14090f933: mov         rax,qword ptr [rsp+AA8]
14090f93b: mov         qword ptr [rsp+AB0],rax
14090f943: mov         r9,qword ptr [rsp+AB0]
14090f94b: lea         r8,[rsp+470]
14090f953: lea         rdx,[rsp+478]
14090f95b: lea         rcx,[rsp+0B8]
14090f963: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090f969: mov         qword ptr [rsp+AB8],rax
14090f971: mov         rax,qword ptr [rsp+AB8]
14090f979: mov         qword ptr [rsp+AC0],rax
14090f981: mov         rdx,qword ptr [rsp+AC0]
14090f989: lea         rcx,[rsp+108]
14090f991: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
14090f997: nop
14090f998: lea         rcx,[rsp+478]
14090f9a0: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f9a6: nop
14090f9a7: lea         rcx,[rsp+468]
14090f9af: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f9b5: nop
14090f9b6: lea         rcx,[rsp+470]
14090f9be: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090f9c4: lea         rdx,[00000001427BB5E0h] ; 'nominalValue'
14090f9cb: lea         rcx,[rsp+488]
14090f9d3: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090f9d9: nop
14090f9da: lea         rcx,[rsp+480]
14090f9e2: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090f9e8: mov         qword ptr [rsp+AC8],rax
14090f9f0: mov         rax,qword ptr [rsp+AC8]
14090f9f8: mov         qword ptr [rsp+AD0],rax
14090fa00: mov         r9,qword ptr [rsp+AD0]
14090fa08: lea         r8,[rsp+488]
14090fa10: lea         rdx,[rsp+490]
14090fa18: lea         rcx,[rsp+0B8]
14090fa20: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090fa26: mov         qword ptr [rsp+AD8],rax
14090fa2e: mov         rax,qword ptr [rsp+AD8]
14090fa36: mov         qword ptr [rsp+AE0],rax
14090fa3e: xor         edx,edx
14090fa40: mov         rcx,qword ptr [rsp+AE0]
14090fa48: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
14090fa4e: movsd       mmword ptr [rsp+0D0],xmm0
14090fa57: lea         rcx,[rsp+490]
14090fa5f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fa65: nop
14090fa66: lea         rcx,[rsp+480]
14090fa6e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fa74: nop
14090fa75: lea         rcx,[rsp+488]
14090fa7d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fa83: movzx       eax,byte ptr [rsp+140]
14090fa8b: test        eax,eax
14090fa8d: je          14090FB56
14090fa93: lea         rdx,[00000001427BB5F0h] ; 'funcEntryNo'
14090fa9a: lea         rcx,[rsp+4A0]
14090faa2: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090faa8: nop
14090faa9: lea         rcx,[rsp+498]
14090fab1: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090fab7: mov         qword ptr [rsp+AE8],rax
14090fabf: mov         rax,qword ptr [rsp+AE8]
14090fac7: mov         qword ptr [rsp+AF0],rax
14090facf: mov         r9,qword ptr [rsp+AF0]
14090fad7: lea         r8,[rsp+4A0]
14090fadf: lea         rdx,[rsp+4A8]
14090fae7: lea         rcx,[rsp+0B8]
14090faef: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090faf5: mov         qword ptr [rsp+AF8],rax
14090fafd: mov         rax,qword ptr [rsp+AF8]
14090fb05: mov         qword ptr [rsp+B00],rax
14090fb0d: mov         r8d,0Ah
14090fb13: xor         edx,edx
14090fb15: mov         rcx,qword ptr [rsp+B00]
14090fb1d: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090fb23: mov         dword ptr [rsp+0C8],eax
14090fb2a: lea         rcx,[rsp+4A8]
14090fb32: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fb38: nop
14090fb39: lea         rcx,[rsp+498]
14090fb41: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fb47: nop
14090fb48: lea         rcx,[rsp+4A0]
14090fb50: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fb56: lea         rcx,[rsp+120]
14090fb5e: call        14018F9E0
14090fb63: nop
14090fb64: lea         rcx,[rsp+138]
14090fb6c: call        14018F9E0
14090fb71: nop
14090fb72: lea         rcx,[rsp+0D8]
14090fb7a: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090fb80: nop
14090fb81: lea         rcx,[rsp+118]
14090fb89: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090fb8f: nop
14090fb90: lea         rcx,[rsp+0F0]
14090fb98: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090fb9e: nop
14090fb9f: lea         rdx,[rsp+130]
14090fba7: lea         rcx,[rsp+0B8]
14090fbaf: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
14090fbb5: nop
14090fbb6: lea         rcx,[rsp+130]
14090fbbe: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090fbc4: movzx       eax,al
14090fbc7: test        eax,eax
14090fbc9: jne         140910843
14090fbcf: lea         rdx,[rsp+150]
14090fbd7: lea         rcx,[rsp+130]
14090fbdf: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
14090fbe5: nop
14090fbe6: lea         rcx,[rsp+150]
14090fbee: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090fbf4: movzx       eax,al
14090fbf7: test        eax,eax
14090fbf9: jne         1409107DC
14090fbff: lea         rdx,[rsp+4B8]
14090fc07: lea         rcx,[rsp+150]
14090fc0f: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090fc15: mov         qword ptr [rsp+B08],rax
14090fc1d: mov         rax,qword ptr [rsp+B08]
14090fc25: mov         qword ptr [rsp+B10],rax
14090fc2d: lea         rdx,[00000001427BB600h] ; 'EnumData'
14090fc34: mov         rcx,qword ptr [rsp+B10]
14090fc3c: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090fc42: mov         byte ptr [rsp+4B0],al
14090fc49: lea         rcx,[rsp+4B8]
14090fc51: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fc57: movzx       edx,byte ptr [rsp+4B0]
14090fc5f: test        edx,edx
14090fc61: je          14090FDFC
14090fc67: lea         rdx,[00000001427BB60Ch] ; 'Text'
14090fc6e: lea         rcx,[rsp+4C8]
14090fc76: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090fc7c: nop
14090fc7d: lea         rcx,[rsp+4C0]
14090fc85: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090fc8b: mov         qword ptr [rsp+B18],rax
14090fc93: mov         rax,qword ptr [rsp+B18]
14090fc9b: mov         qword ptr [rsp+B20],rax
14090fca3: mov         r9,qword ptr [rsp+B20]
14090fcab: lea         r8,[rsp+4C8]
14090fcb3: lea         rdx,[rsp+4D0]
14090fcbb: lea         rcx,[rsp+150]
14090fcc3: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090fcc9: mov         qword ptr [rsp+B28],rax
14090fcd1: mov         rax,qword ptr [rsp+B28]
14090fcd9: mov         qword ptr [rsp+B30],rax
14090fce1: mov         rdx,qword ptr [rsp+B30]
14090fce9: lea         rcx,[rsp+110]
14090fcf1: call        140004B90
14090fcf6: nop
14090fcf7: lea         rcx,[rsp+4D0]
14090fcff: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fd05: nop
14090fd06: lea         rcx,[rsp+4C0]
14090fd0e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fd14: nop
14090fd15: lea         rcx,[rsp+4C8]
14090fd1d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fd23: lea         rdx,[00000001427BB614h] ; 'Value'
14090fd2a: lea         rcx,[rsp+4E8]
14090fd32: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
14090fd38: nop
14090fd39: lea         rcx,[rsp+4E0]
14090fd41: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14090fd47: mov         qword ptr [rsp+B38],rax
14090fd4f: mov         rax,qword ptr [rsp+B38]
14090fd57: mov         qword ptr [rsp+B40],rax
14090fd5f: mov         r9,qword ptr [rsp+B40]
14090fd67: lea         r8,[rsp+4E8]
14090fd6f: lea         rdx,[rsp+4F0]
14090fd77: lea         rcx,[rsp+150]
14090fd7f: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14090fd85: mov         qword ptr [rsp+B48],rax
14090fd8d: mov         rax,qword ptr [rsp+B48]
14090fd95: mov         qword ptr [rsp+B50],rax
14090fd9d: mov         r8d,0Ah
14090fda3: xor         edx,edx
14090fda5: mov         rcx,qword ptr [rsp+B50]
14090fdad: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14090fdb3: mov         dword ptr [rsp+4D8],eax
14090fdba: lea         rdx,[rsp+4D8]
14090fdc2: lea         rcx,[rsp+0E8]
14090fdca: call        140025100
14090fdcf: nop
14090fdd0: lea         rcx,[rsp+4F0]
14090fdd8: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fdde: nop
14090fddf: lea         rcx,[rsp+4E0]
14090fde7: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fded: nop
14090fdee: lea         rcx,[rsp+4E8]
14090fdf6: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fdfc: lea         rdx,[rsp+500]
14090fe04: lea         rcx,[rsp+150]
14090fe0c: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090fe12: mov         qword ptr [rsp+B58],rax
14090fe1a: mov         rax,qword ptr [rsp+B58]
14090fe22: mov         qword ptr [rsp+B60],rax
14090fe2a: lea         rdx,[00000001427BB620h] ; 'NominalData'
14090fe31: mov         rcx,qword ptr [rsp+B60]
14090fe39: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090fe3f: mov         byte ptr [rsp+4F8],al
14090fe46: lea         rcx,[rsp+500]
14090fe4e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fe54: movzx       edx,byte ptr [rsp+4F8]
14090fe5c: test        edx,edx
14090fe5e: je          14090FF99
14090fe64: lea         rdx,[rsp+158]
14090fe6c: lea         rcx,[rsp+150]
14090fe74: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
14090fe7a: nop
14090fe7b: lea         rcx,[rsp+158]
14090fe83: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090fe89: movzx       eax,al
14090fe8c: test        eax,eax
14090fe8e: jne         14090FF8B
14090fe94: lea         rdx,[rsp+160]
14090fe9c: lea         rcx,[rsp+158]
14090fea4: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
14090feaa: nop
14090feab: lea         rcx,[rsp+160]
14090feb3: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14090feb9: movzx       eax,al
14090febc: test        eax,eax
14090febe: jne         14090FF7C
14090fec4: lea         rdx,[rsp+510]
14090fecc: lea         rcx,[rsp+160]
14090fed4: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090feda: mov         qword ptr [rsp+B68],rax
14090fee2: mov         rax,qword ptr [rsp+B68]
14090feea: mov         qword ptr [rsp+B70],rax
14090fef2: lea         rdx,[00000001427BB62Ch] ; 'string'
14090fef9: mov         rcx,qword ptr [rsp+B70]
14090ff01: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090ff07: mov         byte ptr [rsp+508],al
14090ff0e: lea         rcx,[rsp+510]
14090ff16: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090ff1c: movzx       edx,byte ptr [rsp+508]
14090ff24: test        edx,edx
14090ff26: je          14090FF7C
14090ff28: lea         rdx,[rsp+518]
14090ff30: lea         rcx,[rsp+160]
14090ff38: call        qword ptr [141E0C5A0h] ; QtXml4_64.dll!?text@QDomElement@@QEBA?AVQString@@XZ
14090ff3e: mov         qword ptr [rsp+B78],rax
14090ff46: mov         rax,qword ptr [rsp+B78]
14090ff4e: mov         qword ptr [rsp+B80],rax
14090ff56: mov         rdx,qword ptr [rsp+B80]
14090ff5e: lea         rcx,[rsp+0D8]
14090ff66: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
14090ff6c: nop
14090ff6d: lea         rcx,[rsp+518]
14090ff75: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090ff7b: nop
14090ff7c: lea         rcx,[rsp+160]
14090ff84: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
14090ff8a: nop
14090ff8b: lea         rcx,[rsp+158]
14090ff93: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
14090ff99: lea         rdx,[rsp+528]
14090ffa1: lea         rcx,[rsp+150]
14090ffa9: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14090ffaf: mov         qword ptr [rsp+B88],rax
14090ffb7: mov         rax,qword ptr [rsp+B88]
14090ffbf: mov         qword ptr [rsp+B90],rax
14090ffc7: lea         rdx,[00000001427BB638h] ; 'UpperData'
14090ffce: mov         rcx,qword ptr [rsp+B90]
14090ffd6: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14090ffdc: mov         byte ptr [rsp+520],al
14090ffe3: lea         rcx,[rsp+528]
14090ffeb: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14090fff1: movzx       edx,byte ptr [rsp+520]
14090fff9: test        edx,edx
14090fffb: je          14091013B
140910001: lea         rdx,[rsp+168]
140910009: lea         rcx,[rsp+150]
140910011: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
140910017: nop
140910018: lea         rcx,[rsp+168]
140910020: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140910026: movzx       eax,al
140910029: test        eax,eax
14091002b: jne         140910128
140910031: lea         rdx,[rsp+170]
140910039: lea         rcx,[rsp+168]
140910041: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
140910047: nop
140910048: lea         rcx,[rsp+170]
140910050: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140910056: movzx       eax,al
140910059: test        eax,eax
14091005b: jne         140910119
140910061: lea         rdx,[rsp+538]
140910069: lea         rcx,[rsp+170]
140910071: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
140910077: mov         qword ptr [rsp+B98],rax
14091007f: mov         rax,qword ptr [rsp+B98]
140910087: mov         qword ptr [rsp+BA0],rax
14091008f: lea         rdx,[00000001427BB644h] ; 'string'
140910096: mov         rcx,qword ptr [rsp+BA0]
14091009e: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
1409100a4: mov         byte ptr [rsp+530],al
1409100ab: lea         rcx,[rsp+538]
1409100b3: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409100b9: movzx       edx,byte ptr [rsp+530]
1409100c1: test        edx,edx
1409100c3: je          140910119
1409100c5: lea         rdx,[rsp+540]
1409100cd: lea         rcx,[rsp+170]
1409100d5: call        qword ptr [141E0C5A0h] ; QtXml4_64.dll!?text@QDomElement@@QEBA?AVQString@@XZ
1409100db: mov         qword ptr [rsp+BA8],rax
1409100e3: mov         rax,qword ptr [rsp+BA8]
1409100eb: mov         qword ptr [rsp+BB0],rax
1409100f3: mov         rdx,qword ptr [rsp+BB0]
1409100fb: lea         rcx,[rsp+118]
140910103: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
140910109: nop
14091010a: lea         rcx,[rsp+540]
140910112: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910118: nop
140910119: lea         rcx,[rsp+170]
140910121: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
140910127: nop
140910128: lea         rcx,[rsp+168]
140910130: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140910136: jmp         1409107DC
14091013b: lea         rdx,[rsp+550]
140910143: lea         rcx,[rsp+150]
14091014b: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
140910151: mov         qword ptr [rsp+BB8],rax
140910159: mov         rax,qword ptr [rsp+BB8]
140910161: mov         qword ptr [rsp+BC0],rax
140910169: lea         rdx,[00000001427BB650h] ; 'LowerData'
140910170: mov         rcx,qword ptr [rsp+BC0]
140910178: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14091017e: mov         byte ptr [rsp+548],al
140910185: lea         rcx,[rsp+550]
14091018d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910193: movzx       edx,byte ptr [rsp+548]
14091019b: test        edx,edx
14091019d: je          1409102DD
1409101a3: lea         rdx,[rsp+178]
1409101ab: lea         rcx,[rsp+150]
1409101b3: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
1409101b9: nop
1409101ba: lea         rcx,[rsp+178]
1409101c2: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
1409101c8: movzx       eax,al
1409101cb: test        eax,eax
1409101cd: jne         1409102CA
1409101d3: lea         rdx,[rsp+180]
1409101db: lea         rcx,[rsp+178]
1409101e3: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
1409101e9: nop
1409101ea: lea         rcx,[rsp+180]
1409101f2: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
1409101f8: movzx       eax,al
1409101fb: test        eax,eax
1409101fd: jne         1409102BB
140910203: lea         rdx,[rsp+560]
14091020b: lea         rcx,[rsp+180]
140910213: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
140910219: mov         qword ptr [rsp+BC8],rax
140910221: mov         rax,qword ptr [rsp+BC8]
140910229: mov         qword ptr [rsp+BD0],rax
140910231: lea         rdx,[00000001427BB65Ch] ; 'string'
140910238: mov         rcx,qword ptr [rsp+BD0]
140910240: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
140910246: mov         byte ptr [rsp+558],al
14091024d: lea         rcx,[rsp+560]
140910255: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091025b: movzx       edx,byte ptr [rsp+558]
140910263: test        edx,edx
140910265: je          1409102BB
140910267: lea         rdx,[rsp+568]
14091026f: lea         rcx,[rsp+180]
140910277: call        qword ptr [141E0C5A0h] ; QtXml4_64.dll!?text@QDomElement@@QEBA?AVQString@@XZ
14091027d: mov         qword ptr [rsp+BD8],rax
140910285: mov         rax,qword ptr [rsp+BD8]
14091028d: mov         qword ptr [rsp+BE0],rax
140910295: mov         rdx,qword ptr [rsp+BE0]
14091029d: lea         rcx,[rsp+0F0]
1409102a5: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
1409102ab: nop
1409102ac: lea         rcx,[rsp+568]
1409102b4: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409102ba: nop
1409102bb: lea         rcx,[rsp+180]
1409102c3: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
1409102c9: nop
1409102ca: lea         rcx,[rsp+178]
1409102d2: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
1409102d8: jmp         1409107DC
1409102dd: lea         rdx,[rsp+578]
1409102e5: lea         rcx,[rsp+150]
1409102ed: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
1409102f3: mov         qword ptr [rsp+BE8],rax
1409102fb: mov         rax,qword ptr [rsp+BE8]
140910303: mov         qword ptr [rsp+BF0],rax
14091030b: lea         rdx,[00000001427BB668h] ; 'KwEntities'
140910312: mov         rcx,qword ptr [rsp+BF0]
14091031a: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
140910320: mov         byte ptr [rsp+570],al
140910327: lea         rcx,[rsp+578]
14091032f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910335: movzx       edx,byte ptr [rsp+570]
14091033d: test        edx,edx
14091033f: je          14091055F
140910345: lea         rdx,[rsp+188]
14091034d: lea         rcx,[rsp+150]
140910355: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
14091035b: nop
14091035c: lea         rcx,[rsp+188]
140910364: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14091036a: movzx       eax,al
14091036d: test        eax,eax
14091036f: jne         14091054C
140910375: lea         rdx,[rsp+190]
14091037d: lea         rcx,[rsp+188]
140910385: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
14091038b: nop
14091038c: lea         rcx,[rsp+190]
140910394: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14091039a: movzx       eax,al
14091039d: test        eax,eax
14091039f: jne         1409104E5
1409103a5: lea         rdx,[rsp+588]
1409103ad: lea         rcx,[rsp+190]
1409103b5: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
1409103bb: mov         qword ptr [rsp+BF8],rax
1409103c3: mov         rax,qword ptr [rsp+BF8]
1409103cb: mov         qword ptr [rsp+C00],rax
1409103d3: lea         rdx,[00000001427BB678h] ; 'KwEntity'
1409103da: mov         rcx,qword ptr [rsp+C00]
1409103e2: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
1409103e8: mov         byte ptr [rsp+580],al
1409103ef: lea         rcx,[rsp+588]
1409103f7: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409103fd: movzx       edx,byte ptr [rsp+580]
140910405: test        edx,edx
140910407: je          1409104E5
14091040d: lea         rdx,[00000001427BB22Ch] ; 'id'
140910414: lea         rcx,[rsp+598]
14091041c: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140910422: nop
140910423: lea         rcx,[rsp+590]
14091042b: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140910431: mov         qword ptr [rsp+C08],rax
140910439: mov         rax,qword ptr [rsp+C08]
140910441: mov         qword ptr [rsp+C10],rax
140910449: mov         r9,qword ptr [rsp+C10]
140910451: lea         r8,[rsp+598]
140910459: lea         rdx,[rsp+5A0]
140910461: lea         rcx,[rsp+190]
140910469: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
14091046f: mov         qword ptr [rsp+C18],rax
140910477: mov         rax,qword ptr [rsp+C18]
14091047f: mov         qword ptr [rsp+C20],rax
140910487: mov         r8d,0Ah
14091048d: xor         edx,edx
14091048f: mov         rcx,qword ptr [rsp+C20]
140910497: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14091049d: mov         dword ptr [rsp+198],eax
1409104a4: lea         rcx,[rsp+5A0]
1409104ac: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409104b2: nop
1409104b3: lea         rcx,[rsp+590]
1409104bb: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409104c1: nop
1409104c2: lea         rcx,[rsp+598]
1409104ca: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409104d0: lea         rdx,[rsp+198]
1409104d8: lea         rcx,[rsp+138]
1409104e0: call        140025100
1409104e5: lea         rdx,[rsp+5A8]
1409104ed: lea         rcx,[rsp+188]
1409104f5: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
1409104fb: mov         qword ptr [rsp+C28],rax
140910503: mov         rax,qword ptr [rsp+C28]
14091050b: mov         qword ptr [rsp+C30],rax
140910513: mov         rdx,qword ptr [rsp+C30]
14091051b: lea         rcx,[rsp+188]
140910523: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
140910529: nop
14091052a: lea         rcx,[rsp+5A8]
140910532: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140910538: nop
140910539: lea         rcx,[rsp+190]
140910541: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
140910547: jmp         14091035C
14091054c: lea         rcx,[rsp+188]
140910554: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
14091055a: jmp         1409107DC
14091055f: lea         rdx,[rsp+5B8]
140910567: lea         rcx,[rsp+150]
14091056f: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
140910575: mov         qword ptr [rsp+C38],rax
14091057d: mov         rax,qword ptr [rsp+C38]
140910585: mov         qword ptr [rsp+C40],rax
14091058d: lea         rdx,[00000001427BB688h] ; 'KwOperations'
140910594: mov         rcx,qword ptr [rsp+C40]
14091059c: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
1409105a2: mov         byte ptr [rsp+5B0],al
1409105a9: lea         rcx,[rsp+5B8]
1409105b1: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409105b7: movzx       edx,byte ptr [rsp+5B0]
1409105bf: test        edx,edx
1409105c1: je          1409107DC
1409105c7: lea         rdx,[rsp+1A0]
1409105cf: lea         rcx,[rsp+150]
1409105d7: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
1409105dd: nop
1409105de: lea         rcx,[rsp+1A0]
1409105e6: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
1409105ec: movzx       eax,al
1409105ef: test        eax,eax
1409105f1: jne         1409107CE
1409105f7: lea         rdx,[rsp+1A8]
1409105ff: lea         rcx,[rsp+1A0]
140910607: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
14091060d: nop
14091060e: lea         rcx,[rsp+1A8]
140910616: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
14091061c: movzx       eax,al
14091061f: test        eax,eax
140910621: jne         140910767
140910627: lea         rdx,[rsp+5C8]
14091062f: lea         rcx,[rsp+1A8]
140910637: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14091063d: mov         qword ptr [rsp+C48],rax
140910645: mov         rax,qword ptr [rsp+C48]
14091064d: mov         qword ptr [rsp+C50],rax
140910655: lea         rdx,[00000001427BB698h] ; 'KwOperation'
14091065c: mov         rcx,qword ptr [rsp+C50]
140910664: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14091066a: mov         byte ptr [rsp+5C0],al
140910671: lea         rcx,[rsp+5C8]
140910679: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091067f: movzx       edx,byte ptr [rsp+5C0]
140910687: test        edx,edx
140910689: je          140910767
14091068f: lea         rdx,[00000001427BB2FCh] ; 'id'
140910696: lea         rcx,[rsp+5D8]
14091069e: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
1409106a4: nop
1409106a5: lea         rcx,[rsp+5D0]
1409106ad: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
1409106b3: mov         qword ptr [rsp+C58],rax
1409106bb: mov         rax,qword ptr [rsp+C58]
1409106c3: mov         qword ptr [rsp+C60],rax
1409106cb: mov         r9,qword ptr [rsp+C60]
1409106d3: lea         r8,[rsp+5D8]
1409106db: lea         rdx,[rsp+5E0]
1409106e3: lea         rcx,[rsp+1A8]
1409106eb: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
1409106f1: mov         qword ptr [rsp+C68],rax
1409106f9: mov         rax,qword ptr [rsp+C68]
140910701: mov         qword ptr [rsp+C70],rax
140910709: mov         r8d,0Ah
14091070f: xor         edx,edx
140910711: mov         rcx,qword ptr [rsp+C70]
140910719: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14091071f: mov         dword ptr [rsp+1B0],eax
140910726: lea         rcx,[rsp+5E0]
14091072e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910734: nop
140910735: lea         rcx,[rsp+5D0]
14091073d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910743: nop
140910744: lea         rcx,[rsp+5D8]
14091074c: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910752: lea         rdx,[rsp+1B0]
14091075a: lea         rcx,[rsp+120]
140910762: call        140025100
140910767: lea         rdx,[rsp+5E8]
14091076f: lea         rcx,[rsp+1A0]
140910777: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
14091077d: mov         qword ptr [rsp+C78],rax
140910785: mov         rax,qword ptr [rsp+C78]
14091078d: mov         qword ptr [rsp+C80],rax
140910795: mov         rdx,qword ptr [rsp+C80]
14091079d: lea         rcx,[rsp+1A0]
1409107a5: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
1409107ab: nop
1409107ac: lea         rcx,[rsp+5E8]
1409107b4: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
1409107ba: nop
1409107bb: lea         rcx,[rsp+1A8]
1409107c3: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
1409107c9: jmp         1409105DE
1409107ce: lea         rcx,[rsp+1A0]
1409107d6: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
1409107dc: lea         rdx,[rsp+5F0]
1409107e4: lea         rcx,[rsp+130]
1409107ec: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
1409107f2: mov         qword ptr [rsp+C88],rax
1409107fa: mov         rax,qword ptr [rsp+C88]
140910802: mov         qword ptr [rsp+C90],rax
14091080a: mov         rdx,qword ptr [rsp+C90]
140910812: lea         rcx,[rsp+130]
14091081a: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
140910820: nop
140910821: lea         rcx,[rsp+5F0]
140910829: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
14091082f: nop
140910830: lea         rcx,[rsp+150]
140910838: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
14091083e: jmp         14090FBB6
140910843: lea         rax,[rsp+5F8]
14091084b: mov         qword ptr [rsp+600],rax
140910853: lea         rax,[rsp+610]
14091085b: mov         qword ptr [rsp+618],rax
140910863: lea         rdx,[rsp+108]
14091086b: mov         rcx,qword ptr [rsp+600]
140910873: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140910879: mov         qword ptr [rsp+C98],rax
140910881: mov         rax,qword ptr [rsp+C98]
140910889: mov         qword ptr [rsp+CA0],rax
140910891: lea         rdx,[rsp+608]
140910899: lea         rcx,[rsp+128]
1409108a1: call        qword ptr [141E03F50h] ; QtCore4_64.dll!?toLocal8Bit@QString@@QEBA?AVQByteArray@@XZ
1409108a7: mov         qword ptr [rsp+CA8],rax
1409108af: mov         rax,qword ptr [rsp+CA8]
1409108b7: mov         qword ptr [rsp+CB0],rax
1409108bf: mov         rcx,qword ptr [rsp+CB0]
1409108c7: call        qword ptr [141E04228h] ; QtCore4_64.dll!?data@QByteArray@@QEAAPEADXZ
1409108cd: mov         rcx,rax
1409108d0: call        140921E90
1409108d5: mov         esi,eax
1409108d7: lea         rdx,[rsp+090]
1409108df: mov         rcx,qword ptr [rsp+618]
1409108e7: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
1409108ed: mov         qword ptr [rsp+CB8],rax
1409108f5: mov         rdi,qword ptr [rsp+0000000000001030h]
1409108fd: mov         rdi,qword ptr [rdi]
140910900: lea         rax,[rsp+138]
140910908: mov         qword ptr [rsp+40h],rax
14091090d: lea         rax,[rsp+120]
140910915: mov         qword ptr [rsp+38h],rax
14091091a: movsd       xmm0,mmword ptr [rsp+0D0]
140910923: movsd       mmword ptr [rsp+30h],xmm0
140910929: mov         eax,dword ptr [rsp+084]
140910930: mov         dword ptr [rsp+28h],eax
140910934: mov         eax,dword ptr [rsp+144]
14091093b: mov         dword ptr [rsp+20h],eax
14091093f: mov         r9,qword ptr [rsp+CA0]
140910947: mov         r8d,esi
14091094a: mov         rdx,qword ptr [rsp+CB8]
140910952: mov         rcx,qword ptr [rsp+0000000000001030h]
14091095a: call        qword ptr [rdi+0000000000000118h]
140910960: nop
140910961: lea         rcx,[rsp+608]
140910969: call        qword ptr [141E04290h] ; QtCore4_64.dll!??1QByteArray@@QEAA@XZ
14091096f: lea         rax,[rsp+620]
140910977: mov         qword ptr [rsp+628],rax
14091097f: lea         rax,[rsp+630]
140910987: mov         qword ptr [rsp+638],rax
14091098f: lea         rdx,[rsp+090]
140910997: mov         rcx,qword ptr [rsp+628]
14091099f: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
1409109a5: mov         qword ptr [rsp+CC0],rax
1409109ad: mov         rax,qword ptr [rsp+0000000000001030h]
1409109b5: mov         rax,qword ptr [rax]
1409109b8: mov         rdx,qword ptr [rsp+CC0]
1409109c0: mov         rcx,qword ptr [rsp+0000000000001030h]
1409109c8: call        qword ptr [rax+0000000000000128h]
1409109ce: mov         dword ptr [rsp+CC8],eax
1409109d5: mov         edi,dword ptr [rsp+CC8]
1409109dc: sub         edi,1
1409109df: lea         rdx,[rsp+090]
1409109e7: mov         rcx,qword ptr [rsp+638]
1409109ef: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
1409109f5: mov         qword ptr [rsp+CD0],rax
1409109fd: mov         rax,qword ptr [rsp+0000000000001030h]
140910a05: mov         rax,qword ptr [rax]
140910a08: mov         r8d,edi
140910a0b: mov         rdx,qword ptr [rsp+CD0]
140910a13: mov         rcx,qword ptr [rsp+0000000000001030h]
140910a1b: call        qword ptr [rax+0000000000000138h]
140910a21: mov         qword ptr [rsp+148],rax
140910a29: mov         edx,dword ptr [rsp+0E0]
140910a30: mov         rcx,qword ptr [rsp+148]
140910a38: call        14010F1E0
140910a3d: movzx       edx,byte ptr [rsp+140]
140910a45: mov         rcx,qword ptr [rsp+148]
140910a4d: call        140917170
140910a52: movzx       edx,byte ptr [rsp+0F8]
140910a5a: mov         rcx,qword ptr [rsp+148]
140910a62: call        140917190
140910a67: lea         rcx,[rsp+0D8]
140910a6f: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140910a75: movzx       eax,al
140910a78: test        eax,eax
140910a7a: jne         140910ABF
140910a7c: lea         rax,[rsp+640]
140910a84: mov         qword ptr [rsp+648],rax
140910a8c: lea         rdx,[rsp+0D8]
140910a94: mov         rcx,qword ptr [rsp+648]
140910a9c: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140910aa2: mov         qword ptr [rsp+CD8],rax
140910aaa: mov         rdx,qword ptr [rsp+CD8]
140910ab2: mov         rcx,qword ptr [rsp+148]
140910aba: call        140921980
140910abf: lea         rcx,[rsp+118]
140910ac7: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140910acd: movzx       eax,al
140910ad0: test        eax,eax
140910ad2: jne         140910B17
140910ad4: lea         rax,[rsp+650]
140910adc: mov         qword ptr [rsp+658],rax
140910ae4: lea         rdx,[rsp+118]
140910aec: mov         rcx,qword ptr [rsp+658]
140910af4: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140910afa: mov         qword ptr [rsp+CE0],rax
140910b02: mov         rdx,qword ptr [rsp+CE0]
140910b0a: mov         rcx,qword ptr [rsp+148]
140910b12: call        140921AE0
140910b17: lea         rcx,[rsp+0F0]
140910b1f: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140910b25: movzx       eax,al
140910b28: test        eax,eax
140910b2a: jne         140910B6F
140910b2c: lea         rax,[rsp+660]
140910b34: mov         qword ptr [rsp+668],rax
140910b3c: lea         rdx,[rsp+0F0]
140910b44: mov         rcx,qword ptr [rsp+668]
140910b4c: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140910b52: mov         qword ptr [rsp+CE8],rax
140910b5a: mov         rdx,qword ptr [rsp+CE8]
140910b62: mov         rcx,qword ptr [rsp+148]
140910b6a: call        140921C40
140910b6f: movzx       eax,byte ptr [rsp+140]
140910b77: test        eax,eax
140910b79: je          1409111C4
140910b7f: mov         qword ptr [rsp+1D8],0
140910b8b: mov         qword ptr [rsp+1C8],0
140910b97: mov         qword ptr [rsp+1E0],0
140910ba3: lea         rcx,[rsp+1B8]
140910bab: call        140A98820
140910bb0: nop
140910bb1: lea         rcx,[rsp+138]
140910bb9: call        140004AE0
140910bbe: mov         dword ptr [rsp+1C0],eax
140910bc5: cmp         dword ptr [rsp+1C0],0
140910bcd: jle         140910C58
140910bd3: xor         edx,edx
140910bd5: lea         rcx,[rsp+138]
140910bdd: call        1400250C0
140910be2: mov         r8,rax
140910be5: lea         rdx,[rsp+670]
140910bed: lea         rcx,[rsp+1B8]
140910bf5: call        14006BCC0
140910bfa: cmp         dword ptr [rsp+1C0],1
140910c02: jle         140910C30
140910c04: mov         edx,1
140910c09: lea         rcx,[rsp+138]
140910c11: call        1400250C0
140910c16: mov         r8,rax
140910c19: lea         rdx,[rsp+678]
140910c21: lea         rcx,[rsp+1B8]
140910c29: call        14006BCC0
140910c2e: jmp         140910C58
140910c30: mov         dword ptr [rsp+680],1
140910c3b: lea         r8,[rsp+680]
140910c43: lea         rdx,[rsp+688]
140910c4b: lea         rcx,[rsp+1B8]
140910c53: call        14006BCC0
140910c58: mov         edx,0Dh
140910c5d: lea         rcx,[rsp+690]
140910c65: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910c6b: mov         r8d,1
140910c71: movzx       edx,word ptr [rax]
140910c74: lea         rcx,[rsp+0D8]
140910c7c: call        qword ptr [141E04EE0h] ; QtCore4_64.dll!?remove@QString@@QEAAAEAV1@VQChar@@W4CaseSensitivity@Qt@@@Z
140910c82: mov         edx,0Ah
140910c87: lea         rcx,[rsp+6A0]
140910c8f: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910c95: movzx       edx,word ptr [rax]
140910c98: lea         rcx,[rsp+698]
140910ca0: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140910ca6: mov         qword ptr [rsp+CF0],rax
140910cae: mov         rax,qword ptr [rsp+CF0]
140910cb6: mov         qword ptr [rsp+CF8],rax
140910cbe: mov         edx,0Dh
140910cc3: lea         rcx,[rsp+6B0]
140910ccb: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910cd1: movzx       edx,word ptr [rax]
140910cd4: lea         rcx,[rsp+6A8]
140910cdc: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140910ce2: mov         qword ptr [rsp+D00],rax
140910cea: mov         rax,qword ptr [rsp+D00]
140910cf2: mov         qword ptr [rsp+D08],rax
140910cfa: mov         r8,qword ptr [rsp+CF8]
140910d02: mov         rdx,qword ptr [rsp+D08]
140910d0a: lea         rcx,[rsp+6B8]
140910d12: call        1400048F0
140910d17: mov         qword ptr [rsp+D10],rax
140910d1f: mov         rax,qword ptr [rsp+D10]
140910d27: mov         qword ptr [rsp+D18],rax
140910d2f: mov         edx,0Ah
140910d34: lea         rcx,[rsp+6C0]
140910d3c: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910d42: mov         r9d,1
140910d48: mov         r8,qword ptr [rsp+D18]
140910d50: movzx       edx,word ptr [rax]
140910d53: lea         rcx,[rsp+0D8]
140910d5b: call        qword ptr [141E04F40h] ; QtCore4_64.dll!?replace@QString@@QEAAAEAV1@VQChar@@AEBV1@W4CaseSensitivity@Qt@@@Z
140910d61: nop
140910d62: lea         rcx,[rsp+6B8]
140910d6a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910d70: nop
140910d71: lea         rcx,[rsp+6A8]
140910d79: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910d7f: nop
140910d80: lea         rcx,[rsp+698]
140910d88: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910d8e: mov         edx,0Dh
140910d93: lea         rcx,[rsp+6C2]
140910d9b: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910da1: mov         r8d,1
140910da7: movzx       edx,word ptr [rax]
140910daa: lea         rcx,[rsp+118]
140910db2: call        qword ptr [141E04EE0h] ; QtCore4_64.dll!?remove@QString@@QEAAAEAV1@VQChar@@W4CaseSensitivity@Qt@@@Z
140910db8: mov         edx,0Ah
140910dbd: lea         rcx,[rsp+6D0]
140910dc5: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910dcb: movzx       edx,word ptr [rax]
140910dce: lea         rcx,[rsp+6C8]
140910dd6: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140910ddc: mov         qword ptr [rsp+D20],rax
140910de4: mov         rax,qword ptr [rsp+D20]
140910dec: mov         qword ptr [rsp+D28],rax
140910df4: mov         edx,0Dh
140910df9: lea         rcx,[rsp+6E0]
140910e01: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910e07: movzx       edx,word ptr [rax]
140910e0a: lea         rcx,[rsp+6D8]
140910e12: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140910e18: mov         qword ptr [rsp+D30],rax
140910e20: mov         rax,qword ptr [rsp+D30]
140910e28: mov         qword ptr [rsp+D38],rax
140910e30: mov         r8,qword ptr [rsp+D28]
140910e38: mov         rdx,qword ptr [rsp+D38]
140910e40: lea         rcx,[rsp+6E8]
140910e48: call        1400048F0
140910e4d: mov         qword ptr [rsp+D40],rax
140910e55: mov         rax,qword ptr [rsp+D40]
140910e5d: mov         qword ptr [rsp+D48],rax
140910e65: mov         edx,0Ah
140910e6a: lea         rcx,[rsp+6F0]
140910e72: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910e78: mov         r9d,1
140910e7e: mov         r8,qword ptr [rsp+D48]
140910e86: movzx       edx,word ptr [rax]
140910e89: lea         rcx,[rsp+118]
140910e91: call        qword ptr [141E04F40h] ; QtCore4_64.dll!?replace@QString@@QEAAAEAV1@VQChar@@AEBV1@W4CaseSensitivity@Qt@@@Z
140910e97: nop
140910e98: lea         rcx,[rsp+6E8]
140910ea0: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910ea6: nop
140910ea7: lea         rcx,[rsp+6D8]
140910eaf: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910eb5: nop
140910eb6: lea         rcx,[rsp+6C8]
140910ebe: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910ec4: mov         edx,0Dh
140910ec9: lea         rcx,[rsp+6F2]
140910ed1: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910ed7: mov         r8d,1
140910edd: movzx       edx,word ptr [rax]
140910ee0: lea         rcx,[rsp+0F0]
140910ee8: call        qword ptr [141E04EE0h] ; QtCore4_64.dll!?remove@QString@@QEAAAEAV1@VQChar@@W4CaseSensitivity@Qt@@@Z
140910eee: mov         edx,0Ah
140910ef3: lea         rcx,[rsp+700]
140910efb: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910f01: movzx       edx,word ptr [rax]
140910f04: lea         rcx,[rsp+6F8]
140910f0c: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140910f12: mov         qword ptr [rsp+D50],rax
140910f1a: mov         rax,qword ptr [rsp+D50]
140910f22: mov         qword ptr [rsp+D58],rax
140910f2a: mov         edx,0Dh
140910f2f: lea         rcx,[rsp+710]
140910f37: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910f3d: movzx       edx,word ptr [rax]
140910f40: lea         rcx,[rsp+708]
140910f48: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140910f4e: mov         qword ptr [rsp+D60],rax
140910f56: mov         rax,qword ptr [rsp+D60]
140910f5e: mov         qword ptr [rsp+D68],rax
140910f66: mov         r8,qword ptr [rsp+D58]
140910f6e: mov         rdx,qword ptr [rsp+D68]
140910f76: lea         rcx,[rsp+718]
140910f7e: call        1400048F0
140910f83: mov         qword ptr [rsp+D70],rax
140910f8b: mov         rax,qword ptr [rsp+D70]
140910f93: mov         qword ptr [rsp+D78],rax
140910f9b: mov         edx,0Ah
140910fa0: lea         rcx,[rsp+720]
140910fa8: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140910fae: mov         r9d,1
140910fb4: mov         r8,qword ptr [rsp+D78]
140910fbc: movzx       edx,word ptr [rax]
140910fbf: lea         rcx,[rsp+0F0]
140910fc7: call        qword ptr [141E04F40h] ; QtCore4_64.dll!?replace@QString@@QEAAAEAV1@VQChar@@AEBV1@W4CaseSensitivity@Qt@@@Z
140910fcd: nop
140910fce: lea         rcx,[rsp+718]
140910fd6: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910fdc: nop
140910fdd: lea         rcx,[rsp+708]
140910fe5: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910feb: nop
140910fec: lea         rcx,[rsp+6F8]
140910ff4: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140910ffa: lea         rax,[rsp+728]
140911002: mov         qword ptr [rsp+730],rax
14091100a: lea         rdx,[rsp+1B8]
140911012: mov         rcx,qword ptr [rsp+730]
14091101a: call        140917800
14091101f: mov         qword ptr [rsp+D80],rax
140911027: mov         rax,qword ptr [rsp+D80]
14091102f: mov         qword ptr [rsp+D88],rax
140911037: lea         rdx,[rsp+738]
14091103f: lea         rcx,[rsp+108]
140911047: call        qword ptr [141E03F50h] ; QtCore4_64.dll!?toLocal8Bit@QString@@QEBA?AVQByteArray@@XZ
14091104d: mov         qword ptr [rsp+D90],rax
140911055: mov         rax,qword ptr [rsp+D90]
14091105d: mov         qword ptr [rsp+D98],rax
140911065: mov         rcx,qword ptr [rsp+D98]
14091106d: call        qword ptr [141E04228h] ; QtCore4_64.dll!?data@QByteArray@@QEAAPEADXZ
140911073: mov         rdi,rax
140911076: lea         rdx,[rsp+740]
14091107e: lea         rcx,[rsp+128]
140911086: call        qword ptr [141E03F50h] ; QtCore4_64.dll!?toLocal8Bit@QString@@QEBA?AVQByteArray@@XZ
14091108c: mov         qword ptr [rsp+DA0],rax
140911094: mov         rax,qword ptr [rsp+DA0]
14091109c: mov         qword ptr [rsp+DA8],rax
1409110a4: mov         rcx,qword ptr [rsp+DA8]
1409110ac: call        qword ptr [141E04228h] ; QtCore4_64.dll!?data@QByteArray@@QEAAPEADXZ
1409110b2: mov         rcx,rax
1409110b5: call        140921E90
1409110ba: mov         ecx,eax
1409110bc: lea         rax,[rsp+1E0]
1409110c4: mov         qword ptr [rsp+48h],rax
1409110c9: lea         rax,[rsp+1C8]
1409110d1: mov         qword ptr [rsp+40h],rax
1409110d6: lea         rax,[rsp+1D8]
1409110de: mov         qword ptr [rsp+38h],rax
1409110e3: lea         rax,[rsp+0F0]
1409110eb: mov         qword ptr [rsp+30h],rax
1409110f0: lea         rax,[rsp+118]
1409110f8: mov         qword ptr [rsp+28h],rax
1409110fd: lea         rax,[rsp+0D8]
140911105: mov         qword ptr [rsp+20h],rax
14091110a: mov         r9,qword ptr [rsp+D88]
140911112: mov         r8d,dword ptr [rsp+084]
14091111a: mov         rdx,rdi
14091111d: call        140916530
140911122: mov         dword ptr [rsp+1D0],eax
140911129: lea         rcx,[rsp+740]
140911131: call        qword ptr [141E04290h] ; QtCore4_64.dll!??1QByteArray@@QEAA@XZ
140911137: nop
140911138: lea         rcx,[rsp+738]
140911140: call        qword ptr [141E04290h] ; QtCore4_64.dll!??1QByteArray@@QEAA@XZ
140911146: cmp         qword ptr [rsp+1D8],0
14091114f: je          1409111B2
140911151: cmp         qword ptr [rsp+1C8],0
14091115a: je          1409111B2
14091115c: cmp         qword ptr [rsp+1E0],0
140911165: je          1409111B2
140911167: xor         r9d,r9d
14091116a: mov         r8d,dword ptr [rsp+0C8]
140911172: mov         rdx,qword ptr [rsp+1D8]
14091117a: mov         rcx,qword ptr [rsp+148]
140911182: call        1409210D0
140911187: mov         rdx,qword ptr [rsp+1E0]
14091118f: mov         rcx,qword ptr [rsp+148]
140911197: call        1409213F0
14091119c: mov         rdx,qword ptr [rsp+1C8]
1409111a4: mov         rcx,qword ptr [rsp+148]
1409111ac: call        1409212F0
1409111b1: nop
1409111b2: lea         rcx,[rsp+1B8]
1409111ba: call        1408EC6F0
1409111bf: jmp         14091142A
1409111c4: movzx       eax,byte ptr [rsp+0F8]
1409111cc: test        eax,eax
1409111ce: je          140911219
1409111d0: cvttsd2si   edx,mmword ptr [rsp+0D0]
1409111d9: mov         rcx,qword ptr [rsp+148]
1409111e1: call        140921590
1409111e6: lea         r8,[rsp+0E8]
1409111ee: lea         rdx,[rsp+110]
1409111f6: mov         rcx,qword ptr [rsp+148]
1409111fe: call        1409214F0
140911203: cvttsd2si   edx,mmword ptr [rsp+0D0]
14091120c: mov         rcx,qword ptr [rsp+148]
140911214: call        140921590
140911219: lea         rcx,[rsp+118]
140911221: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140911227: movzx       eax,al
14091122a: test        eax,eax
14091122c: jne         14091134F
140911232: lea         rcx,[rsp+0F0]
14091123a: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140911240: movzx       eax,al
140911243: test        eax,eax
140911245: jne         14091134F
14091124b: mov         byte ptr [rsp+1F0],0
140911253: movzx       eax,byte ptr [rsp+0F8]
14091125b: test        eax,eax
14091125d: je          140911267
14091125f: mov         byte ptr [rsp+1F0],1
140911267: movzx       r9d,byte ptr [rsp+1F0]
140911270: lea         r8,[rsp+118]
140911278: lea         rdx,[rsp+0F0]
140911280: lea         rcx,[rsp+1E8]
140911288: call        140922330
14091128d: nop
14091128e: mov         edx,dword ptr [rsp+0E0]
140911295: lea         rcx,[rsp+0F0]
14091129d: call        140922AE0
1409112a2: movsd       mmword ptr [rsp+1F8],xmm0
1409112ab: movzx       eax,byte ptr [rsp+0F8]
1409112b3: test        eax,eax
1409112b5: je          1409112CF
1409112b7: cvttsd2si   edx,mmword ptr [rsp+1F8]
1409112c0: mov         rcx,qword ptr [rsp+148]
1409112c8: call        1409217C0
1409112cd: jmp         1409112E5
1409112cf: movsd       xmm1,mmword ptr [rsp+1F8]
1409112d8: mov         rcx,qword ptr [rsp+148]
1409112e0: call        140921030
1409112e5: mov         edx,dword ptr [rsp+0E0]
1409112ec: lea         rcx,[rsp+118]
1409112f4: call        140922AE0
1409112f9: movsd       mmword ptr [rsp+1F8],xmm0
140911302: movzx       eax,byte ptr [rsp+0F8]
14091130a: test        eax,eax
14091130c: je          140911326
14091130e: cvttsd2si   edx,mmword ptr [rsp+1F8]
140911317: mov         rcx,qword ptr [rsp+148]
14091131f: call        140921650
140911324: jmp         14091133D
140911326: movsd       xmm1,mmword ptr [rsp+1F8]
14091132f: mov         rcx,qword ptr [rsp+148]
140911337: call        140920F90
14091133c: nop
14091133d: lea         rcx,[rsp+1E8]
140911345: call        1408EC6F0
14091134a: jmp         14091142A
14091134f: lea         rcx,[rsp+118]
140911357: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
14091135d: movzx       eax,al
140911360: test        eax,eax
140911362: jne         1409113BD
140911364: mov         edx,dword ptr [rsp+0E0]
14091136b: lea         rcx,[rsp+118]
140911373: call        140922AE0
140911378: movsd       mmword ptr [rsp+200],xmm0
140911381: movzx       eax,byte ptr [rsp+0F8]
140911389: test        eax,eax
14091138b: je          1409113A5
14091138d: cvttsd2si   edx,mmword ptr [rsp+200]
140911396: mov         rcx,qword ptr [rsp+148]
14091139e: call        140921650
1409113a3: jmp         1409113BB
1409113a5: movsd       xmm1,mmword ptr [rsp+200]
1409113ae: mov         rcx,qword ptr [rsp+148]
1409113b6: call        140920F90
1409113bb: jmp         14091142A
1409113bd: lea         rcx,[rsp+0F0]
1409113c5: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
1409113cb: movzx       eax,al
1409113ce: test        eax,eax
1409113d0: jne         14091142A
1409113d2: mov         edx,dword ptr [rsp+0E0]
1409113d9: lea         rcx,[rsp+0F0]
1409113e1: call        140922AE0
1409113e6: movsd       mmword ptr [rsp+208],xmm0
1409113ef: movzx       eax,byte ptr [rsp+0F8]
1409113f7: test        eax,eax
1409113f9: je          140911413
1409113fb: cvttsd2si   edx,mmword ptr [rsp+208]
140911404: mov         rcx,qword ptr [rsp+148]
14091140c: call        1409217C0
140911411: jmp         14091142A
140911413: movsd       xmm1,mmword ptr [rsp+208]
14091141c: mov         rcx,qword ptr [rsp+148]
140911424: call        140921030
140911429: nop
14091142a: lea         rcx,[rsp+130]
140911432: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140911438: nop
140911439: lea         rcx,[rsp+0F0]
140911441: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911447: nop
140911448: lea         rcx,[rsp+118]
140911450: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911456: nop
140911457: lea         rcx,[rsp+0D8]
14091145f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911465: nop
140911466: lea         rcx,[rsp+138]
14091146e: call        1402081A0
140911473: nop
140911474: lea         rcx,[rsp+120]
14091147c: call        1402081A0
140911481: nop
140911482: lea         rcx,[rsp+108]
14091148a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911490: nop
140911491: lea         rcx,[rsp+100]
140911499: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091149f: nop
1409114a0: lea         rcx,[rsp+128]
1409114a8: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409114ae: nop
1409114af: lea         rcx,[rsp+0E8]
1409114b7: call        1402081A0
1409114bc: nop
1409114bd: lea         rcx,[rsp+110]
1409114c5: call        1400048D0
1409114ca: lea         rdx,[rsp+750]
1409114d2: lea         rcx,[rsp+0B8]
1409114da: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
1409114e0: mov         qword ptr [rsp+DB0],rax
1409114e8: mov         rax,qword ptr [rsp+DB0]
1409114f0: mov         qword ptr [rsp+DB8],rax
1409114f8: lea         rdx,[00000001427BB6A8h] ; 'Geometry'
1409114ff: mov         rcx,qword ptr [rsp+DB8]
140911507: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14091150d: mov         byte ptr [rsp+748],al
140911514: lea         rcx,[rsp+750]
14091151c: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911522: movzx       edx,byte ptr [rsp+748]
14091152a: test        edx,edx
14091152c: je          1409127B2
140911532: mov         rdx,qword ptr [rsp+0000000000001020h]
14091153a: lea         rcx,[rsp+258]
140911542: call        qword ptr [141E03F60h] ; QtCore4_64.dll!??0QFileInfo@@QEAA@AEBVQString@@@Z
140911548: nop
140911549: lea         rdx,[rsp+238]
140911551: lea         rcx,[rsp+258]
140911559: call        qword ptr [141E04580h] ; QtCore4_64.dll!?path@QFileInfo@@QEBA?AVQString@@XZ
14091155f: nop
140911560: lea         rdx,[00000001427BB6B8h] ; 'GeoOperation'
140911567: lea         rcx,[rsp+760]
14091156f: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911575: nop
140911576: lea         rcx,[rsp+758]
14091157e: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140911584: mov         qword ptr [rsp+DC0],rax
14091158c: mov         rax,qword ptr [rsp+DC0]
140911594: mov         qword ptr [rsp+DC8],rax
14091159c: mov         r9,qword ptr [rsp+DC8]
1409115a4: lea         r8,[rsp+760]
1409115ac: lea         rdx,[rsp+768]
1409115b4: lea         rcx,[rsp+0B8]
1409115bc: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
1409115c2: mov         qword ptr [rsp+DD0],rax
1409115ca: mov         rax,qword ptr [rsp+DD0]
1409115d2: mov         qword ptr [rsp+DD8],rax
1409115da: mov         r8d,0Ah
1409115e0: xor         edx,edx
1409115e2: mov         rcx,qword ptr [rsp+DD8]
1409115ea: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
1409115f0: mov         dword ptr [rsp+210],eax
1409115f7: lea         rcx,[rsp+768]
1409115ff: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911605: nop
140911606: lea         rcx,[rsp+758]
14091160e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911614: nop
140911615: lea         rcx,[rsp+760]
14091161d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911623: lea         rdx,[00000001427BB6C8h] ; 'GeoObject'
14091162a: lea         rcx,[rsp+778]
140911632: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911638: nop
140911639: lea         rcx,[rsp+770]
140911641: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140911647: mov         qword ptr [rsp+DE0],rax
14091164f: mov         rax,qword ptr [rsp+DE0]
140911657: mov         qword ptr [rsp+DE8],rax
14091165f: mov         r9,qword ptr [rsp+DE8]
140911667: lea         r8,[rsp+778]
14091166f: lea         rdx,[rsp+780]
140911677: lea         rcx,[rsp+0B8]
14091167f: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911685: mov         qword ptr [rsp+DF0],rax
14091168d: mov         rax,qword ptr [rsp+DF0]
140911695: mov         qword ptr [rsp+DF8],rax
14091169d: mov         r8d,0Ah
1409116a3: xor         edx,edx
1409116a5: mov         rcx,qword ptr [rsp+DF8]
1409116ad: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
1409116b3: mov         dword ptr [rsp+21C],eax
1409116ba: lea         rcx,[rsp+780]
1409116c2: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409116c8: nop
1409116c9: lea         rcx,[rsp+770]
1409116d1: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409116d7: nop
1409116d8: lea         rcx,[rsp+778]
1409116e0: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409116e6: mov         dword ptr [rsp+228],0
1409116f1: lea         rdx,[00000001427BB6D8h] ; 'MorphMethod'
1409116f8: lea         rcx,[rsp+790]
140911700: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911706: nop
140911707: lea         rcx,[rsp+788]
14091170f: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140911715: mov         qword ptr [rsp+E00],rax
14091171d: mov         rax,qword ptr [rsp+E00]
140911725: mov         qword ptr [rsp+E08],rax
14091172d: mov         r9,qword ptr [rsp+E08]
140911735: lea         r8,[rsp+790]
14091173d: lea         rdx,[rsp+240]
140911745: lea         rcx,[rsp+0B8]
14091174d: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911753: nop
140911754: lea         rcx,[rsp+788]
14091175c: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911762: nop
140911763: lea         rcx,[rsp+790]
14091176b: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911771: lea         rcx,[rsp+240]
140911779: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
14091177f: movzx       eax,al
140911782: test        eax,eax
140911784: jne         1409117A3
140911786: mov         r8d,0Ah
14091178c: xor         edx,edx
14091178e: lea         rcx,[rsp+240]
140911796: call        qword ptr [141E04090h] ; QtCore4_64.dll!?toInt@QString@@QEBAHPEA_NH@Z
14091179c: mov         dword ptr [rsp+228],eax
1409117a3: lea         rdx,[00000001427BB6E8h] ; 'UpperGeoFile'
1409117aa: lea         rcx,[rsp+7A0]
1409117b2: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
1409117b8: nop
1409117b9: lea         rcx,[rsp+798]
1409117c1: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
1409117c7: mov         qword ptr [rsp+E10],rax
1409117cf: mov         rax,qword ptr [rsp+E10]
1409117d7: mov         qword ptr [rsp+E18],rax
1409117df: mov         r9,qword ptr [rsp+E18]
1409117e7: lea         r8,[rsp+7A0]
1409117ef: lea         rdx,[rsp+7A8]
1409117f7: lea         rcx,[rsp+0B8]
1409117ff: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911805: mov         qword ptr [rsp+E20],rax
14091180d: mov         rax,qword ptr [rsp+E20]
140911815: mov         qword ptr [rsp+E28],rax
14091181d: lea         rdx,[00000001427BB314h] ; '/'
140911824: lea         rcx,[rsp+7B0]
14091182c: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911832: mov         qword ptr [rsp+E30],rax
14091183a: mov         rax,qword ptr [rsp+E30]
140911842: mov         qword ptr [rsp+E38],rax
14091184a: mov         r8,qword ptr [rsp+E38]
140911852: lea         rdx,[rsp+238]
14091185a: lea         rcx,[rsp+7B8]
140911862: call        1400048F0
140911867: mov         qword ptr [rsp+E40],rax
14091186f: mov         rax,qword ptr [rsp+E40]
140911877: mov         qword ptr [rsp+E48],rax
14091187f: mov         r8,qword ptr [rsp+E28]
140911887: mov         rdx,qword ptr [rsp+E48]
14091188f: lea         rcx,[rsp+250]
140911897: call        1400048F0
14091189c: nop
14091189d: lea         rcx,[rsp+7B8]
1409118a5: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409118ab: nop
1409118ac: lea         rcx,[rsp+7B0]
1409118b4: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409118ba: nop
1409118bb: lea         rcx,[rsp+7A8]
1409118c3: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409118c9: nop
1409118ca: lea         rcx,[rsp+798]
1409118d2: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409118d8: nop
1409118d9: lea         rcx,[rsp+7A0]
1409118e1: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409118e7: lea         rdx,[00000001427BB6F8h] ; 'LowerGeoFile'
1409118ee: lea         rcx,[rsp+7C8]
1409118f6: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
1409118fc: nop
1409118fd: lea         rcx,[rsp+7C0]
140911905: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
14091190b: mov         qword ptr [rsp+E50],rax
140911913: mov         rax,qword ptr [rsp+E50]
14091191b: mov         qword ptr [rsp+E58],rax
140911923: mov         r9,qword ptr [rsp+E58]
14091192b: lea         r8,[rsp+7C8]
140911933: lea         rdx,[rsp+7D0]
14091193b: lea         rcx,[rsp+0B8]
140911943: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911949: mov         qword ptr [rsp+E60],rax
140911951: mov         rax,qword ptr [rsp+E60]
140911959: mov         qword ptr [rsp+E68],rax
140911961: lea         rdx,[00000001427BB34Ch] ; '/'
140911968: lea         rcx,[rsp+7D8]
140911970: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911976: mov         qword ptr [rsp+E70],rax
14091197e: mov         rax,qword ptr [rsp+E70]
140911986: mov         qword ptr [rsp+E78],rax
14091198e: mov         r8,qword ptr [rsp+E78]
140911996: lea         rdx,[rsp+238]
14091199e: lea         rcx,[rsp+7E0]
1409119a6: call        1400048F0
1409119ab: mov         qword ptr [rsp+E80],rax
1409119b3: mov         rax,qword ptr [rsp+E80]
1409119bb: mov         qword ptr [rsp+E88],rax
1409119c3: mov         r8,qword ptr [rsp+E68]
1409119cb: mov         rdx,qword ptr [rsp+E88]
1409119d3: lea         rcx,[rsp+248]
1409119db: call        1400048F0
1409119e0: nop
1409119e1: lea         rcx,[rsp+7E0]
1409119e9: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409119ef: nop
1409119f0: lea         rcx,[rsp+7D8]
1409119f8: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409119fe: nop
1409119ff: lea         rcx,[rsp+7D0]
140911a07: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911a0d: nop
140911a0e: lea         rcx,[rsp+7C0]
140911a16: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911a1c: nop
140911a1d: lea         rcx,[rsp+7C8]
140911a25: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911a2b: lea         rdx,[00000001427BB708h] ; 'NominalGeoFile'
140911a32: lea         rcx,[rsp+7F0]
140911a3a: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911a40: nop
140911a41: lea         rcx,[rsp+7E8]
140911a49: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140911a4f: mov         qword ptr [rsp+E90],rax
140911a57: mov         rax,qword ptr [rsp+E90]
140911a5f: mov         qword ptr [rsp+E98],rax
140911a67: mov         r9,qword ptr [rsp+E98]
140911a6f: lea         r8,[rsp+7F0]
140911a77: lea         rdx,[rsp+7F8]
140911a7f: lea         rcx,[rsp+0B8]
140911a87: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911a8d: mov         qword ptr [rsp+EA0],rax
140911a95: mov         rax,qword ptr [rsp+EA0]
140911a9d: mov         qword ptr [rsp+EA8],rax
140911aa5: lea         rdx,[00000001427BB3B4h] ; '/'
140911aac: lea         rcx,[rsp+800]
140911ab4: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911aba: mov         qword ptr [rsp+EB0],rax
140911ac2: mov         rax,qword ptr [rsp+EB0]
140911aca: mov         qword ptr [rsp+EB8],rax
140911ad2: mov         r8,qword ptr [rsp+EB8]
140911ada: lea         rdx,[rsp+238]
140911ae2: lea         rcx,[rsp+808]
140911aea: call        1400048F0
140911aef: mov         qword ptr [rsp+EC0],rax
140911af7: mov         rax,qword ptr [rsp+EC0]
140911aff: mov         qword ptr [rsp+EC8],rax
140911b07: mov         r8,qword ptr [rsp+EA8]
140911b0f: mov         rdx,qword ptr [rsp+EC8]
140911b17: lea         rcx,[rsp+230]
140911b1f: call        1400048F0
140911b24: nop
140911b25: lea         rcx,[rsp+808]
140911b2d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911b33: nop
140911b34: lea         rcx,[rsp+800]
140911b3c: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911b42: nop
140911b43: lea         rcx,[rsp+7F8]
140911b4b: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911b51: nop
140911b52: lea         rcx,[rsp+7E8]
140911b5a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911b60: nop
140911b61: lea         rcx,[rsp+7F0]
140911b69: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911b6f: mov         byte ptr [rsp+22C],0
140911b77: lea         rdx,[00000001427BB718h] ; 'UpperDefined'
140911b7e: lea         rcx,[rsp+818]
140911b86: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911b8c: nop
140911b8d: lea         rdx,[rsp+818]
140911b95: lea         rcx,[rsp+0B8]
140911b9d: call        qword ptr [141E0C438h] ; QtXml4_64.dll!?hasAttribute@QDomElement@@QEBA_NAEBVQString@@@Z
140911ba3: mov         byte ptr [rsp+810],al
140911baa: lea         rcx,[rsp+818]
140911bb2: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911bb8: movzx       edx,byte ptr [rsp+810]
140911bc0: test        edx,edx
140911bc2: je          140911CEA
140911bc8: lea         rdx,[00000001427BB728h] ; 'UpperDefined'
140911bcf: lea         rcx,[rsp+828]
140911bd7: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911bdd: nop
140911bde: lea         rdx,[00000001427BB738h] ; 'false'
140911be5: lea         rcx,[rsp+838]
140911bed: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911bf3: mov         qword ptr [rsp+ED0],rax
140911bfb: mov         rax,qword ptr [rsp+ED0]
140911c03: mov         qword ptr [rsp+ED8],rax
140911c0b: lea         rcx,[rsp+820]
140911c13: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140911c19: mov         qword ptr [rsp+EE0],rax
140911c21: mov         rax,qword ptr [rsp+EE0]
140911c29: mov         qword ptr [rsp+EE8],rax
140911c31: mov         r9,qword ptr [rsp+EE8]
140911c39: lea         r8,[rsp+828]
140911c41: lea         rdx,[rsp+830]
140911c49: lea         rcx,[rsp+0B8]
140911c51: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911c57: mov         qword ptr [rsp+EF0],rax
140911c5f: mov         rax,qword ptr [rsp+EF0]
140911c67: mov         qword ptr [rsp+EF8],rax
140911c6f: mov         rdx,qword ptr [rsp+ED8]
140911c77: mov         rcx,qword ptr [rsp+EF8]
140911c7f: call        qword ptr [141E03F08h] ; QtCore4_64.dll!??8QString@@QEBA_NAEBV0@@Z
140911c85: movzx       eax,al
140911c88: test        eax,eax
140911c8a: je          140911C96
140911c8c: mov         byte ptr [rsp+F00],0
140911c94: jmp         140911C9E
140911c96: mov         byte ptr [rsp+F00],1
140911c9e: movzx       eax,byte ptr [rsp+F00]
140911ca6: mov         byte ptr [rsp+22C],al
140911cad: lea         rcx,[rsp+830]
140911cb5: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911cbb: nop
140911cbc: lea         rcx,[rsp+820]
140911cc4: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911cca: nop
140911ccb: lea         rcx,[rsp+838]
140911cd3: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911cd9: nop
140911cda: lea         rcx,[rsp+828]
140911ce2: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911ce8: jmp         140911D07
140911cea: lea         rcx,[rsp+250]
140911cf2: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140911cf8: movzx       eax,al
140911cfb: test        eax,eax
140911cfd: jne         140911D07
140911cff: mov         byte ptr [rsp+22C],1
140911d07: mov         byte ptr [rsp+218],1
140911d0f: lea         rdx,[00000001427BB740h] ; 'LowerDefined'
140911d16: lea         rcx,[rsp+848]
140911d1e: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911d24: nop
140911d25: lea         rdx,[rsp+848]
140911d2d: lea         rcx,[rsp+0B8]
140911d35: call        qword ptr [141E0C438h] ; QtXml4_64.dll!?hasAttribute@QDomElement@@QEBA_NAEBVQString@@@Z
140911d3b: mov         byte ptr [rsp+840],al
140911d42: lea         rcx,[rsp+848]
140911d4a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911d50: movzx       edx,byte ptr [rsp+840]
140911d58: test        edx,edx
140911d5a: je          140911E82
140911d60: lea         rdx,[00000001427BB750h] ; 'LowerDefined'
140911d67: lea         rcx,[rsp+858]
140911d6f: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911d75: nop
140911d76: lea         rdx,[00000001427BB760h] ; 'false'
140911d7d: lea         rcx,[rsp+868]
140911d85: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
140911d8b: mov         qword ptr [rsp+F08],rax
140911d93: mov         rax,qword ptr [rsp+F08]
140911d9b: mov         qword ptr [rsp+F10],rax
140911da3: lea         rcx,[rsp+850]
140911dab: call        qword ptr [141E044C0h] ; QtCore4_64.dll!??0QString@@QEAA@XZ
140911db1: mov         qword ptr [rsp+F18],rax
140911db9: mov         rax,qword ptr [rsp+F18]
140911dc1: mov         qword ptr [rsp+F20],rax
140911dc9: mov         r9,qword ptr [rsp+F20]
140911dd1: lea         r8,[rsp+858]
140911dd9: lea         rdx,[rsp+860]
140911de1: lea         rcx,[rsp+0B8]
140911de9: call        qword ptr [141E0C668h] ; QtXml4_64.dll!?attribute@QDomElement@@QEBA?AVQString@@AEBV2@0@Z
140911def: mov         qword ptr [rsp+F28],rax
140911df7: mov         rax,qword ptr [rsp+F28]
140911dff: mov         qword ptr [rsp+F30],rax
140911e07: mov         rdx,qword ptr [rsp+F10]
140911e0f: mov         rcx,qword ptr [rsp+F30]
140911e17: call        qword ptr [141E03F08h] ; QtCore4_64.dll!??8QString@@QEBA_NAEBV0@@Z
140911e1d: movzx       eax,al
140911e20: test        eax,eax
140911e22: je          140911E2E
140911e24: mov         byte ptr [rsp+F38],0
140911e2c: jmp         140911E36
140911e2e: mov         byte ptr [rsp+F38],1
140911e36: movzx       eax,byte ptr [rsp+F38]
140911e3e: mov         byte ptr [rsp+218],al
140911e45: lea         rcx,[rsp+860]
140911e4d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911e53: nop
140911e54: lea         rcx,[rsp+850]
140911e5c: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911e62: nop
140911e63: lea         rcx,[rsp+868]
140911e6b: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911e71: nop
140911e72: lea         rcx,[rsp+858]
140911e7a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140911e80: jmp         140911E9F
140911e82: lea         rcx,[rsp+248]
140911e8a: call        qword ptr [141E03FC8h] ; QtCore4_64.dll!?isEmpty@QString@@QEBA_NXZ
140911e90: movzx       eax,al
140911e93: test        eax,eax
140911e95: jne         140911E9F
140911e97: mov         byte ptr [rsp+22C],1
140911e9f: lea         rdx,[rsp+220]
140911ea7: lea         rcx,[rsp+0B8]
140911eaf: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
140911eb5: nop
140911eb6: lea         rax,[rsp+870]
140911ebe: mov         qword ptr [rsp+878],rax
140911ec6: lea         rax,[rsp+880]
140911ece: mov         qword ptr [rsp+888],rax
140911ed6: lea         rax,[rsp+890]
140911ede: mov         qword ptr [rsp+898],rax
140911ee6: lea         rdx,[rsp+248]
140911eee: mov         rcx,qword ptr [rsp+878]
140911ef6: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140911efc: mov         qword ptr [rsp+F40],rax
140911f04: mov         rax,qword ptr [rsp+F40]
140911f0c: mov         qword ptr [rsp+F48],rax
140911f14: lea         rdx,[rsp+250]
140911f1c: mov         rcx,qword ptr [rsp+888]
140911f24: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140911f2a: mov         qword ptr [rsp+F50],rax
140911f32: mov         rax,qword ptr [rsp+F50]
140911f3a: mov         qword ptr [rsp+F58],rax
140911f42: lea         rdx,[rsp+230]
140911f4a: mov         rcx,qword ptr [rsp+898]
140911f52: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140911f58: mov         qword ptr [rsp+F60],rax
140911f60: mov         rax,qword ptr [rsp+F48]
140911f68: mov         qword ptr [rsp+28h],rax
140911f6d: mov         rax,qword ptr [rsp+F58]
140911f75: mov         qword ptr [rsp+20h],rax
140911f7a: mov         r9,qword ptr [rsp+F60]
140911f82: mov         r8d,dword ptr [rsp+21C]
140911f8a: mov         edx,dword ptr [rsp+210]
140911f91: mov         rcx,qword ptr [rsp+0A8]
140911f99: call        1408175C0
140911f9e: mov         rcx,qword ptr [rsp+0A8]
140911fa6: call        140817840
140911fab: sub         eax,1
140911fae: mov         dword ptr [rsp+214],eax
140911fb5: movzx       r8d,byte ptr [rsp+22C]
140911fbe: mov         edx,dword ptr [rsp+214]
140911fc5: mov         rcx,qword ptr [rsp+0A8]
140911fcd: call        140817F40
140911fd2: movzx       r8d,byte ptr [rsp+218]
140911fdb: mov         edx,dword ptr [rsp+214]
140911fe2: mov         rcx,qword ptr [rsp+0A8]
140911fea: call        140817FA0
140911fef: mov         edx,dword ptr [rsp+228]
140911ff6: mov         rcx,qword ptr [rsp+0A8]
140911ffe: call        140817890
140912003: lea         rcx,[rsp+220]
14091200b: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140912011: movzx       eax,al
140912014: test        eax,eax
140912016: jne         14091274A
14091201c: lea         rdx,[rsp+260]
140912024: lea         rcx,[rsp+220]
14091202c: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
140912032: nop
140912033: lea         rcx,[rsp+260]
14091203b: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140912041: movzx       eax,al
140912044: test        eax,eax
140912046: jne         1409126E3
14091204c: lea         rdx,[rsp+8A8]
140912054: lea         rcx,[rsp+260]
14091205c: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
140912062: mov         qword ptr [rsp+F68],rax
14091206a: mov         rax,qword ptr [rsp+F68]
140912072: mov         qword ptr [rsp+F70],rax
14091207a: lea         rdx,[00000001427BB768h] ; 'UpperCorrespondence'
140912081: mov         rcx,qword ptr [rsp+F70]
140912089: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
14091208f: mov         byte ptr [rsp+8A0],al
140912096: lea         rcx,[rsp+8A8]
14091209e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409120a4: movzx       edx,byte ptr [rsp+8A0]
1409120ac: test        edx,edx
1409120ae: je          14091239A
1409120b4: lea         rdx,[rsp+268]
1409120bc: lea         rcx,[rsp+260]
1409120c4: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
1409120ca: nop
1409120cb: lea         rcx,[rsp+268]
1409120d3: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
1409120d9: movzx       eax,al
1409120dc: test        eax,eax
1409120de: jne         140912387
1409120e4: lea         rdx,[rsp+270]
1409120ec: lea         rcx,[rsp+268]
1409120f4: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
1409120fa: nop
1409120fb: lea         rcx,[rsp+270]
140912103: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140912109: movzx       eax,al
14091210c: test        eax,eax
14091210e: jne         140912320
140912114: lea         rdx,[rsp+8B8]
14091211c: lea         rcx,[rsp+270]
140912124: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
14091212a: mov         qword ptr [rsp+F78],rax
140912132: mov         rax,qword ptr [rsp+F78]
14091213a: mov         qword ptr [rsp+F80],rax
140912142: lea         rdx,[00000001427BB780h] ; 'PointPair'
140912149: mov         rcx,qword ptr [rsp+F80]
140912151: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
140912157: mov         byte ptr [rsp+8B0],al
14091215e: lea         rcx,[rsp+8B8]
140912166: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091216c: movzx       edx,byte ptr [rsp+8B0]
140912174: test        edx,edx
140912176: je          140912320
14091217c: lea         rdx,[rsp+2C0]
140912184: lea         rcx,[rsp+270]
14091218c: call        qword ptr [141E0C5A0h] ; QtXml4_64.dll!?text@QDomElement@@QEBA?AVQString@@XZ
140912192: nop
140912193: lea         rdx,[00000001427BB47Ch] ; ', '
14091219a: lea         rcx,[rsp+8C0]
1409121a2: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
1409121a8: nop
1409121a9: mov         dword ptr [rsp+20h],1
1409121b1: xor         r9d,r9d
1409121b4: lea         r8,[rsp+8C0]
1409121bc: lea         rdx,[rsp+2A0]
1409121c4: lea         rcx,[rsp+2C0]
1409121cc: call        qword ptr [141E04288h] ; QtCore4_64.dll!?split@QString@@QEBA?AVQStringList@@AEBV1@W4SplitBehavior@1@W4CaseSensitivity@Qt@@@Z
1409121d2: nop
1409121d3: lea         rcx,[rsp+8C0]
1409121db: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409121e1: xor         edx,edx
1409121e3: lea         rcx,[rsp+2A0]
1409121eb: call        140004B40
1409121f0: xor         edx,edx
1409121f2: mov         rcx,rax
1409121f5: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
1409121fb: movsd       mmword ptr [rsp+2A8],xmm0
140912204: mov         edx,1
140912209: lea         rcx,[rsp+2A0]
140912211: call        140004B40
140912216: xor         edx,edx
140912218: mov         rcx,rax
14091221b: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
140912221: movsd       mmword ptr [rsp+298],xmm0
14091222a: mov         edx,2
14091222f: lea         rcx,[rsp+2A0]
140912237: call        140004B40
14091223c: xor         edx,edx
14091223e: mov         rcx,rax
140912241: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
140912247: movsd       mmword ptr [rsp+280],xmm0
140912250: mov         edx,3
140912255: lea         rcx,[rsp+2A0]
14091225d: call        140004B40
140912262: xor         edx,edx
140912264: mov         rcx,rax
140912267: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
14091226d: movsd       mmword ptr [rsp+278],xmm0
140912276: movsd       xmm2,mmword ptr [rsp+298]
14091227f: movsd       xmm1,mmword ptr [rsp+2A8]
140912288: lea         rcx,[rsp+2B0]
140912290: call        qword ptr [141E057D8h] ; QtCore4_64.dll!??0QPointF@@QEAA@NN@Z
140912296: movsd       xmm2,mmword ptr [rsp+278]
14091229f: movsd       xmm1,mmword ptr [rsp+280]
1409122a8: lea         rcx,[rsp+288]
1409122b0: call        qword ptr [141E057D8h] ; QtCore4_64.dll!??0QPointF@@QEAA@NN@Z
1409122b6: lea         rdi,[rsp+940]
1409122be: lea         rsi,[rsp+288]
1409122c6: mov         ecx,10h
1409122cb: rep movs    byte ptr [rdi],byte ptr [rsi]
1409122cd: lea         rdi,[rsp+930]
1409122d5: lea         rsi,[rsp+2B0]
1409122dd: mov         ecx,10h
1409122e2: rep movs    byte ptr [rdi],byte ptr [rsi]
1409122e4: lea         r9,[rsp+940]
1409122ec: lea         r8,[rsp+930]
1409122f4: xor         edx,edx
1409122f6: mov         rcx,qword ptr [rsp+0A8]
1409122fe: call        140818000
140912303: nop
140912304: lea         rcx,[rsp+2A0]
14091230c: call        1400048D0
140912311: nop
140912312: lea         rcx,[rsp+2C0]
14091231a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912320: lea         rdx,[rsp+8C8]
140912328: lea         rcx,[rsp+268]
140912330: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
140912336: mov         qword ptr [rsp+F88],rax
14091233e: mov         rax,qword ptr [rsp+F88]
140912346: mov         qword ptr [rsp+F90],rax
14091234e: mov         rdx,qword ptr [rsp+F90]
140912356: lea         rcx,[rsp+268]
14091235e: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
140912364: nop
140912365: lea         rcx,[rsp+8C8]
14091236d: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912373: nop
140912374: lea         rcx,[rsp+270]
14091237c: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
140912382: jmp         1409120CB
140912387: lea         rcx,[rsp+268]
14091238f: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912395: jmp         1409126E3
14091239a: lea         rdx,[rsp+8D8]
1409123a2: lea         rcx,[rsp+260]
1409123aa: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
1409123b0: mov         qword ptr [rsp+F98],rax
1409123b8: mov         rax,qword ptr [rsp+F98]
1409123c0: mov         qword ptr [rsp+FA0],rax
1409123c8: lea         rdx,[00000001427BB790h] ; 'LowerCorrespondence'
1409123cf: mov         rcx,qword ptr [rsp+FA0]
1409123d7: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
1409123dd: mov         byte ptr [rsp+8D0],al
1409123e4: lea         rcx,[rsp+8D8]
1409123ec: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409123f2: movzx       edx,byte ptr [rsp+8D0]
1409123fa: test        edx,edx
1409123fc: je          1409126E3
140912402: lea         rdx,[rsp+2C8]
14091240a: lea         rcx,[rsp+260]
140912412: call        qword ptr [141E0C5B8h] ; QtXml4_64.dll!?firstChild@QDomNode@@QEBA?AV1@XZ
140912418: nop
140912419: lea         rcx,[rsp+2C8]
140912421: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140912427: movzx       eax,al
14091242a: test        eax,eax
14091242c: jne         1409126D5
140912432: lea         rdx,[rsp+2D0]
14091243a: lea         rcx,[rsp+2C8]
140912442: call        qword ptr [141E0C5B0h] ; QtXml4_64.dll!?toElement@QDomNode@@QEBA?AVQDomElement@@XZ
140912448: nop
140912449: lea         rcx,[rsp+2D0]
140912451: call        qword ptr [141E0C538h] ; QtXml4_64.dll!?isNull@QDomNode@@QEBA_NXZ
140912457: movzx       eax,al
14091245a: test        eax,eax
14091245c: jne         14091266E
140912462: lea         rdx,[rsp+8E8]
14091246a: lea         rcx,[rsp+2D0]
140912472: call        qword ptr [141E0C5A8h] ; QtXml4_64.dll!?tagName@QDomElement@@QEBA?AVQString@@XZ
140912478: mov         qword ptr [rsp+FA8],rax
140912480: mov         rax,qword ptr [rsp+FA8]
140912488: mov         qword ptr [rsp+FB0],rax
140912490: lea         rdx,[00000001427BB7A8h] ; 'PointPair'
140912497: mov         rcx,qword ptr [rsp+FB0]
14091249f: call        qword ptr [141E04468h] ; QtCore4_64.dll!??8QString@@QEBA_NPEBD@Z
1409124a5: mov         byte ptr [rsp+8E0],al
1409124ac: lea         rcx,[rsp+8E8]
1409124b4: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409124ba: movzx       edx,byte ptr [rsp+8E0]
1409124c2: test        edx,edx
1409124c4: je          14091266E
1409124ca: lea         rdx,[rsp+320]
1409124d2: lea         rcx,[rsp+2D0]
1409124da: call        qword ptr [141E0C5A0h] ; QtXml4_64.dll!?text@QDomElement@@QEBA?AVQString@@XZ
1409124e0: nop
1409124e1: lea         rdx,[00000001427BB574h] ; ', '
1409124e8: lea         rcx,[rsp+8F0]
1409124f0: call        qword ptr [141E04480h] ; QtCore4_64.dll!??0QString@@QEAA@PEBD@Z
1409124f6: nop
1409124f7: mov         dword ptr [rsp+20h],1
1409124ff: xor         r9d,r9d
140912502: lea         r8,[rsp+8F0]
14091250a: lea         rdx,[rsp+300]
140912512: lea         rcx,[rsp+320]
14091251a: call        qword ptr [141E04288h] ; QtCore4_64.dll!?split@QString@@QEBA?AVQStringList@@AEBV1@W4SplitBehavior@1@W4CaseSensitivity@Qt@@@Z
140912520: nop
140912521: lea         rcx,[rsp+8F0]
140912529: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091252f: xor         edx,edx
140912531: lea         rcx,[rsp+300]
140912539: call        140004B40
14091253e: xor         edx,edx
140912540: mov         rcx,rax
140912543: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
140912549: movsd       mmword ptr [rsp+308],xmm0
140912552: mov         edx,1
140912557: lea         rcx,[rsp+300]
14091255f: call        140004B40
140912564: xor         edx,edx
140912566: mov         rcx,rax
140912569: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
14091256f: movsd       mmword ptr [rsp+2F8],xmm0
140912578: mov         edx,2
14091257d: lea         rcx,[rsp+300]
140912585: call        140004B40
14091258a: xor         edx,edx
14091258c: mov         rcx,rax
14091258f: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
140912595: movsd       mmword ptr [rsp+2E0],xmm0
14091259e: mov         edx,3
1409125a3: lea         rcx,[rsp+300]
1409125ab: call        140004B40
1409125b0: xor         edx,edx
1409125b2: mov         rcx,rax
1409125b5: call        qword ptr [141E05810h] ; QtCore4_64.dll!?toDouble@QString@@QEBANPEA_N@Z
1409125bb: movsd       mmword ptr [rsp+2D8],xmm0
1409125c4: movsd       xmm2,mmword ptr [rsp+2F8]
1409125cd: movsd       xmm1,mmword ptr [rsp+308]
1409125d6: lea         rcx,[rsp+310]
1409125de: call        qword ptr [141E057D8h] ; QtCore4_64.dll!??0QPointF@@QEAA@NN@Z
1409125e4: movsd       xmm2,mmword ptr [rsp+2D8]
1409125ed: movsd       xmm1,mmword ptr [rsp+2E0]
1409125f6: lea         rcx,[rsp+2E8]
1409125fe: call        qword ptr [141E057D8h] ; QtCore4_64.dll!??0QPointF@@QEAA@NN@Z
140912604: lea         rdi,[rsp+960]
14091260c: lea         rsi,[rsp+2E8]
140912614: mov         ecx,10h
140912619: rep movs    byte ptr [rdi],byte ptr [rsi]
14091261b: lea         rdi,[rsp+950]
140912623: lea         rsi,[rsp+310]
14091262b: mov         ecx,10h
140912630: rep movs    byte ptr [rdi],byte ptr [rsi]
140912632: lea         r9,[rsp+960]
14091263a: lea         r8,[rsp+950]
140912642: xor         edx,edx
140912644: mov         rcx,qword ptr [rsp+0A8]
14091264c: call        140818080
140912651: nop
140912652: lea         rcx,[rsp+300]
14091265a: call        1400048D0
14091265f: nop
140912660: lea         rcx,[rsp+320]
140912668: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091266e: lea         rdx,[rsp+8F8]
140912676: lea         rcx,[rsp+2C8]
14091267e: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
140912684: mov         qword ptr [rsp+FB8],rax
14091268c: mov         rax,qword ptr [rsp+FB8]
140912694: mov         qword ptr [rsp+FC0],rax
14091269c: mov         rdx,qword ptr [rsp+FC0]
1409126a4: lea         rcx,[rsp+2C8]
1409126ac: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
1409126b2: nop
1409126b3: lea         rcx,[rsp+8F8]
1409126bb: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
1409126c1: nop
1409126c2: lea         rcx,[rsp+2D0]
1409126ca: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
1409126d0: jmp         140912419
1409126d5: lea         rcx,[rsp+2C8]
1409126dd: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
1409126e3: lea         rdx,[rsp+900]
1409126eb: lea         rcx,[rsp+220]
1409126f3: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
1409126f9: mov         qword ptr [rsp+FC8],rax
140912701: mov         rax,qword ptr [rsp+FC8]
140912709: mov         qword ptr [rsp+FD0],rax
140912711: mov         rdx,qword ptr [rsp+FD0]
140912719: lea         rcx,[rsp+220]
140912721: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
140912727: nop
140912728: lea         rcx,[rsp+900]
140912730: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912736: nop
140912737: lea         rcx,[rsp+260]
14091273f: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
140912745: jmp         140912003
14091274a: lea         rcx,[rsp+220]
140912752: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912758: nop
140912759: lea         rcx,[rsp+230]
140912761: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912767: nop
140912768: lea         rcx,[rsp+248]
140912770: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912776: nop
140912777: lea         rcx,[rsp+250]
14091277f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912785: nop
140912786: lea         rcx,[rsp+240]
14091278e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912794: nop
140912795: lea         rcx,[rsp+238]
14091279d: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409127a3: nop
1409127a4: lea         rcx,[rsp+258]
1409127ac: call        qword ptr [141E03F88h] ; QtCore4_64.dll!??1QFileInfo@@QEAA@XZ
1409127b2: lea         rdx,[rsp+908]
1409127ba: lea         rcx,[rsp+098]
1409127c2: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
1409127c8: mov         qword ptr [rsp+FD8],rax
1409127d0: mov         rax,qword ptr [rsp+FD8]
1409127d8: mov         qword ptr [rsp+FE0],rax
1409127e0: mov         rdx,qword ptr [rsp+FE0]
1409127e8: lea         rcx,[rsp+098]
1409127f0: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
1409127f6: nop
1409127f7: lea         rcx,[rsp+908]
1409127ff: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912805: nop
140912806: lea         rcx,[rsp+0B8]
14091280e: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
140912814: jmp         14090F42C
140912819: lea         rcx,[rsp+098]
140912821: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912827: nop
140912828: lea         rcx,[rsp+088]
140912830: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912836: nop
140912837: lea         rcx,[rsp+090]
14091283f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140912845: lea         rdx,[rsp+910]
14091284d: lea         rcx,[rsp+58h]
140912852: call        qword ptr [141E0C598h] ; QtXml4_64.dll!?nextSibling@QDomNode@@QEBA?AV1@XZ
140912858: mov         qword ptr [rsp+FE8],rax
140912860: mov         rax,qword ptr [rsp+FE8]
140912868: mov         qword ptr [rsp+FF0],rax
140912870: mov         rdx,qword ptr [rsp+FF0]
140912878: lea         rcx,[rsp+58h]
14091287d: call        qword ptr [141E0C560h] ; QtXml4_64.dll!??4QDomNode@@QEAAAEAV0@AEBV0@@Z
140912883: nop
140912884: lea         rcx,[rsp+910]
14091288c: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
140912892: nop
140912893: lea         rcx,[rsp+78h]
140912898: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
14091289e: jmp         14090EE79
1409128a3: mov         rax,qword ptr [rsp+0000000000001030h]
1409128ab: mov         rax,qword ptr [rax]
1409128ae: mov         rcx,qword ptr [rsp+0000000000001030h]
1409128b6: call        qword ptr [rax+00000000000001E0h]
1409128bc: lea         rcx,[rsp+70h]
1409128c1: call        140004AE0
1409128c6: mov         dword ptr [rsp+60h],eax
1409128ca: mov         dword ptr [rsp+50h],0
1409128d2: jmp         1409128DF
1409128d4: mov         eax,dword ptr [rsp+50h]
1409128d8: add         eax,1
1409128db: mov         dword ptr [rsp+50h],eax
1409128df: mov         eax,dword ptr [rsp+60h]
1409128e3: cmp         dword ptr [rsp+50h],eax
1409128e7: jge         1409129B5
1409128ed: mov         edx,dword ptr [rsp+50h]
1409128f1: lea         rcx,[rsp+70h]
1409128f6: call        140160C10
1409128fb: mov         rax,qword ptr [rax]
1409128fe: mov         qword ptr [rsp+328],rax
140912906: cmp         qword ptr [rsp+328],0
14091290f: je          1409129B0
140912915: lea         rax,[rsp+918]
14091291d: mov         qword ptr [rsp+920],rax
140912925: mov         edx,dword ptr [rsp+50h]
140912929: lea         rcx,[rsp+68h]
14091292e: call        140004B40
140912933: mov         rdx,rax
140912936: mov         rcx,qword ptr [rsp+920]
14091293e: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140912944: mov         qword ptr [rsp+FF8],rax
14091294c: mov         rax,qword ptr [rsp+0000000000001030h]
140912954: mov         rax,qword ptr [rax]
140912957: mov         rdx,qword ptr [rsp+FF8]
14091295f: mov         rcx,qword ptr [rsp+0000000000001030h]
140912967: call        qword ptr [rax+0000000000000180h]
14091296d: mov         edx,eax
14091296f: mov         rax,qword ptr [rsp+0000000000001030h]
140912977: mov         rax,qword ptr [rax]
14091297a: mov         rcx,qword ptr [rsp+0000000000001030h]
140912982: call        qword ptr [rax+00000000000001A0h]
140912988: mov         qword ptr [rsp+330],rax
140912990: cmp         qword ptr [rsp+330],0
140912999: je          1409129B0
14091299b: mov         rdx,qword ptr [rsp+330]
1409129a3: mov         rcx,qword ptr [rsp+328]
1409129ab: call        140100A60
1409129b0: jmp         1409128D4
1409129b5: movzx       eax,byte ptr [rsp+64h]
1409129ba: mov         byte ptr [rsp+928],al
1409129c1: lea         rcx,[rsp+58h]
1409129c6: call        qword ptr [141E0C6A8h] ; QtXml4_64.dll!??1QDomNode@@QEAA@XZ
1409129cc: nop
1409129cd: lea         rcx,[rsp+68h]
1409129d2: call        140004B00
1409129d7: nop
1409129d8: lea         rcx,[rsp+70h]
1409129dd: call        1402081A0
1409129e2: nop
1409129e3: mov         rcx,qword ptr [rsp+0000000000001020h]
1409129eb: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1409129f1: nop
1409129f2: mov         rcx,qword ptr [rsp+0000000000001028h]
1409129fa: call        qword ptr [141E0C6B0h] ; QtXml4_64.dll!??1QDomElement@@QEAA@XZ
140912a00: mov         al,byte ptr [rsp+928]
140912a07: add         rsp,1008h
140912a0e: pop         rdi
140912a0f: pop         rsi
140912a10: ret
