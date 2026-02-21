#include "main.h"

int main(int argc, char *argv[])
{
    int choice;

    if(argc < 2)
    {
        printf("[Info] : Not Enough Arguments\n");
        printf("[Usage] : %s <file.txt> [<file1.txt> <file1.txt>.........]\n", argv[0]);
        return 1;
    }

    Flist *head = NULL;

    //Skip the first argument [a.out]
    argc -= 1;

    for(int i = 1; i <= argc; i++)
    {
        int ret = read_and_validation(argv, i, &head);
        if(ret == SUCCESS)
            printf("[Info] : Read And Validation of [%s] Is Successfull Completed\n", argv[i]);
        else if(ret == FAILURE)
            printf("[Info] : Read And Validation of [%s] Is Failed\n", argv[i]);    
    }
    print_list(head);

    hash_T hash_t[27];
    initialize_hashTable(hash_t);

    while(1)
    {
        static char *menu[] = {"1. Create Database", "2. Display Database","3. Search Database", "4. Update Database", "5. Save Database", "6. Exit"};
        for(int i = 0; i < 6; i++)  {printf("%s", menu[i]); printf("\n");}
        printf("Enter the Choice : ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1: 
                if(create_database(hash_t, head) == SUCCESS)
                {
                    printf("[Info] : Database has been created Successfully\n");
                }
                else
                {
                    printf("[Error] : An Error has Occured in Dynamic Memory Allocation\n");
                }
                break;
            case 2:
                display_database(hash_t);
                break;
            case 3:
                //Search Database();
                printf("Yet To Implemented\n");
                break;
            case 4:
                //Search Database();
                printf("Yet To Implemented\n");
                break;
            case 5:
                //Search Database();
                printf("Yet To Implemented\n");
                break;
            case 6:
                free_hash_table(hash_t);
                free_list(&head);
                printf("Program Exited Succefully\n");
                return 0;
            default:
                printf("Invalid Choice\n"); 
        }

    }
    return 1;
}