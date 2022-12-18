#pragma once
#include "Commons.h"
#include "Transaction.h"
#include "Log.h"


struct TransactionData {
    TransactionType type;
    int value;
};

struct Transactions {
    int layer;
    bool readOnly;
    std::vector<TransactionData> transactions;
};

class TransactionReader
{
public:
    TransactionReader();
    ~TransactionReader();

    Transactions ReadTranactions(const std::string& filename);

private:
    std::fstream m_File;
};
