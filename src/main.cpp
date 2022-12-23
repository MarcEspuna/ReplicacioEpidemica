#include "Controller.h"
#include "web_socket\WsClient.h"

#define TRANSACTION_DIRECTORY           "../../ClientTransactions"


int main(int argc, char** argv) {
    // Static initializations
    Log::StartLogging();
    Log::CreateLogger("CORE");
    Socket::Init();

    {
        Controller controller;
        controller.Run();
    }

    // Static finalizations
    Socket::Finit();
    Log::EndLogging();
    return 0;
}
