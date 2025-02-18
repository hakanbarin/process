// MEMORY ARENA KISMINA BAK

// aynı pipe üzerinden child ve parent haberleşme yapmak istersen sorun yaşıyorsun. ANLADIĞIM KADARIYLA WRITE GERİ DÖNÜŞ
//  DEĞERİ DÖNDÜRENE KADAR READİN ALTINDAKİ KOMUT SATIRINA GİRİLİYOR BU SENKRONİZASYON HATASI MEYDANA GETİRİYOR.
// race condition gibi bir şey oluyor

// exiti nerede kullanırsan kullan tüm programı kapatıyor
// return ile en büyük farkı bu returnle fonksiyondan da çıkabiliyorsun ama mainde ikisi de aynı

// 5. VİDEOYA BİR DAHA BAK MULTIPLE FORK ÖNEMLİ GİBİ DURUYOR
// 12. VİDEOYU TEKRARDAN İZLE KODUNU BİR DAHA YAZ
// signal handle işlemlerine biraz daha detaylı bak

// int pid = fork(); int pid2 = fork(); hem ana süreç hem de ilk çocuk süreci tekrar fork() çağırır.

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>


#define PROCESS_NUM 3


int main(){

    int pipes[PROCESS_NUM + 1][2]; // 3 process için 4 tane boru hattı gerekiyor şema çizerek düşün
    int pid[PROCESS_NUM];

    int i;
    for(i =0; i < PROCESS_NUM + 1;i++){
        if(pipe(pipes[i]) == -1){
            printf("pipe olusturulamadi");
            return 1;
        }
    }
    //https://stackoverflow.com/questions/26793402/visually-what-happens-to-fork-in-a-for-loop
    for(i = 0; i < PROCESS_NUM;i++){

        pid[i] = fork();

        if(pid[i] == -1){
            printf("error with creating pid");
            return 2;
        }

        if(pid[i] == 0){
            
            for(int j = 0; j < PROCESS_NUM + 1; j++){
                if(i != j){

                    close(pipes[j][0]); // OKUMAYI İ'DEN YAZMAYI İ+1 DEN YAPTIĞI İÇİN BU DURUMLAR HARİÇ HEPSİNİ KAPATIYORUZ.
                                        // SOL ÜSTTEN SAĞ ALTA DOĞRU OKUYOR 2x1 MATRİX DÜŞÜN 5 TANE YAN YANA
                }
                if(i + 1 != j){
                    close(pipes[j][1]);
                }
            }
        
            int x;
            if(read(pipes[i][0], &x, sizeof(int)) == -1){
                printf("error read pipes");
            }
            printf("(%d) got result = %d\n",i,x);
            x++;
            if(write(pipes[i+1][1], &x, sizeof(int)) == -1){
                printf("error write pipes");
            }
            printf("(%d) sent result = %d\n",i,x);
            close(pipes[i][0]);
            close(pipes[i+1][1]);

        return 0; // burada çocuk processlerin artmasını engelliyoruz sadece parent processler üzerinden ilerleniyor.
    }

       
    }
    for(int j = 0; j < PROCESS_NUM + 1; j++){
        if(j != PROCESS_NUM){

            close(pipes[j][0]);
        }
        if(j != 0){
            close(pipes[j][1]);
        }
    }
    int y = 5;
    if(write(pipes[0][1], &y, sizeof(int)) == -1){
        printf("error main write ");
    }
    if(read(pipes[PROCESS_NUM][0], &y, sizeof(int)) == -1){
        printf("error main read ");
    }

    close(pipes[0][1]);
    close(pipes[PROCESS_NUM][0]);

    for(i = 0; i < PROCESS_NUM; i++){
        waitpid(pid[i], NULL, 0);
    }
    printf("final result = %d",y);
}







/*
int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("çocuk Süreç: PID = %d\n", getpid());
        printf("pid in child = %d ",pid);
    } else if (pid > 0) {
        printf("ana Süreç: PID = %d\n", getpid());
        printf("pid in parent = %d\n", pid);
    } else{
        perror("fork failed");
        return 1;
    }

    return 0;
}

*/

