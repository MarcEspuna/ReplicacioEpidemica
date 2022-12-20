#include "Controller.h"



Controller::Controller()
    :   A1("A1", 8888), A2("A2", 8889), A3("A3", 8890), 
        B1("B1", 8891), B2("B2", 8892),
        C1("C1", 8893), C2("C2", 8894),
        m_FileListener(BIND_FILE_HANDLER(Controller::FileModifiedCallback)),
        m_WatchId(m_FileWatcher.addWatch(TRANSACTION_DIRECTORY, &m_FileListener, false))
{}

Controller::~Controller()
{
    LOG_INFO("Removing watch");
    m_FileWatcher.removeWatch(m_WatchId);

    LOG_INFO("Shutting down nodes");
    
    A1.Shutdown();
    A2.Shutdown();
    A3.Shutdown();
    B1.Shutdown();
    B2.Shutdown();
    C1.Shutdown();
    C2.Shutdown();
    LOG_INFO("Waiting for nodes to finish");

    A1.Wait();
    A2.Wait();
    A3.Wait();
    B1.Wait();
    B2.Wait();
    C1.Wait();
    C2.Wait();
    LOG_INFO("All nodes finished.\n");
}

void Controller::Run()
{
    /* Start watching files */
    m_FileWatcher.watch();

    /* Node connections */
    A1.Connect({8889, 8890});
    A2.Connect({8890}, {8892}, {8891});
    A3.Connect({},{}, {8892});

    B2.Connect({}, {},{8893, 8894});

    /* Begin execution of nodes */
    A1.Start();
    A2.Start();
    A3.Start();

    B1.Start();
    B2.Start();

    C1.Start();
    C2.Start();

    std::cin.get();
}

void Controller::FileModifiedCallback(const std::string &filename, const std::string &dir)
{
    Node* node = nullptr;
    std::string path(dir + filename);
    LOG_INFO("File modified. Filepath {}", path);
    TransactionReader reader;
    Transactions transactions = reader.ReadTransactions(path);
    if (!transactions.readOnly)
    {
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
    }else 
    {
        // TODO: Read the entire layer
        LOG_WARN("Transaction read only to be implemented.");
    }

    if (node)
    {
        for (const auto& trans : transactions.transactions)
            node->ExecuteTransaction(trans);
    }
}
