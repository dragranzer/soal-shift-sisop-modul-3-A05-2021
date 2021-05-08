#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

struct shared{
    int data[4][6];
    int batas[4][6];
};
struct one_shared{
    int num;
    int limit;
};
int row=0;
void* factorial(void* arg){
	struct one_shared a = *((struct one_shared*)arg);
	free(arg);
	int total = 1;
    int b = a.num;
    int end = a.limit;
    while(end > 0){
        total *= b;
        b--;
        end--;
    }

	if(row >= 6){
		row = 0;
	}
	printf("%d ", total);
	row++;
}



int result[4][6];

void main(){
    key_t key = 1244;
    int shmid = shmget(key,sizeof(struct shared),0666);
    struct shared *shmptr;
    shmptr = (struct shared *) shmat(shmid, NULL, 0);

    memcpy(result, &shmptr->data, 24 * sizeof(int));
    int i,j,k;
    printf("Hasil Perkalian matrix dari 2a:\n");
    for(i=0;i<4;i++){
        for(j=0;j<6;j++){
            printf("%d ",result[i][j]);
        }
        printf("\n");
    }
    int matrixb[4][6],num;
    printf("input matrix a yang merupakan batasan faktorial:\n");
    for(i=0;i<4;i++){
        for(j=0;j<6;j++){
            scanf("%d",&shmptr->batas[i][j]);
        }
    }
    // for(i=0;i<4;i++){
    //     for(j=0;j<6;j++){
    //         printf("%d ",shmptr->batas[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("Jumlah faktorial dari variabel matriks C : \n");
    pthread_t tid[4][6];
	for(int i = 0; i < 4;i++){
        for(int j=0;j<6;j++){
            struct one_shared *x =  malloc(sizeof(*x));
            if( x == NULL){
                printf("TIDAK BISA MEMBUAT RUANG\n");
                exit(1);
            }

            x->num = shmptr->data[i][j];
            x->limit = shmptr->batas[i][j];
            pthread_create(&(tid[i][j]), NULL, &factorial, x);
            pthread_join(tid[i][j], NULL);
        }	
        printf("\n");
	}
}
