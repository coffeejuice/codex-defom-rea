
void FUN_140ecc540(longlong param_1,int param_2)

{
  char cVar1;
  int iVar2;
  int aiStackX_10 [6];
  int aiStack_28 [2];
  longlong lStack_20;
  undefined4 auStack_18 [6];
  
  aiStackX_10[0] = param_2;
  lStack_20 = FUN_140130f90(param_1);
  if (lStack_20 != 0) {
    aiStack_28[0] = 0;
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f0));
    if (cVar1 != '\0') {
      aiStack_28[0] = 0;
    }
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f8));
    if (cVar1 != '\0') {
      aiStack_28[0] = 1;
    }
    cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300));
    if (cVar1 != '\0') {
      aiStack_28[0] = 2;
    }
    if (aiStack_28[0] == 2) {
      auStack_18[0] = 1;
      func_0x0001408da6f0(lStack_20,auStack_18);
    }
    else {
      func_0x0001408da6f0(lStack_20,aiStack_28);
    }
    iVar2 = func_0x0001408da6d0(lStack_20);
    if (iVar2 == 0) {
      if (aiStackX_10[0] == 0) {
        _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x200),1);
        _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x210),0);
      }
      else {
        _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x200),0);
        _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x210),1);
      }
      FUN_14089b450(*(undefined8 *)(param_1 + 0xe8),aiStackX_10);
    }
    else {
      iVar2 = func_0x0001408da6d0(lStack_20);
      if (iVar2 == 1) {
        if (aiStackX_10[0] == 0) {
          _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x248),1);
          _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x240),0);
        }
        else {
          _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x248),0);
          _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x240),1);
        }
        FUN_14089b450(*(undefined8 *)(param_1 + 0xe8),aiStackX_10);
      }
    }
  }
  return;
}

