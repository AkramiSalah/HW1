#include "Transaction.h"
#include "Utilities.h"
#include <iostream>


void TransactionDumpInfo(const Transaction& transaction, ofstream& file)
{
    std::cout << "Sender Name: " << transaction.sender << std::endl;
    std::cout << "Receiver Name: " << transaction.receiver << std::endl;
    std::cout << "Transaction Value: " << transaction.value << std::endl;
}

string TransactionHashedMessage(const Transaction& transaction)
{
    return hash(transaction.value, transaction.sender, transaction.receiver);
    std::cout << "Sender Name: " << transaction.sender << endl;
    std::cout << "Receiver Name: " << transaction.receiver << endl;
    std::cout << "Transaction Value: " << transaction.value << endl;
}

bool TransactionVerifyHashedMessage(const Transaction& transaction,string hashedMessage)
{
    return TransactionHashedMessage(transaction)==hashedMessage;
}