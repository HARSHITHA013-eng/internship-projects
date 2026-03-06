#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1         // Max bytes to read from secret file at a time
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) // Each byte needs 8 image bytes for encoding
#define MAX_FILE_SUFFIX 4               // Max file extension length

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;        // // pointer to hold the name of Source image  (.bmp)
    FILE *fptr_src_image;           // File pointer for source image
    uint image_capacity;           // Capacity of the image
    uint bits_per_pixel;  //size of secret file
    char image_data[MAX_IMAGE_BUF_SIZE];  // Buffer to hold image data during encoding

    /* Secret File Info */
    char *secret_fname;          // Secret file name (.txt)
    FILE *fptr_secret;             // File pointer for secret file
    char extn_secret_file[MAX_FILE_SUFFIX];      // File extension of secret file
    char secret_data[MAX_SECRET_BUF_SIZE];        // Buffer to store secret data temporarily
    long size_secret_file;

    /* Stego Image Info */
    char *stego_image_fname;                // Output (encoded) image file name
    FILE *fptr_stego_image;                  // File pointer for stego image

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */        
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

//hide size of sectrete file 

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);


/* Encode integer (like size) into LSBs of image bytes */
Status encode_size_to_lsb(int size, char *size_buffer);

#endif



