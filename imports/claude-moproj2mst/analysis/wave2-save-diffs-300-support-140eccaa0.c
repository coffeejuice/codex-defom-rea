
void FUN_140eccaa0(longlong param_1,undefined4 *param_2)

{
  int iVar1;
  longlong lVar2;
  
  lVar2 = FUN_140130f90(param_1);
  if (lVar2 != 0) {
    iVar1 = func_0x0001408da6d0(lVar2);
    if (iVar1 == 1) {
      _setCurrentIndex_QComboBox__QEAAXH_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x250),*param_2);
      FUN_140ecc540(param_1,*param_2);
    }
    else {
      _setCurrentIndex_QComboBox__QEAAXH_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1f8),*param_2);
      FUN_140ecc540(param_1,*param_2);
    }
  }
  return;
}

