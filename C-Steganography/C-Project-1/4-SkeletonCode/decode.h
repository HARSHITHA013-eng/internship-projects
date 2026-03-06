#ifndef DECODE_H
#define DECODE_H
// Maximum allowed lengths and buffer sizes
#include "types.h"
#define MAX_FILE_NAME 50    // Maximum filename length
#define MAX_EXTN_SIZE 5       // Maximum extension size (.txt)
#define MAX_SECRET_BUF_SIZE 1    // Secret data buffer size (1 byte)
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)       // Each byte needs 8 image bytes
typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;      // Stego image file name
    FILE *fptr_stego_image;         // File pointer for stego image
     

    /* Output Secret File Info */
    char *output_fname;             // Output secret file name
    FILE *fptr_output;               // File pointer for output file

    char extn_secret_file[MAX_EXTN_SIZE];   // Stores decoded file extension
    long size_secret_file;                       // Stores decoded secret file size
    char image_data[MAX_IMAGE_BUF_SIZE];         // Buffer for image data
    char secret_data[MAX_SECRET_BUF_SIZE];          // Buffer for decoded secret data

} DecodeInfo;

/* Read and validate Encode args from argv */        
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files required for decoding */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Decode one byte from LSBs of 8 bytes */
char decode_byte_from_lsb(char *image_buffer);

/* Decode size (integer) from image */
int decode_size_from_lsb(char *image_buffer);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif