#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

pthread_mutex_t lock;
pthread_cond_t cond;

typedef int data_t;

typedef struct node{
    data_t _data;
    struct node * _next;
}node_t,*node_p,**node_pp,*list_head;

list_head head = NULL;

static node_p buy_node(data_t data)
{
    node_p new_node = (node_p)malloc(sizeof(node_t));
    if(new_node == NULL){
        printf("%d : %s \n",errno,strerror(errno));
        exit(1);
    }
    new_node->_data = data;
    new_node->_next = NULL;
    return new_node;
}

static void delete_node(node_p node)
{
    if(node){
        free(node);
        node = NULL;
    }
}

int is_empty(list_head head)
{
    if(head && head->_next == NULL){
        return 1;
    }
    return 0;
}

void list_init(node_pp head)
{
    *head = buy_node(-1);
}

void push_head(list_head head,data_t data)
{
    if(head){
        node_p tmp = buy_node(data);

        tmp->_next = head->_next;
        head->_next = tmp;
    }
}

int pop_head(list_head head,data_t* data)
{
    if(is_empty(head)){
        return 0;
    }

    node_p tmp = head->_next;
    head->_next = head->_next->_next;
    *data = tmp->_data;
    delete_node(tmp);

    return 1;
}

void clear(list_head head)
{
    data_t tmp = 0;
    while(!is_empty(head)){
        pop_head(head,&tmp);
    }
    delete_node(head);
}

void show_list(list_head head)
{
    node_p cur = head->_next;
    while(cur){
        printf("%d",cur->_data);
        cur = cur->_next;
    }
}

void* consumer_data(void* arg)
{
    int count = 10;
    data_t data = -1;
    while(count--){
        pthread_mutex_lock(&lock);
        while(is_empty(head)){
            pthread_cond_wait(&cond,&lock);
        }
        int ret = pop_head(head,&data);
        pthread_mutex_unlock(&lock);

        if(ret == 0){
            printf("consume data failed..\n");
        }else{
            printf("comsume %d data success...\n",data);
        }
        sleep(1);
        data = -1;
    }
}

void* producter_data(void *arg)
{
    int count = 10;
    while(count--){
        pthread_mutex_lock(&lock);
        push_head(head,count);
        pthread_mutex_unlock(&lock);

        pthread_cond_signal(&cond);
        printf("data is ready,please consume!\n");
        printf("producte %d data done..\n",count);
        sleep(5);
    }
}

int main()
{
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);
    list_init(&head);

    pthread_t consumer,producter;
    pthread_create(&consumer,NULL,consumer_data,NULL);
    pthread_create(&producter,NULL,producter_data,NULL);

    pthread_join(consumer,NULL);
    pthread_join(producter,NULL);
    clear(head);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    
    return 0;
}
