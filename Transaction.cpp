#include "Transaction.h"
#include "Utilities.h"
#include <iostream>

using std::cout;
using std::endl;

//gets a transaction and a file and puts the transaction info into the file, returns nothing
void TransactionDumpInfo(const Transaction& transaction, ofstream& file)
{
    file << "Sender Name: " << transaction.sender << endl;
    file << "Receiver Name: " << transaction.receiver << endl;
    file << "Transaction Value: " << transaction.value << endl;
}

//gets a transaction and returning the hashed message of it
string TransactionHashedMessage(const Transaction& transaction)
{
    return hash(transaction.value, transaction.sender, transaction.receiver);

}

//compares between a given hashed message and a transaction, returns true if the hashed message is the hashed message of the transaction and false otherwise
bool TransactionVerifyHashedMessage(const Transaction& transaction,string hashedMessage)
{
    return TransactionHashedMessage(transaction) == hashedMessage;
}