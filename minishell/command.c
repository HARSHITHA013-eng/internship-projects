#include "header.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char *external_commands[153];
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                                                "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                                                "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","bg","jobs" ,NULL};
                                                

char *get_command(char *input_string)
{
    static char command[20];
    int i=0;
       
    while (input_string[i] != ' ' && input_string[i]!='\0')
    {
        command[i] = input_string[i];
        i++;
    }

    command[i] = '\0';
    

    return command;
}

int check_command_type(char *command)
{
    // Check internal commands
   
    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(command, builtins[i]) == 0)
        {
            return 1;
        }
    }
    // Check EXTERNAL commands
    for (int i = 0; external_commands[i] != NULL; i++)
    {   
       
        if (strcmp(command, external_commands[i]) == 0)
        {
          
            return 2;
        }
    }
   
    
    return NO_COMMAND; // 3
}
void extract_external_commands(char **external_commands)
{
    int fd = open("ext_cmd.txt", O_RDONLY);
    if(fd<0)
    {
        printf("Failed to open ext_cmd.txt\n");
        return;
    }
    char ch;
    char buffer[50];
    int i = 0;
    int j = 0;
    while (read(fd, &ch, 1) > 0 && i<151)
    {
        if (ch == '\n')
        {
            buffer[j] = '\0';

            // allocate memory for command
            external_commands[i] = malloc(strlen(buffer)+1); // 50
            // store the command to 2d array
            strcpy(external_commands[i], buffer);
            i++;
            j=0;
            if(i>=152)
            {
                break;
            }
        }
        else if(ch!='\r')
        {
            buffer[j] = ch;
            j++;
        }
    }

    external_commands[i] = NULL;
    //check all the commands are stored in 2d array (declare 2d array globally)
  /*  for(int i=0; external_commands[i]!=NULL; i++)
    {
        printf("external commands[%d]=%s\n",i,external_commands[i]);
    }
  */
    close(fd);
    //printf("Loaded %d external commands\n",i);
}
void execute_internal_commands(char *input_string)
{
    char buff[500];
   
    if (strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(input_string, "pwd") == 0)
    {
        //getcwd(buff, 50);
        char *cwd=getcwd(NULL, 0);
        if(cwd)
        {
           printf("%s\n", cwd);
           free(cwd);
        }
    }
    else if (strncmp(input_string, "cd",2) == 0)
    {
        chdir(input_string + 3);
        getcwd(buff, 50);
    }
    else if(strcmp(input_string,"ls")==0)
    {
        pid=fork();
        if(pid>0)
        {
            //parent
            waitpid(pid,&status,0);
        }
        else if(pid==0)
        {
              char *args[]={"ls",NULL};
              execvp("ls",args);
              exit(1);
        }
        else{
            printf("failed\n");
        }
    }
    //check for fg
    else if(strcmp(input_string,"fg")==0)
    {
        fg_cmd();
        return;
    }
    //check for bg
    else if(strcmp(input_string,"bg")==0)
    {
       
       bg_cmd();
        return;
    }
    //check for job
    else if (strcmp(input_string, "jobs") == 0)
    {
        //print list
        print_jobs();
        return;
    }
    else if (strncmp(input_string, "echo", 4) == 0)
   {
       char *arg = input_string + 5;  // Skip "echo "
    
       if (strcmp(arg, "$$") == 0)
        {
           printf("%d\n", getpid());
        }
        else if (strcmp(arg, "$?") == 0) 
        {
           if(WIFEXITED(status))
            {
                printf("Terminated normally\n");
            }
             else 
            {
               printf("Abnormal Termination\n");
            }
         }
          else if (strcmp(arg, "$SHELL") == 0)
        {
            printf("%s", getenv("SHELL"));
        }
        else
        {
            printf("%s\n",arg);

        }
        printf("\n");
    }
   
    // to here

    // echo S?
    // keep pid and status in global variable
    // write only once

    // echo $SHELL
    // print the path of environmental varibale
    // use getenv("SHELL");
}

void execute_external_commands(char *input_string)
{
    char *argv[30];
    int count=0;
    int arr[100];
    int argc=0;
    int pipe_count=0;
      
     argv[argc]=strtok(input_string," ");
     
    while(argv[argc]!=NULL)
    {
        argc++;
        argv[argc]=strtok(NULL," ");
    }
        //pipe present and n pipe logic
        arr[count++]=0;
        for(int i=0; i<argc; i++)
     {
        if(strcmp(argv[i],"|")==0)
        {
            argv[i]=NULL;
            arr[count++]=i+1;
            pipe_count++;
            
           
        }
     }
       //create pipe
     //int status;
     int pipe_fd[2];
     for(int i=0; i<=pipe_count ;i++)
     {
        if(i!=pipe_count)
        {
            pipe(pipe_fd);
        }
        
            int pid1=fork();
         
           
            if(pid1>0)
            {
                 waitpid(pid1, &status, 0);
                //parent
                if(i!=pipe_count)
                {
                    dup2(pipe_fd[0],0);
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);

                }
               
            }
            else if(pid1==0)
            {
                //child1
                if(i!=pipe_count)
                {
                     dup2(pipe_fd[1],1);
                     close(pipe_fd[0]);
                }
                execvp(argv[arr[i]],argv+arr[i]);
               
                
            }
        
       }
    
    
    
}

