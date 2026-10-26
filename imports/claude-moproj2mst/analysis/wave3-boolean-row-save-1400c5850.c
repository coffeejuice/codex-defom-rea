
undefined8 FUN_1400c5850(undefined8 param_1,int param_2,undefined8 param_3)

{
  undefined4 uVar1;
  undefined1 auStack_a8 [8];
  undefined8 *puStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  longlong lStack_88;
  undefined1 auStack_80 [8];
  undefined1 auStack_78 [8];
  undefined1 auStack_70 [8];
  undefined8 *puStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 *puStack_10;
  
  uStack_50 = 0xfffffffffffffffe;
  puStack_a0 = (undefined8 *)0x0;
  uStack_98 = 0;
  lStack_88 = FUN_14146eabc(0x30);
  if (lStack_88 == 0) {
    uStack_48 = 0;
  }
  else {
    uStack_48 = FUN_140d4bed0(lStack_88);
  }
  uStack_90 = uStack_48;
  uStack_98 = uStack_48;
  func_0x0001400eb5e0(uStack_48,1);
  _appendRow_QStandardItem__QEAAXPEAV1__Z(param_3,uStack_98);
  _number_QString__SA_AV1_HH_Z(auStack_a8,param_2,10);
  if (param_2 == 0) {
    uStack_40 = FUN_1400c81c0(auStack_80,&UNK_14215e468,0);
    uStack_38 = uStack_40;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_a8,uStack_40);
    __1QString__QEAA_XZ(auStack_80);
  }
  else if (param_2 == 1) {
    uStack_30 = FUN_1400c81c0(auStack_78,&UNK_14215e478,0);
    uStack_28 = uStack_30;
    __YQString__QEAAAEAV0_AEBV0__Z(auStack_a8,uStack_30);
    __1QString__QEAA_XZ(auStack_78);
  }
  else {
    uStack_20 = FUN_1400c81c0(auStack_70,&UNK_14215e480,0);
    uStack_18 = uStack_20;
    __YQString__QEAAAEAV0_AEBV0__Z(auStack_a8,uStack_20);
    __1QString__QEAA_XZ(auStack_70);
  }
  puStack_60 = (undefined8 *)FUN_14146eabc(0x10);
  if (puStack_60 == (undefined8 *)0x0) {
    puStack_10 = (undefined8 *)0x0;
  }
  else {
    __0QStandardItem__QEAA_AEBVQString___Z(puStack_60,auStack_a8);
    *puStack_60 = &UNK_14210eb18;
    puStack_10 = puStack_60;
  }
  puStack_68 = puStack_10;
  puStack_a0 = puStack_10;
  uVar1 = _row_QStandardItem__QEBAHXZ(uStack_98);
  _setChild_QStandardItem__QEAAXHHPEAV1__Z(param_3,uVar1,1,puStack_a0);
  uStack_58 = uStack_98;
  __1QString__QEAA_XZ(auStack_a8);
  return uStack_58;
}

