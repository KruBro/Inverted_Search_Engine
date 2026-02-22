/**
 * @file   flist_utils.c
 * @brief  Utility functions for the Flist (file linked list).
 *
 * Provides insert, print, and free operations for the singly-linked
 * list of filenames that the engine has loaded.
 */

#include "main.h"

/**
 * @brief  Appends a filename to the end of the Flist if it is not a duplicate.
 *
 * @param  head   Pointer-to-pointer to the Flist head.
 * @param  fname  Filename string to insert.
 * @return SUCCESS if the node was inserted,
 *         FAILURE if malloc fails or a duplicate was detected.
 */
Status insert_at_last(Flist **head, char *fname)
{
    Flist *new = malloc(sizeof(Flist));
    if(new == NULL)
        return FAILURE;

    strcpy(new->file_name, fname);
    new->link = NULL;

    /* First node — set as head */
    if(*head == NULL)
    {
        *head = new;
        return SUCCESS;
    }

    /* Duplicate check before traversing to the tail */
    if(compare(*head, new->file_name) == DUPLICATE)
    {
        free(new);
        return FAILURE;
    }

    /* Traverse to the tail and append */
    Flist *temp = *head;
    while(temp->link)
        temp = temp->link;

    temp->link = new;
    return SUCCESS;
}

/**
 * @brief  Prints all filenames in the Flist as a formatted chain.
 *
 * @param  head  Head of the Flist to print.
 */
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

/**
 * @brief  Frees all nodes in the Flist and sets head to NULL.
 *
 * @param  head  Pointer-to-pointer to the Flist head.
 */
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
        free(temp);
    }
}