#include "main.h"

void display_database(hash_T *arr)
{
    // Top Border
    printf(H_CYAN "+------------+-----------------+------------+------------+------------------------------------------+\n" RESET);

    // Header Row - Macros must be placed directly inside or adjacent to string literals
    printf(H_CYAN "|" RESET BG_BLUE BOLD_WHITE " %-10s " RESET 
           H_CYAN "|" RESET BG_BLUE BOLD_WHITE " %-15s " RESET 
           H_CYAN "|" RESET BG_BLUE BOLD_WHITE " %-10s " RESET 
           H_CYAN "|" RESET BG_BLUE BOLD_WHITE " %-10s " RESET 
           H_CYAN "|" RESET BG_BLUE BOLD_WHITE " %-40s " RESET 
           H_CYAN "|\n" RESET, 
           "Index", "Word", "FileCount", "WordCount", "Filenames");

    // Header-Separator
    printf(H_CYAN "+------------+-----------------+------------+------------+------------------------------------------+\n" RESET);

    for (int i = 0; i < 27; i++)
    {
        mNode *mTemp = arr[i].link;
        while (mTemp)
        {
            char all_files[1024] = ""; 
            u_int total_word_count = 0;
            sNode *sTemp = mTemp->sLink;

            while (sTemp)
            {
                total_word_count += sTemp->wordcount;
                strcat(all_files, sTemp->file_name);
                if (sTemp->subLink) strcat(all_files, ", ");
                sTemp = sTemp->subLink;
            }

            // Data Row
            printf(H_CYAN "|" RESET " " H_YELLOW "%-10d" RESET 
                   H_CYAN " |" RESET " " H_GREEN "%-15s" RESET 
                   H_CYAN " |" RESET " %-10u " 
                   H_CYAN "|" RESET " %-10u " 
                   H_CYAN "|" RESET " " H_MAGENTA "%-40s" RESET 
                   H_CYAN "|\n" RESET, 
                   i, mTemp->word, mTemp->filecount, total_word_count, all_files);

            mTemp = mTemp->mLink;
        }
    }
    printf(H_CYAN "+------------+-----------------+------------+------------+------------------------------------------+\n" RESET);
}