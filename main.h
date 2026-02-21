#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef unsigned int u_int;

typedef enum 
{
    SUCCESS,
    FAILURE,
    LIST_EMPTY,
    DUPLICATE,
} Status;

typedef struct Node
{
    char file_name[20];
    struct Node *link;
} Flist;    

typedef struct subNode
{
    u_int wordcount;
    char file_name[20];
    struct subNode *subLink;
}sNode;

typedef struct mainNode
{
    u_int filecount;
    char word[20];
    sNode *sLink;
    struct mainNode *mLink;
}mNode;

typedef struct hashT
{
    u_int index;
    mNode *link;
} hash_T;


Status read_and_validation(char *argv[], int i, Flist **head);
Status compare(Flist *head, char *fname);
Status insert_at_last(Flist **head, char *fname);
void print_list(Flist *head);
void free_list(Flist **head);
void initialize_hashTable(hash_T *arr);
Status create_database(hash_T *arr, Flist *head);
void free_hash_table(hash_T *arr);
void display_database(hash_T *arr);


#endif