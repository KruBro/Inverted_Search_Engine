/**
 * @file   main.c
 * @brief  Entry point and interactive menu loop for the Inverted Search Engine.
 *
 * Flow:
 *   1. Validate command-line arguments (at least one .txt file required).
 *   2. Validate and load each file into the Flist.
 *   3. Initialize the 27-bucket hash table.
 *   4. Enter the menu loop — user drives all operations from here.
 *   5. On exit: auto-save, free all heap memory, and return.
 */

#include "main.h"

Status main(int argc, char *argv[])
{
    int choice;

    /* ── Argument check ── */
    if(argc < 2)
    {
        printf(H_RED "[Info] : Not Enough Arguments\n" RESET);
        printf(H_YELLOW "[Usage] : %s <file.txt> [<file1.txt> ...]\n" RESET, argv[0]);
        return 1;
    }

    Flist *head = NULL;

    /* ── Validate and load each file argument into the Flist ── */
    argc -= 1;  /* Subtract argv[0] (program name) */
    for(int i = 1; i <= argc; i++)
    {
        int ret = read_and_validation(argv, i, &head);
        if(ret == SUCCESS)
            printf(BOLD_GREEN "[Info] : Read And Validation of [%s] Is Successfull Completed\n" RESET, argv[i]);
        else
            printf(BOLD_RED "[Info] : Read And Validation of [%s] Is Failed\n" RESET, argv[i]);
    }
    print_list(head);

    /* ── Initialize the hash table ── */
    hash_T hash_t[27];
    initialize_hashTable(hash_t);

    /* ── Menu loop ── */
    while(1)
    {
        static char *menu[] = {
            BOLD_CYAN "1. Create Database"  RESET,
            BOLD_CYAN "2. Display Database" RESET,
            BOLD_CYAN "3. Search Database"  RESET,
            BOLD_CYAN "4. Update Database"  RESET,
            BOLD_CYAN "5. Save Database"    RESET,
            BOLD_RED  "6. Exit"             RESET
        };
        for(int i = 0; i < 6; i++) { printf("%s\n", menu[i]); }
        printf(GREEN "Enter the Choice : " RESET);
        scanf("%d", &choice);

        switch(choice)
        {
            /* ── 1. Index all files in the Flist ── */
            case 1:
            {
                if(create_database(hash_t, head) == SUCCESS)
                    printf(BOLD_BLUE "[Info] : Database has been created / Updated Successfully\n" RESET);
                else
                    printf(BOLD_RED "[Error] : An Error has Occured in Dynamic Memory Allocation\n" RESET);
                printf("\n");
                break;
            }

            /* ── 2. Print the full index as a colored table ── */
            case 2:
            {
                display_database(hash_t);
                printf("\n");
                break;
            }

            /* ── 3. Look up a specific word ── */
            case 3:
            {
                char keyword[20];
                printf(H_CYAN "Enter the word you want to search : " RESET);
                scanf("%s", keyword);

                if(search_database(hash_t, keyword) == DATA_NOT_FOUND)
                    printf(H_MAGENTA "[Info] : %s is not found in the database\n" RESET, keyword);

                printf("\n");
                break;
            }

            /* ── 4. Add new files to the existing index ── */
            case 4:
            {
                unsigned int fileCount;
                printf(BLUE "Enter how many files you want to update to the database : " RESET);
                scanf("%u", &fileCount);

                /* Collect filenames into a VLA of strdup'd strings */
                char *fileHolder[fileCount];
                for(int i = 0; i < fileCount; i++)
                {
                    char tempFileName[20];
                    printf(H_YELLOW "Enter the File Name : " RESET);
                    scanf("%s", tempFileName);
                    fileHolder[i] = strdup(tempFileName);
                }

                printf(H_GREEN "[Info] : Files have been loaded successfully\n" RESET);
                for(int i = 0; i < fileCount; i++)
                    printf("%s\n", fileHolder[i]);

                /* Pass &head so the Flist updates are visible after return */
                update_database(hash_t, &head, fileHolder, fileCount);

                /* Free only the strdup'd strings — fileHolder itself is on the stack */
                for(int i = 0; i < fileCount; i++)
                    free(fileHolder[i]);

                break;
            }

            /* ── 5. Export the index to database.txt ── */
            case 5:
            {
                if(save_database(hash_t) == SUCCESS)
                    printf(H_GREEN "[Info] : Database has been saved Successfully\n" RESET);
                else
                    printf(H_RED "[Error] : Error Occured While Saving the database\n" RESET);
                break;
            }

            /* ── 6. Auto-save, free all memory, and exit ── */
            case 6:
            {
                save_database(hash_t);
                free_hash_table(hash_t);
                free_list(&head);
                printf(H_CYAN "Program Exited Successfully\n" RESET);
                return SUCCESS;
            }

            default:
            {
                printf(H_RED "Invalid Choice\n" RESET);
            }
        }
    }

    return FAILURE;
}