#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <errno.h>

#define RUN_B1 (1)
#define RUN_B2 (2)
#define RUN_B3 (3)
#define RUN_B4 (4)

/* Bài tập 1: Tạo và Chạy Luồng Cơ bản
   Yêu cầu: Viết một chương trình bắt tín hiệu SIGINT (được gửi khi nhấn Ctrl+C) và in ra thông báo khi nhận được tín hiệu này.
   1. Sử dụng hàm signal() để đăng ký một hàm xử lý cho tín hiệu SIGINT.
   2. Trong hàm xử lý, in ra thông báo "SIGINT received" mỗi khi nhận được tín hiệu SIGINT.
   3. Chương trình sẽ tiếp tục chạy cho đến khi nhận được SIGINT lần thứ 3, sau đó kết thúc.
   Gợi ý: Sử dụng một biến đếm toàn cục để đếm số lần tín hiệu SIGINT được nhận và kiểm tra biến này trong hàm xử lý.
   Câu hỏi: Nếu bỏ qua tín hiệu SIGINT, chuyện gì sẽ xảy ra khi nhấn Ctrl+C?
*/

sig_atomic_t count1 = 0;
void handle_sigint(int sig) {
    printf("\nSIGINT received, count=%d", ++count1);
    if (count1 >= 3)
        exit(0);
}

void b1() {
    signal(SIGINT, handle_sigint);
    while (1) {
        pause();
    }
}

/* Bài tập 2: Sử dụng Tín hiệu SIGALRM để Tạo Timer
   Yêu cầu: Viết chương trình tạo một bộ đếm thời gian bằng cách sử dụng tín hiệu SIGALRM.
   1. Sử dụng hàm alarm() để đặt thời gian kích hoạt SIGALRM mỗi giây.
   2. Khi nhận tín hiệu SIGALRM, chương trình sẽ tăng biến đếm và in ra dòng "Timer: <giây> seconds".
   3. Dừng chương trình sau khi đếm đến 10 giây.
   Gợi ý: Sử dụng hàm signal() để đăng ký xử lý tín hiệu SIGALRM, và đặt alarm(1) để bộ đếm lặp lại hàng giây.
   Câu hỏi: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?
*/

sig_atomic_t count2 = 0;
void handle_alarm(int sig) {
    printf("\nTimer=%d", ++count2);
    alarm(1);
    if (count2 >= 10)
        exit(0);
}

void b2() {
    signal(SIGALRM, handle_alarm);
    alarm(1);
    while (1) {
        pause();
    }
}

/* Bài tập 3: Tạo Giao tiếp giữa Các Process bằng Tín hiệu
   Yêu cầu: Viết chương trình tạo hai process (cha và con) và sử dụng tín hiệu SIGUSR1 để giao tiếp giữa chúng.
   1. Dùng fork() để tạo process con từ process cha.
   2. Process cha sẽ gửi tín hiệu SIGUSR1 cho process con mỗi 2 giây.
   3. Khi nhận tín hiệu SIGUSR1, process con sẽ in ra thông báo "Received signal from parent".
   4. Dừng chương trình sau khi đã gửi tín hiệu 5 lần.
   Gợi ý: Dùng kill() để gửi tín hiệu từ process cha đến process con, và sử dụng signal() trong process con để bắt tín hiệu SIGUSR1.
   Câu hỏi: Tại sao chúng ta cần sử dụng kill() để gửi tín hiệu trong bài này?
*/

sig_atomic_t count3 = 0;
void handle_user1(int sig) {
    printf("\nReceived signal from parent");
    count3++;
    if (count3 == 5)
        exit(0);
}

void b3() {
    int rel_val;
    signal(SIGUSR1, handle_user1);
    rel_val = fork();
    if (rel_val == 0) {
        while (1)
            pause();
    } else {
        int i;
        for (i = 0; i < 5; i++) {
            sleep(2);
            if (kill(rel_val, SIGUSR1) == -1)
                perror("\nErr send kill");
        }
        wait(NULL);
    }
}

/* Bài tập 4: Chương trình chờ tín hiệu kết hợp chờ người dùng nhập liệu
   Yêu cầu: Viết một chương trình có thể nhận tín hiệu đồng thời cho phép người dùng nhập liệu từ bàn phím.
   1. Dùng select() hoặc poll() để cho phép chương trình xử lý tín hiệu mà không làm gián đoạn khi nhận dữ liệu nhập từ bàn phím.
   2. Khi nhận tín hiệu SIGINT, in ra thông báo “SIGINT received.”
   3. Nếu nhận tín hiệu SIGTERM, thoát chương trình.
   4. Khi người dùng nhập liệu và nhấn Enter, in nội dung ra màn hình console.
*/

volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigterm_received = 0;

void handle_sigint_b4(int sig) {
    sigint_received = 1;
}

void handle_sigterm_b4(int sig) {
    sigterm_received = 1;
}

void b4() {
    char buffer[1024];

    // Đăng ký handler
    signal(SIGINT, handle_sigint_b4);
    signal(SIGTERM, handle_sigterm_b4);

    fd_set readfds;
    int ret;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, NULL);

        if (ret == -1 && errno == EINTR) {
            // select bị ngắt bởi tín hiệu, tiếp tục vòng lặp
            continue;
        } else if (ret == -1) {
            perror("select");
            break;
        }

        if (ret > 0) {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    printf("\nBạn vừa nhập: %s", buffer);
                } else {
                    printf("Lỗi đọc stdin hoặc EOF.\n");
                    break;
                }
            }
        }

        if (sigterm_received == 1) {
            printf("\nSIGTERM received. Exiting...\n");
            break;
        }

        if (sigint_received == 1) {
            printf("\nSIGINT received.\n");
            sigint_received = 0;
        }
    }
}

/* Main App */
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Vui lòng truyền một đối số (1, 2 hoặc 3 hoặc 4).\n");
        return 1;
    }
    int option = atoi(argv[1]);
    if (option > 4 || option < 1) {
        printf("Vui lòng truyền một đối số (1, 2 hoặc 3 hoặc 4).\n");
        return 1;
    }
    if (option == RUN_B1)
        b1();
    else if (option == RUN_B2)
        b2();
    else if (option == RUN_B3)
        b3();
    else
        b4();
    return 0;
}
