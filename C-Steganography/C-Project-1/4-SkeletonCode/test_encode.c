/*
    Name          : HARSHITHA G H
    Project       : LSB Image Steganography
    Description   : Program to hide secret text data inside a BMP image using the 
                    Least Significant Bit (LSB) technique and retrieve it.Includes encoding and decoding 
                    functionalities with validation for file formats and input arguments.
    Date          : 27-10-2025
    Batch ID      : 25021_268
*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"

char MAGIC_STRING[20];   // Buffer to store magic string entered by user
int main(int argc, char *argv[])
{
     // Check if user has entered sufficient arguments
     if(argc==1 || argc==2)  
        {
           printf("It is unsupported\n");
           printf("|---------------------------------------------------------------------|\n");
           printf("| For Encoding: ./a.out -e <source.bmp> <secret.txt> [stego.bmp]      |\n");
           printf("| For Decoding: ./a.out -e <stego.bmp>  [out.bmp]                     |\n");
           printf("|---------------------------------------------------------------------|\n");
           return e_failure;
        }
     // If user chooses encoding operation
    if (check_operation_type(argv) == e_encode)
    {
        // Check if minimum arguments are provided
       if (argc < 4)
      {
        printf("ERROR: Missing arguments!\n");
        printf("|---------------------------------------------------------------------|\n");
        printf("| Usage: ./a.out -e <source.bmp> <secret.txt> [stego.bmp]             |\n");
        printf("|---------------------------------------------------------------------|\n");
        return e_failure;
      }
      printf("You have choosed encoding\n");
      printf("*******************************\n");
 
       EncodeInfo encInfo;
       // Validate and read encoding arguments
     if (read_and_validate_encode_args(argv, &encInfo) == e_success)
     {
       
         //taking magic string from user
        printf("Enter the magic string: \n");
        scanf("%s", MAGIC_STRING);
        printf("--------------------------------------------------------\n");
        printf("=====================================\n");
        printf("Read and Validate is successfull!\n");
        printf("=====================================\n");
        if (do_encoding(&encInfo) == e_success)
        {
            printf("=====================================\n");
            printf("Encoding completed successfully\n");
            printf("=====================================\n");
        }
        else
        {
            printf("####################################\n");
            printf("Encoding unsuccessfull\n");
            printf("####################################\n");
        }
     }
       else
      {
        printf("=====================================\n");
        printf("Read and Validate is unsuccessfull\n");
        printf("=====================================\n");
        return e_failure;
      }
    }
     // If user chooses decoding operation
    else if(check_operation_type(argv) == e_decode)
    {
        // Check if minimum arguments are provided
        if(argc<3)    
        {
           printf("It is unsupported\n");
           printf("|---------------------------------------------------------------------|\n");
           printf("| For Decoding: ./a.out -e <stego.bmp>  [out.bmp]                     |\n");
           printf("|---------------------------------------------------------------------|\n");
           return e_failure;
        }
        
             printf("You have choosed decoding\n");
             printf("*******************************\n");
       
         DecodeInfo decInfo; 
          // Validate and read decoding arguments
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            //taking magic string from user
              printf("Enter the magic string u have entered for encoding: \n");
               scanf("%s",MAGIC_STRING);
               printf("--------------------------------------------------------\n");
              printf("Arguments validated successfully\n");
              if (do_decoding(&decInfo) == e_success)
            {
               printf("=====================================\n");
               printf("Decoding completed successfully!\n");
               printf("=====================================\n");
            }
            else
            {
                printf("####################################\n");
                printf("Decoding failed!\n");
                printf("####################################\n");
                 return e_failure;
            }
        }
    }
   

    return e_success;
    
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)     // Check if user selected encoding
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d") == 0) // Check if user selected decoding
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
