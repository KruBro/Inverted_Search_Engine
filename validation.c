#include "main.h"


Status read_and_validation(char *argv[], int i, Flist **head)
{
    FILE *fp;
    //check argv[i] is .txt extension or not
    if(strstr(argv[i], ".txt") != NULL)
    {
        //check if the file is present or not
        fp = fopen(argv[i], "r");
        if(fp == NULL)
        {
            printf("[Info] : %s Could Not Open / Not Found\n", argv[i]);
            return FAILURE;
        }
        printf("[Info] : %s File Opened Successfully\n", argv[i]);

        //Check the file is empty or not
        fseek(fp, 0, SEEK_END);
        if(ftell(fp) == 0)
        {
            printf("%s file is empty\n", argv[i]);
            return FAILURE;
        }
        printf("[Info] : File is Not Empty, Adding Operationg Started...\n");

        if(insert_at_last(head, argv[i]) == FAILURE)
        {
            printf("[Info] : %s's Duplicate Found\n", argv[i]);
            return FAILURE;
        }

        printf("[Info] : %s File Successfully Added\n", argv[i]);
    }
    else
    {
        printf("[Info] : %s is not a .txt file\n", argv[i]);
        return FAILURE;
    }

    fclose(fp);
    return SUCCESS;
}

Status compare(Flist *head, char *fname)
{
    //If list is empty
    if(head == NULL)
        return LIST_EMPTY;
    
    Flist *temp = head;
    //Compare the list one by one
    while(temp)
    {
        Flist *temp1 = temp->link;
        while(temp1)
        {
            if(strcmp(temp->file_name, fname) == 0)
                return DUPLICATE;
            temp1 = temp1->link;
        }
        temp = temp->link;
    }
    
    return SUCCESS;
}