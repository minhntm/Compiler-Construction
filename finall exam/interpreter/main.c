/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm_duongnn.h"
#define DEFAULT_STACK_SIZE 2048
#define DEFAULT_CODE_SIZE 1024

extern int debugMode;
extern int stackSize;
extern int codeSize;
extern int fullMode;

int dumpCode;


void printUsage(void) {
  printf("Usage: kplrun input [-s=stack_size] [-c=code_size] [-debug] [-dump]\n");
  printf("   input: input kpl program\n");
  printf("   -s=stack_size: set the stack size\n");
  printf("   -c=code_size: set the code size\n");
  printf("   -debug: enable code dump\n");
  printf("   -full: view every code and stack\n");
}

int analyseParam(char* param) {
  if (strncmp(param, "-s=", 3) == 0) {
    stackSize = atoi(param+3);
    return 1;
  }
  if (strncmp(param, "-c=", 3) == 0) {
    codeSize = atoi(param+3);
    return 1;
  }
  if (strcmp(param, "-debug") == 0) {
    debugMode = 1;
    return 1;
  }
  if (strcmp(param, "-dump") == 0) {
    dumpCode = 1;
    return 1;
  }
  if (strcmp(param, "-full") == 0){
     fullMode = 1;
     return 1;
  }
  return 0;
}

/******************************************************************/

int main(int argc, char *argv[]) {
  int i;
  FILE* f;

  debugMode = 0;
  stackSize = DEFAULT_STACK_SIZE;
  codeSize = DEFAULT_CODE_SIZE;
  dumpCode = 0;

  if (argc <= 1) {
    printf("kplrun: no input file.\n");
    printUsage();
    return -1;
  }

  for ( i = 2; i < argc; i++) 
    if (analyseParam(argv[i]) == 0) {
      printUsage();
      return -1;
    }

  f = fopen(argv[1],"r");
	    
  if (f == NULL) {
    printf("kplrun: Can\'t read input file!\n");
    return -1;
  }

  initVM();
  if (loadExecutable(f) == 0) {
    printf("kplrun: Wrong executable format!\n");
    fclose(f);
    cleanVM();
    return -1;
  }
  fclose(f);

  if (dumpCode) {
    printCodeBuffer();
    return 0;
  }

  switch (run()) {
  case PS_DIVIDE_BY_ZERO:
    printf("Runtime error: Divide by zero!\n");
    break;
  case PS_STACK_OVERFLOW:
    printf("Runtime error: Stack overflow!\n");
    break;
  case PS_IO_ERROR:
    printf("Runtime error: IO error!\n");
    break;
  default:
    break;
  }
  cleanVM();
  return 0;
}
