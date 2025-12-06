#include "BlockChain.h"
#include <iostream>
#include <string>

//creats an empty blockchain, it does not get anything and returns the empty blockchain
BlockChain BlockChainInit(){
    BlockChain blockChain{};
    blockChain.head = nullptr;
    blockChain.size = 0;
    return blockChain;
}

//gets a blockchain and destroys it, it returns nothing
void BlockChainDestroy(BlockChain& blockChain) {
    BlockLink* currentLink = blockChain.head;
    while (currentLink != nullptr) {
        BlockLink* next = currentLink->next;
        delete currentLink;
        currentLink = next;
    }
    blockChain.head = nullptr;
    blockChain.size = -1;
}

//gets a blockchain and returns it's size
int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}

//gets a blockchain and a persons name and returns the personal balance of the person
int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    int sum = 0;
    const BlockLink* currentLink = blockChain.head;
    while (currentLink != nullptr) {
        if (currentLink->transaction.receiver == name)
            sum += currentLink->transaction.value;
        else if (currentLink->transaction.sender == name)
            sum -= currentLink->transaction.value;
        currentLink = currentLink->next;
    }
    return sum;
}

//adds a transaction to a given blockchain with given value,sender,receiver and timestamp and returns nothing
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
){
    BlockLink* newHeadLink = new BlockLink{
        Transaction{value, sender, receiver}, 
        timestamp, 
        blockChain.head
    };

    blockChain.head = newHeadLink;
    blockChain.size += 1;
}

//adds a transaction to a given blockchain with given transaction and returns nothing
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
){
    BlockChainAppendTransaction( 
        blockChain,
        transaction.value,
        transaction.sender,
        transaction.receiver,
        timestamp
    );
}

//reads content from given string and index and returns the string from said index until you get a space
std::string ReadUntilSpace(const string& line, unsigned int& index){
    string reader = "";
    while (index < line.length()) { 
        reader += line[index];
        index++;
        if(line[index] == ' '){
            index++;
            return reader;
        }
    }
    return reader;
}

//reads blockchain content from given file and returning the blockchain
BlockChain BlockChainLoad(ifstream& file) {
    BlockChain blockChain = BlockChainInit();
    string line;
    unsigned int index = 0;
    string sender, receiver, timestamp;
    unsigned int value;
    while (getline(file, line)) {
        index = 0;
        sender = ReadUntilSpace(line,index);
        receiver = ReadUntilSpace(line,index);
        value = std::stoi(ReadUntilSpace(line,index));
        timestamp = ReadUntilSpace(line,index);
        BlockChainAppendTransaction(
            blockChain, value, sender, receiver, timestamp);
    }

    BlockChain blockChainReversed = BlockChainInit();
    const BlockLink* currentLink = blockChain.head;
    while (currentLink != nullptr) {
        BlockChainAppendTransaction(blockChainReversed, currentLink->transaction, currentLink->timestamp);
        currentLink = currentLink->next;
    }

    BlockChainDestroy(blockChain);

    return blockChainReversed;
}

//gets a blockchain and a file and prints all the blockchain content into the file, returns nothing
void BlockChainDump(const BlockChain& blockChain, ofstream& file){
    file << "BlockChain Info:" << std::endl;
    const BlockLink* blockLink = blockChain.head;
    int i = 1;
    while (blockLink != nullptr) {
        file << i << "." << std::endl;
        TransactionDumpInfo(blockLink->transaction, file);
        file << "Transaction timestamp: " << blockLink->timestamp << std::endl;
        blockLink = blockLink->next;
        i++;
    }
}

//gets a blockchain and a file and prints the hashed messages of each transaction (in order) in the blockchain, returns nothing
void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file) {
    const BlockLink* blockLink = blockChain.head;
    while (blockLink->next != nullptr) {
        file << TransactionHashedMessage(blockLink->transaction) << std::endl;
        blockLink = blockLink->next;
    }
    file << TransactionHashedMessage(blockLink->transaction);
}

//verifies if given file with transactional hashed messages and a blockchain have the same hashed messages in order, returns true if it is and false otherwise
bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file) {
    BlockLink* currentLink = blockChain.head;
    string current_hash;
    int hash_count = 0;
    while (getline(file, current_hash)) {
        if(currentLink == nullptr || hash_count > BlockChainGetSize(blockChain)) return false;

        bool currentLinkVerified = TransactionVerifyHashedMessage(
            currentLink->transaction, current_hash);
        if (!currentLinkVerified) {return false;}

        currentLink = currentLink->next;
        hash_count++;
    }
    return hash_count == BlockChainGetSize(blockChain);
}

//compresses blocks of given blockchain that are next to each other and have the same sender and receiver into one block, returns nothing
void BlockChainCompress(BlockChain& blockChain) {
    BlockLink* currentLink = blockChain.head;
    BlockLink* nextLink = currentLink->next;
    while (nextLink != nullptr) {
        if (nextLink->transaction.receiver == currentLink->transaction.receiver &&
            nextLink->transaction.sender == currentLink->transaction.sender) {
            currentLink->transaction.value += nextLink->transaction.value;
            currentLink->next = nextLink->next;
            
            delete nextLink;
            blockChain.size--;
            
            nextLink = currentLink->next;
            }
        else {
            currentLink = nextLink;
            nextLink = currentLink->next;
        }
    }
}

//gets a blockchain and a function and changes the transaction values in the blockchain using the functions, returns nothing
void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
    BlockLink* currentLink = blockChain.head;
    while (currentLink != nullptr) {
        Transaction& current_transaction = currentLink->transaction;
        current_transaction.value = function(current_transaction.value);
        currentLink = currentLink->next;
    }
}