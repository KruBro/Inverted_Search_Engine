/**
 * @file   hash_t_utils.c
 * @brief  Initialization and teardown for the 27-bucket hash table.
 *
 * The hash table is a stack-allocated array of hash_T structs in main().
 * This module provides the two lifecycle operations: setup and full free.
 */

#include "main.h"

/**
 * @brief  Initializes all 27 hash table buckets to a clean state.
 *
 * Sets each bucket's index and sets the mNode link to NULL.
 * Must be called before create_database or any search operations.
 *
 * @param  arr  The hash table array (length 27).
 */
void initialize_hashTable(hash_T *arr)
{
    for(int i = 0; i < 27; i++)
    {
        arr[i].index = i;
        arr[i].link  = NULL;
    }

    printf(H_MAGENTA "Hash Table initialised Successfully\n" RESET);
}

/**
 * @brief  Frees all heap-allocated mNode and sNode chains in the hash table.
 *
 * For each mNode: frees its sNode chain (including each sNode's file_name),
 * then frees the mNode's word string, then frees the mNode itself.
 * Does NOT free the hash_T array (it's stack-allocated in main).
 *
 * @param  arr  The hash table array (length 27).
 */
void free_hash_table(hash_T *arr)
{
    for(int i = 0; i < 27; i++)
    {
        mNode *mTemp = arr[i].link;
        while(mTemp)
        {
            /* ── Free all sNodes for this word first ── */
            sNode *sTemp = mTemp->sLink;
            while(sTemp)
            {
                sNode *sPrev = sTemp;
                sTemp = sTemp->subLink;
                free(sPrev->file_name); /* free heap-allocated filename */
                free(sPrev);
            }

            /* ── Advance before freeing — then free word and node ── */
            mNode *mPrev = mTemp;
            mTemp = mTemp->mLink;
            free(mPrev->word);          /* free heap-allocated word string */
            free(mPrev);
        }
    }
}