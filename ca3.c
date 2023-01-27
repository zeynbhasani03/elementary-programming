
#include "functions.h"

int main(){ 
    userData user_head;
    userData **loggedInUser = (userData**)malloc(sizeof(userData*));
    *loggedInUser = NULL;
    user_head.next = NULL;
    printf("please enter your command:\n");
    while(1){
        if(check_command(&user_head,loggedInUser))
            break;
    }
    saveInFile(user_head);
    return 0;
}