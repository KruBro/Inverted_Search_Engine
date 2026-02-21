#include "main.h"

//Implementation of Database
Status create_database(hash_T *arr, Flist *head)
{
    Flist *temp = head;
    FILE *fp;

    while(temp)
    {
        //Open the File One By One
        fp = fopen(temp->file_name, "r");
        char input_word[20];

        if(fp == NULL)
        {
            printf("[Error] : File Could Not Open\n");
            return FAILURE;
        }
        printf("[Info] : %s Opened Successfully\n", temp->file_name);

        //Read word by word from the file
        while(fscanf(fp, "%s", input_word) != EOF)
        {
            //Find the index
            int index;
            //If Lower Case
            if(islower(input_word[0]))
                index = input_word[0] - 97;
            //If Upper Case
            else if(isupper(input_word[0]))
                index = input_word[0] - 65;
            //If Non Alphabets Character
            else
            {
                index = 26;
            }

            //Check if the index link is null or not
            if(arr[index].link == NULL)
            {
                //Create MainNode
                mNode *new_mainNode = malloc(sizeof(mNode));
                if(new_mainNode == NULL)
                    return FAILURE;     //Safety Check
                //Create SubNode
                sNode *new_subNode = malloc(sizeof(sNode));
                if(new_subNode == NULL)
                {
                    free(new_mainNode);
                    return FAILURE;     //Safety Check
                }     //Safety Check
                
                //Update The nodes and counts
                new_mainNode->filecount = 1;
                strcpy(new_mainNode->word, input_word);
                new_mainNode->sLink = new_subNode;
                new_mainNode->mLink = NULL;

                strcpy(new_subNode->file_name, temp->file_name);
                new_subNode->wordcount = 1;
                new_subNode->subLink = NULL;

                arr[index].link = new_mainNode; 
            }
            else
            {
                //Take two pointers to traverse
                mNode *mTemp = arr[index].link, *mPrev= NULL;
                while(mTemp)
                {
                    //If word is already present in the table
                    if(strcmp(mTemp->word, input_word) == 0)
                    {
                        //If it is in same file;
                        //Take Two Pointers to Traverse
                        sNode *sTemp = mTemp->sLink, *sPrev = NULL;
                        while(sTemp)
                        {
                            if(strcmp(sTemp->file_name, temp->file_name) == 0)
                            {
                                (sTemp->wordcount)++;
                                break;
                            }
                            else
                            {
                                sPrev = sTemp;
                                sTemp = sTemp->subLink;
                            }
                        }

                        //If it is in the different File
                        if(sTemp == NULL)
                        {
                            sNode *new_subNode = malloc(sizeof(sNode));
                            if(new_subNode == NULL)
                                return FAILURE;      //Safety Check
                        
                             //Update the Nodes
                            new_subNode->wordcount = 1;
                            strcpy(new_subNode->file_name, temp->file_name);
                            new_subNode->subLink = NULL;

                            sPrev->subLink = new_subNode;
                            (mTemp->filecount)++;

                            break;
                        }
                    }
                    else
                    {
                        mPrev = mTemp;
                        mTemp = mTemp->mLink;
                    }
                }

                if(mTemp == NULL)
                {
                    //If word is different
                    // Create Mainnode
                    mNode *new_mainNode = malloc(sizeof(mNode));
                    if(new_mainNode == NULL)
                        return FAILURE;     //Safety Check
                    //Create SubNode
                    sNode *new_subNode = malloc(sizeof(sNode));
                    if(new_subNode == NULL)
                    {
                        free(new_mainNode);
                        return FAILURE;     //Safety Check
                    }
                
                    //Update the Nodes and Counts
                    (new_mainNode->filecount) = 1;
                    strcpy(new_mainNode->word, input_word);
                    new_mainNode->sLink = new_subNode;
                    new_mainNode->mLink = NULL;

                    strcpy(new_subNode->file_name, temp->file_name);
                    (new_subNode->wordcount) = 1;
                    new_subNode->subLink = NULL;

                    mPrev->mLink = new_mainNode;
                }

            }

        }
        //Move the node to the next node
        temp = temp->link;
    }

    return SUCCESS;
}