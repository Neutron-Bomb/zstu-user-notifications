//
// Created by Haren on 2023/5/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>

#include "localize.h"
#include "terminal_cli.h"


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

void *input_thread() {
    int input;

    printf("ZstuUserNotifications Backend\nv0.1 alpha\n");
    show_help();

    while (1) {
        input = getchar();
        if (input == 'q') {
            puts(EN_US_QUIT_CONFIRMATION);
            input = getchar();
            if (input == 13 || input == 'y' || input == 'Y') break;
        } else if (input == 'h') show_help();
        usleep(16);
    }

    return NULL;
}

TerminalCLI *terminalCLI_init() {
    TerminalCLI *cli_hdl = (TerminalCLI *) malloc(sizeof(TerminalCLI));

    // 备份终端设置
    tcgetattr(STDIN_FILENO, &(cli_hdl->old_terminal_settings));
    cli_hdl->new_terminal_settings = cli_hdl->old_terminal_settings;

    cli_hdl->new_terminal_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &(cli_hdl->new_terminal_settings));

    return cli_hdl;
}

size_t terminalCLI_deinit(TerminalCLI *hdl) {
    pthread_join(hdl->input_thread_id, NULL);

    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &(hdl->old_terminal_settings));

    free(hdl);
    return 0;
}

size_t terminalCLI_resume(TerminalCLI *hdl) {
    pthread_create(&(hdl->input_thread_id), NULL, input_thread, NULL);
    return 0;
}