#include "main.h"

void initialize_hashTable(hash_T *arr)
{
    for(int i = 0; i < 27; i++)
    {
        arr[i].index = i;
        arr[i].link = NULL;
    }

    printf("Hash Table initialised Successfully\n");
}


void free_hash_table(hash_T *arr)
{
    for(int i = 0; i < 27; i++)
    {
        mNode *mTemp = arr[i].link;
        while(mTemp)
        {
            mNode *mPrev = mTemp;
            sNode *sTemp = mPrev->sLink;
            while(sTemp)
            {
                sNode *sPrev = sTemp;
                sTemp = sTemp->subLink;
                free(sPrev);
            }

            mTemp = mTemp->mLink;
            free(mPrev);
        }
    }

    printf("[Info] : Hast Table is Successfully Freed From Memory\n");
}