/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include "curses.h"

#include "vm_duongnn.h"



CodeBlock *codeBlock;
WORD* stack;
char* stackState;
WORD* global;
char* stackUse;

int t;
int b;
int pc;
int ps;
int max_t = -1;

int stackSize;
int codeSize;
int debugMode;
int fullMode;

void resetVM(void) {
  pc = 0;
  t = -1;
  b = 0;
  ps = PS_INACTIVE;
}

void initVM(void) {
  int i;
  codeBlock = createCodeBlock(codeSize);
  stack = (Memory) malloc(stackSize * sizeof(WORD));
  stackUse = (char*) malloc(stackSize * sizeof(char));
  for(i = 0; i< stackSize; i++)
        stackUse[i] = UNUSED;
  resetVM();
}

void cleanVM(void) {
  freeCodeBlock(codeBlock);
  free(stack);
  free(stackState);
}

int loadExecutable(FILE* f) {
  loadCode(codeBlock,f);
  resetVM();
  return 1;
}

int saveExecutable(FILE* f) {
  saveCode(codeBlock,f);
  return 1;
}

int checkStack(void) {
  return ((t >= 0) && (t <stackSize));
}

int base(int p) {
  int currentBase = b;
  while (p > 0) {
    currentBase = stack[currentBase + 3];
    p --;
  }
  return currentBase;
}

void printMemory(void) {
  int i;
  printf("Start dumping...\n");
  for (i = 0; i <= t; i++) 
    printf("  %4d: %d\n",i,stack[i]);
  printf("Finish dumping!\n");
}

void printCodeBuffer(void) {
  printCodeBlock(codeBlock);
}

int updateMaxT(){
/* Cap nhat gia tri cao nhat cua top */
   if (max_t < t)
      max_t = t;
   return max_t;
}


int ident(int i){
    int j;
    for (j = 0; j < i; j++)
        printf(" ");
    return j;
}

void updateStackUse(int pos){
     stackUse[pos] = USED;
}

int viewStackElement(int pos){
/* Hien thi noi dung mot phan tu cua stack */
   int i;
   
   ident(VIEW_STACK_IDENT);
   printf("%7s","");
   for (i = 0; i < STACK_WIDE; i++)
       printf("-");
   printf("\n");   
   
   ident(VIEW_STACK_IDENT);   
   printf("%6d",pos);
   if (stackUse[pos] == USED){
      i = printf("| %-d", stack[pos]);
      ident(STACK_WIDE - i - 1);
      printf("  |");
   }
   else {
        printf("|");
        ident(STACK_WIDE);
        printf("|");
   }
   if (pos == t)
      printf(" <- t");
   if (pos == b)
      printf(" <- b");  
   
   printf("\n");
   return 0;
}
            
    
int viewStack(void){
/* Hien thi noi dung stack tai mot thoi diem */
   int position = 0;
   int i;
   
   printf("\n");
   while (position <= max_t){
         // Hien noi dung element
         viewStackElement(position);
                  
         // Het stack
         if (position == max_t){
            ident(VIEW_STACK_IDENT  + 7);
            for (i = 0; i <STACK_WIDE; i++)
                printf("-");
            printf("\n");
         }
         
         position ++;
   }
}

