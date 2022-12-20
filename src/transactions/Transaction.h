#pragma once
#include "Commons.h"

/**
 * @brief 
 * We will have various write operations. 
 * 
 */
enum class TransactionType {
    SUM = '+', SUBSTRACT = '-', MULTIPLY = '*', READ = 'r', SET='s', UNKOWN = '5'
};

struct Version {
    std::string name;
    int version;
};

class Transaction {
public:
    Transaction() = delete;
    Transaction(const Version& version);
    ~Transaction() = default;

    // Transaction operations
    void Add(int value);
    void Substract(int value);
    void Multiply(int value);
    void Show();

    // Setters
    void SetVersion(int value) { m_Version.version = value; }

    // Getters
    int GetVersion() { return m_Version.version; }

private:
    Version m_Version;
    static std::mutex m_ShowMutex;      // Used to avoid multiple threads printing at the same time
};


