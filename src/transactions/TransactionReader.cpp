#include "transactions/TransactionReader.h"

static int parseLayer(const std::string& strTrans);
static TransactionData parseTransaction(const std::string& strTrans);
static bool isReadOnly(const std::string& strTrans);

Transactions TransactionReader::ReadTransactions(const std::string &filename)
{
    Transactions transactions;
    m_File.open(filename, std::fstream::in);
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
        m_File.close();
    }else {
        LOG_ERROR("Could not open file, {}", filename);
    }
    return transactions;
}

int parseLayer(const std::string& line)
{
    if (!line.empty())
    {
        int layer;
        std::stringstream ss;
        std::string substr = line.substr(line.find("b")+1, line.find(","));
        ss << substr;
        if (ss >> layer && layer > 0 && layer < 3) return layer; 
    }
    return 0;
}

bool isReadOnly(const std::string &strTrans)
{
    if (!strTrans.empty())
    {
        int layer;
        std::stringstream ss;
        std::string substr = strTrans.substr(strTrans.find("b")+1, strTrans.find(","));
        ss << substr;
        ss >> layer;
        if (!ss.fail()
            && layer >= 0 
            && layer < 3) return true; 
    }
    return false;
}

TransactionData parseTransaction(const std::string &strTrans)
{
    TransactionData data = {TransactionType::UNKOWN, 0};
    /* Read transaction */
    if (strTrans.find("r") != std::string::npos)    
    {
        data.type = TransactionType::READ;
        size_t start = strTrans.find("(");
        size_t finish = strTrans.find(")");
        if (start != std::string::npos && finish != std::string::npos)
        {
            std::stringstream ss;
            ss << strTrans.substr(start, finish-start);
            ss >> data.value;
        }
    }  
    /* Write transaction */
    else if (strTrans.find("w") != std::string::npos) {
        size_t start = strTrans.find("(");
        size_t mid = strTrans.find(",");
        size_t finish = strTrans.find(")");
        if (start != std::string::npos && mid != std::string::npos && finish != std::string::npos)
        {
            std::string type = strTrans.substr(start+1, mid-start);
            LOG_INFO("Parsed transaction type string {}", type);
            if (type.find("sum") != std::string::npos)              data.type = TransactionType::SUM;        
            else if (type.find("subs") != std::string::npos)        data.type = TransactionType::SUBSTRACT;
            else if (type.find("mult") != std::string::npos)        data.type = TransactionType::MULTIPLY;

            std::stringstream ss;
            ss << strTrans.substr(mid+1, finish-mid);
            ss >> data.value;
        }
    }
    return data;
}

