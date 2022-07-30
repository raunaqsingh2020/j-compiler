/*
 * token.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"

int read_token (token *theToken, FILE *theFile) {
  char tokenString[MAX_TOKEN_LENGTH];
  int literal, argN, i;
  unsigned int hex;
  int empty = 1;
  
  if (feof(theFile)) {
    return 1;
  }

  fscanf(theFile, "%s", &tokenString[0]);

  if (tokenString[0] == ';') {
    char c;
    do {
      c = fgetc(theFile);
      if (feof(theFile)) {
        return 1;
      }
    } while (c != '\n');
    return read_token(theToken, theFile);
  }

  for (i = 0; i < strlen(tokenString); i++) {
    if (!isspace(tokenString[i])) {
      empty = 0;
    }
  }

  if (empty) {
    return 1;
  }

  if (!strcmp(tokenString, "+")) {
    theToken->type = PLUS;
  } else if (!strcmp(tokenString, "-")) {
    theToken->type = MINUS;
  } else if (!strcmp(tokenString, "*")) {
    theToken->type = MUL;
  } else if (!strcmp(tokenString, "/")) {
    theToken->type = DIV;
  } else if (!strcmp(tokenString, "%")) {
    theToken->type = MOD;
  } else if (!strcmp(tokenString, "lt")) {
    theToken->type = LT;
  } else if (!strcmp(tokenString, "le")) {
    theToken->type = LE;
  } else if (!strcmp(tokenString, "eq")) {
    theToken->type = EQ;
  } else if (!strcmp(tokenString, "ge")) {
    theToken->type = GE;
  } else if (!strcmp(tokenString, "gt")) {
    theToken->type = GT;
  } else if (!strcmp(tokenString, "and")) {
    theToken->type = AND;
  } else if (!strcmp(tokenString, "or")) {
    theToken->type = OR;
  } else if (!strcmp(tokenString, "not")) {
    theToken->type = NOT;
  } else if (!strcmp(tokenString, "drop")) {
    theToken->type = DROP;
  } else if (!strcmp(tokenString, "dup")) {
    theToken->type = DUP;
  } else if (!strcmp(tokenString, "swap")) {
    theToken->type = SWAP;
  } else if (!strcmp(tokenString, "rot")) {
    theToken->type = ROT;
  } else if (!strcmp(tokenString, "if")) {
    theToken->type = IF;
  } else if (!strcmp(tokenString, "else")) {
    theToken->type = ELSE;
  } else if (!strcmp(tokenString, "endif")) {
    theToken->type = ENDIF;
  } else if (!strcmp(tokenString, "defun")) {
    theToken->type = DEFUN;
  } else if (!strcmp(tokenString, "return")) {
    theToken->type = RETURN;
  } else if (sscanf(tokenString, "arg%d", &argN) && (argN >= 1 && argN <= 20)) {
    theToken->type = ARG;
    theToken->arg_no = argN;
  } else if (sscanf(tokenString, "0x%X", &hex)) {
    literal = (signed short int) hex;
    theToken->type = LITERAL;
    theToken->literal_value = literal;
  } else if (sscanf(tokenString, "%d", &literal)) {
    theToken->type = LITERAL;
    theToken->literal_value = literal;
  } else if (is_ident(tokenString)) {
    theToken->type = IDENT;
  } else {
    theToken->type = BAD_TOKEN;
  }

  strcpy(theToken->str, tokenString);

  return 0;
}

void print_token (token *theToken) {
  char* typeNames[] = { "DEFUN", "IDENT", "RETURN", 
	       "PLUS", "MINUS", "MUL", "DIV", "MOD", 
	       "AND", "OR", "NOT", 
	       "LT", "LE", "EQ", "GE", "GT",
	       "IF", "ELSE", "ENDIF", 
	       "DROP", "DUP", "SWAP", "ROT",
	       "ARG",
	       "LITERAL",
	       "BAD_TOKEN" };
  
  printf("type: %s, literal: %d, argN: %d, string: %s\n", typeNames[theToken->type], theToken->literal_value, theToken->arg_no, theToken->str);
}

int is_ident (char string[]) {
  if (strlen(string) < 1) {
    return 0;
  }

  if (!isalpha(string[0])) {
    return 0;
  }

  for (int i = 0; i < strlen(string); i++) {
    if (!isalpha(string[i]) && !isdigit(string[i]) && string[i] != '_') {
      return 0;
    }
  }

  return 1;
}