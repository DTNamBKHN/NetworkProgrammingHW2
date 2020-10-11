#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> 
#include <arpa/inet.h>
typedef struct {
    char output[100];
    
} elementtype;
typedef struct node node;
typedef struct node {
	elementtype element;
	node *next;
} node;
typedef struct {
  node *root;
  node *cur;
  node *prev;
  node *tail;
} singleList;
singleList list;
//Declare function of linked list
void createSingleList(singleList *list);
node *makeNewNode(elementtype e);
node *insertEnd(singleList *list, elementtype e);
void printSingleListIP(singleList list);
void printSingleListHostname(singleList list);
//Declare function to get ip and host name
int get_official_ip(char * hostname, singleList *list) ;
int get_official_name(char *ipAddress, singleList *list);
int check_number(char *ip_part);
int ip_valid(char *ip);

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Please enter option and ip(Host name)\n");
        exit(1);
    }
    createSingleList(&list);
    int option = atoi(argv[1]);
    char *input = argv[2];
    printf("%s\n", argv[2]);
    if (option == 1){//Input is IP
        if (ip_valid(input) == 0){
            printf("Wrong parameter\n");
            exit(1);
        }
        else{
            if (get_official_name(input, &list))
                printSingleListHostname(list);
        }
    }
    else if (option == 2){//Input is hostname
        if (ip_valid(input) == 1){
            printf("Wrong parameter\n");
            exit(1);
        }
        else{
            if(get_official_ip(input, &list))
                printSingleListIP(list);
        }
    }
}

//Implement function to get ip and hostname
//Input hostname, insert to list
int get_official_ip(char * hostname, singleList *list) 
{   
    struct hostent *he;     
    struct in_addr **addr_list;        
    if ( (he = gethostbyname(hostname)) == NULL)     
    {
        printf("Not found information\n");     
        return 0;    
    }
    else{  
        elementtype element;
        addr_list = (struct in_addr **) he->h_addr_list;
        for(int i = 0; addr_list[i] != NULL; i++) {
            strcpy(element.output, inet_ntoa(*addr_list[i]));
            insertEnd(list, element);
        }
        return 1;
    }
}

// Input IP, insert to list
int get_official_name(char *ipAddress, singleList *list) {
    struct in_addr addr;
    inet_aton(ipAddress, &addr);
    struct hostent *host = gethostbyaddr(&addr, sizeof(addr), AF_INET);

    if(host == NULL) {
        printf("Not found information\n");
        return 0;
    }
    else{
        elementtype element;
        strcpy(element.output, host->h_name);
        insertEnd(list, element);
        if (host->h_aliases[0] != NULL){
            for(int i = 0; host->h_aliases[i] != NULL; i++){
                strcpy(element.output, host->h_aliases[i]);
                insertEnd(list, element);
            }
            return 1;
        }
        return 1;
    }   
}

// Check number between 0 and 256
int check_number(char *ip_part) {
    int number = atoi(ip_part);

    if(number < 0 || number > 256)
        return 0;
    return 1;
}
// Check ip address 
//1 is IP
//0 is Hostname
int ip_valid(char *ip) {
    char str[40];
    strcpy(str, ip);

    int count = 0;

    char *token = strtok(str, "."); 

    while (token != NULL) 
    { 
        if(!check_number(token))
            return 0;

        token = strtok(NULL, "."); 
        count++;
    }

    if(count != 4) 
        return 0;

    return 1;
}
//Implement function of linked list
void createSingleList(singleList *list)
{
    (*list).root = (*list).prev = (*list).cur = (*list).tail  = NULL;
}
node *makeNewNode(elementtype e)
{
    node *new = (node *)malloc(sizeof(node));
    new->element = e;
    new->next = NULL;
    return new;
}
node *insertEnd(singleList *list, elementtype e)
{
	node *new = makeNewNode(e);
	if((*list).root == NULL)
	{
        (*list).root = (*list).tail = new;
	}
	else
	{
		(*list).tail->next = new;
		(*list).tail = new;
	}
	return (*list).tail;
}
void printSingleListIP(singleList list){
    list.cur = list.root;
    printf("Official IP: %s\n", list.cur->element.output);
    list.cur = list.cur->next;
    if (list.cur != NULL){
        printf("Alias IP: \n");
    }
	while(list.cur != NULL)
    {
        printf("%s\n", list.cur->element.output);
        list.cur = list.cur->next;
    }
    return;
}
void printSingleListHostname(singleList list){
    list.cur = list.root;
    printf("Official name: %s\n", list.cur->element.output);
    list.cur = list.cur->next;
    if (list.cur != NULL){
        printf("Alias name: \n");
    }
	while(list.cur != NULL)
    {
        printf("%s\n", list.cur->element.output);
        list.cur = list.cur->next;
    }
    return;
}