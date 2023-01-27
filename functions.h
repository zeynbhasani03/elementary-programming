#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statics.h"

char * stralloc(int delOrGetTok){
    char c;
    int i=1;
    char *str;
    str=(char *)malloc(sizeof(char));
    c=getchar();
    str[0]=c;
    while ((c=getchar())!='\n'&& (c!=' ' || delOrGetTok == DELETE_BUFFER ||delOrGetTok == GET_POST )){
        if(delOrGetTok== GET_TOKEN||delOrGetTok==GET_POST){
            str=(char *)realloc(str,(sizeof(char))*(++i));
            str[i-1]=c;   
        }        
    }    
    str=(char *)realloc(str,(sizeof(char))*(++i));
    str[i-1]='\0';
    return str;    
}

void insert_account(userData * user_head,userData *newUser){ 
    userData *currUser;
    currUser=user_head;
    while(currUser->next != NULL){
        currUser=currUser->next;
    }
    currUser->next=newUser;
}
void insert_post(userData **loggedInUser,postData * newPost){
    postData * current=(*loggedInUser)->posts_head;
    int i=1;
    while (current->next!=NULL){
        current=current->next;
        i++;
    }
    current->next=newPost;
    newPost->post_id=i;
}
postData * make_post(char * postText,userData ** loggedInUser){
    postData * newPost=(postData *)malloc(sizeof(postData));
    newPost->post_text=postText;
    newPost->poster=(*loggedInUser)->userName;
    newPost->post_id=0;
    newPost->likes=0;
    newPost->usersWhoLiked=(char **)malloc(0);
    newPost->next=NULL;
    return newPost;
}
userData *makeAccount(char *username){
    userData *newUser;
    newUser =malloc(sizeof(userData));
    newUser->userName=username;
    newUser->password=stralloc(GET_TOKEN);
    newUser->posts_head=(postData *)malloc(sizeof(postData));
    newUser->posts_head->next=NULL;    
    newUser->next=NULL;
    return newUser;
}

userData * search_user(userData * user_head, char * username){
    userData *current=user_head->next;
    while(current!=NULL){
        if (!strcmp(current->userName,username)){
            return current;
        } 
        current=current->next;
    }
    return NULL;
}

postData * search_post(userData *user_head,char *username,int idToFind,postData **prev){
    userData *postOwner=search_user(user_head,username);
    *prev = postOwner->posts_head;
    postData *current;
    while ((*prev)->next!=NULL){
        if ((*prev)->next->post_id==idToFind){
            current=(*prev)->next;
            return current;
        }
        *prev=(*prev)->next;   
    }
    return NULL;
}

void saveInFile(userData user_head){
    FILE *accountsFile=fopen("Accounts.txt","w");
    if (accountsFile!=NULL){
        userData *currAccount=(&user_head)->next;
        while(currAccount!=NULL){
            postData * current=currAccount->posts_head->next;
            int counter=0;
        while (current!=NULL){
            counter++;
            current=current->next;
        }         
        fprintf(accountsFile,"%s %s %d\n",currAccount->userName,currAccount->password,counter);
        currAccount=currAccount->next;
        }
    }
    else
        printf("Unable to open a file!");
    fclose(accountsFile);
    FILE *postsFile=fopen("Posts.txt","w");
    if (postsFile!=NULL){
        userData *currAccount=(&user_head)->next;
        while(currAccount!=NULL){
            postData *currPost=currAccount->posts_head->next;
            while(currPost!=NULL){    
            fprintf(postsFile,"%s %s %d\n",currPost->post_text,currPost->poster,currPost->likes);
            currPost=currPost->next;
            }
            currAccount=currAccount->next;
        }
    }
    else
        printf("Unable to open a file!");
    fclose(postsFile);
    getchar();
}