/*
int main() {
    int fd[3][2];
    for(int i = 0; i < 3; i++){
        if(pipe(fd[i]) == -1){
            return 1;
        }
    }

    int pid1 = fork();

    if(pid1 == 0){
        int x;
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[2][0]);
        close(fd[2][1]);

        if(read(fd[0][0], &x, sizeof(int)) == -1){
            return 2;
        }
        printf("cocuk1 aldim  x = %d\n",x);
        x = x + 5;
        if(write(fd[1][1], &x, sizeof(int)) == -1){
            return 3;
        }
        printf("cocuk2 yazdim  x = %d\n",x);
        close(fd[0][0]);
        close(fd[1][1]);
        printf("cocuk 1 pid %d parent pid %d\n",getpid(), getppid());

    }
    // BU ŞEKİLDE 2 ÇOCUK OLUŞTURLUYOR EĞER FORKLARI ALT ALTA ÇAĞIRSAYDIK 4 FARKLI PROCESS(3 ÇOCUK) OLUŞACAKTI.
    int pid2 = fork();
    if(pid2 == 0){
        int x;
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][1]);
        close(fd[2][2]);
        if(read(fd[1][0], &x, sizeof(int)) == -1){
            return 4;
        }
        printf("cocuk2 aldim  x = %d\n",x);
        if(write(fd[2][1], &x, sizeof(int)) == -1){
            return 5;
        }
        printf("cocuk2 yazdim  x = %d\n",x);

        close(fd[2][1]);
        close(fd[1][0]);
        printf("cocuk 2 pid %d parent pid %d\n",getpid(), getppid());

    }
    close(fd[0][0]);
    close(fd[1][0]);
    close(fd[1][1]);
    close(fd[2][1]);
    int x = 5;
    if(write(fd[0][1], &x, sizeof(int)) == -1){
        return 6;
    }
    printf("baba yazdim  x = %d\n",x);
    if(read(fd[2][0], &x, sizeof(int)) == -1){
        return 7;
    }
    close(fd[0][1]);
    close(fd[2][0]);
    waitpid(pid1,NULL,0); // wait pid ile ne kadar beklersek bekleyelim istediğimiz processi ilk başta sonlandırabiliyoruz.
    waitpid(pid2,NULL,0);
}


*/

/*
int main(){

    int fd[2];

    if(pipe(fd) == -1){
        return 1;
    }

    int pid = fork();

    if(pid == 0){

    dup2(fd[1], STDOUT_FILENO); // BURADA PİNG DİREKT PIPE'A YAZDIĞINDAN STDOUTTAN ÇIKIŞ GÖRMÜYORUZ DİĞER PROCESSTEKİ
    close(fd[0]);               // PİPEIN ALICI KISMINA YAZDIRIYORUZ.
    close(fd[1]);
    execlp("ping", "ping", "-c", "5", "google.com",NULL);


}
else{

dup2(fd[0], STDIN_FILENO);      //BU KOD İÇİN BU KISMI İKİNCİ BİR PROCESSİN CHILD KISMINDA YAZABİLİRSİN
close(fd[0]);                   // BU SAYEDE PARENTLER AYAKTA KALIRÇ
close(fd[1]);

                                        // BURADA ping -c 5 google.com | grep rtt processleri çalışıyor!!!!
execlp("grep","grep", "rtt",NULL);
}
waitpid(pid, NULL, 0 );


}


*/

/*
int main()
{
    int fd[2];
    if(pipe(fd) == -1){
        return 1;
    }

    int pid = fork();
    if (pid == -1)
    {
        return 2;
    }

    if (pid == 0){  // child

    close(fd[0]); //read olmayacak
    char arr[200];
    fgets(arr, 200, stdin);

    arr[strlen(arr) - 1] = '\0'; //  arrayin en sonuna NULL ekliyoruz
        printf("Sending arr: %s\n", arr);

        int n = strlen(arr) + 1; // \0 ı katmıyor bu durumdan dolayı +1 ekliyoruz.

        if(write(fd[1], &n, sizeof(int)) == -1){
            return 3;
        }
        if(write(fd[1], &arr, sizeof(char) * n) == -1 ){
            return 4;
        }
        close(fd[1]);

    }

    else{   // parent
    close(fd[1]);
    int n = 0;
    char arr[200];
    if(read(fd[0], &n, sizeof(int)) == -1){
        return 5;
    }
    if(read(fd[0], &arr, sizeof(char) * n) == -1){
        return 6;
    }
        printf("Received: %s\n", arr);
        wait(NULL);

    }
}

*/

