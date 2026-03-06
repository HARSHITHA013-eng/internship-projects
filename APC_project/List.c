#include "head.h"

int Insert_at_first(LL **head, int data, LL **tail)
{
    LL *new = malloc(sizeof(LL));
    if (new == NULL)
    {
        printf("Element not created\n");
        return Failure;
    }
    new->data = data;
    new->Next = NULL;
    new->Prev = NULL;

    if (*head == NULL)
    {
        *head = new;
        *tail = new;
        return Success;
    }

    (*head)->Prev = new;
    new->Next = *head;
    *head = new;
    return Success;
}

void print_List(LL *head)
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }
    while (head != NULL)
    {
        printf("%d", head->data);
        head = head->Next;
    }
    printf("\n");
}

int Compare(const char *operand1, const char *operand2)
{
    size_t len1 = strlen(operand1);
    size_t len2 = strlen(operand2);

    // Compare lengths first
    if (len1 < len2)
        return 1; // operand1 < operand2
    if (len1 > len2)
        return -1; // operand1 > operand2

    // Lengths equal — compare lexicographically
    for (size_t i = 0; i < len1; i++)
    {
        if (operand1[i] < operand2[i])
            return 1;
        if (operand1[i] > operand2[i])
            return -1;
    }

    return 0; // equal
}

int head_compare(LL *head1, LL *head2) // checks head 1 is greater or not
{
    // count digits
    int len1 = 0, len2 = 0;
    LL *t1 = head1, *t2 = head2;

    while (t1)
    {
        len1++;
        t1 = t1->Next;
    }
    while (t2)
    {
        len2++;
        t2 = t2->Next;
    }

    if (len1 > len2)
        return 1;
    if (len1 < len2)
        return -1;

    // same length → compare digit-by-digit
    while (head1 && head2)
    {
        if (head1->data > head2->data)
            return 1;
        if (head1->data < head2->data)
            return -1;

        head1 = head1->Next;
        head2 = head2->Next;
    }

    return 0; // equal
}

void free_list(LL **head)
{
    LL *temp;

    while (*head != NULL)
    {
        temp = *head;          // store current node
        *head = (*head)->Next; // move head to next
        free(temp);            // free current node
    }

    *head = NULL; // make sure head points to NULL after freeing
}

void copy_list(LL *sourceHead, LL *sourceTail, LL **destHead, LL **destTail)
{
    *destHead = *destTail = NULL; // initialize destination

    LL *temp = sourceTail; // start from tail because we use Insert_at_first
    while (temp)
    {
        Insert_at_first(destHead, temp->data, destTail);
        temp = temp->Prev;
    }
}

// Inserts a new node with `data` at the end of the list
int Insert_at_last(LL **head, int data, LL **tail)
{
    LL *newNode = (LL *)malloc(sizeof(LL));
    if (!newNode)
        return Failure; // memory allocation failed

    newNode->data = data;
    newNode->Next = NULL;
    newNode->Prev = NULL;

    if (*head == NULL) // empty list
    {
        *head = *tail = newNode;
    }
    else
    {
        newNode->Prev = *tail;
        (*tail)->Next = newNode;
        *tail = newNode;
    }

    return Success;
}

// int remove_zero(LL **head,LL **tail)
// {
//     while (*head && (*head)->data == 0 && *head != *tail)
//     {
//         LL *del = *head;
//         *head = (*head)->Next;
//         free(del);
//         (*head)->Prev = NULL;
//     }
//     return Success;
// }

int remove_zero(LL **head, LL **tail)
{
    while (*head && (*head)->data == 0 && *head != *tail)
    {
        LL *del = *head;
        *head = (*head)->Next;
        free(del);
        (*head)->Prev = NULL;
    }

    // If number becomes empty (e.g., "0000"), make it a single zero node
    if (*head == NULL)
    {
        LL *newnode = malloc(sizeof(LL));
        newnode->data = 0;
        newnode->Prev = newnode->Next = NULL;
        *head = *tail = newnode;
        return Success;
    }

    // Fix tail (if all nodes but one are removed, tail might be incorrect)
    LL *temp = *head;
    while (temp->Next != NULL)
        temp = temp->Next;

    *tail = temp;

    return Success;
}
