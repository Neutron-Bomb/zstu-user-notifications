//
// Created by Haren on 2023/5/26.
//

#ifndef ZSTUUSERNOTIFICATIONS_TERMINAL_CLI_H
#define ZSTUUSERNOTIFICATIONS_TERMINAL_CLI_H

#include <pthread.h>
#include <termios.h>

typedef struct {
    struct termios old_terminal_settings;
    struct termios new_terminal_settings;
    pthread_t input_thread_id;
} TerminalCLI;


TerminalCLI *terminalCLI_init();

size_t terminalCLI_deinit(TerminalCLI *hdl);

size_t terminalCLI_resume(TerminalCLI *hdl);

#endif //ZSTUUSERNOTIFICATIONS_TERMINAL_CLI_H
