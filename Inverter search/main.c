/*Name:Harshitha G H
Date: 15-12-2025
Discription: This project implements an inverted search engine using a hash table to index words from multiple text files, enabling efficient search, display, save, and update of word occurrences across files.
Batch: 25021A
 */

#include<stdio.h>
#include<stdlib.h>
#include "head.h"
#include<string.h>

int main(int argc, char *argv[])
{
    if(argc <2)
    {
        printf("Invalid argument\n");
        printf("-------------------------------\n");
        printf("Usage : ./a.out filename.txt\n");
        printf("-------------------------------\n");
        return -1;
    }
    for(int i=1; i<argc; i++)
    {
        if(strstr(argv[i],".txt") ==NULL)
        {
           printf(".txt extension missing for %s\n",argv[i]);
           printf("-------------------------------\n");    
           printf("Usage : ./a.out filename.txt\n");
           printf("-------------------------------\n");
           return -1;
        }
        for(int j=i+1; j<argc; j++)
        {
          if( (strcmp (argv[i],argv[j] ) ) ==0 ) 
          {   
            printf("|-------------------------------|\n");   
            printf("|  Duplicates are not allowed ! |\n");
            printf("|-------------------------------|\n");   
            return Failure;
         }
        }
    }
    
    file_list *f_head=NULL;
    main_node *hashtable[27] ={NULL};

    while(1)
    {
        printf("|---------------------|\n");
        printf("|1. Create            |\n");
        printf("|2. Search            |\n");
        printf("|3. Display           |\n");
        printf("|4. Update            |\n");
        printf("|5. Save              |\n");
        printf("|6. Exit              |\n");
        printf("|---------------------|\n");
        int option;
        printf("Select the operation to perform : ");
        scanf("%d",&option);
        switch(option)
        {
            //create
            case 1:
              create(&f_head,hashtable,argc,argv);
            break;

            //search
            case 2:
             char ser_word[100];
             printf("Enter the word to search : ");
             scanf(" %[^\n]",ser_word);
             search(hashtable,ser_word);
            break;

            //display
            case 3:
             display(hashtable);
            break;

            //update
            case 4:
             char fname[100];
              printf("Enter backup file name: ");
              scanf("%s", fname);
              update(hashtable, fname);
            break;

            //save
            case 5:
            char name[100];
            printf("Enter the file name:");
            scanf("%s",name);
            if(strstr(name,".txt")!=NULL)
            {
              save(hashtable,name);
            }
            else
            {
                printf(".txt extension missing for %s\n",name);
                printf("-------------------------------\n");    
                return -1;
            }
            break;

            //exit
            case 6:
            exit(0);

        }
    }
}