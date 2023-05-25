//
// Created by Haren on 2023/4/26.
//

#ifndef CPLAYGROUNDS_RECEIVER_H
#define CPLAYGROUNDS_RECEIVER_H

#include <curl/curl.h>
#include <time.h>

#define ELEC_QUERY_URL "https://xqh5.17wanxiao.com/smartWaterAndElectricityService/SWAEServlet"
#define IDUO_NOTIFICATION_URL "http://fangke.zstu.edu.cn:6007/api/WeChat/SendWeChatMessage"

typedef char rcv_err_t;

/* Definitions for error constants. */
#define RCV_OK              0   // rcv_err_t value indicating success (no error)
#define RCV_FAIL            1   // Generic rcv_err_t code indicating failure

#pragma region (Properties)Receiver

typedef struct {
    char *fullName; /* EXAMPLE dormFullName = "2区7号公寓-70304" */
    double balance;
    time_t updateTime;
} Dorm;

typedef struct {
    char *uid;
    char *name;
    char *json_rawValue;
    char *post_data;

    Dorm *dorm;
} Receiver;

enum NotifyMode {
    print,
    printAndNotify,
    notify
};

const Dorm *receiver_dorm(const Receiver *hdl);

double dorm_balance(const Dorm *hdl);

time_t dorm_updateTime(const Dorm *hdl);

#pragma endregion

#pragma region (Methods)Receiver

size_t receiver_elec_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

Receiver *receiver_init(char *name, char *uid);

rcv_err_t receiver_deinit(Receiver *hdl);

rcv_err_t receiver_notify(Receiver *rcv, enum NotifyMode mode);

#pragma endregion

#endif //CPLAYGROUNDS_RECEIVER_H
