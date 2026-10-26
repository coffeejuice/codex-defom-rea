
void FUN_140ecfc30(longlong param_1,int param_2)

{
  longlong lVar1;
  
  lVar1 = FUN_140130f90(param_1);
  if (param_2 == 0) {
    *(undefined8 *)(*(longlong *)(lVar1 + 0x378) + (longlong)(*(int *)(lVar1 + 0x32c) << 1) * 8) = 0
    ;
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x13c0),0);
  }
  else if (param_2 == 1) {
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x13c0),1);
  }
  return;
}