int run(void) {
  Instruction* code = codeBlock->code;
  int count = 0;
  int number;
  char s[100];

//  WINDOW* win = initscr();
//  nonl();
//  cbreak();
//  noecho();
//  scrollok(win,TRUE);
  
  ps = PS_ACTIVE;
  while (ps == PS_ACTIVE) {
        if (debugMode) {
           sprintInstruction(s,&(code[pc]));
//           wprintw(win, "%6d-%-4d:  %s\n",count++,pc,s);
           printf("%6d-%-4d:  %s\n",count++,pc,s);
        }

        switch (code[pc].op) {
               case OP_LA: 
                    t ++;
                    if (checkStack())
	                stack[t] = base(code[pc].p) + code[pc].q;
                    updateMaxT();
                    updateStackUse(t);	                
                    break;
               case OP_LV: 
                    t ++;
                    if (checkStack())
	                stack[t] = stack[base(code[pc].p) + code[pc].q];
                    updateMaxT();
                    updateStackUse(t);
                    break;
               case OP_LC: 
                    t ++;
                    if (checkStack())
	                stack[t] = code[pc].q;
	                updateMaxT();
	                updateStackUse(t);
                    break;
               case OP_LI: 
                    stack[t] = stack[stack[t]];
                    break;
               case OP_INT:
                    t += code[pc].q;
                    checkStack();
                    updateMaxT();
                    break;
               case OP_DCT: 
                    t -= code[pc].q;
                    checkStack();
                    break;
               case OP_J: 
                    pc = code[pc].q - 1;
                    break;
               case OP_FJ: 
                    if (stack[t] == FALSE) 
	                   pc = code[pc].q - 1;
                    t --;
                    checkStack();
                    break;
               case OP_HL: 
                    ps = PS_NORMAL_EXIT;
                    break;
               case OP_ST: 
                    stack[stack[t-1]] = stack[t];
                    updateStackUse(stack[t-1]);
                    t -= 2;
                    checkStack();
                    break;
               case OP_CALL: 
                    stack[t+2] = b;                 // Dynamic Link
                    stack[t+3] = pc;                // Return Address
                    stack[t+4] = base(code[pc].p);  // Static Link
                    b = t + 1;                      // Base & Result
                    pc = code[pc].q - 1;            // duongnn: dang ra phai la   pc = code[pc].q   chu?
                                                    // duongnn: OK, vi sau nay se co  pc++ 
                    updateStackUse(t+2);
                    updateStackUse(t+3);
                    updateStackUse(t+4);
                    break;
               case OP_EP: 
                    t = b - 1;                      // Previous top
                    pc = stack[b+2];                // Saved return address
                    b = stack[b+1];                 // Saved base
                    updateMaxT();
                    updateStackUse(t);
                    break;
               case OP_EF:
                    t = b;                          // return value is on the top of the stack
                    pc = stack[b+2];                // Saved return address
                    b = stack[b+1];                 // saved base
                    updateMaxT();
                    updateStackUse(t);
                    break;
               case OP_RC: 
                    t ++;
//                    echo();
//                    wscanw(win,"%c",&number);
                    fflush(stdin);
                    scanf("%c",&number);

//                    noecho();
                    stack[t] = number;
                    updateStackUse(t);
                    checkStack();
                    updateMaxT();
                    break;
               case OP_RI:
                    t ++;
//                    echo();
//                    wscanw(win,"%d",&number);
                    scanf("%d",&number);
//                    noecho();
                    stack[t] = number;
                    updateStackUse(t);
                    checkStack();
                    updateMaxT();
                    break;
               case OP_WRC: 
//                    wprintw(win,"%c",stack[t]);
                    printf("%c",stack[t]);                      
                    t --;
                    updateStackUse(t);
                    checkStack();
                    break;     
               case OP_WRI: 
//                    wprintw(win,"%d",stack[t]);
                    printf("%d",stack[t]);
                    t --;
                    checkStack();
                    break;
               case OP_WLN:
//                    wprintw(win,"\n");
                    printf("\n");
                    break;
               case OP_AD:
                    t --;
                    if (checkStack()){ 
	                   stack[t] += stack[t+1];
	                   updateStackUse(t);
                    }
                    break;
               case OP_SB:
                    t --;
                    if (checkStack()){
                       stack[t] -= stack[t+1];
                       updateStackUse(t);
                    }
                    break;
               case OP_ML:
                    t --;
                    if (checkStack()) {
	                   stack[t] *= stack[t+1];
	                   updateStackUse(t);
                    }
                    break;
               case OP_DV: 
                    t --;
                    if (checkStack()) {
	                   if (stack[t+1] == 0)
	                      ps = PS_DIVIDE_BY_ZERO;
                       else{
                           stack[t] /= stack[t+1];
                           updateStackUse(t);
                       }
                    }
                    break;
               case OP_NEG:
                    stack[t] = - stack[t];
                    break;
               case OP_CV: 
                    stack[t+1] = stack[t];
                    t ++;
                    updateStackUse(t);
                    checkStack();
                    updateMaxT();
                    break;
               case OP_EQ:
                    t --;
                    if (stack[t] == stack[t+1]) 
	                   stack[t] = TRUE;
                    else
                        stack[t] = FALSE;
                    checkStack();
                    break;
               case OP_NE:
                    t --;
                    if (stack[t] != stack[t+1]) 
	                   stack[t] = TRUE;
                    else
                        stack[t] = FALSE;
                    checkStack();
                    break;
               case OP_GT:
                    t --;
                    if (stack[t] > stack[t+1]) 
	                   stack[t] = TRUE;
                    else
                        stack[t] = FALSE;
                    checkStack();
                    break;
               case OP_LT:
                    t --;
                    if (stack[t] < stack[t+1]) 
	                   stack[t] = TRUE;
                    else
                        stack[t] = FALSE;
                    checkStack();
                    break;
               case OP_GE:
                    t --;
                    if (stack[t] >= stack[t+1]) 
	                   stack[t] = TRUE;
                    else
                        stack[t] = FALSE;
                    checkStack();
                    break;
               case OP_LE:
                    t --;
                    if (stack[t] <= stack[t+1]) 
	                   stack[t] = TRUE;
                    else
                        stack[t] = FALSE;
                    checkStack();
                    break;
               case OP_BP:
               //Just for debugging
                      debugMode = 1;
                      break;
               default: break;
    } // switch (code[pc].op)

    if (debugMode) {
       int command;
       int level, offset;
       int interactive = 1;
      
       do {
	      interactive = 0;

	      fflush(stdin);
          command = getchar();
	      switch (command){
                 case 'v':
                 case 'V':
                      viewStack();
                      interactive = 1;
                      break;
                 case 'a':
                 case 'A':
//	                  wprintw(win,"\nEnter memory location (level, offset):");
	                  printf("\nEnter memory location (level, offset):");
//	                  wscanw(win,"%d %d", &level, &offset);
	                  scanf("%d %d",&level, &offset);
//	                  wprintw(win,"Absolute address = %d\n", base(level) + offset);
	                  printf("Absolute address = %d\n", base(level) + offset);
	                  interactive = 1;
	                  break;
                 case 'm':
	             case 'M':
//	                  wprintw(win,"\nEnter memory location (level, offset):");
	                  printf("\nEnter memory location (level, offset):");
//	                  wscanw(win,"%d %d", &level, &offset);
	                  scanf("%d %d",&level, &offset);
//	                  wprintw(win,"Value = %d\n", stack[base(level) + offset]);
	                  printf("Value = %d\n", stack[base(level) + offset]);
	                  interactive = 1;
	                  break;
                 case 't':
	             case 'T':
//	                  wprintw(win,"Top (%d) = %d\n", t, stack[t]);
	                  printf("Top (%d) = %d\n", t, stack[t]);
	                  interactive = 1;
	                  break;
                 case 'c':
	             case 'C':
	                  debugMode = 0;
	                  break;
                 case 'h':
	             case 'H':
	                  ps = PS_NORMAL_EXIT;
	                  break;
                 default: break;
           }
      } while (interactive);
    } // if (debugMode)
    pc ++;
  }// (ps == PS_ACTIVE)
//  wprintw(win,"\nPress any key to exit...");
  printf("\nPress any key to exit...");
  //getch();
//  endwin();
  return ps;
}
