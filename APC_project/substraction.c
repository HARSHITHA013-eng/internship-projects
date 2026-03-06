#include "head.h"

int Substraction(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3)
{
    int borrow = 0, diff = 0;
    while (tail1 || tail2)
    {

        int val1 = 0, val2 = 0;
        if (tail1)
        {
            val1 = tail1->data;
        }
        if (tail2)
        {
            val2 = tail2->data;
        }

        if (val1 < val2)
        {
            borrow = 1;
        }
        else
            borrow = 0;

        diff = val1 - val2;
        if (borrow == 1)
        {
            diff = diff + 10;
            if (tail1->Prev != NULL)
                tail1->Prev->data = tail1->Prev->data - 1;
        }
        Insert_at_first(head3, diff, tail3);
        if (tail1)
            tail1 = tail1->Prev;
        if (tail2)
            tail2 = tail2->Prev;
    }

    while (*head3 && (*head3)->data == 0 && *head3 != *tail3)
    {
        LL *del = *head3;
        *head3 = (*head3)->Next;
        free(del);
        (*head3)->Prev = NULL;
    }
    return Success;
}