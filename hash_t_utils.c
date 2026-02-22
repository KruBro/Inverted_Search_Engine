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
 * Walks every bucket's mNode chain and, for each mNode, walks and frees
 * its sNode chain before freeing the mNode itself.
 * Does NOT free the hash_T array itself (it's stack-allocated in main).
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
            /* Free all sNodes for this word first */
            sNode *sTemp = mTemp->sLink;
            while(sTemp)
            {
                sNode *sPrev = sTemp;
                sTemp = sTemp->subLink;
                free(sPrev);
            }

            /* Then free the mNode itself */
            mNode *mPrev = mTemp;
            mTemp = mTemp->mLink;
            free(mPrev);
        }
    }
}