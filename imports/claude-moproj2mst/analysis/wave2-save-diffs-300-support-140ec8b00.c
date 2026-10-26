
void FUN_140ec8b00(longlong *param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  int iStack_88;
  undefined1 auStack_80 [8];
  longlong lStack_78;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [8];
  undefined4 auStack_60 [2];
  undefined1 auStack_58 [8];
  undefined1 auStack_50 [8];
  undefined1 auStack_48 [8];
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_40 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_70);
  FUN_1400653e0(auStack_80);
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x1650));
  _clear_QComboBox__QEAAXXZ(*(undefined8 *)(param_1[0x28] + 0x1688));
  auStack_60[0] = 0;
  uStack_38 = FUN_140f34710(auStack_68,&UNK_142b14ee0,0);
  uStack_30 = uStack_38;
  uStack_28 = FUN_140ec98e0(param_1,auStack_58,auStack_60,uStack_38);
  uStack_20 = uStack_28;
  FUN_140cca3c0(auStack_80,uStack_28);
  __1QString__QEAA_XZ(auStack_58);
  __1QString__QEAA_XZ(auStack_68);
  iStack_88 = 0;
  while( true ) {
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    iVar2 = FUN_140c7c1a0(uVar3);
    if (iVar2 <= iStack_88) {
      FUN_1400fcc50(auStack_80);
      _insertItems_QComboBox__QEAAXHAEBVQStringList___Z
                (*(undefined8 *)(param_1[0x28] + 0x1650),0,auStack_80);
      _insertItems_QComboBox__QEAAXHAEBVQStringList___Z
                (*(undefined8 *)(param_1[0x28] + 0x1688),0,auStack_80);
      FUN_1400048d0(auStack_80);
      __1QString__QEAA_XZ(auStack_70);
      return;
    }
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    lStack_78 = FUN_140c7aed0(uVar3);
    if (lStack_78 == 0) break;
    cVar1 = FUN_140834fa0(lStack_78);
    if (cVar1 != '\0') {
      __0QString__QEAA_PEBD_Z(auStack_50,lStack_78 + 0x828);
      uStack_18 = FUN_140ec98e0(param_1,auStack_48,lStack_78 + 8,auStack_50);
      uStack_10 = uStack_18;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_70,uStack_18);
      __1QString__QEAA_XZ(auStack_48);
      __1QString__QEAA_XZ(auStack_50);
      FUN_140cca3c0(auStack_80);
    }
    iStack_88 = iStack_88 + 1;
  }
  FUN_1400048d0(auStack_80);
  __1QString__QEAA_XZ(auStack_70);
  return;
}

