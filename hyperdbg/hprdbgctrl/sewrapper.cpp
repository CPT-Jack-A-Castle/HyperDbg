/**
 * @file debugger.cpp
 * @author M.H. Gholamrezei (gholamrezaei.mh@gmail.com)
 * @author Sina Karvandi (sina@rayanfam.com)
 * @brief Interpret general fields
 * @details
 * @version 0.1
 * @date 2020-10-25
 *
 * @copyright This project is released under the GNU Public License v3.
 *
 */
#include "pch.h"

//
// Include parser
//
#define SCRIPT_ENGINE_USER_MODE
#include "ScriptEngineCommon.h"

//
// Function links (wrapper)
//

PVOID ScriptEngineParseWrapper(char *str) {
  PSYMBOL_BUFFER SymbolBuffer;
  SymbolBuffer = ScriptEngineParse(str);

  //
  // Check if there is an error or not
  //
  if (SymbolBuffer->Message == NULL) {
    return SymbolBuffer;
  } else {
    return NULL;
  }
}

void PrintSymbolBufferWrapper(PVOID SymbolBuffer) {

  PrintSymbolBuffer((PSYMBOL_BUFFER)SymbolBuffer);
}

//
// test function
//
VOID ScriptEngineWrapperTestPerformAction(PGUEST_REGS_USER_MODE GuestRegs,
                                          string Expr) {

  //
  // Test Parser
  //
  PSYMBOL_BUFFER CodeBuffer = ScriptEngineParse((char *)Expr.c_str());

  UINT64 g_TempList[MAX_TEMP_COUNT] = {0};
  UINT64 g_VariableList[MAX_VAR_COUNT] = {0};

  if (CodeBuffer->Message == NULL) {
    PrintSymbolBuffer(CodeBuffer);

    for (int i = 0; i < CodeBuffer->Pointer;) {
      printf("%d\n", i);

      ScriptEngineExecute(GuestRegs, NULL, FALSE, (UINT64 *)g_TempList,
                          (UINT64 *)g_VariableList, CodeBuffer, &i);
    }
    RemoveSymbolBuffer(CodeBuffer);
  } else {
    printf("%s\n", CodeBuffer->Message);
  }
}

VOID ScriptEngineWrapperTestParser(string Expr) {

  typedef struct _TEST_STRUCT {
    UINT64 Var1;
    UINT64 Var2;
    UINT64 Var3;
    UINT64 Var4;
  } TEST_STRUCT, *PTEST_STRUCT;

  PTEST_STRUCT TestStruct = (PTEST_STRUCT)malloc(sizeof(TEST_STRUCT));
  RtlZeroMemory(TestStruct, sizeof(TEST_STRUCT));

  TestStruct->Var1 = 0x41414141;
  TestStruct->Var3 = 0x4242424242424242;

  GUEST_REGS_USER_MODE GuestRegs = {0};

  GuestRegs.rax = 0x1;
  GuestRegs.rcx = (UINT64)TestStruct;
  GuestRegs.rdx = 0x3;
  GuestRegs.rbx = 0x4;
  GuestRegs.rsp = 0x5;
  GuestRegs.rbp = 0x6;
  GuestRegs.rsi = 0x7;
  GuestRegs.rdi = 0x8;
  GuestRegs.r8 = 0x9;
  GuestRegs.r9 = 0xa;
  GuestRegs.r10 = 0xb;
  GuestRegs.r11 = 0xc;
  GuestRegs.r12 = 0xd;
  GuestRegs.r13 = 0xe;
  GuestRegs.r14 = 0xf;
  GuestRegs.r15 = 0x10;

  ScriptEngineWrapperTestPerformAction(&GuestRegs, Expr);
}

UINT64 ScriptEngineWrapperGetHead(PVOID SymbolBuffer) {

  return (UINT64)((PSYMBOL_BUFFER)SymbolBuffer)->Head;
}

UINT32 ScriptEngineWrapperGetSize(PVOID SymbolBuffer) {

  return (UINT32)((PSYMBOL_BUFFER)SymbolBuffer)->Size;
}

UINT32 ScriptEngineWrapperGetPointer(PVOID SymbolBuffer) {

  return (UINT32)((PSYMBOL_BUFFER)SymbolBuffer)->Pointer;
}
