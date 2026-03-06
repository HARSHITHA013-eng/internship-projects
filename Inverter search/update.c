#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "head.h"

int update(main_node *hashtable[], char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Unable to open file\n");
        return Failure;
    }

    char ch;
    while (fscanf(fp, " %c", &ch) != EOF)
    {
        //check for '#'
        if (ch != '#')
        {
            printf("Invalid file format\n");
            fclose(fp);
            return Failure;
        }

        int index;
        char word[100];
        char fcount_str[10];
        int file_count;

        // read index, word, filecount
        fscanf(fp, "%d ; %[^;] ; %[^;] ;", &index, word, fcount_str);
        file_count = atoi(fcount_str);

        // create main node
        main_node *new_main = malloc(sizeof(main_node));
        if (!new_main)
            return Failure;

        strcpy(new_main->word, word);
        new_main->file_count = file_count;
        new_main->next = NULL;
        new_main->sub_list = NULL;

        // insert main node into hash table
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_main;
        }
        else
        {
            main_node *temp = hashtable[index];
            while (temp->next)
                temp = temp->next;
            temp->next = new_main;
        }

        // loop file_count times (sub nodes)
        sub_node *prev_sub = NULL;
        for (int i = 0; i < file_count; i++)
        {
            char fname[100];
            char wcount_str[10];
            int wcount;

            fscanf(fp, "%[^;] ; %[^;] ;", fname, wcount_str);
            wcount = atoi(wcount_str);

            sub_node *new_sub = malloc(sizeof(sub_node));
            strcpy(new_sub->filename, fname);
            new_sub->word_count = wcount;
            new_sub->link = NULL;

            if (prev_sub == NULL)
                new_main->sub_list = new_sub;
            else
                prev_sub->link = new_sub;

            prev_sub = new_sub;
        }

        // read ending '#'
        fscanf(fp, " #");
    }

    fclose(fp);
    printf("Update operation completed successfully\n");
    return Success;
}
