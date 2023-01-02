#include "nodes/Node.h"
#include "Log.h"

#define WEBSOCKET_URL "ws://localhost:8126/foo"

Node::Node(const std::string& name, int id)
    :   MsgHandler(id), 
        m_Running(true), 
        m_Transaction({name, 0}), 
        m_WsClient(WEBSOCKET_URL)
{
    try 
    {
        // Start the file logger
        m_Logger = spdlog::basic_logger_mt(name, "../../logs/node_" + name + ".log");
    } 
    catch (const spdlog::spdlog_ex& ex) {
        LOG_ERROR("Log initialization failed: {}", ex.what());
    }
    StartConnectionHandling();
}

Node::~Node()
{
    LOG_WARN("Deleting node {}", m_Id);
}

void Node::Start()
{
    m_MainThread = std::async(std::launch::async, &Node::Run, this);
}

void Node::Shutdown()
{
    m_Running = false;
}

void Node::Wait() const
{
    m_MainThread.wait();
}

/**
 * @brief Executes a transaction on the node
 * 
 * @param transaction Transaction parameters to execute
 */
void Node::ExecuteTransaction(TransactionData transaction)
{
    LOG_INFO("Executing transaction {}", (char)transaction.type);
    switch (transaction.type)
    {
    case TransactionType::SUM:
        m_Transaction.Add(transaction.value);
        break;
    case TransactionType::SUBSTRACT:
        m_Transaction.Substract(transaction.value);
        break;
    case TransactionType::MULTIPLY:
        m_Transaction.Multiply(transaction.value);
        break;
    case TransactionType::SET:
        m_Transaction.SetVersion(transaction.value);
        break;
    case TransactionType::READ:
        m_Transaction.Show();
        break;
    default:
        break;
    }
    
    // If write transaction, send current version to web socket server
    if (transaction.type != TransactionType::READ)
    {
        std::stringstream ss;   // Used to parse dat
        ss << m_Transaction.GetVersion();
        m_WsClient.Send(m_Transaction.GetName() + ": " + ss.str());
    }
    m_Logger->info("Transaction {} executed, argument{}. Current version: {}", (char)transaction.type, transaction.value, m_Transaction.GetVersion());
}