#ifndef VIEW_H
#define VIEW_H

#include "types.h"

//function prototype to vies the mp3 tag information
Status view_tag(const char *filename);
//to edit title
Status edit(const char *frame_id, const char *new_value,const char *filename);

#endif
  