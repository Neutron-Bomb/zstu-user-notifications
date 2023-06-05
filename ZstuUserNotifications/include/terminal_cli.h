//
// Created by Haren on 2023/5/26.
//

#ifndef ZSTUUSERNOTIFICATIONS_TERMINAL_CLI_H
#define ZSTUUSERNOTIFICATIONS_TERMINAL_CLI_H

#include <pthread.h>
#include <termios.h>

typedef struct TerminalCLI {
    struct termios old_terminal_settings;
    struct termios new_terminal_settings;
    pthread_t input_thread_id;

    size_t (*resume)(struct TerminalCLI *);
} TerminalCLI;

TerminalCLI *terminalCLI_init();

size_t terminalCLI_deinit(TerminalCLI *hdl);

#endif //ZSTUUSERNOTIFICATIONS_TERMINAL_CLI_H
