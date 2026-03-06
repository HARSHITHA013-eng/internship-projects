#include "head.h"


int addition(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3)
{
   int carry = 0, sum = 0;
    while (tail1 || tail2)
    {
        if(tail1 && tail2)
        sum = tail1->data + tail2->data + carry;
        else if(tail1)
        sum = tail1->data + carry;
        else if(tail2)
        sum = tail2->data + carry;
        if (sum > 9)
        {
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        sum = sum % 10;
        Insert_at_first(head3, sum, tail3);
        if(tail1)
        tail1 = tail1->Prev;
        if(tail2)
        tail2 = tail2->Prev;
    }

    if (carry == 1)
        Insert_at_first(head3, 1, tail3);

    return Success;
}

