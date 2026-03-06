#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "encode.h"
#include <string.h>
#include "common.h"
//Function to read and validate the file names and extensions 
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check if the 3rd argument (argv[2]) has ".bmp" extension for source image
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];          // Store source image filename
        printf("beautiful.bmp is present\n");
    }
    else
    {
        printf("beautiful.bmp is not present\n");    // Error message if .bmp missing
        printf("INFO: Extension [.bmp] is required...!!!\n");
        return e_failure;                            // Stop execution
    }

    // Check if the 4th argument (argv[3]) has ".txt" extension for secret file
    if (strstr(argv[3], ".txt") != NULL)
    {
        encInfo->secret_fname = argv[3];             // Store secret text filename
        printf("secret.txt is present\n");
    }
    else
    {
        printf("secret.txt is not present\n");        // Error message if .txt missing
        printf("INFO: Extension [.txt] is required...!!!\n");
        return e_failure;                             // Stop execution
    }

   //check for stego.bmp file
    if(argv[4] == NULL)
    {
        //if NULL, create new stego.bmp file
        printf("Stego.bmp file created successfully\n");
        encInfo->stego_image_fname = "stego.bmp";
        
    }
    else if(strstr(argv[4],".bmp") != NULL)
    {
        //if .bmp file is present store it
        printf("stego.bmp is present\n");
        printf("----------------------------------------\n");
        encInfo->stego_image_fname = argv[4]; 
    }
    else
    {
        printf("INFO: Extension[.bmp] is required... !!!\n"); 
        return e_failure;
    }

    return e_success;              //all validations passed
}



//Function to perform encoding 
 Status do_encoding(EncodeInfo *encInfo)
{
    // Step 1: Open all required files
    if (open_files(encInfo) == e_failure)
    {
        printf("Error: Failed to open files\n");
        return e_failure;
    }
    printf("=====================================\n");
    printf("All files opened successfully\n");
    printf("=====================================\n");
    // Step 2: Check if image has enough space to hide the secret file
    if (check_capacity(encInfo) == e_failure)
    {
        printf("Error: Image does not have enough capacity\n");
        return e_failure;
    }
    printf("=====================================\n");
    printf("Image has enough capacity\n");
    printf("=====================================\n");
    // Step 3: Copy the 54-byte BMP header (never modify it)
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error: Failed to copy BMP header\n");
        return e_failure;
    }
    printf("Header copied successfully\n");
    // Step 4: Encode the magic string (“#*”)
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("Error: Failed to encode magic string\n");
        return e_failure;
    }
    printf("Magic string encoded successfully\n");
     // Step 5: Encode secret file extension (.txt)
    if (encode_secret_file_extn(strstr(encInfo->secret_fname, ".txt"), encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret file extension\n");
        return e_failure;
    }
    printf("Secret file extension encoded successfully\n");
    // Step 6: Encode size of the secret file
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret file size\n");
        return e_failure;
    }
    printf("Secret file size encoded successfully\n");
    // Step 7: Encode actual secret message data
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret data\n");
        return e_failure;
    }
    printf("Secret data encoded successfully\n");
    // Step 8: Copy remaining image bytes to output file
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error: Failed to copy remaining data\n");
        return e_failure;
    }
    printf("Remaining image data copied successfully\n");
     // Encoding completed
    printf("=====================================\n");
    printf("Encoding completed successfully!\n");
    printf("=====================================\n");
    return e_success;
}

// Function to open all required files (source, secret, and output files) 
Status open_files(EncodeInfo *encInfo)
{
    // Open the source image file (.bmp) in read mode
    encInfo->fptr_src_image =fopen( encInfo->src_image_fname,"r");
    if(encInfo->fptr_src_image==NULL)
    {
        printf("Source file is not opened\n");
        return e_failure;
    }
    else
    {
        printf("Source file is open\n");
    }
    // Open the secret text file (.txt) in read mode
    encInfo->fptr_secret =fopen( encInfo->secret_fname,"r");
    if(encInfo->fptr_secret==NULL)
    {
        printf("Secret file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Secret file is open\n");
    }
    // Open the output stego image file (.bmp) in write mode
     encInfo->fptr_stego_image =fopen( encInfo->stego_image_fname,"w");
    if(encInfo->fptr_stego_image==NULL)
    {
        printf("Stego file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Stego file is open\n");
    }
   
    return e_success;
   
}

// Function to check whether image has enough capacity to hide secret data- Calculates image capacity and secret file size- Compares them to ensure encoding is possible
 Status check_capacity(EncodeInfo *encInfo)
{
    // getting image size (beautiful.bmp)
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    if( encInfo -> image_capacity != 0)
    {
        printf("Found Image size\n");
    }
    else
    {
        return e_failure;
    }
    // getting secret file size(password)
    encInfo -> size_secret_file = get_file_size(encInfo->fptr_secret);

    if(encInfo -> size_secret_file != 0)
    {
        printf("Found Secret file size\n");
    }
    else
    {
        return e_failure;
    }
    //checking for our o/p file has the capcity to hold all Encoded data
    if(encInfo -> image_capacity < (54 * ( (2 + 4 + 4 + 4) * (encInfo -> size_secret_file)* 8)))
    {
        return e_failure;
    }

    return e_success;                                    // Image has enough capacity
}

