#define PROCESS_NUM 3  // 3 çocuk süreci oluşturulacak

int main() {
    int pipes[PROCESS_NUM][2];  // 3 pipe açılıyor
    int pid[PROCESS_NUM];       // 3 çocuk süreci için PID saklanacak

    // Pipe oluşturma
    for (int i = 0; i < PROCESS_NUM; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe oluşturulamadı");
            return 1;
        }
    }

    // Fork işlemi ile çocuk süreç oluşturma
    for (int i = 0; i < PROCESS_NUM; i++) {
        pid[i] = fork();  // Burada PID alınıyor

        if (pid[i] < 0) {  // Hata kontrolü
            perror("Fork hatası");
            return 1;
        }

        if (pid[i] == 0) {  // Çocuk süreç
            printf("Çocuk süreç oluşturuldu: PID = %d, Parent = %d\n", getpid(), getppid());

            // Çocuk süreçler burada işlerini yapabilir
            close(pipes[i][0]);  // Okuma ucunu kapat
            close(pipes[i][1]);  // Yazma ucunu kapat
            return 0;            // Çocuk süreç hemen çıkıyor
        }
    }

    // Parent süreç, çocukların bitmesini bekler
    for (int i = 0; i < PROCESS_NUM; i++) {
        waitpid(pid[i], NULL, 0);  // Her çocuğu tek tek bekle
    }

    printf("Tüm çocuk süreçler tamamlandı.\n");

    return 0;
}