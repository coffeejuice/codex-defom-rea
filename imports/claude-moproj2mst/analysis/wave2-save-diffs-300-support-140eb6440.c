
void FUN_140eb6440(longlong param_1,longlong param_2,int param_3)

{
  char cVar1;
  undefined1 auStack_b8 [8];
  char cStack_b0;
  undefined1 auStack_a8 [8];
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined1 auStack_80 [8];
  undefined1 auStack_78 [8];
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [8];
  undefined1 auStack_60 [8];
  undefined1 auStack_58 [8];
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_50 = 0xfffffffffffffffe;
  if (param_3 == 0) {
    uStack_90 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x690);
    uStack_a0 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x710);
  }
  else {
    uStack_90 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x728);
    uStack_a0 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x7a8);
  }
  cStack_b0 = '\x01';
  FUN_140f34710(auStack_b8,&UNK_142b14478,0);
  FUN_140f34710(auStack_a8,&UNK_142b14490,0);
  FUN_140f34710(auStack_80,&UNK_142b144a8,0);
  uStack_48 = FUN_140f34710(auStack_70,&UNK_142b144c0,0);
  uStack_40 = uStack_48;
  uStack_38 = __0QString__QEAA_PEBD_Z(auStack_68,&UNK_142b10344);
  uStack_30 = uStack_38;
  uStack_28 = FUN_140f34710(auStack_60,&UNK_142b14510,0);
  uStack_20 = uStack_28;
  uStack_18 = FUN_1400048f0(auStack_58,uStack_28,uStack_30);
  uStack_10 = uStack_18;
  FUN_1400048f0(auStack_78,uStack_18,uStack_40);
  __1QString__QEAA_XZ(auStack_58);
  __1QString__QEAA_XZ(auStack_60);
  __1QString__QEAA_XZ(auStack_68);
  __1QString__QEAA_XZ(auStack_70);
  if (param_2 == 0) {
    cStack_b0 = '\0';
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_b8,auStack_80);
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_a8,auStack_80);
  }
  else if (*(int *)(param_2 + 0xc) == 5) {
    cStack_b0 = '\0';
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_b8,auStack_78);
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_a8,auStack_78);
  }
  _setToolTip_QWidget__QEAAXAEBVQString___Z(uStack_90,auStack_b8);
  _setToolTip_QWidget__QEAAXAEBVQString___Z(uStack_a0,auStack_a8);
  if (cStack_b0 == '\0') {
    __1QString__QEAA_XZ(auStack_78);
    __1QString__QEAA_XZ(auStack_80);
    __1QString__QEAA_XZ(auStack_a8);
    __1QString__QEAA_XZ(auStack_b8);
  }
  else {
    uStack_88 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x6a0);
    uStack_98 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x6b0);
    if (param_3 != 0) {
      uStack_88 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x738);
      uStack_98 = *(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x748);
    }
    cVar1 = FUN_140ed97c0(param_1,param_2);
    if (cVar1 == '\0') {
      _setChecked_QAbstractButton__QEAAX_N_Z(uStack_98,1);
      _setEnabled_QWidget__QEAAX_N_Z(uStack_88,0);
    }
    else {
      _setEnabled_QWidget__QEAAX_N_Z(uStack_88,1);
    }
    __1QString__QEAA_XZ(auStack_78);
    __1QString__QEAA_XZ(auStack_80);
    __1QString__QEAA_XZ(auStack_a8);
    __1QString__QEAA_XZ(auStack_b8);
  }
  return;
}

