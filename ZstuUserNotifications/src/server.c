//
// Created by Haren on 2023/5/23.
//

#include <stdlib.h>
#include <sys/socket.h>

#include "server.h"

void *private_server_main(void *arg);

Server *server_init() {
    Server *srv_hdl = (Server *) malloc(sizeof(Server));
    pthread_create(&srv_hdl->thread, NULL, private_server_main, NULL);
    return srv_hdl;
}

void *private_server_main(void *arg) {
    pthread_t response_thread_id;
    
    return NULL;
}

void *private_server_response(void *arg) {
    return NULL;
}

srv_err_t server_deinit(Server *hdl) {
    pthread_cancel(hdl->thread);
    free(hdl);
    return SRV_OK;
}

