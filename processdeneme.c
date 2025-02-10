
// 9. videoda basic bir şeyi gösteriyor kodu yazılmadı
//  ARGC ve ARGV ne işe yarıyor bak. !!!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

























/*
#define ARR_SIZE 5
// WRITE KODU
int main(){
    if (mkfifo("sum", 0777) == -1) {
        if (errno == EEXIST) {
            printf("FIFO already exists!\n");
        } else {
            perror("mkfifo failed");
        }
    }
    srand(time(NULL));

    int fd = open("sum", O_WRONLY);
    if(fd == -1){
        printf("non opening");
    }
    int arr[ARR_SIZE] = {0};

    for(int i = 0; i < ARR_SIZE ;i++ ){
        arr[i] = rand() % 5;
        printf("%d\n",arr[i]);
    }
    for(int i = 0; i < ARR_SIZE ;i++ ){
        if(write(fd, &arr[i], sizeof(int)) == -1)
        printf("non writed");
    }
    close(fd);
    int sum;
    fd = open("sum", O_RDONLY);

    read(fd, &sum, sizeof(sum));

    printf("%d", sum);
    close(fd);

}

// READ KODU

*/

/*

int main(){

if (mkfifo("myfifo1", 0777) == -1) {
        if (errno == EEXIST) {
            printf("FIFO already exists!\n");
        } else {
            perror("mkfifo failed");
        }
    }
    int fd = open("myfifo1", O_WRONLY); // FIFO AÇILDIĞINDA OKUNMAK İÇİN BEKLİYOR VE PROGRAMI İLERLETMİYOR 11.30 DAN SONRA BAK
    // 8. VİDEO, CAT İLE TERMİNAL ÜZERİNDEN OKUYUNCA PROGRAM KAPANIYOR.
    // KAPANDIKTAN SONRA İÇERİDEKİNİ SİLİYOR(BUFFER TEMİZLENİYOR GİBİ DÜŞÜN)
    int x = 97;
    if(write(fd, &x, sizeof(x))== -1 ) {
        return 2;
    }
    close(fd);
}


*/

/*
int main()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7}; // arrSize = 7
    int arrSize = sizeof(arr) / sizeof(int);
    int start, end;

    int fd[2];

    // fd[0] -> read
    // fd[1] -> write

    if (pipe(fd) == -1)
    {
        printf("failed in pipe");
        return 1;
    }

    int id = fork();

    if (id == -1)
    {
        printf("Failed to fork");
        return 2;
    }

    if (id == 0) // child
    {
        start = 0;
        end = arrSize / 2;
    }
    else // parent
    {
        start = arrSize / 2; // = 3
        end = arrSize;
    }
    int sum = 0;

    for (int i = start; i < end; i++)
    {
        sum += arr[i];
    }

    if (id == 0) // child process
    {
        close(fd[0]);
        write(fd[1], &sum, sizeof(int));
        close(fd[1]);
    }
    else // parent
    {
        int sumofChild;
        close(fd[1]);
        read(fd[0], &sumofChild, sizeof(int));
        close(fd[0]);
        printf("all of sum = %d\n.", sum + sumofChild);
        wait(NULL);
    }
}


*/

// COMMUNICATION BETWEEN PROCESS (USING PIPES) IN C 6. VIDEO
/*
int main(){


int fd[5];

// fd[0] - read
// fd[1] - write

if (pipe(fd) == -1){
    printf("there is a issue for pipe\n");
    return 1;
}
printf("%d\n",fd[0]);
printf("%d\n",fd[1]);

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

*/

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