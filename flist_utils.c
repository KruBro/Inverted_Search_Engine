#include "main.h"

/**
 * OPTIMIZATION (v1.3): Combined duplicate check and tail traversal into a
 * single pass. Previously, compare() walked the full list, then a second
 * loop walked it again to find the tail — two full O(F) traversals.
 * Now both happen in one loop: duplicate check at every node, tail found
 * naturally when temp->link == NULL.
 */
Status insert_at_last(Flist **head, char *fname)
{
    Flist *new = malloc(sizeof(Flist));
    if(new == NULL)
        return FAILURE;

    /* Heap-allocate the filename — consistent with char *file_name in struct */
    new->file_name = strdup(fname);
    if(new->file_name == NULL) { free(new); return FAILURE; }
    new->link = NULL;

    /* Empty list — new node becomes the head */
    if(*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    /* Single pass: duplicate check AND tail find simultaneously */
    Flist *temp = *head;
    while(temp->link)
    {
        if(strcmp(temp->file_name, fname) == 0)
        {
            free(new->file_name);   /* free heap string before dropping the node */
            free(new);
            return FAILURE;         /* Duplicate found mid-list */
        }
        temp = temp->link;
    }

    /* Check the last node before appending */
    if(strcmp(temp->file_name, fname) == 0)
    {
        free(new->file_name);
        free(new);
        return FAILURE;             /* Duplicate is the tail node */
    }

    temp->link = new;
    return SUCCESS;
}

void print_list(Flist *head)
{
    if(head == NULL)
    {
        printf(H_YELLOW "[Info] : List is Empty\n" RESET);
        return;
    }

    printf(BG_BLUE "_____________________________\n" RESET);
    printf("\n");
    while(head)
    {
        printf(BOLD_YELLOW "| %s |" RESET, head->file_name);
        printf(H_CYAN "%s" RESET, head->link ? " -> " : "\n");
        head = head->link;
    }
    printf(BG_BLUE "_____________________________\n" RESET);
    printf("\n");
}

void free_list(Flist **head)
{
    if(*head == NULL)
    {
        printf(H_YELLOW "[Info] : List is Empty\n" RESET);
        return;
    }

    while(*head)
    {
        Flist *temp = *head;
        *head = (*head)->link;
        free(temp->file_name);  /* free heap-allocated filename first */
        free(temp);
    }
}