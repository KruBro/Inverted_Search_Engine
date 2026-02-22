#include "main.h"

Status save_database(hash_T *arr)
{
    // Pro-tip: Saving as .md (Markdown) often triggers even better colors!
    FILE *fp = fopen("database.txt", "w"); 
    if (fp == NULL)
    {
        perror("File Could Not Open");
        return FAILURE;
    }

    fprintf(fp, "____________________________________________________________________________________________________\n");
    // Use only pipes. Most editors will highlight these as 'delimiters'
    fprintf(fp, "| %-10s | %-15s | %-10s | %-10s | %-40s |\n", 
            "Index", "Word", "FileCount", "WordCount", "Filenames");
    

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

            // The 'vibrant' part: the editor will likely color the text between | bars
            fprintf(fp, "| %-10d | %-15s | %-10u | %-10u | %-40s |\n", 
                   i, mTemp->word, mTemp->filecount, total_word_count, all_files);

            mTemp = mTemp->mLink;
        }
    }

    fprintf(fp, "____________________________________________________________________________________________________\n");
    fclose(fp);
    return SUCCESS;
}