#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<stdbool.h>
#include<sys/shm.h>
#include<limits.h>
#include <pthread.h>

#define SHM_KEY_CONNECT 0x1000
int *con_channel_ptr;
int *uniqueID; 
int uniqueID_index = 0;
int client_count = 0;
key_t key = 1000;
int total_requests = 0;

int checkprime(int n)
{
    for(int i=2;i<n;i++)
    {
        if((n%i)==0)
        {
            return 0;
        }
    }
    return 1;
}

void* comm_channel_handler(void* arg){
    int comm_channel_id = shmget(key, 4*sizeof(int), 0666 | IPC_CREAT);
    if(comm_channel_id<0)
    {
        printf("Error in Creating Shared Memory : \n");
        exit(0);
    }
    int *comm_channel_ptr = shmat(comm_channel_id, NULL, 0);
    if(*comm_channel_ptr==-1)
    {
        printf("Error in Attaching Shared Memory to Server : \n");
        exit(0);
    }
    int client_id = con_channel_ptr[0];
    con_channel_ptr[0] = INT_MIN;
    while(con_channel_ptr[1] != INT_MIN){
        continue;
    }
    con_channel_ptr[1] = key;
    key++;
    
    int listen = 1;
    int request_count = 0;
    while(listen){
        while(comm_channel_ptr[0] == INT_MIN){
            continue;
        }
        request_count++;
        total_requests++;
        int request = comm_channel_ptr[0];
        int operand1 = comm_channel_ptr[1];
        int operand2 = comm_channel_ptr[2];
        int result;
        
        switch(request)
        {
            case 1 : 
            {
                printf("\nWorker process initiated successfully!\n");
                result = operand1 + operand2;
            }
                break;
            case 2 : 
            {
                printf("\nWorker process initiated successfully!\n");
                result = operand1 - operand2;
            }
                break;
            case 3 : 
            {
                printf("\nWorker process initiated successfully! \n");
                result = operand1 * operand2;
            }
                break;
            case 4 : 
            {
                printf("\nWorker process initiated successfully! \n");
                result = operand1 / operand2;
            }
                break;
            case 5 : 
            {
                printf("\nWorker process initiated successfully! \n");
                result = checkprime(operand1);
            }
                break;
            case 6 : 
            {
                printf("\nWorker process initiated successfully! \n");
                if(operand1%2==0){
                    result = 0;
                }
                else{
                    result = 1;
                }
            }
                break;
            case 7 : 
            {
                result = 0;
                listen = 0;
                client_count--;
                printf("Client %d Unregistered \n", client_id);
                printf("Total no of requests made by client %d is %d \n", client_id, request_count);
            }
                break;
        }
        
        comm_channel_ptr[3] = result;
    }
    
}

int main(){
    uniqueID = (int *)calloc(100*sizeof(int), 0);
    int con_channel_id = shmget(SHM_KEY_CONNECT, 2*sizeof(int), 0666 | IPC_CREAT);
    if(con_channel_id<0)
    {
        printf("Error in Creating Shared Memory : \n");
        exit(0);
    }
    con_channel_ptr = shmat(con_channel_id, NULL, 0);
    if(*con_channel_ptr==-1)
    {
        printf("Error in Attaching Shared Memory to Server : \n");
        exit(0);
    }
    
    for(int i=0; i<2; i++){
        con_channel_ptr[i] = INT_MIN;
    }
    
    pthread_t threads[100];
    
    while(1){
        if(con_channel_ptr[0] == INT_MIN){
            continue;
        }
        else{
            int userId = con_channel_ptr[0];
            for(int i=0; i<=uniqueID_index; i++){
                if(uniqueID[i] == userId){
                    con_channel_ptr[0] = INT_MIN;
                    printf("Client not Unique : \n ");
                    continue;
                }
            }
        }
        uniqueID[uniqueID_index] = con_channel_ptr[0];
        printf("New Client Added \n");
        client_count++;
        printf("Total number of Clients: %d", client_count);
        pthread_create(&threads[uniqueID_index], NULL, &comm_channel_handler, NULL);
        uniqueID_index++;
    }
}



















