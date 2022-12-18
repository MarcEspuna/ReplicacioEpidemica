#pragma once
#include "Commons.h"

/**
 * @brief 
 * We will have various write operations. 
 * 
 */
enum class TransactionType {
    SUM, SUBSTRACT, MULTIPLY, READ, UNKOWN
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

    void Add(int value);
    void Substract(int value);
    void Multiply(int value);
    void Show();
    
private:
    Version m_Version;

};


