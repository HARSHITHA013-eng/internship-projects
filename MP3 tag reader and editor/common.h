  #ifndef COMMON_H
#define COMMON_H

//ID3v2.3 Frame header
typedef struct 
{
    char title[30];
    char Artist[30];
    char Album[30];
    char Year[4];
    char comments[30];
    char Genre[1];

}mp3tag;

#endif
