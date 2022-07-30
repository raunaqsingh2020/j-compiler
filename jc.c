/*
 * jc.c
 */

#include "token.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]) {
  FILE *jFile, *asmFile;
  token currToken;

  if (argc != 2) {
    exit(0);
  }

  if(strlen(argv[1]) <= 2 || strcmp(argv[1] + strlen(argv[1]) - 2, ".j")) {
    printf("Invalid input.\n");
    exit(0);
  }

  jFile = fopen(argv[1], "r");

  if (jFile == NULL) {
    printf("Could not open .j file.\n");
    exit(EXIT_FAILURE);
  }

  (argv[1])[strlen(argv[1]) - 1] = '\0';
  strcat(argv[1], "asm");
  asmFile = fopen(argv[1], "w");

  if (asmFile == NULL) {
    printf("Could not create .asm file.\n");
    exit(EXIT_FAILURE);
  }

  currToken.type = BAD_TOKEN;
  int cmpNum = 1, ifNum = 1, inFunc = 0;
  Stack the_stack;
  
  Stack_Init(&the_stack);

  while (!read_token(&currToken, jFile)) {

    fprintf(asmFile, ";; %s\n", currToken.str);

    if (currToken.type >= 3 && currToken.type <= 9) {

        fprintf(asmFile, "\tLDR R0, R6, #0\n");
        fprintf(asmFile, "\tLDR R1, R6, #1\n");

        switch (currToken.type) {
            case PLUS :
                fprintf(asmFile, "\tADD R0, R0, R1\n");
                break;
            case MINUS :
                fprintf(asmFile, "\tSUB R0, R0, R1\n");
                break;
            case MUL :
                fprintf(asmFile, "\tMUL R0, R0, R1\n");
                break;
            case DIV :
                fprintf(asmFile, "\tDIV R0, R0, R1\n");
                break;
            case MOD :
                fprintf(asmFile, "\tMOD R0, R0, R1\n");
                break;
            case AND :
                fprintf(asmFile, "\tAND R0, R0, R1\n");
                break;
            case OR :
                fprintf(asmFile, "\tOR R0, R0, R1\n");
                break;
            default :
                break;
        }

        fprintf(asmFile, "\tSTR R0, R6, #1\n");
        fprintf(asmFile, "\tADD R6, R6, #1\n");
        fprintf(asmFile, "\n");

        continue;
    }

    if (currToken.type >= 11 && currToken.type <= 15) {

        fprintf(asmFile, "\tLDR R0, R6, #0\n");
        fprintf(asmFile, "\tLDR R1, R6, #1\n");
        fprintf(asmFile, "\tCMP R0, R1\n");

        switch (currToken.type) {
            case LT :
                fprintf(asmFile, "\tBRzp CMP_FALSE_%d\n", cmpNum);
                break;
            case LE :
                fprintf(asmFile, "\tBRp CMP_FALSE_%d\n", cmpNum);
                break;
            case EQ :
                fprintf(asmFile, "\tBRnp CMP_FALSE_%d\n", cmpNum);
                break;
            case GE :
                fprintf(asmFile, "\tBRn CMP_FALSE_%d\n", cmpNum);
                break;
            case GT :
                fprintf(asmFile, "\tBRnz CMP_FALSE_%d\n", cmpNum);
                break;
            default :
                break;
        }

        fprintf(asmFile, "\tCONST R2, #1\n");
        fprintf(asmFile, "\tBRnzp CMP_DONE_%d\n", cmpNum);

        fprintf(asmFile, "CMP_FALSE_%d\n", cmpNum);
        fprintf(asmFile, "\tCONST R2, #0\n");

        fprintf(asmFile, "CMP_DONE_%d\n", cmpNum);
        fprintf(asmFile, "\tSTR R2, R6, #1\n");
        fprintf(asmFile, "\tADD R6, R6, #1\n");
        fprintf(asmFile, "\n");

        cmpNum++;

        continue;
    }

    switch (currToken.type) {
        case DEFUN :
            if (read_token(&currToken, jFile) || currToken.type != IDENT) {
                printf("ERROR: DEFUN token not followed by IDENT token.\n");
                exit(EXIT_FAILURE);
            }

            if (inFunc) {
                printf("ERROR: Attempting to define function in another function.\n");
                exit(EXIT_FAILURE);
            }

            inFunc = 1;

            fprintf(asmFile, ".CODE\n");
			fprintf(asmFile, ".FALIGN\n");
			fprintf(asmFile, "%s\n", currToken.str);
			fprintf(asmFile, "\tADD R6, R6, #-3\n");
			fprintf(asmFile, "\tSTR R7, R6, #1\n");
			fprintf(asmFile, "\tSTR R5, R6, #0\n");			
			fprintf(asmFile, "\tADD R5, R6, #0\n");
            fprintf(asmFile, "\n");

            break;
        case IDENT :
            fprintf(asmFile, "\tJSR %s\n", currToken.str);
            fprintf(asmFile, "\tADD R6, R6, #-1\n");
            fprintf(asmFile, "\n");
            break;
        case RETURN :
            if (!inFunc) {
                printf("ERROR: RETURN token missing corresponding DEFUN token.\n");
                exit(EXIT_FAILURE);
            }
            
            inFunc = 0;

            fprintf(asmFile, "\tLDR R7, R6, #0\n");
			fprintf(asmFile, "\tSTR R7, R5, #2\n");
			fprintf(asmFile, "\tADD R6, R5, #0\n");
			fprintf(asmFile, "\tLDR R5, R6, #0\n");
			fprintf(asmFile, "\tLDR R7, R6, #1\n");
			fprintf(asmFile, "\tADD R6, R6, #3\n");
			fprintf(asmFile, "\tRET\n");
            fprintf(asmFile, "\n");

            break;
        case NOT :
            fprintf(asmFile, "\tLDR R0, R6, #0\n");
            fprintf(asmFile, "\tNOT R0, R0\n");
            fprintf(asmFile, "\tSTR R0, R6, #0\n");
            // fprintf(asmFile, "\tADD R6, R6, #1\n");
            fprintf(asmFile, "\n");
            break;
        case IF :
            fprintf(asmFile, "\tADD R6, R6, #1\n");
            fprintf(asmFile, "\tLDR R0, R6, #-1\n");
            fprintf(asmFile, "\tBRz ELSE_%d\n", ifNum);
            
            Stack_Push(&the_stack, ifNum);
            ifNum++;

            fprintf(asmFile, "\n");
            break;
        case ELSE :
            fprintf(asmFile, "\tJMP ENDIF_%d\n", Stack_Peek(&the_stack));

            fprintf(asmFile, "ELSE_%d\n", Stack_Peek(&the_stack));

            Stack_Set_Else(&the_stack);

            break;
        case ENDIF :
            if (Stack_Check_Else(&the_stack)) {
                fprintf(asmFile, "ENDIF_%d\n", Stack_Pop(&the_stack));
            } else {
                fprintf(asmFile, "ELSE_%d\n", Stack_Pop(&the_stack));
            }

            break;
        case DROP :
            fprintf(asmFile, "\tADD R6, R6, #1\n");
            fprintf(asmFile, "\n");
            break;
        case DUP :
            fprintf(asmFile, "\tLDR R0, R6, #0\n");
            fprintf(asmFile, "\tSTR R0, R6, #-1\n");
            fprintf(asmFile, "\tADD R6, R6, #-1\n");
            fprintf(asmFile, "\n");
            break;
        case SWAP :
            fprintf(asmFile, "\tLDR R0, R6, #0\n");
            fprintf(asmFile, "\tLDR R1, R6, #1\n");
            fprintf(asmFile, "\tSTR R0, R6, #1\n");
            fprintf(asmFile, "\tSTR R1, R6, #0\n");
            fprintf(asmFile, "\n");
            break;
        case ROT :
            fprintf(asmFile, "\tLDR R0, R6, #0\n");
            fprintf(asmFile, "\tLDR R1, R6, #2\n");
            fprintf(asmFile, "\tSTR R1, R6, #0\n");
            fprintf(asmFile, "\tLDR R1, R6, #1\n");
            fprintf(asmFile, "\tSTR R1, R6, #2\n");
            fprintf(asmFile, "\tSTR R0, R6, #1\n");
            fprintf(asmFile, "\n");
            break;
        case ARG :
            fprintf(asmFile, "\tLDR R0, R5, #%d\n", currToken.arg_no + 2);
	        fprintf(asmFile, "\tSTR R0, R6, #-1\n\n");
            fprintf(asmFile, "\tADD R6, R6, #-1\n");
            fprintf(asmFile, "\n");
            break;
        case LITERAL :
            fprintf(asmFile, "\tCONST R0, #%d\n", (short int) (currToken.literal_value & 0x00FF));
	        fprintf(asmFile, "\tHICONST R0, #%d\n", (short int) ((currToken.literal_value >> 8) & 0x00FF));
	        fprintf(asmFile, "\tADD R6, R6, #-1\n");
	        fprintf(asmFile, "\tSTR R0, R6, #0\n");
            fprintf(asmFile, "\n");
            break;
        case BAD_TOKEN :
            exit(EXIT_FAILURE);
            break;
        default :
            break;
    }
  }

  fclose(jFile);
  fclose(asmFile);

  return 0;
}