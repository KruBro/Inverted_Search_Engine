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
#include "main.h"

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

    mNode *mTemp = arr[index].link;
    int search_len = strlen(word);
    int found_any = 0; // Flag to track if we found at least one match

    /* ── Traverse the ENTIRE mNode chain looking for prefix matches ── */
    while(mTemp != NULL)
    {
        // Compare only up to the length of the search word
        if(strncasecmp(mTemp->word, word, search_len) == 0)
        {
            found_any = 1; // We found at least one!
            
            u_int word_count = 0;
            sNode *sTemp = mTemp->sLink;

            // Print the full matched word
            printf("Found match: [" H_GREEN "%s" RESET "]\n", mTemp->word);

            while(sTemp)
            {
                printf("  -> in %s : %d times\n", sTemp->file_name, sTemp->wordcount);
                word_count += sTemp->wordcount;
                sTemp = sTemp->subLink;
            }

            printf("  -> Total appearances: " H_MAGENTA "%d" RESET " Times\n\n", word_count);
        }
        
        mTemp = mTemp->mLink; // Keep checking the rest of the bucket!
    }

    if(found_any == 0)
        return DATA_NOT_FOUND;

    return SUCCESS;
}