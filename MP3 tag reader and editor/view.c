#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "types.h"
#include "view.h"

// function to view MP3 tags from a given file
Status view_tag(const char *filename)
{
  char header[10];
  // open file pointer
  FILE *fp = NULL;
  fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    printf("Error: Unable to open %s file\n", filename);
    return e_failure;
  }
  // skip 10 bytes from ID3 header(header id , version, flag, size)
  fseek(fp, 10, SEEK_SET);

  // structure to store the data
  mp3tag tag = {0};
  // read frames after header

  char frame_header[10];
  
  for(int i=0; i<6; i++)
  {
    // Read frame header (10 bytes: 4 ID + 4 size + 2 flags)
    fread(frame_header, 1, 10, fp);
    char frame_id[5] = {0};
    for (int j = 0; j < 4; j++)
    {
      frame_id[j] = frame_header[j];
    }

    if (frame_id[0] == 0)
    {
      break;
    }

    // Convert size from big endian
    unsigned char size_bytes[4];

    // Copy the 4 bytes
    for (int i = 0; i < 4; i++)
    {
       size_bytes[i] = frame_header[4 + i];
    }

    // Swap (convert big ↔ little endian)
    for (int i = 0; i < 2; i++)
    {
        unsigned char temp = size_bytes[i];
       size_bytes[i] = size_bytes[3 - i];
       size_bytes[3 - i] = temp;
    }

    int size = *(int *)size_bytes;
    if (size <= 0)
    {
      break;
    }

    // Allocate memory for frame data
    char *data = malloc(size + 1);
    if (!data)
    {
      printf("Memory allocation failed\n");
      fclose(fp);
      return e_failure;
    }

    // Read frame content
    if (fread(data, 1, size, fp) != (size_t)size)
    {
      printf("Unable to read frame data for %s\n", frame_id);
      free(data);
      break;
    }

    data[size] = '\0';

    // ID3v2 text frames start with encoding byte
    char *text = data + 1;

    // Copy into the structure
    if (strcmp(frame_id, "TIT2") == 0)
    {
      strncpy(tag.title, text, sizeof(tag.title) - 1);
    }
    else if (strcmp(frame_id, "TPE1") == 0)
    {
      strncpy(tag.Artist, text, sizeof(tag.Artist) - 1);
    }
    else if (strcmp(frame_id, "TALB") == 0)
    {
      strncpy(tag.Album, text, sizeof(tag.Album) - 1);
    }
    else if (strcmp(frame_id, "TYER") == 0)
    {
      strncpy(tag.Year, text, sizeof(tag.Year) - 1);
    }
    else if (strcmp(frame_id, "COMM") == 0)
    {
      strncpy(tag.comments, text, sizeof(tag.comments) - 1);
    }
   else if (strcmp(frame_id, "TCON") == 0)
    {
      strncpy(tag.Genre, text, sizeof(tag.Genre) - 1);
    }

    printf("%s  : ", frame_id);
    for(int i=1; i<size; i++)   //skip encoding byte.
    {
         putchar(data[i]);
    }
    printf("\n");

    free(data);
  }

  fclose(fp);
  printf("-------------------------------------------------------\n");
   return e_success;
}



