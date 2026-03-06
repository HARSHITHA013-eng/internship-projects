#include<stdio.h>
#include "head.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int create( file_list **f_head, main_node *hashtable[], int argc, char *argv[])
{
    for(int i=1; i<argc; i++)
    {
        FILE *fp=fopen(argv[i],"r");
        if(!fp)
        {
            printf("Cannot open %s\n",argv[i]);
            return Failure;
        }
        char word[100];
        while(fscanf(fp,"%s",word)!=EOF)
        {
            for(int j=0; word[j]; j++)
            {
                word[j] = tolower(word[j]);
            }
            int index=hash_index(word);
            insert_word(hashtable,index,word,argv[i]);
        }
        fclose(fp);
    }
   

    printf("Create operation done successfully\n");
    return Success;
} 
int hash_index(char *word)
{
   char ch=tolower(word[0]);
   if(ch >='a' && ch <='z')
   {
    return ch-'a';
   }
   return 26;

}
int  insert_word(main_node *hashtable[],int index,char word[],char filename[])
{
    main_node *prev=NULL;
    main_node *temp=hashtable[index];
    //if index is empty
    if(temp==NULL) //create new node  
    {
        main_node  *new_main=malloc(sizeof(main_node));
        if(new_main==NULL)
        {
            printf("Unable to create main node\n");
            return Failure;
        }
       //create main node
       strcpy(new_main->word,word);
       new_main->file_count=1;
       new_main->sub_list=NULL;
       new_main->next=NULL;

       //create sub node
       sub_node *new_sub=malloc(sizeof(sub_node));
       if(new_sub==NULL)
       {
         printf("Unable to create sub node\n");
         return Failure;
       }
       strcpy(new_sub->filename,filename);
       new_sub->word_count=1;
       new_sub->link=NULL;

       //update the main node with sub node link
       new_main->sub_list=new_sub;

       //update the new main address to hashtable
       hashtable[index]=new_main;
       return Success;

    }
    //if hash table is not null
    
    while(temp!=NULL)
    {
        //check if word is present or not
        if(strcmp(temp->word,word)==0)
        {
            sub_node *sub=temp->sub_list;
            sub_node *sub_prev=NULL;
              //if found  traverse through the sub node
              while(sub!=NULL)
              {
                 //if same filename update the word count 
                 if(strcmp(sub->filename,filename)==0)
                 {
                    sub->word_count++;
                    return Success;
                 }
                 sub_prev=sub;
                 sub=sub->link;
              }
              

              //filename not found->create new sub node
             sub_node *new_sub=malloc(sizeof(sub_node));
             if(new_sub==NULL)
             {
               printf("Unable to create new sub node\n");
               return Failure;
              }
             //update the new sub node
              strcpy(new_sub->filename,filename);
              new_sub->word_count=1;
              new_sub->link=NULL;
              
              //update new sub node address to prev sub node 
               if(sub_prev==NULL)
               {
                 temp->sub_list=new_sub;
               }
               else
               {
                  sub_prev->link = new_sub;
               }
              //update the file count
              temp->file_count++;
              return Success;
        }
        prev=temp;
         temp=temp->next;
       
    }
    //word not found create new main and sub node

    main_node *new_main=malloc(sizeof(main_node));
    if(new_main==NULL)
    {
        printf("Unable to create new main node for new word\n");
        return Failure;
    }
    //update the main node
    strcpy(new_main->word,word);
    new_main->file_count=1;
    new_main->next=NULL;
    new_main->sub_list=NULL;

    //create the sub node
    sub_node *new_sub=malloc(sizeof(sub_node));
    //update the sub node
    strcpy(new_sub->filename,filename);
    new_sub->word_count=1;
    new_sub->link=NULL;

    //update the main node sub link
    new_main->sub_list=new_sub;
     
    //insert new main node to prev main node link
    if(prev==NULL)
    {
        hashtable[index]=new_main;
    }
    else
    {
        prev->next=new_main;
    }
    return Success;

      

}