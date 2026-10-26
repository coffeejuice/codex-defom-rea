
undefined8 * FUN_140eb26f0(undefined8 *param_1,longlong param_2)

{
  undefined8 uVar1;
  longlong lVar2;
  undefined1 *puVar3;
  uint uVar4;
  int iStack_e8;
  undefined1 auStack_a8 [8];
  undefined1 auStack_a0 [24];
  undefined8 uStack_88;
  longlong lStack_80;
  undefined1 auStack_78 [8];
  undefined1 *puStack_70;
  undefined8 *puStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  undefined1 *puStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 *puStack_10;
  
  uStack_58 = 0xfffffffffffffffe;
  FUN_140130740(param_1,param_2,0);
  *param_1 = &UNK_142b165b8;
  param_1[2] = &UNK_142b16580;
  __0QPixmap__QEAA_XZ(param_1 + 0x24);
  FUN_14018f9e0(param_1 + 0x2d);
  puStack_50 = (undefined1 *)FUN_14146eabc(0x1808);
  if (puStack_50 == (undefined1 *)0x0) {
    puStack_50 = (undefined1 *)0x0;
  }
  else {
    puVar3 = puStack_50;
    for (lVar2 = 0x1808; lVar2 != 0; lVar2 = lVar2 + -1) {
      *puVar3 = 0;
      puVar3 = puVar3 + 1;
    }
  }
  param_1[0x28] = puStack_50;
  FUN_140ed9830(param_1[0x28],param_1);
  if (param_2 != 0) {
    FUN_140054540(param_2);
  }
  lVar2 = FUN_14146eabc(0x28);
  if (lVar2 == 0) {
    uStack_48 = 0;
  }
  else {
    uVar1 = FUN_140377b50(param_1);
    uVar1 = FUN_140c90f10(uVar1);
    uStack_48 = FUN_140f341d0(lVar2,param_1,uVar1);
  }
  param_1[0x2a] = uStack_48;
  func_0x000140ecf300(param_1,0);
  func_0x000140ecf320(param_1,0);
  _hide_QWidget__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x16f0));
  _setCurrentIndex_QStackedWidget__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x1700),0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 8),&UNK_142b13988,param_1,&UNK_142b13958,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x58),&UNK_142b139d8,param_1,&UNK_142b139c8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x50),&UNK_142b139f8,param_1,&UNK_142b139e8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xcf8),&UNK_142b13a28,param_1,&UNK_142b13a08,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xd08),&UNK_142b13a58,param_1,&UNK_142b13a38,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xd58),&UNK_142b13a88,param_1,&UNK_142b13a68,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xd70),&UNK_142b13ab8,param_1,&UNK_142b13a98,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe08),&UNK_142b13ae8,param_1,&UNK_142b13ac8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe28),&UNK_142b13b18,param_1,&UNK_142b13af8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe70),&UNK_142b13b48,param_1,&UNK_142b13b28,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe80),&UNK_142b13b78,param_1,&UNK_142b13b58,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe78),&UNK_142b13ba8,param_1,&UNK_142b13b88,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe88),&UNK_142b13bd8,param_1,&UNK_142b13bb8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x2f0),&UNK_142b13c00,param_1,&UNK_142b13be8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x2f8),&UNK_142b13c28,param_1,&UNK_142b13c10,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x300),&UNK_142b13c50,param_1,&UNK_142b13c38,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x988),&UNK_142b13c90,param_1,&UNK_142b13c60,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x978),&UNK_142b13cd0,param_1,&UNK_142b13ca0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x7e8),&UNK_142b13cf8,param_1,&UNK_142b13ce0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x808),&UNK_142b13d20,param_1,&UNK_142b13d08,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x138),&UNK_142b13d58,param_1,&UNK_142b13d30,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x128),&UNK_142b13da0,param_1,&UNK_142b13d78,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x1610),&UNK_142b13df0,param_1,&UNK_142b13dc0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x838),&UNK_142b13e38,param_1,&UNK_142b13e10,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x708),&UNK_142b13e70,param_1,&UNK_142b13e48,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x7a0),&UNK_142b13ea8,param_1,&UNK_142b13e80,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x710),&UNK_142b13ed8,param_1,&UNK_142b13eb8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x7a8),&UNK_142b13f08,param_1,&UNK_142b13ee8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x9c0),&UNK_142b13f40,param_1,&UNK_142b13f18,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x6d0),&UNK_142b13f68,param_1,&UNK_142b13f50,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x6e0),&UNK_142b13f98,param_1,&UNK_142b13f80,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x6f0),&UNK_142b13fc8,param_1,&UNK_142b13fb0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x768),&UNK_142b13ff8,param_1,&UNK_142b13fe0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x778),&UNK_142b14028,param_1,&UNK_142b14010,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x788),&UNK_142b14058,param_1,&UNK_142b14040,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x6a8),&UNK_142b14090,param_1,&UNK_142b14070,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x740),&UNK_142b140c8,param_1,&UNK_142b140a8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x7c0),&UNK_142b14100,param_1,&UNK_142b140e0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x520),&UNK_142b14138,param_1,&UNK_142b14110,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xce0),&UNK_142b14168,param_1,&UNK_142b14150,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x240),&UNK_142b14190,param_1,&UNK_142b14178,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xde8),&UNK_142b141b8,param_1,&UNK_142b141a0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xd80),&UNK_142b141e0,param_1,&UNK_142b141c8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0xe98),&UNK_142b14208,param_1,&UNK_142b141f0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x210),&UNK_142b14230,param_1,&UNK_142b14218,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x250),&UNK_142b14260,param_1,&UNK_142b14240,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x1f8),&UNK_142b14290,param_1,&UNK_142b14270,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x9b8),&UNK_142b142c8,param_1,&UNK_142b142a0,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x11a0),&UNK_142b14300,param_1,&UNK_142b142d8,0);
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (*(undefined8 *)(param_1[0x28] + 0x1180),&UNK_142b14338,param_1,&UNK_142b14310,0);
  _setCurrentRow_QListWidget__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 8),0);
  param_1[10] = *(undefined8 *)(param_1[0x28] + 0x78);
  param_1[0xb] = *(undefined8 *)(param_1[0x28] + 0x70);
  param_1[0xc] = *(undefined8 *)(param_1[0x28] + 0x90);
  param_1[0xd] = *(undefined8 *)(param_1[0x28] + 200);
  param_1[0xe] = *(undefined8 *)(param_1[0x28] + 0xa0);
  param_1[0xf] = *(undefined8 *)(param_1[0x28] + 0xa8);
  for (iStack_e8 = 0; iStack_e8 < 6; iStack_e8 = iStack_e8 + 1) {
    lVar2 = FUN_14146eabc(0x78);
    if (lVar2 == 0) {
      uStack_40 = 0;
    }
    else {
      uStack_40 = FUN_140898dc0(lVar2);
    }
    param_1[(longlong)iStack_e8 + 0x19] = uStack_40;
  }
  param_1[0x1f] = 0;
  *(undefined4 *)(param_1 + 0x2f) = 0;
  *(undefined4 *)(param_1 + 0x20) = 0xffffffff;
  *(undefined4 *)((longlong)param_1 + 0x11c) = 0xffffffff;
  FUN_140eb7f50(param_1);
  uVar1 = _item_QListWidget__QEBAPEAVQListWidgetItem__H_Z(*(undefined8 *)(param_1[0x28] + 8),8);
  uStack_38 = _icon_QListWidgetItem__QEBA_AVQIcon__XZ(uVar1,auStack_a8);
  uStack_30 = uStack_38;
  uStack_28 = _pixmap_QIcon__QEBA_AVQPixmap__HW4Mode_1_W4State_1__Z(uStack_38,auStack_a0,0,0,1);
  uStack_20 = uStack_28;
  __4QPixmap__QEAAAEAV0_AEBV0__Z(param_1 + 0x24,uStack_28);
  __1QPixmap__UEAA_XZ(auStack_a0);
  __1QIcon__QEAA_XZ(auStack_a8);
  param_1[0x10] = *(undefined8 *)(param_1[0x28] + 0x1268);
  param_1[0x11] = *(undefined8 *)(param_1[0x28] + 0x1270);
  param_1[0x12] = *(undefined8 *)(param_1[0x28] + 0x1278);
  param_1[0x13] = *(undefined8 *)(param_1[0x28] + 0x1280);
  param_1[0x14] = *(undefined8 *)(param_1[0x28] + 0x1298);
  param_1[0x15] = *(undefined8 *)(param_1[0x28] + 0x1288);
  param_1[0x16] = *(undefined8 *)(param_1[0x28] + 0x1290);
  for (iStack_e8 = 0; iStack_e8 < 6; iStack_e8 = iStack_e8 + 1) {
    lStack_80 = FUN_14146eabc(0x78);
    if (lStack_80 == 0) {
      uStack_18 = 0;
    }
    else {
      uStack_18 = FUN_140898dc0(lStack_80);
    }
    uStack_88 = uStack_18;
    param_1[(longlong)iStack_e8 + 0x19] = uStack_18;
  }
  param_1[0x18] = 0;
  *(undefined4 *)(param_1 + 0x17) = 0;
  *(undefined4 *)((longlong)param_1 + 0xbc) = 0;
  FUN_140eb5630(param_1,0,0);
  FUN_140eb9090(param_1);
  *(undefined1 *)((longlong)param_1 + 0x105) = 0;
  FUN_140eb5370(param_1);
  lVar2 = _layout_QWidget__QEBAPEAVQLayout__XZ(param_1);
  if (lVar2 != 0) {
    uVar1 = _layout_QWidget__QEBAPEAVQLayout__XZ(param_1);
    _activate_QLayout__QEAA_NXZ(uVar1);
    _instance_QCoreApplication__SAPEAV1_XZ();
    puStack_70 = auStack_78;
    uVar1 = func_0x000140004a60(puStack_70,0);
    _processEvents_QCoreApplication__SAXV__QFlags_W4ProcessEventsFlag_QEventLoop_____Z(uVar1);
  }
  puStack_60 = (undefined8 *)FUN_14146eabc(0x20);
  if (puStack_60 == (undefined8 *)0x0) {
    puStack_10 = (undefined8 *)0x0;
  }
  else {
    __0QTimer__QEAA_PEAVQObject___Z(puStack_60,0);
    *puStack_60 = &UNK_1420fbb78;
    puStack_10 = puStack_60;
  }
  puStack_68 = puStack_10;
  param_1[0x2e] = puStack_10;
  _setSingleShot_QTimer__QEAAX_N_Z(param_1[0x2e],1);
  _setInterval_QTimer__QEAAXH_Z(param_1[0x2e],0);
  uVar4 = 0;
  _connect_QObject__SA_NPEBV1_PEBD01W4ConnectionType_Qt___Z
            (param_1[0x2e],&UNK_142b14360,param_1,&UNK_142b14348,0);
  FUN_140eb50f0(param_1);
  param_1[0x29] = 0;
  FUN_140ecf410(param_1);
  FUN_1402cfbe0(0x143d76700,&UNK_142b14390,param_1,&UNK_142b14370,uVar4 & 0xffffff00);
  FUN_140ed1010(param_1);
  return param_1;
}

