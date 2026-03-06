#include "header.h"
#include<string.h>

char prompt[25]="minishell$:";

void my_signal_handler(int sig_num)
{
    if(sig_num == SIGCHLD)
    {
        printf("Signal called\n");
       waitpid(-1, &status, WNOHANG);
    }
    else if(sig_num == SIGINT || sig_num == SIGTSTP)
    {
        if(pid==0)
        {
           write(1,"\n",1);
           printf("%s",prompt);
           fflush(stdout);
        }
    }
   
    return;
   
}


void scan_input(char *prompt, char *input_string)
{
     extract_external_commands(external_commands);
    
     signal(SIGINT,my_signal_handler);
     signal(SIGTSTP,my_signal_handler);
   
     //int status;

        while(1)
        {
                printf("%s",prompt);
                scanf("%[^\n]",input_string);
                    getchar();
        
                //to change the prompt

        if(strncmp(input_string,"PS1=",4)==0)
        {
            if(input_string[4]==' ')
            {
                printf("Invalid command\n");
                printf("Usage: PS1=command\n");
                exit(0);
            }
            else
            {
                strcpy(prompt,input_string+4);

            }
        }

        else
        {
           char *cmd=get_command(input_string);
             //printf("Command copied : %s\n",cmd);
             //check for internal and external
             int type=check_command_type(cmd);
             
             if (type == 1)
             {
               //printf("Builtin command\n");
                 execute_internal_commands(input_string);   
             }
             else if (type == 2)
             {
                //printf("External command\n");
                pid = fork();  
                if( pid >0)
               {
                //parent
                 waitpid(pid,&status,WUNTRACED);
                 //check if any processs stopped by using ctrl+z add that command and pid to linked list
                 //to check process is stopped use WIFSTOPPED(ststus)-> true -> add to list

                    //check if process stopped using ctrl+z
                    if(WIFSTOPPED(status))
                    {
                        insert_first(pid, input_string);
                        printf("[%d] Stopped %s\n", pid, input_string);
                    }
                   
                  
                }
              else if(pid == 0)   
              {
                signal(SIGINT,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);

                   execute_external_commands(input_string);
                   exit(0);
               
             }
             else
             {
                printf("No such command\n");
             }
             
                 
        }
        strcpy(input_string,"NULL");
        
        
    }
}

}
