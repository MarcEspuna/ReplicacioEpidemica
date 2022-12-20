#include "transactions/Transaction.h"
#include "Log.h"

std::mutex Transaction::m_ShowMutex;

Transaction::Transaction(const Version &version)
    : m_Version(version)
{
}

void Transaction::Add(int value)
{
    LOG_TRACE("Adding {} with {}", m_Version.version, value);
    m_Version.version += value;
}

void Transaction::Substract(int value)
{
    LOG_TRACE("Substracting {} from {}", m_Version.version, value);
    m_Version.version -= value;
}

void Transaction::Multiply(int value)
{
    LOG_TRACE("Multiplying {} by {}", m_Version.version, value);
    m_Version.version *= value;
}

void Transaction::Show()
{   
    std::lock_guard<std::mutex> lock(m_ShowMutex);
    std::cout << "\t**** Reading ****\n";
    std::cout << "\t\tName: " << m_Version.name << std::endl;
    std::cout << "\t\tValue: " << m_Version.version << std::endl;
}