/*
void handle_sigusr1(int sig)
{
    for(int i = 10; i > 0;i-- ){
        printf("DUNYA YOK OLACAK %d SANIYE ICINDE ISLEMI YAP\n",i);
        fflush(stdout);
        sleep(1);
    }
    kill(getppid(), SIGKILL);

}

int main()
{
    int pid = fork();
    if (pid == -1)
    {
        return 2;
    }

    if (pid == 0)
    {
        sleep(5);
        kill(getppid(), SIGUSR1); //get parent pid--> parentin pidyi alıp SIGUSR1 sinyalini yolluyor o processe
    }

    else
    {

    struct sigaction sa;

    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1; // kodun taşınabilir olması için bunu kullanmak daha mantıklı
    sigaction(SIGUSR1, &sa, NULL); //burada sinyal yakalanıp fonksiyonda ne yazılıysa o yapılıyorç

    int x;
    printf("5x3 kaç eder ?\n");
        scanf("%d", &x);
        if (x == 15)
        {

        printf("dogru\n");
    }
    else
    {

    printf("yanlis\n");
}
}
}

*/
/*
int main()
{

int pid = fork();
if (pid == 0)
    {
        while(1){

        printf("CIKISCIKIS\n");
        usleep(50000);
    }
}
else
{
    kill(pid, SIGSTOP);
    int t;

    do
    {
        printf("ne kadar calissin?\n");
        scanf("%d", &t);
        if (t > 0)
        {
            kill(pid, SIGCONT);
            sleep(t);
            kill(pid, SIGSTOP);
        }
    }while (t > 0);
    kill(pid,SIGKILL);
    wait(NULL);
}
}

/*
int main(){

int pid = fork();

if(pid == -1){
    return 2;
}

if(pid == 0){
    while(1){
        printf("cocuk calisiyor\n");
        usleep(50000);

    }
}

else{
    kill(pid,SIGSTOP);
    sleep(2);
    kill(pid,SIGCONT);
    sleep(1);
    kill(pid, SIGKILL);
    printf("cocuk gitti\n");
}
}

*/

/*
int main() {

int pid = fork();
if (pid == -1){
    return 1;
}

if(pid == 0){
    while(1){
        printf("naber\n");
        usleep(50000);
    }
}
else{
    sleep(2);
    kill(pid, SIGKILL);
    wait(NULL);
}
// https://askubuntu.com/questions/59811/kill-pid-not-really-killing-the-process-why
}



*/

/*
int main()
{

    int pid = fork();

    if (pid == 0)
    {
        int fold = open("deneme.txt", O_WRONLY | O_CREAT, 0777);
        // r (read) → 4 w (write) → 2 x (execute) → 1      0rwx  burada sahip grup diğerlerine okuma yazma çalıştırma yetkisi veriliyor.
        int stateVariable = dup2(fold, 1);
        if (stateVariable == -1)
        {
            printf("hata var\n");
        }
        printf("hello bro\n");
        fflush(stdout); // çıktıyı hemen dosyaya yaz

        int err = execlp("bash", "bash", "-c", "ping -c 3 google.com | grep rtt", NULL);
        // execlp("ping", "ping", "-c" "3" "google.com" "|"" "grep" "rtt", NULL); yukarıdaki kodu bu şekilde yazmaya
        // çalışssaydık rtt name or service not known hatası verirdi çünkü bu komutla executable bir dosyayı çalıştırıyoruz
        // ping kodunun main kısmında argv[] parametre olarak sadece yazdıklarımızı alırsa anlamlı oluyor(ya da işe
        // yarayacak diğerlerini), grep ayrı bir executable dosya olduğu için bashte çalıştırıyormuşuz gibi aynı şekilde
        // çalıştırma şansımız yok. Eğer yukarıdaki gibi yazarsak bash argüman aldığı için bunu yapabiliriz !!!!!.
        if (err == -1)
        {
            return 1;
        }
    }
    else
    {
        int wstatus;
        wait(&wstatus);
        if (WIFEXITED(wstatus))
        { // BU İKİ TANIMLAYICIYA DİKKAT ET
        int statusCode = WEXITSTATUS(wstatus);
        if (statusCode == 0)
        {
            printf("succes\n");
        }
        else
        {
            printf("failure. status code...%d\n", statusCode);
        }
    }
}
}
*/

