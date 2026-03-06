#include "head.h"

int multiplication(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3)
{
    *head3 = NULL, *tail3 = NULL;
    LL *temp2 = tail2;
    int count = 0; // to keep track of adding 0 to last

    while (temp2)
    {
        LL *temp_head = NULL, *temp_tail = NULL;
        LL *temp1 = tail1;
        int carry = 0;

        while (temp1)
        {
            int product = carry;
            if (temp1)
            {
                product = product + temp1->data * temp2->data;
                temp1 = temp1->Prev;
            }
            carry = product / 10;
            product = product % 10;
            Insert_at_first(&temp_head,product,&temp_tail);

            
        }
          if (carry > 0)
            Insert_at_first(&temp_head, carry, &temp_tail);

        // append zeros according to digit position
        for (int i = 0; i < count; i++)
             Insert_at_last(&temp_head, 0, &temp_tail);

            if (*head3 == NULL)
        {
            copy_list(temp_head, temp_tail, head3, tail3);
        }
        else
        {
            LL *sumH = NULL, *sumT = NULL;// used to store value temp
            addition(*head3, *tail3, temp_head, temp_tail, &sumH, &sumT);
            free_list(head3);
            *head3 = sumH;
            *tail3 = sumT;
        }

        free_list(&temp_head); // free partial product
        temp2 = temp2->Prev;
        count++;
    }

    return 1;
}