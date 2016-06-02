/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define DC_VALUE 0
#define INT_SIZE 1
#define CHAR_SIZE 1

typedef int WORD;

enum OpCode {
  OP_LA,   // Load Address:    t := t + 1; s[t] := base(p) + q;
  OP_LV,   // Load Value:      t := t + 1; s[t] := s[base(p) + q];
  OP_LC,   // load Constant    t := t + 1; s[t] := q;
  OP_LI,   // Load Indirect    s[t] := s[s[t]];
  OP_INT,  // Increment t      t := t + q;
  OP_DCT,  // Decrement t      t := t - q;
  OP_J,    // Jump             pc := q;
  OP_FJ,   // False Jump       if s[t] = 0 then pc := q; t := t - 1;
  OP_HL,   // Halt             Halt
  OP_ST,   // Store            s[s[t-1]] := s[t]; t := t -2;
  OP_CALL, // Call             s[t+2] := b; s[t+3] := pc; s[t+4]:= base(p); b:=t+1; pc:=q;
  OP_EP,   // Exit Procedure   t := b - 1;  pc := s[b+2];  b := s[b+1];
  OP_EF,   // Exit Function    t := b;  pc := s[b+2];  b := s[b+1];
  OP_RC,   // Read Char        read one character into s[s[t]];  t := t - 1;
  OP_RI,   // Read Integer     read integer to s[s[t]];  t := t-1;
  OP_WRC,  // Write Char       write one character from s[t];  t := t-1;
  OP_WRI,  // Write Int        write integer from s[t];  t := t-1;
  OP_WLN,  // WriteLN          CR/LF
  OP_AD,   // Add              t := t-1;  s[t] := s[t] + s[t+1];
  OP_SB,   // Substract        t := t-1;  s[t] := s[t] - s[t+1];
  OP_ML,   // Multiple         t := t-1;  s[t] := s[t] * s[t+1];
  OP_DV,   // Divide           t := t-1;  s[t] := s[t] / s[t+1];
  OP_NEG,  // Negative         s[t] := - s[t];
  OP_CV,   // Copy Top         s[t+1] := s[t]; t := t + 1;
  OP_EQ,   // Equal            t := t - 1;  if s[t] = s[t+1] then s[t] := 1 else s[t] := 0;
  OP_NE,   // Not Equal        t := t - 1;  if s[t] != s[t+1] then s[t] := 1 else s[t] := 0;
  OP_GT,   // Greater          t := t - 1;  if s[t] > s[t+1] then s[t] := 1 else s[t] := 0;
  OP_LT,   // Less             t := t - 1;  if s[t] < s[t+1] then s[t] := 1 else s[t] := 0;
  OP_GE,   // Greater or Equal t := t - 1;  if s[t] >= s[t+1] then s[t] := 1 else s[t] := 0;
  OP_LE,   // Less or Equal    t := t - 1;  if s[t] >= s[t+1] then s[t] := 1 else s[t] := 0;

  OP_BP    // Break point. Just for debugging
};

struct Instruction_ {
  enum OpCode op;
  WORD p;
  WORD q;
};

typedef struct Instruction_ Instruction;
typedef int CodeAddress;

struct CodeBlock_ {
  Instruction* code;
  int codeSize;
  int maxSize;
};

typedef struct CodeBlock_ CodeBlock;

CodeBlock* createCodeBlock(int maxSize);
void freeCodeBlock(CodeBlock* codeBlock);

int emitCode(CodeBlock* codeBlock, enum OpCode op, WORD p, WORD q);

int emitLA(CodeBlock* codeBlock, WORD p, WORD q);
int emitLV(CodeBlock* codeBlock, WORD p, WORD q);
int emitLC(CodeBlock* codeBlock, WORD q);
int emitLI(CodeBlock* codeBlock);
int emitINT(CodeBlock* codeBlock, WORD q);
int emitDCT(CodeBlock* codeBlock, WORD q);
int emitJ(CodeBlock* codeBlock, WORD q);
int emitFJ(CodeBlock* codeBlock, WORD q);
int emitHL(CodeBlock* codeBlock);
int emitST(CodeBlock* codeBlock);
int emitCALL(CodeBlock* codeBlock, WORD p, WORD q);
int emitEP(CodeBlock* codeBlock);
int emitEF(CodeBlock* codeBlock);
int emitRC(CodeBlock* codeBlock);
int emitRI(CodeBlock* codeBlock);
int emitWRC(CodeBlock* codeBlock);
int emitWRI(CodeBlock* codeBlock);
int emitWLN(CodeBlock* codeBlock);
int emitAD(CodeBlock* codeBlock);
int emitSB(CodeBlock* codeBlock);
int emitML(CodeBlock* codeBlock);
int emitDV(CodeBlock* codeBlock);
int emitNEG(CodeBlock* codeBlock);
int emitCV(CodeBlock* codeBlock);
int emitEQ(CodeBlock* codeBlock);
int emitNE(CodeBlock* codeBlock);
int emitGT(CodeBlock* codeBlock);
int emitLT(CodeBlock* codeBlock);
int emitGE(CodeBlock* codeBlock);
int emitLE(CodeBlock* codeBlock);

int emitBP(CodeBlock* codeBlock);

void printInstruction(Instruction* instruction);
void printCodeBlock(CodeBlock* codeBlock);

void loadCode(CodeBlock* codeBlock, FILE* f);
void saveCode(CodeBlock* codeBlock, FILE* f);

#endif
