#include "main.h"

void display_database(hash_T *arr)
{
    // 1. Top Border
    printf("-----------------------------------------------------------------------\n");
    printf("| %-5s | %-15s | %-10s | %-15s | %-10s |\n", "Index", "Word", "File Count", "File Name", "Word Count");
    printf("-----------------------------------------------------------------------\n");

    for(int i = 0; i < 27; i++)
    {
        mNode *mTemp = arr[i].link;
        
        while(mTemp)
        {
            sNode *sTemp = mTemp->sLink;
            int first_file = 1;

            while(sTemp)
            {
                if(first_file)
                {
                    // Print full row for the first file occurrence of a word
                    printf("| %-5d | %-15s | %-10u | %-15s | %-10u |\n", 
                            i, mTemp->word, mTemp->filecount, sTemp->file_name, sTemp->wordcount);
                }
                else
                {
                    // Print only sub-node info, leave word info blank for clarity
                    printf("| %-5s | %-15s | %-10s | %-15s | %-10u |\n", 
                            "", "", "", sTemp->file_name, sTemp->wordcount);
                }
                
                sTemp = sTemp->subLink;
                first_file = 0;
            }
            
            // 2. Draw a line after each Word (mNode) group
            printf("-----------------------------------------------------------------------\n");
            mTemp = mTemp->mLink;
        }
    }
}
