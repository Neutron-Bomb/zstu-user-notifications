//
// Created by Haren on 2023/4/28.
//

#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

#include "receiver.h"

const Dorm *receiver_dorm(const Receiver *hdl) {
    return hdl->dorm;
}

double dorm_balance(const Dorm *hdl) {
    return hdl->balance;
}

time_t dorm_updateTime(const Dorm *hdl) {
    return hdl->updateTime;
}

size_t receiver_elec_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    strcat(userdata, ptr);
    return nmemb;
}

Receiver *receiver_init(char *name, char *uid) {
    Receiver *rcv_hdl = (Receiver *) malloc(sizeof(Receiver));
    Dorm *drm_hdl = (Dorm *) malloc(sizeof(Dorm));

    drm_hdl->balance = 0;
    drm_hdl->fullName = 0;
    drm_hdl->updateTime = 0;

    rcv_hdl->name = name;
    rcv_hdl->uid = uid;
    rcv_hdl->dorm = drm_hdl;

    return rcv_hdl;
}

rcv_err_t receiver_deinit(Receiver *hdl) {
    free(hdl->dorm);
    free(hdl);

    return RCV_OK;
}

rcv_err_t receiver_collecdate_to_timestamp(Receiver *rcv, const char *collecdate) {
    size_t collecdate_length = strlen(collecdate);

    // format: {0: year, 1: month, 2: day, 3: hour, 4: minute, 5: second}
    char *parse[6] = {};
    for (int i = 0; i < collecdate_length; ++i) {
        if (i < 4) parse[0][i] = collecdate[i];                 // year
        else if (i > 4 && i < 7) parse[1][i] = collecdate[i];   // month
        else if (i > 7 && i < 10) parse[2][i] = collecdate[i];  // day
        else if (i > 10 && i < 13) parse[3][i] = collecdate[i]; // hour
        else if (i > 13 && i < 16) parse[4][i] = collecdate[i]; // minute
        else if (i > 16 && i < 19) parse[5][i] = collecdate[i]; // second
    }

    return RCV_OK;
}

rcv_err_t receiver_parse_balance(Receiver *rcv) {
    cJSON *json = cJSON_Parse(rcv->json_rawValue);
    if (json == NULL) return RCV_FAIL;

    cJSON *body = cJSON_Parse(cJSON_GetObjectItem(json, "body")->valuestring);
    if (body == NULL) return RCV_FAIL;

    char *roomfullname = cJSON_GetObjectItem(body, "roomfullname")->valuestring;
    if (roomfullname == NULL) return RCV_FAIL;

    rcv->dorm->fullName = roomfullname;

    // parse array type "modlist"
    cJSON *modlist = cJSON_GetObjectItem(body, "modlist");
    if (modlist == NULL) return RCV_FAIL;

    cJSON *modlist_element = cJSON_GetArrayItem(modlist, 0);
    if (modlist_element == NULL) return RCV_FAIL;

    rcv->dorm->balance = cJSON_GetObjectItem(modlist_element, "odd")->valuedouble;

    // free up the RAM
    cJSON_free(json);
    cJSON_free(body);
    cJSON_free(modlist);
    cJSON_free(modlist_element);
    return RCV_OK;
}

rcv_err_t receiver_request_balance(Receiver *rcv) {
    CURL *curl_elec = curl_easy_init();

    char encoded_str[128] = {};
    char param[80] = {};
    char elec_data[10240] = {};

    char *encoded_param = NULL;

    // 将学号插入字符串
    sprintf(param, "{\"cmd\":\"getstuindexpage\",\"account\":\"%s\"}", rcv->uid);

    // 对param字段进行url-encode
    encoded_param = curl_easy_escape(curl_elec, param, 0);

    // 合并其他所有内容
    sprintf(encoded_str, "param=%s&customercode=599&method=getstuindexpage", encoded_param);
    curl_free(encoded_param);

    rcv->post_data = encoded_str;

    // 完美校园请求配置
    curl_easy_setopt(curl_elec, CURLOPT_URL, ELEC_QUERY_URL);
    curl_easy_setopt(curl_elec, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_elec, CURLOPT_POSTFIELDS, rcv->post_data);
    curl_easy_setopt(curl_elec, CURLOPT_WRITEFUNCTION, receiver_elec_write_callback);
    curl_easy_setopt(curl_elec, CURLOPT_WRITEDATA, elec_data);

    // 执行余额查询请求
    curl_easy_perform(curl_elec);

    rcv->json_rawValue = elec_data;
    rcv->dorm->updateTime = time(NULL);

    curl_easy_cleanup(curl_elec);

    receiver_parse_balance(rcv);
    return 0;
}

rcv_err_t receiver_notify(Receiver *rcv, enum NotifyMode mode) {
    receiver_request_balance(rcv);
    if (mode < 2) printf("当前寝室电费余额: %.02f度\n", rcv->dorm->balance);
    if (mode == print) return RCV_OK;

    CURL *curl_iduo = curl_easy_init();

    char queryData[1024] = {};
    sprintf(queryData, u8"{"
                       u8"Domain: \"ZSTU\","
                       u8"UserId: \"%s\","
                       u8"UserName: \"%s\","
                       u8"NotifyType: \"Wechat\","
                       u8"WeChat: {"
                       u8"MegType: \"textcard\","
                       u8"Message: \"今日寝室电费还剩余: %.02f度\","
                       u8"CardInfo: {"
                       u8"Description: \"今日寝室电费还剩余: %.02f度\","
                       u8"Title: \"每日提醒 #寝室电费余额\","
                       u8"Url: \"https://github.com/haren724\""
                       u8"}"
                       u8"}"
                       u8"}", rcv->uid, rcv->name, rcv->dorm->balance, rcv->dorm->balance);

    // 一多推送请求配置
    curl_easy_setopt(curl_iduo, CURLOPT_URL, IDUO_NOTIFICATION_URL);
    curl_easy_setopt(curl_iduo, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_iduo, CURLOPT_POSTFIELDS, queryData);

    struct curl_slist *list;
    list = curl_slist_append(NULL, "Content-Type: application/json-patch+json");
    list = curl_slist_append(list, "Content-Type: application/json-patch+json");
    list = curl_slist_append(list, "Connection: keep-alive");
    curl_easy_setopt(curl_iduo, CURLOPT_HTTPHEADER, list);

    curl_easy_perform(curl_iduo);

    curl_slist_free_all(list);

    curl_easy_cleanup(curl_iduo);
    return RCV_OK;
}
