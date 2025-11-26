#include "Transaction.h"
#include <iostream>


void TransactionDumpInfo(const Transaction& transaction, ofstream& file)
{
    std::cout << "Sender Name: " << transaction.sender << endl;
    std::cout << "Receiver Name: " << transaction.receiver << endl;
    std::cout << "Transaction Value: " << transaction.value << endl;
}