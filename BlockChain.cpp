#include "BlockChain.h"
#include <string>
BlockChain BlockChainInit(){
    BlockChain blockChain;
    blockChain.head = nullptr;
    blockChain.tail = nullptr;
    blockChain.size = 0;
    return blockChain;
}

int BlockChainGetSize(const BlockChain& blockChain){
    return blockChain.size;
}
void BlockChainDestroy(BlockChain& blockChain) {
    const BlockLink* current=blockChain.head;
    while (current!=nullptr) {
        const BlockLink* next=blockChain.head->next;
        delete current;
        current=next;
    }
    blockChain.tail=nullptr;
    blockChain.head=nullptr;
    blockChain.size=-1;
}
void BlockChainAppendTransaction(
        BlockChain& blockChain,
        unsigned int value,
        const string& sender,
        const string& receiver,
        const string& timestamp
){
    const Transaction* transaction = new Transaction({value, sender, receiver});
    const Block newBlock = {transaction, timestamp};
    BlockLink* newHeadLink = new BlockLink(newBlock, blockChain.head);

    if(BlockChainGetSize(blockChain) == 0) blockChain.tail = newHeadLink;

    blockChain.head = newHeadLink;
    blockChain.size += 1;
    
}
