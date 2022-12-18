#include "TransactionReader.h"

static int parseLayer(const std::string& strTrans);
static TransactionData parseTransaction(const std::string& strTrans);
static bool isReadOnly(const std::string& strTrans);

TransactionReader::TransactionReader()
{

}

TransactionReader::~TransactionReader()
{
}

// b<f>, r(30), r(39), r(77) 
Transactions TransactionReader::ReadTranactions(const std::string &filename)
{
    Transactions transactions;
    m_File.open(filename, std::fstream::in | std::fstream::out);
    if (m_File.is_open())
    {
        std::string strTransaction;
        m_File >> strTransaction;
        transactions.layer = parseLayer(strTransaction);
        transactions.readOnly = isReadOnly(strTransaction);
        std::cout << strTransaction << std::endl;
        while(!m_File.eof())
        {   
            m_File >> strTransaction;
            std::cout << strTransaction << std::endl;
            TransactionData data = parseTransaction(strTransaction);
            if (data.type != TransactionType::UNKOWN){
                transactions.transactions.push_back(data);
            }
        }
        m_File.clear();
        m_File.close();
    }else {
        LOG_ERROR("Could not open file, {}", filename);
    }
    return transactions;
}

int parseLayer(const std::string& line)
{
    int layer;
    std::stringstream ss;
    std::string substr = line.substr(line.find("b"), line.back());
    ss << substr;
    if (ss >> layer && layer > 0 && layer < 3) return layer; 
    return 0;
}

TransactionData parseTransaction(const std::string &strTrans)
{
    TransactionData data = {TransactionType::UNKOWN, 0};
    if (strTrans.find("r") != std::string::npos)    // Read transaction
    {
        data.type = TransactionType::READ;
        size_t start = strTrans.find("(");
        size_t finish = strTrans.find(")");
        if (start != std::string::npos && finish != std::string::npos)
        {
            std::stringstream ss;
            ss << strTrans.substr(start, finish);
            ss >> data.value;
        }
    }  
    else if (strTrans.find("w") != std::string::npos) {
        LOG_WARN("Write found. Currently unimplemented.");
    }
    return data;
}

bool isReadOnly(const std::string &strTrans)
{
    int layer;
    std::stringstream ss;
    std::string substr = strTrans.substr(strTrans.find("b"), strTrans.back());
    ss << substr;
    if (ss >> layer && layer > 0 && layer < 3) return true; 
    return false;
}
