//ARGC ve ARGV ne işe yarıyor bak. !!!
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
//COMMUNICATION BETWEEN PROCESS (USING PIPES) IN C 6. VIDEO

int main(){

    
    int fd[2];
    // fd[0] - read  
    // fd[1] - write
    
    if (pipe(fd) == -1){
        printf("there is a issue for pipe\n");
        return 1;
    }
    int id = fork();

    if(id == -1){
        printf("there is a issue on creating child");
    }

    if(id == 0){ //child
        close(fd[0]);
        int x;
        printf("sayi gir\n");
        scanf("%d",&x);
        if(write(fd[1], &x, sizeof(int)) == -1){
            printf("there is a issue for write\n");
            return 2;
        }
        close(fd[1]);
    }
    else{
        close(fd[1]);
        int y;
        if(read(fd[0], &y, sizeof(int)) == -1){ //burada write ile pipe'a veri yazılmadığı sürece beklenilmeye devam edilir.!!!!!!
            printf("there is a issue for read\n");
            return 3;
        }
        close(fd[0]);
        printf("%d\n",y);
    }
    if(wait(NULL) != -1 || errno != ECHILD);
}

    
























/*
int main(){
    int id = fork();
    int id1 = fork();


    if (id != 0){
        if(id1 != 0){
            printf("you are in parent\n");
        }
        else{
            printf("you are in child id2\n");
        }
}
else {
    if(id1 != 0){
        printf("you are in child id\n");
    }
    else{
        printf("you are in child in child\n");
    }
}
while (wait(NULL) != -1 || errno != ECHILD){
    printf("waiting child\n");
    //ÇOCUK SÜREÇ BİTTİĞİNDE wait(NULL) -1 döndürür ve errnoyu ECHILD olarak ayarlar bu sayede
    //döngüden çıkılır !!!!!!!!! eğer çocuk process devam ediyorsa PIDsini döndürür
    //Personel ID
}

}

*/

/*
int main(){
    int id = fork();
    int n;
    if(id == 0){
        n = 1;
    }
    else{
        n = 6;
    }
    if(id != 0){

        wait(NULL);
    }
    int i;
    for(i = n; i < n + 5; i++){
        printf("%d ", i);
        fflush(stdout);
    }

    if(id != 0){

        printf("\n");
    }

}
    */