int check_command(userData* user_head,userData ** loggedInUser){
    char *command=stralloc(GET_TOKEN);
    if(!strcmp(command,"signup")){
        if(*loggedInUser==NULL){
            char* userName=stralloc(GET_TOKEN);
            if (!search_user(user_head,userName)){

                 insert_account(user_head,makeAccount(userName));
                 printf("you're signed up successfully!\n");
            }
            else{ 
                printf("this account already exists\n");        
                stralloc(DELETE_BUFFER);//to free password remaining in buffer
            }
        }
        else{
            stralloc(DELETE_BUFFER);
            printf("you must be logged out to be able to sign up1\n");
        }
    }       
    else if (!strcmp(command,"login")){
        if(*loggedInUser==NULL){
            userData * temp=search_user(user_head,stralloc(GET_TOKEN));
            if (temp==NULL){
                stralloc(DELETE_BUFFER);
                printf("this account does not exist, you dumbass!\n");
            }
            else{
                char * password=stralloc(GET_TOKEN);
                if (!strcmp(password,temp->password)){
                    printf("you're logged in succesfully!\n");
                    *loggedInUser=temp;
                }
                else{
                    printf("wrong password\n");
                }
            }
        }
        else{
            printf("you'tr already logged in!\n");
            stralloc(DELETE_BUFFER);
        }
    }
    else if (!strcmp(command,"logout")){
        if(*loggedInUser!=NULL){
            *loggedInUser=NULL;
            printf("you're logged out successfully!\n");
        }
        else{
            printf("you were not logged in, hence you cant log out!\n");
        }        
    }
    else if (!strcmp(command,"post")){
        if (*loggedInUser!=NULL){
            char *postText=stralloc(GET_POST);
            insert_post(loggedInUser,make_post(postText,loggedInUser));
            printf("you created a post successfully!\n");
        }
        else{
            stralloc(DELETE_BUFFER);
            printf("you,re not logged in therefor you cannot create a post!\n");
        }        
    }
    else if (!strcmp(command,"like")){
        if (*loggedInUser!=NULL){
            char* userWhoLiked = stralloc(GET_TOKEN);
            postData **prev;
            prev=(postData**)malloc(sizeof(postData*));
            postData *postToLike=search_post(user_head,userWhoLiked,atoi(stralloc(GET_TOKEN)),prev);
            int isLiked=0;
            for (int i = 0; i <(postToLike->likes) ; i++){
                if(!strcmp((*loggedInUser)->userName,postToLike->usersWhoLiked[i])){
                    isLiked=1;
                    printf("you have already liked this post,not able to like again!\n");
                    break;
                }                
            }
            if(!isLiked){
                    (postToLike->likes)++;
                    postToLike->usersWhoLiked=(char **)realloc(postToLike->usersWhoLiked,sizeof((postToLike->likes)*sizeof(char *)));
                    postToLike->usersWhoLiked[postToLike->likes-1]= (*loggedInUser)->userName;
                    printf("you liked this post!\n");
            }
        }        
        
    }
    else if (!strcmp(command,"delete")) {
        if (*loggedInUser!=NULL){
            int idToDel=atoi(stralloc(GET_TOKEN));
            postData **prev;
            prev=(postData**)malloc(sizeof(postData*));
            postData *curr=search_post(user_head,(*loggedInUser)->userName,idToDel,prev);
            (*prev)->next =curr->next;
            free(curr);
            printf("the post was deleted successfully!\n");
        }
        else{
        printf("you're not logged in therefor you can't delete post!\n ");
        stralloc(DELETE_BUFFER);
        }        
    }
    else if (!strcmp(command,"info")){
        if (*loggedInUser!=NULL){
            printf("username is:%s\npassword is:%s\n",(*loggedInUser)->userName,(*loggedInUser)->password);
            postData * a=(*loggedInUser)->posts_head->next;
            for (; a!=NULL; a=a->next){
                printf("post id is:%d\n post text is:%s\n,number of likes is:%d\n\n",a->post_id,a->post_text,a->likes);
            }    
        }
        else{
            printf("you're not logged in therefor you can't watch info!");
            stralloc(DELETE_BUFFER);
        }
    }
    else if (!strcmp(command,"find_user")){
        if (*loggedInUser!=NULL){
            userData * wannaFindUser=search_user(user_head,stralloc(GET_TOKEN));
            if (wannaFindUser==NULL){
                printf("user was not found!\n");
            }
            else{
                printf("username is:%s\n",wannaFindUser->userName);
                postData * a=wannaFindUser->posts_head->next;
                for (; a!=NULL; a=a->next){
                    printf("post id is:%d\npost %d text is:%s\n",a->post_id,a->post_id,a->post_text);
                }    
            }
        }  
        else{
        printf("you're not logged in therefor you can't find a user!\n");
        stralloc(DELETE_BUFFER);
        }
    }    
    else if(!strcmp(command,"exit"))
        return 1;
    else{
        printf("invalid command\n");
        stralloc(DELETE_BUFFER);//to free password remaining in buffer
    }
    free(command);
    return 0;
}
