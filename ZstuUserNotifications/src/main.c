#include "receiver.h"

int main()
{
    // USER cck
    Receiver* cck = receiver_init(u8"陈驰坤", "2020316101023");

    // USER lsf
    Receiver* lsf = receiver_init(u8"楼胜峰", "2022331200164");

    // USER lzh
    Receiver* lzh = receiver_init(u8"林展航", "2020316101012");

    // USER yzj
    Receiver* yzj = receiver_init(u8"杨正杰", "2020316101015");

    // USER yzj
    Receiver* ljh = receiver_init(u8"李佳禾", "2020316101061");

    // USER zbh
    Receiver* zbh = receiver_init(u8"张碧涵", "2020316101022");

    Receiver* receivers[] = {
            cck, ljh
    }; int receivers_length = sizeof(receivers) / sizeof(receivers[0]);

    for (int i = 0; i < receivers_length; ++i) {
        receiver_request_balance(receivers[i]);
        receiver_parse_balance(receivers[i]);
//        receiver_get_notify(receivers[i]);
        printf("当前寝室电费余额: %.02f度\n", dorm_balance(receiver_dorm(receivers[i])));
    }

    receiver_deinit(cck);
    receiver_deinit(ljh);

    return 0;
}