// Function to get image size (width * height * 3)- Reads BMP width and height from header-Calculates total number of bytes for RGB data
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;

    fseek(fptr_image, 18, SEEK_SET);                     // Move pointer to byte 18 (width position)
    fread(&width, sizeof(int), 1, fptr_image);           // Read width (4 bytes)
    printf("width = %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image);          // Read height (next 4 bytes)
    printf("height = %u\n", height);

    return width * height * 3;                           // Each pixel has 3 color bytes (RGB)
}

/* Function to get secret file size- Moves pointer to end and uses ftell() to get total bytes*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);                            // Move pointer to end of file
    unsigned long int size= ftell(fptr) ;                              // Print current position (file size)
    rewind (fptr);
    printf("size of secret file = %lu\n", size);   
    return size;                                  // Return total file size in bytes
}


//Function to copy BMP header (first 54 bytes) - Header contains file format info (not to be modified)
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    int arr[54];                                         // Buffer to store 54-byte header
    fseek(fptr_src_image, 0, SEEK_SET);                  // Move pointer to start of file
    fread(arr, 54, 1, fptr_src_image);                  // Read header from source image
    fwrite(arr, 54, 1, fptr_stego_image);              // Write header to stego image
    return e_success;
}


//Function to encode magic string "#*"- Helps identify that image has hidden data
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // Call helper function to encode each character of magic string
    encode_data_to_image(MAGIC_STRING, strlen(MAGIC_STRING),encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}


//Function to encode given data string into image bytes- Each character (1 byte) requires 8 image bytes
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char arr[8];                                         // Buffer of 8 bytes (1 byte per bit)
    for (int i = 0; i < size; i++)                       // Loop for each character in string
    {
        fread(arr, 8, 1, fptr_src_image);                // Read 8 bytes from image
        encode_byte_to_lsb(data[i], arr);                // Encode character bits into LSBs
        fwrite(arr, 8, 1, fptr_stego_image);             // Write modified bytes to stego file
    }
    return e_success;
}


//Function to encode one byte into the LSBs of 8 image bytes
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)                          
    {
        image_buffer[i] = (image_buffer[i] & (~1)) | ((data & (1 << i)) >> i);
        // Clears LSB of image byte and sets it to the corresponding data bit
    }
    return e_success;
}


// Function to encode secret file extension (.txt)
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    int size = strlen(file_extn);                        // Length of extension (.txt)
    char arr[32];                                        // Buffer to encode size info

    for (int i = 0; i < size; i++)                       // Copy extension to structure
    {
        encInfo->extn_secret_file[i] = file_extn[i];
    }

    fread(arr, 32, 1, encInfo->fptr_src_image);          // Read 32 bytes from image
    encode_size_to_lsb(size, arr);                       // Encode extension size (int → 32 bits)
    fwrite(arr, 32, 1, encInfo->fptr_stego_image);       // Write encoded bytes

    // Now encode actual extension characters 
    encode_data_to_image(encInfo->extn_secret_file, size,encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

//Function to encode integer (size) into LSBs of 32 bytes
Status encode_size_to_lsb(int size, char *size_buffer)
{
    for (int i = 0; i < 32; i++)                       
    {
        size_buffer[i] = (size_buffer[i] & (~1)) | ((size & (1 << i)) >> i);
    }
     return e_success;
}
 
//Function to encode secret file size (length in bytes)
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char arr[32];   // Temporary buffer
    // Read 32 bytes from source image
    fread(arr, 32, 1, encInfo->fptr_src_image);
    // Encode file size into LSBs of arr
     encode_size_to_lsb(encInfo->size_secret_file, arr) ;
    // Write 32 bytes to stego image
     fwrite(arr, 32, 1, encInfo->fptr_stego_image) ;
    return e_success;
}

// Function to encode secret file data (actual text message)
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // Check if file pointers are valid
    if (encInfo->fptr_secret == NULL || encInfo->fptr_src_image == NULL || encInfo->fptr_stego_image == NULL)
    {
        printf("Error: File pointers not initialized properly\n");
        return e_failure;
    }

    int size = encInfo->size_secret_file;            // Get total size of the secret file
    fseek(encInfo->fptr_secret, 0, SEEK_SET);        // Move to start of secret file

    char ch, data[100];                              // Temporary buffer to store secret data
    int i = 0;

    // Read secret file character by character
    while ((ch = fgetc(encInfo->fptr_secret)) != EOF)
    {
        if (i < 100)                                 // Prevent buffer overflow
            data[i++] = ch;
        else
        {
            printf("Error: Secret file too large for buffer\n");
            return e_failure;
        }
    }

    // Encode secret message data into the image
    if (encode_data_to_image(data, size, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("Error: Failed to encode secret data\n");
        return e_failure;
    }

    return e_success;                                // Return success if encoding completed
}


//Function to copy remaining image data after encoding
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;                                             // Temporary variable

    if (fptr_src == NULL || fptr_dest == NULL) 
    {                                        // Check for valid pointers
        return e_failure;
    }
    clearerr(fptr_src);                                  // Clear previous file errors

    while (fread(&ch, 1, 1, fptr_src) > 0)               // Read remaining bytes from source
    {
        fwrite(&ch, 1, 1, fptr_dest);                    // Write remaining bytes to stego
    }

    if (ferror(fptr_src))
    {                                // Check if reading failed
        return e_failure;
    }
    return e_success;                                    // Successfully copied remaining data
}
