#include <pthread.h>
#include <termios.h>
#include <unistd.h>

#include "receiver.h"
#include "localize.h"

void show_help() {
    puts("====Help====\n"
         "[h]\tshow this man page\n"
         "[i]\tshow the service status\n"
         "[p]\tpause the notification pusher\n"
         "[q]\tterminate the whole service\n"
         "[s]\tsave the user subscriptions immediately\n"
         "\twhich is located in \"~/.config/zstuNotify\"\n"
         "[w]\t"
         );
}

void *input_thread(void *arg) {
    int input;

    printf("ZstuUserNotifications Backend\nv0.1 alpha\n");
    show_help();

    loop:
    while (1) {
        input = getchar();
        if (input == 'q') break;
        else if (input == 'h') show_help();

        usleep(16);
    }

    puts(EN_US_QUIT_CONFIRMATION);
    input = getchar();
    if (input != 13 && input != 'y' && input != 'Y') goto loop;
    return NULL;
}

int main() {
    pthread_t input_thread_id;
    pthread_create(&input_thread_id, NULL, input_thread, NULL);

    struct termios old_terminal_settings;
    struct termios new_terminal_settings;

    // 备份终端设置
    tcgetattr(STDIN_FILENO, &old_terminal_settings);
    new_terminal_settings = old_terminal_settings;

    new_terminal_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_settings);

    // USER cck
    Receiver *cck = receiver_init(u8"陈驰坤", "2020316101023");

    // USER yzj
    Receiver *ljh = receiver_init(u8"李佳禾", "2020316101061");
/*
    // USER lsf
    Receiver* lsf = receiver_init(u8"楼胜峰", "2022331200164");

    // USER lzh
    Receiver* lzh = receiver_init(u8"林展航", "2020316101012");

    // USER yzj
    Receiver* yzj = receiver_init(u8"杨正杰", "2020316101015");

    // USER zbh
    Receiver* zbh = receiver_init(u8"张碧涵", "2020316101022");
*/
    Receiver *receivers[] = {
            cck, ljh
    };
    int receivers_length = sizeof(receivers) / sizeof(receivers[0]);

    for (int i = 0; i < receivers_length; ++i) {
        receiver_notify(receivers[i], print);
    }

    // free up the memory
    receiver_deinit(cck);
    receiver_deinit(ljh);

    pthread_join(input_thread_id, NULL);

    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal_settings);
    return 0;
}
