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
    {
        printf("[Info] : Error Happened\n");
        return FAILURE;
    }
    
    Flist *temp = *head;
    while(temp->link)
    {
        temp = temp->link;
    }

    temp->link = new;
    return SUCCESS;
}

void print_list(Flist *head)
{
    if(head == NULL)
    {
        printf("[Info] : List is Empty\n");
        return;
    }

    printf("_____________________________\n");
    printf("\n");
    while(head)
    {
        printf("| %s |", head->file_name);
        printf("%s", head->link ? " -> " : "\n");
        head = head->link;
    }
    printf("_____________________________\n");
    printf("\n");
}

void free_list(Flist **head)
{
    if(*head == NULL)
        printf("[Info] : List is Empty\n");
    
    while(*head)
    {
        Flist *temp = *head;
        *head = (*head)->link;
        free(temp);
    }

    printf("[Info] : List Deletion Successfull\n");
}
