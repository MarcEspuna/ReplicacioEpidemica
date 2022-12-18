#include "Controller.h"



Controller::Controller()
    : A1("A1", 8888), A2("A2", 8889), A3("A3", 8890), 
    m_FileListener(std::bind(&Controller::FileModifiedCallback, this, std::placeholders::_1, std::placeholders::_2)),
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
    LOG_INFO("Waiting for nodes to finish");

    A1.Wait();
    A2.Wait();
    A3.Wait();
    LOG_INFO("All nodes finished.\n");
}

void Controller::Run()
{
    /* Start watching files */
    m_FileWatcher.watch();

    /* Node connections */
    A1.Connect({8889, 8890});
    A2.Connect({8890});
    A3.Connect();

    /* Begin execution of nodes */
    A1.Start();
    A2.Start();
    A3.Start();

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
        LOG_WARN("Transaction read only to be implemented.");
    }

    if (node)
    {
        for (const auto& trans : transactions.transactions)
            node->ExecuteTransaction(trans);
    }
}
