#include "BlockChain.h"
#include <string>
BlockChain BlockChainInit(){
    BlockChain blockChain{};
    blockChain.head = nullptr;
    blockChain.size = 0;
    return blockChain;
}

void BlockChainDestroy(BlockChain& blockChain) {
    const BlockLink* current=blockChain.head;
    while (current!=nullptr) {
        const BlockLink* next = current->next;
        delete current;
        current=next;
    }
    blockChain.head=nullptr;
    blockChain.size=-1;
}

int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}



int BlockChainPersonalBalance(const BlockChain& blockChain, const string& name) {
    int sum = 0;
    const BlockLink* current = blockChain.head;
    while (current!=nullptr) {
        if (current->block.transaction.receiver==name)
            sum += current->block.transaction.value;
        else if (current->block.transaction.sender==name)
            sum -= current->block.transaction.value;
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

    Transaction transaction = Transaction({value, sender, receiver});
    const Block newBlock = {transaction, timestamp};
    BlockLink* newHeadLink = new BlockLink {newBlock, blockChain.head};

    blockChain.head = newHeadLink;
    blockChain.size += 1;
    
}

void BlockChainAppendTransaction(
        BlockChain& blockChain,
        const Transaction& transaction,
        const string& timestamp
){

    Block newBlock = {transaction, timestamp};
    BlockLink* newHeadLink = new BlockLink {newBlock, blockChain.head};

    blockChain.head = newHeadLink;
    blockChain.size += 1;

}

void BlockChainDump(const BlockChain& blockChain, ofstream& file)
{
    file << "BlockChain info:\n" << std::endl;
    const BlockLink* block_link =blockChain.head;
    int i = 0;
    while (block_link!=nullptr) {
        file << i << "." << std::endl;
        TransactionDumpInfo(block_link->block.transaction, file);
        block_link=block_link->next;
        i++;
    }

}

bool BlockChainVerifyFile(const BlockChain& blockChain, std::ifstream& file) {
    const BlockLink* current_link = blockChain.head;
    string current_hash;
    while (!file.eof()) {
        getline(file,current_hash);
        bool currentLinkVerified = TransactionVerifyHashedMessage(current_link->block.transaction,current_hash );
        if (!currentLinkVerified) {return false;}
        current_link = current_link->next;
    }
    return true;
}

void BlockChainTransform(BlockChain& blockChain, updateFunction function) {
    BlockChain newChain = BlockChainInit();
    BlockLink* current_old_link = blockChain.head;

    while (current_old_link != nullptr) {
        const Transaction& old_transaction = current_old_link->block.transaction;
        unsigned int new_value = function(old_transaction.value);

        const Transaction new_transaction = {
            new_value,
            old_transaction.sender,
            old_transaction.receiver
        };
        const std::string& timestamp = current_old_link->block.timestamp;
        BlockChainAppendTransaction(newChain, new_transaction, timestamp);


        current_old_link = current_old_link->next;
    }
    BlockChainDestroy(blockChain);
    blockChain.head = newChain.head;
    blockChain.size = newChain.size;

}








