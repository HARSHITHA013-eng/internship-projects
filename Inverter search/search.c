#include<stdio.h>
#include "head.h"
#include<string.h>
int search(main_node *hashtable[],char ser_word[])
{
    int index=hash_index(ser_word);
    main_node *temp=hashtable[index];
    //search main node list
    while(temp!=NULL)
    {
        if(strcmp(temp->word,ser_word)==0)
        {
            printf("\n");
            printf("********************************************\n");
             printf("\nWord : %s\n",temp->word);
             printf("Appears in %d files\n",temp->file_count);

             sub_node *new_sub=temp->sub_list;

             //sub node data to print
             printf("                  :: File details ::             \n");
             printf("-------------------------------------------------\n");
             while(new_sub!=NULL)
             {
                printf("         File : %s -> Count: %d\n",new_sub->filename,new_sub->word_count);
                new_sub=new_sub->link;
             }
             printf("********************************************\n");
             printf("\n");
             return Success;

        }
        temp=temp->next;
    }
    
    printf("%s not found in the index table\n",ser_word);
    printf("\n");
    return Failure;
}

void display(main_node *hashtable[])
{
    printf("========================== Hash Table Contents ==========================\n");
    for(int i=0; i<27; i++)
    {
        if(hashtable[i]==NULL)
        {
            continue;
        }
        main_node *temp=hashtable[i];
        //print main_node
        while(temp!=NULL)
        {
            printf("---------------------------------------\n");
            printf("word       : %s\n",temp->word);
            printf("File count : %d\n",temp->file_count);
            printf("---------------------------------------\n");

            printf("FILENAME          |    WORD COUNT \n");
            //print sub-node
            sub_node *sub=temp->sub_list;
            while(sub!=NULL)
            {
                printf("   %s       ->    %d\n",sub->filename,sub->word_count);
                sub=sub->link;
            }
            temp=temp->next;
        }
         printf("---------------------------------------\n");
         printf("\n");
    }
}