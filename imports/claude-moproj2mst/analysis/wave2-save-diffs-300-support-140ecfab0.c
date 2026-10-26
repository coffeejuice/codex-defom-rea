
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ecfab0(longlong param_1,int param_2)

{
  longlong lVar1;
  
  lVar1 = FUN_140130f90(param_1);
  if (param_2 == 0) {
    **(undefined8 **)(lVar1 + 0x378) = 0;
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1370),0);
  }
  else if (param_2 == 1) {
    **(undefined8 **)(lVar1 + 0x378) = _UNK_1421d3c90;
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1370),0);
  }
  else if (param_2 == 2) {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1370),1);
  }
  return;
}

