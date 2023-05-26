#include "receiver.h"
#include "localize.h"
#include "terminal_cli.h"

int main() {
    TerminalCLI *cli = terminalCLI_init();
    terminalCLI_resume(cli);

    // USER cck
    Receiver *cck = receiver_init(u8"陈驰坤", "2020316101023");

    // USER yzj
    Receiver *ljh = receiver_init(u8"李佳禾", "2020316101061");
/*
    // USER lsf
    Receiver* lsf = receiver_init(u8"楼胜峰", "2022331200164");

    // USER lzh
    Receiver* lzh = receiver_init(u8"林展航", "2020316101012");

    // USER yzj
    Receiver* yzj = receiver_init(u8"杨正杰", "2020316101015");

    // USER zbh
    Receiver* zbh = receiver_init(u8"张碧涵", "2020316101022");
*/
    Receiver *receivers[] = {
            cck, ljh
    };
    int receivers_length = sizeof(receivers) / sizeof(receivers[0]);

    for (int i = 0; i < receivers_length; ++i) {
        receiver_notify(receivers[i], print);
    }

    // free up the memory
    receiver_deinit(cck);
    receiver_deinit(ljh);

    terminalCLI_deinit(cli);
    return 0;
}
