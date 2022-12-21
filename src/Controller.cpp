#include "Controller.h"

#define NODE_A1        0
#define NODE_A2        1
#define NODE_A3        2
#define NODE_B1        0
#define NODE_B2        1
#define NODE_C1        0
#define NODE_C2        1

template<typename T, size_t S>
static void ExecuteTransactionsOnLayer(std::array<T, S>& nodes, const std::vector<TransactionData>& transactions);

Controller::Controller()
    :  m_CoreNodes{{{"A1", 8888}, {"A2", 8889},{"A3", 8890}}},                             // Core layer nodes
       m_LayerOneNodes{{{"B1",8891}, {"B2", 8892}}},                                       // Layer 1 nodes                                     
       m_LayerTwoNodes{{{"C1",8893}, {"C2",8894}}},                                        // Layer 2 nodes
       m_FileListener(BIND_FILE_HANDLER(Controller::FileModifiedCallback)),                // File system listener
       m_WatchId(m_FileWatcher.addWatch(TRANSACTION_DIRECTORY, &m_FileListener, false))    // File system watcher
{}

Controller::~Controller()
{
    LOG_INFO("Removing watch");

    // Remove filesystem watcher
    m_FileWatcher.removeWatch(m_WatchId);

    LOG_INFO("Shutting down nodes");
    
    // Shutdown nodes
    for (auto &node : m_CoreNodes)              node.Shutdown();
    for (auto &node : m_LayerOneNodes)          node.Shutdown();
    for (auto &node : m_LayerTwoNodes)          node.Shutdown();

    LOG_INFO("Waiting for nodes to finish");

    // Wait for nodes to finish
    for (const auto &node : m_CoreNodes)        node.Wait();
    for (const auto &node : m_LayerOneNodes)    node.Wait();
    for (const auto &node : m_LayerTwoNodes)    node.Wait();

    LOG_INFO("All nodes finished.\n");
}

void Controller::Run()
{
    // Start watching files
    m_FileWatcher.watch();

    // Node connections
    m_CoreNodes[NODE_A1].Connect({8889, 8890});
    m_CoreNodes[NODE_A2].Connect({8890}, {8891});
    m_CoreNodes[NODE_A3].Connect({},{8892});
    m_LayerOneNodes[NODE_B2].Connect({}, {},{8893, 8894});

    /* Begin execution of nodes */
    for (auto &node : m_CoreNodes)              node.Start();
    for (auto &node : m_LayerOneNodes)          node.Start();
    for (auto &node : m_LayerTwoNodes)          node.Start();

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
        if (filename.find("A1") != std::string::npos)                   // Transaction to A1
        {
            LOG_INFO("Transaction to A1");
            node = &m_CoreNodes[NODE_A1];
        } else if (filename.find("A2") != std::string::npos)            // Transaction to A2
        {       
            LOG_INFO("Transaction to A2");
            node = &m_CoreNodes[NODE_A2];
        } else if (filename.find("A3") != std::string::npos)            // Transaction to A3
        {
            node = &m_CoreNodes[NODE_A3];
            LOG_INFO("Transaction to A3");
        }
        if (node)
        {
            for (const auto& trans : transactions.transactions)
                node->ExecuteTransaction(trans);
        }
    }else 
    {   // Execute the read only transactions on all the nodes of the layer
        switch (transactions.layer)
        {
        case CORE_LAYER:
            ExecuteTransactionsOnLayer(m_CoreNodes, transactions.transactions);
            break;
        case LAYER_ONE:
            ExecuteTransactionsOnLayer(m_LayerOneNodes, transactions.transactions);
            break;
        case LAYER_TWO:
            ExecuteTransactionsOnLayer(m_LayerTwoNodes, transactions.transactions);
            break;
        default:
            LOG_CRITICAL("Invalid layer");
            break;
        }

    }
}


template<typename T, size_t S>
static void ExecuteTransactionsOnLayer(std::array<T, S>& nodes, const std::vector<TransactionData>& transactions)
{
    for (const auto& transaction : transactions)
    {
        for (auto& node : nodes)  
            node.ExecuteTransaction(transaction);
    }
}