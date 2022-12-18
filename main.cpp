#include "Commons.h"
#include "Log.h"
#include "CoreNode.h"
#include "LayerOneNode.h"
#include "LayerTwoNode.h"
#include "FileWatcher.h"
#include "TransactionReader.h"

#define TRANSACTION_DIRECTORY           "../../ClientTransactions"


int main(int argc, char** argv) {
    /* Initializations */
    Log::StartLogging();
    Log::CreateLogger("CORE");
    Socket::Init();
    {

        /* Node constructors */
        CoreNode A1("A1", 8888);
        CoreNode A2("A2", 8889);
        CoreNode A3("A3", 8890);

        /* Node connections */
        A1.Connect({8889, 8890});
        A2.Connect({8890});
        A3.Connect();

        /* Begin execution of nodes */
        A1.Start();
        A2.Start();
        A3.Start();

        /* Start file watcher */
        efsw::FileWatcher fileWatcher;
        FileWatcher listener([&](const std::string& filename, const std::string dir)
        {
            Node* node = nullptr;
            std::string path(dir + filename);
            LOG_INFO("File modified. Filepath {}", path);

            TransactionReader reader;
            Transactions transactions = reader.ReadTranactions(path);
            // IF NOT READ ONLY
            if (filename.find("A1") != std::string::npos)                // Transaction to A1
            {
                LOG_INFO("Transaction to A1");
                node = &A1;
            } else if (filename.find("A2") != std::string::npos)        // Transaction to A2
            {       
                LOG_INFO("Transaction to A2");
                node = &A2;
            } else if (filename.find("A3") != std::string::npos)        // Transaction to A3
            {
                node = &A3;
                LOG_INFO("Transaction to A3");
            }

            if (node)
            {
                for (const auto& trans : transactions.transactions)
                    node->ExecuteTransaction(trans);
            }
        });
        
        efsw::WatchID watchId = fileWatcher.addWatch(TRANSACTION_DIRECTORY, &listener, false);
        fileWatcher.watch();

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
