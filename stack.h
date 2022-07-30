/*
 * stack.h
 */

#include <stdbool.h>

typedef struct stack_node_st {
  int val;
  int hasElse;
  struct stack_node_st *next;
} Stack_Node;

typedef struct stack_st {
  Stack_Node *top, *bottom;
} Stack;

void Stack_Init(Stack *s);

void Stack_Push(Stack *s, int val);

int Stack_Pop(Stack *s);

bool Stack_Is_Empty(Stack *s);

int Stack_Peek(Stack *s);

void Stack_Clear(Stack *s);

void Stack_Print(Stack *s);

int Stack_Check_Else(Stack *s);

void Stack_Set_Else(Stack *s);