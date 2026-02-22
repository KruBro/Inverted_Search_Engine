/**
 * @file   create_database.c
 * @brief  Builds the inverted index by reading each file word-by-word.
 *
 * For every word read from a file, this module:
 *   1. Computes the hash bucket index from the first character.
 *   2. Searches the bucket's mNode chain for an existing entry.
 *   3. If found, increments the word count for the current file
 *      (or creates a new sNode if this is a new file for that word).
 *   4. If not found, creates a new mNode + sNode pair.
 *
 * BUG FIX (v1.1): The outer while(mTemp) loop was missing a `break` after
 * a word match was found and handled. This caused an infinite loop whenever
 * a word appeared more than once in the same file.
 */

#include "main.h"

/**
 * @brief  Reads all files in the Flist and indexes their words into the hash table.
 *
 * @param  arr   The 27-bucket hash table.
 * @param  head  Head of the Flist (files to index).
 * @return SUCCESS on completion, FAILURE if a file cannot be opened or malloc fails.
 */
Status create_database(hash_T *arr, Flist *head)
{
    Flist *temp = head;
    FILE  *fp;

    /* ── Iterate over each file in the linked list ── */
    while(temp)
    {
        fp = fopen(temp->file_name, "r");
        if(fp == NULL)
        {
            printf(H_RED "[Error] : File Could Not Open\n" RESET);
            return FAILURE;
        }
        printf(BOLD_GREEN "[Info] : %s Opened Successfully\n" RESET, temp->file_name);

        char input_word[20];

        /* ── Read the file one word at a time ── */
        while(fscanf(fp, "%s", input_word) != EOF)
        {
            /* Compute bucket index from the first character */
            int index;

            strip_punctuation(input_word);
            
            if(islower(input_word[0]))
                index = input_word[0] - 'a';        /* a=0, b=1, ..., z=25 */
            else if(isupper(input_word[0]))
                index = input_word[0] - 'A';        /* A=0, B=1, ..., Z=25 */
            else
                index = 26;                          /* Non-alphabetic token */

            /* ── Bucket is empty: create the first mNode + sNode ── */
            if(arr[index].link == NULL)
            {
                mNode *new_mainNode = malloc(sizeof(mNode));
                if(new_mainNode == NULL) return FAILURE;

                sNode *new_subNode = malloc(sizeof(sNode));
                if(new_subNode == NULL) { free(new_mainNode); return FAILURE; }

                new_mainNode->filecount = 1;
                strcpy(new_mainNode->word, input_word);
                new_mainNode->sLink = new_subNode;
                new_mainNode->mLink = NULL;

                strcpy(new_subNode->file_name, temp->file_name);
                new_subNode->wordcount = 1;
                new_subNode->subLink   = NULL;

                arr[index].link = new_mainNode;
            }
            else
            {
                /* ── Bucket has entries: search the mNode chain ── */
                mNode *mTemp = arr[index].link, *mPrev = NULL;

                while(mTemp)
                {
                    /* ── Word already exists in the index ── */
                    if(strcmp(mTemp->word, input_word) == 0)
                    {
                        /* Search for a sub-node matching the current file */
                        sNode *sTemp = mTemp->sLink, *sPrev = NULL;
                        while(sTemp)
                        {
                            if(strcmp(sTemp->file_name, temp->file_name) == 0)
                            {
                                /* Same file → just increment the count */
                                (sTemp->wordcount)++;
                                break;
                            }
                            sPrev = sTemp;
                            sTemp = sTemp->subLink;
                        }

                        /* Word is in a new file → add a new sNode */
                        if(sTemp == NULL)
                        {
                            sNode *new_subNode = malloc(sizeof(sNode));
                            if(new_subNode == NULL) return FAILURE;

                            new_subNode->wordcount = 1;
                            strcpy(new_subNode->file_name, temp->file_name);
                            new_subNode->subLink = NULL;

                            sPrev->subLink = new_subNode;
                            (mTemp->filecount)++;
                        }

                        /* FIX: break the outer loop — word is handled */
                        break;
                    }
                    else
                    {
                        mPrev = mTemp;
                        mTemp = mTemp->mLink;
                    }
                }

                /* ── Word not found in bucket: create a new mNode + sNode ── */
                if(mTemp == NULL)
                {
                    mNode *new_mainNode = malloc(sizeof(mNode));
                    if(new_mainNode == NULL) return FAILURE;

                    sNode *new_subNode = malloc(sizeof(sNode));
                    if(new_subNode == NULL) { free(new_mainNode); return FAILURE; }

                    new_mainNode->filecount = 1;
                    strcpy(new_mainNode->word, input_word);
                    new_mainNode->sLink = new_subNode;
                    new_mainNode->mLink = NULL;

                    strcpy(new_subNode->file_name, temp->file_name);
                    new_subNode->wordcount = 1;
                    new_subNode->subLink   = NULL;

                    mPrev->mLink = new_mainNode;
                }
            }
        }

        fclose(fp);
        temp = temp->link;
    }

    return SUCCESS;
}