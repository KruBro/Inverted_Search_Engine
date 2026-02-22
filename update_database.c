/**
 * @file   update_database.c
 * @brief  Incrementally adds new files to the existing index.
 *
 * Instead of clearing and rebuilding the entire hash table, this module
 * records where the Flist ended before adding new files, then passes only
 * the new tail to create_database — avoiding redundant re-indexing.
 *
 * BUG FIX (v1.1 — Part A): Changed signature from (Flist *head) to
 *   (Flist **head) so that new nodes added to the list are visible to the caller.
 *
 * BUG FIX (v1.1 — Part B): Removed free(fileHolder) from main.c.
 *   fileHolder is a stack-allocated VLA; calling free() on it is undefined behavior.
 *
 * BUG FIX (v1.2 — Incremental update): Previously, the entire hash table was
 *   wiped and rebuilt from scratch on every update. Now only the newly added
 *   files are passed to create_database.
 */

#include "main.h"

/**
 * @brief  Validates and adds new files to the Flist, then indexes only those files.
 *
 * @param  arr        The 27-bucket hash table (modified in-place).
 * @param  head       Pointer-to-pointer to the Flist head (updated if new nodes added).
 * @param  fileHolder Array of filename strings to add.
 * @param  fileCount  Number of filenames in fileHolder.
 * @return SUCCESS always (individual file failures are logged but not fatal).
 */
Status update_database(hash_T *arr, Flist **head, char *fileHolder[], u_int fileCount)
{
    /* ── Step 1: Record the current tail of the list ──────────────────────
     * After adding new files, we'll start indexing from the node AFTER this
     * pointer — so we only process the newly added files.                  */
    Flist *lastNode = *head;
    if (lastNode != NULL)
        while (lastNode->link != NULL)
            lastNode = lastNode->link;

    /* ── Step 2: Validate and append new files to the Flist ── */
    for (int i = 0; i < fileCount; i++)
    {
        if (read_and_validation(fileHolder, i, head) == SUCCESS)
            printf(H_YELLOW "[Info] : Read and Validation of [%s] is Successful\n" RESET, fileHolder[i]);
        else
            printf(H_RED "[Error] : Read and Validation of [%s] is Failed\n" RESET, fileHolder[i]);
    }

    /* ── Step 3: Determine where to start indexing ──────────────────────
     * If the list was empty before, start from the new head.
     * If the list already had files, start from the node after the old tail. */
    Flist *startNode = (lastNode == NULL) ? *head : lastNode->link;

    /* ── Step 4: Index only the new files ── */
    if (startNode != NULL)
    {
        if (create_database(arr, startNode) == FAILURE)
            return FAILURE;
    }
    else
    {
        printf(H_BLUE "[Info] : No new valid files were added to index\n" RESET);
    }

    return SUCCESS;
}