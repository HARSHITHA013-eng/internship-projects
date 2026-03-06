/*Name: Harshitha G H
Date: 8-02-2026
Description: Minishell is a simplified implementation of a Unix shell written in C, designed to understand and execute user commands in an interactive environment. The project emphasizes low-level system programming concepts such as process management, file descriptors, signals, and environment variables.
*/
#include <stdio.h>
#include "header.h"


char input_string[100];
int pid=-1;
int status;

job_t *job_head;


job_t *job_head=NULL;
//declare head globlly for s
int main()
{
        printf("=== MINISHELL STARTING ===\n");
        system("clear");
       // char prompt[25]="minishell$:";
      //  char input_string[25];
      

        scan_input(prompt,input_string);
        return 0;
}
