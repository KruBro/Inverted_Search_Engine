#include "main.h"

int main(int argc, char *argv[])
{
    Flist *head = NULL;

    while(read_and_validation(argc, argv, &head) == SUCCESS);
    printf("[Info] : Read And Validation Is Successfull\n");
    print_list(head);


    return 0;
}