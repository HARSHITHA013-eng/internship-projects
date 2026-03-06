#include "header.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


void insert_first(int pid,char *cmd)
{
    
    job_t *new = malloc(sizeof(job_t));
    if(new==NULL)
    {
        return ;
    }
    new->pid=pid;
    int i=0;
    while (cmd[i] != '\0')
    {
        new->cmd[i] = cmd[i];
        i++;
    }
    new->cmd[i] = '\0';   
    new->next=job_head;
    job_head=new;

}

void delete_first(void)
{
    
    if(job_head==NULL)
    {
          printf(" List is empty, nothing to delete\n");
        return ;
    }
    job_t *temp=job_head;
    job_head=job_head->next;
    free(temp);

    return ;
}

void print_jobs()
{
    
    if(job_head == NULL)
     {
        printf("Job list is EMPTY!\n");
        return;
    }
    
    printf("Printing job list:\n");
     job_t *temp = job_head;
    while (temp)
    {
        printf("[%d] %s\n", temp->pid, temp->cmd);
        temp = temp->next;
    }
    
    //printf("=== DEBUG print_jobs END ===\n");
}

void fg_cmd()
{
   
    if(job_head == NULL)
    {
        printf("No jobs\n");
     
        return;
    }
    
    // Send SIGCONT to resume
    printf("%d\n",job_head->pid);
    kill(job_head->pid, SIGCONT);
    waitpid(job_head->pid,&status,WUNTRACED);
 //wait(NULL);
    
    delete_first();
    //free(job_head);
    printf("=== fg_cmd END ===\n");
    return;
}
   
void bg_cmd()
{
      signal(SIGCHLD,my_signal_handler);
   
    if(job_head == NULL)
    {
        printf("No jobs\n");
        return;
    }

    printf("%d\n",job_head->pid);
    
    // Send SIGCONT to resume the job in background
    kill(job_head->pid, SIGCONT) ;
    delete_first();
   // free(job_head);
    printf("=== bg_cmd END ===\n");
    return;
}
