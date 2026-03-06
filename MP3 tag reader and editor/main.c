#include <stdio.h>
#include "common.h"
#include "types.h"
#include <string.h>
#include "view.h"

int main(int argc, char *argv[])
{
     if(argc<2)
     {
          printf("Missing file name\n");
          printf("--------------------------------------------------------------------------\n");
          printf("Usage(for help): -h/--help\n");
          printf("Usage(for view): -v <filename.mp3>\n");
          printf("usage(for edit): ./a.out -e -(options) <new_value> <filename.mp3>\n");
          printf("--------------------------------------------------------------------------\n");
          return e_failure;
     }
     //Check if user asked for help
     if( ( strcmp(argv[1],"--help") ==0 ) || ( strcmp (argv[1],"-h") ==0) )
     {
          printf("Asked for help\n");
          printf("|-----------------------------------------------|\n");
          printf("|  usage: mp3tag -[tTaAycg] value file1         |\n");
          printf("|  mp3tag -v                                    |\n");
          printf("|  -t  Modifies a Title tag                     |\n");
          printf("|  -a  Modifies an Artist tag                   |\n");
          printf("|  -A  Modifies an Album tag                    |\n");
          printf("|  -y  Modifies a Year tag                      |\n");
          printf("|  -c  Modifies a Comment tag                   |\n");
          printf("|  -g  Modifies a Genre tag                     |\n");
          printf("|  -h  Displays this help info                  |\n");
          printf("|  -v  Prints version info                      |\n"); 
          printf("|-----------------------------------------------|\n");
          return e_success;

     }
     if (strcmp(argv[1],"-v")==0)
     {
        if(argc!=3)
        {
            printf("filename is missing\n");
            printf("Usage: -v <filename.mp3>\n");
            return e_failure;
        }
        printf("View option is selected\n");
        printf("-------------------------------------------------------\n");
        view_tag(argv[2]);
     }
     if(strcmp(argv[1],"-e")==0)
     {
          printf("Edit option is selected\n");
          if(argc !=5)
          {
               printf("usage: ./a.out -e -(options) <new_value> <filename.mp3>\n");
               return e_failure;
          }
          edit(argv[2],argv[3],argv[4]);
          
     }
   
     

}