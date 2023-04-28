//
// Created by Haren on 2023/4/26.
//

#ifndef CPLAYGROUNDS_RECEIVER_H
#define CPLAYGROUNDS_RECEIVER_H

#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

#define ELEC_QUERY_URL "https://xqh5.17wanxiao.com/smartWaterAndElectricityService/SWAEServlet"
#define IDUO_NOTIFICATION_URL "http://fangke.zstu.edu.cn:6007/api/WeChat/SendWeChatMessage"

typedef char rcv_err_t;

/* Definitions for error constants. */
#define RCV_OK              0   // rcv_err_t value indicating success (no error)
#define RCV_FAIL            1   // Generic rcv_err_t code indicating failure


#pragma region (Properties)Receiver
typedef struct {
    char* uid;
    char* name;
    char* json_rawValue;
    char* post_data;
    char* dormFullName; /* EXAMPLE
 * dormFullName = "2区7号公寓-70304" */
    double balance;
} Receiver;
#pragma endregion

#pragma region (Methods)Receiver
size_t receiver_elec_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

Receiver* receiver_init(char* name, char* uid);

rcv_err_t receiver_request_balance(Receiver* rcv);

/*
 * NAME
 * receiver_request_balance()
 *
 * DESCRIPTION
 * 从完美校园服务器的返回数据（JSON）中获取寝室电费余额
 */
rcv_err_t receiver_parse_balance(Receiver* rcv);

rcv_err_t receiver_get_notify(const Receiver* rcv);

#pragma endregion

#endif //CPLAYGROUNDS_RECEIVER_H
