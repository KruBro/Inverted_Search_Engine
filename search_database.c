/**
 * @file   search_database.c
 * @brief  Case-insensitive word lookup across the inverted index.
 *
 * Computes the hash bucket from the word's first character, then
 * traverses the mNode chain using strcasecmp for case-insensitive
 * matching. If found, prints every file the word appears in along
 * with occurrence counts, then displays the total.
 */

#include "main.h"

/**
 * @brief  Searches for a word in the hash table and prints the results.
 *
 * @param  arr   The 27-bucket hash table.
 * @param  word  The word to search for (case-insensitive).
 * @return SUCCESS if the word was found,
 *         DATA_NOT_FOUND if the word is not in the index.
 */
Status search_database(hash_T *arr, char *word)
{
    /* ── Compute bucket index from first character ── */
    int index;
    if(word != NULL && islower(word[0]))
        index = word[0] - 'a';
    else if(word != NULL && isupper(word[0]))
        index = word[0] - 'A';
    else
        index = 26;     /* Non-alphabetic token */

    /* ── Traverse the mNode chain for a case-insensitive match ── */
    mNode *mTemp = arr[index].link;
    while(mTemp && strcasecmp(mTemp->word, word) != 0)
        mTemp = mTemp->mLink;

    if(mTemp == NULL)
        return DATA_NOT_FOUND;

    /* ── Print per-file occurrences and accumulate total ── */
    u_int    word_count = 0;
    sNode   *sTemp      = mTemp->sLink;

    while(sTemp)
    {
        printf("[" H_CYAN "%s" RESET "]" CYAN ": found in %s : %d times\n" RESET,
               word, sTemp->file_name, sTemp->wordcount);
        word_count += sTemp->wordcount;
        sTemp       = sTemp->subLink;
    }

    printf("[" H_CYAN "%s" RESET "]" H_YELLOW ": Word appeared" RESET
           H_MAGENTA " %d" RESET " Times in The Database\n", word, word_count);

    return SUCCESS;
}