
undefined8 FUN_140c9b8c0(undefined8 param_1)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  
  lVar1 = func_0x000140c9b7c0(param_1);
  if (lVar1 == 0) {
    uVar3 = 1;
  }
  else {
    plVar2 = (longlong *)func_0x000140c9b7c0(param_1);
    uVar3 = (**(code **)(*plVar2 + 0x58))(plVar2);
  }
  return uVar3;
}

