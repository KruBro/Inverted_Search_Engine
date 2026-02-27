/**
 * @file   validation.c
 * @brief  File validation and duplicate detection for the Flist.
 *
 * BUG FIX (v1.1): compare() contained a logic error — the inner while loop
 * advanced temp1 but the strcmp always used temp->file_name instead of
 * temp1->file_name. This meant the last node in the list was never checked,
 * allowing duplicates to be inserted at the tail. Fixed by flattening the
 * traversal to a single loop comparing each node against fname.
 */

#include "main.h"

/**
 * @brief  Validates a filename and, if valid, appends it to the Flist.
 *
 * Checks (in order):
 *   1. The file has a ".txt" extension.
 *   2. The file can be opened (exists and is accessible).
 *   3. The file is not empty.
 *   4. The filename is not already in the Flist (duplicate check).
 *
 * @param  argv  Argument vector (or any string array).
 * @param  i     Index into argv for the filename to validate.
 * @param  head  Pointer-to-pointer to the Flist head.
 * @return SUCCESS if the file passes all checks and was inserted,
 *         FAILURE otherwise (reason is printed to stdout).
 */
Status read_and_validation(char *argv[], int i, Flist **head)
{
    FILE *fp;

    /* Check for .txt extension */
    if(strstr(argv[i], ".txt") != NULL)
    {
        /* Check the file can be opened */
        fp = fopen(argv[i], "r");
        if(fp == NULL)
            return FAILURE;

        /* Check the file is not empty */
        fseek(fp, 0, SEEK_END);
        if(ftell(fp) == 0)
        {
            printf(H_RED "%s file is empty\n" RESET, argv[i]);
            fclose(fp);
            return FAILURE;
        }

        fclose(fp);

        /* Attempt insertion — insert_at_last handles duplicate detection */
        if(insert_at_last(head, argv[i]) == FAILURE)
        {
            printf(H_YELLOW "[Info] : %s's Duplicate Found\n" RESET, argv[i]);
            return FAILURE;
        }
    }
    else
    {
        printf(H_RED "[Info] : %s is not a .txt file\n" RESET, argv[i]);
        return FAILURE;
    }

    return SUCCESS;
}