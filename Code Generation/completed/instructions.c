/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include "instructions.h"

#define MAX_BLOCK 50

CodeBlock* createCodeBlock(int maxSize) {
  CodeBlock* codeBlock = (CodeBlock*) malloc(sizeof(CodeBlock));

  codeBlock->code = (Instruction*) malloc(maxSize * sizeof(Instruction));
  codeBlock->codeSize = 0;
  codeBlock->maxSize = maxSize;
  return codeBlock;
}

void freeCodeBlock(CodeBlock* codeBlock) {
  free(codeBlock->code);
  free(codeBlock);
}

int emitCode(CodeBlock* codeBlock, enum OpCode op, WORD p, WORD q) {
  Instruction* bottom = codeBlock->code + codeBlock->codeSize;

  if (codeBlock->codeSize >= codeBlock->maxSize) return 0;

  bottom->op = op;
  bottom->p = p;
  bottom->q = q;
  codeBlock->codeSize ++;
  return 1;
}

int emitLA(CodeBlock* codeBlock, WORD p, WORD q) { return emitCode(codeBlock, OP_LA, p, q); }
int emitLV(CodeBlock* codeBlock, WORD p, WORD q) { return emitCode(codeBlock, OP_LV, p, q); }
int emitLC(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_LC, DC_VALUE, q); }
int emitLI(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_LI, DC_VALUE, DC_VALUE); }
int emitINT(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_INT, DC_VALUE, q); }
int emitDCT(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_DCT, DC_VALUE, q); }
int emitJ(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_J, DC_VALUE, q); }
int emitFJ(CodeBlock* codeBlock, WORD q) { return emitCode(codeBlock, OP_FJ, DC_VALUE, q); }
int emitHL(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_HL, DC_VALUE, DC_VALUE); }
int emitST(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_ST, DC_VALUE, DC_VALUE); }
int emitCALL(CodeBlock* codeBlock, WORD p, WORD q) { return emitCode(codeBlock, OP_CALL, p, q); }
int emitEP(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_EP, DC_VALUE, DC_VALUE); }
int emitEF(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_EF, DC_VALUE, DC_VALUE); }
int emitRC(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_RC, DC_VALUE, DC_VALUE); }
int emitRI(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_RI, DC_VALUE, DC_VALUE); }
int emitWRC(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_WRC, DC_VALUE, DC_VALUE); }
int emitWRI(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_WRI, DC_VALUE, DC_VALUE); }
int emitWLN(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_WLN, DC_VALUE, DC_VALUE); }
int emitAD(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_AD, DC_VALUE, DC_VALUE); }
int emitSB(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_SB, DC_VALUE, DC_VALUE); }
int emitML(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_ML, DC_VALUE, DC_VALUE); }
int emitDV(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_DV, DC_VALUE, DC_VALUE); }
int emitNEG(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_NEG, DC_VALUE, DC_VALUE); }
int emitCV(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_CV, DC_VALUE, DC_VALUE); }
int emitEQ(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_EQ, DC_VALUE, DC_VALUE); }
int emitNE(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_NE, DC_VALUE, DC_VALUE); }
int emitGT(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_GT, DC_VALUE, DC_VALUE); }
int emitLT(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_LT, DC_VALUE, DC_VALUE); }
int emitGE(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_GE, DC_VALUE, DC_VALUE); }
int emitLE(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_LE, DC_VALUE, DC_VALUE); }

int emitBP(CodeBlock* codeBlock) { return emitCode(codeBlock, OP_BP, DC_VALUE, DC_VALUE); }


void printInstruction(Instruction* inst) {
  switch (inst->op) {
  case OP_LA: printf("LA %d,%d", inst->p, inst->q); break;
  case OP_LV: printf("LV %d,%d", inst->p, inst->q); break;
  case OP_LC: printf("LC %d", inst->q); break;
  case OP_LI: printf("LI"); break;
  case OP_INT: printf("INT %d", inst->q); break;
  case OP_DCT: printf("DCT %d", inst->q); break;
  case OP_J: printf("J %d", inst->q); break;
  case OP_FJ: printf("FJ %d", inst->q); break;
  case OP_HL: printf("HL"); break;
  case OP_ST: printf("ST"); break;
  case OP_CALL: printf("CALL %d,%d", inst->p, inst->q); break;
  case OP_EP: printf("EP"); break;
  case OP_EF: printf("EF"); break;
  case OP_RC: printf("RC"); break;
  case OP_RI: printf("RI"); break;
  case OP_WRC: printf("WRC"); break;
  case OP_WRI: printf("WRI"); break;
  case OP_WLN: printf("WLN"); break;
  case OP_AD: printf("AD"); break;
  case OP_SB: printf("SB"); break;
  case OP_ML: printf("ML"); break;
  case OP_DV: printf("DV"); break;
  case OP_NEG: printf("NEG"); break;
  case OP_CV: printf("CV"); break;
  case OP_EQ: printf("EQ"); break;
  case OP_NE: printf("NE"); break;
  case OP_GT: printf("GT"); break;
  case OP_LT: printf("LT"); break;
  case OP_GE: printf("GE"); break;
  case OP_LE: printf("LE"); break;

  case OP_BP: printf("BP"); break;
  default: break;
  }
}

void printCodeBlock(CodeBlock* codeBlock) {
  Instruction* pc = codeBlock->code;
  int i;
  for (i = 0 ; i < codeBlock->codeSize; i ++) {
    printf("%d:  ",i);
    printInstruction(pc);
    printf("\n");
    pc ++;
  }
}


void loadCode(CodeBlock* codeBlock, FILE* f) {
  Instruction* code = codeBlock->code;
  int n;

  codeBlock->codeSize = 0;
  while (!feof(f)) {
    n = fread(code, sizeof(Instruction), MAX_BLOCK, f);
    code += n;
    codeBlock->codeSize += n;
  }
}


void saveCode(CodeBlock* codeBlock, FILE* f) {
  fwrite(codeBlock->code, sizeof(Instruction), codeBlock->codeSize, f);
}
