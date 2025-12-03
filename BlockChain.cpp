#include "BlockChain.h"
#include <iostream>
#include <string>

BlockChain BlockChainInit(){
    BlockChain blockChain{};
    blockChain.head = nullptr;
    blockChain.size = 0;
    return blockChain;
}

void BlockChainDestroy(BlockChain& blockChain) {
    BlockLink* current = blockChain.head;
    while (current != nullptr) {
        BlockLink* next = current->next;
        delete current;
        current = next;
    }
    blockChain.head = nullptr;
    blockChain.size = -1;
}

int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}

int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    int sum = 0;
    const BlockLink* current = blockChain.head;
    while (current != nullptr) {
        if (current->transaction.receiver == name)
            sum += current->transaction.value;
        else if (current->transaction.sender == name)
            sum -= current->transaction.value;
        current = current->next;
    }
    return sum;
}

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

std::string ContentReader(const string& line, unsigned int& index){
    string reader = "";
    while (index < line.length() && line[index] != ' ') { 
        reader += line[index];
        index++;
    }
    if (index < line.length()) { 
        index += 1; 
    }
    return reader;
}
void BuildBlockChainBackwards(ifstream& file,BlockChain& blockChain) {
    string line;
    unsigned int index=0;
    string sender, receiver, timestamp;
    unsigned int value;
    if (getline(file, line)) {
        BuildBlockChainBackwards(file, blockChain);
        index=0;
        sender = ContentReader(line,index);
        receiver = ContentReader(line,index);
        value = std::stoi(ContentReader(line,index));
        string reader = "";
        while (index < line.length() && line[index] != '\n') {
            reader += line[index];
            index++;
        }
        if (index < line.length()) {
            index += 1;
        }
        timestamp = reader;
        BlockChainAppendTransaction(
            blockChain, value, sender, receiver, timestamp);
    }
}

BlockChain BlockChainLoad(ifstream& file) {
    BlockChain blockChain = BlockChainInit();
    BuildBlockChainBackwards(file,blockChain);
    return blockChain;
}

void BlockChainDump(const BlockChain& blockChain, ofstream& file){
    file << "BlockChain info:" << std::endl;
    const BlockLink* block_link = blockChain.head;
    int i = 1;
    while (block_link != nullptr) {
        file << i << "." << std::endl;
        TransactionDumpInfo(block_link->transaction, file);
        file << "Transaction timestamp: " << block_link->timestamp << std::endl;
        block_link = block_link->next;
        i++;
    }
}

void BlockChainDumpHashed(const BlockChain& blockChain, ofstream& file) {
    const BlockLink* block_link = blockChain.head;
    while (block_link->next != nullptr) {
        file << TransactionHashedMessage(block_link->transaction) << std::endl;
        block_link = block_link->next;
    }
    file << TransactionHashedMessage(block_link->transaction)  ;
}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file) {
    const BlockLink* current_link = blockChain.head;
    string current_hash;
    while (!file.eof()) {
        getline(file, current_hash);
        bool currentLinkVerified = TransactionVerifyHashedMessage(current_link->transaction, current_hash);
        if (!currentLinkVerified) {return false;}
        current_link = current_link->next;
    }
    return true;
}

void BlockChainCompress(BlockChain& blockChain) {
    BlockLink* current_link = blockChain.head;
    BlockLink* next_link = current_link->next;
    while (next_link!=nullptr) {
        if (next_link->transaction.receiver == current_link->transaction.receiver &&
            next_link->transaction.sender == current_link->transaction.sender) {
            current_link->transaction.value += next_link->transaction.value;
            current_link->next = next_link->next;
            delete next_link;
            next_link=current_link->next;
            }
        else {
            current_link = next_link;
            next_link = current_link->next;
        }
    }
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
    BlockLink* current_link = blockChain.head;
    while (current_link != nullptr) {
        Transaction& current_transaction = current_link->transaction;
        current_transaction.value = function(current_transaction.value);
        current_link = current_link->next;
    }
}








