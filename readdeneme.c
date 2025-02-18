#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
// fifo ile okuma yazmayı yapıyor.
#define ARR_SIZE 5

int main(){
    int sum = 0;
    int fd = open("sum", O_RDONLY);
    int arr[ARR_SIZE] = {0};

    for(int i = 0; i < ARR_SIZE; i++){
        if(read(fd, &arr[i], sizeof(int))== -1){

            printf("non read");
        }
    }
    for(int i = 0; i < ARR_SIZE; i++){
        printf("%d\n",arr[i]);
    }
    close(fd);
    fd = open("sum", O_WRONLY);
    for(int i = 0; i < ARR_SIZE; i++){
        sum += arr[i];
    }
    write(fd, &sum, sizeof(sum));
    close(fd);
    
}