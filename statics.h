#define DELETE_BUFFER 0
#define GET_TOKEN 1
#define GET_POST 2
typedef struct post{
    char * poster;
    char * post_text;
    int post_id;
    int likes;
    char ** usersWhoLiked;
    struct post * next;
}postData;
typedef struct user{
    char *userName;
    char *password;
    postData * posts_head;
    struct user* next;
}userData;