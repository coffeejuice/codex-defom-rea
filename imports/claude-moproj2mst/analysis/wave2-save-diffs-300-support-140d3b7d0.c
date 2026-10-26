
void FUN_140d3b7d0(longlong param_1,undefined8 param_2,char param_3)

{
  char cVar1;
  int iVar2;
  undefined1 *puVar3;
  longlong lVar4;
  undefined1 *puVar5;
  longlong lStack_318;
  undefined1 auStack_310 [16];
  undefined1 auStack_300 [16];
  undefined1 auStack_2f0 [16];
  undefined1 auStack_2e0 [16];
  undefined1 auStack_2d0 [16];
  undefined1 auStack_2c0 [16];
  undefined1 auStack_2b0 [16];
  undefined1 auStack_2a0 [16];
  undefined1 auStack_290 [16];
  undefined1 auStack_280 [16];
  undefined1 auStack_270 [16];
  undefined1 auStack_260 [16];
  undefined1 auStack_250 [16];
  undefined1 auStack_240 [16];
  undefined1 auStack_230 [16];
  undefined1 auStack_220 [16];
  undefined1 auStack_210 [16];
  undefined1 auStack_200 [16];
  undefined1 auStack_1f0 [16];
  undefined1 auStack_1e0 [16];
  undefined1 auStack_1d0 [16];
  undefined4 auStack_1c0 [2];
  undefined1 auStack_1b8 [16];
  undefined1 auStack_1a8 [16];
  undefined4 auStack_198 [4];
  undefined1 auStack_188 [16];
  undefined1 auStack_178 [16];
  undefined1 auStack_168 [16];
  undefined1 auStack_158 [16];
  undefined1 auStack_148 [16];
  undefined1 auStack_138 [16];
  undefined1 auStack_128 [16];
  undefined1 auStack_118 [16];
  undefined1 auStack_108 [16];
  undefined1 auStack_f8 [16];
  undefined1 auStack_e8 [16];
  undefined1 auStack_d8 [16];
  undefined1 auStack_c8 [16];
  undefined1 auStack_b8 [16];
  undefined1 auStack_a8 [16];
  undefined1 auStack_98 [16];
  undefined1 auStack_88 [16];
  undefined1 auStack_78 [16];
  undefined1 auStack_68 [16];
  undefined1 auStack_58 [16];
  undefined1 auStack_48 [16];
  undefined1 auStack_38 [16];
  undefined1 auStack_28 [24];
  
  if (*(int *)(param_1 + 0x30) == 2) {
    cVar1 = FUN_1402ca9a0(0x143d766b8,0);
  }
  else {
    if ((*(int *)(param_1 + 0x30) != 1) || (param_3 == '\0')) goto code_r0x000140d3b846;
    cVar1 = FUN_1402ca920(0x143d766b8,0);
  }
  if (cVar1 != '\0') {
    return;
  }
code_r0x000140d3b846:
  lStack_318 = FUN_140d39d80(param_1,param_2);
  if (lStack_318 != 0) {
    puVar3 = (undefined1 *)FUN_140025080(auStack_270,&UNK_142a0ac50,&lStack_318);
    puVar5 = auStack_28;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = auStack_28;
    puVar5 = auStack_280;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_310,0,0);
    puVar5 = auStack_f8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_300,0,0);
    puVar5 = auStack_108;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2f0,0,0);
    puVar5 = auStack_118;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2e0,0,0);
    puVar5 = auStack_128;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2d0,0,0);
    puVar5 = auStack_138;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2c0,0,0);
    puVar5 = auStack_148;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2b0,0,0);
    puVar5 = auStack_158;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2a0,0,0);
    puVar5 = auStack_168;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_290,0,0);
    puVar5 = auStack_178;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = auStack_280;
    puVar5 = auStack_188;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    FUN_1402cf3c0(0x143d76700,&UNK_142a0ac58,auStack_188,auStack_178,auStack_168,auStack_158,
                  auStack_148,auStack_138,auStack_128,auStack_118,auStack_108,auStack_f8);
    *(longlong *)(param_1 + 0x28) = lStack_318;
    iVar2 = func_0x0001402cb7a0(0x143d766b8);
    if (iVar2 == 1) {
      auStack_1c0[0] = func_0x000140d4ea70(lStack_318);
      auStack_198[0] = 0;
      puVar3 = (undefined1 *)FUN_140025080(auStack_1d0,&UNK_142a072ac,auStack_1c0);
      puVar5 = auStack_38;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = auStack_38;
      puVar5 = auStack_1e0;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)FUN_140025080(auStack_1a8,&UNK_142a0741c,auStack_198);
      puVar5 = auStack_48;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = auStack_48;
      puVar5 = auStack_1b8;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_260,0,0);
      puVar5 = auStack_58;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_250,0,0);
      puVar5 = auStack_68;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_240,0,0);
      puVar5 = auStack_78;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_230,0,0);
      puVar5 = auStack_88;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_220,0,0);
      puVar5 = auStack_98;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_210,0,0);
      puVar5 = auStack_a8;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_200,0,0);
      puVar5 = auStack_b8;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_1f0,0,0);
      puVar5 = auStack_c8;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = auStack_1e0;
      puVar5 = auStack_d8;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      puVar3 = auStack_1b8;
      puVar5 = auStack_e8;
      for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
        *puVar5 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar5 = puVar5 + 1;
      }
      FUN_1402cf3c0(0x143d76700,&UNK_142a0ac78,auStack_e8,auStack_d8,auStack_c8,auStack_b8,
                    auStack_a8,auStack_98,auStack_88,auStack_78,auStack_68,auStack_58);
    }
  }
  return;
}

