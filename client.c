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
int *comm_channel_ptr;

int main(){
    int con_channel_id = shmget(SHM_KEY_CONNECT, 2*sizeof(int), 0666 | IPC_CREAT);
    if(con_channel_id<0)
    {
        printf("Error in Creating Shared Memory : \n");
        exit(0);
    }
    con_channel_ptr = shmat(con_channel_id, NULL, 0);
    if(*con_channel_ptr==-1)
    {
        printf("Error in Attaching Shared Memory to Client : \n");
        exit(0);
    }
    
    int userId;
    
    printf("Enter User ID: ");
    scanf("%d", &userId);
    
    while(con_channel_ptr[0] != INT_MIN){
        continue;
    }
    
    con_channel_ptr[0] = userId;

    while(con_channel_ptr[1] == INT_MIN){
        continue;
    }
    
    key_t connect_key = con_channel_ptr[1];
    con_channel_ptr[1] = INT_MIN;
    int comm_channel_id =  shmget(connect_key, 4*sizeof(int), 0666 | IPC_CREAT);
    if(comm_channel_id<0)
    {
        printf("Error in Creating Shared Memory : \n");
        exit(0);
    }
    comm_channel_ptr = shmat(comm_channel_id, NULL, 0);
    if(*comm_channel_ptr == -1)
    {
        printf("Error in Attaching Shared Memory to Client : /n");
        exit(0);
    }
    
    
    
    int con = 1;
    while(con){
        for(int i=0; i<4; i++){
            comm_channel_ptr[i] = INT_MIN;
        }
        
        int request, operand1, operand2;
        printf("The Following Operations are : \n");
        printf("Press 1 for Addition of 2 numbers \n");
        printf("Press 2 for Subtraction of 2 numbers : \n");
        printf("Press 3 for Multiplication of 2 numbers : \n");
        printf("Press 4 for Division of 2 numbers : \n");
        printf("Press 5 for checking if the number is Prime or not : \n");
        printf("Press 6 for checking if number is even or odd: \n"); 
        printf("Press 7 to disconnect from the server");
        
        printf("Enter the Request : \n");
        scanf("%d", &request);
        
        if(request>=1 && request<=4){
            printf("Enter the first and second number  : ");
            scanf("%d %d", &operand1, &operand2);
            printf("\n");
        }
        else if(request == 5 || request == 6) {
            printf("Enter the number  : ");
            scanf("%d", &operand1);
            operand2 = INT_MIN;
            printf("\n");
        }
        else if(request == 7){
            //disconnect
            operand1 = INT_MIN;
            operand2 = INT_MIN;
            con = 0;
        }
        
        comm_channel_ptr[0] = request;
        comm_channel_ptr[1] = operand1;
        comm_channel_ptr[2] = operand2;
        
        while(comm_channel_ptr[3] == INT_MIN){
            continue;
        }
        
        int result = comm_channel_ptr[3];
        
        if(request>=1 && request <=4){
            printf("Result is %d", result);
        }
        else if(request == 5){
            if(result == 1){
                printf("%d is Prime", operand1);
            }
            else{
                printf("%d is not Prime", operand1);
            }
        }
        else if(request == 6){
            if(result == 1){
                printf("%d is Odd", operand1);
            }
            else{
                printf("%d is Even", operand1);
            }
        }
        else if(request == 7){
            printf("Disconnecting\n");
        }
    }
    
    printf("Client %d disconnected", userId);
    return 0;
}

















