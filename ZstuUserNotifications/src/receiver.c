//
// Created by Haren on 2023/4/28.
//

#include "receiver.h"

size_t receiver_elec_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    strcat(userdata, ptr);
    return nmemb;
}

Receiver* receiver_init(char* name, char* uid)
{
    Receiver* hdl = (Receiver*) malloc(sizeof(Receiver));
    hdl->name = name;
    hdl->uid = uid;
    return hdl;
}

rcv_err_t receiver_request_balance(Receiver* rcv)
{
    CURL* curl_elec = curl_easy_init();

    char encoded_str[128] = {};
    char param[80] = {};
    char elec_data[10240] = {};

    char* encoded_param = NULL;

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

    curl_easy_cleanup(curl_elec);
    return 0;
}

rcv_err_t receiver_parse_balance(Receiver* rcv)
{
    cJSON* json = cJSON_Parse(rcv->json_rawValue);
    if (json == NULL) return RCV_FAIL;

    cJSON* body = cJSON_Parse(cJSON_GetObjectItem(json, "body")->valuestring);
    cJSON_free(json);

    cJSON* modlist = cJSON_GetObjectItem(body, "modlist");
    cJSON_free(body);

    cJSON* modlist_element = cJSON_GetArrayItem(modlist, 0);
    cJSON_free(modlist);

    if (modlist_element != NULL) {
        rcv->balance = cJSON_GetObjectItem(modlist_element, "odd")->valuedouble;
        cJSON_free(modlist_element);
        return 0;
    } else return 1;
}

rcv_err_t receiver_get_notify(const Receiver* rcv)
{
    CURL* curl_iduo = curl_easy_init();

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
                       u8"}", rcv->uid, rcv->name, rcv->balance, rcv->balance);

    // 一多推送请求配置
    curl_easy_setopt(curl_iduo, CURLOPT_URL, IDUO_NOTIFICATION_URL);
    curl_easy_setopt(curl_iduo, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_iduo, CURLOPT_POSTFIELDS, queryData);

    struct curl_slist* list;
    list = curl_slist_append(NULL, "Content-Type: application/json-patch+json");
    list = curl_slist_append(list, "Content-Type: application/json-patch+json");
    list = curl_slist_append(list, "Connection: keep-alive");
    curl_easy_setopt(curl_iduo, CURLOPT_HTTPHEADER, list);

    curl_easy_perform(curl_iduo);

    curl_slist_free_all(list);

    curl_easy_cleanup(curl_iduo);
    return RCV_OK;
}
