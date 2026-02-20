#include "main.h"

Status insert_at_last(Flist **head, char *fname)
{
    Flist *new = malloc(sizeof(Flist));
    if(new == NULL)
        return FAILURE;
    
    strcpy(new->file_name, fname);
    new->link = NULL;

    if(*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    if(compare(*head, new->file_name) == DUPLICATE)
        return FAILURE;
    
    Flist *temp = *head;
    while(temp)
    {
        temp = temp->link;
    }

    temp = new;
    return SUCCESS;
}

void print_list(Flist *head)
{
    if(head == NULL)
    {
        printf("[Info] : List is Empty\n");
        return;
    }

    while(head)
    {
        printf("| %s | ->", head->file_name);
        head = head->link;
    }

}

