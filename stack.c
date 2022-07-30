/*
 * stack.c
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for malloc, free, and EXIT_FAILURE

#include "stack.h"

void Stack_Init(Stack *s) {
  s->top = NULL;
  s->bottom = NULL;
}

void Stack_Push(Stack *s, int val) {
  Stack_Node *node = malloc(sizeof(Stack_Node));

  if (node == NULL) {
    printf("Couldn't allocate space for a new Stack Node\n");
    exit(EXIT_FAILURE);
  }

  node->val = val;
  node->hasElse = 0;

  if (s->top) {
    node->next = s->top;
    s->top = node;
  } else {
    node->next = NULL;
    s->top = node;
    s->bottom = node;
  }
}

int Stack_Pop(Stack *s) {
  if (!s->top) {
    return -1; //popping from empty stack
  }
  Stack_Node* next_top = s->top->next;

  if (s->top == s->bottom) {
    s->bottom = NULL;
    next_top = NULL;
  }

  int val = s->top->val;
  free(s->top);
  s->top = next_top;
  return val;
}

int Stack_Check_Else(Stack *s) {
  if (!s->top) {
    return -1; //empty stack
  }
  return s->top->hasElse;
}

void Stack_Set_Else(Stack *s) {
  if (s->top) {
    s->top->hasElse = 1; //empty stack
  }
}

int Stack_Peek(Stack *s) {
  if (!s->top) {
    return -1; //peeking from empty stack
  }
  return s->top->val;
}

bool Stack_Is_Empty(Stack *s) {
  if (!s->top) {
    return true;
  }
  return false;
}

void Stack_Clear(Stack *s) {
  while(!Stack_Is_Empty(s)) {
    Stack_Pop(s);
  }
}

void Stack_Print(Stack *s) {
  Stack_Node *curr = s->top;
  printf("\nSTACK CONTAINS (TOP TO BOTTOM):\n");
  while(curr) {
    printf("%d\n", curr->val);
    curr = curr->next;
  }
}
