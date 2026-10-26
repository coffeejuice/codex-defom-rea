
void FUN_14002a0e0(longlong *param_1,longlong param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined1 *puVar3;
  longlong lVar4;
  undefined1 *puVar5;
  longlong alStackX_10 [3];
  undefined4 uStack_428;
  undefined4 uStack_424;
  undefined1 auStack_420 [8];
  undefined1 auStack_418 [8];
  undefined1 auStack_410 [16];
  undefined1 auStack_400 [16];
  undefined1 auStack_3f0 [16];
  undefined1 auStack_3e0 [16];
  undefined1 auStack_3d0 [16];
  undefined1 auStack_3c0 [16];
  undefined1 auStack_3b0 [16];
  undefined1 auStack_3a0 [16];
  undefined1 auStack_390 [16];
  undefined1 auStack_380 [16];
  undefined1 auStack_370 [16];
  undefined1 auStack_360 [16];
  undefined1 auStack_350 [16];
  undefined1 auStack_340 [16];
  undefined1 auStack_330 [16];
  undefined1 auStack_320 [16];
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
  undefined1 auStack_230 [24];
  undefined1 auStack_218 [16];
  undefined1 auStack_208 [16];
  undefined1 auStack_1f8 [16];
  undefined1 auStack_1e8 [16];
  undefined1 auStack_1d8 [16];
  undefined1 auStack_1c8 [16];
  undefined1 auStack_1b8 [16];
  undefined1 auStack_1a8 [16];
  undefined1 auStack_198 [16];
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
  undefined8 uStack_28;
  
  uStack_28 = 0xfffffffffffffffe;
  *(undefined1 *)(param_1 + 0x78) = 0;
  alStackX_10[0] = param_2;
  FUN_140013d40(param_1,0,0);
  if (alStackX_10[0] == 0) {
    if (param_1[0x5c] != 0) {
      FUN_140116ea0(param_1[0x5c],0);
    }
    uVar1 = __0QLatin1String__QEAA_PEBD_Z(auStack_418,&UNK_1420fc0df);
    __0QString__QEAA_AEBVQLatin1String___Z(auStack_420,uVar1);
    _setWindowTitle_QWidget__QEAAXAEBVQString___Z(param_1[0x8f],auStack_420);
    __1QString__QEAA_XZ(auStack_420);
    FUN_140018b10(param_1);
    if (param_1[0x8c] != 0) {
      (**(code **)(*(longlong *)param_1[0x8c] + 0x58))(param_1[0x8c],0);
      uVar1 = _toggleViewAction_QDockWidget__QEBAPEAVQAction__XZ(param_1[0x8c]);
      _removeAction_QWidget__QEAAXPEAVQAction___Z(param_1[0x1d],uVar1);
    }
    if (param_1[0x8e] != 0) {
      (**(code **)(*(longlong *)param_1[0x8e] + 0x58))(param_1[0x8e],0);
      uVar1 = _toggleViewAction_QDockWidget__QEBAPEAVQAction__XZ(param_1[0x8e]);
      _removeAction_QWidget__QEAAXPEAVQAction___Z(param_1[0x1d],uVar1);
    }
    puVar3 = (undefined1 *)FUN_140025080(auStack_370,&UNK_142100148,alStackX_10);
    puVar5 = auStack_38;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = auStack_38;
    puVar5 = auStack_380;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_410,0,0);
    puVar5 = auStack_188;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_400,0,0);
    puVar5 = auStack_198;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_3f0,0,0);
    puVar5 = auStack_1a8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_3e0,0,0);
    puVar5 = auStack_1b8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_3d0,0,0);
    puVar5 = auStack_1c8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_3c0,0,0);
    puVar5 = auStack_1d8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_3b0,0,0);
    puVar5 = auStack_1e8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_3a0,0,0);
    puVar5 = auStack_1f8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_390,0,0);
    puVar5 = auStack_208;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = auStack_380;
    puVar5 = auStack_218;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    FUN_1402cf3c0(0x143d76700,&UNK_142100158,auStack_218,auStack_208,auStack_1f8,auStack_1e8,
                  auStack_1d8,auStack_1c8,auStack_1b8,auStack_1a8,auStack_198,auStack_188);
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_360,0,0);
    puVar5 = auStack_e8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_350,0,0);
    puVar5 = auStack_f8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_340,0,0);
    puVar5 = auStack_108;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_330,0,0);
    puVar5 = auStack_118;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_320,0,0);
    puVar5 = auStack_128;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_310,0,0);
    puVar5 = auStack_138;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_300,0,0);
    puVar5 = auStack_148;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2f0,0,0);
    puVar5 = auStack_158;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2e0,0,0);
    puVar5 = auStack_168;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2d0,0,0);
    puVar5 = auStack_178;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    FUN_1402cf3c0(0x143d76700,&UNK_142100170,auStack_178,auStack_168,auStack_158,auStack_148,
                  auStack_138,auStack_128,auStack_118,auStack_108,auStack_f8,auStack_e8);
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2c0,0,0);
    puVar5 = auStack_48;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2b0,0,0);
    puVar5 = auStack_58;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_2a0,0,0);
    puVar5 = auStack_68;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_290,0,0);
    puVar5 = auStack_78;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_280,0,0);
    puVar5 = auStack_88;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_270,0,0);
    puVar5 = auStack_98;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_260,0,0);
    puVar5 = auStack_a8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_250,0,0);
    puVar5 = auStack_b8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_240,0,0);
    puVar5 = auStack_c8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    puVar3 = (undefined1 *)__0QGenericArgument__QEAA_PEBDPEBX_Z(auStack_230,0,0);
    puVar5 = auStack_d8;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar5 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
    }
    FUN_1402cf3c0(0x143d76700,&UNK_142100190,auStack_d8,auStack_c8,auStack_b8,auStack_a8,auStack_98,
                  auStack_88,auStack_78,auStack_68,auStack_58,auStack_48);
    uVar1 = FUN_140013750(param_1);
    lVar4 = func_0x000140024f10(uVar1);
    if (lVar4 != 0) {
      FUN_1413420e0(lVar4);
    }
  }
  else {
    if (param_1[0x5c] != 0) {
      FUN_140116ea0(param_1[0x5c],1);
    }
    uStack_428 = 0;
    uVar1 = FUN_1400a5890(alStackX_10[0]);
    uVar2 = FUN_1400133a0(param_1);
    uStack_424 = FUN_140c79ee0(uVar2,uVar1);
    FUN_14002a9b0(param_1,&uStack_424,&uStack_428,1);
  }
  (**(code **)(*param_1 + 0x2c0))(param_1);
  if ((char)param_1[0x78] == '\0') {
    (**(code **)(*param_1 + 0x7f0))(param_1,0);
  }
  return;
}

