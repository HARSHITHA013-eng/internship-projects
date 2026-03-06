#include "head.h"

/// as i am not chaning the values of first and seccond head and tail so i am using *(single) pointer
LL *division(LL *head1, LL *tail1, LL *head2, LL *tail2, LL **head3, LL **tail3)
{

    int count = 0;
    LL *one_head = NULL, *res_head = NULL;
    LL *one_tail = NULL, *res_tail = NULL;
    Insert_at_first(&one_head, 1, &one_tail);
    Insert_at_first(head3, 0, tail3);
    Insert_at_first(&res_head, 0, &res_tail);

    if (head_compare(head2, *head3) == 0)
    {
        printf("Can't be divided by 0\n");
        return 0;                                                       
    }

    if (head_compare(head1, head2) == -1)
    {
        // printf("from 0\n");
        return res_head;
    }
   
    while (head_compare(head1, *head3) == 1) 

    {
        LL *head4 = NULL, *tail4 = NULL, *temp_head = NULL, *temp_tail = NULL;
        addition(head2, tail2, *head3, *tail3, &head4, &tail4);
  

        *head3 = head4;
        *tail3 = tail4;

        addition(one_head, one_tail, res_head, res_tail, &temp_head, &temp_tail);
        res_head = temp_head;
        res_tail = temp_tail;
      
    }

  
    return res_head;
}

