/*
Name: Harshitha G H
Date: 10-02-2025
Description: A calculator that determines result based on user-defined parameters .
Batch: 25021A
*/

#include "head.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Invalid arguments\n");
        printf("-------------------------------------------\n");
        printf("./a.out Num1 operator(+ - * / ) Num2\n");
        printf("-------------------------------------------\n");
        return Failure;
    }
    char operator = argv[2][0];
    char *operand1 = argv[1];
    char *operand2 = argv[3];

    LL *head1 = NULL;
    LL *tail1 = NULL;

    LL *head2 = NULL;
    LL *tail2 = NULL;

    LL *head3 = NULL;
    LL *tail3 = NULL;

    int i = strlen(operand1) - 1, j = strlen(operand2) - 1;
    while (i >= 0)
    {

        if (i == 0 && (operand1[i] == '-' || operand1[i] == '+'))
            break;

        if (!isdigit(operand1[i]))
        {
            printf("Invalid 1\n");
            break;
        }
        int digit = operand1[i--] - '0';

        if (Insert_at_first(&head1, digit, &tail1) == Failure)
        {
            printf("Storing Operand 1 in list is failed\n");
            return Failure;
        }
    }

    while (j >= 0)
    {

        if (j == 0 && (operand2[j] == '-' || operand2[j] == '+'))
            break;
        if (!isdigit(operand2[j]))
        {
            printf("Invalid 2\n");
            break;
        }
        int digit = operand2[j--] - '0';
        if (Insert_at_first(&head2, digit, &tail2) == Failure)
        {
            printf("Storing Operand 2 in list is failed\n");
            return Failure;
        }
    }

    if (head1 == NULL)
    {
        printf("Enter value of list1\n");
    }
    if (head2 == NULL)
    {

        printf("Enter value of list2\n");
    }
    if (head1 == NULL || head2 == NULL)
    {
        exit(1);
    }

    remove_zero(&head1, &tail1);
    remove_zero(&head2, &tail2);

    printf("Elements in First list is :");
    print_List(head1);
    // printf("Operator is %c\n", operator);
    printf("Elements in Second list is :");
    print_List(head2);
    char op = operator;
    int sign = 0, swap = 0;

    if (op == '+' || op == '-')
    {
        char *abs1 = (operand1[0] == '+' || operand1[0] == '-') ? operand1 + 1 : operand1;
        char *abs2 = (operand2[0] == '+' || operand2[0] == '-') ? operand2 + 1 : operand2;
        int cmp = head_compare(head1, head2);

        if (cmp == -1 && op == '-') // first number smaller for subtraction
            swap = 1;

        if (operand1[0] == '-' && operand2[0] == '-')
        {
            op = operator;
            if (operator == '+')
                sign = 1;
            else if (operator == '-')
                sign = (cmp == 1) ? 1 : 0;
        }
        else if ((operand1[0] == '+' || isdigit(operand1[0])) &&
                 (operand2[0] == '+' || isdigit(operand2[0])))
        {
            op = operator;
            if (operator == '+')
                sign = 0;
            else if (operator == '-')
                sign = (cmp == -1) ? 1 : 0;
        }
        else if (operand1[0] == '-' && (operand2[0] == '+' || isdigit(operand2[0])))
        {
            if (operator == '+')
            {
                op = '-';
                sign = (cmp == -1) ? 0 : 1;
            }
            else if (operator == '-')
            {
                op = '+';
                sign = 1;
            }
        }
        else if ((operand1[0] == '+' || isdigit(operand1[0])) && operand2[0] == '-')
        {
            if (operator == '+')
            {
                op = '-';
                sign = (cmp == -1) ? 1 : 0;
            }
            else if (operator == '-')
            {
                op = '+';
                sign = 0;
            }
        }
    }
    if (op == '/' || op == 'x' || op == 'X' || op == '*')
    {
        if ((operand1[0] == '+' || isdigit(operand1[0])) && operand2[0] == '-')
        {
            sign = 1;
        }
        else if (operand1[0] == '-' && (operand2[0] == '+' || isdigit(operand2[0])))
        {
            sign = 1;
        }
    }

    if (op == '/' || op == 'x' || op == 'X' || op == '*')
    {
        if ((operand1[0] == '+' || isdigit(operand1[0])) && operand2[0] == '-')
        {
            sign = 1;
        }
        else if (operand1[0] == '-' && (operand2[0] == '+' || isdigit(operand2[0])))
        {
            sign = 1;
        }
    }
    // printf("My new operator is %c\n", op);
    int res = head_compare(head1, head2);
    // printf("\nRes --> %d\n", res);
    printf("::::ARBITARY PRECISION CALCULATOR::::\n");
    printf("--------------------------------------\n");
    printf("Num1    : ");
    print_List(head1);
    printf("OP      : %c\n", op);
    printf("Num2    : ");
    print_List(head2);
    printf("--------------------------------------\n");

 
    int cmp = head_compare(head1, head2);
   

    if (swap == 1)
    {
        
        LL *temp1, *temp2;
        temp1 = head1;
        head1 = head2;
        head2 = temp1;

        temp2 = tail1;
        tail1 = tail2;
        tail2 = temp2;
    }

    printf("Result  : ");
    if (sign == 1)
    {
        printf("-");
    }

    switch (op)
    {
    case '+':
        addition(head1, tail1, head2, tail2, &head3, &tail3);
        remove_zero(&head3, &tail3);
        print_List(head3); // print result
        break;
    case '-':
        // printf("You have Entered substraction : ");

        Substraction(head1, tail1, head2, tail2, &head3, &tail3);
        remove_zero(&head3, &tail3);
        print_List(head3); // print result

        break;
    case 'x':
    case 'X':
    case '^':
        multiplication(head1, tail1, head2, tail2, &head3, &tail3);
        remove_zero(&head3, &tail3);
        print_List(head3);
        break;
    case '/':
        LL *temp_head = NULL;
        LL *temp_tail = NULL;
        temp_head = division(head1, tail1, head2, tail2, &head3, &tail3);
        if (temp_head != NULL)
        {
            remove_zero(&head3, &tail3);
            print_List(temp_head); // print result
        }
        free_list(&temp_head);

        break;
    default:
        printf("Please Enter correct operator !!\n");
    }
    printf("--------------------------------------\n");
}
