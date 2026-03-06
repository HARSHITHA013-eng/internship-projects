#ifndef HEAD_H
#define HEAD_H
#define Success 0
#define Failure -1


typedef struct node
{
    char filename[20];
    struct node *link;
}file_list;

//sub node
typedef struct inner
{
    char filename[100];   
    int word_count;
    struct inner *link;

}sub_node;  

//main node
typedef struct count
{
    char word[100];
    int file_count;
    struct count *next; //next word
    sub_node *sub_list; //sub node
    
}main_node;

int create( file_list **f_head, main_node *hashtable[], int argc, char *argv[]);
int hash_index(char *word);
int  insert_word(main_node *hashtable[],int index,char word[],char filename[]);
int search(main_node *hashtable[],char ser_word[]);
void display(main_node *hashtable[]);
void save(main_node *hashtable[],char name[]);
int update(main_node *hashtable[], char *filename);


#endif
