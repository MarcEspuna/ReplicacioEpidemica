#include "nodes/Node.h"

Node::Node(const std::string& name, int id)
    : MsgHandler(id), m_Running(true), m_Transaction({name, 0})
{
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

void Node::Wait()
{
    m_MainThread.wait();
}

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
    case TransactionType::READ:
        m_Transaction.Show();
        break;
    default:
        break;
    }
}