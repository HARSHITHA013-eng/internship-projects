#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>

//function to read and validate the file name and extension
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
   //check if the 3rd argument (argv[2]) has .bmp file extension
  if(strstr(argv[2],".bmp")!=NULL)
  {
    printf(".bmp is present\n");
    decInfo->stego_image_fname=argv[2];
  }
  else
  {
    printf(".bmp is not present\n");
    return e_failure;
  }
  
    // Check if output file name is provided
    if (argv[3] == NULL)
    {
      printf(".txt file name not found, default file added: output.txt\n");
      decInfo->output_fname = "output.txt";  // Assign default output file name if not provided
    }
    else if (strstr(argv[3], ".txt") != NULL)
    {
      decInfo->output_fname = argv[3];       // Assign provided output file name if it has .txt extension
       printf(".txt is present\n");
    }
    else
    {
       printf("Invalid output file extension, must be .txt\n");  // Error if extension is not .txt
       return e_failure;                                         // Return failure for invalid file type
    }

    return e_success;
}

/* Function to perform decoding */
Status do_decoding(DecodeInfo *decInfo)
{
   if( open_decode_files(decInfo)==e_failure)
   {
    printf("error: Failed to open files\n");
    return e_failure;
   }
   printf("All files are opened successfully\n");

  // Step 2: Decode and verify magic string
  if (decode_magic_string(decInfo->fptr_stego_image, decInfo) == e_failure)
    {
        printf("Error: Magic string mismatch. Not a valid stego image!\n");
        return e_failure;
    }

    printf("Magic string verified successfully\n");
    printf("----------------------------------------\n");

  // Step 3: Decode file extension
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("Error decoding file extension\n");
        return e_failure;
    }

    printf("Decoded file extension: %s\n", decInfo->extn_secret_file);

    // Step 4: Decode secret file size
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("Error decoding secret file size\n");
        return e_failure;
    }

    printf("Decoded secret file size: %ld bytes\n", decInfo->size_secret_file);
     // Step 5: Decode secret message data
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("Error decoding secret data\n");
        return e_failure;
    }

    printf("Secret data decoded successfully!\n");

    return e_success;
}
  
/* Open the stego image and output file */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");  // Open stego image in read mode
    if (decInfo->fptr_stego_image == NULL)   // Check if file opening failed
    {
        printf("Error opening stego image file\n");
        return e_failure;
    }

    decInfo->fptr_output = fopen(decInfo->output_fname, "w");        // Open output file in write mode
    if (decInfo->fptr_output == NULL)
    {
        printf("Error opening output file\n");
        return e_failure;
    }

    return e_success;
}

/* Decode magic string */
Status decode_magic_string(FILE *fptr_stego_image, DecodeInfo *decInfo)
{
    char arr[8];
    char magic_data[4];
    
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        fread(arr,8,1,decInfo->fptr_stego_image);
        //decode_data_from_image(MAGIC_STRING, strlen(MAGIC_STRING), decInfo->fptr_stego_image);// Extract magic string from image
        magic_data[i] = decode_byte_from_lsb(arr);
    }
     
    magic_data[strlen(MAGIC_STRING)] = '\0';       // Null terminate string


    if (strcmp(magic_data, MAGIC_STRING) == 0)      // Compare with expected magic string
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}


/* Decode one byte from LSBs of 8 bytes */
char decode_byte_from_lsb(char *image_buffer)
{
    char ch = 0;
    for (int i = 0; i < 8; i++)
    {
        ch =ch| (image_buffer[i] & 1) << i; //  Extract LSB and shift to correct position
    }
    return ch;
}

/* Decode size (integer) from image */
int decode_size_from_lsb(char *image_buffer)
{
    int size=0;
    for(int i=0; i<32; i++)
    {
          size= size|( (image_buffer[i]&1)<<i);         // Extract LSB of each byte and form integer
    }
    return size;
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
  // Step 1: Read the size of the extension 32bytes 
  char arr[32];     
  char ext[8];                     
  fread(arr,32,1,decInfo->fptr_stego_image);           //reading 32bytes from stego
  int size_of_secrete_file=decode_size_from_lsb(arr);     //.txt file size
  printf("size of secrete file is : %d\n",size_of_secrete_file);
    for(int i=0; i<size_of_secrete_file; i++)
    {
        fread(ext,8,1,decInfo->fptr_stego_image) ;          //1char 8 bytes
        decInfo->extn_secret_file[i]= decode_byte_from_lsb(ext);
    }
    decInfo->extn_secret_file[size_of_secrete_file] = '\0';
  return e_success;
}
   


/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    decInfo->size_secret_file = decode_size_from_lsb(buffer);
    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char image_buffer[8];
    char ch;

    // Step 1: Read 'size_secret_file' bytes from image and write to output file
    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(image_buffer, 8, 1, decInfo->fptr_stego_image);
        ch = decode_byte_from_lsb(image_buffer);
        fputc(ch, decInfo->fptr_output);
    }

    // Step 2: Close output file after writing
    fclose(decInfo->fptr_output);
    return e_success;
}