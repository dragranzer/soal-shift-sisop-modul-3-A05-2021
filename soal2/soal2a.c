#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int matrix1[4][3] = {{1, 1, 1}, {2, 2, 2}, {1, 1, 1}, {1, 2, 1}};
int matrix2[3][6] = {{1, 2, 1, 2, 1, 1}, {1, 2, 2, 1, 1, 2}, {1, 2, 1, 3, 1, 3}};
int result[4][6];

void phrase_2a()
{
    int i, j, k;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 6; j++){
            result[i][j] = 0;
            for(k = 0; k < 3; k++){
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

struct shared{
    int data[4][6];
};

void main(){
    phrase_2a();
    int i,j,k;
    for(i=0;i<4;i++){
        for(j=0;j<6;j++){
            printf("%d ",result[i][j]);
        }
        printf("\n");
    }
    key_t key = 1244;
    int shmid = shmget(key,sizeof(struct shared),IPC_CREAT|0666);
    struct shared *shmptr;
    shmptr = (struct shared *) shmat(shmid, NULL, 0);

    for(i=0;i<4;i++){
        for(j=0;j<6;j++){
            shmptr->data[i][j] = result[i][j];
        }
        //printf("\n");
    }
    
    sleep(10);
    shmdt((void *) shmptr);
    shmctl(shmid, IPC_RMID, NULL);  
}