Status edit(const char *option, const char *new_value, const char *filename)
{
  FILE *src = fopen(filename, "rb");
  if (src == NULL)
  {
    printf("Failed to open source file \n");
    return e_failure;
  }
  printf("Source file opened successfully\n");
  FILE *dest = fopen("destination.mp3", "wb");
  if (dest == NULL)
  {
    printf("Failed to open destination/output file\n");
    fclose(src);
    return e_failure;
  }
  printf("Destination file opened successfully\n");

  // copy 10 bytes ID3V2 header
  unsigned char header[10];
  fread(header, 1, 10, src);
  fwrite(header, 1, 10, dest);

  char frame_header[10];

  // Determine target frame based on option
  const char *target_id = NULL;
  if (strcmp(option, "-t") == 0)
  {
    printf("You have chosen -t\n");
    target_id = "TIT2";
  }
  else if (strcmp(option, "-a") == 0)
  {
    printf("You have chosen -a\n");
    target_id = "TPE1";
  }
  else if (strcmp(option, "-A") == 0)
  {
    printf("You have chosen -A\n");
    target_id = "TALB";
  }
  else if (strcmp(option, "-y") == 0)
  {
    printf("You have chosen -y\n");
    target_id = "TYER";
  }
  else if (strcmp(option, "-c") == 0)
  {
    printf("You have chosen -c\n");
    target_id = "COMM";
  }
  else if (strcmp(option, "-g") == 0)
  {
    printf("You have chosen -g\n");
    target_id = "TCON";
  }

  if (target_id == NULL)
  {
    printf("Invalid option entered\n");
    fclose(src);
    fclose(dest);
    return e_failure;
  }

  // Read and process all frames
  while (fread(frame_header, 1, 10, src) == 10)
  {
    char frame_id[5];
    for (int i = 0; i < 4; i++)
    {
      frame_id[i] = frame_header[i];
    }
    frame_id[4] = '\0';

    if (frame_id[0] == 0)
    {
      break;
    }

    // Convert size from big endian
    unsigned char size_bytes[4];

    // Copy the 4 bytes
   for (int i = 0; i < 4; i++)
  {
    size_bytes[i] = frame_header[4 + i];
  }

   // Swap (convert big ↔ little endian)
    for (int i = 0; i < 2; i++)
  {
    unsigned char temp = size_bytes[i];
    size_bytes[i] = size_bytes[3 - i];
    size_bytes[3 - i] = temp;
  }

     int size = *(int *)size_bytes;

    if (size <= 0)
    {
      break;
    }

    // read data
    unsigned char *data = malloc(size);
    if (!data)
    {
      printf("Memory allocation failed\n");
      fclose(src);
      fclose(dest);
      return e_failure;
    }

    fread(data, 1, size, src);

    //printf("Found frame ID: %s | Target ID: %s\n", frame_id, target_id);

    if (strcmp(frame_id, target_id) == 0)
    {
     // printf("Chosen to edit %s\n", target_id);

      int new_size = strlen(new_value) + 1; // +1 for encoding byte
      unsigned char new_frame_header[10];

      // Copy old header
// Manually copy frame header bytes instead of memcpy
    for (int i = 0; i < 10; i++)
    {
        new_frame_header[i] = frame_header[i];
    }
      // Write new size (big endian)
     unsigned char size_bytes_new[4];

    // Copy new_size bytes (little endian first)
    for (int i = 0; i < 4; i++)
    {
      size_bytes_new[i] = ((unsigned char *)&new_size)[i];
    }

    // Swap bytes to convert (little ↔ big)
   for (int i = 0; i < 2; i++)
   {
    unsigned char temp = size_bytes_new[i];
    size_bytes_new[i] = size_bytes_new[3 - i];
    size_bytes_new[3 - i] = temp;
  }

     // Write swapped size into header
    for (int i = 0; i < 4; i++)
    {
      new_frame_header[4 + i] = size_bytes_new[i];
    }

     // Write new frame header
      fwrite(new_frame_header, 1, 10, dest);

      // Write encoding byte (0x00)
      unsigned char encoding = 0x00;
      fwrite(&encoding, 1, 1, dest);

      // Write new text value
      fwrite(new_value, strlen(new_value), 1, dest);

      //printf("%s : %s\n", frame_id, new_value);
    }
    //option does not match copy as it is 
    else
    {
      fwrite(frame_header, 1, 10, dest);
      fwrite(data, 1, size, dest);
    }

    free(data);
   
  }

  // Copy remaining bytes (audio data)
  char ch;
  while (fread(&ch, 1, 1, src) == 1)
  {
    fwrite(&ch, 1, 1, dest);
  }

  fclose(src);
  fclose(dest);

  printf("---------------------------------------------\n");
  printf("Editing completed successfully\n");
   printf("---------------------------------------------\n");

  return e_success;
}