/*

int main() {

    int oldfd = open("output.txt", O_WRONLY | O_CREAT, 0644);
    if (oldfd == -1) {
        perror("open failed");
        return 1;
    }
    //  FILE DESCRIPTOR      FILE
    //        0              STDIN
    //        1              STDOUT---------| buraya output.txt geliyor
    //        2              STDERR         |
    //        3              output.txt ----|

    //  FILE DESCRIPTOR      FILE
    //        0              STDIN
    //        1              output.txt
    //        2              STDERR
    //        3              output.txt

    // BURADA FILE DESCRIPTORSLARDAN 1. OLAN STD_OUTUN ÜZERİNDE output.txt yi kopyalıyoruz ve STDOUT veren her şey
    // output.txtnin içine yazılıyor inanılmaz harika bir şeymiş bu bunu yerim

    if (dup2(oldfd, STDOUT_FILENO) == -1) {
        perror("dup2 failed");
        return 1;
    }

    printf("bunlarin hepsi output.txtnin içine yazilacak\n");

    close(oldfd);

    return 0;
}


*/

/*

// argv[] kodu terminalde(command line) çalıştırırken ilk elemanını exe dosyasının konumunu alır ve diğer argümanları test
// için kendimize göre ayarlayabiliriz. örnek strtol(argv[i],NULL,10) result += argv[i] i'yi 1 den başlatırız ve argc sayısı
// kadar iterasyon yaparız.pythonda model eğitirken terminal üzerinden girdiğin data yolları num_workers gibi düşün.

int calc(int **arr[]){
    int sum = 0;
    for(int i = 0; i < 2; i++){
        sum = **arr[i] + sum;
        printf("%p\n",arr[i]);
    }
    **arr[0] = 2;
    **arr[1] = 432;
    printf("sum = %d\n",sum);
    return sum;
}



int main(int argc, char* argv[]){
    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;
    int e = 6;
    int *arr1 = &a;
    int *arr2 = &b;
    int **arr[] = {&arr1,&arr2};
    int sum = calc(arr);
    int sum2 = **arr[0] + **arr[1];
    printf("sum = %d\n",sum);
    printf("sum2 = %d\n",sum2);

}

*/

/*
int main() {
    printf("Before execl\n");

    execlp("ping", "ping","google.com", NULL);
    //  processin pid'si değişmez, ancak bellekteki tüm kod ve veri segmentleri yeni programla değiştirilir.
    //  cvp olunca argüman olarak array(vektör) alıyor.


    perror("execl failed\n   ");
    return 1;
}

*/

/*

int main(){
    int pid = fork();


    if(pid == 0){
        execlp("mkfifo", "mkfifo", "sum", NULL);
        printf("\n");
    }
    else{
        int wstatus;
        wait(&wstatus);

        if(WIFEXITED(wstatus)){
            int statusCode = WEXITSTATUS(wstatus);
            if(statusCode == 0){
                printf("dogru cikti\n");
            }
            else{
                printf("yanlis cikti kodu...%d\n",statusCode);
            }

        }

    }
}



*/

/*
typedef struct foo
{
    int reads;
    int write;
} foo;

int main()
{
    foo beyza;
    foo hakan;
    srand(time(NULL));
    int p1[2]; // C -> F com
    int p2[2]; // F -> C com


    if (pipe(p1) == -1)
    {
        return 2;
    }
    if (pipe(p2) == -1)
    {
        return 3;
    }

    beyza.reads = p1[0];  // C -> F com READ
    beyza.write = p1[1];  // C -> F com WRITE

    hakan.reads = p2[0]; // F -> C com READ
    hakan.write = p2[1]; // F -> C com WRITE
    int pid = fork();
    if (pid == -1)
    {
        return 4;
    }

    if (pid == 0) // C -> F pipe'ı kullanılacak yani p1
    {
        int x;
        close(beyza.reads);
        close(hakan.write);
        if (read(hakan.reads, &x, sizeof(int)) == -1)
        {
            return 5;
        }
        printf("coming variable..%d\n", x);
        x *= 5;
        printf("sending variable..%d\n", x);
        if (write(beyza.write, &x, sizeof(int)) == -1)
        {
            return 6;
        }
    }
    else
    {
        close(hakan.reads);
        close(beyza.write);
        int randNum = rand() % 5;
        printf("sending variable father process..%d\n", randNum);
        int comingNode;
        if (write(hakan.write, &randNum, sizeof(int)) == -1)
        {
            return 7;
        }

        if (read(beyza.reads, &comingNode, sizeof(int)) == -1)
        {
            return 8;
        }
        printf("%d\n", comingNode);
    }

}

*/
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

https://stackoverflow.com/questions/40864527/what-is-file-descriptor


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