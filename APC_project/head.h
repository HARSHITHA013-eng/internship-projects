#ifndef HEAD_H
#define HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct List
{
    /* data */
    int data;
    struct List *Next;
    struct List *Prev;

} LL;

enum
{
    Success,
    Failure,
    List_Empty,
    List_Full
};

int Insert_at_first(LL **head, int data, LL **tail);
void print_List(LL *head);
int addition(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3);
int Substraction(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3);

int Compare(const char *operand1, const char *operand2);
LL *division(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3);

int head_compare(LL *head1, LL *head2); //(num1>num2) if yes then return 1 (flase returns -1) equals returns 0
void free_list(LL **head);

int multiplication(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3);
void copy_list(LL *sourceHead, LL *sourceTail, LL **destHead, LL **destTail);
int Insert_at_last(LL **head, int data, LL **tail);
int remove_zero(LL **head,LL **tail);
#endif // HEAD_H