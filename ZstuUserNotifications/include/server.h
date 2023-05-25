//
// Created by Haren on 2023/5/23.
//

#ifndef ZSTUUSERNOTIFICATIONS_SERVER_H
#define ZSTUUSERNOTIFICATIONS_SERVER_H

#include <pthread.h>

#define SRV_BUFFER_SIZE 1024
#define SRV_LISTEN_PORT 6009

typedef char srv_err_t;

#define SRV_OK              0
#define SRV_FAIL            1

typedef struct {
    time_t last_update_time;
    pthread_t thread;

} Server;

Server *server_init();

srv_err_t server_deinit(Server *hdl);



#endif //ZSTUUSERNOTIFICATIONS_SERVER_H
