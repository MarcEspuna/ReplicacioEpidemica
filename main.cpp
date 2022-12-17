#include "Commons.h"
#include "Log.h"
#include "CoreNode.h"
#include "LayerOneNode.h"
#include "FileWatcher.h"

#define TRANSACTION_DIRECTORY           "../../ClientTransactions"


int main(int argc, char** argv) {
    /* Initializations */
    Log::StartLogging();
    Log::CreateLogger("CORE");
    Socket::Init();
    {
        efsw::FileWatcher fileWatcher;
        FileWatcher listener([&](const std::string& filename)
        {
            LOG_TRACE("File modified log. From listener.");
            /**
            * @TODO read file and process transactions
            * 
            */
        });
        efsw::WatchID watchId = fileWatcher.addWatch(TRANSACTION_DIRECTORY, &listener, false);
        fileWatcher.watch();

        /* Node constructors */
        CoreNode A1(8888);
        CoreNode A2(8889);
        CoreNode A3(8890);
        
        /* Node connections */
        A1.Connect({8889, 8890});
        A2.Connect({8890});
        A3.Connect();

        /* Begin execution of nodes */
        A1.Start();
        A2.Start();
        A3.Start();

        std::cin.get();

        fileWatcher.removeWatch(watchId);

        A1.Shutdown();
        A2.Shutdown();
        A3.Shutdown();
        A1.Wait();
        A2.Wait();
        A3.Wait();
    }
    /* Finalization */
    Socket::Finit();
    Log::EndLogging();
    return 0;
}
