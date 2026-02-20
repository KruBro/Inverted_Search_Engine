#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum 
{
    SUCCESS,
    FAILURE,
    LIST_EMPTY,
    DUPLICATE,
} Status;

typedef struct Node
{
    char *file_name;
    struct Node *link;
} Flist;

Status read_and_validation(int argc, char *argv[], Flist **head);
Status compare(Flist *head, char *fname);
Status insert_at_last(Flist **head, char *fname);
void print_list(Flist *head);


#endif