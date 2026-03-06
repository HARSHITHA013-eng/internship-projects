#include "head.h"
#include<stdio.h>

void save(main_node *hashtable[],char name[] )
{
    FILE *fp=fopen(name,"w");
    if(fp==NULL)
    {
        printf("Unable to create the file\n");
        printf("\n");
        return ;
    }
    printf("%s file created successfully\n",name);
    //run loop for 26 times to save 
    
    for(int i=0; i<27; i++)
    {
        if(hashtable[i]==NULL)
        {
            continue;
        }
        main_node *temp=hashtable[i];
        while(temp!=NULL)
        {
            fprintf(fp,"#%d ; %s ; %d ;",i,temp->word,temp->file_count);

            //printing the sub node
            sub_node *sub=temp->sub_list;
            while(sub!=NULL)
            {
                fprintf(fp,"%s ; %d ;",sub->filename,sub->word_count);
                sub=sub->link;
            }
            fprintf(fp,"#\n");
            temp=temp->next;

        }
    }
    fclose(fp);

}