
void FUN_140ec17c0(longlong param_1)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  longlong lStack_50;
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined1 auStack_28 [8];
  undefined1 auStack_20 [8];
  undefined1 auStack_18 [8];
  undefined8 uStack_10;
  
  uStack_10 = 0xfffffffffffffffe;
  uVar2 = func_0x000140917db0(1);
  uVar3 = func_0x000140917db0(0);
  lStack_50 = 0;
  cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x978));
  if (cVar1 == '\0') {
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x988));
    if (cVar1 != '\0') {
      lStack_50 = 0;
    }
  }
  else {
    lStack_50 = *(longlong *)(*(longlong *)(param_1 + 0x140) + 0x978);
  }
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 == '\0') {
    if (lStack_50 == 0) {
      __0QString__QEAA_PEBD_Z(auStack_30,uVar2);
      _setText_QLabel__QEAAXAEBVQString___Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x998),auStack_30);
      __1QString__QEAA_XZ(auStack_30);
    }
    else {
      __0QString__QEAA_PEBD_Z(auStack_28,&UNK_142b0ffcf);
      _setText_QLabel__QEAAXAEBVQString___Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x998),auStack_28);
      __1QString__QEAA_XZ(auStack_28);
    }
  }
  else {
    __0QString__QEAA_PEBD_Z(auStack_38,uVar2);
    _setText_QLabel__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x958),auStack_38);
    __1QString__QEAA_XZ(auStack_38);
  }
  __0QString__QEAA_PEBD_Z(auStack_20,uVar2);
  _setText_QLabel__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x940),auStack_20);
  __1QString__QEAA_XZ(auStack_20);
  __0QString__QEAA_PEBD_Z(auStack_18,uVar3);
  _setText_QLabel__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x910),auStack_18);
  __1QString__QEAA_XZ(auStack_18);
  return;
}

